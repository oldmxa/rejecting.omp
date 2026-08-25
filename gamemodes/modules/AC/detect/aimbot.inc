#if defined _ac_aim_included
    #endinput
#endif
#define _ac_aim_included

#define AC_SILENT_LOW      (14.0)
#define AC_SILENT_HIGH     (35.0)
#define AC_SILENT_STRIKES  (5)

#define AC_HIT_BASE_ONFOOT (8.0)
#define AC_HIT_BASE_OTHER  (20.0)
#define AC_HIT_DRIFT_FOOT  (12.0)
#define AC_HIT_DRIFT_OTHER (60.0)
#define AC_FAKE_HIT_FRESH  (1500)
#define AC_FAKE_STRIKES    (5)

#define AC_ANTIAIM_LIMIT   (60.0)
#define AC_ANTIAIM_STRIKES (5)
#define AC_ANTIAIM_EVERY   (1000)

static
    Float:ACAimVec[MAX_PLAYERS][3],
    bool:ACAimSet[MAX_PLAYERS],
    ACSilentStrike[MAX_PLAYERS],
    ACFakeStrike[MAX_PLAYERS],
    ACAntiAimStrike[MAX_PLAYERS],
    ACAntiAimTick[MAX_PLAYERS];

static Float:AC_VectorAngle(Float:ax, Float:ay, Float:az, Float:bx, Float:by, Float:bz)
{
    new Float:dot = ax * bx + ay * by + az * bz;

    if (dot > 1.0) dot = 1.0;
    else if (dot < -1.0) dot = -1.0;

    return acos(dot);
}

stock AC_StoreAimVector(playerid, Float:x, Float:y, Float:z)
{
    ACAimVec[playerid][0] = x;
    ACAimVec[playerid][1] = y;
    ACAimVec[playerid][2] = z;
    ACAimSet[playerid] = true;

    AC_CheckAntiAim(playerid);
    return 1;
}

stock AC_ResetAim(playerid)
{
    ACAimSet[playerid] = false;
    ACSilentStrike[playerid] = ACFakeStrike[playerid] = ACAntiAimStrike[playerid] = 0;
    ACAntiAimTick[playerid] = 0;
    return 1;
}

static AC_CheckAntiAim(playerid)
{
    new tick = GetTickCount();

    if (tick - ACAntiAimTick[playerid] < AC_ANTIAIM_EVERY) return 1;

    ACAntiAimTick[playerid] = tick;

    if (!IsPlayerLogged(playerid) || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
    {
        ACAntiAimStrike[playerid] = 0;
        return 1;
    }

    new KEY:keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);

    if (!(keys & KEY_HANDBRAKE))
    {
        ACAntiAimStrike[playerid] = 0;
        return 1;
    }

    new Float:facing, Float:camera = atan2(-ACAimVec[playerid][0], ACAimVec[playerid][1]);
    GetPlayerFacingAngle(playerid, facing);

    new Float:offset = floatabs(facing - camera);

    while (offset > 180.0) offset = 360.0 - offset;

    AC_Debug(playerid, AC_ANTI_AIM, "body %.1f, camera %.1f, offset %.1f (limit %.1f)", facing, camera, offset, AC_ANTIAIM_LIMIT);

    if (offset < AC_ANTIAIM_LIMIT)
    {
        ACAntiAimStrike[playerid] = 0;
        return 1;
    }

    if (++ACAntiAimStrike[playerid] < AC_ANTIAIM_STRIKES) return 1;

    ACAntiAimStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_ANTI_AIM);
    return 1;
}

static AC_FakeHitStrike(playerid, const reason[])
{
    if (++ACFakeStrike[playerid] < AC_FAKE_STRIKES) return 1;

    ACFakeStrike[playerid] = 0;
    AC_Debug(playerid, AC_AIMBOT, "%d forged hits in a row: %s", AC_FAKE_STRIKES, reason);
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_AIMBOT);
    return 1;
}

static bool:AC_IsDegenerateBullet(playerid, Float:ox, Float:oy, Float:oz, Float:hx, Float:hy, Float:hz)
{
    if (hx == 0.0 && hy == 0.0 && hz == 0.0) return true;
    if (ox == hx && oy == hy) return true;

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    if (ox == px && oy == py && oz == pz) return true;

    return false;
}

static AC_CheckFakeHit(playerid, hitid, Float:hx, Float:hy, Float:hz)
{
    if (hitid == INVALID_PLAYER_ID || !IsPlayerConnected(hitid) || hitid == playerid) return 1;
    if (!IsPlayerLogged(hitid)) return 1;

    if (WC_IsPlayerPaused(hitid) || GetPlayerState(hitid) == PLAYER_STATE_WASTED)
    {
        ACFakeStrike[playerid] = 0;
        return 1;
    }

    if (GetTickCount() - AC_LastTeleport(hitid) < GetPlayerPing(playerid) + AC_FAKE_HIT_FRESH)
    {
        ACFakeStrike[playerid] = 0;
        return 1;
    }

    new Float:base = AC_HIT_BASE_OTHER, Float:drift = AC_HIT_DRIFT_OTHER;

    if (GetPlayerState(hitid) == PLAYER_STATE_ONFOOT
     && GetPlayerSurfingObjectID(hitid) == INVALID_OBJECT_ID
     && GetPlayerSurfingVehicleID(hitid) == INVALID_VEHICLE_ID)
    {
        base = AC_HIT_BASE_ONFOOT;
        drift = AC_HIT_DRIFT_FOOT;
    }

    new Float:limit = base + drift * float(GetPlayerPing(playerid) + GetPlayerPing(hitid)) / 1000.0;
    new Float:distance = GetPlayerDistanceFromPoint(hitid, hx, hy, hz);

    AC_Debug(playerid, AC_AIMBOT, "hit on %s[%d] is %.1f from the real target (limit %.1f)",
        User[hitid][userName], hitid, distance, limit);

    if (distance < limit)
    {
        ACFakeStrike[playerid] = 0;
        return 1;
    }

    return AC_FakeHitStrike(playerid, "target far from the impact point");
}

static ACBulletProbe[MAX_PLAYERS];

stock AC_DebugBullet(playerid, hitType)
{
    if (!AC_DebugActive()) return 0;
    if (GetTickCount() - ACBulletProbe[playerid] < 1500) return 0;

    ACBulletProbe[playerid] = GetTickCount();

    return AC_Debug(playerid, AC_LOW_FOV_SILENT, "bullet packet: hitType %d (1=player), aim stored %d",
        hitType, _:ACAimSet[playerid]);
}

stock AC_CheckSilentAim(playerid, BULLET_HIT_TYPE:hitType, hitid, Float:ox, Float:oy, Float:oz, Float:hx, Float:hy, Float:hz)
{
    if (hitType != BULLET_HIT_TYPE_PLAYER) return 1;
    if (!IsPlayerLogged(playerid)) return 1;

    if (AC_IsDegenerateBullet(playerid, ox, oy, oz, hx, hy, hz))
        return AC_FakeHitStrike(playerid, "degenerate bullet packet");

    AC_CheckFakeHit(playerid, hitid, hx, hy, hz);

    if (!ACAimSet[playerid]) return 1;

    new Float:dx = hx - ox, Float:dy = hy - oy, Float:dz = hz - oz;
    new Float:len = floatsqroot(dx * dx + dy * dy + dz * dz);

    if (len < 0.5) return 1;

    dx /= len;
    dy /= len;
    dz /= len;

    new Float:angle = AC_VectorAngle(dx, dy, dz, ACAimVec[playerid][0], ACAimVec[playerid][1], ACAimVec[playerid][2]);

    if (angle != angle) return 1;

    AC_Debug(playerid, AC_LOW_FOV_SILENT, "aim deviation %.1f degrees (limit %.1f)", angle, AC_SILENT_LOW);

    if (angle < AC_SILENT_LOW)
    {
        ACSilentStrike[playerid] = 0;
        return 1;
    }

    if (++ACSilentStrike[playerid] < AC_SILENT_STRIKES) return 1;

    ACSilentStrike[playerid] = 0;

    CallRemoteFunction("OnRejectingACDetection", "ii", playerid,
        (angle >= AC_SILENT_HIGH) ? AC_HIGH_FOV_SILENT : AC_LOW_FOV_SILENT);

    return 1;
}

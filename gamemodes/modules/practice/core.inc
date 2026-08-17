#if defined _practice_core_incl
    #endinput
#endif
#define _practice_core_incl

forward OnPracticeWaypoint(playerid);
forward OnPracticeRespawn(playerid);
forward PracticeOnSetAfk(playerid);

public PracticeOnSetAfk(playerid)
{
    EndPracticeSession(playerid);
    return 1;
}

stock ResetPracticeState(playerid)
{
    g_IsPlayerInPractice[playerid]     = false;
    g_Practice[playerid][pracActive]   = false;
    g_Practice[playerid][pracModel]    = 0;
    g_Practice[playerid][pracTargetType] = PRACTICE_TYPE_BALL;
    g_Practice[playerid][pracSkin]       = PRACTICE_SKIN_DEFAULT;
    g_Practice[playerid][pracMoveStyle]  = 1;
    g_Practice[playerid][pracNpcFight]   = false;
    g_Practice[playerid][pracNpcWeapon]  = 0;
    g_Practice[playerid][pracNpcId]      = INVALID_NPC_ID;
    g_Practice[playerid][pracRecording]  = false;
    g_Practice[playerid][pracRecPending] = false;
    g_Practice[playerid][pracRecordId]   = -1;
    g_Practice[playerid][pracSpeed]    = PRACTICE_SPEED_DEFAULT;
    g_Practice[playerid][pracHeight]   = PRACTICE_HEIGHT_DEFAULT;
    g_Practice[playerid][pracTolerance]  = PRACTICE_TOLERANCE_DEFAULT;
    g_Practice[playerid][pracLatency]    = PRACTICE_LATENCY_DEFAULT;
    g_Practice[playerid][pracMoving]   = false;
    g_Practice[playerid][pracHasPos1]  = false;
    g_Practice[playerid][pracHasPos2]  = false;
    g_Practice[playerid][pracObject]   = INVALID_STREAMER_ID;
    g_Practice[playerid][pracHealth]   = 0.0;
    g_Practice[playerid][pracWaypoint] = 0;
    g_Practice[playerid][pracMoveTimer] = 0;
    g_Practice[playerid][pracHits]     = 0;
    g_Practice[playerid][pracShots]    = 0;
    g_Practice[playerid][pracShotId]     = 0;
    g_Practice[playerid][pracHitShotId]  = -1;
    return 1;
}

stock DestroyPracticeBall(playerid)
{

    if (g_Practice[playerid][pracMoveTimer])
    {
        KillTimer(g_Practice[playerid][pracMoveTimer]);
        g_Practice[playerid][pracMoveTimer] = 0;
    }

    if (g_Practice[playerid][pracObject] != INVALID_STREAMER_ID)
    {
        if (IsValidDynamicObject(g_Practice[playerid][pracObject]))
            DestroyDynamicObject(g_Practice[playerid][pracObject]);

        g_Practice[playerid][pracObject] = INVALID_STREAMER_ID;
    }
    return 1;
}

stock DestroyPracticeTarget(playerid)
{
    DestroyPracticeBall(playerid);
    DestroyPracticeNPC(playerid);
    return 1;
}

static Float:GetPracticeTargetRadius(model)
{
    new Float:offX, Float:offY, Float:offZ, Float:radius;
    new Float:result = PRACTICE_RADIUS_FALL;

    if (CA_GetModelBoundingSphere(model, offX, offY, offZ, radius) && radius > 0.0)
        result = radius * PRACTICE_RADIUS_SCALE;

    if (result < PRACTICE_RADIUS_MIN) result = PRACTICE_RADIUS_MIN;
    return result;
}

stock GetPracticePoint(playerid, slot, &Float:x, &Float:y, &Float:z)
{
    if (slot == 2)
    {
        x = g_Practice[playerid][pracPos2][0];
        y = g_Practice[playerid][pracPos2][1];
        z = g_Practice[playerid][pracPos2][2];
    }
    else
    {
        x = g_Practice[playerid][pracPos1][0];
        y = g_Practice[playerid][pracPos1][1];
        z = g_Practice[playerid][pracPos1][2];
    }

    z += g_Practice[playerid][pracHeight];
    return 1;
}

stock GetPracticeStartPos(playerid, &Float:x, &Float:y, &Float:z)
{
    if (g_Practice[playerid][pracHasPos1])
        return GetPracticePoint(playerid, 1, x, y, z);

    GetPlayerPos(playerid, x, y, z);

    new Float:angle;
    GetPlayerFacingAngle(playerid, angle);

    x += floatsin(-angle, degrees) * 6.0;
    y += floatcos(-angle, degrees) * 6.0;
    z += g_Practice[playerid][pracHeight];
    return 1;
}

stock bool:PracticeCanMove(playerid)
{
    return (g_Practice[playerid][pracMoving] && g_Practice[playerid][pracHasPos1] && g_Practice[playerid][pracHasPos2]);
}

stock StartPracticeMovement(playerid)
{
    if (!PracticeCanMove(playerid)) return 0;

    new obj = g_Practice[playerid][pracObject];
    if (obj == INVALID_STREAMER_ID || !IsValidDynamicObject(obj)) return 0;

    new dest = g_Practice[playerid][pracWaypoint] ? 0 : 1;
    g_Practice[playerid][pracWaypoint] = dest;

    new Float:speed = g_Practice[playerid][pracSpeed];
    if (speed <= 0.0) speed = PRACTICE_SPEED_DEFAULT;

    new Float:tx, Float:ty, Float:tz;
    GetPracticePoint(playerid, dest ? 2 : 1, tx, ty, tz);
    tz += PRACTICE_BALL_LIFT;

    new Float:cx, Float:cy, Float:cz;
    GetDynamicObjectPos(obj, cx, cy, cz);

    MoveDynamicObject(obj, tx, ty, tz, speed);

    new Float:dist = VectorSize(tx - cx, ty - cy, tz - cz);
    new travelMs = floatround((dist / speed) * 1000.0, floatround_round);

    if (travelMs < 50) travelMs = 50;

    g_Practice[playerid][pracMoveTimer] = SetTimerEx("OnPracticeWaypoint", travelMs, false, "i", playerid);
    return 1;
}

public OnPracticeWaypoint(playerid)
{
    g_Practice[playerid][pracMoveTimer] = 0;

    if (!IsPlayerConnected(playerid) || !g_Practice[playerid][pracActive])
        return 1;

    StartPracticeMovement(playerid);
    return 1;
}

stock CreatePracticeTarget(playerid)
{
    if (g_Practice[playerid][pracTargetType] == PRACTICE_TYPE_NPC)
    {
        DestroyPracticeBall(playerid);
        return EnsurePracticeNPC(playerid);
    }

    DestroyPracticeTarget(playerid);


    new idx = g_Practice[playerid][pracModel];
    if (idx < 0 || idx >= sizeof(g_PracticeModels)) idx = 0;

    new Float:x, Float:y, Float:z;
    GetPracticeStartPos(playerid, x, y, z);
    z += PRACTICE_BALL_LIFT;

    g_Practice[playerid][pracWaypoint] = 0;

    g_Practice[playerid][pracObject] = CreateDynamicObject(
        g_PracticeModels[idx][pmModel],
        x, y, z,
        0.0, 0.0, 0.0,
        PRACTICE_VW_BASE + playerid, 0, playerid, 300.0
    );

    g_Practice[playerid][pracHealth] = PRACTICE_TARGET_HP;

    if (PracticeCanMove(playerid))
        StartPracticeMovement(playerid);

    return 1;
}

stock SetPracticePoint(playerid, slot)
{
    if (IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot set a movement point while inside a vehicle.");
        return 0;
    }

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_SPECTATING)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot set a movement point right now.");
        return 0;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (slot == 1)
    {
        g_Practice[playerid][pracPos1][0] = x;
        g_Practice[playerid][pracPos1][1] = y;
        g_Practice[playerid][pracPos1][2] = z;
        g_Practice[playerid][pracHasPos1] = true;
    }
    else
    {
        g_Practice[playerid][pracPos2][0] = x;
        g_Practice[playerid][pracPos2][1] = y;
        g_Practice[playerid][pracPos2][2] = z;
        g_Practice[playerid][pracHasPos2] = true;
    }

    va_SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Point {MAIN}%d {FFFFFF}set at your position.", slot);

    if (g_Practice[playerid][pracMoving] && !PracticeCanMove(playerid))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Now set point {MAIN}%d {FFFFFF}to start the movement.", (slot == 1) ? 2 : 1);
    }

    CreatePracticeTarget(playerid);
    return 1;
}

stock ClearPracticePoint(playerid, slot)
{
    new bool:wasMoving = PracticeCanMove(playerid);

    if (slot == 1) g_Practice[playerid][pracHasPos1] = false;
    else g_Practice[playerid][pracHasPos2] = false;

    va_SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Point {MAIN}%d {FFFFFF}removed.", slot);

    if (wasMoving)
        SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}The target stopped, it needs both points to move.");

    CreatePracticeTarget(playerid);
    return 1;
}

public OnPracticeRespawn(playerid)
{
    if (!IsPlayerConnected(playerid) || !g_Practice[playerid][pracActive])
        return 1;

    CreatePracticeTarget(playerid);
    return 1;
}

stock bool:IsPlayerInLounge(playerid)
{
    return (User[playerid][isAfk] && User[playerid][locationId] == ARENA_AFK);
}

stock EnterPractice(playerid)
{
    if (g_Practice[playerid][pracActive]) return 0;

    g_IsPlayerInPractice[playerid]   = true;
    g_Practice[playerid][pracActive] = true;
    g_Practice[playerid][pracHits]   = 0;
    g_Practice[playerid][pracShots]  = 0;
    g_Practice[playerid][pracShotId]    = 0;
    g_Practice[playerid][pracHitShotId] = -1;

    SetPlayerVirtualWorld(playerid, PRACTICE_VW_BASE + playerid);

    hidePlayerTextdraw(playerid);
    if (User[playerid][showPlayerHud])
        PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);

    CreatePracticeTarget(playerid);

    SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}You are now in your private practice world. Use {MAIN}/prac exit {FFFFFF}to leave.");
    return 1;
}

stock EndPracticeSession(playerid)
{
    if (!g_Practice[playerid][pracActive]) return 0;

    if (g_Practice[playerid][pracRecording]) StopRecordingPlayerData(playerid);
    g_Practice[playerid][pracRecording] = false;

    PracticeDropRecord(playerid);

    DestroyPracticeTarget(playerid);
    g_Practice[playerid][pracActive] = false;
    g_IsPlayerInPractice[playerid]   = false;

    if (User[playerid][showPlayerHud]) showPlayerTextDraw(playerid);
    else hidePlayerTextdraw(playerid);

    if (GetPlayerVirtualWorld(playerid) == PRACTICE_VW_BASE + playerid)
    {
        new vw = User[playerid][loungeVW];
        SetPlayerVirtualWorld(playerid, vw ? vw : LOUNGEVW);
    }

    return 1;
}

stock ExitPractice(playerid)
{
    if (!EndPracticeSession(playerid)) return 0;

    setAfk(playerid);

    if (User[playerid][showPlayerHud]) showPlayerTextDraw(playerid);
    else hidePlayerTextdraw(playerid);

    return 1;
}

stock PracticeValidateWorld(playerid)
{
    if (!g_Practice[playerid][pracActive]) return 0;

    if (GetPlayerVirtualWorld(playerid) == PRACTICE_VW_BASE + playerid && IsPlayerInLounge(playerid))
        return 1;

    EndPracticeSession(playerid);
    return 0;
}

static Float:GetPracticeWeaponDamage(WEAPON:weaponid)
{
    switch (weaponid)
    {
        case WEAPON_DEAGLE:  return PRACTICE_DMG_DEAGLE;
        case WEAPON_SHOTGUN: return PRACTICE_DMG_SHOTGUN;
        case WEAPON_M4:      return PRACTICE_DMG_M4;
        case WEAPON_SNIPER:  return PRACTICE_DMG_SNIPER;
    }
    return PRACTICE_DMG_DEFAULT;
}

static Float:GetPracticeShotDistance(
    Float:ox, Float:oy, Float:oz,
    Float:ex, Float:ey, Float:ez,
    Float:tx, Float:ty, Float:tz)
{
    new Float:dx = ex - ox, Float:dy = ey - oy, Float:dz = ez - oz;
    new Float:len = VectorSize(dx, dy, dz);

    if (len <= 0.001)
        return VectorSize(tx - ox, ty - oy, tz - oz);

    new Float:t = ((tx - ox) * dx + (ty - oy) * dy + (tz - oz) * dz) / (len * len);

    if (t < 0.0) t = 0.0;
    else if (t > 1.0) t = 1.0;

    new Float:cx = ox + dx * t, Float:cy = oy + dy * t, Float:cz = oz + dz * t;
    return VectorSize(tx - cx, ty - cy, tz - cz);
}

stock HandlePracticeShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (!PracticeValidateWorld(playerid)) return 1;
    if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_BALL) return 1;

    new obj = g_Practice[playerid][pracObject];
    if (obj == INVALID_STREAMER_ID || !IsValidDynamicObject(obj)) return 1;

    new Float:objX, Float:objY, Float:objZ;
    GetDynamicObjectPos(obj, objX, objY, objZ);

    new bool:hit = false;

    if (g_Practice[playerid][pracLatency] <= 0 &&
        (hittype == BULLET_HIT_TYPE_OBJECT || hittype == BULLET_HIT_TYPE_PLAYER_OBJECT))
    {
        if (Streamer_GetItemStreamerID(playerid, STREAMER_TYPE_OBJECT, hitid) == obj)
            hit = true;
    }

    if (!hit)
    {
        new Float:ox, Float:oy, Float:oz, Float:ex, Float:ey, Float:ez;
        GetPlayerLastShotVectors(playerid, ox, oy, oz, ex, ey, ez);

        if (ex == 0.0 && ey == 0.0 && ez == 0.0)
        {
            if (hittype != BULLET_HIT_TYPE_NONE) return 1;

            ex = fX; ey = fY; ez = fZ;
            GetPlayerPos(playerid, ox, oy, oz);
        }

        new idx = g_Practice[playerid][pracModel];
        if (idx < 0 || idx >= sizeof(g_PracticeModels)) idx = 0;

        new Float:radius = GetPracticeTargetRadius(g_PracticeModels[idx][pmModel]);

        if (PracticeCanMove(playerid))
        {
            new Float:destX, Float:destY, Float:destZ;
            GetPracticePoint(playerid, g_Practice[playerid][pracWaypoint] ? 2 : 1, destX, destY, destZ);
            destZ += PRACTICE_BALL_LIFT;

            new Float:dx = destX - objX, Float:dy = destY - objY, Float:dz = destZ - objZ;
            new Float:len = VectorSize(dx, dy, dz);

            if (len > 0.01)
            {
                new Float:shift = 0.0;

                new lag = g_Practice[playerid][pracLatency];
                if (lag > 0)
                {
                    shift = g_Practice[playerid][pracSpeed] * (float(lag) / 1000.0);
                    if (shift > len) shift = len;
                }

                new ping = GetPlayerPing(playerid);
                if (ping > g_Practice[playerid][pracTolerance]) ping = g_Practice[playerid][pracTolerance];

                if (ping > 0)
                    shift -= g_Practice[playerid][pracSpeed] * (float(ping) / 2000.0);

                objX += (dx / len) * shift;
                objY += (dy / len) * shift;
                objZ += (dz / len) * shift;
            }
        }

        if (GetPracticeShotDistance(ox, oy, oz, ex, ey, ez, objX, objY, objZ) <= radius)
            hit = true;
    }

    if (!hit) return 1;

    new Float:dmg = GetPracticeWeaponDamage(weaponid);

    g_Practice[playerid][pracHits]++;
    g_Practice[playerid][pracHealth] -= dmg;

    WC_ShowDamageFeedGiven(playerid, playerid, dmg, weaponid);

    new snd = User[playerid][hitSoundID];
    if (snd == 0) snd = PRACTICE_HIT_SOUND;
    PlayerPlaySound(playerid, snd, 0.0, 0.0, 0.0);

    if (g_Practice[playerid][pracHealth] <= 0.0)
    {
        DestroyPracticeBall(playerid);
        SetTimerEx("OnPracticeRespawn", PRACTICE_RESPAWN_DELAY, false, "i", playerid);
    }
    return 1;
}

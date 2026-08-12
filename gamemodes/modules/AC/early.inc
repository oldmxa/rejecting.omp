#if defined _ac_early_included
    #endinput
#endif
#define _ac_early_included

#include "modules/AC/config.inc"

#define AC_NOP_SETTLE     (1500)
#define AC_NOP_INTERVAL   (1000)
#define AC_NOP_DISTANCE   (3.0)

#define AC_MOVE_GRACE     (2500)

#define AC_RUN_SPEED      (22.0)
#define AC_RUN_STRIKES    (3)
#define AC_WARP_DISTANCE  (60.0)
#define AC_VEHICLE_WARP   (30.0)

#define AC_AIR_HEIGHT     (5.0)
#define AC_AIR_SPEED      (3.0)
#define AC_AIR_FALL       (0.05)
#define AC_AIR_STRIKES    (3)

#define AC_SURF_STRIKES   (4)

#define AC_FALL_HEIGHT    (25.0)
#define AC_FALL_DROP      (1.0)

#define AC_NO_GROUND      (999.0)
#define AC_UNDERGROUND    (4.0)
#define AC_UG_STRIKES     (3)

static
    bool:ACFrozen[MAX_PLAYERS],
    ACFrozenTick[MAX_PLAYERS],
    Float:ACFrozenX[MAX_PLAYERS],
    Float:ACFrozenY[MAX_PLAYERS],

    ACMoveTick[MAX_PLAYERS],
    ACTeleportTick[MAX_PLAYERS],
    Float:ACMoveX[MAX_PLAYERS],
    Float:ACMoveY[MAX_PLAYERS],
    ACRunStrike[MAX_PLAYERS],
    ACAirStrike[MAX_PLAYERS],
    ACUgStrike[MAX_PLAYERS],

    Float:ACMoveZ[MAX_PLAYERS],
    Float:ACFallTop[MAX_PLAYERS],
    bool:ACFalling[MAX_PLAYERS],
    ACSurfStrike[MAX_PLAYERS],
    ACSurfLogged[MAX_PLAYERS];

forward AC_TickMovement();

new bool:ACDebugOn[MAX_PLAYERS];
static ACDebugCount = 0;

stock bool:AC_DebugActive()
{
    return (ACDebugCount > 0);
}

stock AC_SetDebug(playerid, bool:on)
{
    if (ACDebugOn[playerid] == on) return 0;

    ACDebugOn[playerid] = on;
    ACDebugCount += (on) ? (1) : (-1);

    return 1;
}

#define AC_DEBUG_FILE "ac_debug.log"

stock AC_Debug(playerid, code, const fmt[], va_args<>)
{
    if (!AC_DebugActive()) return 0;

    new text[144];
    va_format(text, sizeof(text), fmt, va_start<3>);

    foreach (new i : Player)
    {
        if (!ACDebugOn[i]) continue;

        va_SendClientMessage(i, -1, "{BEBEBE}[RAC] %s[%d] {FFFFFF}#%02d {BEBEBE}%s",
            User[playerid][userName], playerid, code + 1, text);
    }

    new File:log = fopen(AC_DEBUG_FILE, io_append);
    if (log)
    {
        new hour, minute, second, line[220];
        gettime(hour, minute, second);

        format(line, sizeof(line), "%02d:%02d:%02d  %s[%d]  #%02d  %s\r\n",
            hour, minute, second, User[playerid][userName], playerid, code + 1, text);

        fwrite(log, line);
        fclose(log);
    }
    return 1;
}

stock AC_LastTeleport(playerid)
{
    return ACTeleportTick[playerid];
}

static AC_FreezeBaseline(playerid)
{
    new Float:z;
    GetPlayerPos(playerid, ACFrozenX[playerid], ACFrozenY[playerid], z);
    ACFrozenTick[playerid] = GetTickCount();
    return 1;
}

stock AC_TogglePlayerControllable(playerid, toggle)
{
    ACFrozen[playerid] = (toggle == 0);

    if (!toggle) AC_FreezeBaseline(playerid);

    return TogglePlayerControllable(playerid, toggle);
}

stock AC_PutPlayerInVehicle(playerid, vehicleid, seatid)
{
    ACTeleportTick[playerid] = GetTickCount();
    ACRunStrike[playerid] = ACAirStrike[playerid] = 0;

    return PutPlayerInVehicle(playerid, vehicleid, seatid);
}

stock AC_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    if (ACFrozen[playerid])
    {
        ACFrozenX[playerid] = x;
        ACFrozenY[playerid] = y;
        ACFrozenTick[playerid] = GetTickCount();
    }

    ACMoveX[playerid] = x;
    ACMoveY[playerid] = y;
    ACMoveTick[playerid] = ACTeleportTick[playerid] = GetTickCount();
    ACRunStrike[playerid] = ACAirStrike[playerid] = 0;

    return SetPlayerPos(playerid, x, y, z);
}

static Float:AC_HeightAboveGround(Float:x, Float:y, Float:z)
{
    new Float:hx, Float:hy, Float:hz;

    if (!CA_RayCastLine(x, y, z - 0.9, x, y, z - 200.0, hx, hy, hz))
        return 999.0;

    return (z - 0.9) - hz;
}

static AC_CheckSurf(playerid)
{
    new objectid = GetPlayerSurfingObjectID(playerid);

    if (objectid == INVALID_OBJECT_ID)
    {
        ACSurfStrike[playerid] = ACSurfLogged[playerid] = 0;
        return 1;
    }

    new streamerid = Streamer_GetItemStreamerID(playerid, STREAMER_TYPE_OBJECT, objectid);
    new bool:real = (streamerid > 0) ? (Streamer_IsItemVisible(playerid, STREAMER_TYPE_OBJECT, streamerid) != 0) : (IsValidObject(objectid) != 0);

    if (ACSurfLogged[playerid] != objectid)
    {
        ACSurfLogged[playerid] = objectid;
        AC_Debug(playerid, AC_INVISIBLE_SURF, "surfing object %d, streamer id %d, real %d", objectid, streamerid, _:real);
    }

    if (real)
    {
        ACSurfStrike[playerid] = 0;
        return 1;
    }

    if (++ACSurfStrike[playerid] < AC_SURF_STRIKES) return 1;

    ACSurfStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_INVISIBLE_SURF);
    return 1;
}

static AC_CheckFall(playerid, Float:drop)
{
    if (ACNoFallDamage || GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
    {
        ACFalling[playerid] = false;
        ACFallTop[playerid] = 0.0;
        return 1;
    }

    if (drop >= AC_FALL_DROP)
    {
        if (!ACFalling[playerid])
        {
            ACFalling[playerid] = true;
            ACFallTop[playerid] = 0.0;
        }

        ACFallTop[playerid] += drop;
        return 1;
    }

    if (!ACFalling[playerid]) return 1;

    new Float:total = ACFallTop[playerid], Float:health;
    GetPlayerHealth(playerid, health);

    ACFalling[playerid] = false;
    ACFallTop[playerid] = 0.0;

    AC_Debug(playerid, AC_NO_FALL, "landed after %.1f (limit %.1f), health %.0f", total, AC_FALL_HEIGHT, health);

    if (total >= AC_FALL_HEIGHT && health >= 100.0)
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NO_FALL);

    return 1;
}

static AC_MovementTick(playerid)
{
    new tick = GetTickCount();
    new elapsed = tick - ACMoveTick[playerid];

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new Float:dx = x - ACMoveX[playerid], Float:dy = y - ACMoveY[playerid];
    new Float:moved = floatsqroot(dx * dx + dy * dy);

    new Float:drop = ACMoveZ[playerid] - z;

    ACMoveX[playerid] = x;
    ACMoveY[playerid] = y;
    ACMoveZ[playerid] = z;
    ACMoveTick[playerid] = tick;

    if (elapsed < 1) return 1;

    AC_CheckFall(playerid, drop);

    if (tick - ACTeleportTick[playerid] < AC_MOVE_GRACE
     || User[playerid][isAfk]
     || IsPlayerInAnyVehicle(playerid)
     || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT
     || GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK
     || GetPlayerSurfingObjectID(playerid) != INVALID_OBJECT_ID
     || GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID)
    {
        ACRunStrike[playerid] = ACAirStrike[playerid] = 0;
        return 1;
    }

    new Float:speed = moved * 1000.0 / float(elapsed);

    if (speed >= 8.0 || moved >= 20.0)
        AC_Debug(playerid, AC_MAP_RUN, "speed %.1f u/s (limit %.1f), jump %.1f", speed, AC_RUN_SPEED, moved);

    if (moved >= AC_WARP_DISTANCE)
    {
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_CLICK_WARP);
        return 1;
    }

    if (speed >= AC_RUN_SPEED)
    {
        if (++ACRunStrike[playerid] >= AC_RUN_STRIKES)
        {
            ACRunStrike[playerid] = 0;
            CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_MAP_RUN);
        }
    }
    else ACRunStrike[playerid] = 0;

    new Float:vx, Float:vy, Float:vz;
    GetPlayerVelocity(playerid, vx, vy, vz);

    new Float:height = AC_HeightAboveGround(x, y, z);

    new bool:level = (speed >= AC_AIR_SPEED && vz > -AC_AIR_FALL && vz < AC_AIR_FALL);

    if (level)
        AC_Debug(playerid, AC_AIRBREAK, "airborne? height %.1f (limit %.1f), vz %.3f, speed %.1f", height, AC_AIR_HEIGHT, vz, speed);

    if (!level || height < AC_AIR_HEIGHT) ACAirStrike[playerid] = 0;
    else if (++ACAirStrike[playerid] >= AC_AIR_STRIKES)
    {
        ACAirStrike[playerid] = 0;
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_AIRBREAK);
    }

    if (height < AC_NO_GROUND)
    {
        ACUgStrike[playerid] = 0;
        return 1;
    }

    new Float:gx, Float:gy, Float:gz;

    if (!CA_RayCastLine(x, y, z + 300.0, x, y, z + 0.5, gx, gy, gz))
    {
        ACUgStrike[playerid] = 0;
        return 1;
    }

    AC_Debug(playerid, AC_INVISIBLE_UG, "no ground, ceiling %.1f above (limit %.1f)", gz - z, AC_UNDERGROUND);

    if (gz - z < AC_UNDERGROUND)
    {
        ACUgStrike[playerid] = 0;
        return 1;
    }

    if (++ACUgStrike[playerid] >= AC_UG_STRIKES)
    {
        ACUgStrike[playerid] = 0;
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_INVISIBLE_UG);
    }

    return 1;
}

public AC_TickMovement()
{
    foreach (new playerid : Player)
    {
        if (!User[playerid][isLogged]) continue;

        AC_CheckSurf(playerid);
        AC_MovementTick(playerid);

        if (!ACFrozen[playerid]) continue;
        if (GetTickCount() - ACFrozenTick[playerid] < AC_NOP_SETTLE) continue;

        if (IsPlayerInAnyVehicle(playerid)) continue;

        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);

        new Float:moved = floatsqroot((x - ACFrozenX[playerid]) * (x - ACFrozenX[playerid])
                                    + (y - ACFrozenY[playerid]) * (y - ACFrozenY[playerid]));

        if (moved >= 0.5)
            AC_Debug(playerid, AC_NOP_CONTROLLABLE, "frozen and moved %.1f (limit %.1f)", moved, AC_NOP_DISTANCE);

        if (moved < AC_NOP_DISTANCE) continue;

        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NOP_CONTROLLABLE);
        AC_FreezeBaseline(playerid);
    }
    return 1;
}

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    ACFrozen[playerid] = false;
    ACRunStrike[playerid] = ACAirStrike[playerid] = 0;
    ACMoveTick[playerid] = ACTeleportTick[playerid] = GetTickCount();
    AC_SetDebug(playerid, false);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    AC_SetDebug(playerid, false);
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    ACFrozen[playerid] = false;
    return 1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (oldstate != PLAYER_STATE_ONFOOT) return 1;
    if (newstate != PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_PASSENGER) return 1;

    if (!User[playerid][isLogged] || User[playerid][isAfk]) return 1;
    if (GetTickCount() - ACTeleportTick[playerid] < AC_MOVE_GRACE) return 1;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new Float:dx = x - ACMoveX[playerid], Float:dy = y - ACMoveY[playerid];
    new Float:moved = floatsqroot(dx * dx + dy * dy);

    AC_Debug(playerid, AC_CLICK_WARP, "entered a vehicle %.1f from the last point (limit %.1f)", moved, AC_VEHICLE_WARP);

    if (moved >= AC_VEHICLE_WARP)
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_CLICK_WARP);

    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    ACFrozen[playerid] = false;
    return 1;
}

hook OnGameModeInit()
{
    SetTimer("AC_TickMovement", AC_NOP_INTERVAL, true);
    return 1;
}

#if defined _ALS_TogglePlayerControllable
    #undef TogglePlayerControllable
#else
    #define _ALS_TogglePlayerControllable
#endif
#define TogglePlayerControllable AC_TogglePlayerControllable

#if defined _ALS_SetPlayerPos
    #undef SetPlayerPos
#else
    #define _ALS_SetPlayerPos
#endif
#define SetPlayerPos AC_SetPlayerPos

#if defined _ALS_PutPlayerInVehicle
    #undef PutPlayerInVehicle
#else
    #define _ALS_PutPlayerInVehicle
#endif
#define PutPlayerInVehicle AC_PutPlayerInVehicle

#if defined _ac_nops_included
    #endinput
#endif
#define _ac_nops_included

#include <YSI_Coding\y_hooks>

#define AC_NOPCHK_SETTLE  (2500)
#define AC_NOPCHK_TIMER   (1000)
#define AC_NOPCHK_DIST    (40.0)
#define AC_NOPCHK_SLOTS   (13)

static
    ACNopIntTick[MAX_PLAYERS],
    ACNopInterior[MAX_PLAYERS],

    ACNopWeaponTick[MAX_PLAYERS],
    WEAPON:ACNopWeapon[MAX_PLAYERS],

    ACNopPosTick[MAX_PLAYERS],
    Float:ACNopPos[MAX_PLAYERS][3],

    ACNopAnimTick[MAX_PLAYERS],
    ACNopAnimFreeze[MAX_PLAYERS],

    ACNopEngineTick[MAX_PLAYERS],
    ACNopEngineVeh[MAX_PLAYERS];

forward AC_CheckNopCalls();

static AC_ClearNops(playerid)
{
    ACNopIntTick[playerid] = ACNopWeaponTick[playerid] = ACNopPosTick[playerid] = 0;
    ACNopAnimTick[playerid] = ACNopEngineTick[playerid] = 0;
    ACNopWeapon[playerid] = WEAPON_FIST;
    return 1;
}

stock AC_NOP_SetPlayerInterior(playerid, interiorid)
{
    ACNopInterior[playerid] = interiorid;
    ACNopIntTick[playerid] = GetTickCount();

    return SetPlayerInterior(playerid, interiorid);
}

stock AC_NOP_GivePlayerWeapon(playerid, WEAPON:weaponid, ammo)
{
    if (ammo > 0 && weaponid != WEAPON_FIST)
    {
        ACNopWeapon[playerid] = weaponid;
        ACNopWeaponTick[playerid] = GetTickCount();
    }

    return GivePlayerWeapon(playerid, weaponid, ammo);
}

stock AC_NOP_ResetPlayerWeapons(playerid)
{
    ACNopWeaponTick[playerid] = 0;

    return ResetPlayerWeapons(playerid);
}

stock AC_NOP_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
    ACNopPos[playerid][0] = x;
    ACNopPos[playerid][1] = y;
    ACNopPos[playerid][2] = z;
    ACNopPosTick[playerid] = GetTickCount();

    return SetPlayerPos(playerid, x, y, z);
}

stock AC_NOP_ApplyAnimation(playerid, const animlib[], const animname[], Float:fDelta, loop, lockx, locky, freeze, animTime, FORCE_SYNC:forcesync = SYNC_NONE)
{
    if (fDelta > 0.0 && (loop || freeze || animTime > 1500) && strcmp(animname, "null", true) != 0)
    {
        ACNopAnimTick[playerid] = GetTickCount();
        ACNopAnimFreeze[playerid] = freeze;
    }
    else
    {
        ACNopAnimTick[playerid] = 0;
    }

    return ApplyAnimation(playerid, animlib, animname, fDelta, bool:loop, bool:lockx, bool:locky, bool:freeze, animTime, forcesync);
}

stock AC_NOP_ClearAnimations(playerid, FORCE_SYNC:forcesync = SYNC_NONE)
{
    ACNopAnimTick[playerid] = 0;
    return ClearAnimations(playerid, forcesync);
}

stock AC_NOP_SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective)
{
    if (engine == 0 || engine == VEHICLE_PARAMS_OFF)
    {
        foreach (new i : Player)
        {
            if (GetPlayerVehicleID(i) == vehicleid && GetPlayerVehicleSeat(i) == 0)
            {
                ACNopEngineTick[i] = GetTickCount();
                ACNopEngineVeh[i] = vehicleid;
            }
        }
    }
    else if (engine == 1 || engine == VEHICLE_PARAMS_ON)
    {
        foreach (new i : Player)
        {
            if (GetPlayerVehicleID(i) == vehicleid)
            {
                ACNopEngineTick[i] = 0;
            }
        }
    }

    return SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
}

public AC_CheckNopCalls()
{
    new tick = GetTickCount();

    foreach (new playerid : Player)
    {
        if (!User[playerid][isLogged]) continue;

        if (GetPlayerState(playerid) == PLAYER_STATE_WASTED
         || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
        {
            AC_ClearNops(playerid);
            continue;
        }

        if (ACNopIntTick[playerid] && tick - ACNopIntTick[playerid] >= AC_NOPCHK_SETTLE)
        {
            new current = GetPlayerInterior(playerid);
            ACNopIntTick[playerid] = 0;

            AC_Debug(playerid, AC_NOP_INTERIOR, "interior %d, server set %d", current, ACNopInterior[playerid]);

            if (current != ACNopInterior[playerid])
                CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NOP_INTERIOR);
        }

        if (ACNopWeaponTick[playerid] && tick - ACNopWeaponTick[playerid] >= AC_NOPCHK_SETTLE)
        {
            new WEAPON:expected = ACNopWeapon[playerid], WEAPON:carried, ammo;
            new bool:found = false;
            ACNopWeaponTick[playerid] = 0;

            for (new slot; slot < AC_NOPCHK_SLOTS; slot++)
            {
                GetPlayerWeaponData(playerid, WEAPON_SLOT:slot, carried, ammo);

                if (carried == expected && ammo > 0)
                {
                    found = true;
                    break;
                }
            }

            AC_Debug(playerid, AC_NOP_WEAPON, "weapon %d given, carried %d", _:expected, _:found);

            if (!found)
                CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NOP_WEAPON);
        }

        if (ACNopPosTick[playerid] && tick - ACNopPosTick[playerid] >= AC_NOPCHK_SETTLE)
        {
            ACNopPosTick[playerid] = 0;

            if (IsPlayerInAnyVehicle(playerid)
             || User[playerid][isAfk]
             || GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK
             || GetPlayerSurfingObjectID(playerid) != INVALID_OBJECT_ID
             || GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID) continue;

            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);

            new Float:dx = x - ACNopPos[playerid][0], Float:dy = y - ACNopPos[playerid][1];
            new Float:moved = floatsqroot(dx * dx + dy * dy);

            AC_Debug(playerid, AC_NOP_POS, "stayed %.1f from the destination (limit %.1f)", moved, AC_NOPCHK_DIST);

            if (moved >= AC_NOPCHK_DIST)
                CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NOP_POS);
        }

        if (ACNopAnimTick[playerid] && tick - ACNopAnimTick[playerid] >= AC_NOPCHK_SETTLE)
        {
            ACNopAnimTick[playerid] = 0;

            if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT
             && !User[playerid][isAfk]
             && GetPlayerSurfingObjectID(playerid) == INVALID_OBJECT_ID
             && GetPlayerSurfingVehicleID(playerid) == INVALID_VEHICLE_ID)
            {
                new animIndex = GetPlayerAnimationIndex(playerid);

                if (animIndex == 0)
                {
                    AC_Debug(playerid, AC_NOP_ANIMATION, "expected animation but index is 0");
                    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NOP_ANIMATION);
                }
            }
        }

        if (ACNopEngineTick[playerid] && tick - ACNopEngineTick[playerid] >= AC_NOPCHK_SETTLE)
        {
            ACNopEngineTick[playerid] = 0;

            if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) == ACNopEngineVeh[playerid])
            {
                new engine, lights, alarm, doors, bonnet, boot, objective;
                GetVehicleParamsEx(ACNopEngineVeh[playerid], engine, lights, alarm, doors, bonnet, boot, objective);

                if (engine == 0 || engine == VEHICLE_PARAMS_OFF)
                {
                    new Float:vx, Float:vy, Float:vz;
                    GetVehicleVelocity(ACNopEngineVeh[playerid], vx, vy, vz);
                    new Float:speed = floatsqroot(vx * vx + vy * vy + vz * vz);

                    if (speed > 0.15)
                    {
                        AC_Debug(playerid, AC_NOP_ENGINE_STATE, "engine off but moving at speed %.2f", speed);
                        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_NOP_ENGINE_STATE);
                    }
                }
            }
        }
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    AC_ClearNops(playerid);
    return 1;
}

hook OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    AC_ClearNops(playerid);
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    AC_ClearNops(playerid);
    return 1;
}

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (oldstate == PLAYER_STATE_DRIVER)
    {
        ACNopEngineTick[playerid] = 0;
    }
    if (newstate != PLAYER_STATE_ONFOOT)
    {
        ACNopAnimTick[playerid] = 0;
    }
    return 1;
}

hook OnGameModeInit()
{
    SetTimer("AC_CheckNopCalls", AC_NOPCHK_TIMER, true);
    return 1;
}

#if defined _ALS_SetPlayerInterior
    #undef SetPlayerInterior
#else
    #define _ALS_SetPlayerInterior
#endif
#define SetPlayerInterior AC_NOP_SetPlayerInterior

#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon AC_NOP_GivePlayerWeapon

#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons AC_NOP_ResetPlayerWeapons

#if defined _ALS_SetPlayerPos
    #undef SetPlayerPos
#else
    #define _ALS_SetPlayerPos
#endif
#define SetPlayerPos AC_NOP_SetPlayerPos

#if defined _ALS_ApplyAnimation
    #undef ApplyAnimation
#else
    #define _ALS_ApplyAnimation
#endif
#define ApplyAnimation AC_NOP_ApplyAnimation

#if defined _ALS_ClearAnimations
    #undef ClearAnimations
#else
    #define _ALS_ClearAnimations
#endif
#define ClearAnimations AC_NOP_ClearAnimations

#if defined _ALS_SetVehicleParamsEx
    #undef SetVehicleParamsEx
#else
    #define _ALS_SetVehicleParamsEx
#endif
#define SetVehicleParamsEx AC_NOP_SetVehicleParamsEx


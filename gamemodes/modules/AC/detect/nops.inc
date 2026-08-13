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
    Float:ACNopPos[MAX_PLAYERS][3];

forward AC_CheckNopCalls();

static AC_ClearNops(playerid)
{
    ACNopIntTick[playerid] = ACNopWeaponTick[playerid] = ACNopPosTick[playerid] = 0;
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

#if defined _ac_ammo_included
    #endinput
#endif
#define _ac_ammo_included

#define AC_WS_MARGIN      (1)
#define AC_WS_BUDGET      (10)
#define AC_WS_MAX_SLOTS   (13)

static
    ACShotCount[MAX_PLAYERS][AC_WS_MAX_SLOTS],
    ACLastAmmo[MAX_PLAYERS][AC_WS_MAX_SLOTS],
    bool:ACAmmoKnown[MAX_PLAYERS][AC_WS_MAX_SLOTS],
    ACExtraBudget[MAX_PLAYERS],
    ACStaticShots[MAX_PLAYERS][AC_WS_MAX_SLOTS];

static bool:AC_IsTrackedSlot(slot)
{
    switch (slot)
    {
        case 2, 4, 5, 6: return true;
    }
    return false;
}

stock AC_ResetAmmoSlot(playerid, slot)
{
    if (slot < 0 || slot >= AC_WS_MAX_SLOTS) return 0;

    ACShotCount[playerid][slot] = 0;
    ACAmmoKnown[playerid][slot] = false;
    ACStaticShots[playerid][slot] = 0;
    return 1;
}

stock AC_ResetAmmoTracking(playerid)
{
    for (new i; i < AC_WS_MAX_SLOTS; i++)
    {
        ACShotCount[playerid][i] = 0;
        ACLastAmmo[playerid][i] = 0;
        ACAmmoKnown[playerid][i] = false;
        ACStaticShots[playerid][i] = 0;
    }

    ACExtraBudget[playerid] = 0;
    return 1;
}

stock AC_CountShot(playerid, WEAPON:weaponid)
{
    new slot = GetWeaponSlot(weaponid);

    if (!AC_IsTrackedSlot(slot)) return 0;

    ACShotCount[playerid][slot]++;
    return 1;
}

stock AC_CheckSlotAmmo(playerid, slot, ammo)
{
    if (!AC_IsTrackedSlot(slot) || !IsPlayerLogged(playerid)) return 0;

    if (!ACAmmoKnown[playerid][slot])
    {
        ACLastAmmo[playerid][slot] = ammo;
        ACAmmoKnown[playerid][slot] = true;
        ACShotCount[playerid][slot] = 0;
        ACStaticShots[playerid][slot] = 0;
        return 0;
    }

    new spent = ACLastAmmo[playerid][slot] - ammo, shots = ACShotCount[playerid][slot];

    ACLastAmmo[playerid][slot] = ammo;
    ACShotCount[playerid][slot] = 0;

    if (spent < 0) return 0;
    if (!shots) return 0;

    if (spent == 0)
    {
        ACStaticShots[playerid][slot] += shots;
        if (ACStaticShots[playerid][slot] >= 12)
        {
            ACStaticShots[playerid][slot] = 0;
            AC_Debug(playerid, AC_INFINITE_AMMO, "slot %d: 12 shots with 0 ammo spent (infinite ammo)", slot);
            CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_INFINITE_AMMO);
            return 1;
        }
    }
    else
    {
        ACStaticShots[playerid][slot] = 0;
    }

    new extra = shots - spent;

    AC_Debug(playerid, AC_EXTRA_WS, "slot %d: %d shots, %d ammo spent, %d extra (margin %d, budget %d/%d)",
        slot, shots, spent, extra, AC_WS_MARGIN, ACExtraBudget[playerid], AC_WS_BUDGET);

    if (extra <= AC_WS_MARGIN)
    {
        if (ACExtraBudget[playerid] > 0) ACExtraBudget[playerid]--;
        return 0;
    }

    ACExtraBudget[playerid] += extra - AC_WS_MARGIN;

    if (ACExtraBudget[playerid] < AC_WS_BUDGET) return 0;

    ACExtraBudget[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_EXTRA_WS);
    return 1;
}

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    AC_ResetAmmoTracking(playerid);
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    AC_ResetAmmoTracking(playerid);
    return 1;
}

hook OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    AC_ResetAmmoTracking(playerid);
    return 1;
}

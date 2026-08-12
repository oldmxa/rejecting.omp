#if defined _ac_combat_included
    #endinput
#endif
#define _ac_combat_included

#include <YSI_Coding\y_hooks>

#define AC_DAMAGE_MARGIN (1.6)

#define AC_FIRE_STRIKES (4)

#define AC_CBUG_WINDOW  (350)
#define AC_CBUG_SAMPLES (4)
#define AC_CBUG_SPREAD  (15)
#define AC_CBUG_STRIKES (3)

static
    ACLastShot[MAX_PLAYERS],
    ACLastWeapon[MAX_PLAYERS],
    ACFireStrike[MAX_PLAYERS],
    ACCbugSample[MAX_PLAYERS][AC_CBUG_SAMPLES],
    ACCbugCount[MAX_PLAYERS],
    ACCbugStrike[MAX_PLAYERS];

static bool:AC_IsCbugWeapon(WEAPON:weaponid)
{
    switch (weaponid)
    {
        case WEAPON_DEAGLE, WEAPON_SHOTGUN, WEAPON_SHOTGSPA, WEAPON_RIFLE, WEAPON_SNIPER: return true;
    }
    return false;
}

static AC_CheckAutoCbug(playerid, WEAPON:weaponid, elapsed)
{
    if (!AC_IsCbugWeapon(weaponid) || elapsed <= 0 || elapsed >= AC_CBUG_WINDOW)
    {
        ACCbugCount[playerid] = 0;
        return 1;
    }

    ACCbugSample[playerid][ACCbugCount[playerid] % AC_CBUG_SAMPLES] = elapsed;

    if (++ACCbugCount[playerid] < AC_CBUG_SAMPLES) return 1;

    ACCbugCount[playerid] = AC_CBUG_SAMPLES;

    new low = ACCbugSample[playerid][0], high = low;

    for (new i = 1; i < AC_CBUG_SAMPLES; i++)
    {
        new value = ACCbugSample[playerid][i];

        if (value < low) low = value;
        if (value > high) high = value;
    }

    AC_Debug(playerid, AC_AUTO_CBUG, "%d c-bugs in a row, spread %dms (limit %dms, strike %d/%d)",
        AC_CBUG_SAMPLES, high - low, AC_CBUG_SPREAD, ACCbugStrike[playerid], AC_CBUG_STRIKES);

    if (high - low > AC_CBUG_SPREAD)
    {
        ACCbugStrike[playerid] = 0;
        return 1;
    }

    ACCbugCount[playerid] = 0;

    if (++ACCbugStrike[playerid] < AC_CBUG_STRIKES) return 1;

    ACCbugStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_AUTO_CBUG);
    return 1;
}

static AC_MinFireDelay(WEAPON:weaponid)
{
    switch (weaponid)
    {
        case WEAPON_COLT45, WEAPON_SILENCED, WEAPON_DEAGLE: return 45;
        case WEAPON_SHOTGUN, WEAPON_SHOTGSPA: return 150;
        case WEAPON_SAWEDOFF: return 100;
        case WEAPON_UZI, WEAPON_TEC9: return 30;
        case WEAPON_MP5: return 35;
        case WEAPON_AK47, WEAPON_M4: return 65;
        case WEAPON_RIFLE, WEAPON_SNIPER: return 200;
    }
    return 0;
}

hook OnPlayerConnect(playerid)
{
    ACLastShot[playerid] = ACFireStrike[playerid] = ACCbugCount[playerid] = ACCbugStrike[playerid] = 0;
    ACLastWeapon[playerid] = -1;
    return 1;
}

hook OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    new minDelay = AC_MinFireDelay(weaponid);
    if (!minDelay) return 1;

    new tick = GetTickCount(), elapsed = tick - ACLastShot[playerid];
    new bool:first = (ACLastShot[playerid] == 0 || ACLastWeapon[playerid] != _:weaponid);
    ACLastShot[playerid] = tick;
    ACLastWeapon[playerid] = _:weaponid;

    if (first)
    {
        ACFireStrike[playerid] = ACCbugCount[playerid] = 0;
        return 1;
    }
    if (elapsed > 0)
        AC_Debug(playerid, AC_RAPID_FIRE, "weapon %d, shot in %dms (minimum %dms), hitType %d", _:weaponid, elapsed, minDelay, _:hittype);

    AC_CheckAutoCbug(playerid, weaponid, elapsed);

    if (elapsed <= 0 || elapsed >= minDelay)
    {
        ACFireStrike[playerid] = 0;
        return 1;
    }
    if (++ACFireStrike[playerid] >= AC_FIRE_STRIKES)
    {
        ACFireStrike[playerid] = 0;
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_RAPID_FIRE);
    }
    return 1;
}

hook OnPlayerDamageDone(playerid, Float:amount, issuerid, WEAPON:weapon, bodypart)
{
    if (issuerid == INVALID_PLAYER_ID || !User[issuerid][isLogged]) return 1;

    AC_NoteStun(playerid);

    new Float:maximum = GetWeaponDamage(weapon);
    if (maximum <= 0.0) return 1;

    if (amount > maximum)
        AC_Debug(issuerid, AC_DAMAGER, "damage %.1f (weapon deals %.1f, cap %.1f)", amount, maximum, maximum * AC_DAMAGE_MARGIN);

    if (amount > maximum * AC_DAMAGE_MARGIN)
        CallRemoteFunction("OnRejectingACDetection", "ii", issuerid, AC_DAMAGER);

    return 1;
}


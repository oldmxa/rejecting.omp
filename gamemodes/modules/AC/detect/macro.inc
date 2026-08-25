#if defined _ac_macro_included
    #endinput
#endif
#define _ac_macro_included

#include <YSI_Coding\y_hooks>

#define AC_MACRO_MAX_WARNINGS     (3)
#define AC_MACRO_MIN_SPEED        (40)
#define AC_MACRO_MIN_SPEED_DIFF   (0)
#define AC_MACRO_SPRINT_LIMIT     (65)
#define AC_MACRO_FORGET_AFTER     (900)
#define AC_MACRO_PACKETLOSS       (1.5)

static
    ACMacroLastSprint[MAX_PLAYERS],
    ACMacroLastSpeed[MAX_PLAYERS],
    ACMacroWarnings[MAX_PLAYERS],
    ACMacroLastWarn[MAX_PLAYERS];

static AC_MacroUpdate(playerid, speed, tick, bool:forget)
{
    ACMacroLastSprint[playerid] = tick;
    ACMacroLastSpeed[playerid] = speed;

    if (forget && ACMacroWarnings[playerid] > 0
        && tick - ACMacroLastWarn[playerid] >= AC_MACRO_FORGET_AFTER - GetPlayerPing(playerid))
        ACMacroWarnings[playerid] = 0;

    return 1;
}

static AC_MacroSpeed(playerid)
{
    new Float:vx, Float:vy, Float:vz;

    GetPlayerVelocity(playerid, vx, vy, vz);
    return floatround(VectorSize(vx, vy, vz) * 179.28625);
}

hook OnPlayerConnect(playerid)
{
    ACMacroLastSprint[playerid] = 0;
    ACMacroLastSpeed[playerid] = 0;
    ACMacroWarnings[playerid] = 0;
    ACMacroLastWarn[playerid] = 0;
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!ACSprintMacro || !AC_PRESSED(KEY_SPRINT)) return 1;
    if (!IsPlayerLogged(playerid) || IsPlayerAFK(playerid)) return 1;

    if (NetStats_PacketLossPercent(playerid) >= AC_MACRO_PACKETLOSS) return 1;

    new speed = AC_MacroSpeed(playerid), tick = GetTickCount();

    if (GetPlayerVehicleID(playerid) != 0
        || GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID
        || speed < AC_MACRO_MIN_SPEED
        || speed - ACMacroLastSpeed[playerid] < AC_MACRO_MIN_SPEED_DIFF)
        return AC_MacroUpdate(playerid, speed, tick, true);

    new diff = tick - ACMacroLastSprint[playerid];

    if (diff >= AC_MACRO_SPRINT_LIMIT || diff == 0)
        return AC_MacroUpdate(playerid, speed, tick, true);

    ACMacroWarnings[playerid]++;
    ACMacroLastWarn[playerid] = tick;

    if (AC_DebugActive())
        AC_Debug(playerid, E_REJECTING_AC_SPRINTHOOK,
            "sprint macro %d/%d, gap %dms, speed %d (+%d)",
            ACMacroWarnings[playerid], AC_MACRO_MAX_WARNINGS, diff,
            speed, speed - ACMacroLastSpeed[playerid]);

    if (ACMacroWarnings[playerid] >= AC_MACRO_MAX_WARNINGS)
    {
        ACMacroWarnings[playerid] = 0;
        ACMacroLastWarn[playerid] = 0;

        TogglePlayerControllable(playerid, false);
        TogglePlayerControllable(playerid, true);

        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, E_REJECTING_AC_SPRINTHOOK);
        return 1;
    }

    return AC_MacroUpdate(playerid, speed, tick, false);
}

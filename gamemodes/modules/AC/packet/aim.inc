#if defined _ac_sync_included
    #endinput
#endif
#define _ac_sync_included

const AC_AIM_SYNC = 203;
#define AC_AIM_STRIKES  (4)

#define AC_AIM_LEN_MIN  (0.80)
#define AC_AIM_LEN_MAX  (1.20)

static ACAimStrike[MAX_PLAYERS];
static ACAimProbe[MAX_PLAYERS];

#define AC_SCOPE_CAM_MODE (55)
#define AC_SCOPE_STRIKES  (15)

static ACScopeStrike[MAX_PLAYERS];

static AC_CheckScopeCam(playerid, camMode)
{
    if (camMode != AC_SCOPE_CAM_MODE || !IsPlayerLogged(playerid) || IsPlayerAFK(playerid))
    {
        ACScopeStrike[playerid] = 0;
        return 0;
    }

    new const WEAPON:weapon = GetPlayerWeapon(playerid);

    switch (weapon)
    {
        case WEAPON_SNIPER, WEAPON_ROCKETLAUNCHER, WEAPON_HEATSEEKER, WEAPON_CAMERA:
        {
            ACScopeStrike[playerid] = 0;
            return 0;
        }
    }

    AC_Debug(playerid, AC_FAKE_CAMMODE, "scope cam %d holding weapon %d (strike %d/%d)",
        camMode, _:weapon, ACScopeStrike[playerid] + 1, AC_SCOPE_STRIKES);

    if (++ACScopeStrike[playerid] < AC_SCOPE_STRIKES) return 0;

    ACScopeStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_FAKE_CAMMODE);
    return 1;
}

static bool:AC_IsSaneFloat(Float:value)
{
    if (value != value) return false;
    if (value < -100000.0 || value > 100000.0) return false;

    return true;
}

IPacket:AC_AIM_SYNC(playerid, BitStream:bs)
{
    if (IsPlayerNPC(playerid)) return 1;

    new aimData[PR_AimSync];

    BS_IgnoreBits(bs, 8);
    BS_ReadAimSync(bs, aimData);
    if (AC_DebugActive() && GetTickCount() - ACAimProbe[playerid] > 2000)
    {
        ACAimProbe[playerid] = GetTickCount();
        AC_Debug(playerid, AC_INVALID_AIMZ, "aim packet: vector %.2f %.2f %.2f, aimZ %.2f",
            aimData[PR_camFrontVec][0], aimData[PR_camFrontVec][1], aimData[PR_camFrontVec][2], aimData[PR_aimZ]);
    }

    AC_CheckScopeCam(playerid, aimData[PR_camMode]);

    new bool:bad = false;

    for (new i; i < 3; i++)
    {
        if (!AC_IsSaneFloat(aimData[PR_camFrontVec][i]) || !AC_IsSaneFloat(aimData[PR_camPos][i]))
        {
            bad = true;
            break;
        }
    }

    if (!bad && !AC_IsSaneFloat(aimData[PR_aimZ])) bad = true;

    if (!bad)
    {
        new Float:len = floatsqroot(
            aimData[PR_camFrontVec][0] * aimData[PR_camFrontVec][0] +
            aimData[PR_camFrontVec][1] * aimData[PR_camFrontVec][1] +
            aimData[PR_camFrontVec][2] * aimData[PR_camFrontVec][2]);

        if (len < AC_AIM_LEN_MIN || len > AC_AIM_LEN_MAX) bad = true;
    }

    if (!bad)
    {
        ACAimStrike[playerid] = 0;
        AC_StoreAimVector(playerid, aimData[PR_camFrontVec][0], aimData[PR_camFrontVec][1], aimData[PR_camFrontVec][2]);
        return 1;
    }

    if (++ACAimStrike[playerid] < AC_AIM_STRIKES) return 1;

    ACAimStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_INVALID_AIMZ);
    return 1;
}

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    ACAimStrike[playerid] = 0;
    ACScopeStrike[playerid] = 0;
    AC_ResetAim(playerid);
    return 1;
}

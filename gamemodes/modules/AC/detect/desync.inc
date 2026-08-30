#if defined _ac_desync_included
    #endinput
#endif
#define _ac_desync_included

#include <YSI_Coding\y_hooks>

#define AC_DESYNC_GAP_MIN (700)
#define AC_DESYNC_GAP_MAX (4000)
#define AC_DESYNC_WINDOW  (30_000)
#define AC_DESYNC_BURSTS  (4)
#define AC_DESYNC_MAX_PL  (3.0)
#define AC_DESYNC_GRACE   (20)

static
    ACSyncTick[MAX_PLAYERS],
    ACDesyncCount[MAX_PLAYERS],
    ACDesyncWindow[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    ACSyncTick[playerid] =
    ACDesyncWindow[playerid] = GetTickCount();

    ACDesyncCount[playerid] = 0;
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if (IsPlayerNPC(playerid)) return 1;

    new const tickCount = GetTickCount();
    new const gap = tickCount - ACSyncTick[playerid];

    ACSyncTick[playerid] = tickCount;

    if (tickCount - ACDesyncWindow[playerid] > AC_DESYNC_WINDOW)
    {
        ACDesyncWindow[playerid] = tickCount;
        ACDesyncCount[playerid] = 0;
    }

    if (gap < AC_DESYNC_GAP_MIN) return 1;

    if (!IsPlayerLogged(playerid) || IsPlayerAFK(playerid) || IsPlayerSpectating(playerid)
     || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING
     || gettime() - User[playerid][timerRespawn] < AC_DESYNC_GRACE)
    {
        ACDesyncCount[playerid] = 0;
        return 1;
    }

    new const Float:packetLoss = NetStats_PacketLossPercent(playerid);

    if (gap > AC_DESYNC_GAP_MAX || packetLoss > AC_DESYNC_MAX_PL)
    {
        ACDesyncCount[playerid] = 0;
        return 1;
    }

    AC_Debug(playerid, AC_FAKE_LAG, "sync gap %d ms (burst %d/%d, loss %.2f%%, ping %d)",
        gap, ACDesyncCount[playerid] + 1, AC_DESYNC_BURSTS, packetLoss, GetPlayerPing(playerid));

    if (++ACDesyncCount[playerid] < AC_DESYNC_BURSTS) return 1;

    ACDesyncCount[playerid] = 0;
    ACDesyncWindow[playerid] = tickCount;

    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_FAKE_LAG);
    return 1;
}

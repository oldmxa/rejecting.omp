#if defined _ac_limits_included
    #endinput
#endif
#define _ac_limits_included


#define AC_LIMIT_INTERVAL (5000)
#define AC_LIMIT_STRIKES  (3)
#define AC_LIMIT_GRACE    (20)

enum
{
    AC_LIMIT_PING,
    AC_LIMIT_PL,
    AC_LIMIT_FPS,

    MAX_AC_LIMITS
}

static ACLimitStrike[MAX_PLAYERS][MAX_AC_LIMITS];

forward AC_CheckLimits();

static AC_LimitKick(playerid, type, const detail[])
{
    static const REASON[MAX_AC_LIMITS][] = {"ping", "packet loss", "FPS"};

    for (new i; i < MAX_AC_LIMITS; i++) ACLimitStrike[playerid][i] = 0;

    va_SendClientMessage(playerid, -1, "{MAIN}[RAC] {FFFFFF}You were kicked for %s: {FF0000}%s{FFFFFF}.", REASON[type], detail);

    foreach (new i : Player)
    {
        if(IsAdmin(i))
            va_SendClientMessage(i, -1, AC_TAG_NORM"%s[%d] kicked for %s (%s).", User[playerid][userName], playerid, REASON[type], detail);
    }

    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "kicks", "Rejecting AC", detail);
    KickEx(playerid, 1500);
    return 1;
}

static AC_CheckLimit(playerid, type, bool:over, const detail[])
{
    if (!over)
    {
        ACLimitStrike[playerid][type] = 0;
        return 0;
    }

    if (++ACLimitStrike[playerid][type] < AC_LIMIT_STRIKES) return 0;

    AC_LimitKick(playerid, type, detail);
    return 1;
}

public AC_CheckLimits()
{
    new detail[32];

    foreach (new playerid : Player)
    {
        if (IsPlayerNPC(playerid)) continue;
        if (!IsPlayerLogged(playerid) || IsPlayerAFK(playerid)) continue;
        if (IsPlayerInLounge(playerid)) continue;
        if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING) continue;

        if (gettime() - User[playerid][timerRespawn] < AC_LIMIT_GRACE) continue;

        new ping = GetPlayerPing(playerid);
        format(detail, sizeof(detail), "%d ms", ping);
        if (ServerMaxPing > 0 && AC_CheckLimit(playerid, AC_LIMIT_PING, (ping > ServerMaxPing), detail)) continue;

        new Float:packetLoss = NetStats_PacketLossPercent(playerid);
        format(detail, sizeof(detail), "%.2f%%", packetLoss);
        if (ServerMaxPacketLoss > 0.0 && AC_CheckLimit(playerid, AC_LIMIT_PL, (packetLoss > ServerMaxPacketLoss), detail)) continue;

        if (!IsFpsFreeArena(User[playerid][locationId]))
        {
            new fps = User[playerid][FPS];
            format(detail, sizeof(detail), "%d fps", fps);
            new bool:badFps = (fps > 0 && (fps < ServerMinFPS || fps > ServerMaxFPS));
            AC_CheckLimit(playerid, AC_LIMIT_FPS, badFps, detail);
        }
    }
    return 1;
}

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
    for (new i; i < MAX_AC_LIMITS; i++) ACLimitStrike[playerid][i] = 0;
    return 1;
}

hook OnGameModeInit()
{
    SetTimer("AC_CheckLimits", AC_LIMIT_INTERVAL, true);
    return 1;
}

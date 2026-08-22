#if defined _ac_version_included
    #endinput
#endif
#define _ac_version_included

#include <YSI_Coding\y_hooks>

enum e_AC_CLIENT
{
    acVersion[24],
    acUpdateCount,
    bool:acAnswered[2]
}

static ACClient[MAX_PLAYERS][e_AC_CLIENT];

forward AC_DelayedKick(playerid);

public AC_DelayedKick(playerid)
{
    if (IsPlayerConnected(playerid)) Kick(playerid);
    return 1;
}

stock AC_SendClientChecks(playerid)
{
    if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return 0;

    ACClient[playerid][acUpdateCount] = -1;
    ACClient[playerid][acAnswered][0] = false;
    ACClient[playerid][acAnswered][1] = false;

    GetPlayerVersion(playerid, ACClient[playerid][acVersion], 24);

    if (!strequal(ACClient[playerid][acVersion], "0.3.7"))
    {
        ACClient[playerid][acUpdateCount] = 0;
        SendClientCheck(playerid, AC_CHECK_R2_A, AC_CHECK_R2_A_ADR, 0, AC_CHECK_R2_A_LEN);
        SendClientCheck(playerid, AC_CHECK_R2_B, AC_CHECK_R2_B_ADR, 0, AC_CHECK_R2_B_LEN);
    }

    SendClientCheck(playerid, AC_CHECK_DESKTOP, AC_CHECK_DESKTOP_ADR, 0, AC_CHECK_DESKTOP_LEN);
    return 1;
}

stock AC_GetClientVersion(playerid, destination[], len = sizeof(destination))
{
    format(destination, len, "%s", ACClient[playerid][acVersion]);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    ACClient[playerid][acUpdateCount] = -1;
    AC_SendClientChecks(playerid);

    if (OMPKickEnabled && !IsPlayerNPC(playerid) && !IsPlayerUsingOmp(playerid))
    {
        SendClientMessage(playerid, 0xE74C3CFF, "[AC] O servidor exige conexao exclusiva via open.mp Launcher oficial.");
        SendClientMessage(playerid, -1, "Baixe o launcher oficial em {MAIN}open.mp{FFFFFF} para jogar.");
        KickEx(playerid, AC_KICK_DELAY);
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    ACClient[playerid][acUpdateCount] = -1;
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if (IsPlayerNPC(playerid)) return 1;
    if (ACClient[playerid][acUpdateCount] == -1) return 1;

    if (++ACClient[playerid][acUpdateCount] < AC_R2_GRACE_UPDATES) return 1;

    ACClient[playerid][acUpdateCount] = -1;

    if (!R2KickEnabled) return 1;

    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, E_REJECTING_AC_FAKE_VERSION);
    return 1;
}

hook OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
    switch (actionid)
    {
        case AC_CHECK_R2_A:
        {
            if (memaddr != AC_CHECK_R2_A_ADR) return 1;

            ACClient[playerid][acAnswered][0] = true;
        }
        case AC_CHECK_R2_B: ACClient[playerid][acAnswered][1] = true;
        case AC_CHECK_DESKTOP: return SetPVarInt(playerid, "NotAndroid", 1);
        default: return 1;
    }

    if (ACClient[playerid][acAnswered][0] && ACClient[playerid][acAnswered][1])
        ACClient[playerid][acUpdateCount] = -1;

    return 1;
}

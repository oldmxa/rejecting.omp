#if defined _ac_version_included
    #endinput
#endif
#define _ac_version_included

#include <YSI_Coding\y_hooks>

enum e_AC_CLIENT
{
    acVersion[24],
    acUpdateCount,
    bool:acAnswered[2],
    bool:acSerialChecked
}

#define AC_SERIAL_LEN (40)

static ACClient[MAX_PLAYERS][e_AC_CLIENT];

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

static bool:AC_IsValidSerial(const serial[])
{
    new const len = strlen(serial);

    if (len != AC_SERIAL_LEN) return false;

    for (new i; i < len; i++)
    {
        new const c = serial[i];

        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) continue;

        return false;
    }
    return true;
}

stock AC_CheckSerial(playerid)
{
    if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return 0;

    new serial[64];
    gpci(playerid, serial, sizeof(serial));

    if (isnull(serial) || AC_IsValidSerial(serial)) return 0;

    AC_Debug(playerid, AC_FAKE_SERIAL, "serial has %d chars, expected %d hex",
        strlen(serial), AC_SERIAL_LEN);

    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_FAKE_SERIAL);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    ACClient[playerid][acUpdateCount] = -1;
    ACClient[playerid][acSerialChecked] = false;
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

    if (!ACClient[playerid][acSerialChecked] && IsPlayerLogged(playerid))
    {
        ACClient[playerid][acSerialChecked] = true;
        AC_CheckSerial(playerid);
    }

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

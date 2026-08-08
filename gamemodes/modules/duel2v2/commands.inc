#if defined _duel2v2_commands_included
    #endinput
#endif
#define _duel2v2_commands_included

// Duel 2v2 

@cmd(.alts = {"duel2v2", "create2v2"}) x1c(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return 1;

    if(IsPlayerInDuel2v2(playerid))
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are already in a 2v2 duel session!");
    }

    ShowDuel2v2ArenaDialog(playerid);
    return 1;
}

@cmd(.alts = {"accept2v2"}) gx1(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return 1;

    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid == -1 || !g_Duel2v2Data[matchid][d2_Active] || g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_PENDING)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You do not have any pending 2v2 invitations.");
    }

    ShowDuel2v2AcceptDialog(playerid);
    return 1;
}

@cmd() cancel2v2(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return 1;

    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid == -1 || !g_Duel2v2Data[matchid][d2_Active])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are not currently in a 2v2 duel!");
    }

    new reason[96];
    format(reason, sizeof(reason), "%s cancelled the 2v2 duel.", getPlayerName(playerid));
    ForfeitDuel2v2Session(matchid, playerid, reason);
    return 1;
}

#if defined _duel2v2_commands_included
    #endinput
#endif
#define _duel2v2_commands_included

// Duel 2v2 

@cmd(.alts = {"duel2v2", "create2v2"}) x1c(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return 1;

    if(User[playerid][isAfk] || User[playerid][locationId] == ARENA_AFK)
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot create a 2v2 duel while in the lounge/AFK.");

    if(User[playerid][isSpectating])
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot use this while spectating.");

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are already in a 1v1 duel session!");

    if(IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are already in a 2v2 duel session!");

    if(g_PlayerTraining[playerid][pTrain_Active])
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot create a 2v2 duel while in training mode.");

    if(g_Practice[playerid][pracActive])
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot create a 2v2 duel while in practice mode.");

    if(User[playerid][isCreatingArena])
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot use this while in the arena creator.");

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

    if(HasAcceptedDuel2v2(matchid, playerid))
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You already accepted this duel. Waiting for the other players.");
    }

    if(User[playerid][isSpectating] || User[playerid][isAfk] || User[playerid][isCreatingArena] ||
       GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE ||
       g_PlayerTraining[playerid][pTrain_Active] || g_Practice[playerid][pracActive])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are busy and cannot accept a 2v2 duel right now.");
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

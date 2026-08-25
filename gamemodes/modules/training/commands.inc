#if defined _training_commands_included
    #endinput
#endif
#define _training_commands_included

// commands

@cmd() training(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot enter training while in a 2v2 duel.");

    if (IsPlayerInTournamentMatch(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot enter training while playing a tournament match.");

    if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot enter training while in a duel.");

    if (IsPracticeActive(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Leave the practice world first.");

    if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot enter training while in a vehicle.");

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot enter training right now.");

    if (IsPlayerFrozen(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot do this while frozen.");

    if (IsPlayerSpectating(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Stop spectating first.");

    if (!g_PlayerTraining[playerid][pTrain_Active] && !IsPlayerInLounge(playerid))
    {
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The training mode can only be started from the lounge.");
    }

    ShowTrainingDialog(playerid);
    return 1;
}

@cmd() exittraining(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (!g_PlayerTraining[playerid][pTrain_Active])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You are not currently in a training arena.");
    }

    new bool:wasRunning = g_PlayerTraining[playerid][pTrain_ChallengeOn];

    ExitTrainingArena(playerid);

    if (wasRunning)
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You left the arena and the challenge was discarded.");
    else
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You exited the training arena.");

    return 1;
}

@cmd(.alts = {"rt", "reset"}) restarttraining(playerid, params[], help)
{
    if (!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (!g_PlayerTraining[playerid][pTrain_Active])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You are not currently in a training arena.");
    }

    RestartTrainingChallenge(playerid);
    return 1;
}

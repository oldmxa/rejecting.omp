#if defined _training_commands_included
    #endinput
#endif
#define _training_commands_included

// commands

@cmd() training(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot enter training while in a 2v2 duel.");

    ShowTrainingDialog(playerid);
    return 1;
}

@cmd() exittraining(playerid, params[], help)
{
    if (!User[playerid][isLogged])
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
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (!g_PlayerTraining[playerid][pTrain_Active])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You are not currently in a training arena.");
    }

    RestartTrainingChallenge(playerid);
    return 1;
}

#if defined _training_dialogs_included
    #endinput
#endif
#define _training_dialogs_included

// dialog training arenas.

static g_TrainingDialogBuf[256 * (TRAINING_MAX_ARENAS + 2) + 128];

forward DIALOG_TRAINING_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_TRAINING_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    if (IsValidTrainingArena(listitem))
    {
        EnterTrainingArena(playerid, listitem);
    }
    else if (listitem == g_TotalTrainingArenas && g_PlayerTraining[playerid][pTrain_Active])
    {
        ExitTrainingArena(playerid);
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You exited the training arena.");
    }
    return 1;
}

forward DIALOG_TRAINING_CHALLENGES(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_TRAINING_CHALLENGES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (response)
    {
        new empty[1];
        DIALOG_RANK1(playerid, 0, 1, 0, empty);
    }
    return 1;
}

forward ShowTrainingChallengeDialog(playerid);
public ShowTrainingChallengeDialog(playerid)
{
    new title[64] = "{MAIN}Training Challenges: {FFFFFF}Records";
    format_MainColor(playerid, title);

    if (g_TotalTrainingArenas == 0)
    {
        Dialog_ShowCallback(
            playerid,
            using public DIALOG_TRAINING_CHALLENGES<iiiis>,
            DIALOG_STYLE_MSGBOX,
            title,
            "No training arenas are currently registered.",
            "<",
            "Close"
        );
        return 1;
    }

    new line[192];
    format(g_TrainingDialogBuf, sizeof(g_TrainingDialogBuf), "{MAIN}Arena\t{MAIN}Your Record\t{MAIN}Server Record\t{MAIN}Holder\n");

    for (new i = 0; i < g_TotalTrainingArenas; i++)
    {
        new personal[24], server[24], holder[MAX_PLAYER_NAME + 1];

        if (g_ChallengePlayerRecord[playerid][i] > 0)
            format(personal, sizeof(personal), "{MAIN}%d dmg", g_ChallengePlayerRecord[playerid][i]);
        else
            format(personal, sizeof(personal), "{BEBEBE}-");

        if (g_ChallengeServerRecord[i] > 0)
        {
            format(server, sizeof(server), "{MAIN}%d dmg", g_ChallengeServerRecord[i]);
            format(holder, sizeof(holder), "%s", g_ChallengeServerHolder[i]);
        }
        else
        {
            format(server, sizeof(server), "{BEBEBE}-");
            format(holder, sizeof(holder), "-");
        }

        format(
            line, sizeof(line),
            "{FFFFFF}%s\t%s\t%s\t{FFFFFF}%s\n",
            g_TrainingArenas[i][tArena_Name],
            personal,
            server,
            holder
        );
        strcat(g_TrainingDialogBuf, line);
    }

    format_MainColor(playerid, g_TrainingDialogBuf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_TRAINING_CHALLENGES<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS,
        title,
        g_TrainingDialogBuf,
        "<",
        "Close"
    );
    return 1;
}

stock ShowTrainingDialog(playerid)
{
    if (g_TotalTrainingArenas == 0)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}No training arenas are currently registered.");
        return 1;
    }

    new line[256], title[64];
    format(title, sizeof(title), "{MAIN}Training System - Arenas");
    format_MainColor(playerid, title);

    format(g_TrainingDialogBuf, sizeof(g_TrainingDialogBuf), "{MAIN}Arena Name\t{MAIN}Category\t{MAIN}Your Record\t{MAIN}Server Record\t{MAIN}Status\n");

    for (new i = 0; i < g_TotalTrainingArenas; i++)
    {
        new bool:isCurrent = (g_PlayerTraining[playerid][pTrain_Active] && g_PlayerTraining[playerid][pTrain_ArenaID] == i);

        new personalRecord[24], serverRecord[48];

        if (g_ChallengePlayerRecord[playerid][i] > 0)
            format(personalRecord, sizeof(personalRecord), "{MAIN}%d dmg", g_ChallengePlayerRecord[playerid][i]);
        else
            format(personalRecord, sizeof(personalRecord), "{BEBEBE}-");

        if (g_ChallengeServerRecord[i] > 0)
            format(serverRecord, sizeof(serverRecord), "{MAIN}%d dmg {BEBEBE}(%s)", g_ChallengeServerRecord[i], g_ChallengeServerHolder[i]);
        else
            format(serverRecord, sizeof(serverRecord), "{BEBEBE}-");

        format(
            line,
            sizeof(line),
            "{FFFFFF}%s\t{MAIN}%s\t%s\t%s\t%s\n",
            g_TrainingArenas[i][tArena_Name],
            g_TrainingArenas[i][tArena_Type],
            personalRecord,
            serverRecord,
            isCurrent ? ("{23C552}Currently In") : ("{BEBEBE}Available")
        );
        strcat(g_TrainingDialogBuf, line);
    }

    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        strcat(g_TrainingDialogBuf, "{F3230E}[ Exit Training ]\t{BEBEBE}Leave Session\t{BEBEBE}-\t{BEBEBE}-\t{F3230E}Click to Exit\n");
    }

    format_MainColor(playerid, g_TrainingDialogBuf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_TRAINING_MAIN<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS,
        title,
        g_TrainingDialogBuf,
        "Select",
        "Close"
    );
    return 1;
}

forward DIALOG_TRAINING_HELP(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_TRAINING_HELP(playerid, dialogid, response, listitem, string:inputtext[])
{
    new cmd[16];
    strcopy(cmd, "/help");
    Command_ReProcess(playerid, cmd, 0);
    return 1;
}

stock ShowTrainingHelpDialog(playerid)
{
    new title[64] = "{MAIN}Training System";
    format_MainColor(playerid, title);

    new buf[1024];
    format(buf, sizeof(buf), "{MAIN}Command / Feature\t{MAIN}Description\n");
    strcat(buf, "{FFFFFF}\x95 {B4B5B7}/training\t{FFFFFF}Open Training menu & select an arena\n");
    strcat(buf, "{FFFFFF}\x95 {B4B5B7}/rt\t{FFFFFF}Restart current training challenge instantly\n");
    strcat(buf, "{FFFFFF}\x95 {B4B5B7}Key 'N'\t{FFFFFF}Shortcut key to restart the challenge in real-time\n");
    strcat(buf, "{FFFFFF}\x95 {B4B5B7}/exittraining\t{FFFFFF}Exit current training arena and return to Lounge\n");
    strcat(buf, "{FFFFFF}\x95 {B4B5B7}Records\t{FFFFFF}Personal & server high-scores are shown in the /training menu");

    format_MainColor(playerid, buf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_TRAINING_HELP<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS,
        title,
        buf,
        "Back",
        ""
    );
    return 1;
}

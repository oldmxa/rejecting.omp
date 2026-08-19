#if defined _duel2v2_dialogs_included
    #endinput
#endif
#define _duel2v2_dialogs_included

#include <YSI_Coding/y_hooks>

// Duel 2v2 Dialogs

forward DIALOG_DUEL2V2_ARENA(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_DUEL2V2_WEAPONS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_DUEL2V2_ROUNDS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_DUEL2V2_PLAYERS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_DUEL2V2_ACCEPT(playerid, dialogid, response, listitem, string:inputtext[]);

stock ShowDuel2v2ArenaDialog(playerid)
{
    if(IsPlayerInDuel2v2(playerid))
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are already in a 2v2 duel session!");
    }

    new title[64] = "{MAIN}Duel 2v2 - Select Arena";
    format_MainColor(playerid, title);

    new buf[1024];
    format(buf, sizeof(buf), "{MAIN}#\t{MAIN}Arena Name\n");

    new totalArenas = sizeof(DuelS_ArenaNames);
    for(new i = 0; i < totalArenas; i++)
    {
        new line[96];
        format(line, sizeof(line), "{MAIN}#%02d\t{FFFFFF}%s\n", i + 1, DuelS_ArenaNames[i]);
        strcat(buf, line);
    }

    format_MainColor(playerid, buf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_DUEL2V2_ARENA<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS,
        title,
        buf,
        "Next",
        "Cancel"
    );
    return 1;
}

public DIALOG_DUEL2V2_ARENA(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(listitem >= 0 && listitem < sizeof(DuelS_ArenaNames))
        {
            g_Duel2v2Wiz_Arena[playerid] = listitem;
            ShowDuel2v2WeaponsDialog(playerid);
        }
    }
    return 1;
}

stock ShowDuel2v2WeaponsDialog(playerid)
{
    new title[64] = "{MAIN}Duel 2v2 - Select Weapons";
    format_MainColor(playerid, title);

    new buf[512];
    format(buf, sizeof(buf), "{MAIN}#\t{MAIN}Weapon Loadout\n");

    new totalSets = sizeof(g_Duel2v2WeaponSets);
    for(new i = 0; i < totalSets; i++)
    {
        new line[96];
        format(line, sizeof(line), "{MAIN}#%02d\t{FFFFFF}%s\n", i + 1, g_Duel2v2WeaponSets[i][d2w_Name]);
        strcat(buf, line);
    }

    format_MainColor(playerid, buf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_DUEL2V2_WEAPONS<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS,
        title,
        buf,
        "Next",
        "Back"
    );
    return 1;
}

public DIALOG_DUEL2V2_WEAPONS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(listitem >= 0 && listitem < sizeof(g_Duel2v2WeaponSets))
        {
            g_Duel2v2Wiz_Weapons[playerid] = listitem;
            ShowDuel2v2RoundsDialog(playerid);
        }
    }
    else
    {
        ShowDuel2v2ArenaDialog(playerid);
    }
    return 1;
}

stock ShowDuel2v2RoundsDialog(playerid)
{
    new title[64] = "{MAIN}Duel 2v2 - Select Rounds";
    format_MainColor(playerid, title);

    new buf[512];
    format(buf, sizeof(buf), "{MAIN}Format\t{MAIN}Rounds\n");
    strcat(buf, "Single Round\t{FFFFFF}1 Round (Sudden Death)\n");
    strcat(buf, "Best of 3\t{FFFFFF}First to 2 Wins\n");
    strcat(buf, "Best of 5\t{FFFFFF}First to 3 Wins\n");
    strcat(buf, "Best of 7\t{FFFFFF}First to 4 Wins");

    format_MainColor(playerid, buf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_DUEL2V2_ROUNDS<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS,
        title,
        buf,
        "Next",
        "Back"
    );
    return 1;
}

public DIALOG_DUEL2V2_ROUNDS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0: g_Duel2v2Wiz_Rounds[playerid] = 1;
            case 1: g_Duel2v2Wiz_Rounds[playerid] = 3;
            case 2: g_Duel2v2Wiz_Rounds[playerid] = 5;
            case 3: g_Duel2v2Wiz_Rounds[playerid] = 7;
            default: g_Duel2v2Wiz_Rounds[playerid] = 1;
        }
        ShowDuel2v2PlayersDialog(playerid);
    }
    else
    {
        ShowDuel2v2WeaponsDialog(playerid);
    }
    return 1;
}

stock ShowDuel2v2PlayersDialog(playerid)
{
    new title[64] = "{MAIN}Duel 2v2 - Enter Player IDs";
    format_MainColor(playerid, title);

    new buf[512];
    format(buf, sizeof(buf),
        "{FFFFFF}Please enter the player IDs for your 2v2 duel in order:\n\n" \
        "{MAIN}Format: {FFFFFF}[Teammate ID] [Opponent 1 ID] [Opponent 2 ID]\n" \
        "{BEBEBE}Example: 5 12 18");

    format_MainColor(playerid, buf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_DUEL2V2_PLAYERS<iiiis>,
        DIALOG_STYLE_INPUT,
        title,
        buf,
        "Invite",
        "Back"
    );
    return 1;
}

public DIALOG_DUEL2V2_PLAYERS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new teammateid, opp1, opp2;
        if(sscanf(inputtext, "ddd", teammateid, opp1, opp2))
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Invalid format! Use: [Teammate ID] [Opp1 ID] [Opp2 ID]");
            ShowDuel2v2PlayersDialog(playerid);
            return 1;
        }

        if(IsPlayerBusy(teammateid))
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Teammate ID is busy, offline, or already in a duel/AFK.");
            ShowDuel2v2PlayersDialog(playerid);
            return 1;
        }
        if(IsPlayerBusy(opp1))
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Opponent 1 ID is busy, offline, or already in a duel/AFK.");
            ShowDuel2v2PlayersDialog(playerid);
            return 1;
        }
        if(IsPlayerBusy(opp2))
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Opponent 2 ID is busy, offline, or already in a duel/AFK.");
            ShowDuel2v2PlayersDialog(playerid);
            return 1;
        }

        if(teammateid == playerid || opp1 == playerid || opp2 == playerid ||
           teammateid == opp1 || teammateid == opp2 || opp1 == opp2)
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}All 4 player IDs must be unique and different!");
            ShowDuel2v2PlayersDialog(playerid);
            return 1;
        }

        if(IsPlayerInDuel2v2(teammateid) || IsPlayerInDuel2v2(opp1) || IsPlayerInDuel2v2(opp2))
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}One or more invited players are already in a duel!");
            return 1;
        }

        if(IsPlayerBusy(playerid))
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are busy and cannot create a 2v2 duel right now.");
            return 1;
        }

        new arenaid = g_Duel2v2Wiz_Arena[playerid];
        new wset = g_Duel2v2Wiz_Weapons[playerid];
        new rounds = g_Duel2v2Wiz_Rounds[playerid];

        if(CreateDuel2v2Challenge(playerid, teammateid, opp1, opp2, arenaid, wset, rounds) == -1)
        {
            SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Could not create the 2v2 duel. Please start over with /duel2v2.");
            return 1;
        }
    }
    else
    {
        ShowDuel2v2RoundsDialog(playerid);
    }
    return 1;
}

stock ShowDuel2v2AcceptDialog(playerid)
{
    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid == -1 || !g_Duel2v2Data[matchid][d2_Active] || g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_PENDING)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You do not have any pending 2v2 invitations.");
    }

    new creatorid = g_Duel2v2Data[matchid][d2_TeamA_Player1];
    new teammateid = g_Duel2v2Data[matchid][d2_TeamA_Player2];
    new opp1 = g_Duel2v2Data[matchid][d2_TeamB_Player1];
    new opp2 = g_Duel2v2Data[matchid][d2_TeamB_Player2];
    new arenaid = g_Duel2v2Data[matchid][d2_ArenaID];
    new wset = g_Duel2v2Data[matchid][d2_WeaponSet];
    new rounds = g_Duel2v2Data[matchid][d2_MaxRounds];

    new title[64] = "{MAIN}Duel 2v2 Invitation";
    format_MainColor(playerid, title);

    new buf[512];
    format(buf, sizeof(buf),
        "{FFFFFF}You have been invited to a 2v2 duel challenge!\n\n" \
        "{MAIN}Challenger: {FFFFFF}%s\n" \
        "{MAIN}Team A: {FFFFFF}%s & %s\n" \
        "{MAIN}Team B: {FFFFFF}%s & %s\n" \
        "{MAIN}Arena: {FFFFFF}%s\n" \
        "{MAIN}Weapons: {FFFFFF}%s\n" \
        "{MAIN}Rounds: {FFFFFF}%d Rounds\n\n" \
        "{FFFFFF}Do you want to accept this 2v2 duel?",
        getPlayerName(creatorid), getPlayerName(creatorid), getPlayerName(teammateid),
        getPlayerName(opp1), getPlayerName(opp2),
        DuelS_ArenaNames[arenaid], g_Duel2v2WeaponSets[wset][d2w_Name], rounds);

    format_MainColor(playerid, buf);

    Dialog_ShowCallback(
        playerid,
        using public DIALOG_DUEL2V2_ACCEPT<iiiis>,
        DIALOG_STYLE_MSGBOX,
        title,
        buf,
        "Accept",
        "Decline"
    );
    return 1;
}

public DIALOG_DUEL2V2_ACCEPT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        AcceptDuel2v2Challenge(playerid);
    }
    else
    {
        new matchid = g_PlayerDuel2v2ID[playerid];
        if(matchid != -1 && g_Duel2v2Data[matchid][d2_Active] &&
           g_Duel2v2Data[matchid][d2_State] == DUEL2V2_STATE_PENDING)
        {
            new reason[96];
            format(reason, sizeof(reason), "%s declined the 2v2 duel invitation.", getPlayerName(playerid));
            CancelDuel2v2Session(matchid, reason);
        }
    }
    return 1;
}

#if defined _tourney_dialogs_included
    #endinput
#endif
#define _tourney_dialogs_included

// Tournament Dialogs & Configuration Interface

forward DIALOG_TOURNEY_ADMIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_NAME(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_MODE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_SLOTS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_PRIZE_COINS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_PRIZE_MONEY(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ACHIEVEMENT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ENTRY_FEE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_WEAPONS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ARENA_CAT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ARENA_DUEL(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ARENA_SERVER(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ARENA_CUSTOM(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_ROUNDS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_TIME(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_SCHEDULE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_SCHEDULE_CUSTOM(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_MANAGE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TOURNEY_KICK_CONFIRM(playerid, dialogid, response, listitem, string:inputtext[]);

static stock ShowTourneyDuelArenasDialog(playerid)
{
    new title[64] = "{MAIN}Select Duel Arena";

    format_MainColor(playerid, title);
    new body[2048];
    format(body, sizeof(body), "{MAIN}ID\t{MAIN}Arena Name\t{MAIN}Interior\n");
    strcat(body, "{23C552}#00\t{23C552}[Random] Duel Arena\t{B4B5B7}Any Duel Map\n");

    for(new i = 0; i < sizeof(DuelS_ArenaNames); i++)
    {
        strcat(body, fmat_return("{B4B5B7}#%02d\t{FFFFFF}%s\t{BEBEBE}%d\n", i + 1, DuelS_ArenaNames[i], DuelS_ArenaInteriors[i]));
    }

    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ARENA_DUEL<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

static stock ShowTourneyServerArenasDialog(playerid)
{
    new title[64] = "{MAIN}Select Server Arena";

    format_MainColor(playerid, title);
    new body[4096];
    format(body, sizeof(body), "{MAIN}ID\t{MAIN}Zone Name\t{MAIN}Size\n");
    strcat(body, "{23C552}#00\t{23C552}[Random] Server Arena\t{B4B5B7}Any Zone\n");

    for(new i = 0; i < MAX_TOURNEY_SERVER_ARENAS; i++)
    {
        new sizeName[16];
        switch(arenaData[i][category])
        {
            case ARENA_CATEGORY_SMALL: strcopy(sizeName, "{23C552}Small");
            case ARENA_CATEGORY_MEDIUM: strcopy(sizeName, "{FFD700}Medium");
            case ARENA_CATEGORY_BIG: strcopy(sizeName, "{FF6347}Big");
            default: strcopy(sizeName, "{FFFFFF}Normal");
        }
        strcat(body, fmat_return("{B4B5B7}#%02d\t{FFFFFF}%s\t%s\n", i + 1, arenaData[i][arenaName], sizeName));
    }

    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ARENA_SERVER<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

static stock ShowTourneyCustomArenasDialog(playerid)
{
    new title[64] = "{MAIN}Select Custom Map";

    format_MainColor(playerid, title);
    new body[1024];
    format(body, sizeof(body), "{MAIN}ID\t{MAIN}Custom Map Name\t{MAIN}Environment\n");
    strcat(body, "{23C552}#00\t{23C552}[Random] Custom Map\t{B4B5B7}Any Custom Map\n");
    strcat(body, "{B4B5B7}#01\t{FFFFFF}Chinese Garden\t{B4B5B7}Asian Garden & Bridge\n");
    strcat(body, "{B4B5B7}#02\t{FFFFFF}Desert\t{B4B5B7}Desert Military Compound\n");
    strcat(body, "{B4B5B7}#03\t{FFFFFF}Egypt\t{B4B5B7}Pyramid & Historic Ruins\n");
    strcat(body, "{B4B5B7}#04\t{FFFFFF}Urbex\t{B4B5B7}Industrial Abandoned Ruins\n");
    strcat(body, "{B4B5B7}#05\t{FFFFFF}Zoo\t{B4B5B7}Urban Tactical Zoo Arena\n");

    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ARENA_CUSTOM<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

static stock ShowTourneyManageDialog(playerid)
{
    new title[64] = "{MAIN}Manage Tournament Participants";

    new body[2048];
    format(body, sizeof(body), "{MAIN}Slot\t{MAIN}Participant\t{MAIN}Status\n");

    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(g_TourneyParticipants[i][tp_Registered])
        {
            new dName[64];
            GetParticipantDisplayName(i, dName, sizeof(dName));
            strcat(body, fmat_return("{B4B5B7}#%02d\t{FFFFFF}%s\t{FF6347}[Click to Kick]\n", i + 1, dName));
        }
        else
        {
            strcat(body, fmat_return("{BEBEBE}#%02d\t{808080}Empty Slot\t{808080}Available\n", i + 1));
        }
    }

    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_MANAGE<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

stock ShowTournamentAdminMenu(playerid)
{
    if(!IsPlayerLogged(playerid) || User[playerid][adminLevel] < 4) return 0;

    new title[64] = "{MAIN}Tournament Control Panel";

    new body[2048];
    format(body, sizeof(body), "{MAIN}Setting\t{MAIN}Current Value\n");

    // 0: Status
    new statusStr[64];
    switch(g_TourneyData[t_State])
    {
        case TOURNAMENT_STATE_NONE: strcopy(statusStr, "{808080}Inactive / Idle");
        case TOURNAMENT_STATE_REGISTRATION: format(statusStr, sizeof(statusStr), "{23C552}Open ({FFFFFF}%d/%d registered{23C552})", g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        case TOURNAMENT_STATE_STARTING: strcopy(statusStr, "{FFFF00}Starting soon...");
        case TOURNAMENT_STATE_IN_PROGRESS: format(statusStr, sizeof(statusStr), "{00B6F6}In Progress (Round %d/%d)", g_TourneyData[t_CurrentRound], g_TourneyData[t_TotalRounds]);
        case TOURNAMENT_STATE_FINISHED: format(statusStr, sizeof(statusStr), "{FFD700}Finished (Winner: %s)", g_TourneyData[t_WinnerName]);
    }
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Tournament Status\t%s\n", statusStr));

    // 1: Tournament Name
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Tournament Name\t{00B6F6}%s\n", g_TourneyData[t_Name]));

    // 2: Modality
    new modeStr[32];
    switch(g_TourneyData[t_Mode])
    {
        case TOURNAMENT_MODE_1V1: strcopy(modeStr, "{FFFFFF}1v1 Elimination");
        case TOURNAMENT_MODE_2V2: strcopy(modeStr, "{FFFFFF}2v2 Team Battle");
        case TOURNAMENT_MODE_CLAN: strcopy(modeStr, "{FFFFFF}Clan War");
    }
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Modality\t%s\n", modeStr));

    // 3: Max Slots
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Bracket Size (Slots)\t{23C552}%d Participants\n", g_TourneyData[t_MaxSlots]));

    // 4: Prize Coins
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Prize (Coins)\t{FFD700}%d Coins\n", g_TourneyData[t_PrizeCoins]));

    // 5: Prize Money
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Prize (Money)\t{23C552}%s\n", FormatMoney(g_TourneyData[t_PrizeMoney])));

    // 6: Prize Achievement / Tag
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Prize (Achievement / Tag)\t{FF4500}%s\n", g_TourneyData[t_AchievementTag]));

    // 7: Entry Fee
    if(g_TourneyData[t_EntryFee] > 0)
        strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Entry Fee\t{FF6347}%s\n", FormatMoney(g_TourneyData[t_EntryFee])));
    else
        strcat(body, "{MAIN}\xBB {B4B5B7}Entry Fee\t{23C552}FREE\n");

    // 8: Weapon Set
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Weapon Loadout\t{FFFFFF}%s\n", g_TourneyWeaponSets[g_TourneyData[t_WeaponSet]][twi_Name]));

    // 9: Armour Config
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Armour Setting\t%s\n", g_TourneyData[t_GiveArmour] ? "{23C552}100% Armour + 100 HP" : "{FF6347}No Armour (100 HP Only)"));

    // 10: Arena Selection
    new arenaDisplay[64];
    GetTourneyArenaDisplayName(g_TourneyData[t_ArenaCategory], g_TourneyData[t_ArenaID], arenaDisplay, sizeof(arenaDisplay));
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Arena Selection\t{00B6F6}%s\n", arenaDisplay));

    // 11: Match Format
    new matchFmtStr[32];
    if(g_TourneyData[t_RoundsPerMatch] == 1) strcopy(matchFmtStr, "Best of 1 (1 Round)");
    else if(g_TourneyData[t_RoundsPerMatch] == 3) strcopy(matchFmtStr, "Best of 3 (Bo3)");
    else format(matchFmtStr, sizeof(matchFmtStr), "Best of %d (Bo%d)", g_TourneyData[t_RoundsPerMatch], g_TourneyData[t_RoundsPerMatch]);
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Match Format\t{FFFFFF}%s\n", matchFmtStr));

    // 12: Round Time Limit
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Round Time Limit\t{FFFFFF}%d seconds\n", g_TourneyData[t_RoundTimeLimit]));

    // 13: Event Date & Time (Schedule)
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Event Date & Time\t{00B6F6}%s\n", g_TourneyData[t_Schedule]));

    // 14: Discord Requirement
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Discord App Requirement\t%s\n", g_TourneyData[t_RequireDiscord] ? "{23C552}Required" : "{FF6347}Optional (Direct In-Game)"));

    // 15: Auto-Start
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Auto-Start on Full\t%s\n", g_TourneyData[t_AutoStart] ? "{23C552}[Enabled]" : "{FF6347}[Disabled]"));

    // 16: Manage Participants
    strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}Manage Participants\t{00B6F6}View & Kick {B4B5B7}({FFFFFF}%d/%d{B4B5B7})\n", g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]));

    // Action buttons based on state
    if(g_TourneyData[t_State] == TOURNAMENT_STATE_NONE || g_TourneyData[t_State] == TOURNAMENT_STATE_FINISHED)
    {
        strcat(body, "{MAIN}\xBB {23C552}Start Registration\t{BEBEBE}Open tournament registrations\n");
    }
    else if(g_TourneyData[t_State] == TOURNAMENT_STATE_REGISTRATION)
    {
        strcat(body, "{MAIN}\xBB {FFD700}Force Start Now\t{BEBEBE}Start tournament with registered players\n");
        strcat(body, "{MAIN}\xBB {FF6347}Cancel Tournament\t{BEBEBE}Refund players and cancel event\n");
    }
    else
    {
        strcat(body, "{MAIN}\xBB {00B6F6}View Live Bracket\t{BEBEBE}View live matches & bracket\n");
        strcat(body, "{MAIN}\xBB {FF6347}Cancel Tournament\t{BEBEBE}Force terminate active tournament\n");
    }

    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ADMIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Close");
}

public DIALOG_TOURNEY_ADMIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;

    switch(listitem)
    {
        case 0: // Status
        {
            return ShowTournamentAdminMenu(playerid);
        }
        case 1: // Tournament Name
        {
            new title[64] = "{MAIN}Set Tournament Name";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_NAME<iiiis>, DIALOG_STYLE_INPUT, title, "{FFFFFF}Enter the custom name for this tournament:\n{BEBEBE}(Example: Rejecting Summer Championship 2026, 1v1 Deagle Masters)", "Set", "Back");
        }
        case 2: // Modality
        {
            if(g_TourneyData[t_State] != TOURNAMENT_STATE_NONE)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Cannot change modality while a tournament is active!");
                return ShowTournamentAdminMenu(playerid);
            }
            new title[64] = "{MAIN}Select Modality";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_MODE<iiiis>, DIALOG_STYLE_LIST, title, "1v1 Elimination\n2v2 Team Battle\nClan War", "Select", "Back");
        }
        case 3: // Slots
        {
            if(g_TourneyData[t_State] != TOURNAMENT_STATE_NONE)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Cannot change slots while a tournament is active!");
                return ShowTournamentAdminMenu(playerid);
            }
            new title[64] = "{MAIN}Select Bracket Size";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_SLOTS<iiiis>, DIALOG_STYLE_LIST, title, "2 Participants (1 Round)\n4 Participants (2 Rounds)\n8 Participants (3 Rounds)\n16 Participants (4 Rounds)\n32 Participants (5 Rounds)", "Select", "Back");
        }
        case 4: // Prize Coins
        {
            new title[64] = "{MAIN}Set Prize (Coins)";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_PRIZE_COINS<iiiis>, DIALOG_STYLE_INPUT, title, "{FFFFFF}Enter the amount of {FFD700}Coins {FFFFFF}for the 1st place winner:", "Set", "Back");
        }
        case 5: // Prize Money
        {
            new title[64] = "{MAIN}Set Prize (Money)";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_PRIZE_MONEY<iiiis>, DIALOG_STYLE_INPUT, title, "{FFFFFF}Enter the in-game cash prize ($) for the 1st place winner:", "Set", "Back");
        }
        case 6: // Prize Achievement / Tag
        {
            new title[64] = "{MAIN}Set Winner Achievement / Tag";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ACHIEVEMENT<iiiis>, DIALOG_STYLE_INPUT, title, "{FFFFFF}Enter the custom Achievement / Tag name that the Champion will receive:\n{BEBEBE}(Example: CHAMPION, Summer 2026 Champion, 1v1 Deagle Master)", "Set", "Back");
        }
        case 7: // Entry Fee
        {
            if(g_TourneyData[t_State] != TOURNAMENT_STATE_NONE)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Cannot change entry fee while a tournament is active!");
                return ShowTournamentAdminMenu(playerid);
            }
            new title[64] = "{MAIN}Set Entry Fee";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ENTRY_FEE<iiiis>, DIALOG_STYLE_INPUT, title, "{FFFFFF}Enter the registration fee per player (0 for FREE):", "Set", "Back");
        }
        case 8: // Weapon Set
        {
            new title[64] = "{MAIN}Select Weapon Set";
            format_MainColor(playerid, title);
            new str[512];
            for(new i = 0; i < sizeof(g_TourneyWeaponSets); i++)
            {
                strcat(str, fmat_return("%s\n", g_TourneyWeaponSets[i][twi_Name]));
            }
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_WEAPONS<iiiis>, DIALOG_STYLE_LIST, title, str, "Select", "Back");
        }
        case 9: // Armour Toggle
        {
            g_TourneyData[t_GiveArmour] = !g_TourneyData[t_GiveArmour];
            return ShowTournamentAdminMenu(playerid);
        }
        case 10: // Arena Selection (Category Dialog)
        {
            new title[64] = "{MAIN}Select Arena Category";

            new body[1024];
            format(body, sizeof(body), "{MAIN}Category\t{MAIN}Description\n");
            strcat(body, "{MAIN}\xBB {B4B5B7}[Random] All Categories\t{FFFFFF}Picks from Duel, Server & Custom\n");
            strcat(body, "{MAIN}\xBB {B4B5B7}[Duel] Arenas (21 Arenas)\t{FFFFFF}Apartment LS, Grove, Warehouse, etc.\n");
            strcat(body, "{MAIN}\xBB {B4B5B7}[Server] Arenas (68 Arenas)\t{FFFFFF}Vinewood, Ganton, Las Brujas, etc.\n");
            strcat(body, "{MAIN}\xBB {B4B5B7}[Custom] Arenas (5 Maps)\t{FFFFFF}Chinese Garden, Desert, Egypt, Urbex, Zoo\n");

            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ARENA_CAT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        }
        case 11: // Match Format (Rounds)
        {
            new title[64] = "{MAIN}Select Match Format";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_ROUNDS<iiiis>, DIALOG_STYLE_LIST, title, "Best of 1 (1 Round)\nBest of 3 (Bo3)\nBest of 5 (Bo5)", "Select", "Back");
        }
        case 12: // Round Time Limit
        {
            new title[64] = "{MAIN}Set Round Time Limit";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_TIME<iiiis>, DIALOG_STYLE_LIST, title, "30 seconds\n60 seconds (1 minute)\n90 seconds (1.5 minutes)\n120 seconds (2 minutes)\n180 seconds (3 minutes)\n300 seconds (5 minutes)", "Select", "Back");
        }
        case 13: // Event Date & Time (Schedule)
        {
            new title[64] = "{MAIN}Set Event Schedule";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_SCHEDULE<iiiis>, DIALOG_STYLE_LIST, title,
                "Now (Immediate Start)\nIn 15 minutes\nIn 30 minutes\nIn 1 hour\nToday at 19:00 (UTC-3)\nToday at 20:00 (UTC-3)\nToday at 21:00 (UTC-3)\nTomorrow at 19:00 (UTC-3)\nTomorrow at 20:00 (UTC-3)\nTomorrow at 21:00 (UTC-3)\nSaturday at 20:00 (UTC-3)\nSunday at 20:00 (UTC-3)\nCustom Date & Time (Type below)...",
                "Select", "Back");
        }
        case 14: // Discord Requirement Toggle
        {
            g_TourneyData[t_RequireDiscord] = !g_TourneyData[t_RequireDiscord];
            return ShowTournamentAdminMenu(playerid);
        }
        case 15: // Auto-Start Toggle
        {
            g_TourneyData[t_AutoStart] = !g_TourneyData[t_AutoStart];
            return ShowTournamentAdminMenu(playerid);
        }
        case 16: // Manage Participants
        {
            return ShowTourneyManageDialog(playerid);
        }
        case 17: // Action button 1
        {
            if(g_TourneyData[t_State] == TOURNAMENT_STATE_NONE || g_TourneyData[t_State] == TOURNAMENT_STATE_FINISHED)
            {
                StartTournamentRegistration(playerid);
                return ShowTournamentAdminMenu(playerid);
            }
            else if(g_TourneyData[t_State] == TOURNAMENT_STATE_REGISTRATION)
            {
                if(g_TourneyData[t_RegisteredCount] < 2)
                {
                    SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}At least 2 participants are required to start!");
                    return ShowTournamentAdminMenu(playerid);
                }
                g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
                StartTournamentCountdown(0);
                return 1;
            }
            else
            {
                return ShowTourneyBracketDialog(playerid);
            }
        }
        case 18: // Action button 2 (Cancel)
        {
            CancelTournament(playerid, "Cancelled from Admin Menu");
            return 1;
        }
    }
    return 1;
}

public DIALOG_TOURNEY_NAME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !IsNull(inputtext))
    {
        strcopy(g_TourneyData[t_Name], inputtext, sizeof(g_TourneyData[t_Name]));
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ACHIEVEMENT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !IsNull(inputtext))
    {
        new cleanTag[32];
        format(cleanTag, sizeof(cleanTag), "%s", inputtext);
        if(cleanTag[0] == '[') strdel(cleanTag, 0, 1);
        new len = strlen(cleanTag);
        if(len > 0 && cleanTag[len - 1] == ']') cleanTag[len - 1] = '\0';
        strcopy(g_TourneyData[t_AchievementTag], cleanTag, sizeof(g_TourneyData[t_AchievementTag]));
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_MODE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        g_TourneyData[t_Mode] = listitem + 1;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_SLOTS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0: g_TourneyData[t_MaxSlots] = 2;
            case 1: g_TourneyData[t_MaxSlots] = 4;
            case 2: g_TourneyData[t_MaxSlots] = 8;
            case 3: g_TourneyData[t_MaxSlots] = 16;
            case 4: g_TourneyData[t_MaxSlots] = 32;
        }
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_PRIZE_COINS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new coins = strval(inputtext);
        if(coins < 0) coins = 0;
        if(coins > 100000) coins = 100000;
        g_TourneyData[t_PrizeCoins] = coins;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_PRIZE_MONEY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new money = strval(inputtext);
        if(money < 0) money = 0;
        g_TourneyData[t_PrizeMoney] = money;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ENTRY_FEE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new fee = strval(inputtext);
        if(fee < 0) fee = 0;
        g_TourneyData[t_EntryFee] = fee;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_WEAPONS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && 0 <= listitem < sizeof(g_TourneyWeaponSets))
    {
        g_TourneyData[t_WeaponSet] = listitem;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ARENA_CAT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTournamentAdminMenu(playerid);

    switch(listitem)
    {
        case 0: // Random from Any Category
        {
            g_TourneyData[t_ArenaCategory] = TOURNEY_ARENA_CAT_RANDOM;
            g_TourneyData[t_ArenaID] = -1;
            return ShowTournamentAdminMenu(playerid);
        }
        case 1: // Duel Arenas
        {
            return ShowTourneyDuelArenasDialog(playerid);
        }
        case 2: // Server Arenas
        {
            return ShowTourneyServerArenasDialog(playerid);
        }
        case 3: // Custom Maps
        {
            return ShowTourneyCustomArenasDialog(playerid);
        }
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ARENA_DUEL(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTournamentAdminMenu(playerid);

    g_TourneyData[t_ArenaCategory] = TOURNEY_ARENA_CAT_DUEL;
    if(listitem == 0)
    {
        g_TourneyData[t_ArenaID] = -1; // Random Duel
    }
    else
    {
        g_TourneyData[t_ArenaID] = listitem - 1;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ARENA_SERVER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTournamentAdminMenu(playerid);

    g_TourneyData[t_ArenaCategory] = TOURNEY_ARENA_CAT_SERVER;
    if(listitem == 0)
    {
        g_TourneyData[t_ArenaID] = -1; // Random Server
    }
    else
    {
        g_TourneyData[t_ArenaID] = listitem - 1;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ARENA_CUSTOM(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTournamentAdminMenu(playerid);

    g_TourneyData[t_ArenaCategory] = TOURNEY_ARENA_CAT_CUSTOM;
    if(listitem == 0)
    {
        g_TourneyData[t_ArenaID] = -1; // Random Custom
    }
    else
    {
        g_TourneyData[t_ArenaID] = listitem - 1;
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_ROUNDS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0: g_TourneyData[t_RoundsPerMatch] = 1;
            case 1: g_TourneyData[t_RoundsPerMatch] = 3;
            case 2: g_TourneyData[t_RoundsPerMatch] = 5;
        }
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_TIME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0: g_TourneyData[t_RoundTimeLimit] = 30;
            case 1: g_TourneyData[t_RoundTimeLimit] = 60;
            case 2: g_TourneyData[t_RoundTimeLimit] = 90;
            case 3: g_TourneyData[t_RoundTimeLimit] = 120;
            case 4: g_TourneyData[t_RoundTimeLimit] = 180;
            case 5: g_TourneyData[t_RoundTimeLimit] = 300;
        }
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_SCHEDULE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTournamentAdminMenu(playerid);

    switch(listitem)
    {
        case 0: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Now (Immediate Start)");
        case 1: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "In 15 minutes");
        case 2: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "In 30 minutes");
        case 3: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "In 1 hour");
        case 4: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Today at 19:00 (UTC-3)");
        case 5: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Today at 20:00 (UTC-3)");
        case 6: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Today at 21:00 (UTC-3)");
        case 7: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Tomorrow at 19:00 (UTC-3)");
        case 8: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Tomorrow at 20:00 (UTC-3)");
        case 9: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Tomorrow at 21:00 (UTC-3)");
        case 10: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Saturday at 20:00 (UTC-3)");
        case 11: format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Sunday at 20:00 (UTC-3)");
        case 12:
        {
            new title[64] = "{MAIN}Set Custom Date & Time";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_SCHEDULE_CUSTOM<iiiis>, DIALOG_STYLE_INPUT, title,
                "{FFFFFF}Enter the custom date and time for the tournament:\n{BEBEBE}(Example: 20/08/2026 at 20:30 (UTC-3) or Friday 21:00)", "Set", "Back");
        }
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_SCHEDULE_CUSTOM(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && !IsNull(inputtext))
    {
        strcopy(g_TourneyData[t_Schedule], inputtext, sizeof(g_TourneyData[t_Schedule]));
    }
    return ShowTournamentAdminMenu(playerid);
}

public DIALOG_TOURNEY_MANAGE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTournamentAdminMenu(playerid);

    if(listitem < 0 || listitem >= g_TourneyData[t_MaxSlots]) return ShowTourneyManageDialog(playerid);

    if(!g_TourneyParticipants[listitem][tp_Registered])
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}This slot is empty.");
        return ShowTourneyManageDialog(playerid);
    }

    SetPVarInt(playerid, "tourney_kick_slot", listitem);

    new dName[64];
    GetParticipantDisplayName(listitem, dName, sizeof(dName));

    new title[64] = "{MAIN}Kick Participant";

    format_MainColor(playerid, title);
    new body[256];
    format(body, sizeof(body), "{FFFFFF}Are you sure you want to kick {FF6347}%s {FFFFFF}from the tournament?\n{BEBEBE}Their registration fee will be refunded.", dName);

    return Dialog_ShowCallback(playerid, using public DIALOG_TOURNEY_KICK_CONFIRM<iiiis>, DIALOG_STYLE_MSGBOX, title, body, "Kick", "Cancel");
}

public DIALOG_TOURNEY_KICK_CONFIRM(playerid, dialogid, response, listitem, string:inputtext[])
{
    new slot = GetPVarInt(playerid, "tourney_kick_slot");
    DeletePVar(playerid, "tourney_kick_slot");

    if(response && slot >= 0 && slot < MAX_TOURNEY_SLOTS && g_TourneyParticipants[slot][tp_Registered])
    {
        KickTourneyParticipant(slot, playerid);
    }
    return ShowTourneyManageDialog(playerid);
}

stock ShowTourneyBracketDialog(playerid)
{
    if(!IsPlayerLogged(playerid)) return 0;

    new title[64];
    format(title, sizeof(title), "{MAIN}%s {B4B5B7}- Bracket", g_TourneyData[t_Name]);

    format_MainColor(playerid, title);
    if(g_TourneyData[t_State] == TOURNAMENT_STATE_NONE)
    {
        new noTourneyMsg[256] = "{FFFFFF}There is no active tournament right now.\nStaff members can open one with {MAIN}/tournament{FFFFFF}.";
        format_MainColor(playerid, noTourneyMsg, sizeof(noTourneyMsg));
        return Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, title, noTourneyMsg, "Close", "");
    }

    if(g_TourneyData[t_State] == TOURNAMENT_STATE_REGISTRATION)
    {
        new tourneyArenaName[64];
        GetTourneyArenaDisplayName(g_TourneyData[t_ArenaCategory], g_TourneyData[t_ArenaID], tourneyArenaName, sizeof(tourneyArenaName));

        new regInfo[1024];
        format(regInfo, sizeof(regInfo),
            "{FFFFFF}Tournament is currently in {23C552}REGISTRATION PHASE{FFFFFF}!\n\n"\
            "{MAIN}\xBB {B4B5B7}Tournament:\t{00B6F6}%s\n"\
            "{MAIN}\xBB {B4B5B7}Event Date & Time:\t{00B6F6}%s\n"\
            "{MAIN}\xBB {B4B5B7}Registered Participants:\t{FFFFFF}%d / %d\n"\
            "{MAIN}\xBB {B4B5B7}Modality:\t{FFFFFF}%s\n"\
            "{MAIN}\xBB {B4B5B7}Prize:\t{FFD700}%d Coins {B4B5B7}+ {23C552}%s\n"\
            "{MAIN}\xBB {B4B5B7}Winner Achievement:\t{FF4500}%s\n"\
            "{MAIN}\xBB {B4B5B7}Entry Fee:\t{FFFFFF}%s\n"\
            "{MAIN}\xBB {B4B5B7}Weapon Loadout:\t{FFFFFF}%s\n"\
            "{MAIN}\xBB {B4B5B7}Selected Arena:\t{00B6F6}%s\n"\
            "{MAIN}\xBB {B4B5B7}Armour:\t{FFFFFF}%s\n\n"\
            "{B4B5B7}Type {MAIN}/jointournament {B4B5B7}to enter now!",
            g_TourneyData[t_Name],
            g_TourneyData[t_Schedule],
            g_TourneyData[t_RegisteredCount],
            g_TourneyData[t_MaxSlots],
            (g_TourneyData[t_Mode] == TOURNAMENT_MODE_1V1) ? "1v1 Elimination" : (g_TourneyData[t_Mode] == TOURNAMENT_MODE_2V2 ? "2v2 Team Battle" : "Clan War"),
            g_TourneyData[t_PrizeCoins],
            FormatMoney(g_TourneyData[t_PrizeMoney]),
            g_TourneyData[t_AchievementTag],
            (g_TourneyData[t_EntryFee] > 0) ? FormatMoney(g_TourneyData[t_EntryFee]) : "FREE",
            g_TourneyWeaponSets[g_TourneyData[t_WeaponSet]][twi_Name],
            tourneyArenaName,
            g_TourneyData[t_GiveArmour] ? "100% Armour" : "No Armour (100 HP)"
        );
        format_MainColor(playerid, regInfo, sizeof(regInfo));
        return Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, title, regInfo, "Close", "");
    }

    new bracketStr[4096];
    strcat(bracketStr, "{MAIN}Round\t{MAIN}Match\t{MAIN}Team A\t{MAIN}Team B\t{MAIN}Status\n");

    for(new m = 0; m < MAX_TOURNEY_MATCHES; m++)
    {
        if(g_TourneyMatches[m][tm_Active])
        {
            new nameA[32], nameB[32];
            GetParticipantDisplayName(g_TourneyMatches[m][tm_SlotA], nameA, sizeof(nameA));
            GetParticipantDisplayName(g_TourneyMatches[m][tm_SlotB], nameB, sizeof(nameB));

            new statusStr[32];
            switch(g_TourneyMatches[m][tm_State])
            {
                case MATCH_STATE_PREPARING: strcopy(statusStr, "{FFFF00}Waiting");
                case MATCH_STATE_COUNTDOWN: strcopy(statusStr, "{FFA500}Countdown");
                case MATCH_STATE_ACTIVE: format(statusStr, sizeof(statusStr), "{23C552}Fighting (%d-%d)", g_TourneyMatches[m][tm_ScoreA], g_TourneyMatches[m][tm_ScoreB]);
                case MATCH_STATE_FINISHED:
                {
                    new winName[32];
                    GetParticipantDisplayName(g_TourneyMatches[m][tm_WinnerSlot], winName, sizeof(winName));
                    format(statusStr, sizeof(statusStr), "{00B6F6}Won: %s", winName);
                }
            }

            new roundLabel[16];
            if(g_TourneyMatches[m][tm_Round] == g_TourneyData[t_TotalRounds]) strcopy(roundLabel, "Final");
            else if(g_TourneyMatches[m][tm_Round] == g_TourneyData[t_TotalRounds] - 1) strcopy(roundLabel, "Semi");
            else format(roundLabel, sizeof(roundLabel), "R%d", g_TourneyMatches[m][tm_Round]);

            strcat(bracketStr, fmat_return("{B4B5B7}%s\t{BEBEBE}#%02d\t{FFFFFF}%s\t{FFFFFF}%s\t%s\n",
                roundLabel, m + 1, nameA, nameB, statusStr));
        }
    }

    format_MainColor(playerid, bracketStr);
    return Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, bracketStr, "Close", "");
}

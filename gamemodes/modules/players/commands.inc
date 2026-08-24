
@cmd() id(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new targetid;
    if (sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}/id [id player/ nickname player]");
	if (!IsPlayerConnected(targetid) || !User[targetid][isLogged])
		return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    new pip[18], sampVERSION[24];
    GetPlayerIp(targetid, pip, sizeof(pip));
    GetPlayerVersion(targetid, sampVERSION, sizeof(sampVERSION));

    new strid[224];
    format(strid, sizeof(strid), "{MAIN}[INFO] {FFFFFF}%s [%d] - {FFFFFF}PING: %d  PL: %.2f  FPS: %d  SA:MP Version: %s  %s",
        getPlayerName(targetid),
        targetid,
        GetPlayerPing(targetid),
        NetStats_PacketLossPercent(targetid),
        User[targetid][FPS],
        sampVERSION,
        (User[playerid][adminLevel] > 0) ? (fmat_return("{EC0D0D}IP:{FFFFFF} %s", pip)) : "" );
    SendClientMessage(playerid, -1, "%s", strid);
    return true;
}


@cmd() showagain(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new location = User[playerid][locationId];
    if(location < 0 || location >= MAX_LOCATIONS)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be in an arena to see the last round results.");

    for(new a = 0; a <= 5; a++)
        PlayerTextDrawShow(playerid, topLocationTable[playerid][a]);
    for(new a = 7; a <= 9; a++)
        PlayerTextDrawShow(playerid, topLocationTable[playerid][a]);

    new base = (location * 20) + 26;
    for(new b = 0; b <= 4; b++)
        PlayerTextDrawShow(playerid, topLocationTable[playerid][base + b]);

    SetPVarInt(playerid, "isShowingTableTD", 1);
	return true;
}

StartChangeMode(playerid)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

	if(User[playerid][isSpectating] || playerSpecID[playerid] != INVALID_PLAYER_ID)
		return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

	if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE && GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_WAITING)
		return SendClientMessage(playerid, -1, "{MAIN}[DUEL] {FFFFFF}You cannot change modes while in a duel session.");

	if(IsPlayerInDuel2v2(playerid))
		return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot change modes while in a 2v2 duel. Use /cancel2v2 first.");

	if(IsPlayerInTournamentMatch(playerid))
		return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot change modes while playing a tournament match.");

	if (g_PlayerTraining[playerid][pTrain_Active])
	{
		ExitTrainingArena(playerid);
	}

	if (g_Practice[playerid][pracActive])
	{
		CallLocalFunction("StopPracticeSession", "i", playerid);
	}

    new Float:hp;
    GetPlayerHealth(playerid, hp);

    if(hp < 100.0)
        return SendClientMessage(playerid, -1, "%s", LOW_HEALTH_ERROR);

    if(gettime() < User[playerid][lastDamageTime])
        return SendClientMessage(playerid, -1, "%s", RECENT_DAMAGE_ERROR);

    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, 0xa4aba7ff, "%s", IN_VEHICLE_ERROR);

    if(User[playerid][locationId] != -1)
    {
        GangZoneStopPulseForPlayer(playerid, e_ZONE_PULSE_ZONE, arenaInfo[User[playerid][locationId]][arenaId]);
        User[playerid][lastLocation] = User[playerid][locationId];
    }

    User[playerid][locationId] = -1;
    User[playerid][isAlreadySpawned] = false;
    SetPlayerInterior(playerid, 0);
    SetPlayerPos(playerid, -1504.1549, 595.7162, 42.6563);
    SetPlayerFacingAngle(playerid, 200.5717);
    InterpolateCameraPos(playerid, -1489.660888, 556.384155, 47.324314, -1502.70, 590.56, 42.74, 4000);
    InterpolateCameraLookAt(playerid, -1486.176635, 559.963378, 47.101730, -1502.34, 596.55, 42.93, 4000);

    showLocationSelectDialog(playerid);
    return true;
}

ShowRankingsMenu(playerid)
{
    new empty[1];
    return DIALOG_RANK1(playerid, 0, 1, 0, empty);
}

@cmd() changemode(playerid, params[], help)
{
    return StartChangeMode(playerid);
}

forward ShowStats(playerid, targetid);
forward LookupOfflineStats(playerid, const targetName[]);

@cmd() fullstats(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsNull(params))
    {
        ShowStats(playerid, playerid);
        return true;
    }

    new targetid;
    if(!sscanf(params, "u", targetid) && IsPlayerConnected(targetid) && User[targetid][isLogged])
    {
        ShowStats(playerid, targetid);
        return true;
    }

    LookupOfflineStats(playerid, params);
    return true;
}

@cmd() stats(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsNull(params))
    {
        ShowStats(playerid, playerid);
        return true;
    }

    new targetid;
    if(!sscanf(params, "u", targetid) && IsPlayerConnected(targetid) && User[targetid][isLogged])
    {
        ShowStats(playerid, targetid);
        return true;
    }

    LookupOfflineStats(playerid, params);
    return true;
}

@cmd() menu(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ShowMenu(playerid);
    return true;
}

@cmd(.alts = {"w"}) weather(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsNull(params)) return SendClientMessage(playerid, -1, "%s", WEATHER_HELP);
    if(!IsNumeric(params)) return SendClientMessage(playerid, COLOR_BLUE, "%s", IS_NOT_NUMERIC);

    User[playerid][weather] = strval(params);
    ChangeWeather(playerid, User[playerid][weather]);
    return true;
}

@cmd(.alts = {"t"}) time(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsNull(params)) return SendClientMessage(playerid, -1, "%s", TIME_HELP);
    if(!IsNumeric(params)) return SendClientMessage(playerid,COLOR_BLUE, "%s", IS_NOT_NUMERIC);

    User[playerid][time] = strval(params);
    ChangeTime(playerid, User[playerid][time]);
    return true;
}


@cmd() gunmenu(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot use /gunmenu in a 2v2 duel.");

    if(IsPlayerInTournamentMatch(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot use /gunmenu during a tournament match.");

    new bool:isLoungeOrAfk = (User[playerid][isAfk] || User[playerid][locationId] == -1 || User[playerid][locationId] == ARENA_AFK);
    if(!isLoungeOrAfk)
    {
        if(gettime() - User[playerid][timerRespawn] > 15)
        {
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You can only use /gunmenu in the first 15 seconds after spawning!");
        }
    }

    DeletePVar(playerid, "gunmenu_from_personalization");
	ShowGunMenu(playerid);
    return true;
}

@cmd() spec(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE && GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_WAITING)
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL] {FFFFFF}You cannot spectate while in a duel.");

    if(IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot spectate while in a 2v2 duel.");

    if(IsPlayerInTournamentMatch(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot spectate while playing a tournament match.");

    if(g_PlayerTraining[playerid][pTrain_Active])
        return SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot spectate while in training mode.");

    if(g_Practice[playerid][pracActive])
        return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}You cannot spectate while in practice mode.");

    new PLAYER_STATE:playerState = GetPlayerState(playerid);

    if(playerState != PLAYER_STATE_SPECTATING && User[playerid][adminLevel] < 1)
    {
        if(User[playerid][isAfk] == false)
            return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);
    }

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "%s", SPEC_HELP);

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(User[targetid][isAfk] && User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", TARGET_AFK);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "%s", SPEC_HELP2);

    setPlayerSpectate(playerid, targetid);
    SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    return true;
}

@cmd() specoff(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(User[playerid][isSpectating] == false)
        return SendClientMessage(playerid, -1, "%s", NO_SPEC_MODE);

    StopSpectate(playerid);
    return true;
}

@cmd(.alts = {"re"}) respawn(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE && GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_WAITING)
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL] {FFFFFF}You cannot use /respawn while in a duel.");

    if(IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot use /respawn while in a 2v2 duel.");

    if(IsPlayerInTournamentMatch(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot use /respawn while playing a tournament match.");

    if(g_PlayerTraining[playerid][pTrain_Active])
        return SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot use /respawn while in training mode.");

    if(g_Practice[playerid][pracActive])
        return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}You cannot use /respawn while in practice mode.");

    new timestampRespawn = gettime();
    Respawn(playerid, timestampRespawn);
    return true;
}

@cmd(.alts = {"help", "commands"}) cmds(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    return ShowHelpMainMenu(playerid);
}

ShowHelpMainMenu(playerid)
{
    new helpStr[1024], title[64];
    format(title, sizeof(title), "{MAIN}Server Guide & Help");
    format_MainColor(playerid, title);

    format(helpStr, sizeof(helpStr), "{MAIN}Category / Option\t{MAIN}Description\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}My Account\t{FFFFFF}View account info, statistics & settings\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}Security\t{FFFFFF}Account security, password & Discord link\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}Changemode\t{FFFFFF}Select an arena or change game mode\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}Training Mode\t{FFFFFF}Guide, commands & features of Training System\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}VIP System\t{FFFFFF}View VIP benefits & commands\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}Rankings\t{FFFFFF}View top 10 player & clan leaderboards\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}Rules\t{FFFFFF}Read official server rules & guidelines\n");
    strcat(helpStr, "{MAIN}\xBB {B4B5B7}Commands\t{FFFFFF}View full list of all server commands");
    format_MainColor(playerid, helpStr);

    Dialog_ShowCallback(playerid, using public DIALOG_HELP_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, helpStr, "Select", "Close");
    return true;
}

#define CMD_CATEGORIES (7)

new const g_CmdCategories[CMD_CATEGORIES][] = {
    "General & Navigation",
    "Customization & Animations",
    "Lounge & Vehicles",
    "Clan System",
    "Duels & Arenas",
    "Social, Economy & Staff",
    "Account, VIP & Discord"
};

ShowPlayerCommands(playerid)
{
    new body[512], title[64];
    format(title, sizeof(title), "{MAIN}Server Commands");
    format_MainColor(playerid, title);

    strcat(body, "{MAIN}Category\t{MAIN}Open\n", sizeof(body));

    for(new i = 0; i < CMD_CATEGORIES; i++)
        strcat(body, fmat_return("{MAIN}\xBB {B4B5B7}%s\t{FFFFFF}\xBB\n", g_CmdCategories[i]), sizeof(body));

    format_MainColor(playerid, body);

    Dialog_ShowCallback(playerid, using public DIALOG_PLAYERCMDS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

forward DIALOG_PLAYERCMDS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PLAYERCMDS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowHelpMainMenu(playerid);
    if(listitem < 0 || listitem >= CMD_CATEGORIES) return ShowPlayerCommands(playerid);

    return ShowCommandCategory(playerid, listitem);
}

forward DIALOG_CMDCATEGORY(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_CMDCATEGORY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response) return ShowPlayerCommands(playerid);
    return 1;
}

ShowCommandCategory(playerid, catid)
{
    if(catid < 0 || catid >= CMD_CATEGORIES) return 0;

    new cmdsstr[2560], title[80];
    format(title, sizeof(title), "{MAIN}%s", g_CmdCategories[catid]);
    format_MainColor(playerid, title);

    strcat(cmdsstr, "{MAIN}Command\t{MAIN}Description\n");

    switch(catid)
    {
        case 0:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/changemode\t{FFFFFF}Select arena or change game mode\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/menu\t{FFFFFF}Show player settings menu\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/l (/lounge, /afk)\t{FFFFFF}Teleport to the lounge room\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/respawn (/re)\t{FFFFFF}Respawn your character\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/gunmenu\t{FFFFFF}Open weapon selection menu in arena\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/spec\t{FFFFFF}Spectate a player\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/specoff\t{FFFFFF}Stop spectating\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/stats [id/nick]\t{FFFFFF}View online or offline player stats\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/fullstats [id/nick]\t{FFFFFF}View detailed player stats (online/offline)\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/ach (/achievements)\t{FFFFFF}View your achievements & progress\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/coins\t{FFFFFF}Balance, buy Coins, history & VIP\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/logcoins\t{FFFFFF}Shortcut to your last 15 Coins transactions\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/buycoins [amt]\t{FFFFFF}Exchange in-game money for Coins\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/customhud (/hudstyle)\t{FFFFFF}Change Server HUD style\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/sync\t{FFFFFF}Resync character position\n");
        }
        case 1:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/skin (/ss)\t{FFFFFF}Change your character skin\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/fight (/fightstyle)\t{FFFFFF}Change your fighting style\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/weather (/w)\t{FFFFFF}Change your local weather\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/time (/t)\t{FFFFFF}Change your local time\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/dance\t{FFFFFF}Perform dance animations\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/piss\t{FFFFFF}Perform pee animation\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/stopanim\t{FFFFFF}Stop current animation\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/flash\t{FFFFFF}Toggle hitmark flash effect\n");
        }
        case 2:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/car\t{FFFFFF}Spawn vehicle in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/bike (/b)\t{FFFFFF}Spawn bicycle in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/jetpack\t{FFFFFF}Get jetpack in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/tune\t{FFFFFF}Tune vehicle in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/flip\t{FFFFFF}Flip vehicle in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/fix\t{FFFFFF}Repair vehicle in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/getgun\t{FFFFFF}Get weapons in Lounge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/changelounge (/msp)\t{FFFFFF}Change Lounge spawn location\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/lcmds\t{FFFFFF}List all Lounge commands\n");
        }
        case 3:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/clan\t{FFFFFF}Open Clan management menu\n");
            strcat(cmdsstr, fmat_return("{FFFFFF}\x95 {B4B5B7}/createclan\t{FFFFFF}Create a new Clan (%s)\n", FormatMoney(1000000)));
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/invite [id]\t{FFFFFF}Invite a player to your Clan\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/accept\t{FFFFFF}Accept a Clan invitation\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/leaveclan\t{FFFFFF}Leave your current Clan\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/clans (/clanslist)\t{FFFFFF}View global Clans ranking list\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/territories\t{FFFFFF}View Outpost & territory status\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/clancmds\t{FFFFFF}View full list of Clan commands\n");
        }
        case 4:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/duel [id]\t{FFFFFF}Challenge a player to 1v1 duel\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/acceptduel (/goduel)\t{FFFFFF}Accept a duel challenge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/backduel (/leaveduel)\t{FFFFFF}Cancel or leave a 1v1 duel\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/duel2v2 (/create2v2)\t{FFFFFF}Create a 2v2 team duel challenge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/accept2v2 (/go2v2)\t{FFFFFF}Accept a 2v2 team duel challenge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/cancel2v2\t{FFFFFF}Cancel a 2v2 duel challenge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/safemenu (/safe)\t{FFFFFF}Open Clan Safehouse base menu\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/exitsafehouse (/leavesafe)\t{FFFFFF}Exit Clan Safehouse base\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/training\t{FFFFFF}Open training arena & challenges menu\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/rt (/restarttraining)\t{FFFFFF}Restart current training challenge\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/exittraining\t{FFFFFF}Exit current training arena\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/prac (/practice)\t{FFFFFF}Private aim practice with NPC (level 5+)\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/prac pos1 (/prac pos2)\t{FFFFFF}Set the two target movement points\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/prac rec\t{FFFFFF}Record a movement for the NPC to repeat\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/prac exit\t{FFFFFF}Leave the practice world\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/showagain\t{FFFFFF}Show last round results\n");
        }
        case 5:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/id [id/name]\t{FFFFFF}Find player ID or info\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/pm [id] [msg]\t{FFFFFF}Send a private message\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/transfer [id/name] [amt]\t{FFFFFF}Transfer money to a player\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/report [id] [reason]\t{FFFFFF}Report a player to admins\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/ask [question]\t{FFFFFF}Ask a question to online staff\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/votekick [id]\t{FFFFFF}Initiate a votekick poll\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/yes (/no)\t{FFFFFF}Vote YES or NO in active votekick poll\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/admins (/adminlist)\t{FFFFFF}View online administrators\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/ranks (/rank)\t{FFFFFF}View server rankings\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/credits\t{FFFFFF}Show server credits\n");
        }
        case 6:
        {
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/buyvip (/activatevip)\t{FFFFFF}Open VIP package activation menu\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/vip\t{FFFFFF}View VIP perks & status\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/vips\t{FFFFFF}View online VIP members\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/v [message]\t{FFFFFF}Send a message in VIP chat\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/von (/voff)\t{FFFFFF}Turn ON or OFF VIP chat channel\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/vtag\t{FFFFFF}Customize 3D VIP Head Tag\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/tag\t{FFFFFF}Manage tag visibility (VIP, Clan, Admin, Custom)\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/vipcmds\t{FFFFFF}View full list of VIP commands\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/linkdiscord (/discord)\t{FFFFFF}Link your Discord account\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/dcode [code]\t{FFFFFF}Enter verification code received from Discord\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/udiscord\t{FFFFFF}Unlink your Discord account\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/changepass\t{FFFFFF}Change your account password\n");
            strcat(cmdsstr, "{FFFFFF}\x95 {B4B5B7}/changenick\t{FFFFFF}Change your nickname\n");
        }
    }

    format_MainColor(playerid, cmdsstr);

    Dialog_ShowCallback(playerid, using public DIALOG_CMDCATEGORY<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, cmdsstr, "Back", "Close");
    return 1;
}

@cmd(.alts = {"adminlist"}) admins(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(!ServerAdminsCmd && User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This command is currently disabled.");

    new dialogStr[2048];
    strcat(dialogStr, "{FFFFFF}Name\t{FFFFFF}Rank\t{FFFFFF}Location\n");

    new count = 0;
    foreach(new i : Player)
    {
        if(User[i][isLogged] && User[i][adminLevel] > 0 && !User[i][adminCamu] && GetPVarInt(i, "HideTab") != 1)
        {
            new locStr[64];
            new location = User[i][locationId];
            if(location >= 0 && location < MAX_LOCATIONS && location != ARENA_AFK && !User[i][isAfk])
            {
                new arena_idx = arenaInfo[location][arenaId];
                if(arena_idx >= 0 && arena_idx < MAX_ZONES)
                    format(locStr, sizeof(locStr), "{23C552}Arena (%s)", arenaData[arena_idx][arenaName]);
                else
                    format(locStr, sizeof(locStr), "{23C552}Arena");
            }
            else
            {
                format(locStr, sizeof(locStr), "{F84F31}Lounge");
            }

            strcat(dialogStr, fmat_return("{FFFFFF}%s\t{08A3FD}%s\t%s\n",
                getPlayerName(i),
                adminnames[User[i][adminLevel]][ingame], 
                locStr));

            count++;
        }
    }

    if(count == 0)
    {
        new title[64] = "{MAIN}Administrators Online";
        format_MainColor(playerid, title);
        Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, title, "{FFFFFF}There are no administrators online at the moment.", "Close", "");
    }
    else
    {
        new title[64];
        format(title, sizeof(title), "{FFFFFF}Administrators Online {MAIN}[%d]", count);
        format_MainColor(playerid, title);
        Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, dialogStr, "Close", "");
    }

    return true;
}

@cmd(.alts = {"rank"}) ranks(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ShowRankingsMenu(playerid);
    return true;
}

@cmd() sync(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    SyncPlayer(playerid);
    return true;
}

@cmd() credits(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new creditsStr[2048];
    strcat(creditsStr, "{00CED1}Main Information:\n");
    strcat(creditsStr, fmat_return("{FFFFFF}\x95 {08A3FD}GameMode: {FFFFFF}%s %s\n", ServerHostname, SERVER_MODE));
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Developers: {FFFFFF}oldmxa & underline\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Anticheat: {FFFFFF}RAC - oldmxa\n\n");

    strcat(creditsStr, "{00CED1}Special Thanks:\n");
    strcat(creditsStr, fmat_return("{FFFFFF}\x95 {08A3FD}%s{FFFFFF} - For playing with us and helping the server grow!\n", getPlayerName(playerid)));
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Administration Team: {FFFFFF}For helping keep the servers in order\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Community: {FFFFFF}To all players for their continuous support\n\n");

    strcat(creditsStr, "{00CED1}Core & Framework:\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}open.mp: {FFFFFF}open.mp Team & Community\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}YSI Framework: {FFFFFF}Y_Less\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}sscanf2: {FFFFFF}Y_Less\n\n");

    strcat(creditsStr, "{00CED1}Plugins & Networking:\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Streamer Plugin: {FFFFFF}Incognito\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}MySQL: {FFFFFF}BlueG, maddinat0r\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Pawn.RakNet: {FFFFFF}urShadow, Katursis\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}ColAndreas: {FFFFFF}Pottus, Vince, Slice, Kar\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Discord Connector: {FFFFFF}maddinat0r\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Requests: {FFFFFF}Southclaws\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Bcrypt: {FFFFFF}s00p, SgtPez\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Crashdetect: {FFFFFF}Zeex\n\n");

    strcat(creditsStr, "{00CED1}Libraries & Contributions:\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}weapon-config: {FFFFFF}Slice, OscarWos\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}PreviewModelDialog: {FFFFFF}Mellnik\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}zmessage: {FFFFFF}Ziggi\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}md-sort & sort-inline: {FFFFFF}Mergevos\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}strlib: {FFFFFF}Slice\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}a_zone: {FFFFFF}Mellnik, Cu10\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}alpha-fix: {FFFFFF}oscar-broman, Nexius\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}visual-areas: {FFFFFF}Mergevos\n");

    Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "{00CED1}Server Credits", creditsStr, "Close", "");
    return true;
}

@cmd() tag(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;
    ShowTagMenu(playerid);
    return true;
}

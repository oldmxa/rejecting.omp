
@cmd() id(playerid, params[], help)
{

    new targetid;
    if (sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}/id [id player/ nickname player]");
	if (!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    new pip[18], sampVERSION[24];
    GetPlayerIp(targetid, pip, sizeof(pip));
    GetPlayerVersion(targetid, sampVERSION, sizeof(sampVERSION));

    new strid[144];
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
	if(GetPVarInt(playerid, "isSpec") != 0)
		return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

	if (g_PlayerTraining[playerid][pTrain_Active])
	{
		ExitTrainingArena(playerid);
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
        GangZoneStopPulseForPlayer(playerid, e_ZONE_PULSE_ZONE, arenaInfo[User[playerid][locationId]][arenaId]);

    User[playerid][locationId] = -1;
    User[playerid][isAlreadySpawned] = false;
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

@cmd() fullstats(playerid, params[], help)
{
    new targetid;
    if (sscanf(params, "u", targetid))
    {
        targetid = playerid;
    }

    if (!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ShowStats(playerid, targetid);
    return true;
}

@cmd() stats(playerid, params[], help)
{
    new targetid;
    if (sscanf(params, "u", targetid))
    {
        targetid = playerid;
    }

    if (!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    ShowStats(playerid, targetid);
    return true;
}

@cmd() menu(playerid, params[], help)
{
    ShowMenu(playerid);
    return true;
}

@cmd(.alts = {"w"}) weather(playerid, params[], help)
{
    if(IsNull(params)) return SendClientMessage(playerid, -1, "%s", WEATHER_HELP);
    if(!IsNumeric(params)) return SendClientMessage(playerid, COLOR_BLUE, "%s", IS_NOT_NUMERIC);

    User[playerid][weather] = strval(params);
    ChangeWeather(playerid, User[playerid][weather]);
    return true;
}

@cmd(.alts = {"t"}) time(playerid, params[], help)
{
    if(IsNull(params)) return SendClientMessage(playerid, -1, "%s", TIME_HELP);
    if(!IsNumeric(params)) return SendClientMessage(playerid,COLOR_BLUE, "%s", IS_NOT_NUMERIC);

    User[playerid][time] = strval(params);
    ChangeTime(playerid, User[playerid][time]);
    return true;
}


@cmd() gunmenu(playerid, params[], help)
{
    if(User[playerid][locationId] != -1 && User[playerid][locationId] != ARENA_AFK)
    {
        if(gettime() - User[playerid][timerRespawn] > 15)
        {
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You can only use /gunmenu in the first 15 seconds after spawning!");
        }
    }

	ShowGunMenu(playerid);
    return true;
}

@cmd() spec(playerid, params[], help)
{
    new PLAYER_STATE:playerState = GetPlayerState(playerid);

    if(playerState != PLAYER_STATE_SPECTATING && User[playerid][adminLevel] < 1)
    {
        if(User[playerid][isAfk] == false)
            return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);
    }

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "%s", SPEC_HELP);

    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(User[targetid][isAfk] && User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", TARGET_AFK);

    if (User[targetid][isLogged] == false)
        return SendClientMessage(playerid, -1, "%s", IS_NOT_LOGGED);
    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "%s", SPEC_HELP2);

    setPlayerSpectate(playerid, targetid);
    SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);

    return true;
}

@cmd() specoff(playerid, params[], help)
{
    if(User[playerid][isSpectating] == false)
        return SendClientMessage(playerid, -1, "%s", NO_SPEC_MODE);

    StopSpectate(playerid);
    return true;
}

@cmd(.alts = {"re"}) respawn(playerid, params[], help)
{
    new timestampRespawn = gettime();
    Respawn(playerid, timestampRespawn);
    return true;
}

@cmd(.alts = {"help", "commands"}) cmds(playerid, params[], help)
{
    return ShowHelpMainMenu(playerid);
}

ShowHelpMainMenu(playerid)
{
    new helpStr[1024], title[64];
    format(title, sizeof(title), "{MAIN}Server Guide & Help");
    format_MainColor(playerid, title);

    format(helpStr, sizeof(helpStr), "{MAIN}Category / Option\t{MAIN}Description\n");
    strcat(helpStr, "My Account\t{FFFFFF}View account info, statistics & settings\n");
    strcat(helpStr, "Security\t{FFFFFF}Account security, password & Discord link\n");
    strcat(helpStr, "Changemode\t{FFFFFF}Select an arena or change game mode\n");
    strcat(helpStr, "Training Mode\t{FFFFFF}Guide, commands & features of Training System\n");
    strcat(helpStr, "VIP System & Perks\t{FFFFFF}View VIP benefits, packages & commands\n");
    strcat(helpStr, "Rankings\t{FFFFFF}View top 10 player & clan leaderboards\n");
    strcat(helpStr, "Server Rules\t{FFFFFF}Read official server rules & guidelines\n");
    strcat(helpStr, "Commands List\t{FFFFFF}View full list of all server commands");
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
        strcat(body, fmat_return("{BEBEBE}%s\t"COL_WHITE"\xBB\n", g_CmdCategories[i]), sizeof(body));

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

    new cmdsstr[1536], title[80];
    format(title, sizeof(title), "{MAIN}%s", g_CmdCategories[catid]);
    format_MainColor(playerid, title);

    strcat(cmdsstr, "{MAIN}Command\t{MAIN}Description\n");

    switch(catid)
    {
        case 0:
        {
            strcat(cmdsstr, "{BEBEBE}/changemode\t"COL_WHITE"Select arena or change game mode\n");
            strcat(cmdsstr, "{BEBEBE}/menu\t"COL_WHITE"Show player settings menu\n");
            strcat(cmdsstr, "{BEBEBE}/l (/lounge)\t"COL_WHITE"Teleport to the lounge room\n");
            strcat(cmdsstr, "{BEBEBE}/respawn\t"COL_WHITE"Respawn your character\n");
            strcat(cmdsstr, "{BEBEBE}/gunmenu\t"COL_WHITE"Open weapon selection menu in arena\n");
            strcat(cmdsstr, "{BEBEBE}/spec\t"COL_WHITE"Spectate a player\n");
            strcat(cmdsstr, "{BEBEBE}/specoff\t"COL_WHITE"Stop spectating\n");
            strcat(cmdsstr, "{BEBEBE}/stats\t"COL_WHITE"View your or player stats\n");
            strcat(cmdsstr, "{BEBEBE}/fullstats\t"COL_WHITE"View detailed player stats\n");
            strcat(cmdsstr, "{BEBEBE}/ach\t"COL_WHITE"View your achievements & progress\n");
            strcat(cmdsstr, "{BEBEBE}/coins\t"COL_WHITE"Check your Coins balance\n");
            strcat(cmdsstr, "{BEBEBE}/customhud\t"COL_WHITE"Change Server HUD style\n");
            strcat(cmdsstr, "{BEBEBE}/sync\t"COL_WHITE"Resync character position\n");
        }
        case 1:
        {
            strcat(cmdsstr, "{BEBEBE}/skin\t"COL_WHITE"Change your character skin\n");
            strcat(cmdsstr, "{BEBEBE}/fight\t"COL_WHITE"Change your fighting style\n");
            strcat(cmdsstr, "{BEBEBE}/weather\t"COL_WHITE"Change your local weather\n");
            strcat(cmdsstr, "{BEBEBE}/time\t"COL_WHITE"Change your local time\n");
            strcat(cmdsstr, "{BEBEBE}/dance\t"COL_WHITE"Perform dance animations\n");
            strcat(cmdsstr, "{BEBEBE}/piss\t"COL_WHITE"Perform pee animation\n");
            strcat(cmdsstr, "{BEBEBE}/stopanim\t"COL_WHITE"Stop current animation\n");
            strcat(cmdsstr, "{BEBEBE}/flash\t"COL_WHITE"Toggle hitmark flash effect\n");
        }
        case 2:
        {
            strcat(cmdsstr, "{BEBEBE}/car\t"COL_WHITE"Spawn vehicle in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/bike\t"COL_WHITE"Spawn bicycle in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/jetpack\t"COL_WHITE"Get jetpack in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/tune\t"COL_WHITE"Tune vehicle in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/flip\t"COL_WHITE"Flip vehicle in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/fix\t"COL_WHITE"Repair vehicle in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/getgun\t"COL_WHITE"Get weapons in Lounge\n");
            strcat(cmdsstr, "{BEBEBE}/changelounge\t"COL_WHITE"Change Lounge spawn location\n");
            strcat(cmdsstr, "{BEBEBE}/lcmds\t"COL_WHITE"List all Lounge commands\n");
        }
        case 3:
        {
            strcat(cmdsstr, "{BEBEBE}/clan\t"COL_WHITE"Open Clan management menu\n");
            strcat(cmdsstr, fmat_return("{BEBEBE}/createclan\t"COL_WHITE"Create a new Clan (%s)\n", FormatMoney(1000000)));
            strcat(cmdsstr, "{BEBEBE}/clans\t"COL_WHITE"View global Clans ranking list\n");
            strcat(cmdsstr, "{BEBEBE}/territories\t"COL_WHITE"View Outpost & territory status\n");
            strcat(cmdsstr, "{BEBEBE}/clancmds\t"COL_WHITE"View full list of Clan commands\n");
        }
        case 4:
        {
            strcat(cmdsstr, "{BEBEBE}/duel [id]\t"COL_WHITE"Challenge a player to 1v1 duel\n");
            strcat(cmdsstr, "{BEBEBE}/acceptduel\t"COL_WHITE"Accept a duel challenge\n");
            strcat(cmdsstr, "{BEBEBE}/x1c\t"COL_WHITE"Create a 2v2 team duel challenge\n");
            strcat(cmdsstr, "{BEBEBE}/gx1\t"COL_WHITE"Accept a 2v2 team duel challenge\n");
            strcat(cmdsstr, "{BEBEBE}/leaveduel\t"COL_WHITE"Cancel or leave a duel\n");
            strcat(cmdsstr, "{BEBEBE}/newarena\t"COL_WHITE"Create a custom private arena\n");
            strcat(cmdsstr, "{BEBEBE}/training\t"COL_WHITE"Open training arena & challenges menu\n");
            strcat(cmdsstr, "{BEBEBE}/rt (/restarttraining)\t"COL_WHITE"Restart current training challenge\n");
            strcat(cmdsstr, "{BEBEBE}/exittraining\t"COL_WHITE"Exit current training arena\n");
            strcat(cmdsstr, "{BEBEBE}/showagain\t"COL_WHITE"Show last round results\n");
        }
        case 5:
        {
            strcat(cmdsstr, "{BEBEBE}/id [id/name]\t"COL_WHITE"Find player ID or info\n");
            strcat(cmdsstr, "{BEBEBE}/pm [id] [msg]\t"COL_WHITE"Send a private message\n");
            strcat(cmdsstr, "{BEBEBE}/transfer [id/name] [amt]\t"COL_WHITE"Transfer money to a player\n");
            strcat(cmdsstr, "{BEBEBE}/report [id] [reason]\t"COL_WHITE"Report a player to admins\n");
            strcat(cmdsstr, "{BEBEBE}/ask [question]\t"COL_WHITE"Ask a question to online staff\n");
            strcat(cmdsstr, "{BEBEBE}/votekick [id]\t"COL_WHITE"Initiate a votekick poll\n");
            strcat(cmdsstr, "{BEBEBE}/admins\t"COL_WHITE"View online administrators\n");
            strcat(cmdsstr, "{BEBEBE}/ranks\t"COL_WHITE"View server rankings\n");
            strcat(cmdsstr, "{BEBEBE}/credits\t"COL_WHITE"Show server credits\n");
        }
        case 6:
        {
            strcat(cmdsstr, "{BEBEBE}/buyvip\t"COL_WHITE"Open VIP package activation menu\n");
            strcat(cmdsstr, "{BEBEBE}/vip\t"COL_WHITE"View VIP perks & status\n");
            strcat(cmdsstr, "{BEBEBE}/vtag\t"COL_WHITE"Customize 3D VIP Head Tag\n");
            strcat(cmdsstr, "{BEBEBE}/tag\t"COL_WHITE"Manage tag visibility (VIP, Clan, Admin, Custom)\n");
            strcat(cmdsstr, "{BEBEBE}/vipcmds\t"COL_WHITE"View full list of VIP commands\n");
            strcat(cmdsstr, "{BEBEBE}/dvincular\t"COL_WHITE"Link your Discord account\n");
            strcat(cmdsstr, "{BEBEBE}/udiscord\t"COL_WHITE"Unlink your Discord account\n");
            strcat(cmdsstr, "{BEBEBE}/changepass\t"COL_WHITE"Change your account password\n");
            strcat(cmdsstr, "{BEBEBE}/changenick\t"COL_WHITE"Change your nickname\n");
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
    ShowRankingsMenu(playerid);
    return true;
}

@cmd() sync(playerid, params[], help)
{
    SyncPlayer(playerid);
    return true;
}

@cmd() credits(playerid, params[], help)
{
    new creditsStr[2048];
    strcat(creditsStr, "{00CED1}Main Information:\n");
    strcat(creditsStr, fmat_return("{FFFFFF}\x95 {08A3FD}GameMode: {FFFFFF}%s %s\n", ServerHostname, SERVER_MODE));
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Developers: {FFFFFF}oldmxa & underline\n\n");

    strcat(creditsStr, "{00CED1}Special Thanks:\n");
    strcat(creditsStr, fmat_return("{FFFFFF}\x95 {08A3FD}%s{FFFFFF} - For playing with us and helping the server grow!\n", getPlayerName(playerid)));
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Administration Team: {FFFFFF}For helping keep the servers in order\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Community: {FFFFFF}To all players for their continuous support\n\n");

    strcat(creditsStr, "{00CED1}Plugins & Libraries:\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}YSI Framework: {FFFFFF}Y_Less\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}sscanf2: {FFFFFF}Y_Less\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Streamer: {FFFFFF}Incognito\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}MySQL: {FFFFFF}BlueG, madvsh\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Pawn.RakNet: {FFFFFF}urShadow\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}ColAndreas: {FFFFFF}Pottus, Vince\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Discord Connector: {FFFFFF}maddinat0r\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Requests: {FFFFFF}Southclaws\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}Bcrypt: {FFFFFF}s00p, SgtPez\n");
    strcat(creditsStr, "{FFFFFF}\x95 {08A3FD}SKY: {FFFFFF}Osas, Zash");

    Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "{00CED1}Server Credits", creditsStr, "Close", "");
    return true;
}

@cmd() tag(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;
    ShowTagMenu(playerid);
    return true;
}

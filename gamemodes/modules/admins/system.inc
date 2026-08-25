
@cmd() interiors(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if(User[playerid][int_InteriorSelecionado] != 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are already viewing an interior! Type /exitint to exit.");

    User[playerid][int_VW] = GetPlayerVirtualWorld(playerid);
    User[playerid][int_PInterior] = GetPlayerInterior(playerid);
    GetPlayerPos(playerid, User[playerid][int_PosX], User[playerid][int_PosY], User[playerid][int_PosZ]);
    GetPlayerFacingAngle(playerid, User[playerid][int_PosFA]);

    SetPlayerVirtualWorld(playerid, playerid + 100);
    ShowInteriorsMenu(playerid);
    return true;
}

@cmd() exitint(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if(User[playerid][int_InteriorSelecionado] == 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are not viewing an interior!");

    ClearChat(playerid, 100);
    SendClientMessage(playerid, -1, "{MAIN}[INTERIOR] {FFFFFF}You exited the interior viewing mode and returned to the original world.");
    SetPlayerVirtualWorld(playerid, User[playerid][int_VW]);
    SetPlayerPos(playerid, User[playerid][int_PosX], User[playerid][int_PosY], User[playerid][int_PosZ]);
    SetPlayerFacingAngle(playerid, User[playerid][int_PosFA]);
    SetPlayerInterior(playerid, User[playerid][int_PInterior]);
    User[playerid][int_InteriorSelecionado] = 0;
    return true;
}

@cmd() saveint(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if(User[playerid][int_InteriorSelecionado] == 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are not viewing an interior!");

    new Float:Pos[3], Float:PFA, IntA;
    GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
    GetPlayerFacingAngle(playerid, PFA);
    IntA = GetPlayerInterior(playerid);

    new File:Arquivo = fopen("Interiors.txt", io_append);
    if(Arquivo)
    {
        new stringcat[300];
        format(stringcat, sizeof(stringcat), "SetPlayerPos(playerid, %f, %f, %f); // %s\r\n", Pos[0], Pos[1], Pos[2], params);
        fwrite(Arquivo, stringcat);
        format(stringcat, sizeof(stringcat), "SetPlayerFacingAngle(playerid, %f); // %s\r\n", PFA, params);
        fwrite(Arquivo, stringcat);
        format(stringcat, sizeof(stringcat), "SetPlayerInterior(playerid, %d); // %s \r\n\r\n", IntA, params);
        fwrite(Arquivo, stringcat);
        fclose(Arquivo);
        SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Coordinates successfully saved to {00EEFE}Interiors.txt{FFFFFF}.");
    }
    else
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Could not open the file to save!");
    }
    return true;
}

@cmd() setlounge(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new targetid, reason[128];
    if (sscanf(params, "us[128]", targetid, reason))
        return SendClientMessage(playerid, -1, ADM_TAG_NORM"/setlounge [id] [reason]");

    if (!IsPlayerLogged(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if (User[targetid][isAfk])
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This player is already in the lounge.");

    new coloredAdmin[MAX_PLAYER_NAME + 18], coloredTarget[MAX_PLAYER_NAME + 18];
    format(coloredAdmin, sizeof(coloredAdmin), "{%06x}%s{FFFFFF}", GetPlayerDisplayColour(playerid), User[playerid][userName]);
    format(coloredTarget, sizeof(coloredTarget), "{%06x}%s{FFFFFF}", GetPlayerDisplayColour(targetid), User[targetid][userName]);

    va_SendClientMessageToAll(-1, "{MAIN}[LNG] {FFFFFF}Admin %s[%d] sent %s[%d] to the lounge. Reason: %s", coloredAdmin, playerid, coloredTarget, targetid, reason);
    va_SendClientMessage(targetid, -1, "{MAIN}[LNG] {FFFFFF}You have been sent to the lounge by Admin %s. Reason: %s", coloredAdmin, reason);

    setAfk(targetid);
    updatePlayerHud(targetid);

    format(msgdiscord, sizeof(msgdiscord), "**Admin:** %s\n**Target:** %s\n**Reason:** %s", User[playerid][userName], User[targetid][userName], reason);
    utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
    SendDiscordEmbed(adminchat, "Set Lounge", msgencoded, DISCORD_COLOR_ADMIN);

    new log_lng[256];
    format(log_lng, sizeof(log_lng), "Sent %s to lounge. Reason: %s", User[targetid][userName], reason);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "SetLounge", log_lng);

    return true;
}

@cmd() changearena(playerid, params[], help)
{
    if (!IsAdmin(playerid, 2))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if(changeArenaDelay > gettime() &&  User[playerid][adminLevel] < 4)
        return va_SendClientMessage(playerid, -1, CHANGEARENA_DELAY, convertTime(changeArenaDelay - gettime()));

    new location = User[playerid][locationId];
    if(location < 0 || location >= MAX_LOCATIONS || location == ARENA_AFK)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be inside an arena to use this command.");

    if(location == ARENA_CUSTOM)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The Custom Room has its own map list. Use {MAIN}/clist{FFFFFF}.");

    new arenaid;
    if(sscanf(params, "i", arenaid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_NUMERIC);

    if(arenaid < 0 || arenaid >= MAX_ZONES)
        return va_SendClientMessage(playerid, -1, NOT_VALID_ARENA, MAX_ZONES-1);

    if(IsCustomArena(arenaid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}That arena belongs to the Custom Room.");

    if(arenaid == lastArenaId[location])
        return va_SendClientMessage(playerid, -1, ARENA_JUST_PLAYED, arenaData[arenaid][arenaName]);

    ApplyArenaChange(playerid, location, arenaid);

    new log_ca[256];
    format(log_ca, sizeof(log_ca), "Changed arena to %s (ID %d) in location %d", arenaData[arenaid][arenaName], arenaid, location);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "arenas", "ChangeArena", log_ca);

    return true;
}

@cmd() clist(playerid, params[], help)
{
    if (!IsAdmin(playerid, 2))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if (User[playerid][locationId] != ARENA_CUSTOM)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be in the Custom Room to use this command.");

    return ShowCustomMapListDialog(playerid);
}

@cmd() maplist(playerid, params[], help)
{
    if (!IsAdmin(playerid, 2))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if (User[playerid][locationId] == ARENA_CUSTOM)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The Custom Room has its own map list. Use {MAIN}/clist{FFFFFF}.");

    DeletePVar(playerid, "playerCurrentPage");
    DeletePVar(playerid, "pageItems");
    showMapListDialog(playerid);

    if (!IsAdmin(playerid, 4))
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Maplist was shown.");
    return true;
}

@cmd() randomchangearena(playerid, params[], help)
{
    if (!IsAdmin(playerid, 2))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if(changeArenaDelay > gettime() &&  User[playerid][adminLevel] < 4)
        return va_SendClientMessage(playerid, -1, CHANGEARENA_DELAY, convertTime(changeArenaDelay - gettime()));

    new location = User[playerid][locationId];
    if(location < 0 || location >= MAX_LOCATIONS || location == ARENA_AFK)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be inside an arena to use this command.");

    if(location == ARENA_CUSTOM)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The Custom Room has its own map list. Use {MAIN}/clist{FFFFFF}.");

    new newArenaId = determineArenaID(location);
    ApplyArenaChange(playerid, location, newArenaId);

    new log_rca[256];
    format(log_rca, sizeof(log_rca), "Random changed arena to %s (ID %d) in location %d", arenaData[newArenaId][arenaName], newArenaId, location);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "arenas", "RandomChangeArena", log_rca);

    return true;
}

@cmd() saveaccounts(playerid, params[], help)
{
    if (!IsAdmin(playerid, 4))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    foreach(new i : Player)
    {
        if(User[i][isLogged])
            UpdatePlayerData(i);
    }
    va_SendClientMessageToAll(-1, ADM_TAG_NORM"%s saved the account of all players online.", User[playerid][userName]);
    printf("[ADMIN] %s saved every online account.", User[playerid][userName]);
    return true;
}

@cmd() gmx(playerid, params[], help)
{
    if (!IsAdmin(playerid, 4))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    RestartServerSafe();
    return true;
}

@cmd() count(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1)) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    
    AdminCountdown(5);
    return true;
}

forward AdminCountdown(count);
public AdminCountdown(count)
{
    new str[32];
    if(count > 0)
    {
        format(str, sizeof(str), "~w~%d", count);
        GameTextForAll(str, 1000, 3);
        SetTimerEx("AdminCountdown", 1000, false, "i", count - 1);
    }
    else
    {
        GameTextForAll("~g~GO GO GO!", 2000, 3);
    }
    return 1;
}

@cmd() avtn(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1)) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new text[128], str[144];
    if(sscanf(params, "s[128]", text)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/avtn [text]");
    format(str, sizeof(str), "~w~%s: ~y~%s", User[playerid][userName], text);
    GameTextForAll(str, 5000, 3);
    return true;
}

@cmd() cnn(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1)) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new text[128], str[144];
    if(sscanf(params, "s[128]", text)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Use: /cnn [text]");
    format(str, sizeof(str), "# ~p~%s~p~: ~w~%s #", User[playerid][userName], text);
    GameTextForAll(str, 4000, 3);
    return true;
}

@cmd() hostname(playerid, params[], help)
{
    if (!IsAdmin(playerid, 4)) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    if(isnull(params)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/hostname [new name]");

    new str[144];
    format(str, sizeof(str), "hostname %s", params);
    SendRconCommand(str);
    
    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s changed the server hostname to: %s", User[playerid][userName], params);

    new log_hn[256];
    format(log_hn, sizeof(log_hn), "Changed hostname to: %s", params);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "ChangeHostname", log_hn);

    return true;
}

@cmd() kickall(playerid, params[], help)
{
	if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
		return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

	CheckAdminAuth(playerid);

	new count = 0;
	foreach(new i : Player)
	{
		if(i != playerid)
		{
			if(User[i][isLogged])
			{
				UpdatePlayerData(i);
			}
			SendClientMessage(i, -1, "{MAIN}[SERVER] {FFFFFF}You have been disconnected from the server by administrator.");
			KickEx(i, 500);
			count++;
		}
	}

	va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You disconnected all players from the server (%d players kicked).", count);

	new log_details[128];
	format(log_details, sizeof(log_details), "Admin %s executed /kickall (%d players kicked)", User[playerid][userName], count);
	LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "auth", "KickAll", log_details);

	return 1;
}

@cmd() lockserver(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need level 4 + RCON to use this command.");

    CheckAdminAuth(playerid);

    if(isnull(params))
        return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/lockserver [password]");

    new str[64];
    format(str, sizeof(str), "password %s", params);
    SendRconCommand(str);
    ServerLocked = true;
    
    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s locked the server.", User[playerid][userName]);

    new log_ls[128];
    format(log_ls, sizeof(log_ls), "Locked the server with password");
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "security", "LockServer", log_ls);

    return true;
}

@cmd() unlockserver(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need level 4 + RCON to use this command.");

    CheckAdminAuth(playerid);

    SendRconCommand("password 0");
    ServerLocked = false;
    va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s unlocked the server.", User[playerid][userName]);

    new log_uls[128];
    format(log_uls, sizeof(log_uls), "Unlocked the server");
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "security", "UnlockServer", log_uls);

    return true;
}

@cmd() players(playerid, params[], help)
{
    if (!IsAdmin(playerid, 1))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new str[4096], count;
    strcat(str, "Name\tRatio\tAccuracy\tArena Kills\tArena Deaths\n");

    foreach(new i : Player)
    {
        if(!User[i][isLogged]) continue;

        new kd[8];
        FormatRatio(User[i][playerKills], User[i][playerDeaths], kd);

        new Float:acc = 0.0;
        new aKills = 0, aDeaths = 0;

        if(0 <= User[i][locationId] < MAX_LOCATIONS)
        {
            acc = GetPlayerAccuracy(i, User[i][locationId]);
            aKills = playerArenaInfo[User[i][locationId]][i][arenaKills];
            aDeaths = playerArenaInfo[User[i][locationId]][i][arenaDeaths];
        }

        strcat(str, sprintf("{36cf85}%s (%d)\t{BEBEBE}%s\t{BEBEBE}%.1f%%\t{BEBEBE}%d\t{BEBEBE}%d\n", User[i][userName], i, kd, acc, aKills, aDeaths));
        count++;
    }

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}Online Players (%d)", count), str, "Close", "");
    return true;
}

@cmd() lifez(playerid, params[], help)
{
    if (!IsAdmin(playerid, 5))
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new Float:value;
    if(sscanf(params, "f", value))
        return SendClientMessage(playerid, -1, fmat_return("{MAIN}[LIFE] {FFFFFF}Use: /lifez [height] {BEBEBE}(current: %.2f | default: %.2f)", g_LifeLabelZ, LIFE_LABEL_Z_DEFAULT));

    if(value < -2.0 || value > 3.0)
        return SendClientMessage(playerid, -1, "{MAIN}[LIFE] {FFFFFF}Out of the useful range (-2.00 to 3.00).");

    g_LifeLabelZ = value;
    RebuildAllLifeLabels();

    return SendClientMessage(playerid, -1, fmat_return("{MAIN}[LIFE] {FFFFFF}Bar height set to {MAIN}%.2f{FFFFFF}. Write it into LIFE_LABEL_Z_DEFAULT once it looks right.", g_LifeLabelZ));
}

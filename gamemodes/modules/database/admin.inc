forward OffBanPlayer(adminid, username[], reason[], hours, unbants);
public OffBanPlayer(adminid, username[], reason[], hours, unbants)
{
	new pip[18], country[50], isp[100], bantimestamp, adminlevel, target_gpci[41];
	bantimestamp = gettime();

	new rows = cache_num_rows();
	if(rows)
	{
		cache_get_value_name(0, "lastip", pip);
		cache_get_value_name(0, "country", country);
		cache_get_value_name(0, "isp", isp);
		cache_get_value_int(0, "adminlevel", adminlevel);
		cache_get_value_name(0, "gpci", target_gpci);

		if(adminlevel >= User[adminid][adminLevel])
			return SendClientMessage(adminid, -1, "%s", SUPERIOR_RIGHTS);

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
			"INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
			username, pip, User[adminid][userName], reason, country, isp, bantimestamp, unbants, target_gpci);
		mysql_tquery(dbHandle, queryGlobal);

		new banMsg[256];
		format(banMsg, sizeof(banMsg), COL_ADM_PUNISH"[Admin] %s "COL_ADM_PUNISH"%s banned player %s (Reason: %s)", adminnames[User[adminid][adminLevel]][ingame], User[adminid][userName], username, reason);
		SendClientMessageToAll(-1, "%s", banMsg);

		new log_details[256];
		format(log_details, sizeof(log_details), "Off-Banned player %s. Reason: %s | Time: %d hours", username, reason, hours);
		LogServerAction(User[adminid][user_sqlid], User[adminid][userName], "bans", "Off-Ban", log_details);

		new string[1024];
		new global_kills, global_deaths;
		cache_get_value_int(0, "kills", global_kills);
		cache_get_value_int(0, "deaths", global_deaths);

		format(string, sizeof(string), "**Admin:** %s\n**Target:** %s\n**Reason:** %s\n**Unban Date:** %s\n\n**Statistics (Global):**\n**Kills:** %d\n**Deaths:** %d",
			User[adminid][userName],
			username,
			reason,
			returnDate(unbants-10800),
			global_kills,
			global_deaths);

		return SendDiscordEmbed(logs_ban, "New Ban (Offline)", string, DISCORD_COLOR_ERROR);
	}
	else {
		return SendClientMessage(adminid, COLOR_PINK, "{MAIN}[Admin] {FFFFFF} This player does not exists.");
	}
}

forward BanPlayer(adminid, targetid, reason[], hours, unbants);
public BanPlayer(adminid, targetid, reason[], hours, unbants)
{
	new pip[18], bantimestamp;
	bantimestamp = gettime();

	new rows = cache_num_rows();
	if(rows)
	{
		GetPlayerIp(targetid, pip, 18);

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
			"INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
			 User[targetid][userName], pip, User[adminid][userName], reason, User[targetid][Country], User[targetid][Isp], bantimestamp, unbants, User[targetid][user_gpci]);
		mysql_tquery(dbHandle, queryGlobal);

		new banMsg[256];
		format(banMsg, sizeof(banMsg), COL_ADM_PUNISH"[Admin] %s "COL_ADM_PUNISH"%s banned player %s (Reason: %s)", adminnames[User[adminid][adminLevel]][ingame], User[adminid][userName], User[targetid][userName], reason);
		SendClientMessageToAll(-1, "%s", banMsg);

		new log_details[256];
		format(log_details, sizeof(log_details), "Banned player %s. Reason: %s | Time: %d hours", User[targetid][userName], reason, hours);
		LogServerAction(User[adminid][user_sqlid], User[adminid][userName], "bans", "Ban", log_details);

		GameTextForPlayer(targetid, "~r~BANNED~w~!", 2500, 3);
		SendBanNotice(targetid);

		new loc = User[targetid][locationId];
		new a_kills = (loc == -1) ? 0 : playerArenaInfo[loc][targetid][arenaKills];
		new a_deaths = (loc == -1) ? 0 : playerArenaInfo[loc][targetid][arenaDeaths];
		new a_acc = (loc == -1) ? 0 : floatround(GetPlayerAccuracy(targetid, loc));

		new string[1024];
		format(string, sizeof(string), "**Admin:** %s\n**Target:** %s\n**Reason:** %s\n**Unban Date:** %s\n\n**Statistics (Arena):**\n**Kills:** %d\n**Deaths:** %d\n**Accuracy:** %d%%",
			User[adminid][userName],
			User[targetid][userName],
			reason,
			returnDate(unbants-10800),
			a_kills,
			a_deaths,
			a_acc);

		SendDiscordEmbed(logs_ban, "New Ban", string, DISCORD_COLOR_ERROR);

		KickEx(targetid, 2000);
		return true;
	}
	else {
		return SendClientMessage(adminid, COLOR_PINK, "{MAIN}[Admin] {FFFFFF} This player does not exists.");
	}
}

forward OffBanPlayerAC(adminid, const username[], const adminNick[], const reason[], hours, unbants);
public OffBanPlayerAC(adminid, const username[], const adminNick[], const reason[], hours, unbants)
{
	new pip[18], country[50], isp[100], bantimestamp, adminlevel, target_gpci[41];
	bantimestamp = gettime();

	new rows = cache_num_rows();
	if(rows)
	{
		cache_get_value_name(0, "lastip", pip);
		cache_get_value_name(0, "country", country);
		cache_get_value_name(0, "isp", isp);
		cache_get_value_int(0, "adminlevel", adminlevel);
		cache_get_value_name(0, "gpci", target_gpci);

		if(adminlevel >= User[adminid][adminLevel])
			return SendClientMessage(adminid, -1, "%s", SUPERIOR_RIGHTS);

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
			"INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
			username, pip, adminNick, reason, country, isp, bantimestamp, unbants, target_gpci);
		mysql_tquery(dbHandle, queryGlobal);

		new banMsg[256];
		format(banMsg, sizeof(banMsg), COL_ADM_PUNISH"[Admin] Anti-Cheat "COL_ADM_PUNISH"%s banned player %s (Reason: %s)", adminNick, username, reason);
		SendClientMessageToAll(-1, "%s", banMsg);

		new log_details[256];
		format(log_details, sizeof(log_details), "Off-Banned player %s as %s. Reason: %s | Time: %d hours", username, adminNick, reason, hours);
		LogServerAction(User[adminid][user_sqlid], User[adminid][userName], "bans", "SlimBan", log_details);

		new string[1024];
		new global_kills, global_deaths;
		cache_get_value_int(0, "kills", global_kills);
		cache_get_value_int(0, "deaths", global_deaths);

		format(string, sizeof(string), "**Admin:** %s (by %s)\n**Target:** %s\n**Reason:** %s\n**Unban Date:** %s\n\n**Statistics (Global):**\n**Kills:** %d\n**Deaths:** %d",
			adminNick,
			User[adminid][userName],
			username,
			reason,
			returnDate(unbants-10800),
			global_kills,
			global_deaths);

		return SendDiscordEmbed(logs_ban, "New Ban (Anti-Cheat)", string, DISCORD_COLOR_ERROR);
	}
	else {
		return SendClientMessage(adminid, COLOR_PINK, "{MAIN}[Admin] {FFFFFF} This player does not exists.");
	}
}

forward BanPlayerAC(adminid, targetid, const adminNick[], const reason[], hours, unbants);
public BanPlayerAC(adminid, targetid, const adminNick[], const reason[], hours, unbants)
{
	new pip[18], bantimestamp;
	bantimestamp = gettime();

	new rows = cache_num_rows();
	if(rows)
	{
		GetPlayerIp(targetid, pip, 18);

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
			"INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
			 User[targetid][userName], pip, adminNick, reason, User[targetid][Country], User[targetid][Isp], bantimestamp, unbants, User[targetid][user_gpci]);
		mysql_tquery(dbHandle, queryGlobal);

		new banMsg[256];
		format(banMsg, sizeof(banMsg), COL_ADM_PUNISH"[Admin] Anti-Cheat "COL_ADM_PUNISH"%s banned player %s (Reason: %s)", adminNick, User[targetid][userName], reason);
		SendClientMessageToAll(-1, "%s", banMsg);

		new log_details[256];
		format(log_details, sizeof(log_details), "Banned player %s as %s. Reason: %s | Time: %d hours", User[targetid][userName], adminNick, reason, hours);
		LogServerAction(User[adminid][user_sqlid], User[adminid][userName], "bans", "SlimBan", log_details);

		GameTextForPlayer(targetid, "~r~BANNED~w~!", 2500, 3);
		SendBanNotice(targetid, ban_message_color"[BANNED] Flagged by the Anti-Cheat.");

		new loc = User[targetid][locationId];
		new a_kills = (loc == -1) ? 0 : playerArenaInfo[loc][targetid][arenaKills];
		new a_deaths = (loc == -1) ? 0 : playerArenaInfo[loc][targetid][arenaDeaths];
		new a_acc = (loc == -1) ? 0 : floatround(GetPlayerAccuracy(targetid, loc));

		new string[1024];
		format(string, sizeof(string), "**Admin:** %s (by %s)\n**Target:** %s\n**Reason:** %s\n**Unban Date:** %s\n\n**Statistics (Arena):**\n**Kills:** %d\n**Deaths:** %d\n**Accuracy:** %d%%",
			adminNick,
			User[adminid][userName],
			User[targetid][userName],
			reason,
			returnDate(unbants-10800),
			a_kills,
			a_deaths,
			a_acc);

		SendDiscordEmbed(logs_ban, "New Ban (Anti-Cheat)", string, DISCORD_COLOR_ERROR);

		KickEx(targetid, 2000);
		return true;
	}
	return false;
}

forward ForcePlayerOMP(adminid, targetid);
public ForcePlayerOMP(adminid, targetid)
{
	new rows = cache_num_rows();
	if(rows)
	{
		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `forced_omp` = 1 WHERE `username` = '%e'", User[targetid][userName]);
		mysql_tquery(dbHandle, queryGlobal);

		new banMsg[256];
		format(banMsg, sizeof(banMsg), COL_ADM_PUNISH"[Admin] %s "COL_ADM_PUNISH"%s banned player %s (Reason: Forced open.mp Launcher)", adminnames[User[adminid][adminLevel]][ingame], User[adminid][userName], User[targetid][userName]);
		SendClientMessageToAll(-1, "%s", banMsg);

		GameTextForPlayer(targetid, "~r~BANNED~w~!", 2500, 3);
		SendBanNotice(targetid, ban_message_color"[BANNED] This account is forced to use the official open.mp launcher.");

		KickEx(targetid, 2000);

		new string[1024];
		format(string, sizeof(string), "**Admin:** %s\n**Target:** %s",
			User[adminid][userName],
			User[targetid][userName]);

		return SendDiscordEmbed(logs_ban, "New Force open.mp Ban", string, DISCORD_COLOR_ERROR);
	}

	return true;
}

forward UnBanOMP(adminid, admin[], player[]);
public UnBanOMP(adminid, admin[], player[])
{
	new rows = cache_num_rows();
	if(rows)
	{
		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `forced_omp` = 0 WHERE `username` = '%e'", player);
		mysql_tquery(dbHandle, queryGlobal);

		va_SendClientMessage(adminid, -1, OMP_UNBAN, player);

		new string[1024];
		format(string, sizeof(string), "**Admin:** %s\n**Target:** %s",
			admin,
			player);

		return SendDiscordEmbed(logs_unban, "New Force open.mp Unban", string, DISCORD_COLOR_SUCCESS);
	}

	else
	{
		SendClientMessage(adminid, -1, "{MAIN}**{FFFFFF} This player isnt open.mp Banned.");
	}

	return true;
}

forward UnBanPlayer(playerid);
public UnBanPlayer(playerid)
{
	new username[MAX_PLAYER_NAME + 1];

	new rows = cache_num_rows();
	if(rows)
	{
		cache_get_value_name(0, "username", username, sizeof(username));

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "DELETE FROM `banimentos` where `username` = '%e' LIMIT 1", username);
		mysql_tquery(dbHandle, queryGlobal);

		foreach(new i : Player)
		{
			if(User[i][adminLevel] > 0)
			{
				va_SendClientMessage(i, -1, UNBAN_MESSAGE, User[playerid][userName], username);
			}
		}

		new log_details[256];
		format(log_details, sizeof(log_details), "Unbanned player/IP %s", username);
		LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "unbans", "Unban", log_details);

		new string[1024];
		format(string, sizeof(string), "**Admin:** %s\n**Target:** %s", User[playerid][userName], username);
		SendDiscordEmbed(logs_unban, "New Unban", string, DISCORD_COLOR_SUCCESS);
	}
	else{
		SendClientMessage(playerid, COLOR_PINK, "{MAIN}[Admin] {FFFFFF} This player is not banned!");
	}
}

forward OffDox(playerid);
public OffDox(playerid)
{
	new rows = cache_num_rows();
	if(rows)
	{
		new
		regip[18],
		lastip[18],
		city[64],
		region[64],
		regioncode[3],
		country[64],
		countrycode[3],
		latitude[15],
		longitude[15],
		timezone[30],
		network[25],
		ispnumber[25],
		isp[64],
		bool:proxy,
		bool:vpn;

		cache_get_value(0, "regip", regip);
		cache_get_value(0, "lastip", lastip);
		cache_get_value(0, "city", city);
		cache_get_value(0, "region", region);
		cache_get_value(0, "regioncode", regioncode);
		cache_get_value(0, "country", country);
		cache_get_value(0, "countrycode", countrycode);
		cache_get_value(0, "latitude", latitude);
		cache_get_value(0, "longitude", longitude);
		cache_get_value(0, "timezone", timezone);
		cache_get_value(0, "network", network);
		cache_get_value(0, "ispnumber", ispnumber);
		cache_get_value(0, "isp", isp);
		cache_get_value_bool(0, "proxy", proxy);
		cache_get_value_bool(0, "vpn", vpn);

		new username[MAX_PLAYER_NAME + 1];
		cache_get_value(0, "username", username);

		new strlookup[2048], title[64];

		format(title, sizeof(title), "{MAIN}Lookup (offline): {FFFFFF}%s", username);
		format_MainColor(playerid, title);

		new accGpci[42];
		cache_get_value(0, "gpci", accGpci);
		if(isnull(accGpci)) format(accGpci, sizeof(accGpci), "N/A");

		if(isnull(isp))
		{
			new why[260];

			format(why, sizeof(why),
				"{FFFFFF}This account has no network data stored.\n\n"\
				"{B4B5B7}Registration IP: {FFFFFF}%s\n"\
				"{B4B5B7}Last IP: {FFFFFF}%s\n"\
				"{B4B5B7}GPCI / Serial: {FFFFFF}%s\n\n"\
				"{B4B5B7}The lookup only runs on public addresses. A rejected request is logged as {FFFFFF}[ANTIVPN]{B4B5B7}.",
				regip, lastip, accGpci);

			return Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, title, why, "Close", "");
		}

		format(strlookup, sizeof(strlookup),
			"{MAIN}Field\t{MAIN}Value\n"\
			"{B4B5B7}Registration IP\t{FFFFFF}%s\n"\
			"{B4B5B7}Last IP\t{FFFFFF}%s\n"\
			"{B4B5B7}GPCI / Serial\t{FFFFFF}%s\n"\
			"{B4B5B7}ISP\t{FFFFFF}%s\n"\
			"{B4B5B7}Network\t{FFFFFF}%s\n"\
			"{B4B5B7}ISP Number\t{FFFFFF}%s\n"\
			"{B4B5B7}City\t{FFFFFF}%s\n"\
			"{B4B5B7}Region\t{FFFFFF}%s (%s)\n"\
			"{B4B5B7}Country\t{FFFFFF}%s (%s)\n"\
			"{B4B5B7}Coordinates\t{FFFFFF}%s, %s\n"\
			"{B4B5B7}Time Zone\t{FFFFFF}%s\n"\
			"{B4B5B7}VPN\t%s\n"\
			"{B4B5B7}Proxy\t%s",
			regip,
			lastip,
			accGpci,
			isp,
			network,
			ispnumber,
			city,
			region, regioncode,
			country, countrycode,
			latitude, longitude,
			timezone,
			vpn == true ? (""COL_KGREEN"Yes") : (""COL_RED"No"),
			proxy == true ? (""COL_KGREEN"Yes") : (""COL_RED"No")
		);

		format_MainColor(playerid, strlookup);
		Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, strlookup, "Close", "");
		return true;
	}
	else{
		return SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} This player doesn't exist.");
	}
}

forward OnPlayerBanInfo(playerid);
public OnPlayerBanInfo(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;

	new rows = cache_num_rows();
	if(rows)
	{
		new username[MAX_PLAYER_NAME + 1], admin[MAX_PLAYER_NAME + 1], reason[128], country[64], isp[100], ip[18], banGpci[42];
		new timestampBan, timestampUnban;

		cache_get_value_name(0, "username", username);
		cache_get_value_name(0, "admin", admin);
		cache_get_value_name(0, "ip", ip);
		cache_get_value_name(0, "country", country);
		cache_get_value_name(0, "isp", isp);
		cache_get_value_name(0, "reason", reason);
		cache_get_value_name(0, "gpci", banGpci);
		cache_get_value_int(0, "ban_timestamp", timestampBan);
		cache_get_value_int(0, "unban_timestamp", timestampUnban);

		new banDate[64], unbanDate[64];
		if(timestampBan > 0) format(banDate, sizeof(banDate), "%s", returnDate(timestampBan - 10800));
		else format(banDate, sizeof(banDate), "N/A");

		if(timestampUnban == -1 || timestampUnban == 0) format(unbanDate, sizeof(unbanDate), "Permanent");
		else format(unbanDate, sizeof(unbanDate), "%s", returnDate(timestampUnban - 10800));

		new dialogStr[1024];
		format(dialogStr, sizeof(dialogStr),
			"Field\tValue\n\
			{36cf85}Username\t{BEBEBE}%s\n\
			{36cf85}Admin\t{BEBEBE}%s\n\
			{36cf85}IP\t{BEBEBE}%s\n\
			{36cf85}GPCI (Serial)\t{BEBEBE}%s\n\
			{36cf85}Country\t{BEBEBE}%s\n\
			{36cf85}ISP\t{BEBEBE}%s\n\
			{36cf85}Reason\t{BEBEBE}%s\n\
			{36cf85}Ban Date\t{BEBEBE}%s\n\
			{36cf85}Unban Date\t{BEBEBE}%s",
			username, admin, isnull(ip) ? "N/A" : ip, isnull(banGpci) ? "N/A" : banGpci,
			isnull(country) ? "N/A" : country, isnull(isp) ? "N/A" : isp,
			reason, banDate, unbanDate
		);

		Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}Ban Information: {23C552}%s", username), dialogStr, "Close", "");
	}
	else
	{
		SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}No ban record found for the specified nick or IP.");
	}
	return 1;
}


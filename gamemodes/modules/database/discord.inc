forward BanPlayerDiscord(admin[], username[], reason[], unbants, DCC_User:user);
public BanPlayerDiscord(admin[], username[], reason[], unbants, DCC_User:user)
{
	new pip[18], bantimestamp;
	bantimestamp = gettime();

	new targetid = GetOnlinePlayerIDByName(username);

	if(strequal(username, SERVER_OWNER, true))
		return SendDiscordResponse(comandos, sprintf(" **%s**, you cannot do that with the owners.", username));

	new bool:isBot;
	DCC_IsUserBot(user, isBot);

	if(_:user == 0 || (_:user != 0 && isBot))
	{
		return 0;
	}

	new rcount;
	DCC_GetGuildMemberRoleCount(guildaid, user, rcount);

	for(new i; i != rcount; i++)
	{
		new DCC_Role:role;
		DCC_GetGuildMemberRole(guildaid, user, i, role);

		new outputRole[DCC_ID_SIZE];
		DCC_GetRoleId(role, outputRole);

		if(strequal("824869097929310219", outputRole) && (IsPlayerConnected(targetid) && User[targetid][OnTempREC]))
		{
			SendDiscordResponse(comandos, sprintf(" **%s**, this player is in a temprec right now, he can only be forced banned if you are an master.", admin));
			return false;
		}
	}

	new rows = cache_num_rows();
	if(rows)
	{
		new country[51], isp[101], target_gpci[42];

		if(IsPlayerConnected(targetid))
		{
			GetPlayerIp(targetid, pip, 18);
			format(target_gpci, sizeof(target_gpci), "%s", User[targetid][user_gpci]);
		}
		else{
			cache_get_value(0, "lastip", pip);
			cache_get_value(0, "gpci", target_gpci);
		}

		cache_get_value(0, "country", country);
		cache_get_value(0, "isp", isp);

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
			"INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`,  `country`, `isp`, `ban_timestamp`, `unban_timestamp`, `gpci`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d, '%e')",
			 username, pip, admin, reason,  country, isp, bantimestamp, unbants, target_gpci);

		mysql_tquery(dbHandle, queryGlobal);

		if(IsPlayerConnected(targetid))
		{
			ShowBanDialog(targetid, admin, reason, returnDate(bantimestamp-10800), returnDate(unbants-10800));
			KickEx(targetid, 2000);
		}

		new string[1024];
		new global_kills, global_deaths;
		cache_get_value_int(0, "kills", global_kills);
		cache_get_value_int(0, "deaths", global_deaths);

		format(string, sizeof(string), "**Admin:** %s\n**Target:** %s\n**Reason:** %s\n**Unban Date:** %s\n\n**Statistics (Global):**\n**Kills:** %d\n**Deaths:** %d",
			admin,
			username,
			reason,
			returnDate(unbants-10800),
			global_kills,
			global_deaths);

		SendDiscordEmbed(logs_ban, "New Ban", string, DISCORD_COLOR_ERROR);
		SendDiscordResponse(comandos, sprintf(":white_check_mark: The player **%s** has been banned!", username));
	}
	else
		return SendDiscordResponse(comandos, sprintf(" **%s**, this player does not exists.", admin));

	return 1;
}

forward UnBanPlayerDiscord(admin[]);
public UnBanPlayerDiscord(admin[])
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
    			va_SendClientMessage(i, -1, ADM_TAG_NORM"Admin (Discord) "COL_WHITE"%s "COL_ADM_CMD"unbanned player "COL_WHITE"%s"COL_ADM_CMD".", admin, username);
    		}
    	}

		new string[1024];
		format(string, sizeof(string), "**NEW UNBAN**\n\nAdmin: **%s**\nUser unbanned: **%s**", admin, username);
		SendDiscordEmbed(logs_unban, "New Unban", string, DISCORD_COLOR_SUCCESS);
	    SendDiscordResponse(comandos, sprintf(":white_check_mark: The player **%s** has been unbanned!", username));
	}
	else{
		SendDiscordResponse(comandos, "This player is not banned!");
	}
}

forward SetAdminDiscord(admin[], username[], role[], level, actorLevel);
public SetAdminDiscord(admin[], username[], role[], level, actorLevel)
{
	new rows = cache_num_rows();
	if(level < 0 || level >= sizeof(adminnames))
	{
		SendDiscordResponse(comandos, sprintf(" **%s**, invalid admin level.", admin));
		return 0;
	}

	if(rows)
	{
		new idplayer, targetLevel;
		cache_get_value_int(0, "id", idplayer);
		cache_get_value_int(0, "adminlevel", targetLevel);

		if(targetLevel >= actorLevel)
		{
			SendDiscordResponse(comandos, sprintf(" **%s**, you cannot change the level of an administrator with a higher or equal level than yours.", admin));
			return 0;
		}

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
			"UPDATE `accounts` SET `adminlevel` = %d WHERE `id` = %d", level, idplayer);
		mysql_tquery(dbHandle, queryGlobal);

		new targetid = GetOnlinePlayerIDByName(username);

		if(IsPlayerConnected(targetid))
		{
			if(User[targetid][isLogged])
			{
				new string[256];
				foreach(new i : Player)
				{
					if(User[i][adminLevel] > 0)
					{
						format(string, sizeof(string), "{MAIN}[Admin] {E63114} Administrator %s %s {FFFFFF}%s{E63114} to %s.",
							admin, (level > User[targetid][adminLevel]) ? ("promoted") : ("demoted"), User[targetid][userName], adminnames[level][ingame]);
						SendClientMessage(i, 0x01A2F8AA, "%s", string);
					}
				}

				User[targetid][adminLevel] = level;
				SyncPlayerDiscordRoles(targetid);
			}
		}

		new string[1024];
		format(string, sizeof(string), "**ADMIN LEVEL CHANGE**\n\nAdmin: **%s**\nUser: **%s**\nLevel: **%s**", admin, username, role);
		SendDiscordEmbed(logs_promotions, "Admin Level Change", string, DISCORD_COLOR_ADMIN);

		format(string, sizeof(string), ":white_check_mark: Admin for player **%s** is: **%s**!", username, role);
		SendDiscordResponse(comandos, string);
	}
	else
		SendDiscordResponse(comandos, sprintf(" **%s**, This player does not exist.", admin));

	return 1;
}

forward BanInfoDiscord();
public BanInfoDiscord()
{
	new rows = cache_num_rows();
	if(rows)
	{
		new username[MAX_PLAYER_NAME + 1], admin[MAX_PLAYER_NAME + 1], reason[120 + 1], country[50 + 1], isp[100 + 1], timestampBan, timestampUnban, ip[18];
		cache_get_value_name(0, "username", username);
		cache_get_value_name(0, "admin", admin);
		cache_get_value_name(0, "ip", ip);
		cache_get_value_name(0, "country", country);
		cache_get_value_name(0, "isp", isp);
		cache_get_value_name(0, "reason", reason);
		cache_get_value_int(0, "ban_timestamp", timestampBan);
		cache_get_value_int(0, "unban_timestamp", timestampUnban);

		new reasonEncoded[120 + 1];
		utf8encode(reasonEncoded, reason, sizeof(reasonEncoded));

		new stringBan[70], stringUnban[70];
		format(stringBan, sizeof(stringBan), "%s", returnDate(timestampBan-10800));
		format(stringUnban, sizeof(stringUnban), "%s", returnDate(timestampUnban-10800));

		new string[1024];
		format(string, sizeof(string), "**Admin:** '%s'\n\
		**User banned:** '%s'\n\
		**IP:** '%s'\n\
		**Country:** %s\n\
		**ISP:** %s\n\
		**Reason:** '%s'\n\
		**Ban Date:** '%s'\n\
		**Unban Date:** '%s'", admin, username, ip, country, isp, reasonEncoded, stringBan, stringUnban);

		SendDiscordEmbedReply("Ban Information", string, DISCORD_COLOR_INFO);
	}
	else
		SendDiscordResponse(comandos, " This player isn't banned.");
}

forward PlayerInfoDiscord();
public PlayerInfoDiscord()
{
	new rows = cache_num_rows();
	if(rows)
	{
		new username[MAX_PLAYER_NAME + 1], regip[18], lastip[18], country[50 + 1], isp[100 + 1], gpciInfo[41 + 1], kills, deaths, adminlevel, level, lastlogin;
		cache_get_value(0, "username", username);
		cache_get_value(0, "regip", regip);
		cache_get_value(0, "lastip", lastip);
		cache_get_value(0, "country", country);
		cache_get_value(0, "isp", isp);
		cache_get_value(0, "gpci", gpciInfo);
		cache_get_value_int(0, "kills", kills);
		cache_get_value_int(0, "deaths", deaths);
		cache_get_value_int(0, "adminlevel", adminlevel);
		cache_get_value_int(0, "level", level);
		cache_get_value_int(0, "last_login", lastlogin);

		new role[14], lastLoginString[50];
		format(role, sizeof(role), "%s", adminnames[adminlevel][discord]);
		format(lastLoginString, sizeof(lastLoginString), "%s", returnDate(lastlogin-10800));

		new string[1024];
		format(string, sizeof(string), "**User:** '%s'\n\
		**Registration IP:** '%s'\n\
		**Last IP:** '%s'\n\
		**Last login:** '%s'\n\
		**GPCI:** '%s'\n\
		**Country:** '%s'\n\
		**ISP:** '%s'\n\
		**Level:** %d\n\
		**Kills:** %d\n\
		**Deaths:** %d\n\
		**Admin Level:** %s", username, regip, lastip, lastLoginString, gpciInfo, country, isp, level, kills, deaths, role);

		SendDiscordEmbedReply("Player Information", string, DISCORD_COLOR_INFO);
	}
	else
		SendDiscordResponse(comandos, "This player does not exist.");
}

forward UnbanIspDiscord();
public UnbanIspDiscord()
{
	new rows = cache_num_rows();
	if(rows)
	{
		new isp[151];

		cache_get_value_name(0, "isp", isp, sizeof(isp));

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "DELETE FROM `ispsbanidas` where `isp` = '%e'", isp);
		mysql_tquery(dbHandle, queryGlobal);

		SendDiscordResponse(comandos, "✅ ISP deleted.");
	}
	else
		SendDiscordResponse(comandos, "This ISP does not exist.");
}

forward BanIspDiscord(isp[]);
public BanIspDiscord(isp[])
{
	if(cache_num_rows() == 0)
	{
		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "INSERT INTO `ispsbanidas` (`isp`) VALUES('%e')", isp);
		mysql_tquery(dbHandle, queryGlobal);

		SendDiscordResponse(comandos, "✅ ISP banned.");
	}
	else
		SendDiscordResponse(comandos, "This ISP already exists.");
}

forward AkaDiscord(username[], admin[]);
public AkaDiscord(username[], admin[])
{
	new rows = cache_num_rows();
	if(rows > 0)
	{
		new regip[18];
		cache_get_value_name(0, "regip", regip, sizeof(regip));

		inline const OnAkaListed()
		{
			new MEGAString[2500], String[MAX_PLAYER_NAME + 256];

			strcat(MEGAString, "**Account name**\n\n", sizeof(MEGAString));
			for(new i = 0; i < cache_num_rows(); i++)
			{
				new pname[MAX_PLAYER_NAME + 1];
				cache_get_value_name(i, "username", pname, sizeof(pname));
				format(String, sizeof(String), "**%s**\n", pname);
				strcat(MEGAString, String, sizeof(MEGAString));
			}
			SendDiscordEmbedReply("Accounts Linked", MEGAString, DISCORD_COLOR_INFO);
		}
		MySQL_TQueryInline(dbHandle, using inline OnAkaListed,
			"SELECT `username` FROM `accounts` WHERE `regip` = '%e'", regip);
	}
	else
		SendDiscordResponse(comandos, sprintf(" **%s**, this player does not exist.", admin));
}

forward ForceR2Discord(admin[], targetid);
public ForceR2Discord(admin[], targetid)
{
	new rows = cache_num_rows();
	if(rows)
	{
		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `forcedr2` = 1 WHERE `username` = '%e'", User[targetid][userName]);
		mysql_tquery(dbHandle, queryGlobal);

		va_SendClientMessageToAll(-1, FORCED_R2_BAN, admin, User[targetid][userName]);

		ShowBanDialog(targetid, admin, "Forced to use R2+", "Now", "Never (Update SA:MP)");
		KickEx(targetid, 2000);

		SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, the player %s got forced to play with R2+.", admin, User[targetid][userName]));

		new string[1024];
		format(string, sizeof(string), "**Admin:** **%s**\n**User banned:** **%s**",
			admin,
			User[targetid][userName]);

		return SendDiscordEmbed(logs_ban, "New Force R2 Ban", string, DISCORD_COLOR_ERROR);
	}

	return true;
}

forward UnBanR2Discord(admin[], player[]);
public UnBanR2Discord(admin[], player[])
{
	new rows = cache_num_rows();
	if(rows)
	{
		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `forcedr2` = 0 WHERE `username` = '%e'", player);
		mysql_tquery(dbHandle, queryGlobal);

		SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, the player %s won't be forced to play with R2+ no more.", admin, player));

		new string[1024];
		format(string, sizeof(string), "**Admin:** **%s**\n**User unbanned:** **%s**",
			admin,
			player);

		return SendDiscordEmbed(logs_unban, "New Force R2 Unban", string, DISCORD_COLOR_SUCCESS);
	}

	else
	{
		SendDiscordResponse(comandos, sprintf(" **%s**, the player %s isn't force banned to play with R2+.", admin, player));
	}

	return true;
}

forward DoxDiscord(admin[]);
public DoxDiscord(admin[])
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

		cache_get_value_name(0, "regip", regip);
		cache_get_value_name(0, "lastip", lastip);
		cache_get_value_name(0, "city", city);
		cache_get_value_name(0, "region", region);
		cache_get_value_name(0, "regioncode", regioncode);
		cache_get_value_name(0, "country", country);
		cache_get_value_name(0, "countrycode", countrycode);
		cache_get_value_name(0, "latitude", latitude);
		cache_get_value_name(0, "longitude", longitude);
		cache_get_value_name(0, "timezone", timezone);
		cache_get_value_name(0, "network", network);
		cache_get_value_name(0, "ispnumber", ispnumber);
		cache_get_value_name(0, "isp", isp);
		cache_get_value_bool(0, "proxy", proxy);
		cache_get_value_bool(0, "vpn", vpn);

		new strdox[2048];
		format(strdox, sizeof(strdox),
			"Status - Success\n\
			Registration IP: *'%s'*\n\
			Last IP: *'%s'*\n\
			City: *'%s'*\n\
			Region: *'%s'*\n\
			Region Code: *'%s'*\n\
			Country: *'%s'*\n\
			Country Code: *'%s'*\n\
			Latitude: *'%s'*\n\
			Longtude: *'%s'*\n\
			Time Zone: *'%s'*\n\
			Network: *'%s'*\n\
			ISP: *'%s'*\n\
			ISP Number: *'%s'*\n\
			VPN: *'%s'*\n\
			Proxy: *'%s'*",
			regip, lastip,
			city,
			region,	regioncode,
			country, countrycode,
			latitude, longitude,
			timezone,	network,
			isp, ispnumber,
			vpn == true ? ("Yes") : ("No"),
			proxy == true ? ("Yes") : ("No")
		);

		return SendDiscordEmbedReply("IP DOX Information", strdox, DISCORD_COLOR_INFO);
	}
	else{
		return SendDiscordResponse(comandos, sprintf("**%s**, This player/ip doesn't exist.", admin));
	}
}

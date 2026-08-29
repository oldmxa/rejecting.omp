
forward LoginTimeout(playerid);
public LoginTimeout(playerid)
{
    if (IsPlayerNPC(playerid)) return 1;

    if(!IsPlayerLogged(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You took too long to login and were kicked.");
        KickEx(playerid);
    }
    return 1;
}

forward OnLoginTypePassword(playerid);
public OnLoginTypePassword(playerid)
{
    new dialogStr[512], dialogTitle[64];
    if(GetPVarInt(playerid, "LoginType") == 1) // Login
    {
        format(dialogTitle, sizeof(dialogTitle), "{MAIN}Authorization");
        format(dialogStr, sizeof(dialogStr),
"{FFFFFF}Welcome back to {MAIN}%s{FFFFFF}, {MAIN}%s{FFFFFF}!\n\n\
This account is registered in our database.\n\
Please enter your password below to access your account.",
            ServerHostname, User[playerid][userName]);
        

        format_MainColor(playerid, dialogTitle);
        format_MainColor(playerid, dialogStr);
        Dialog_ShowCallback(playerid, using public LOGIN<iiiis>, DIALOG_STYLE_PASSWORD, dialogTitle, dialogStr, "Login", "Cancel");
    }
    else // Register
    {
        format(dialogTitle, sizeof(dialogTitle), "{MAIN}Registration");
        format(dialogStr, sizeof(dialogStr),
"{FFFFFF}Welcome to {MAIN}%s{FFFFFF}, {MAIN}%s{FFFFFF}!\n\n\
This account {e26d5b}is not{FFFFFF} registered yet.\n\
Enter a password below to create your account.\n\n\
{B4B5B7}Letters and numbers only, %d-%d characters.",
            ServerHostname, User[playerid][userName], MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);


        format_MainColor(playerid, dialogTitle);
        format_MainColor(playerid, dialogStr);
        Dialog_ShowCallback(playerid, using public REGISTER<iiiis>, DIALOG_STYLE_PASSWORD, dialogTitle, dialogStr, "Register", "Cancel");
    }
    return 1;
}

showRegistrationDialog(playerid)
{
    SetPVarInt(playerid, "LoginType", 0);
    ShowLoginScreen(playerid, 0);
}

showAutorizationDialog(playerid)
{
    SetPVarInt(playerid, "LoginType", 1);
    ShowLoginScreen(playerid, 1);
}

forward LOGIN(playerid, dialogid, response, listitem, string:inputtext[]);
public LOGIN(playerid, dialogid, response, listitem, string:inputtext[])
{
	if (response)
	{
		if(strlen(inputtext) != NULL)
		{
			new pass_attempt[MAX_PASSWORD_LENGTH + 1];
			strcopy(pass_attempt, inputtext, sizeof(pass_attempt));

			inline const OnLoginPasswordFetch()
			{
				if(IsPlayerConnected(playerid) && cache_num_rows())
				{
					new password[BCRYPT_HASH_LENGTH];
					cache_get_value_name(0, "password", password, BCRYPT_HASH_LENGTH);
					bcrypt_verify(playerid, "OnLoginVerify", pass_attempt, password);
				}
			}
			MySQL_TQueryInline(dbHandle, using inline OnLoginPasswordFetch,
				"SELECT `password` FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);

			return true;
		}

		showAutorizationDialog(playerid);
		return true;
	}

	SelectTextDraw(playerid, 0x00BFFFFF);
	return true;
}

forward OnLoginVerify(playerid, bool:success);
public OnLoginVerify(playerid, bool:success)
{
 	if(success)
	{
        KillTimer(User[playerid][loginTimer]);
		HideLoginScreen(playerid);
		new planeString[250];
		strcopy(planeString, returnDate(User[playerid][lastLogin]-10800, 5), sizeof(planeString));

		if(!User[playerid][hideWelcomeMsg])
		{
			new coloredNick[MAX_PLAYER_NAME + 16];
			format(coloredNick, sizeof(coloredNick), "{%06x}%s",
				GetPlayerDisplayColour(playerid), User[playerid][userName]);

			va_SendClientMessage(playerid, -1, GetRandomWelcomeGreeting(), coloredNick);

			if (IsAdmin(playerid, 1))
			{
				va_SendClientMessage(playerid, -1, GetRandomAdminAccountMsg(), adminnames[User[playerid][adminLevel]][ingame]);
			}
			else
			{
				va_SendClientMessage(playerid, -1, GetRandomPlayerAccountMsg());
			}

			if(!strcmp(User[playerid][dc_username], "-1", true))
			{
				SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} Discord: {7289da}Not Linked");
			}
			else
			{
				if(isnull(User[playerid][dc_discriminator]) || !strcmp(User[playerid][dc_discriminator], "0") || !strcmp(User[playerid][dc_discriminator], "0000") || !strcmp(User[playerid][dc_discriminator], "-1"))
					SendClientMessage(playerid, -1, fmat_return("{MAIN}**{FFFFFF} Discord: {7289da}%s", User[playerid][dc_username]));
				else
					SendClientMessage(playerid, -1, fmat_return("{MAIN}**{FFFFFF} Discord: {7289da}%s#%s", User[playerid][dc_username], User[playerid][dc_discriminator]));
			}

			if(IsPlayerInClan(playerid))
			{
				new cid = User[playerid][clanID];
				new clanColorHex[7], coloredClanName[64];
				format(clanColorHex, sizeof(clanColorHex), "%06x", ClanInfo[cid][clanColor] >>> 8);
				format(coloredClanName, sizeof(coloredClanName), "{%s}%s", clanColorHex, ClanInfo[cid][clanName]);

				va_SendClientMessage(playerid, -1, GetRandomClanWelcomeMsg(), coloredClanName);
			}

			va_SendClientMessage(playerid, -1, GetRandomLastLoginMsg(), planeString);
		}

		User[playerid][isLogged] = true;
        User[playerid][isAfk] = true;
        User[playerid][locationId] = ARENA_AFK;
        User[playerid][lastLocation] = ARENA_AFK;

        SpawnPlayerEx(playerid);
        setAfk(playerid);

        CheckPlayerVipOnLogin(playerid);

        // modules/training/records.inc
        CallLocalFunction("LoadPlayerChallengeRecords", "i", playerid);

		if(User[playerid][adminLevel] > 0 && User[playerid][adminAuthType] == 1)
		{
			StartAdminPasswordAuth(playerid);
		}

		mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `is_online` = 1 WHERE `id` = %d", User[playerid][user_sqlid]);
		mysql_tquery(dbHandle, queryGlobal);

		format(User[playerid][user_gpci], 41, "%s", ReturnCI(playerid));
		format(User[playerid][user_lastip], 18, "%s", getPlayerIp(playerid));
 	}
 	else
	{
        User[playerid][loginAttempts]++;
        if(User[playerid][loginAttempts] >= ServerMaxLoginAttempts)
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You have been banned for %d minutes due to excessive login failures.", MAX_ACCOUNT_LOCKTIME);

            new timestamp = gettime();
            new unban_timestamp = timestamp + (MAX_ACCOUNT_LOCKTIME * 60);
            
            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO `banimentos` (`username`, `ip`, `gpci`, `admin`, `reason`, `ban_timestamp`, `unban_timestamp`) VALUES ('%e', '%e', '%e', 'System', 'Excessive Login Failures', %d, %d)",
                User[playerid][userName], User[playerid][user_lastip], User[playerid][user_gpci], timestamp, unban_timestamp);
            mysql_tquery(dbHandle, queryGlobal);
            
            KickEx(playerid);
        }
        else
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Incorrect password. Attempt {FFFFFF}%d{FFFFFF}/{FF0000}%d.", User[playerid][loginAttempts], ServerMaxLoginAttempts);
 		    showAutorizationDialog(playerid);
        }
 	}
	return true;
}

forward REGISTER(playerid, dialogid, response, listitem, string:inputtext[]);
public REGISTER(playerid, dialogid, response, listitem, string:inputtext[])
{
	if(response)
	{
		if(ServerMaxAccountsPerIP > 0 && playerAccounts[playerid] >= ServerMaxAccountsPerIP)
			return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This connection already has %d accounts registered.", ServerMaxAccountsPerIP);

		if (IsValidPassword(inputtext))
		{
			bcrypt_hash(playerid, "OnRegisterHash", inputtext, BCRYPT_COST);
			return true;
		}
		va_SendClientMessage(playerid, -1, ERROR_INVALIDPASS, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
		showRegistrationDialog(playerid);
		return true;
	}

	SelectTextDraw(playerid, 0x00BFFFFF);
	return true;
}

forward CountPlayerAccounts(playerid);
public CountPlayerAccounts(playerid)
{
	if(cache_num_rows()) cache_get_value_name_int(0, "total", playerAccounts[playerid]);
	return 1;
}

CheckAccountLimit(playerid)
{
	new pip[18];
	GetPlayerIp(playerid, pip, sizeof(pip));

	playerAccounts[playerid] = 0;

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT COUNT(*) AS `total` FROM `accounts` WHERE `regip` = '%e' OR `gpci` = '%e'", pip, ReturnCI(playerid));
	mysql_tquery(dbHandle, queryGlobal, "CountPlayerAccounts", "i", playerid);
	return 1;
}

forward OnRegisterHash(playerid, accountname[]);
public OnRegisterHash(playerid, accountname[])
{
    new password[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(password);

	new stringQ[512], pip[18];
	GetPlayerIp(playerid, pip, 18);
	format(User[playerid][user_gpci], 41, "%s", ReturnCI(playerid));
	format(User[playerid][user_regdate], 32, "%s", returnDate(gettime() - 10800));

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"INSERT INTO `accounts`(`username`, `regip`, `password`, `gpci`, `is_online`, `hud_type`, `hudcolor`, `hud_shade_one`, `hud_shade_two`, `chat_main_color`) VALUES('%e', '%e', '%e', '%e', 1, %d, %d, %d, %d, %d)",
		User[playerid][userName], pip, password, User[playerid][user_gpci], ServerDefaultHud, ServerDefaultHudColor, ServerDefaultHudShadeOne, ServerDefaultHudShadeTwo, ServerDefaultChatMainColor);

	mysql_tquery(dbHandle, queryGlobal , "InsertDataSQL", "i", playerid);

	HideLoginScreen(playerid);

	format(stringQ, sizeof(stringQ), REGISTER_WRITE_MSG, ServerHostname);
	SendClientMessage(playerid, -1, "%s", stringQ);

	new string[MAX_PLAYER_NAME + 50];
	format(string, sizeof(string), "{92B5B8}*** {979E9E} %s has connected", User[playerid][userName]);
    SendClientMessageToAll(0xC4C4C4FF, "%s", string);

	PlaySuccessSound(playerid);

	format(User[playerid][user_lastip], 18, "%s", pip);
	User[playerid][isLogged] = true;
    User[playerid][isAfk] = true;
    User[playerid][locationId] = ARENA_AFK;
    User[playerid][lastLocation] = ARENA_AFK;
    User[playerid][hudType] = ServerDefaultHud;
    User[playerid][lifeType] = LIFE_DEFAULT;
    User[playerid][hudColor] = ServerDefaultHudColor;
    User[playerid][hudShadeOne] = ServerDefaultHudShadeOne;
    User[playerid][hudShadeTwo] = ServerDefaultHudShadeTwo;
    User[playerid][chatMainColor] = ServerDefaultChatMainColor;
    User[playerid][weather] = 32;
    User[playerid][time] = 7;
    SetPlayerWeather(playerid, 32);
    SetPlayerTime(playerid, 7, 0);
    RecreatePlayerFpsPingTd(playerid);
    UpdateRoundStatsColors(playerid);
    User[playerid][loungeX] = LOUNGE_SPAWN_X;
    User[playerid][loungeY] = LOUNGE_SPAWN_Y;
    User[playerid][loungeZ] = LOUNGE_SPAWN_Z;
    User[playerid][loungeA] = LOUNGE_SPAWN_A;
    User[playerid][loungeVW] = LOUNGEVW;
    User[playerid][loungeINT] = 0;
    User[playerid][showClanHud] = true;
    User[playerid][showConnectMsg] = true;
    User[playerid][hideGlobalMsg] = false;
    User[playerid][hideWelcomeMsg] = false;
    User[playerid][show3DDeathTag] = false;
    User[playerid][dmgFeedStyle] = 1;
    User[playerid][dmgFeedColorLeft] = -1;
    User[playerid][dmgFeedColorRight] = -1;
    SetDamageFeedStyle(playerid, 1);
    ApplyDamageFeedColors(playerid);
    User[playerid][showRandomDeathTags] = true;
    User[playerid][hitSoundID] = 17802;
    User[playerid][killSoundID] = 1057;

	return true;
}

forward DIALOG_LOGIN_NICK(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_LOGIN_NICK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if(strlen(inputtext) < 3 || strlen(inputtext) > 20)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The name must be between 3 and 20 characters.");
            return CallLocalFunction("ShowChangeNickDialog", "d", playerid);
        }

        for(new i = 0; inputtext[i] != EOS; ++i)
        {
            switch(inputtext[i])
            {
                case '0'..'9', 'A'..'Z', 'a'..'z', '_', '[', ']', '(', ')', '.', '@': continue;
                default:
                {
                    SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Your name contains invalid characters.");
                    return CallLocalFunction("ShowChangeNickDialog", "d", playerid);
                }
            }
        }

        if(SetPlayerName(playerid, inputtext) == -1)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This name is already in use or invalid.");
            return CallLocalFunction("ShowChangeNickDialog", "d", playerid);
        }

        User[playerid] = User[MAX_PLAYERS];

        GetPlayerName(playerid, User[playerid][userName], MAX_PLAYER_NAME);
        
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);
        mysql_tquery(dbHandle, queryGlobal, "OnPlayerNameChangeCheck", "d", playerid);
    }
    else SelectTextDraw(playerid, 0x00BFFFFF);
    return 1;
}

forward OnPlayerNameChangeCheck(playerid);
public OnPlayerNameChangeCheck(playerid)
{
    new rows = cache_num_rows();
    
    CallLocalFunction("UpdateLoginNameTD", "d", playerid);

    if(rows)
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);
        mysql_tquery(dbHandle, queryGlobal, "OnPlayerJoin", "d", playerid);
    }
    else
    {
        showRegistrationDialog(playerid);
    }
    return 1;
}

forward ShowChangeNickDialog(playerid);
public ShowChangeNickDialog(playerid)
{
    new title[64];
    format(title, sizeof(title), "{MAIN}Change Nickname");
    format_MainColor(playerid, title);
    Dialog_ShowCallback(playerid, using public DIALOG_LOGIN_NICK<iiiis>, DIALOG_STYLE_INPUT, title, "{FFFFFF}Enter your new nickname below:\n{B4B5B7}(This will allow you to login to another account)", "Change", "Back");
    return 1;
}

forward InsertDataSQL(playerid);
public InsertDataSQL(playerid)
{
    User[playerid][user_sqlid] = cache_insert_id();
    LoadPlayerAchievements(playerid, "");
}

forward ForcedOMPData(playerid);
public ForcedOMPData(playerid)
{
	new rows = cache_num_rows();
	if (rows)
	{
		cache_get_value_bool(0, "forced_omp", User[playerid][ForcedOMP]);
	}

	if (User[playerid][ForcedOMP])
	{
		if (!IsPlayerUsingOmp(playerid))
		{
			new string[160];
			format(string, sizeof(string), "[OPEN.MP REQUIRED] The account %s is forced to play with open.mp launcher. Download it at open.mp and reconnect.", User[playerid][userName]);
			SendClientMessage(playerid, 0xB81A1AFF, "%s", string);
			return KickEx(playerid, 1000);
		}
	}

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
	"SELECT * FROM `banimentos` \
	WHERE (`username` = '%e' OR `ip` = '%e' OR `gpci` = '%e') \
	AND (`unban_timestamp` <= 0 OR `unban_timestamp` > %d) \
	ORDER BY (`unban_timestamp` <= 0) DESC, `unban_timestamp` DESC LIMIT 1",
	User[playerid][userName], User[playerid][user_lastip], User[playerid][user_gpci], gettime());
	mysql_tquery(dbHandle, queryGlobal, "VerifyBan", "d", playerid);

	return true;
}

forward VerifyBan(playerid);
public VerifyBan(playerid)
{
	if(cache_num_rows())
	{
		new adminname[25], reason[120 + 1], unbantimestamp, bantimestamp;
		cache_get_value_int(0, "unban_timestamp", unbantimestamp);
		cache_get_value_int(0, "ban_timestamp", bantimestamp);
		cache_get_value(0, "admin", adminname);
		cache_get_value(0, "reason", reason);

		if(strlen(reason) < 1)
			reason = "No reason given.";

		new banTS[50], unbanTS[50];
		strcopy(banTS, returnDate(bantimestamp-10800), sizeof(banTS));

		if(unbantimestamp <= 0)
			strcopy(unbanTS, "Permanent", sizeof(unbanTS));
		else
			strcopy(unbanTS, returnDate(unbantimestamp-10800), sizeof(unbanTS));

		ShowBanDialog(playerid, adminname, reason, banTS, unbanTS);

		KickEx(playerid, 2000);
		return true;
	}

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);

	mysql_tquery(dbHandle, queryGlobal, "OnPlayerJoin", "d", playerid);
	return true;
}

forward OnPlayerJoin(playerid);
public OnPlayerJoin(playerid)
{
	SetClanColor(playerid);
	ClearChat(playerid, 100);
	new rows = cache_num_rows();
	if(rows)
	{
		cache_get_value_int(0, "id", User[playerid][user_sqlid]);
		new strweaps[32];

		cache_get_value_name(0, "password", User[playerid][user_pass], BCRYPT_HASH_LENGTH);
		cache_get_value_name(0, "regip", User[playerid][user_regip], 18);
		cache_get_value_name(0, "gpci", User[playerid][user_gpci], 41);
		cache_get_value_name(0, "created_at", User[playerid][user_regdate], 32);
		cache_get_value_int(0, "level", User[playerid][playerLevel]);
		cache_get_value_int(0, "player_money", User[playerid][playerMoney]);
		cache_get_value_int(0, "coins", User[playerid][playerCoins]);
		cache_get_value_int(0, "kills", User[playerid][playerKills]);
		cache_get_value_int(0, "kills_semanais", User[playerid][kills_semanais]);
		cache_get_value_int(0, "max_damage", User[playerid][maxDamage]);
		cache_get_value_int(0, "damage_semanal", User[playerid][dmgSemanal]);
		cache_get_value_int(0, "max_streak", User[playerid][maxStreak]);
		cache_get_value_int(0, "deaths", User[playerid][playerDeaths]);
		cache_get_value_int(0, "time", User[playerid][time]);
		cache_get_value_int(0, "weather", User[playerid][weather]);
		cache_get_value_int(0, "adminlevel", User[playerid][adminLevel]);
		cache_get_value_int(0, "clanid", User[playerid][clanID]);
		cache_get_value_int(0, "clanrole", User[playerid][clanRole]);
		cache_get_value_int(0, "hudcolor", User[playerid][hudColor]);
		cache_get_value_int(0, "hud_shade_one", User[playerid][hudShadeOne]);
		cache_get_value_int(0, "hud_shade_two", User[playerid][hudShadeTwo]);

		cache_get_value_int(0, "chat_color", User[playerid][chatColor]);
		cache_get_value_int(0, "chat_id_color", User[playerid][chatIDColor]);
		User[playerid][chatMainColor] = 0;
		cache_get_value_int(0, "chat_main_color", User[playerid][chatMainColor]);
		cache_get_value_int(0, "vip_level", User[playerid][vipLevel]);
		cache_get_value_int(0, "vip_expire", User[playerid][vipExpire]);
		cache_get_value_int(0, "vip_head_tag", User[playerid][vipHeadTag]);
		cache_get_value_int(0, "hitmarkcolor", User[playerid][hitmarkColor]);
		new fightingStyle;
		cache_get_value_int(0, "fighting_style", fightingStyle);
		User[playerid][fightStyle] = FIGHT_STYLE:fightingStyle;
		cache_get_value_int(0, "skin_id", User[playerid][playerSkin]);
		cache_get_value_int(0, "last_login", User[playerid][lastLogin]);
		cache_get_value_int(0, "won_arenas", User[playerid][wonArenas]);
		cache_get_value_int(0, "duel_wins", User[playerid][duelWins]);
		cache_get_value_int(0, "duel_losses", User[playerid][duelLosses]);
		cache_get_value_int(0, "duel2v2_wins", User[playerid][duel2v2Wins]);
		cache_get_value_int(0, "duel2v2_losses", User[playerid][duel2v2Losses]);
		cache_get_value_int(0, "warnings", User[playerid][warnings]);
		cache_get_value_bool(0, "block_pm", User[playerid][blockPM]);
		cache_get_value_int(0, "staticdamage", User[playerid][dmgInformerStyle]);
		cache_get_value_int(0, "showplayerhud", User[playerid][showPlayerHud]);
		cache_get_value_bool(0, "show_clan_hud", User[playerid][showClanHud]);
		cache_get_value_int(0, "hud_type", User[playerid][hudType]);
		cache_get_value_int(0, "life_type", User[playerid][lifeType]);
		RecreatePlayerFpsPingTd(playerid);
        cache_get_value_bool(0, "show_headshot", User[playerid][showHeadshot]);
        cache_get_value_bool(0, "show_connect_msg", User[playerid][showConnectMsg]);
        cache_get_value_bool(0, "hide_global_msg", User[playerid][hideGlobalMsg]);
        cache_get_value_bool(0, "hide_welcome_msg", User[playerid][hideWelcomeMsg]);
        // "Welcome to server" It only appears for new accounts. (OnRegisterHash);
        cache_get_value_bool(0, "show_3d_deathtag", User[playerid][show3DDeathTag]);
        cache_get_value_int(0, "dmgfeed_style", User[playerid][dmgFeedStyle]);
        if(User[playerid][dmgFeedStyle] < 0 || User[playerid][dmgFeedStyle] > 2) User[playerid][dmgFeedStyle] = 1;
        SetDamageFeedStyle(playerid, User[playerid][dmgFeedStyle]);
        cache_get_value_int(0, "dmgfeed_color_left", User[playerid][dmgFeedColorLeft]);
        cache_get_value_int(0, "dmgfeed_color_right", User[playerid][dmgFeedColorRight]);
        ApplyDamageFeedColors(playerid);
        cache_get_value_bool(0, "show_random_deathtag", User[playerid][showRandomDeathTags]);
        cache_get_value_bool(0, "hide_vip_tag", User[playerid][hideVipTag]);
        cache_get_value_bool(0, "hide_clan_tag", User[playerid][hideClanTag]);
        cache_get_value_bool(0, "hide_admin_tag", User[playerid][hideAdminTag]);
        cache_get_value_bool(0, "spawn_safehouse", User[playerid][spawnSafehouse]);
        cache_get_value_bool(0, "hide_custom_tag", User[playerid][hideCustomTag]);
        cache_get_value_bool(0, "custom_tag_perm", User[playerid][customTagPerm]);
        User[playerid][customTag][0] = '\0';
        cache_get_value_name(0, "custom_tag", User[playerid][customTag], 33);
        if(User[playerid][vipHeadTag] == 10) User[playerid][hideVipTag] = true;
        cache_get_value_int(0, "hit_sound_id", User[playerid][hitSoundID]);
        cache_get_value_int(0, "kill_sound_id", User[playerid][killSoundID]);
        cache_get_value_bool(0, "auto_login", User[playerid][autoLogin]);
        cache_get_value_int(0, "admin_auth_type", User[playerid][adminAuthType]);
        cache_get_value_name(0, "admin_password", User[playerid][adminPass], BCRYPT_HASH_LENGTH);
		//DISCORD LINK
		cache_get_value_name(0, "dc_username", User[playerid][dc_username], 100);
		cache_get_value_name(0, "dc_discriminator", User[playerid][dc_discriminator], 100);
		cache_get_value_name(0, "dc_uid", User[playerid][dc_uid], 100);
		//ADMIN CONFIG
		cache_get_value_int(0, "acfg_pm", User[playerid][acfg_pm]);
		cache_get_value_int(0, "acfg_clanpm", User[playerid][acfg_clanpm]);
		cache_get_value_int(0, "acfg_cmds", User[playerid][acfg_cmds]);
		cache_get_value_int(0, "acfg_local", User[playerid][acfg_local]);

		cache_get_value_float(0, "lounge_x", User[playerid][loungeX]);
		cache_get_value_float(0, "lounge_y", User[playerid][loungeY]);
		cache_get_value_float(0, "lounge_z", User[playerid][loungeZ]);
		cache_get_value_float(0, "lounge_a", User[playerid][loungeA]);
		cache_get_value_int(0, "lounge_vw", User[playerid][loungeVW]);
		cache_get_value_int(0, "lounge_int", User[playerid][loungeINT]);

		if((User[playerid][loungeX] == 0.0 && User[playerid][loungeY] == 0.0) || User[playerid][loungeX] == -1504.1549 || User[playerid][loungeVW] == 0)
		{
			User[playerid][loungeX] = LOUNGE_SPAWN_X;
			User[playerid][loungeY] = LOUNGE_SPAWN_Y;
			User[playerid][loungeZ] = LOUNGE_SPAWN_Z;
			User[playerid][loungeA] = LOUNGE_SPAWN_A;
			User[playerid][loungeVW] = LOUNGEVW;
			User[playerid][loungeINT] = 0;
		}

		cache_get_value_int(0, "unmute_timestamp", User[playerid][muteTimestamp]);
		cache_get_value_int(0, "played_time", User[playerid][totalOnTime]);

		if(User[playerid][hudColor]  < 0 || User[playerid][hudColor]  >= sizeof(hudPersonalization))  User[playerid][hudColor]  = 0;
		if(User[playerid][chatColor] < 0) User[playerid][chatColor] = 0;
		if(User[playerid][chatIDColor] < 0) User[playerid][chatIDColor] = 0;
		if(User[playerid][vipHeadTag] < 0)                                                            User[playerid][vipHeadTag] = 0;
		if(User[playerid][dmgInformerStyle] < 0 || User[playerid][dmgInformerStyle] > 3)              User[playerid][dmgInformerStyle] = 1;
		if(User[playerid][hudType] < 0 || User[playerid][hudType] >= MAX_HUD_TYPES)                   User[playerid][hudType] = HUD_REJECTING;
		if(User[playerid][adminLevel] < 0 || User[playerid][adminLevel] >= sizeof(adminnames))        User[playerid][adminLevel] = 0;
		if(User[playerid][playerLevel] < 1)                                                          User[playerid][playerLevel] = 1;

		if(User[playerid][clanID] < -1 || User[playerid][clanID] >= MAX_CLANS || (g_ClansLoaded && User[playerid][clanID] != -1 && !ClanInfo[User[playerid][clanID]][clanIDUsed]))
		{
			User[playerid][clanID]   = -1;
			User[playerid][clanRole] = -1;
		}
		else if(User[playerid][clanID] != -1 && (User[playerid][clanRole] < 0 || User[playerid][clanRole] >= MAX_ROLES))
		{
			User[playerid][clanRole] = RANK_TRIAL;
		}

        User[playerid][lastLocation] = -1;

        UpdateRoundStatsColors(playerid);
        UpdateVipHeadTag(playerid);
        SyncPlayerDiscordRoles(playerid);
        CheckPlayerVipOnLogin(playerid);

        // load achievements directly from the account (uses a global buffer to avoid stack overflow)
        g_achSerialBuf[0] = '\0';
        cache_get_value_name(0, "achievements_data", g_achSerialBuf, ACH_SERIAL_BUF_SIZE);
        LoadPlayerAchievements(playerid, g_achSerialBuf);

        new current_gpci[42];
        gpci(playerid, current_gpci, sizeof(current_gpci));
        if(strcmp(current_gpci, User[playerid][user_gpci]) != 0)
        {
            foreach(new i : Player)
            {
                if(IsAdmin(i))
                {
                    va_SendClientMessage(i, -1, "{MAIN}[SECURITY] {FFFFFF}Player %s[%d] logged in with a different Serial (GPCI).", User[playerid][userName], playerid);
                }
            }
            printf("[SECURITY] Player %s logged in with different Serial. Old: %s, New: %s", User[playerid][userName], User[playerid][user_gpci], current_gpci);
        }

		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid, User[playerid][playerMoney]);

		cache_get_value(0, "player_weapons", strweaps, sizeof(strweaps));
		sscanf(strweaps, "p<,>a<i>[4]", User[playerid][playerWeapons]);

		for (new i; i < 4; i ++)
		{
			if (!(0 <= User[playerid][playerWeapons][i] < sizeof(gunmenuWInfo)))
			{
				User[playerid][playerWeapons][i] = 0;
			}
		}

		if(User[playerid][lastLogin] == 0)
			User[playerid][lastLogin] = 1000198120;

		new current_ip[18];
		GetPlayerIp(playerid, current_ip, sizeof(current_ip));
		new bool:isLinked = (strcmp(User[playerid][dc_username], "-1", true) != 0);

		new stored_lastip[18];
		cache_get_value_name(0, "lastip", stored_lastip, sizeof(stored_lastip));

		new bool:ipMatches = (strequal(current_ip, User[playerid][user_regip]) || strequal(current_ip, stored_lastip));

		if(User[playerid][autoLogin] && isLinked && ipMatches && strequal(current_gpci, User[playerid][user_gpci]))
		{
			KillTimer(User[playerid][loginTimer]);
			HideLoginScreen(playerid);
			OnLoginVerify(playerid, true);
			SendServerInfoMessage(playerid, AUTO_LOGIN_SUCCESS);
		}
		else
		{
			showAutorizationDialog(playerid);
		}

    	new string[MAX_PLAYER_NAME + 50];
		format(string, sizeof(string), "{92B5B8}*** {979E9E} %s has joined the server.", User[playerid][userName]);
        
        foreach(new i : Player)
        {
            if(User[i][showConnectMsg])
            {
                SendClientMessage(i, 0xC4C4C4FF, "%s", string);
            }
        }

		new DCC_User:user, userid[20 + 1];
		user = DCC_FindUserById(User[playerid][dc_uid]);
		if (user != DCC_INVALID_USER)
		{
			DCC_GetUserId(user, userid);
			DCC_SetGuildMemberNickname(guildaid, user, getPlayerName(playerid));
		}
		else
		{
			if(!strequal(User[playerid][dc_username], "-1"))
			{
				mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "UPDATE `accounts` SET `dc_username` = '-1', `dc_discriminator` = '-1', `dc_uid` = '-1' WHERE `id` = %d", User[playerid][user_sqlid]);
				mysql_tquery(dbHandle, queryGlobal);

				format(User[playerid][dc_username], 100, "-1");
				format(User[playerid][dc_discriminator], 100, "-1");
				format(User[playerid][dc_uid], 100, "-1");
				format(User[playerid][dc_code], 100, "-1");

				SendClientMessage(playerid, -1, "%s", DC_LEFT);
			}
		}
		return true;
	}

	va_SendClientMessage(playerid, -1, WELCOME_MSG, ServerHostname);

	showRegistrationDialog(playerid);
	return true;
}

UpdatePlayerData(playerid, disconnect = 0)
{
	if(!IsPlayerLogged(playerid) || User[playerid][user_sqlid] <= 0) return 0;
	new weaponArrayStr[128];

	for (new i; i < 4; i ++)
	{
		format(weaponArrayStr, sizeof(weaponArrayStr), "%s%s%d", weaponArrayStr, i ? "," : "", User[playerid][playerWeapons][i]);
	}

	mysql_format(dbHandle, queryGlobal, sizeof (queryGlobal), "UPDATE `accounts` SET\
		`username`='%e',\
		`lastip`='%e',\
		`level`=%d,\
		`kills`=%d,\
		`kills_semanais`=%d,\
		`deaths`=%d,\
		`max_damage`=%d,\
		`damage_semanal`=%d,\
		`max_streak`=%d,\
		`weather`=%d,\
		`time`=%d,\
		`hitmarkcolor`=%d,\
		`hudcolor`=%d,\
		`hud_shade_one`=%d,\
		`hud_shade_two`=%d,\
		`chat_color`=%d,\
		`chat_id_color`=%d,\
		`chat_main_color`=%d,\
		`hud_type`=%d,\
		`life_type`=%d,\
		`adminlevel`=%d,\
		`skin_id`=%d, \
		`fighting_style`=%d, \
		`unmute_timestamp`=%d,\
		`last_login`=%d,\
		`block_pm`=%d,\
		`player_weapons`='%e',\
		`player_money`=%d,\
		`coins`=%d,\
		`warnings`=%d,\
		`played_time`=%d,\
		`won_arenas`=%d, \
		`duel_wins`=%d, \
		`duel_losses`=%d, \
		`duel2v2_wins`=%d, \
		`duel2v2_losses`=%d, \
		`gpci` = '%e',\
		`clanid` = %d,\
		`clanrole` = %d, \
		`country` = COALESCE(NULLIF('%e', ''), `country`), \
		`isp` = COALESCE(NULLIF('%e', ''), `isp`), \
		`city` = COALESCE(NULLIF('%e', ''), `city`), \
		`region` = COALESCE(NULLIF('%e', ''), `region`), \
		`regioncode` = COALESCE(NULLIF('%e', ''), `regioncode`), \
		`countrycode` = COALESCE(NULLIF('%e', ''), `countrycode`), \
		`latitude` = COALESCE(NULLIF('%e', ''), `latitude`), \
		`longitude` = COALESCE(NULLIF('%e', ''), `longitude`), \
		`timezone` = COALESCE(NULLIF('%e', ''), `timezone`), \
		`network` = COALESCE(NULLIF('%e', ''), `network`), \
		`ispnumber` = COALESCE(NULLIF('%e', ''), `ispnumber`), \
		`vpn` = %d, \
		`proxy` = %d, \
		`staticdamage` = %d, \
		`showplayerhud` = %d,\
		`show_clan_hud` = %d,\
        `show_headshot` = %d,\
        `show_connect_msg` = %d,\
        `hide_global_msg` = %d,\
        `hide_welcome_msg` = %d,\
        `show_3d_deathtag` = %d,\
        `dmgfeed_style` = %d,\
		`dmgfeed_color_left` = %d,\
		`dmgfeed_color_right` = %d,\
        `show_random_deathtag` = %d,\
        `hide_vip_tag` = %d,\
        `hide_clan_tag` = %d,\
        `hide_admin_tag` = %d,\
        `spawn_safehouse` = %d,\n        `hide_custom_tag` = %d,\
        `custom_tag_perm` = %d,\
        `custom_tag` = '%e',\
        `hit_sound_id` = %d,\
        `kill_sound_id` = %d,\
		`acfg_pm` = %d,\
		`acfg_clanpm` = %d,\
		`acfg_cmds` = %d,\
		`acfg_local` = %d,\
		`admin_auth_type` = %d,\
		`admin_password` = '%e',\
		`vip_level` = %d,\
		`vip_expire` = %d,\
		`vip_head_tag` = %d,\
		`lounge_x` = %f,\
		`lounge_y` = %f,\
		`lounge_z` = %f,\
		`lounge_a` = %f,\
		`lounge_vw` = %d,\
		`lounge_int` = %d,\
		`is_online` = %d WHERE `ID` = %d",
		User[playerid][userName],
		User[playerid][user_lastip],
		User[playerid][playerLevel],
		User[playerid][playerKills],
		User[playerid][kills_semanais],
		User[playerid][playerDeaths],
		User[playerid][maxDamage],
		User[playerid][dmgSemanal],
		User[playerid][maxStreak],
		User[playerid][weather],
		User[playerid][time],
		User[playerid][hitmarkColor],
		User[playerid][hudColor],
		User[playerid][hudShadeOne],
		User[playerid][hudShadeTwo],
		User[playerid][chatColor],
		User[playerid][chatIDColor],
		User[playerid][chatMainColor],
		User[playerid][hudType],
		User[playerid][lifeType],
		User[playerid][adminLevel],
		(User[playerid][playerSkin] > 0) ? (User[playerid][playerSkin]) : (GetPlayerSkin(playerid)),
		_:User[playerid][fightStyle],
		User[playerid][muteTimestamp],
		gettime(),
		User[playerid][blockPM],
		weaponArrayStr,
		User[playerid][playerMoney],
		User[playerid][playerCoins],
		User[playerid][warnings],
		User[playerid][totalOnTime]+(gettime()-User[playerid][loginTime]),
		User[playerid][wonArenas],
		User[playerid][duelWins],
		User[playerid][duelLosses],
		User[playerid][duel2v2Wins],
		User[playerid][duel2v2Losses],
		User[playerid][user_gpci],
		User[playerid][clanID],
		User[playerid][clanRole],

		// anti vpn
		User[playerid][Country],
		User[playerid][Isp],
		User[playerid][City],
		User[playerid][Region],
		User[playerid][RegionCode],
		User[playerid][CountryCode],
		User[playerid][Latitude],
		User[playerid][Longitude],
		User[playerid][TimeZone],
		User[playerid][Network],
		User[playerid][IspNumber],
		User[playerid][VPN],
		User[playerid][Proxy],

		User[playerid][dmgInformerStyle],
		User[playerid][showPlayerHud],
		User[playerid][showClanHud],
        User[playerid][showHeadshot],
        User[playerid][showConnectMsg],
        User[playerid][hideGlobalMsg],
        User[playerid][hideWelcomeMsg],
        User[playerid][show3DDeathTag],
        User[playerid][dmgFeedStyle],
        User[playerid][dmgFeedColorLeft],
        User[playerid][dmgFeedColorRight],
        User[playerid][showRandomDeathTags],
        User[playerid][hideVipTag],
        User[playerid][hideClanTag],
        User[playerid][hideAdminTag],
        User[playerid][spawnSafehouse],
        User[playerid][hideCustomTag],
        User[playerid][customTagPerm],
        User[playerid][customTag],
        User[playerid][hitSoundID],
        User[playerid][killSoundID],
		User[playerid][acfg_pm],
		User[playerid][acfg_clanpm],
		User[playerid][acfg_cmds],
		User[playerid][acfg_local],
		User[playerid][adminAuthType],
		User[playerid][adminPass],
		User[playerid][vipLevel],
		User[playerid][vipExpire],
		User[playerid][vipHeadTag],
		User[playerid][loungeX],
		User[playerid][loungeY],
		User[playerid][loungeZ],
		User[playerid][loungeA],
		User[playerid][loungeVW],
		User[playerid][loungeINT],
		(disconnect ? (0) : (_:IsPlayerLogged(playerid))),
		User[playerid][user_sqlid]
	);

	mysql_tquery(dbHandle, queryGlobal);

	FlushPlayerAchievements(playerid);
	return true;
}

forward OnPlayerTryChangeNick(playerid, newnick[], oldnick[]);
public OnPlayerTryChangeNick(playerid, newnick[], oldnick[])
{
	if(cache_num_rows() == 0)
	{
		if(User[playerid][playerMoney] >= 300000)
		{
			switch(SetPlayerName(playerid, newnick))
			{
				case -1:
				{
					SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} Invalid nickname entered, try another one");
				}
				case 0:
				{
					SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} You already use this nickname, what the fuck?");
				}
				case 1:{

					va_SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} You changed your nickname to %s!", newnick);
					va_SendClientMessageToAll(COLOR_GYELLOW, "%s changed his nickname to: %s!", oldnick, newnick);
					GivePlayerCash(playerid, -300000);
					LoadPlayerMoney(playerid);
					GetPlayerName(playerid, User[playerid][userName], MAX_PLAYER_NAME + 1);
					UpdatePlayerData(playerid);

					mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO name_changes (uid, old_name, new_name) VALUES (%d, '%e', '%e')", User[playerid][user_sqlid], oldnick, newnick);
					mysql_tquery(dbHandle, queryGlobal);

                    new log_details[256];
                    format(log_details, sizeof(log_details), "Player %s changed name to %s", oldnick, newnick);
                    LogServerAction(User[playerid][user_sqlid], newnick, "names", "Name Change", log_details);

                    new nickMsg[256];
                    format(nickMsg, sizeof(nickMsg), "**Old Name:** %s\n**New Name:** %s\n**Cost:** $300,000", oldnick, newnick);
                    SendDiscordEmbed(logs_nick, "Nickname Change", nickMsg, DISCORD_COLOR_INFO);

					new DCC_User:user = DCC_FindUserByName(User[playerid][dc_username], User[playerid][dc_discriminator]);

					if (user != DCC_INVALID_USER)
					{
						DCC_SetGuildMemberNickname(guildaid, user, getPlayerName(playerid));
					}
				}
			}
		}
		else{
			SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} You need {MAIN}$300,000{FFFFFF} to change your nickname");
		}
	}
	else SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} Someone is already using this nickname!");

	return true;
}

forward OnPlayerChangePass(playerid);
public OnPlayerChangePass(playerid)
{
	new password[BCRYPT_HASH_LENGTH];
	bcrypt_get_hash(password);

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"UPDATE `accounts` SET `password` = '%e' WHERE `id` = %d",
		password, User[playerid][user_sqlid]);

	mysql_tquery(dbHandle, queryGlobal);

	SendClientMessage(playerid, COLOR_KGREEN, "{MAIN}[INFO] {FFFFFF}Congratulations, your password has been changed.");

	return true;
}


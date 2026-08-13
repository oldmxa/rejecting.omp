
@cmd() kick(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new reason[64], targetid;
    if(sscanf(params, "us[64]", targetid, reason))
        return SendClientMessage(playerid,-1, "%s", KICK_HELP);

    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid,-1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    va_SendClientMessageToAll(-1, KICK_REASON, User[playerid][userName], User[targetid][userName], targetid, reason);
    
    new log_details[256];
    format(log_details, sizeof(log_details), "Kicked player %s (ID: %d). Reason: %s", User[targetid][userName], targetid, reason);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "kicks", "Kick", log_details);

    new string[1024];
    format(string, sizeof(string), "**Admin:** %s\n**Target:** %s\n**Reason:** %s",
        User[playerid][userName],
        User[targetid][userName],
        reason);

    SendDiscordEmbed(logs_kick, "New Kick", string, DISCORD_COLOR_WARNING);

    KickEx(targetid, 1000);
    return true;
}

@cmd() ban(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid, reason[120], hours;

    if(sscanf(params, "dds[120]", targetid, hours, reason))
        return SendClientMessage(playerid,-1, "%s", BAN_HELP);

    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    if(User[targetid][OnTempREC] && User[playerid][adminLevel] < 3)
        return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}This player is in a temprec right now.");

    if(hours < 1 || hours > 9999)
        return SendClientMessage(playerid,-1, "%s", BAN_HELP2);

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);

	mysql_tquery(dbHandle, queryGlobal, "BanPlayer", "ddsdd", playerid, targetid, reason, hours, gettime()+hours*3600);

    return true;
}

@cmd() fakeban(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 3)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetStr[32], hours, reason[120];

    if(sscanf(params, "s[32]ds[120]", targetStr, hours, reason))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/fakeban [id/nickname] [hours] [reason]");

    if(hours < 1 || hours > 9999)
        return SendClientMessage(playerid, -1, "%s", BAN_HELP2);

    new targetName[MAX_PLAYER_NAME + 1];
    new targetid = ResolvePlayerTarget(targetStr);

    if(targetid == INVALID_PLAYER_ID && IsNumeric(targetStr))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The specified player ID is not connected or not logged in.");

    if(targetid != INVALID_PLAYER_ID)
        strcopy(targetName, User[targetid][userName], sizeof(targetName));
    else
        strcopy(targetName, targetStr, sizeof(targetName));

    if(targetid != INVALID_PLAYER_ID)
    {
        if(targetid == playerid)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

        if(User[targetid][adminLevel] >= User[playerid][adminLevel])
            return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);
    }

    new banMsg[256];
    format(banMsg, sizeof(banMsg), COL_ADM_PUNISH"[Admin] %s "COL_ADM_PUNISH"%s banned player %s (Reason: %s)", adminnames[User[playerid][adminLevel]][ingame], User[playerid][userName], targetName, reason);
    SendClientMessageToAll(-1, "%s", banMsg);

    if(targetid != INVALID_PLAYER_ID)
    {
        GameTextForPlayer(targetid, "~r~BANNED~w~!", 2500, 3);
        SendBanNotice(targetid);

        KickEx(targetid, 2000);
    }

    return true;
}

@cmd() banr2(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "r", targetid))
        return SendClientMessage(playerid,-1, "%s", FORCE_R2_HELP);

    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(!strequal(User[targetid][playerVersion], "0.3.7"))
        return SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} This player already has R2+.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);
	mysql_tquery(dbHandle, queryGlobal, "ForcePlayerR2", "dd", playerid, targetid);

    return true;
}

@cmd() unbanr2(playerid, params[], help)
{
	if (User[playerid][adminLevel] < 2)
	    return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

	CheckAdminAuth(playerid);

	new player[MAX_PLAYER_NAME + 1];
	if(sscanf(params, "s[25]", player))
		return SendClientMessage(playerid,-1, "%s", UNBAN_R2_HELP);

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT `id` FROM `accounts` WHERE `username` = '%e' AND `forcedr2` = 1 LIMIT 1", player);

	mysql_tquery(dbHandle, queryGlobal, "UnBanR2", "iss", playerid, User[playerid][userName], player);

	return true;
}

@cmd() offban(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new username[25], reason[120], hours;
    if(sscanf(params, "s[25]ds[120]", username, hours, reason))
        return SendClientMessage(playerid,-1, "%s", BANOFF_HELP);

    if(hours < 1 || hours > 99999)
        return SendClientMessage(playerid,-1, "%s", BAN_HELP2);

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", username);

	mysql_tquery(dbHandle, queryGlobal, "OffBanPlayer", "dssdd", playerid, username, reason, hours, gettime()+hours*3600);

    return true;
}

@cmd() mute(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid, reason[64], minutes;

    if(sscanf(params, "uds[64]", targetid, minutes, reason))
        return SendClientMessage(playerid,-1, "%s", MUTE_HELP);

    if(minutes < 1 || minutes > 500)
        return SendClientMessage(playerid,-1, "%s", MUTE_HELP2);

    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid,-1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    if(User[targetid][muteTimestamp] != 0)
        return SendClientMessage(playerid, -1, "{MAIN}[Admin] {FFFFFF} The player is already muted!");

    foreach(new i : Player)
    {
        if(i != targetid)
            va_SendClientMessage(i, -1, MUTE_REASON, User[playerid][userName], playerid, User[targetid][userName], targetid, minutes, reason);
    }
    va_SendClientMessage(targetid, -1, MUTED_PLAYER, User[playerid][userName], playerid,  minutes);

    new log_details[256];
    format(log_details, sizeof(log_details), "Muted player %s (ID: %d) for %d minutes. Reason: %s", User[targetid][userName], targetid, minutes, reason);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "mutes", "Mute", log_details);

    User[targetid][muteTimestamp] = gettime()+minutes*60;

    return true;
}

@cmd() unmute(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid,-1, "%s", UNMUTE_HELP);

    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid,-1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    if(User[targetid][muteTimestamp] == 0)
        return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF} The player is not muted!");

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, UNMUTED_PLAYER, User[playerid][userName], playerid, User[targetid][userName], targetid);
    }
    va_SendClientMessage(targetid, -1, UNMUTED_MSG, User[playerid][userName], playerid);

    new log_details[256];
    format(log_details, sizeof(log_details), "Unmuted player %s (ID: %d)", User[targetid][userName], targetid);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "mutes", "Unmute", log_details);

    User[targetid][muteTimestamp] = 0;

    return true;
}

@cmd() unban(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid,-1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new nameIP[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", nameIP))
		return SendClientMessage(playerid, -1, "%s", UNBAN_HELP);

    mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
		"SELECT `username` FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e' LIMIT 1", nameIP, nameIP);

	mysql_tquery(dbHandle, queryGlobal, "UnBanPlayer", "d", playerid);

    return true;
}

@cmd(.alts = {"gtemp"}) temprec(playerid, params[],  help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "r", targetid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/temprec [id]");

    if (!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    new stringa[250];
    format(stringa, sizeof(stringa),
        ADM_TAG_PUN"%s{FFFFFF}, you have 5 minutes to start recording your screen, after that time passes you will be banned from the server.",
        User[targetid][userName]
    );

    SendClientMessage(targetid, COLOR_LIGHT_BLUE, "%s", stringa);
    GameTextForPlayer(targetid, " ~n~~b~~h~start recording", 12000, 0);

    format(stringa, sizeof(stringa), ADM_TAG_NORM"%s{FFFFFF}, message sent to the player %s, now u just have to wait until he is banned.", User[playerid][userName], User[targetid][userName]);

    SendClientMessage(playerid, COLOR_LIGHT_BLUE, "%s", stringa);
    GameTextForPlayer(playerid, " ~n~~b~~h~message sent", 5000, 1);

    new gTempIP[18], gTempUser[MAX_PLAYER_NAME + 1], adminUser[MAX_PLAYER_NAME + 1], gTempCountry[50 + 1], gTempISP[100 + 1];

	GetPlayerIp(targetid, gTempIP, sizeof(gTempIP));

	format(gTempUser, MAX_PLAYER_NAME + 1, "%s", User[targetid][userName]);
	format(gTempCountry, sizeof(gTempCountry), "%s", User[targetid][Country]);
	format(gTempISP, sizeof(gTempISP), "%s", User[targetid][Isp]);

	format(adminUser, MAX_PLAYER_NAME + 1, "%s", User[playerid][userName]);

    inline const gTempServer()
	{
        inline const OnTempRecChecked()
        {
            if(cache_num_rows())
            {
                SendClientMessage(playerid, -1, ADM_TAG_NORM" This player got banned in the meanwhile of the temprec. The temprec got cancelled.");
            }
            else
            {
                new
                    bantimestamp = gettime(),
                    hours = 9999,
                    unbants = gettime()+hours*3600,
                    reason[16 + 1] = "review - temprec";

                mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
                    "INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d)",
                    gTempUser, gTempIP, adminUser, reason, gTempCountry, gTempISP, bantimestamp, unbants);
                mysql_tquery(dbHandle, queryGlobal);

                va_SendClientMessageToAll(-1, BAN_TEMPREC_REASON, adminUser,  gTempUser, hours, reason);

                if(IsPlayerConnected(targetid) && strequal(User[targetid][userName], gTempUser))
                {
                    SendClientMessage(targetid, -1, "%s", BAN_TEMPREC_MINUTES);
                    SendClientMessage(targetid, -1, BAN_TEMPREC_WHOBANNED, adminUser, reason, gTempIP);
                    User[targetid][OnTempREC] = false;
                    KickEx(targetid, 2000);
                }

                new string[1024];
                format(string, sizeof(string), "**Admin:** %s\n**Target:** %s",
                    adminUser, gTempUser);

                SendDiscordEmbed(logs_ban, "New Ban - GTEMP/TEMPREC", string, DISCORD_COLOR_ERROR);
            }
        }
        MySQL_TQueryInline(dbHandle, using inline OnTempRecChecked,
            "SELECT `id` FROM `banimentos` WHERE `username` = '%e' LIMIT 1", gTempUser);
	}

	Timer_CreateCallback(using inline gTempServer, 300000, 1);
    User[targetid][OnTempREC] = true;
    return true;
}

@cmd() baninfo(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new target[64];
    if(sscanf(params, "s[64]", target)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Use: /baninfo [nick, ID or IP]");

    new targetid = strval(target);
    if(IsNumeric(target) && IsPlayerConnected(targetid))
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
            "SELECT * FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e' OR `gpci` = '%e' LIMIT 1",
            User[targetid][userName], User[targetid][user_lastip], User[targetid][user_gpci]);
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
            "SELECT * FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e' OR `gpci` = '%e' LIMIT 1",
            target, target, target);
    }

    mysql_tquery(dbHandle, queryGlobal, "OnPlayerBanInfo", "i", playerid);
    return true;
}

@cmd() warn(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid, reason[64];
    if(sscanf(params, "us[64]", targetid, reason)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/warn [id] [reason]");
    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself..");
    
    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    User[targetid][warnings]++;
    
    new log_details[256];
    format(log_details, sizeof(log_details), "Gave warning to %s [%d/%d]. Reason: %s", User[targetid][userName], User[targetid][warnings], ServerMaxWarnings, reason);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "warns", "Warn", log_details);

    va_SendClientMessageToAll(-1, ADM_TAG_PUN"Admin %s gave a warning to %s [%d/%d]. reason: %s", User[playerid][userName], User[targetid][userName], User[targetid][warnings], ServerMaxWarnings, reason);
    
    if(User[targetid][warnings] >= ServerMaxWarnings)
    {
        User[targetid][warnings] = 0;

        new hours = 9999;
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);
        new warnReason[48];
        format(warnReason, sizeof(warnReason), "Exceeded warning limit. (%d/%d)", ServerMaxWarnings, ServerMaxWarnings);
        mysql_tquery(dbHandle, queryGlobal, "BanPlayer", "ddsdd", playerid, targetid, warnReason, hours, gettime()+hours*3600);
    }

    UpdatePlayerData(targetid);
    return true;
}

@cmd() banserial(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    if(!IsPlayerAdmin(playerid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be logged into RCON to use this command..");
    new target[32], reason[64];
    if(sscanf(params, "s[32]s[64]", target, reason)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/banserial [id/name] [reason]");
    
    new hours = 9999;
    new targetid = strval(target);
    if(IsNumeric(target) && IsPlayerConnected(targetid))
    {
        if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself..");

        if(User[targetid][adminLevel] >= User[playerid][adminLevel])
            return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);
        mysql_tquery(dbHandle, queryGlobal, "BanPlayer", "ddsdd", playerid, targetid, reason, hours, gettime()+hours*3600);
    }
    else
    {
        if(strequal(target, User[playerid][userName])) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", target);
        mysql_tquery(dbHandle, queryGlobal, "OffBanPlayer", "dssdd", playerid, target, reason, hours, gettime()+hours*3600);
    }
    return true;
}

@cmd() changebanreason(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new target[MAX_PLAYER_NAME + 1], reason[120];
    if(sscanf(params, "s[25]s[120]", target, reason)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/changebanreason [name] [reason]");

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `banimentos` SET `reason` = '%e' WHERE `username` = '%e'", reason, target);
    mysql_tquery(dbHandle, queryGlobal);
    
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You changed the ban reason of %s to: %s", target, reason);
    return true;
}

@cmd() changebantime(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new target[MAX_PLAYER_NAME + 1], amount, unit[2];
    if(sscanf(params, "s[25]is[2]", target, amount, unit)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/changebantime [name] [amount] [h/d]");

    new seconds;
    if(unit[0] == 'h' || unit[0] == 'H') seconds = amount * 3600;
    else if(unit[0] == 'd' || unit[0] == 'D') seconds = amount * 86400;
    else return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid unit. Use 'h' for hours or 'd' for days.");

    new unbantime = gettime() + seconds;
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `banimentos` SET `unban_timestamp` = %d WHERE `username` = '%e'", unbantime, target);
    mysql_tquery(dbHandle, queryGlobal);

    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You changed the ban expiry time of %s. New unban in %d %s.", target, amount, (unit[0] == 'h' ? "hours" : "days"));
    return true;
}

stock ShowPenaltyHistory(playerid, const targetname[], page = 0)
{
    SetPVarString(playerid, "hist_target_name", targetname);
    SetPVarInt(playerid, "hist_page", page);
    SetPVarInt(playerid, "hist_type", 2);

    new limit = 15;
    new offset = page * limit;

    inline const OnPenaltiesSelect()
    {
        new rows = cache_num_rows();
        if (rows > 0)
        {
            new chadString[4096] = "Action\tDetails\tTimestamp";
            new count = (rows > limit) ? limit : rows;

            for (new i = 0; i < count; i++)
            {
                new action[32], details[128], timestamp[32];
                cache_get_value_name(i, "action", action);
                cache_get_value_name(i, "details", details);
                cache_get_value_name(i, "created_at", timestamp);

                strcat(chadString, fmat_return("\n%s\t%s\t%s", action, details, timestamp), sizeof(chadString));
            }

            if (page > 0)
            {
                strcat(chadString, "\n"COL_RED"<<< Previous Page\t\t\tGo back");
            }
            if (rows > limit)
            {
                strcat(chadString, "\n"COL_KGREEN">>> Next Page\t\t\tGo next");
            }

            Dialog_ShowCallback(playerid, using public DIALOG_HISTORY_NAV<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, sprintf("{FF4500}%s's penalty history - Page %d", targetname, page + 1), chadString, "Select", "Close");
        }
        else
        {
            if(page > 0) ShowPenaltyHistory(playerid, targetname, page - 1);
            else Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Information", "{MAIN}This player has no penalty history recorded.", "Close", "");
        }
    }
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `action`, `details`, `created_at` FROM `server_logs` WHERE `username` = '%e' ORDER BY `created_at` DESC LIMIT %d OFFSET %d", targetname, limit + 1, offset);
    MySQL_TQueryInline(dbHandle, using inline OnPenaltiesSelect, queryGlobal);
    return true;
}

@cmd() penalties(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new target[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", target)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/penalties [name]");

    ShowPenaltyHistory(playerid, target);
    return true;
}

@cmd() bannick(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new target[MAX_PLAYER_NAME + 1], reason[64];
    if(sscanf(params, "s[25]s[64]", target, reason)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/bannick [nick] [reason]");

    if(strequal(target, User[playerid][userName])) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot ban yourself.");

    new hours = 999;
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", target);
    mysql_tquery(dbHandle, queryGlobal, "OffBanPlayer", "dssdd", playerid, target, reason, hours, gettime()+hours*3600);
    return true;
}

@cmd() unbannick(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new target[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", target)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/unbannick [nick]");

    mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "SELECT `username` FROM `banimentos` WHERE `username` = '%e' LIMIT 1", target);
	mysql_tquery(dbHandle, queryGlobal, "UnBanPlayer", "d", playerid);
    return true;
}

@cmd() unbangpci(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new serial_gpci[41];
    if(sscanf(params, "s[41]", serial_gpci)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/unbangpci [serial]");

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "DELETE FROM `banimentos` WHERE `gpci` = '%e'", serial_gpci);
    mysql_tquery(dbHandle, queryGlobal);
    
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You unbanned the serial: "COL_WHITE"%s", serial_gpci);
    return true;
}

@cmd() slimban(playerid, params[], help)
{
	if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

	CheckAdminAuth(playerid);

	new acName[64];
	format(acName, sizeof(acName), "%s-AC", ServerHostname);

	new targetid, hours = 9999;
	if(!sscanf(params, "uI(9999)", targetid, hours))
	{
		if(!IsPlayerConnected(targetid))
			return SendClientMessage(playerid, COLOR_TOMATO, "%s", IS_NOT_CONNECTED);

		if(targetid == playerid)
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot ban yourself.");

		if(User[targetid][adminLevel] >= User[playerid][adminLevel])
			return SendClientMessage(playerid, COLOR_TOMATO, "%s", SUPERIOR_RIGHTS);

		if(hours < 1 || hours > 9999)
			return SendClientMessage(playerid, -1, "%s", BAN_HELP2);

		mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);
		mysql_tquery(dbHandle, queryGlobal, "BanPlayerAC", "ddssdd", playerid, targetid, acName, "Cheater", hours, gettime() + hours * 3600);
		return 1;
	}

	new username[MAX_PLAYER_NAME + 1];
	if(!sscanf(params, "s[25]I(9999)", username, hours))
	{
		if(hours < 1 || hours > 9999)
			return SendClientMessage(playerid, -1, "%s", BAN_HELP2);

		if(strequal(username, User[playerid][userName], true))
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot ban yourself.");

		mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", username);
		mysql_tquery(dbHandle, queryGlobal, "OffBanPlayerAC", "dsssdd", playerid, username, acName, "Cheater", hours, gettime() + hours * 3600);
		return 1;
	}

	return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/slimban [id/name] [hours (optional)]");
}

@cmd() banisp(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, ADM_TAG_NORM"/banisp [id]");
    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(isnull(User[targetid][Isp])) return SendClientMessage(playerid, -1, ADM_TAG_NORM"Player has no ISP data.");

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `isp` FROM `ispsbanidas` WHERE `isp`='%e'", User[targetid][Isp]);
    inline OnBanISPCheck()
    {
        if(cache_num_rows() > 0)
        {
            SendClientMessage(playerid, -1, ADM_TAG_NORM"This ISP is already banned.");
        }
        else
        {
            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO `ispsbanidas` (`isp`) VALUES ('%e')", User[targetid][Isp]);
            mysql_tquery(dbHandle, queryGlobal);

            va_SendClientMessage(playerid, -1, ADM_TAG_PUN"You banned the ISP: %s", User[targetid][Isp]);
            
            new log_details[128];
            format(log_details, sizeof(log_details), "Banned ISP: %s (Player: %s)", User[targetid][Isp], User[targetid][userName]);
            LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "bans", "BanISP", log_details);
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnBanISPCheck, queryGlobal);
    return true;
}

@cmd() unbanisp(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new isp[100];
    if(sscanf(params, "s[100]", isp)) return SendClientMessage(playerid, -1, ADM_TAG_NORM"/unbanisp [isp name]");

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `isp` FROM `ispsbanidas` WHERE `isp`='%e'", isp);
    inline OnUnbanISPCheck()
    {
        if(cache_num_rows() == 0)
        {
            SendClientMessage(playerid, -1, ADM_TAG_NORM"This ISP is not banned.");
        }
        else
        {
            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "DELETE FROM `ispsbanidas` WHERE `isp`='%e'", isp);
            mysql_tquery(dbHandle, queryGlobal);

            va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You unbanned the ISP: "COL_WHITE"%s", isp);

            new log_details[128];
            format(log_details, sizeof(log_details), "Unbanned ISP: %s", isp);
            LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "unbans", "UnbanISP", log_details);
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnUnbanISPCheck, queryGlobal);
    return true;
}

@cmd(.alts = {"resetacc"}) resetaccount(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need level 4 + RCON to use this command.");

    CheckAdminAuth(playerid);

    new target[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", target))
        return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/resetaccount [id/name]");

    new targetid = strval(target);
    if(IsNumeric(target) && IsPlayerConnected(targetid))
    {
        if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");
        User[targetid][playerKills] = 0;
        User[targetid][kills_semanais] = 0;
        User[targetid][playerDeaths] = 0;
        User[targetid][playerExp] = 0;
        User[targetid][playerLevel] = 1;
        User[targetid][playerMoney] = 0;
        User[targetid][maxDamage] = 0;
        User[targetid][dmgSemanal] = 0;
        User[targetid][maxStreak] = 0;
        User[targetid][wonArenas] = 0;
        User[targetid][warnings] = 0;
        User[targetid][totalOnTime] = 0;

        UpdatePlayerData(targetid);
        
        va_SendClientMessageToAll(-1, ADM_TAG_NORM"Admin %s reset the account of %s.", User[playerid][userName], User[targetid][userName]);
        KickEx(targetid, 2000);
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), 
            "UPDATE `accounts` SET `kills` = 0, `kills_semanais` = 0, `deaths` = 0, `level` = 1, `player_money` = 0, `max_damage` = 0, `damage_semanal` = 0, `max_streak` = 0, `won_arenas` = 0, `warnings` = 0, `played_time` = 0 WHERE `username` = '%e'", 
            target);
        mysql_tquery(dbHandle, queryGlobal);
        
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You reset the account (offline) of %s.", target);
    }

    format(msgdiscord, sizeof(msgdiscord), "**Admin:** %s\n**Target:** %s", User[playerid][userName], target);
    utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
    SendDiscordEmbed(adminchat, "Account Reset", msgencoded, DISCORD_COLOR_ERROR);
    return true;
}

@cmd() viewpass(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need level 4 + RCON to use this command.");

    CheckAdminAuth(playerid);

    new target[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", target))
        return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/viewpass [id/name]");

    new log_details[192];
    format(log_details, sizeof(log_details), "Viewed the password hash of %s", target);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "security", "ViewPass", log_details);

    new targetid = strval(target);
    if(IsNumeric(target) && IsPlayerConnected(targetid))
    {
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Password (Hash) of %s: {FFFFFF}%s", User[targetid][userName], User[targetid][user_pass]);
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `password` FROM `accounts` WHERE `username` = '%e' LIMIT 1", target);
        inline OnShowPassOffline()
        {
            if(cache_num_rows() > 0)
            {
                new pass[BCRYPT_HASH_LENGTH];
                cache_get_value_name(0, "password", pass);
                va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Password (Hash) of %s: {FFFFFF}%s", target, pass);
            }
            else SendClientMessage(playerid, -1, ADM_TAG_NORM"Account not found.");
        }
        MySQL_TQueryInline(dbHandle, using inline OnShowPassOffline, queryGlobal);
    }
    return true;
}


@cmd() get(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 3)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "%s", GET_HELP);

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(IsPlayerInTournamentMatch(targetid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}This player is currently fighting in a tournament match!");

    new Float:ppos[3];
    GetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]);
    SetPlayerPos(targetid, ppos[0], ppos[1], ppos[2]+3);
    SetPlayerVirtualWorld(targetid, GetPlayerVirtualWorld(playerid));
    PlayerPlaySound(targetid, 1190, 0.0, 0.0, 0.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, ADM_TAG_NORM"%s brought %s to his position. ", User[playerid][userName], User[targetid][userName]);
    }
    va_SendClientMessage(targetid, -1, ADM_TAG_NORM"%s brought him to his position!", User[playerid][userName]);
    return true;
}

@cmd() go(playerid, params[], help)
{
    if (User[playerid][adminLevel] < 3)
    {
        new cmdBuf[128];
        format(cmdBuf, sizeof(cmdBuf), "/acceptduel %s", params);
        return Command_ReProcess(playerid, cmdBuf, 0);
    }

    new targetid = -1;
    if(!isnull(params))
    {
        #if defined sscanf
            sscanf(params, "u", targetid);
        #else
            targetid = strval(params);
        #endif
    }

    if(targetid != -1 && IsPlayerConnected(targetid) && GetPVarInt(targetid, "DuelS_State") == DUELS_STATE_WAITING && GetPVarInt(playerid, "DuelS_State") == DUELS_STATE_NONE)
    {
        new cmdBuf[128];
        format(cmdBuf, sizeof(cmdBuf), "/acceptduel %s", params);
        return Command_ReProcess(playerid, cmdBuf, 0);
    }

    CheckAdminAuth(playerid);

    if(targetid == -1 || !IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", GOON_HELP);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    new Float:ppos[3];
    GetPlayerPos(targetid, ppos[0], ppos[1], ppos[2]);
    SetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]+3);
    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
    PlayerPlaySound(targetid, 1190, 0.0, 0.0, 0.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, ADM_TAG_NORM"%s went to %s's position. ", User[playerid][userName], User[targetid][userName]);
    }
    va_SendClientMessage(targetid, -1, ADM_TAG_NORM"%s came to your position!", User[playerid][userName]);
    return true;
}

@cmd() slap(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);
    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "%s", SLAP_HELP);

    if (!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    new Float:ppos[3];
    GetPlayerPos(targetid, ppos[0], ppos[1], ppos[2]);
    SetPlayerPos(targetid, ppos[0], ppos[1], ppos[2]+5);
    PlayerPlaySound(targetid, 1190, 0.0, 0.0, 0.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, ADM_TAG_PUN"%s slapped %s", User[playerid][userName], User[targetid][userName]);
    }
    va_SendClientMessage(targetid, -1, SLAP_MSG, User[playerid][userName]);
    return true;
}

@cmd() freeze(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "%s", FREEZE_HELP);

    if (!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    TogglePlayerControllableEx(targetid, false);
    PlayerPlaySound(targetid, 17002, 0.0, 0.0, 0.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, ADM_TAG_PUN"%s froze %s", User[playerid][userName], User[targetid][userName]);
    }

    va_SendClientMessage(targetid, -1, FREEZE_MSG, User[playerid][userName]);
    return true;
}

@cmd() unfreeze(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);
    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, -1, "%s", UNFREEZE_HELP);

    if (!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    TogglePlayerControllableEx(targetid, true);
    PlayerPlaySound(targetid, 1190, 0.0, 0.0, 0.0);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, ADM_TAG_NORM"%s unfroze %s", User[playerid][userName], User[targetid][userName]);
    }
    va_SendClientMessage(targetid, -1, UNFREEZE_MSG_ADM, User[playerid][userName]);
    return true;
}

@cmd() spawn(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid;
    if(sscanf(params, "r", targetid))
        return SendClientMessage(playerid, -1, "%s", SPAWN_HELP);

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid != playerid && User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    SpawnPlayerEx(targetid);
    va_SendClientMessage(targetid, -1, SPAWN_MSG, User[playerid][userName]);
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You spawned %s.", User[targetid][userName]);
    return true;
}

@cmd(.alts = {"namecolor", "colorname"}) nickcolor(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new color;
    if(sscanf(params, "x", color)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/nickcolor [hex color or FFFF00]");
    
    if(color <= 0xFFFFFF) color = (color << 8) | 0x65;
    else color = (color & 0xFFFFFF00) | 0x65;

    SetPlayerColor(playerid, color);
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Your nickname color has been changed.!");
    return true;
}

@cmd() setcoins(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 5) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    new target[MAX_PLAYER_NAME + 1], amount;
    if(sscanf(params, "s[25]d", target, amount))
        return SendClientMessage(playerid, -1, "%s", COINS_SET_HELP);

    if(amount < 0)
        return SendClientMessage(playerid, -1, "%s", COINS_SET_NEGATIVE);

    new targetid = ResolvePlayerTarget(target);

    if(targetid != INVALID_PLAYER_ID)
    {
        new previous = User[targetid][playerCoins];
        User[targetid][playerCoins] = amount;

        new setReason[64];
        format(setReason, sizeof(setReason), "Set by %s", User[playerid][userName]);
        LogCoins(User[targetid][user_sqlid], User[targetid][userName], setReason, amount - previous, amount);

        UpdatePlayerData(targetid);

        va_SendClientMessage(playerid, -1, COINS_SET_DONE, User[targetid][userName], FormatCoins(previous), FormatCoins(amount));
        va_SendClientMessage(targetid, -1, COINS_SET_TARGET, User[playerid][userName], FormatCoins(amount));

        new log_online[192];
        format(log_online, sizeof(log_online), "%s set Coins of %s from %d to %d",
            User[playerid][userName], User[targetid][userName], previous, amount);
        LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "coins", "Set Coins", log_online);
        return true;
    }

    inline OnFindCoinsAccount()
    {
        if(!cache_num_rows())
            return va_SendClientMessage(playerid, -1, COINS_SET_NOACC, target);

        new accountName[MAX_PLAYER_NAME + 1];
        cache_get_value_name(0, "username", accountName, sizeof(accountName));

        new upd[192];
        mysql_format(dbHandle, upd, sizeof(upd),
            "UPDATE `accounts` SET `coins` = %d WHERE `username` = '%e'", amount, accountName);
        mysql_tquery(dbHandle, upd);

        va_SendClientMessage(playerid, -1, COINS_SET_OFFLINE, accountName, FormatCoins(amount));

        new log_off[192];
        format(log_off, sizeof(log_off), "%s set Coins of offline account %s to %d",
            User[playerid][userName], accountName, amount);
        LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "coins", "Set Coins", log_off);
        return 1;
    }

    new findQuery[160];
    mysql_format(dbHandle, findQuery, sizeof(findQuery),
        "SELECT `username` FROM `accounts` WHERE `username` = '%e' LIMIT 1", target);

    MySQL_TQueryInline(dbHandle, using inline OnFindCoinsAccount, findQuery);
    return true;
}

@cmd() setmoney(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid, amount;
    if(sscanf(params, "ud", targetid, amount)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/setmoney [id] [money]");
    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(amount < 0 || amount > 1000000000)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Money amount must be between 0 and 1,000,000,000.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    ResetPlayerMoney(targetid);
    GivePlayerMoney(targetid, amount);
    User[targetid][playerMoney] = amount;
    UpdatePlayerData(targetid);

    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You set the money from %s to %s.", User[targetid][userName], FormatMoney(amount));
    va_SendClientMessage(targetid, -1, ADM_TAG_NORM"The admin %s set their money to %s.", User[playerid][userName], FormatMoney(amount));

    new mLog[256];
    format(mLog, sizeof(mLog), "**Admin:** %s\n**Player:** %s\n**New Balance:** $%s",
        User[playerid][userName], User[targetid][userName], FormatMoney(amount));
    SendDiscordEmbed(logs_money, "Admin Money Set", mLog, DISCORD_COLOR_ADMIN);
    return true;
}

@cmd(.alts = {"camu", "hidetab", "tabhide"}) camouflage(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    User[playerid][adminCamu] = !User[playerid][adminCamu];
    
    if(User[playerid][adminCamu])
    {
        SetPVarInt(playerid, "HideTab", 1);
        SetPlayerColor(playerid, 0x00000000);
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Your camouflage is now {23C552}ON{FFFFFF} (Hidden from TAB, Radar, and /admins).");
    }
    else
    {
        DeletePVar(playerid, "HideTab");
        SetClanColor(playerid);
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Your camouflage is now {F84F31}OFF{FFFFFF} (Visible).");
    }
    return true;
}

@cmd() forcetext(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid, text[128];
    if(sscanf(params, "us[128]", targetid, text)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/forcetext [id] [text]");
    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    OnPlayerText(targetid, text);
    return true;
}

@cmd() setvw(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid, vw;
    if(sscanf(params, "ui", targetid, vw)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/setvw [id] [vw]");
    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(vw < 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Virtual world cannot be negative.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    SetPlayerVirtualWorld(targetid, vw);
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You set %s's virtual world to %d.", User[targetid][userName], vw);
    return true;
}

@cmd() setinterior(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    new targetid, interior;
    if(sscanf(params, "ui", targetid, interior)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/setinterior [id] [interior]");
    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged]) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if(targetid == playerid) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    if(interior < 0 || interior > 255)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Interior ID must be between 0 and 255.");

    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
        return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

    SetPlayerInterior(targetid, interior);
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You set %s's interior to %d.", User[targetid][userName], interior);
    return true;
}

@cmd() fake(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);
    
    if(isnull(params))
    {
        SetPlayerName(playerid, User[playerid][userName]);
        va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Your nick has been reset to %s.", User[playerid][userName]);
        return true;
    }
    new fakenick[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[25]", fakenick)) return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/fake [nick] (Leave empty to reset)");
    
    if(SetPlayerName(playerid, fakenick) == -1)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This nick is already in use or invalid.");
    }
        
    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Your nick has been changed to %s.", fakenick);
    return true;
}

@cmd() fakecmd(playerid, params[], help)
{
	if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

	CheckAdminAuth(playerid);

	new targetid, cmdtext[128];
	if(sscanf(params, "us[128]", targetid, cmdtext))
		return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/fakecmd [id/name] [command]");

	if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
		return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

	if(targetid == playerid)
		return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

	if(User[targetid][adminLevel] >= User[playerid][adminLevel] && targetid != playerid)
		return SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);

	new fullCmd[140];
	if(cmdtext[0] != '/')
		format(fullCmd, sizeof(fullCmd), "/%s", cmdtext);
	else
		format(fullCmd, sizeof(fullCmd), "%s", cmdtext);

	Command_ReProcess(targetid, fullCmd, 0);

	va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Forced player %s (ID %d) to execute command: %s", User[targetid][userName], targetid, fullCmd);

	new log_details[256];
	format(log_details, sizeof(log_details), "Forced %s (ID %d) to run: %s", User[targetid][userName], targetid, fullCmd);
	LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "FakeCmd", log_details);

	return 1;
}

@cmd(.alts = {"giveweapon"}) givegun(playerid, params[], help)
{
	if(User[playerid][adminLevel] < 4) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

	CheckAdminAuth(playerid);

	new targetid, weaponStr[32], ammo;
	if(sscanf(params, "us[32]d", targetid, weaponStr, ammo))
		return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/givegun [id/name] [weaponid/name] [ammo]");

	if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
		return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

	new weapid = -1;
	if(IsNumeric(weaponStr))
	{
		weapid = strval(weaponStr);
	}
	else
	{
		weapid = GetWeaponModelIDFromName(weaponStr);
	}

	if(weapid < 1 || weapid > 46)
		return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid weapon ID or name.");

	if(ammo < 1 || ammo > 9999)
		return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Ammo must be between 1 and 9999.");

	GivePlayerWeapon(targetid, WEAPON:weapid, ammo);

	new weapName[32];
	GetWeaponName(WEAPON:weapid, weapName, sizeof(weapName));

	va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You gave %s (ID %d) a %s (%d ammo).", User[targetid][userName], targetid, weapName, ammo);
	va_SendClientMessage(targetid, -1, "{MAIN}[ADMIN] {FFFFFF}Administrator {MAIN}%s {FFFFFF}gave you a {MAIN}%s {FFFFFF}with {MAIN}%d {FFFFFF}ammo.", User[playerid][userName], weapName, ammo);

	new log_details[256];
	format(log_details, sizeof(log_details), "Gave weapon %s (%d) with %d ammo to %s (ID %d)", weapName, weapid, ammo, User[targetid][userName], targetid);
	LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "GiveGun", log_details);

	return 1;
}

@cmd(.alts = {"alert"}) al(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NEED_ADMIN_LEVEL);

    CheckAdminAuth(playerid);

    new targetid, message[128];
    if(sscanf(params, "us[128]", targetid, message))
        return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/al [id/name] [message]");

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");

    va_SendClientMessage(targetid, -1, "{MAIN}[SERVER ALERT] {FFFFFF}%s", message);

    new gtStr[140];
    format(gtStr, sizeof(gtStr), "~r~ALERT:~n~~w~%s", message);
    GameTextForPlayer(targetid, gtStr, 5000, 3);
    PlayerPlaySound(targetid, 1085, 0.0, 0.0, 0.0);

    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"Alert sent to %s (ID %d): %s", User[targetid][userName], targetid, message);

    new log_details[256];
    format(log_details, sizeof(log_details), "Alert sent to %s (ID %d): %s", User[targetid][userName], targetid, message);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "admin_cmd", "Alert", log_details);

    return 1;
}

forward OnAdminSetNickCheck(playerid, targetid, const newnick[], const oldnick[]);
public OnAdminSetNickCheck(playerid, targetid, const newnick[], const oldnick[])
{
    if(cache_num_rows() > 0)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This nickname is already registered by another account.");
    }

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    }

    switch(SetPlayerName(targetid, newnick))
    {
        case -1:
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid nickname or already in use by another player.");
        case 0:
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Player already has this nickname.");
        case 1:
        {
            format(User[targetid][userName], MAX_PLAYER_NAME + 1, "%s", newnick);
            UpdatePlayerData(targetid);

            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO name_changes (uid, old_name, new_name) VALUES (%d, '%e', '%e')", User[targetid][user_sqlid], oldnick, newnick);
            mysql_tquery(dbHandle, queryGlobal);

            if(!strequal(User[targetid][dc_username], "-1"))
            {
                new DCC_User:user;
                user = DCC_FindUserByName(User[targetid][dc_username], User[targetid][dc_discriminator]);
                if (user != DCC_INVALID_USER)
                {
                    DCC_SetGuildMemberNickname(guildaid, user, newnick);
                }
            }

            va_SendClientMessage(targetid, -1, "{MAIN}[ADMIN] {FFFFFF}Administrator {MAIN}%s {FFFFFF}changed your nickname from {MAIN}%s {FFFFFF}to {MAIN}%s{FFFFFF}.", User[playerid][userName], oldnick, newnick);
            va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You changed %s's nickname to: %s.", oldnick, newnick);
            va_SendClientMessageToAll(COLOR_GYELLOW, "[ADMIN] %s's nickname was changed to: %s by Administrator %s!", oldnick, newnick, User[playerid][userName]);

            new log_details[256];
            format(log_details, sizeof(log_details), "Admin %s changed %s's name to %s (No cost)", User[playerid][userName], oldnick, newnick);
            LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "names", "Admin SetNick", log_details);

            new nickMsg[256];
            format(nickMsg, sizeof(nickMsg), "**Admin:** %s\n**Old Name:** %s\n**New Name:** %s", User[playerid][userName], oldnick, newnick);
            SendDiscordEmbed(logs_nick, "Admin Nickname Change", nickMsg, DISCORD_COLOR_ADMIN);
        }
    }

    return 1;
}

forward OnAdminOfflineSetNickCheck(playerid, const oldnick[], const newnick[]);
public OnAdminOfflineSetNickCheck(playerid, const oldnick[], const newnick[])
{
    if(cache_num_rows() > 0)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This new nickname is already registered by another account.");
    }

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", oldnick);
    mysql_tquery(dbHandle, queryGlobal, "OnAdminOfflineSetNickExec", "dss", playerid, oldnick, newnick);
    return 1;
}

forward OnAdminOfflineSetNickExec(playerid, const oldnick[], const newnick[]);
public OnAdminOfflineSetNickExec(playerid, const oldnick[], const newnick[])
{
    if(cache_num_rows() == 0)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Account not found with nickname: %s.", oldnick);
    }

    new uid;
    cache_get_value_int(0, "id", uid);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `username` = '%e' WHERE `id` = %d", newnick, uid);
    mysql_tquery(dbHandle, queryGlobal);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO name_changes (uid, old_name, new_name) VALUES (%d, '%e', '%e')", uid, oldnick, newnick);
    mysql_tquery(dbHandle, queryGlobal);

    new log_details[256];
    format(log_details, sizeof(log_details), "Admin %s changed offline account %s's name to %s", User[playerid][userName], oldnick, newnick);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "names", "Admin SetNick Offline", log_details);

    new nickMsg[256];
    format(nickMsg, sizeof(nickMsg), "**Admin:** %s\n**Old Name:** %s (Offline)\n**New Name:** %s", User[playerid][userName], oldnick, newnick);
    SendDiscordEmbed(logs_nick, "Admin Nickname Change", nickMsg, DISCORD_COLOR_ADMIN);

    va_SendClientMessage(playerid, -1, ADM_TAG_NORM"You changed offline account %s's nickname to: %s.", oldnick, newnick);
    return 1;
}

@cmd(.alts = {"setname"}) setnick(playerid, params[], help)
{
    if(User[playerid][adminLevel] < 4 || !IsPlayerAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Level 4+ and RCON login required.");

    CheckAdminAuth(playerid);

    new targetid, newnick[MAX_PLAYER_NAME + 1], oldnick[MAX_PLAYER_NAME + 1];
    new isOnline = 1;

    if(sscanf(params, "us[25]", targetid, newnick))
    {
        if(sscanf(params, "s[25]s[25]", oldnick, newnick))
            return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}/setnick [id/name] [new_nick]");
        isOnline = 0;
    }

    if(strlen(newnick) < 3 || strlen(newnick) > 20)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Nickname must be between 3 and 20 characters.");

    for(new i = 0; newnick[i] != EOS; ++i)
    {
        switch(newnick[i])
        {
            case '0'..'9', 'A'..'Z', 'a'..'z', '_', '[', ']', '(', ')', '.', '@': continue;
            default: return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The nickname contains invalid characters.");
        }
    }

    if(isOnline)
    {
        if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
            return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

        format(oldnick, sizeof(oldnick), "%s", User[targetid][userName]);

        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", newnick);
        mysql_tquery(dbHandle, queryGlobal, "OnAdminSetNickCheck", "ddss", playerid, targetid, newnick, oldnick);
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", newnick);
        mysql_tquery(dbHandle, queryGlobal, "OnAdminOfflineSetNickCheck", "dss", playerid, oldnick, newnick);
    }

    return 1;
}

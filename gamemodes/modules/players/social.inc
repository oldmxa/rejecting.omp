
@cmd() report(playerid, params[], help)
{
    if(User[playerid][adminLevel] > 0)
        return SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Administrators cannot use player reports.");

    if(GetPVarInt(playerid, "report_cooldown"))
        return SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Please wait 30 seconds before sending another report.");

    new targetid, reason[100];
	if(sscanf(params, "us[100]", targetid, reason))
		return SendClientMessage(playerid, -1, "%s", REPORT_HELP);
	if(!IsPlayerLogged(targetid))
		return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
	if(targetid == playerid)
		return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_SELF);
	if(User[targetid][adminLevel] > 0)
		return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_ADMIN);

    SetPVarInt(playerid, "report_cooldown", 1);
    SetTimerEx("ReleaseReport", 30000, false, "i", playerid);

	foreach(new i : Player)
    {
		if(IsAdmin(i))
        {
			if(IsPlayerVip(playerid))
			{
				new vipTagStr[32] = "";
				GetVipTagStringEx(playerid, vipTagStr, sizeof(vipTagStr), true);
				va_SendClientMessage(i, -1, "{MAIN}[VIP-REPORT] %s{FF8C00}Player %s[%d] reported: %s[%d] | Reason: %s", vipTagStr, User[playerid][userName], playerid, User[targetid][userName], targetid, reason);
				GameTextForPlayer(i, "~y~~h~VIP REPORT", 3000, 1);
			}
			else
			{
				va_SendClientMessage(i, -1, "{MAIN}[REPORT] Player %s[%d] reported: %s[%d] | Reason: %s", User[playerid][userName], playerid, User[targetid][userName], targetid, reason);
				GameTextForPlayer(i, "~r~~h~NEW REPORT", 3000, 1);
			}
            PlaySuccessSound(i);
		}
	}
	va_SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Your report against {MAIN}%s {FFFFFF}has been sent successfully to the Administration!", User[targetid][userName]);
    PlayErrorSound(playerid);
    
    LogTicket(User[playerid][user_sqlid], User[targetid][userName], reason);

    new log_details[256];
    format(log_details, sizeof(log_details), "Player %s reported %s. Reason: %s", User[playerid][userName], User[targetid][userName], reason);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "reports", "Report", log_details);

	new string[1024];
    format(string, sizeof(string), "**Reporter:** %s\n**Target:** %s\n**Reason:** %s\n\n**Reply:** `.r %d [message]` or `/r %d [message]`",
        User[playerid][userName],
        User[targetid][userName],
        reason,
        playerid,
        playerid);

    SendDiscordEmbed(logs_report, "New Report", string, DISCORD_COLOR_WARNING);

    return true;
}


@cmd() ask(playerid, params[], help)
{
    if(User[playerid][adminLevel] > 0)
        return SendClientMessage(playerid, -1, "{MAIN}[ASK] {FFFFFF}Administrators cannot submit questions via /ask.");

    if(GetPVarInt(playerid, "ask_cooldown"))
        return SendClientMessage(playerid, -1, "{MAIN}[ASK] {FFFFFF}Please wait 30 seconds before sending another question.");

    if(isnull(params))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}/ask [your question]");

    SetPVarInt(playerid, "ask_cooldown", 1);
    SetTimerEx("ReleaseAsk", 30000, false, "i", playerid);

    foreach(new i : Player)
    {
        if(User[i][adminLevel] > 0)
        {
            if(IsPlayerVip(playerid))
            {
                new vipTagStr[32] = "";
                GetVipTagStringEx(playerid, vipTagStr, sizeof(vipTagStr), true);
                va_SendClientMessage(i, -1, "{MAIN}[VIP ASK] %s{FF8C00}Question from %s[%d]: %s", vipTagStr, User[playerid][userName], playerid, params);
                GameTextForPlayer(i, "~y~~h~VIP QUESTION", 2000, 1);
            }
            else
            {
                va_SendClientMessage(i, -1, "{MAIN}[ASK] Question from %s[%d]: %s", User[playerid][userName], playerid, params);
                GameTextForPlayer(i, "~b~~h~NEW QUESTION", 2000, 1);
            }
            PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
        }
    }

    SendClientMessage(playerid, -1, "{MAIN}[ASK] {FFFFFF}Your question has been sent to the Administration, please wait for a response!");
    
    new log_details[256];
    format(log_details, sizeof(log_details), "Player %s asked: %s", User[playerid][userName], params);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "asks", "Ask", log_details);

    if(logs_ask != DCC_INVALID_CHANNEL)
    {
        DCC_SendChannelMessage(logs_ask, "<@&1460104459811033361>");

        new locName[32];
        new locid = User[playerid][locationId];
        if(locid >= 0 && locid < MAX_LOCATIONS) format(locName, sizeof(locName), "%s", ROOM_NAME[locid][E_ROOM_NAME_DISCORD]);
        else format(locName, sizeof(locName), "Spawn / Lounge");

        new askStr[768], askEncoded[768];
        format(askStr, sizeof(askStr), 
            "**User:** %s (ID: %d)\n" \
            "**Question:** %s\n\n" \
            "**Location:** %s\n" \
            "**Level:** %d | **Money:** $%d\n" \
            "**Ping:** %d ms | **FPS:** %d\n\n" \
            "**Reply:** `.d %d [message]` or `/d %d [message]`", 
            User[playerid][userName], playerid,
            params,
            locName,
            User[playerid][playerLevel], User[playerid][playerMoney],
            GetPlayerPing(playerid), User[playerid][FPS],
            playerid, playerid);

        utf8encode(askEncoded, askStr, sizeof(askEncoded));
        SendDiscordEmbed(logs_ask, "New Question - /ask", askEncoded, DISCORD_COLOR_INFO);
    }
    return true;
}
@cmd() pm(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new idplayer, text[128], tString[128];
    if (sscanf(params, "us[128]", idplayer, text))
        return SendClientMessage(playerid, -1, "%s", PM_HELP);
    if (!IsPlayerLogged(idplayer))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
    if (idplayer == playerid)
        return SendClientMessage(playerid, -1, "%s", RETARD);
    if(User[idplayer][blockPM])
    	return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This player has blocked PM messages.");
    if (IsNull(text))
        return SendClientMessage(playerid, -1, "%s", PM_HELP);

    format(tString, sizeof(tString), PM_SEND, getPlayerName(idplayer), idplayer, text);
    SendClientMessage(playerid, COLOR_GYELLOW, "%s", tString);

    format(tString, sizeof(tString), PM_RECIVED, getPlayerName(playerid), playerid, text);
    SendClientMessage(idplayer, COLOR_GYELLOW, "%s", tString);

    GameTextForPlayer(playerid, "~b~~h~~h~Message Sent", 1500, 1);
	GameTextForPlayer(idplayer, "~y~~h~~h~Message Received", 1500, 1);

    PlayerPlaySound(idplayer, 1057, 0.0, 0.0, 0.0);
    PlayerPlaySound(playerid, 5201, 0.0, 0.0, 0.0);

    new chat_details[256];
    format(chat_details, sizeof(chat_details), "To %s: %s", User[idplayer][userName], text);
    LogChatMessage(User[playerid][user_sqlid], User[playerid][userName], "pm", chat_details);
    LogDiscordPM(playerid, idplayer, text);

    foreach(new i : Player)
		if(User[i][adminLevel] > 2 && User[i][isLogged] && User[i][acfg_pm])
			va_SendClientMessage(i, -1, ADMIN_SEE_PMS, User[playerid][userName], playerid, User[idplayer][userName], idplayer, text);

    return true;
}

@cmd() votekick(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(User[playerid][isAfk])
        return SendClientMessage(playerid, -1, "%s", AFK_ERROR_MSG);

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}/votekick [id]");

    if(!IsPlayerLogged(targetid))
        return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}Player isn't logged.");

    if(targetid == playerid)
        return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}You can't votekick yourself, just quit.");

    if(User[targetid][adminLevel] > 0)
        return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}You cannot votekick a member of the staff.");

    if(votingInfo[isRunningVote])
        return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}There is already a votekick running.");

    if(gettime() < GetPVarInt(playerid, "votekick_cooldown"))
        return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}You have to wait 2 minutes to start votekick again.");

    new vkStarter[MAX_PLAYER_NAME + 1], vkTarget[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, vkStarter, sizeof(vkStarter));
    GetPlayerName(targetid, vkTarget, sizeof(vkTarget));

    StartVoteKick(targetid);
    SetPVarInt(playerid, "votekick_cooldown", gettime() + (2 * 60));

    va_SendClientMessageToAll(COLOR_RED, "%s started a votekick to kick %s, the votekick will end in 2 minutes.", vkStarter, vkTarget);
    SendClientMessageToAll(COLOR_LIGHT_BLUE, "Type /yes or /no to cast your vote.");

    return 1;
}

@cmd() yes(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return 1;
    return RegisterVote(playerid, true);
}

@cmd() no(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return 1;
    return RegisterVote(playerid, false);
}

@cmd() transfer(playerid, params[], help)
{
    if(!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(User[playerid][isAfk])
        return SendClientMessage(playerid, -1, "%s", AFK_ERROR_MSG);

    new targetid, money;
    if(sscanf(params, "ud", targetid, money))
        return SendClientMessage(playerid, -1, "%s", ERROR_TRANSFERMONEY);

    if(!IsPlayerLogged(targetid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(targetid == playerid)
        return SendClientMessage(playerid, -1, "%s", TRANSFERMONEY_SELF);

    if(money <= 0)
        return SendClientMessage(playerid, -1, "%s", TRANSFERMONEY_AMOUNT);

    if(money > 100000000)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Maximum transfer limit per transaction is $100,000,000.");

    if(money > User[playerid][playerMoney])
        return SendClientMessage(playerid, -1, "%s", TRANSFERMONEY_NOMONEY);

    if(User[targetid][playerMoney] + money > 2000000000)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Target player's bank/wallet would exceed the maximum limit.");

    GivePlayerMoney(playerid, -money);
    GivePlayerMoney(targetid, money);

    User[playerid][playerMoney] -= money;
    User[targetid][playerMoney] += money;

    UpdatePlayerData(playerid);
    UpdatePlayerData(targetid);

    new sentMoneyString[150], givenMoneyString[150];
    format(sentMoneyString, sizeof(sentMoneyString), TRANSFERMONEY_SENT, FormatMoney(money), getPlayerName(targetid));
    SendClientMessage(playerid, -1, "%s", sentMoneyString);

    format(givenMoneyString, sizeof(givenMoneyString), TRANSFERMONEY_GIVEN, FormatMoney(money), getPlayerName(playerid));
    SendClientMessage(targetid, -1, "%s", givenMoneyString);

    new mLog[256];
    format(mLog, sizeof(mLog), "**Sender:** %s (ID: %d)\n**Recipient:** %s (ID: %d)\n**Amount:** $%s",
        getPlayerName(playerid), playerid, getPlayerName(targetid), targetid, FormatMoney(money));
    SendDiscordEmbed(logs_money, "Money Transfer", mLog, DISCORD_COLOR_INFO);

    return true;
}

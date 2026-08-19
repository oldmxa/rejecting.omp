
public DCC_OnMessageCreate(DCC_Message:message)
{
	new DCC_Channel:channel;
	DCC_GetMessageChannel(message, channel);

	new DCC_User:author;
	DCC_GetMessageAuthor(message, author);

	//	new userid[DCC_ID_SIZE];
	//	DCC_GetUserId(author, userid, sizeof(userid));

	if(_:author == 0)
		return 0;

	new bool:isBot;
	DCC_IsUserBot(author, isBot);

	if(isBot)
		return 0;

	new nickname[DCC_NICKNAME_SIZE];
	DCC_GetGuildMemberNickname(guildaid, author, nickname, sizeof(nickname));

	new adminlevel = DSC_GetLevel(author);
	if(adminlevel < 0 || adminlevel >= sizeof(adminnames)) adminlevel = 0;

	new str[500];
	DCC_GetMessageContent(message, str);

	if(IsNull(nickname))
	{
		DCC_GetUserName(author, nickname, sizeof nickname);
	}

	if(channel == gamechat)
	{
		format(stringdiscord,sizeof(stringdiscord), ""COL_DSAND".: CHAT | %s {B2ACAC}%s [DISCORD] "COL_WHITE": %s", adminnames[adminlevel][ingame], nickname, str);
		strreplace(stringdiscord, "%", "");
		utf8decode(msgencoded, stringdiscord, sizeof(msgencoded));
		SendClientMessageToAll(-1, "%s", msgencoded);
	}

	if(channel == adminchat)
	{
		format(stringdiscord,sizeof(stringdiscord), "%s",str);
		strreplace(stringdiscord, "%", "");
		utf8decode(msgencoded, stringdiscord, sizeof(msgencoded));
		foreach(new i : Player)
		{
			if(User[i][adminLevel] > 0 && User[i][isLogged])
				va_SendClientMessage(i, -1, ""COL_DSAND".: ADMIN | %s {B2ACAC}%s [DISCORD]"COL_WHITE": %s", adminnames[adminlevel][ingame], nickname, msgencoded);
		}
	}

	if(!strcmp(str, "/verify", true, 7) || !strcmp(str, ".verify", true, 7) || !strcmp(str, "!verify", true, 7) || !strcmp(str, "/link", true, 5) || !strcmp(str, ".link", true, 5))
	{
		if(logs_linked == DCC_INVALID_CHANNEL) logs_linked = DCC_FindChannelById(CH_LOGS_LINKED);
		if(channel != logs_linked)
		{
			new uid[DCC_ID_SIZE];
			DCC_GetUserId(author, uid, sizeof(uid));
			new denyMsg[256];
			format(denyMsg, sizeof(denyMsg), "<@%s> :no_entry: Public commands can only be used in <#" CH_LOGS_LINKED ">!", uid);
			DCC_SendChannelMessage(channel, denyMsg);
			return true;
		}

		ProcessDiscordVerifyCmd(author, channel);
		return true;
	}

	if(!strcmp(str, "/tournament", true, 11) || !strcmp(str, ".tournament", true, 11) || !strcmp(str, "!tournament", true, 11))
	{
		if(ch_tourney_register == DCC_INVALID_CHANNEL) ch_tourney_register = DCC_FindChannelById(CH_TOURNEY_REGISTER);
		if(logs_linked == DCC_INVALID_CHANNEL) logs_linked = DCC_FindChannelById(CH_LOGS_LINKED);

		if(channel != ch_tourney_register && channel != logs_linked)
		{
			new uid[DCC_ID_SIZE];
			DCC_GetUserId(author, uid, sizeof(uid));
			new denyMsg[256];
			format(denyMsg, sizeof(denyMsg), "<@%s> :no_entry: Tournament registration can only be used in <#" CH_TOURNEY_REGISTER "> or <#" CH_LOGS_LINKED ">!", uid);
			DCC_SendChannelMessage(channel, denyMsg);
			return true;
		}

		ProcessDiscordTourneyApp(author, channel, str);
		return true;
	}

	return true;
}


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
			if(User[i][adminLevel] > 0 && IsPlayerLogged(i))
				va_SendClientMessage(i, -1, ""COL_DSAND".: ADMIN | %s {B2ACAC}%s [DISCORD]"COL_WHITE": %s", adminnames[adminlevel][ingame], nickname, msgencoded);
		}
	}

	if(str[0] == '.' || str[0] == '!' || str[0] == '/')
	{
		ProcessDiscordTextCommand(author, channel, str);
		return true;
	}

	return true;
}

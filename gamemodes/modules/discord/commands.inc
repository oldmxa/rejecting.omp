#include <YSI_Coding/y_hooks>

// Slash commands

#define DSC_CONTENT_SIZE (256)

static DCC_Interaction:g_CurrentInteraction = DCC_Interaction:0;
static bool:g_InteractionAnswered = false;

static g_InteractionParams[DSC_CONTENT_SIZE];

#define DSC_DEBUG 0

#define DSLASH:%0(%1,%2) \
	forward DSC_%0(DCC_Interaction:DSC_it, DCC_User:DSC_usr); \
	public DSC_%0(DCC_Interaction:DSC_it, DCC_User:DSC_usr) \
	{ \
		DSC_Probe(#%0, _:DSC_it, _:DSC_usr); \
		DSC_Begin(DSC_it, DSC_usr); \
		DSCI_%0(DSC_usr, g_InteractionParams); \
		return DSC_End(); \
	} \
	static stock DSCI_%0(DCC_User:%1, const %2)

stock DSC_Probe(const cmd[], a, b)
{
#if DSC_DEBUG
	new idA[DCC_ID_SIZE], idB[DCC_ID_SIZE], ctA[64], ctB[64];
	idA[0] = EOS; idB[0] = EOS; ctA[0] = EOS; ctB[0] = EOS;

	DCC_GetUserId(DCC_User:a, idA, sizeof(idA));
	DCC_GetUserId(DCC_User:b, idB, sizeof(idB));
	DCC_GetInteractionContent(DCC_Interaction:a, ctA, sizeof(ctA));
	DCC_GetInteractionContent(DCC_Interaction:b, ctB, sizeof(ctB));

	printf("[DSC] /%s  a=%d b=%d", cmd, a, b);
	printf("[DSC]   userId(a)='%s'  userId(b)='%s'", idA, idB);
	printf("[DSC]   content(a)='%s'  content(b)='%s'", ctA, ctB);
#else
	#pragma unused cmd, a, b
#endif
	return 1;
}

static DCC_User:g_CurrentInvoker = DCC_INVALID_USER;
static DCC_Channel:g_CurrentInteractionChannel = DCC_INVALID_CHANNEL;

stock DSC_Begin(DCC_Interaction:interaction, DCC_User:user = DCC_INVALID_USER)
{
	g_InteractionAnswered = false;
	g_CurrentInteraction = interaction;
	g_CurrentInvoker = user;
	g_CurrentInteractionChannel = DCC_INVALID_CHANNEL;

	if(interaction != DCC_Interaction:0)
	{
		DCC_GetInteractionChannel(interaction, g_CurrentInteractionChannel);
	}

	g_InteractionParams[0] = EOS;
	DCC_GetInteractionContent(g_CurrentInteraction, g_InteractionParams, sizeof(g_InteractionParams));

	if(isnull(g_InteractionParams))
	{
		g_InteractionParams[0] = '\1';
		g_InteractionParams[1] = EOS;
	}
	return 1;
}

stock DSC_End()
{
	if(!g_InteractionAnswered && g_CurrentInteraction != DCC_Interaction:0)
	{
		if(g_CurrentInvoker != DCC_INVALID_USER)
		{
			new uid[DCC_ID_SIZE];
			uid[0] = EOS;
			DCC_GetUserId(g_CurrentInvoker, uid, sizeof(uid));
			if(!isnull(uid))
			{
				new msg[128];
				format(msg, sizeof(msg), "<@%s> :hourglass_flowing_sand: Fetching data - the result appears below.", uid);
				DCC_SendInteractionMessage(g_CurrentInteraction, msg);
			}
			else
			{
				DCC_SendInteractionMessage(g_CurrentInteraction, ":hourglass_flowing_sand: Fetching data - the result appears below.");
			}
		}
		else
		{
			DCC_SendInteractionMessage(g_CurrentInteraction, ":hourglass_flowing_sand: Fetching data - the result appears below.");
		}
	}

	g_CurrentInteraction = DCC_Interaction:0;
	g_CurrentInvoker = DCC_INVALID_USER;
	g_CurrentInteractionChannel = DCC_INVALID_CHANNEL;
	g_InteractionAnswered = false;
	return 1;
}

static stock DCC_Interaction:DSC_TakeInteraction()
{
	if(g_CurrentInteraction == DCC_Interaction:0 || g_InteractionAnswered) return DCC_Interaction:0;

	g_InteractionAnswered = true;
	return g_CurrentInteraction;
}

stock bool:DSC_IsOwner(DCC_User:user)
{
    if(user == DCC_INVALID_USER) return false;

    new userid[DCC_ID_SIZE];
    userid[0] = EOS;
    DCC_GetUserId(user, userid, sizeof(userid));

    if(isnull(userid)) return false;

    for (new i; i < sizeof(OWNER_COMMANDS_DISCORD); i ++)
    {
        if (strequal(userid, OWNER_COMMANDS_DISCORD[i])) return true;
    }
    return false;
}

stock bool:DSC_HasRole(DCC_User:user, const roleid[])
{
    if(user == DCC_INVALID_USER) return false;

    new DCC_Role:role = DCC_FindRoleById(roleid);
    if(role == DCC_INVALID_ROLE) return false;

    new bool:has = false;
    DCC_HasGuildMemberRole(guildaid, user, role, has);
    return has;
}

stock DSC_GetLevel(DCC_User:user)
{
    if(DSC_IsOwner(user)) return MAX_ADMIN_LEVEL;

    for(new i = MAX_ADMIN_LEVEL - 1; i >= 0; i--)
    {
        if(DSC_HasRole(user, g_DiscordAdminRoles[i])) return i + 1;
    }
    return 0;
}

stock bool:DSC_IsPlayer(DCC_User:user)
{
    return (DSC_GetLevel(user) > 0 || DSC_HasRole(user, DISCORD_ROLE_LINKED));
}

static stock DSC_Deny(const message[])
{
    if(g_CurrentInteraction != DCC_Interaction:0 && !g_InteractionAnswered)
    {
        g_InteractionAnswered = true;
        DCC_SendInteractionMessage(g_CurrentInteraction, message);
    }
    return 0;
}

stock DSC_RequireLevel(DCC_User:user, level)
{
    if(g_CurrentInteractionChannel != DCC_INVALID_CHANNEL)
    {
        if(ch_comandos_admin == DCC_INVALID_CHANNEL) ch_comandos_admin = DCC_FindChannelById(CH_COMANDOS_ADMIN);
        if(g_CurrentInteractionChannel != ch_comandos_admin)
        {
            return DSC_Deny(":no_entry: Administrative commands can only be used in <#" CH_COMANDOS_ADMIN ">!");
        }
    }

    if(DSC_GetLevel(user) >= level) return 1;

    new msg[160];
    format(msg, sizeof(msg), ":no_entry: This command requires the **%s** role or above.", adminnames[level][discord]);
    return DSC_Deny(msg);
}

stock DSC_RequirePlayer(DCC_User:user)
{
    if(g_CurrentInteractionChannel != DCC_INVALID_CHANNEL)
    {
        if(ch_comandos_players == DCC_INVALID_CHANNEL) ch_comandos_players = DCC_FindChannelById(CH_COMANDOS_PLAYERS);
        if(g_CurrentInteractionChannel != ch_comandos_players)
        {
            return DSC_Deny(":no_entry: Public commands can only be used in <#" CH_COMANDOS_PLAYERS ">!");
        }
    }

    if(DSC_IsPlayer(user)) return 1;

    return DSC_Deny(":no_entry: Link your game account first (use /linkdiscord in-game) to unlock the bot commands.");
}

forward OnDiscordPrivateMsg(const message[]);
public OnDiscordPrivateMsg(const message[])
{
    new DCC_Channel:dm = DCC_GetCreatedPrivateChannel();
    if(dm != DCC_INVALID_CHANNEL)
    {
        DCC_SendChannelMessage(dm, message);
    }
    return 1;
}

forward OnDiscordPrivateEmbed(DCC_Embed:embed_data);
public OnDiscordPrivateEmbed(DCC_Embed:embed_data)
{
    new DCC_Channel:dm = DCC_GetCreatedPrivateChannel();
    if(dm != DCC_INVALID_CHANNEL)
    {
        DCC_SendChannelEmbedMessage(dm, embed_data);
    }
    return 1;
}

forward Discord_SendDiscordResponse(DCC_Channel:channel, const message[]);
public Discord_SendDiscordResponse(DCC_Channel:channel, const message[])
{
	new DCC_Interaction:interaction = DSC_TakeInteraction();
	if(interaction != DCC_Interaction:0) return DCC_SendInteractionMessage(interaction, message);

	if(g_CurrentInvoker != DCC_INVALID_USER)
	{
		DCC_CreatePrivateChannel(g_CurrentInvoker, "OnDiscordPrivateMsg", "s", message);
		return 1;
	}

	return DCC_SendChannelMessage(channel, message);
}

stock SendDiscordEmbedResponse(DCC_Channel:channel, DCC_Embed:embed_data)
{
	new DCC_Interaction:interaction = DSC_TakeInteraction();
	if(interaction != DCC_Interaction:0) return DCC_SendInteractionEmbed(interaction, embed_data);

	if(g_CurrentInvoker != DCC_INVALID_USER)
	{
		DCC_CreatePrivateChannel(g_CurrentInvoker, "OnDiscordPrivateEmbed", "i", _:embed_data);
		return 1;
	}

	return DCC_SendChannelEmbedMessage(channel, embed_data);
}

forward Discord_SendDiscordEmbedReply(const title[], const description[], color);
public Discord_SendDiscordEmbedReply(const title[], const description[], color)
{
	return SendDiscordEmbedResponse(comandos, BuildDiscordEmbed(title, description, color));
}


enum E_SLASH_CMD
{
	slash_name[DCC_COMMAND_SIZE],
	slash_desc[DCC_COMMAND_DESCRIPTION_SIZE],
	slash_level
}

static const g_SlashCommands[][E_SLASH_CMD] = {
	{"setadmin",        "Set a player admin level",      4},
	{"ban",             "Ban a player",                  2},
	{"unban",           "Unban a player or IP",          2},
	{"kick",            "Kick an online player",         1},
	{"mute",            "Mute an online player",         1},
	{"unmute",          "Unmute a player",               1},
	{"a",               "Global admin message",          1},
	{"lc",              "Clear the in-game chat",        1},
	{"warn",            "Warn a player",                 4},
	{"banserial",       "Ban a player serial (GPCI)",    4},
	{"changebanreason", "Change a ban reason",           4},
	{"changebantime",   "Change a ban expiry time",      4},
	{"hostname",        "Change the server name",        4},
	{"penalties",       "Show penalty history",          4},
	{"setmoney",        "Set a player money",            2},
	{"resetpassword",   "Reset a player account password", 5},
	{"players",         "List online players",           1},
	{"baninfo",         "Show ban details",              1},
	{"playerinfo",      "Show account details",          2},
	{"cmds",            "List the bot commands",         1},
	{"unbanisp",        "Unban an ISP",                  4},
	{"banisp",          "Ban an ISP",                    4},
	{"aka",             "Show linked accounts",          1},
	{"gmx",             "Restart the gamemode",          4},
	{"gtemp",           "Demand a player recording",     1},
	{"temprec",         "Demand a player recording",     1},
	{"banomp",          "Force open.mp launcher",        1},
	{"unbanomp",        "Unban open.mp launcher",        2},
	{"unbanip",         "Unban an IP address",           2},
	{"dox",             "Show IP and ISP data",          2},
	{"turnoff",         "Shut the server down",          5},
	{"reloadbans",      "Reload the ban list",           4},
	{"rconunbanip",     "Unban an IP via RCON",          4},
	{"stafflist",       "List the staff members",        1},
	{"d",               "Answer a player question",      1},
	{"r",               "Reply to a player report",      1},
	{"reply",           "Reply to a player report",      1},
	{"weekresults",     "Broadcast weekly results",      4},

	// publicos
	{"help",            "List player commands and help", 0},
	{"commands",        "List the player commands",      0},
	{"verify",          "Link and verify your account",  0},
	{"link",            "Link and verify your account",  0},
	{"howtolink",       "Guide on how to link account",  0},
	{"rules",           "Server rules",                  0},
	{"stats",           "Show account statistics",       0},
	{"clan",            "Show clan info and stats",      0},
	{"topclans",        "Top 10 clans by points",        0},
	{"online",          "Who is playing right now",      0},
	{"vip",             "VIP packages and perks",        0},
	{"top",             "Top 10 players by kills",       0},
	{"tournament",      "Tournament application",        0}
};

forward RegisterSlashCommands();
public RegisterSlashCommands()
{
	new DCC_Guild:g = DCC_FindGuildById(DISCORD_GUILD_ID);
	if(g != DCC_INVALID_GUILD)
		guildaid = g;

	// printf("[DSC] guild at registration: %d (was %d at init)", _:g, _:guildaid);

	new cmdName[DCC_COMMAND_SIZE], cmdDesc[DCC_COMMAND_DESCRIPTION_SIZE];
	new callbackName[DCC_COMMAND_SIZE + 8];

	for(new i = 0; i < sizeof(g_SlashCommands); i++)
	{
		format(cmdName, sizeof(cmdName), "%s", g_SlashCommands[i][slash_name]);
		format(cmdDesc, sizeof(cmdDesc), "%s", g_SlashCommands[i][slash_desc]);
		format(callbackName, sizeof(callbackName), "DSC_%s", cmdName);

		new DCC_Command:c = DCC_CreateCommand(cmdName, cmdDesc, callbackName, true, guildaid);

		#if DSC_DEBUG
			if(i < 3) printf("[DSC] registered /%s -> %s (guild=%d, cmd=%d)", cmdName, callbackName, _:guildaid, _:c);
		#else
			#pragma unused c
		#endif
	}

	// printf("[DSC] %d slash commands registered in guild %d.", sizeof(g_SlashCommands), _:guildaid);

	UpdateDiscordBotActivity();
	return 1;
}

DSLASH:setadmin(user, params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userid[DCC_ID_SIZE];
	DCC_GetUserId(user, userid, sizeof(userid));

	if(user != DCC_INVALID_USER)
	{
		if(!DSC_RequireLevel(user, 4)) return 1;

		new nick[MAX_PLAYER_NAME + 1], level;
		if (sscanf(params, "s[25]d", nick, level))
			return SendDiscordResponse(comandos, sprintf(" **%s**, /setadmin [username] [level].", username));

		if (level < 0 || level > 4)
			return SendDiscordResponse(comandos, sprintf(" **%s**, level between 0 and 4.", username));

		if (strequal(nick, SERVER_OWNER, true))
			return SendDiscordResponse(comandos, sprintf(" **%s**, you cannot do that with the owners.", username));

		new role[14];
		format(role, sizeof(role), "%s", adminnames[level][discord]);

		mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,	"SELECT `id`, `adminlevel` FROM `accounts` WHERE `username` = '%e' LIMIT 1", nick);
		mysql_tquery(dbHandle, queryGlobal, "SetAdminDiscord", "sssdd", username, nick, role, level, DSC_GetLevel(user));

        new log_details[256];
        format(log_details, sizeof(log_details), "Discord Admin %s set player %s to Admin Level %d (%s)", username, nick, level, role);
        LogServerAction(0, username, "promotions", "SetAdmin (Discord)", log_details);

		return true;
	}

	return true;
}

DSLASH:ban(user, params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof(username));
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));

    if(!DSC_RequireLevel(user, 2)) return 1;

	new reason[120], hours, nick[MAX_PLAYER_NAME + 1];

	if(sscanf(params, "s[25]ds[120]", nick, hours, reason))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /ban [nick] [hours] [reason].", username));

	new reasonEncoded[120];
	utf8encode(reasonEncoded, reason, sizeof(reasonEncoded));

	if(hours < 1 || hours > 9999)
		return SendDiscordResponse(comandos, sprintf(" **%s**, please provide a hour between 1 and 9999.", username));

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", nick);
	mysql_tquery(dbHandle, queryGlobal, "BanPlayerDiscord", "sssdd", username, nick, reasonEncoded, gettime()+hours*3600, _:user);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s banned player %s for %d hours. Reason: %s", username, nick, hours, reason);
    LogServerAction(0, username, "bans", "Ban (Discord)", log_details);

	return true;
}

DSLASH:unban(user, params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof(username));
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));

    if(!DSC_RequireLevel(user, 2)) return 1;

	new nameIP[28];
	if(sscanf(params, "s[28]", nameIP))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /unban [nick/ip].", username));

	mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,	"SELECT `username` FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e' LIMIT 1", nameIP, nameIP);
	mysql_tquery(dbHandle, queryGlobal, "UnBanPlayerDiscord", "s", username);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s unbanned player/IP %s", username, nameIP);
    LogServerAction(0, username, "unbans", "Unban (Discord)", log_details);

	return true;
}

DSLASH:kick(user, params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof(username));
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));

    if(!DSC_RequireLevel(user, 1)) return 1;

	new targetid, reason[64];

	if(sscanf(params, "ds[64]", targetid, reason))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /kick [id] [reason]", username));

	if(!IsPlayerConnected(targetid))
		return SendDiscordResponse(comandos, sprintf(" **%s**, the player is not online!", username));

	if(strequal(User[targetid][userName], SERVER_OWNER, true))
		return SendDiscordResponse(comandos, sprintf(" **%s**, you cannot do that with the owners.", username));

	SendDiscordResponse(comandos, sprintf(":white_check_mark: The player **%s** has been kicked!", User[targetid][userName]));
	va_SendClientMessageToAll(COLOR_ADM_PUNISH, KICK_REASON, username, User[targetid][userName], targetid, reason);

	KickEx(targetid, 300);

	new string[1024];
	format(string, sizeof(string), "**Admin:** %s\n**Target:** %s\n**Reason:** %s",
		username,
		User[targetid][userName],
		reason);

	SendDiscordEmbed(logs_kick, "New Kick", string, DISCORD_COLOR_WARNING);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s kicked player %s (ID: %d). Reason: %s", username, User[targetid][userName], targetid, reason);
    LogServerAction(0, username, "kicks", "Kick (Discord)", log_details);

	return true;
}

DSLASH:a(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 1)) return 1;

    if(IsNull(params)) return 1;
    new username[33];
    DCC_GetUserName(user, username, sizeof(username));
    va_SendClientMessageToAll(COLOR_ADM_CMD, "{MAIN}[DISCORD] {FFFFFF}%s: %s", username, params);
    SendDiscordResponse(comandos, ":white_check_mark: Global message sent!");
    return true;
}

DSLASH:lc(user, params[])
{
	#pragma unused params
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 1)) return 1;

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));
    for(new j = 0; j < 100; j++) SendClientMessageToAll(-1, " ");
    va_SendClientMessageToAll(COLOR_ADM_CMD, "{MAIN}[ADMIN] {FFFFFF}[Discord] Admin %s cleared the chat.", username);
    SendDiscordResponse(comandos, ":white_check_mark: Chat successfully cleared!");
    return true;
}

DSLASH:warn(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 4)) return 1;

    new target_nick[24], reason[64];
    if(sscanf(params, "s[24]s[64]", target_nick, reason)) 
        return SendDiscordResponse(comandos, "/warn [nick] [reason]");

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));

    new targetid = GetOnlinePlayerIDByName(target_nick);

    if(targetid != INVALID_PLAYER_ID)
    {
        User[targetid][warnings]++;
        va_SendClientMessageToAll(COLOR_ADM_PUNISH, "{MAIN}[ADMIN] [Discord] Admin %s gave a warning to %s [%d/%d]. Reason: %s", username, User[targetid][userName], User[targetid][warnings], ServerMaxWarnings, reason);
        if(User[targetid][warnings] >= ServerMaxWarnings)
        {
            new hours = 9999;
            new banReason[64];
            format(banReason, sizeof(banReason), "Exceeded warning limit (%d/%d)", ServerMaxWarnings, ServerMaxWarnings);
            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);
            mysql_tquery(dbHandle, queryGlobal, "BanPlayerDiscord", "sssdd", username, User[targetid][userName], banReason, gettime()+hours*3600, _:user);
        }
        else UpdatePlayerData(targetid);
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `warnings` = `warnings` + 1 WHERE `username` = '%e'", target_nick);
        mysql_tquery(dbHandle, queryGlobal);
    }

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s warned player %s. Reason: %s", username, target_nick, reason);
    LogServerAction(0, username, "warns", "Warn (Discord)", log_details);

    new dMsg[256];
    format(dMsg, sizeof(dMsg), "**Admin:** %s (Discord)\n**Player:** %s\n**Warning:** %d/%d\n**Reason:** %s",
        username, target_nick, (targetid != INVALID_PLAYER_ID) ? User[targetid][warnings] : 1, ServerMaxWarnings, reason);
    SendDiscordEmbed(logs_punish, "New Warning", dMsg, DISCORD_COLOR_WARNING);

    SendDiscordResponse(comandos, sprintf("Warning applied to player **%s**!", target_nick));
    return true;
}

DSLASH:mute(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 1)) return 1;

    new target_nick[24], minutes, reason[64];
    if(sscanf(params, "s[24]ds[64]", target_nick, minutes, reason))
        return SendDiscordResponse(comandos, "/mute [nick/id] [minutes] [reason]");

    if(minutes < 1 || minutes > 1440)
        return SendDiscordResponse(comandos, ":x: Invalid duration! (1 - 1440 minutes)");

    new targetid = GetOnlinePlayerIDByName(target_nick);
    if(targetid == INVALID_PLAYER_ID)
    {
        if(IsNumeric(target_nick))
        {
            new id = strval(target_nick);
            if(IsPlayerLogged(id)) targetid = id;
        }
    }

    if(targetid == INVALID_PLAYER_ID)
        return SendDiscordResponse(comandos, ":x: Player is not online!");

    if(IsPlayerMuted(targetid))
        return SendDiscordResponse(comandos, ":x: Player is already muted!");

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));

    foreach(new i : Player)
    {
        if(i != targetid)
            va_SendClientMessage(i, -1, "{MAIN}[ADMIN] {FFFFFF}[Discord] Admin %s muted %s[%d] for %d minutes. Reason: %s", username, User[targetid][userName], targetid, minutes, reason);
    }
    va_SendClientMessage(targetid, -1, "{MAIN}[MUTE] {FFFFFF}You have been muted by [Discord] Admin %s for %d minutes. Reason: %s", username, minutes, reason);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s muted player %s (ID %d) for %d minutes. Reason: %s", username, User[targetid][userName], targetid, minutes, reason);
    LogServerAction(0, username, "mutes", "Mute (Discord)", log_details);

    new dMsg[256];
    format(dMsg, sizeof(dMsg), "**Admin:** %s (Discord)\n**Player:** %s\n**Duration:** %d minutes\n**Reason:** %s",
        username, User[targetid][userName], minutes, reason);
    SendDiscordEmbed(logs_punish, "New Mute", dMsg, DISCORD_COLOR_WARNING);

    User[targetid][muteTimestamp] = gettime() + minutes * 60;
    UpdatePlayerData(targetid);

    return SendDiscordResponse(comandos, sprintf(":white_check_mark: Player **%s** (ID %d) has been muted for **%d minutes**!", User[targetid][userName], targetid, minutes));
}

DSLASH:unmute(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 1)) return 1;

    new target_nick[24];
    if(sscanf(params, "s[24]", target_nick))
        return SendDiscordResponse(comandos, "/unmute [nick/id]");

    new targetid = GetOnlinePlayerIDByName(target_nick);
    if(targetid == INVALID_PLAYER_ID)
    {
        if(IsNumeric(target_nick))
        {
            new id = strval(target_nick);
            if(IsPlayerLogged(id)) targetid = id;
        }
    }

    if(targetid == INVALID_PLAYER_ID)
        return SendDiscordResponse(comandos, ":x: Player is not online!");

    if(!IsPlayerMuted(targetid))
        return SendDiscordResponse(comandos, ":x: Player is not muted!");

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));

    foreach(new i : Player)
    {
        if(IsAdmin(i))
            va_SendClientMessage(i, -1, "{MAIN}[ADMIN] {FFFFFF}[Discord] Admin %s unmuted player %s[%d].", username, User[targetid][userName], targetid);
    }
    va_SendClientMessage(targetid, -1, "{MAIN}[MUTE] {FFFFFF}You have been unmuted by [Discord] Admin %s.", username);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s unmuted player %s (ID %d)", username, User[targetid][userName], targetid);
    LogServerAction(0, username, "mutes", "Unmute (Discord)", log_details);

    new dMsg[256];
    format(dMsg, sizeof(dMsg), "**Admin:** %s (Discord)\n**Player:** %s",
        username, User[targetid][userName]);
    SendDiscordEmbed(logs_punish, "New Unmute", dMsg, DISCORD_COLOR_SUCCESS);

    User[targetid][muteTimestamp] = 0;
    UpdatePlayerData(targetid);

    return SendDiscordResponse(comandos, sprintf(":white_check_mark: Player **%s** (ID %d) has been unmuted!", User[targetid][userName], targetid));
}

DSLASH:banserial(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 4)) return 1;

    new target[32], reason[64];
    if(sscanf(params, "s[32]s[64]", target, reason))
        return SendDiscordResponse(comandos, "/banserial [nick] [reason]");

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));
    
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' LIMIT 1", target);
    mysql_tquery(dbHandle, queryGlobal, "BanPlayerDiscord", "sssdd", username, target, reason, gettime()+9999*3600, _:user);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s serial-banned player %s. Reason: %s", username, target, reason);
    LogServerAction(0, username, "bans", "BanSerial (Discord)", log_details);
    
    SendDiscordResponse(comandos, sprintf(":white_check_mark: Serial ban applied to player **%s**!", target));
    return true;
}

DSLASH:changebanreason(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 4)) return 1;

    new target[24], reason[120];
    if(sscanf(params, "s[24]s[120]", target, reason))
        return SendDiscordResponse(comandos, "/changebanreason [nick] [reason]");

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `banimentos` SET `reason` = '%e' WHERE `username` = '%e'", reason, target);
    mysql_tquery(dbHandle, queryGlobal);
    SendDiscordResponse(comandos, sprintf(COL_ADM_CMD":white_check_mark: Reason for banning **%s** changed!", target));
    return true;
}

DSLASH:changebantime(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 4)) return 1;

    new target[24], amount, unit[2];
    if(sscanf(params, "s[24]is[2]", target, amount, unit))
        return SendDiscordResponse(comandos, "/changebantime [nick] [amount] [h/d]");

    if(unit[0] == 'h' || unit[0] == 'H') { /* hours logic */ }
    else if(unit[0] == 'd' || unit[0] == 'D') { /* days logic */ }
    else return SendDiscordResponse(comandos, "Invalid unit. Use 'h' or 'd'.");

    SendDiscordResponse(comandos, sprintf(COL_ADM_CMD":white_check_mark: Banishment expiry date for **%s** changed! New unban in **%d** **%s**.", target, amount, (unit[0] == 'h' ? "hours" : "days")));
    return true;
}

DSLASH:hostname(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 4)) return 1;

    if(IsNull(params)) return SendDiscordResponse(comandos, "/hostname [name]");
    new str[144];
    format(str, sizeof(str), "hostname %s", params);
    SendRconCommand(str);
    va_SendClientMessageToAll(COLOR_ADM_CMD, "{MAIN}[ADMIN] [Discord] Server renamed to: %s", params);
    SendDiscordResponse(comandos, ":white_check_mark: Hostname successfully updated!");
    return true;
}

DSLASH:penalties(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 4)) return 1;

    new target[24];
    if(sscanf(params, "s[24]", target)) return SendDiscordResponse(comandos, "/penalties [nick]");

    inline const OnDiscordPenalties()
    {
        new rows = cache_num_rows();
        if(rows)
        {
            new p_str[2000] = "**Penalty history for ";
            strcat(p_str, target);
            strcat(p_str, ":**\n");
            for(new j, action[32], details[128], timestamp[32]; j < rows; j++)
            {
                cache_get_value_name(j, "action", action);
                cache_get_value_name(j, "details", details);
                cache_get_value_name(j, "created_at", timestamp);
                format(p_str, sizeof(p_str), "%s`[%s]` **%s**: %s\n", p_str, timestamp, action, details);
            }
            SendDiscordResponse(comandos, p_str);
        }
        else SendDiscordResponse(comandos, "No penalties found for this user.");
    }

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `action`, `details`, `created_at` FROM `server_logs` WHERE `username` = '%e' ORDER BY `created_at` DESC LIMIT 15", target);
    MySQL_TQueryInline(dbHandle, using inline OnDiscordPenalties, queryGlobal);
    return true;
}

DSLASH:setmoney(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 2)) return 1;

    new target_nick[24], amount;
    if(sscanf(params, "s[24]d", target_nick, amount))
        return SendDiscordResponse(comandos, "Usage: /setmoney [nick] [amount]");

    if(amount < 0 || amount > 1000000000)
        return SendDiscordResponse(comandos, "Error: Amount must be between 0 and 1,000,000,000.");

    new targetid = GetOnlinePlayerIDByName(target_nick);

    if(targetid != INVALID_PLAYER_ID)
    {
        ResetPlayerMoney(targetid);
        GivePlayerMoney(targetid, amount);
        User[targetid][playerMoney] = amount;
        va_SendClientMessage(targetid, -1, "{MAIN}[ADMIN] {FFFFFF}[Discord] Your money was set to %s.", FormatMoney(amount));
        
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `player_money` = %d WHERE `username` = '%e'", amount, target_nick);
        mysql_tquery(dbHandle, queryGlobal);
        
        new username[DCC_USERNAME_SIZE];
        DCC_GetUserName(user, username, sizeof(username));

        new mLog[256];
        format(mLog, sizeof(mLog), "**Admin:** %s (Discord)\n**Player:** %s\n**New Balance:** $%s",
            username, target_nick, FormatMoney(amount));
        SendDiscordEmbed(logs_money, "Admin Money Set", mLog, DISCORD_COLOR_ADMIN);

        SendDiscordResponse(comandos, sprintf("Money of **%s** set to **%s**!", target_nick, FormatMoney(amount)));
        return true;
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `player_money` = %d WHERE `username` = '%e'", amount, target_nick);
        mysql_tquery(dbHandle, queryGlobal);

        new username[DCC_USERNAME_SIZE];
        DCC_GetUserName(user, username, sizeof(username));

        new mLog[256];
        format(mLog, sizeof(mLog), "**Admin:** %s (Discord)\n**Player:** %s (Offline)\n**New Balance:** $%s",
            username, target_nick, FormatMoney(amount));
        SendDiscordEmbed(logs_money, "Admin Money Set", mLog, DISCORD_COLOR_ADMIN);

        SendDiscordResponse(comandos, sprintf("Money of offline player **%s** set to **%s**!", target_nick, FormatMoney(amount)));
        return true;
    }
}

#define RESETPASS_LENGTH (10)

static stock GenerateResetPassword(dest[], size = sizeof(dest))
{
    static const charset[] = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    new len = size - 1;

    if(len > RESETPASS_LENGTH) len = RESETPASS_LENGTH;

    for(new i = 0; i < len; i++)
        dest[i] = charset[random(sizeof(charset) - 1)];

    dest[len] = EOS;
    return 1;
}

static stock GetLinkedPlayerId(DCC_User:user)
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(isnull(userid)) return INVALID_PLAYER_ID;

    foreach(new i : Player)
    {
        if(!User[i][isLogged]) continue;
        if(strequal(User[i][dc_uid], "-1", true)) continue;
        if(strequal(User[i][dc_uid], userid, true)) return i;
    }
    return INVALID_PLAYER_ID;
}

forward OnResetPasswordLookup(adminid, const adminName[], const reason[], sqlid);
public OnResetPasswordLookup(adminid, const adminName[], const reason[], sqlid)
{
    if(!cache_num_rows())
        return SendDiscordResponse(comandos, sprintf(":x: **%s**, no account found with SQL ID **%d**.", adminName, sqlid));

    new targetName[MAX_PLAYER_NAME + 1];
    cache_get_value_name(0, "username", targetName, sizeof(targetName));

    new newPass[RESETPASS_LENGTH + 1];
    GenerateResetPassword(newPass, sizeof(newPass));

    bcrypt_hash(adminid, "OnResetPasswordHash", newPass, BCRYPT_COST, "dssss", sqlid, targetName, adminName, reason, newPass);
    return 1;
}

forward OnResetPasswordHash(adminid, sqlid, const targetName[], const adminName[], const reason[], const newPass[]);
public OnResetPasswordHash(adminid, sqlid, const targetName[], const adminName[], const reason[], const newPass[])
{
    new hash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hash);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "UPDATE `accounts` SET `password` = '%e' WHERE `id` = %d", hash, sqlid);
    mysql_tquery(dbHandle, queryGlobal);

    foreach(new i : Player)
    {
        if(User[i][user_sqlid] == sqlid && User[i][isLogged])
        {
            SendClientMessage(i, -1, "{MAIN}[SECURITY] {FFFFFF}Your account password was reset by staff. Please log in again.");
            KickEx(i, 1500);
            break;
        }
    }

    new adminSqlId = 0, adminNick[MAX_PLAYER_NAME + 1] = "Unknown";
    if(adminid != INVALID_PLAYER_ID && IsPlayerConnected(adminid))
    {
        adminSqlId = User[adminid][user_sqlid];
        strcopy(adminNick, User[adminid][userName], sizeof(adminNick));

        va_SendClientMessage(adminid, -1, "{MAIN}[RESET] {FFFFFF}New password for {MAIN}%s {FFFFFF}(SQL #%d): {23C552}%s", targetName, sqlid, newPass);
        va_SendClientMessage(adminid, -1, "{MAIN}[RESET] {FFFFFF}Deliver it to the account owner and tell them to change it with {MAIN}/changepass{FFFFFF}.");
    }

    new details[256];
    format(details, sizeof(details), "Discord Admin %s reset password of %s (SQL #%d). Reason: %s", adminName, targetName, sqlid, reason);
    LogServerAction(adminSqlId, adminName, "security", "ResetPassword (Discord)", details);

    new embed[1024];
    format(embed, sizeof(embed),
        "**Target Account**\n"\
        "- **Nick:** `%s`\n"\
        "- **SQL ID:** `%d`\n\n"\
        "**Performed By**\n"\
        "- **Discord:** `%s`\n"\
        "- **In-Game Nick:** `%s`\n"\
        "- **In-Game SQL ID:** `%d`\n\n"\
        "**Reason**\n%s\n\n"\
        "*The new password was delivered in-game to the acting admin only.*",
        targetName, sqlid, adminName, adminNick, adminSqlId, reason);

    utf8encode(msgencoded, embed, sizeof(msgencoded));
    SendDiscordEmbed(logs_password, ":key: Account Password Reset", msgencoded, DISCORD_COLOR_WARNING);

    SendDiscordResponse(comandos, sprintf(":white_check_mark: Password of **%s** (SQL #%d) was reset. The new password was sent to you in-game.", targetName, sqlid));
    return 1;
}

DSLASH:resetpassword(user, params[])
{
    new adminName[33];
    DCC_GetUserName(user, adminName, sizeof(adminName));

    if(!DSC_RequireLevel(user, 5)) return 1;

    new adminid = GetLinkedPlayerId(user);
    if(adminid == INVALID_PLAYER_ID)
        return SendDiscordResponse(comandos, sprintf(":x: **%s**, you must be linked and **online in-game** to use this command (the new password is delivered in-game).", adminName));

    if (!IsAdmin(adminid, 5))
        return SendDiscordResponse(comandos, sprintf(":no_entry: **%s**, your in-game account is not admin level 5.", adminName));

    new sqlid, reason[128];
    if(sscanf(params, "ds[128]", sqlid, reason))
        return SendDiscordResponse(comandos, sprintf(":x: **%s**, usage: `/resetpassword <account_sql_id> <reason>`", adminName));

    if(sqlid < 1)
        return SendDiscordResponse(comandos, sprintf(":x: **%s**, invalid account SQL ID.", adminName));

    if(strlen(reason) < 5)
        return SendDiscordResponse(comandos, sprintf(":x: **%s**, please write a proper reason (minimum 5 characters).", adminName));

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "SELECT `username` FROM `accounts` WHERE `id` = %d LIMIT 1", sqlid);
    mysql_tquery(dbHandle, queryGlobal, "OnResetPasswordLookup", "dssd", adminid, adminName, reason, sqlid);
    return 1;
}

DSLASH:players(user, params[])
{
	#pragma unused params, user
	if(!DSC_RequireLevel(user, 1)) return 1;

	new pstr[2000];
	new pstr2[2000];
	new pcount;
	new sampVERSION[24];
	new locationName[15];
	new tempUsername[MAX_PLAYER_NAME + 1];
	new locationid;

	pstr[0] = EOS;
	strcat(pstr, "**Online players:** \n");

	foreach(new i : Player)
	{
		if(User[i][isLogged])
		{
			pcount++;
			GetPlayerVersion(i, sampVERSION, sizeof(sampVERSION));

			format(tempUsername, sizeof(tempUsername), "%s", User[i][userName]);
			DCC_EscapeMarkdown(tempUsername, tempUsername,  sizeof(tempUsername));

			if ((locationid = User[i][locationId]) != -1)
			{
				strcopy(locationName, ROOM_NAME[locationid][E_ROOM_NAME_DISCORD]);
			}
			else
			{
				strcopy(locationName, "N/A");
			}

			if(i < 12)
			{
				format(pstr, sizeof(pstr), "%s`[%d]` **_%s_** - `(Location: %s, K/D: %d/%d, Accuracy: %d%%, SAMP: %s, FPS: %d, Ping: %d, PL: %.2f, Mobile: %s, Admin: %s)`\n",
							pstr, i,
							tempUsername,
							locationName,
							locationid == -1 ? 0 : playerArenaInfo[locationid][i][arenaKills],
							locationid == -1 ? 0 : playerArenaInfo[locationid][i][arenaDeaths],
							locationid == -1 ? 0 : floatround(GetPlayerAccuracy(i, locationid), floatround_round),
							sampVERSION,
							User[i][FPS],
							GetPlayerPing(i),
							NetStats_PacketLossPercent(i),
							User[i][isMobile] ? "Yes" : "No",
							User[i][adminLevel] > 0 ? "Yes" : "No");
			}
			else
			{
				format(pstr2, sizeof(pstr2), "%s`[%d]` **_%s_** - `(Location: %s, K/D: %d/%d, Accuracy: %d%%, SAMP: %s, FPS: %d, Ping: %d, PL: %.2f, Mobile: %s, Admin: %s)`\n",
							pstr2, i,
							tempUsername,
							locationName,
							locationid == -1 ? 0 : playerArenaInfo[locationid][i][arenaKills],
							locationid == -1 ? 0 : playerArenaInfo[locationid][i][arenaDeaths],
							locationid == -1 ? 0 : floatround(GetPlayerAccuracy(i, locationid), floatround_round),
							sampVERSION,
							User[i][FPS],
							GetPlayerPing(i),
							NetStats_PacketLossPercent(i),
							User[i][isMobile] ? "Yes" : "No",
							User[i][adminLevel] > 0 ? "Yes" : "No");
			}
		}
	}

	if(pcount < 1)
		format(pstr, sizeof(pstr), "*The server is empty!*");

	SendDiscordResponse(comandos, pstr);

	if(strlen(pstr2) > 0)
		SendDiscordResponse(comandos, pstr2);

	return true;
}

DSLASH:baninfo(user, params[])
{
	if(!DSC_RequireLevel(user, 1)) return 1;

	new username[33];
	DCC_GetUserName(user, username, sizeof(username));
	new nameIP[28];

	if(sscanf(params, "s[28]", nameIP))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /baninfo [nick/ip].", username));

	mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "SELECT * FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e' LIMIT 1", nameIP, nameIP);
	mysql_tquery(dbHandle, queryGlobal, "BanInfoDiscord");

	return true;
}

DSLASH:playerinfo(user, params[])
{
	if(!DSC_RequireLevel(user, 2)) return 1;

	new username[33];
	DCC_GetUserName(user, username, sizeof(username));

	new player[MAX_PLAYER_NAME + 1];
	if(sscanf(params, "s[24]", player))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /playerinfo [nick/ip].", username));

	mysql_format(dbHandle, queryGlobal, sizeof queryGlobal, "SELECT * FROM `accounts` WHERE `username` = '%e' OR `regip` = '%e' OR `lastip` = '%e' LIMIT 1", player, player, player);
	mysql_tquery(dbHandle, queryGlobal, "PlayerInfoDiscord");

	return true;
}

DSLASH:cmds(user, params[])
{
	#pragma unused params
	if(!DSC_RequireLevel(user, 1)) return 1;

	new level = DSC_GetLevel(user), body[1024], line[64], found;
	for(new lv = 1; lv <= level; lv++)
	{
		found = 0;
		for(new i; i < sizeof(g_SlashCommands); i++)
		{
			if(g_SlashCommands[i][slash_level] != lv) continue;

			if(!found++)
			{
				format(line, sizeof(line), "**%s**\n", adminnames[lv][discord]);
				strcat(body, line, sizeof(body));
			}
			else strcat(body, ", ", sizeof(body));

			format(line, sizeof(line), "`/%s`", g_SlashCommands[i][slash_name]);
			strcat(body, line, sizeof(body));
		}
		if(found) strcat(body, "\n\n", sizeof(body));
	}
	strcat(body, "Use `/commands` for the player list.", sizeof(body));

	return SendDiscordEmbedReply("Staff Commands", body, DISCORD_COLOR_INFO);
}

DSLASH:unbanisp(user, params[])
{
	if(!DSC_RequireLevel(user, 4)) return 1;

	new username[33];
	DCC_GetUserName(user, username, sizeof(username));

	new isp[150];
	if(sscanf(params, "s[150]", isp))
		return SendDiscordResponse(comandos, sprintf("**%s**, /unbanisp [isp].", username));

	mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,	"SELECT `isp` FROM `ispsbanidas` WHERE `isp` = '%e' LIMIT 1", isp);
	mysql_tquery(dbHandle, queryGlobal, "UnbanIspDiscord");

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s unbanned ISP: %s", username, isp);
    LogServerAction(0, username, "unbans", "UnbanISP (Discord)", log_details);

	return true;
}

DSLASH:banisp(user, params[])
{
	if(!DSC_RequireLevel(user, 4)) return 1;

	new bool:isBot;
	DCC_IsUserBot(user, isBot);

	if(_:user == 0 || (_:user != 0 && isBot))
	{
		return 0;
	}

	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new rcount;
	DCC_GetGuildMemberRoleCount(guildaid, user, rcount);

	for(new i; i != rcount; i++)
	{
		new DCC_Role:role;
		DCC_GetGuildMemberRole(guildaid, user, i, role);

		new outputRole[DCC_ID_SIZE];
		DCC_GetRoleId(role, outputRole);

		if(strequal("824869097929310219", outputRole))
			return SendDiscordResponse(comandos, sprintf(" **%s**, /banisp can only be used by MASTER+.", username));
	}

	new isp[150];
	if(sscanf(params, "s[150]", isp))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /banisp [isp].", username));

	mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,	"SELECT `isp` FROM `ispsbanidas` WHERE `isp` = '%e' LIMIT 1", isp);
	mysql_tquery(dbHandle, queryGlobal, "BanIspDiscord", "s", isp);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s banned ISP: %s", username, isp);
    LogServerAction(0, username, "bans", "BanISP (Discord)", log_details);

	return true;
}

DSLASH:aka(user, params[])
{
	if(!DSC_RequireLevel(user, 1)) return 1;

	new admin[33];
	DCC_GetUserName(user, admin, sizeof(admin));

	new username[150];
	if(sscanf(params, "s[150]", username))
		return SendDiscordResponse(comandos, sprintf("**%s**, -aka [name].", admin));

	mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,	"SELECT `regip` FROM `accounts` WHERE `username` = '%e' LIMIT 1", username);
	mysql_tquery(dbHandle, queryGlobal, "AkaDiscord", "ss", username, admin);

	return true;
}

DSLASH:gmx(user, params[])
{
	#pragma unused params
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userid[DCC_ID_SIZE];
	DCC_GetUserId(user, userid, sizeof(userid));

	if(user != DCC_INVALID_USER)
	{
		if(!DSC_RequireLevel(user, 4)) return 1;

		RestartServerSafe();
		SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, server will be restarted.", username));
		return true;
	}

	return true;
}

static stock DiscordStartTempRec(targetid, const adminName[])
{
	new stringa[250];
	format(stringa, sizeof(stringa), "%s, you have 5 minutes to start recording your screen, after that time passes you will be banned from the server.", User[targetid][userName]);

	SendClientMessage(targetid, COLOR_LIGHT_BLUE, "%s", stringa);
	GameTextForPlayer(targetid, " ~n~~b~~h~start recording", 12000, 0);

	format(stringa, sizeof(stringa), ":white_check_mark: **%s**, message sent to the player %s, now u just have to wait until he is banned.", adminName, User[targetid][userName]);
	SendDiscordResponse(comandos, stringa);

	new tempRecIP[18], tempRecUser[MAX_PLAYER_NAME + 1], adminUser[MAX_PLAYER_NAME + 1], tempRecCountry[50 + 1], tempRecISP[100 + 1];

	GetPlayerIp(targetid, tempRecIP, sizeof(tempRecIP));

	format(tempRecUser, MAX_PLAYER_NAME + 1, "%s", User[targetid][userName]);
	format(tempRecCountry, sizeof(tempRecCountry), "%s", User[targetid][Country]);
	format(tempRecISP, sizeof(tempRecISP), "%s", User[targetid][Isp]);

	format(adminUser, MAX_PLAYER_NAME + 1, "%s", adminName);

	inline const tempRecDiscord()
	{
		inline const OnTempRecDiscordChecked()
		{
			if(cache_num_rows())
			{
				format(stringa, sizeof(stringa), " **%s**, the player %s was banned in the meanwhile of the temprec. The temprec got cancelled.", adminUser, tempRecUser);
				SendDiscordResponse(comandos, stringa);
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
				tempRecUser, tempRecIP, adminUser, reason, tempRecCountry, tempRecISP, bantimestamp, unbants);
				mysql_tquery(dbHandle, queryGlobal);

				va_SendClientMessageToAll(-1, BAN_TEMPREC_REASON, adminUser,  tempRecUser, hours, reason);

				if(IsPlayerConnected(targetid) && strequal(User[targetid][userName], tempRecUser))
				{
					SendClientMessage(targetid, COLOR_RED, "%s", BAN_TEMPREC_MINUTES);
					SendClientMessage(targetid, COLOR_RED, BAN_TEMPREC_WHOBANNED, adminUser, reason, tempRecIP);
					User[targetid][OnTempREC] = false;
					KickEx(targetid, 2000);
				}

				new string[1024];
				format(string, sizeof(string), "**Admin:** %s\n**Target:** %s", adminUser, tempRecUser);

				SendDiscordEmbed(logs_ban, "New Ban - GTEMP/TEMPREC", string, DISCORD_COLOR_ERROR);

				new log_details[256];
				format(log_details, sizeof(log_details), "Discord Admin %s banned player %s via Temprec (refused/timeout)", adminUser, tempRecUser);
				LogServerAction(0, adminUser, "bans", "Ban-Temprec (Discord)", log_details);
			}
		}
		MySQL_TQueryInline(dbHandle, using inline OnTempRecDiscordChecked,
			"SELECT `id` FROM `banimentos` WHERE `username` = '%e' LIMIT 1", tempRecUser);
	}

	Timer_CreateCallback(using inline tempRecDiscord, 300000, 1);

	User[targetid][OnTempREC] = true;
	return 1;
}

static stock DiscordTempRecCommand(DCC_User:user, const cmdName[], const params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new targetid;
	if(sscanf(params, "r", targetid))
		return SendDiscordResponse(comandos, sprintf(" **%s**, .%s [id].", username, cmdName));

	if(!IsPlayerLogged(targetid))
		return SendDiscordResponse(comandos, sprintf(" **%s**, the player is not online!", username));

	if(User[targetid][adminLevel] > 1)
		return SendDiscordResponse(comandos, SUPERIOR_RIGHTS_DISCORD);

	return DiscordStartTempRec(targetid, username);
}

DSLASH:gtemp(user, params[])
{
	if(!DSC_RequireLevel(user, 1)) return 1;

	return DiscordTempRecCommand(user, "gtemp", params);
}

DSLASH:temprec(user, params[])
{
	if(!DSC_RequireLevel(user, 1)) return 1;

	return DiscordTempRecCommand(user, "temprec", params);
}

DSLASH:banomp(user, params[])
{
	if(!DSC_RequireLevel(user, 1)) return 1;

	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new targetid;
	if(sscanf(params, "r", targetid))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /banomp [id].", username));

	if(!IsPlayerLogged(targetid))
		return SendDiscordResponse(comandos, sprintf(" **%s**, the player is not online!", username));

	if(User[targetid][adminLevel] > 1)
		return SendDiscordResponse(comandos, SUPERIOR_RIGHTS_DISCORD);

	if(IsPlayerUsingOmp(targetid))
		return SendDiscordResponse(comandos, sprintf(" **%s**, this player is already using the open.mp launcher.", username));

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", User[targetid][userName]);

	mysql_tquery(dbHandle, queryGlobal, "ForceOMPDiscord", "sds", username, targetid, User[targetid][userName]);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s forced open.mp launcher on player %s (ID: %d)", username, User[targetid][userName], targetid);
    LogServerAction(0, username, "bans", "BanOMP (Discord)", log_details);

	return true;
}

DSLASH:unbanomp(user, params[])
{
	if(!DSC_RequireLevel(user, 2)) return 1;

	new bool:isBot;
	DCC_IsUserBot(user, isBot);

	if(_:user == 0 || (_:user != 0 && isBot))
	{
		return 0;
	}

	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new rcount;
	DCC_GetGuildMemberRoleCount(guildaid, user, rcount);

	for(new i; i != rcount; i++)
	{
		new DCC_Role:role;
		DCC_GetGuildMemberRole(guildaid, user, i, role);

		new outputRole[DCC_ID_SIZE];
		DCC_GetRoleId(role, outputRole);

		if(strequal("824869097929310219", outputRole))
			return SendDiscordResponse(comandos, sprintf(" **%s**, /unbanomp can only be used by MASTER+.", username));
	}

	new player[MAX_PLAYER_NAME + 1];
	if(sscanf(params, "s[24]", player))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /unbanomp [name].", username));

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT `id` FROM `accounts` WHERE `username` = '%e' AND `forced_omp` = 1 LIMIT 1", player);

	mysql_tquery(dbHandle, queryGlobal, "UnBanOMPDiscord", "ss", username, player);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s removed forced open.mp launcher from player %s", username, player);
    LogServerAction(0, username, "unbans", "UnbanOMP (Discord)", log_details);

	return true;
}

DSLASH:unbanip(user, params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userid[DCC_ID_SIZE];
	DCC_GetUserId(user, userid, sizeof(userid));

	new playerip[20];

	if(sscanf(params, "s[20]", playerip))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /unbanip [ip].", username));

	if(user != DCC_INVALID_USER)
	{
		if(!DSC_RequireLevel(user, 2)) return 1;

		mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"UPDATE `banimentos` SET `ip` = NULL WHERE `ip` = '%e'", playerip);

		mysql_tquery(dbHandle, queryGlobal);
		return SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, IP unbanned.", username));
		}

	return true;
}

DSLASH:dox(user, params[])
{
	if(!DSC_RequireLevel(user, 2)) return 1;

	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userIP[MAX_PLAYER_NAME + 1];
    if(sscanf(params, "s[24]", userIP))
        return SendDiscordResponse(comandos, sprintf(" **%s**, /dox [ip/username].", username));

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT * FROM `accounts` WHERE `username` = '%e' OR `regip` = '%e' LIMIT 1", userIP, userIP);
	mysql_tquery(dbHandle, queryGlobal, "DoxDiscord", "s", username);

	return true;
}

DSLASH:turnoff(user, params[])
{
	#pragma unused params
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userid[DCC_ID_SIZE];
	DCC_GetUserId(user, userid, sizeof(userid));

	if(user != DCC_INVALID_USER)
	{
		if(!DSC_RequireLevel(user, 5)) return 1;

		for(new j; j < 20; j++)
		{
			SendClientMessageToAll(randomcolor[random(sizeof(randomcolor))], "Server will be turned off for maintenance.");
		}

		foreach(new k: Player)
		{
			if(User[k][isLogged])
				UpdatePlayerData(k);
		}

		SendRconCommand("exit");
		return SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, server will be turned off.", username));
	}

	return true;
}

DSLASH:reloadbans(user, params[])
{
	#pragma unused params
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userid[DCC_ID_SIZE];
	DCC_GetUserId(user, userid, sizeof(userid));

	if(user != DCC_INVALID_USER)
	{
		if(!DSC_RequireLevel(user, 4)) return 1;

		SendRconCommand("reloadbans");
		return SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, RCON Bans reloaded.", username));
	}

	return true;
}

DSLASH:rconunbanip(user, params[])
{
	new username[33];
	DCC_GetUserName(user, username, sizeof username);

	new userid[DCC_ID_SIZE];
	DCC_GetUserId(user, userid, sizeof(userid));

	new bannedIP[18];
	if(sscanf(params, "s[18]", bannedIP))
		return SendDiscordResponse(comandos, sprintf(" **%s**, /rconunbanip [ip].", username));

	if(user != DCC_INVALID_USER)
	{
		if(!DSC_RequireLevel(user, 4)) return 1;

		SendRconCommand("unbanip %s", bannedIP);
		return SendDiscordResponse(comandos, sprintf(":white_check_mark: **%s**, RCON IP unbanned.", username));
	}

	return true;
}

DSLASH:stafflist(user, params[])
{
	#pragma unused params
	if(!DSC_RequireLevel(user, 1)) return 1;

	inline const StaffList()
	{
		new rows = cache_num_rows(), list[384], line[96];
		new level, username[MAX_PLAYER_NAME + 1], lastlogin;

		new DCC_Embed:embed = BuildDiscordEmbed(fmat_return("%s - Staff List", ServerHostname), " ", DISCORD_COLOR_ADMIN);

		for(new lv = MAX_ADMIN_LEVEL; lv >= 1; lv--)
		{
			list[0] = EOS;

			for(new i; i < rows; i++)
			{
				cache_get_value_int(i, "adminlevel", level);
				if(level != lv) continue;

				cache_get_value(i, "username", username);
				cache_get_value_int(i, "last_login", lastlogin);

				format(line, sizeof(line), "%s - %s\n", username, returnDate(lastlogin - 10800));
				strcat(list, line, sizeof(list));
			}

			if(!isnull(list)) DCC_AddEmbedField(embed, adminnames[lv][discord], list, false);
		}
		SendDiscordEmbedResponse(comandos, embed);
	}

	MySQL_TQueryInline(dbHandle, using inline StaffList, "SELECT username, adminlevel, last_login FROM `accounts` WHERE adminlevel > 0 ORDER BY adminlevel DESC");

	return true;
}

DSLASH:stats(user, params[])
{
	if(!DSC_RequirePlayer(user)) return 1;

    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));

    new target_nick[24];
    new bool:hasTarget = false;
    if(!IsNull(params) && sscanf(params, "s[24]", target_nick) == 0)
    {
        hasTarget = true;
    }

    inline const OnDiscordStats()
    {
        if(cache_num_rows())
        {
            new acc_id, nick[24], score, money, kills, deaths, last_login, clan_id, c_name[32], c_tag[10];
            cache_get_value_int(0, "id", acc_id);
            cache_get_value(0, "username", nick);
            cache_get_value_int(0, "level", score);
            cache_get_value_int(0, "player_money", money);
            cache_get_value_int(0, "kills", kills);
            cache_get_value_int(0, "deaths", deaths);
            cache_get_value_int(0, "last_login", last_login);
            cache_get_value_int(0, "clanid", clan_id);

            new clan_display[48] = "None";
            if(clan_id != -1)
            {
                cache_get_value_name(0, "c_name", c_name);
                cache_get_value_name(0, "c_tag", c_tag);
                if(strlen(c_name) > 0) format(clan_display, sizeof(clan_display), "[%s] %s", c_tag, c_name);
                else format(clan_display, sizeof(clan_display), "ID: %d", clan_id);
            }

            new Float:kd = (deaths == 0) ? float(kills) : (float(kills) / float(deaths));
            
            new username[33];
            DCC_GetUserName(user, username, sizeof(username));

            new desc[512], descEncoded[512];
            format(desc, sizeof(desc), 
                "**Requested by:** %s\n\n" \
                "**Nick:** %s\n" \
                "**Score:** %d\n" \
                "**Clan:** %s\n" \
                "**Money:** $%d\n" \
                "**K/D:** %.2f\n" \
                "**Last Login:** %s\n\n" \
                "**[Full Profile UCP](https://rejecting.site/?tab=profile&id=%d)**", 
                username, nick, score, clan_display, money, kd, returnDate(last_login-10800), acc_id);

            utf8encode(descEncoded, desc, sizeof(descEncoded));
            SendDiscordEmbedReply("Account Statistics", descEncoded, DISCORD_COLOR_INFO);
        }
        else
        {
            if(hasTarget)
                SendDiscordResponse(comandos, "Account not found with that nickname!");
            else
                SendDiscordResponse(comandos, "Your Discord account is not linked to any game account! Use /linkdiscord in-game or specify a nick: /stats [nick].");
        }
    }

    if(hasTarget)
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), 
            "SELECT a.id, a.username, a.level, a.player_money, a.kills, a.deaths, a.last_login, a.clanid, c.clan_name AS c_name, c.clan_tag AS c_tag \
            FROM `accounts` a LEFT JOIN `clans` c ON a.clanid = c.clan_id WHERE a.`username` = '%e' LIMIT 1", target_nick);
    }
    else
    {
        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), 
            "SELECT a.id, a.username, a.level, a.player_money, a.kills, a.deaths, a.last_login, a.clanid, c.clan_name AS c_name, c.clan_tag AS c_tag \
            FROM `accounts` a LEFT JOIN `clans` c ON a.clanid = c.clan_id WHERE a.`dc_uid` = '%e' LIMIT 1", userid);
    }
    MySQL_TQueryInline(dbHandle, using inline OnDiscordStats, queryGlobal);
    return true;
}

DSLASH:d(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 1)) return 1;

    new targetid, message[128];
    if(sscanf(params, "ds[128]", targetid, message))
        return SendDiscordResponse(comandos, "/d [id] [message]");

    if(!IsPlayerLogged(targetid))
        return SendDiscordResponse(comandos, ":x: Player is not online!");

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));

    va_SendClientMessage(targetid, 0xA478E6FF, "{MAIN}[ASK] {FFFFFF}[Discord] Admin %s answered: %s", username, message);
    GameTextForPlayer(targetid, "~p~~h~Ask Answered", 1500, 1);
    PlayErrorSound(targetid);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s answered ask for player %s (ID %d): %s", username, User[targetid][userName], targetid, message);
    LogServerAction(0, username, "asks", "Ask Reply [Discord]", log_details);

    format(msgdiscord, sizeof(msgdiscord), "**Admin [Discord]:** %s\n**Target:** %s\n**Message:** %s", username, User[targetid][userName], message);
    utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
    SendDiscordEmbed(logs_ask, "Question Reply", msgencoded, DISCORD_COLOR_INFO);

    return SendDiscordResponse(comandos, sprintf(":white_check_mark: Question answer sent to **%s** (ID %d)!", User[targetid][userName], targetid));
}

DSLASH:r(user, params[])
{
    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(!DSC_RequireLevel(user, 1)) return 1;

    new targetid, message[128];
    if(sscanf(params, "ds[128]", targetid, message))
        return SendDiscordResponse(comandos, "/r [id] [message]");

    if(!IsPlayerLogged(targetid))
        return SendDiscordResponse(comandos, ":x: Player is not online!");

    new username[33];
    DCC_GetUserName(user, username, sizeof(username));

    va_SendClientMessage(targetid, 0x91BFCCFF, "{MAIN}[REPORT] {FFFFFF}[Discord] Admin %s replied: %s", username, message);
    GameTextForPlayer(targetid, "~b~~h~Report Answered", 1500, 1);
    PlayErrorSound(targetid);

    new log_details[256];
    format(log_details, sizeof(log_details), "Discord Admin %s replied to player %s (ID %d): %s", username, User[targetid][userName], targetid, message);
    LogServerAction(0, username, "reports", "Report Reply [Discord]", log_details);

    format(msgdiscord, sizeof(msgdiscord), "**Admin [Discord]:** %s\n**Target:** %s\n**Message:** %s", username, User[targetid][userName], message);
    utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
    SendDiscordEmbed(logs_report, "Report Reply", msgencoded, DISCORD_COLOR_INFO);

    return SendDiscordResponse(comandos, sprintf(":white_check_mark: Reply sent to **%s** (ID %d)!", User[targetid][userName], targetid));
}

DSLASH:reply(user, params[])
{
    return DSCI_r(user, params);
}

DSLASH:rules(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	new body[2048];
	format(body, sizeof(body), "**Server Rules - %s**\n\n", ServerHostname);

	for(new i = 0; i < sizeof(g_ServerRules); i++)
		strcat(body, fmat_return("**%d.** %s\n", i + 1, g_ServerRules[i]), sizeof(body));

	strcat(body, "\n> ", sizeof(body));
	strcat(body, g_ServerRulesNote, sizeof(body));

	return SendDiscordResponse(comandos, body);
}

DSLASH:help(user, params[])
{
	#pragma unused params
	return DSC_commands(g_CurrentInteraction, user);
}

DSLASH:commands(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	new body[1024], line[128];
	format(body, sizeof(body), "**Available commands**\n\n");

	for(new i; i < sizeof(g_SlashCommands); i++)
	{
		if(g_SlashCommands[i][slash_level] != 0) continue;

		format(line, sizeof(line), "`/%s` - %s\n", g_SlashCommands[i][slash_name], g_SlashCommands[i][slash_desc]);
		strcat(body, line, sizeof(body));
	}

	if(DSC_GetLevel(user) > 0)
		strcat(body, "\n:shield: Staff: use `/cmds` for the administrative list.", sizeof(body));

	return SendDiscordResponse(comandos, body);
}

DSLASH:online(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	new body[1800], count = 0;
	format(body, sizeof(body), "**Players online**\n\n");

	foreach(new i : Player)
	{
		if(!User[i][isLogged]) continue;

		new where[48];
		new loc = User[i][locationId];

		if(IsPlayerInLounge(i))
		{
			format(where, sizeof(where), "Lounge");
		}
		else if(0 <= loc < MAX_LOCATIONS)
		{
			new zone = arenaInfo[loc][arenaId];
			if(0 <= zone < MAX_ZONES) format(where, sizeof(where), "%s", arenaData[zone][arenaName]);
			else format(where, sizeof(where), "Arena");
		}
		else format(where, sizeof(where), "Lounge");

		if(strlen(body) < 1600)
			strcat(body, fmat_return("`%-20s` %s - %d ms\n", User[i][userName], where, GetPlayerPing(i)), sizeof(body));

		count++;
	}

	if(!count)
		return SendDiscordResponse(comandos, ":zzz: Nobody is online right now.");

	strcat(body, fmat_return("\n**%d/%d** connected.", count, GetMaxPlayers()), sizeof(body));
	return SendDiscordResponse(comandos, body);
}

DSLASH:howtolink(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	new body[768];
	format(body, sizeof(body), "**Linking your game account**\n\n");
	strcat(body, "1. Join the server and log into your account\n", sizeof(body));
	strcat(body, "2. Type `/linkdiscord` in-game\n", sizeof(body));
	strcat(body, "3. Enter your Discord username or ID in the dialog\n", sizeof(body));
	strcat(body, "4. You will get a code in your Discord DM - keep your DMs open\n", sizeof(body));
	strcat(body, "5. Type that code in-game to confirm\n\n", sizeof(body));
	strcat(body, "Linking grants the member role, unlocks the bot commands and enables auto-login.\n", sizeof(body));
	strcat(body, "Use `/udiscord` in-game to unlink.", sizeof(body));

	return SendDiscordResponse(comandos, body);
}

DSLASH:vip(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	new body[1024];
	format(body, sizeof(body), "**VIP packages**\n\nActivate in-game with `/buyvip`.\n\n");

	for(new i = 0; i < GetVipPackageCount(); i++)
		strcat(body, fmat_return("**%d days** - $%d or %d coins\n",
			GetVipPackageDays(i), GetVipPackageMoney(i), GetVipPackageCoins(i)), sizeof(body));

	strcat(body, "\n**Perks**\n", sizeof(body));
	strcat(body, "- 2x money on kills and outpost captures\n", sizeof(body));
	strcat(body, "- 3D [VIP] tag above your character\n", sizeof(body));
	strcat(body, "- VIP role and channels on Discord\n", sizeof(body));
	strcat(body, "- Private VIP chat in-game (`/v`)\n", sizeof(body));

	return SendDiscordResponse(comandos, body);
}

DSLASH:top(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	inline const OnDiscordTop()
	{
		new rows = cache_num_rows();
		if(!rows)
			return SendDiscordResponse(comandos, "No ranking data available yet.");

		new body[1200];
		format(body, sizeof(body), "**Top 10 - kills**\n\n");

		for(new i = 0; i < rows; i++)
		{
			new nick[MAX_PLAYER_NAME + 1], kills, deaths;
			cache_get_value_name(i, "username", nick, sizeof(nick));
			cache_get_value_name_int(i, "kills", kills);
			cache_get_value_name_int(i, "deaths", deaths);

			strcat(body, fmat_return("**%d.** `%-20s` %d kills / %d deaths (%.2f)\n",
				i + 1, nick, kills, deaths, ratio(kills, deaths)), sizeof(body));
		}
		return SendDiscordResponse(comandos, body);
	}

	MySQL_TQueryInline(dbHandle, using inline OnDiscordTop,
		"SELECT `username`, `kills`, `deaths` FROM `accounts` ORDER BY `kills` DESC LIMIT 10");
	return true;
}

DSLASH:topclans(user, params[])
{
	#pragma unused params
	if(!DSC_RequirePlayer(user)) return 1;

	inline const OnDiscordTopClans()
	{
		new rows = cache_num_rows();
		if(!rows)
			return SendDiscordResponse(comandos, "No clan ranking data available yet.");

		new body[1400];
		format(body, sizeof(body), "**🏆 Top 10 Clans**\n\n");

		for(new i = 0; i < rows; i++)
		{
			new tag[CLAN_TAG_LEN], name[CLAN_NAME_LEN], founder[MAX_PLAYER_NAME + 1], points, members;
			cache_get_value_name(i, "clan_tag", tag, sizeof(tag));
			cache_get_value_name(i, "clan_name", name, sizeof(name));
			cache_get_value_name(i, "clan_owner", founder, sizeof(founder));
			cache_get_value_name_int(i, "clan_points", points);
			cache_get_value_name_int(i, "member_count", members);

			strcat(body, fmat_return("**%d.** `[%s]` **%s** \n> Leader: `%s` | Members: **%d** | Points: **%d**\n\n",
				i + 1, tag, name, founder, members, points), sizeof(body));
		}
		return SendDiscordResponse(comandos, body);
	}

	MySQL_TQueryInline(dbHandle, using inline OnDiscordTopClans,
		"SELECT c.clan_tag, c.clan_name, c.clan_owner, c.clan_points, COUNT(a.id) AS member_count \
		FROM `clans` c LEFT JOIN `accounts` a ON a.clanid = c.clan_id \
		GROUP BY c.clan_id ORDER BY c.clan_points DESC, member_count DESC LIMIT 10");
	return true;
}

DSLASH:clan(user, params[])
{
	if(!DSC_RequirePlayer(user)) return 1;

	new queryText[64];
	if(sscanf(params, "s[64]", queryText))
		return SendDiscordResponse(comandos, "/clan [tag/name]");

	inline const OnDiscordClanInfo()
	{
		new rows = cache_num_rows();
		if(!rows)
			return SendDiscordResponse(comandos, ":x: Clan not found with that tag or name!");

		new cid, tag[CLAN_TAG_LEN], name[CLAN_NAME_LEN], founder[MAX_PLAYER_NAME + 1], color, points, members;
		cache_get_value_name_int(0, "clan_id", cid);
		cache_get_value_name(0, "clan_tag", tag, sizeof(tag));
		cache_get_value_name(0, "clan_name", name, sizeof(name));
		cache_get_value_name(0, "clan_owner", founder, sizeof(founder));
		cache_get_value_name_int(0, "clan_color", color);
		cache_get_value_name_int(0, "clan_points", points);
		cache_get_value_name_int(0, "member_count", members);

		new desc[1024], descEncoded[1024];
		format(desc, sizeof(desc),
			"**Clan:** [%s] %s\n" \
			"**Leader / Founder:** %s\n" \
			"**Clan ID:** %d\n" \
			"**Total Members:** %d\n" \
			"**Outpost Points:** %d\n" \
			"**Color (Hex):** #%06x\n\n" \
			"**[View Clan on Web UCP](https://rejecting.site/?tab=view_clan&id=%d)**",
			tag, name, founder, cid, members, points, (color >>> 8), cid);

		utf8encode(descEncoded, desc, sizeof(descEncoded));
		SendDiscordEmbedReply("Clan Information", descEncoded, DISCORD_COLOR_INFO);
	}

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"SELECT c.clan_id, c.clan_tag, c.clan_name, c.clan_owner, c.clan_color, c.clan_points, COUNT(a.id) AS member_count \
		FROM `clans` c LEFT JOIN `accounts` a ON a.clanid = c.clan_id \
		WHERE c.`clan_tag` = '%e' OR c.`clan_name` LIKE '%e' GROUP BY c.clan_id LIMIT 1", queryText, queryText);
	MySQL_TQueryInline(dbHandle, using inline OnDiscordClanInfo, queryGlobal);
	return true;
}

DSLASH:tournament(user, params[])
{
	if(g_CurrentInteractionChannel != DCC_INVALID_CHANNEL)
	{
		if(ch_tourney_register == DCC_INVALID_CHANNEL) ch_tourney_register = DCC_FindChannelById(CH_TOURNEY_REGISTER);
		if(comandos == DCC_INVALID_CHANNEL) comandos = DCC_FindChannelById(CH_COMANDOS);
		if(logs_linked == DCC_INVALID_CHANNEL) logs_linked = DCC_FindChannelById(CH_LOGS_LINKED);

		if(g_CurrentInteractionChannel != ch_tourney_register && g_CurrentInteractionChannel != comandos && g_CurrentInteractionChannel != logs_linked)
		{
			return DSC_Deny(":no_entry: Tournament registration commands can only be used in <#" CH_TOURNEY_REGISTER "> or <#" CH_COMANDOS ">!");
		}
	}
	return ProcessDiscordTourneyApp(user, (g_CurrentInteractionChannel != DCC_INVALID_CHANNEL) ? (g_CurrentInteractionChannel) : ((ch_tourney_register != DCC_INVALID_CHANNEL) ? (ch_tourney_register) : ((comandos != DCC_INVALID_CHANNEL) ? (comandos) : (logs_linked))), params);
}

DSLASH:verify(user, params[])
{
	#pragma unused params
	if(g_CurrentInteractionChannel != DCC_INVALID_CHANNEL)
	{
		if(ch_comandos_players == DCC_INVALID_CHANNEL) ch_comandos_players = DCC_FindChannelById(CH_COMANDOS_PLAYERS);
		if(g_CurrentInteractionChannel != ch_comandos_players)
		{
			return DSC_Deny(":no_entry: Verification commands can only be used in <#" CH_COMANDOS_PLAYERS ">!");
		}
	}
	new DCC_Interaction:it = DSC_TakeInteraction();
	if(it != DCC_Interaction:0)
	{
		DCC_SendInteractionMessage(it, ":envelope_with_arrow: A verification code has been sent to your **Discord DMs**!");
	}
	return ProcessDiscordVerifyCmd(user, DCC_INVALID_CHANNEL);
}

DSLASH:link(user, params[])
{
	#pragma unused params
	if(g_CurrentInteractionChannel != DCC_INVALID_CHANNEL)
	{
		if(ch_comandos_players == DCC_INVALID_CHANNEL) ch_comandos_players = DCC_FindChannelById(CH_COMANDOS_PLAYERS);
		if(g_CurrentInteractionChannel != ch_comandos_players)
		{
			return DSC_Deny(":no_entry: Verification commands can only be used in <#" CH_COMANDOS_PLAYERS ">!");
		}
	}
	new DCC_Interaction:it = DSC_TakeInteraction();
	if(it != DCC_Interaction:0)
	{
		DCC_SendInteractionMessage(it, "A verification code has been sent to your **Discord DMs**!");
	}
	return ProcessDiscordVerifyCmd(user, DCC_INVALID_CHANNEL);
}

DSLASH:weekresults(user, params[])
{
	#pragma unused params
	if(!DSC_RequireLevel(user, 4)) return 1;

	SendWeeklyResultsDiscord(false);
	SendDiscordResponse(comandos, "**Weekly results** have been broadcasted to <#" CH_WEEK_RESULTS ">!");
	return true;
}

stock ProcessDiscordTextCommand(DCC_User:author, DCC_Channel:channel, const message[])
{
	if(message[0] != '.' && message[0] != '!' && message[0] != '/') return 0;

	new cmd[32], rest[256];
	sscanf(message[1], "s[32]S()[256]", cmd, rest);

	if(!strcmp(cmd, "verify", true) || !strcmp(cmd, "link", true))
	{
		if(ch_comandos_players == DCC_INVALID_CHANNEL) ch_comandos_players = DCC_FindChannelById(CH_COMANDOS_PLAYERS);
		if(channel != ch_comandos_players)
		{
			new uid[DCC_ID_SIZE];
			DCC_GetUserId(author, uid, sizeof(uid));
			new denyMsg[256];
			format(denyMsg, sizeof(denyMsg), "<@%s> :no_entry: Verification commands can only be used in <#" CH_COMANDOS_PLAYERS ">!", uid);
			DCC_SendChannelMessage(channel, denyMsg);
			return 1;
		}
		ProcessDiscordVerifyCmd(author, DCC_INVALID_CHANNEL);
		return 1;
	}

	if(!strcmp(cmd, "tournament", true))
	{
		if(ch_tourney_register == DCC_INVALID_CHANNEL) ch_tourney_register = DCC_FindChannelById(CH_TOURNEY_REGISTER);
		if(comandos == DCC_INVALID_CHANNEL) comandos = DCC_FindChannelById(CH_COMANDOS);
		if(logs_linked == DCC_INVALID_CHANNEL) logs_linked = DCC_FindChannelById(CH_LOGS_LINKED);

		if(channel != ch_tourney_register && channel != comandos && channel != logs_linked)
		{
			new uid[DCC_ID_SIZE];
			DCC_GetUserId(author, uid, sizeof(uid));
			new denyMsg[256];
			format(denyMsg, sizeof(denyMsg), "<@%s> :no_entry: Tournament registration can only be used in <#" CH_TOURNEY_REGISTER "> or <#" CH_COMANDOS ">!", uid);
			DCC_SendChannelMessage(channel, denyMsg);
			return 1;
		}
		ProcessDiscordTourneyApp(author, channel, message);
		return 1;
	}

	if(!strcmp(cmd, "online", true) || !strcmp(cmd, "players", true))
	{
		g_CurrentInvoker = author;
		DSCI_online(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "stats", true))
	{
		g_CurrentInvoker = author;
		DSCI_stats(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "clan", true))
	{
		g_CurrentInvoker = author;
		DSCI_clan(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "topclans", true))
	{
		g_CurrentInvoker = author;
		DSCI_topclans(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "top", true))
	{
		g_CurrentInvoker = author;
		DSCI_top(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "r", true) || !strcmp(cmd, "reply", true))
	{
		g_CurrentInvoker = author;
		DSCI_r(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "d", true))
	{
		g_CurrentInvoker = author;
		DSCI_d(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "mute", true))
	{
		g_CurrentInvoker = author;
		DSCI_mute(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "unmute", true))
	{
		g_CurrentInvoker = author;
		DSCI_unmute(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "kick", true))
	{
		g_CurrentInvoker = author;
		DSCI_kick(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	if(!strcmp(cmd, "warn", true))
	{
		g_CurrentInvoker = author;
		DSCI_warn(author, rest);
		g_CurrentInvoker = DCC_INVALID_USER;
		return 1;
	}

	return 0;
}



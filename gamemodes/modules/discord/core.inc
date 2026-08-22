
static stock ApplyDiscordRole(DCC_User:user, const roleid[], bool:give)
{
    new DCC_Role:role = DCC_FindRoleById(roleid);
    if(role == DCC_INVALID_ROLE) return 0;

    if(give) DCC_AddGuildMemberRole(guildaid, user, role);
    else     DCC_RemoveGuildMemberRole(guildaid, user, role);
    return 1;
}

static stock ApplyDiscordAdminLevel(DCC_User:user, level)
{
    for(new i; i < MAX_ADMIN_LEVEL; i++)
        ApplyDiscordRole(user, g_DiscordAdminRoles[i], (i + 1) == level);

    return 1;
}

stock bool:IsDiscordUserVerified(DCC_User:user)
{
    if(user == DCC_INVALID_USER) return false;

    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));
    if(isnull(userid)) return false;

    for(new i; i < sizeof(OWNER_COMMANDS_DISCORD); i++)
    {
        if(strequal(userid, OWNER_COMMANDS_DISCORD[i])) return true;
    }

    new DCC_Role:role = DCC_FindRoleById(DISCORD_ROLE_LINKED);
    if(role != DCC_INVALID_ROLE)
    {
        new bool:has = false;
        DCC_HasGuildMemberRole(guildaid, user, role, has);
        if(has) return true;
    }

    for(new i; i < MAX_ADMIN_LEVEL; i++)
    {
        new DCC_Role:adminRole = DCC_FindRoleById(g_DiscordAdminRoles[i]);
        if(adminRole != DCC_INVALID_ROLE)
        {
            new bool:has = false;
            DCC_HasGuildMemberRole(guildaid, user, adminRole, has);
            if(has) return true;
        }
    }
    return false;
}

stock DCC_User:GetPlayerDiscordUser(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return DCC_INVALID_USER;
    if(!User[playerid][isLogged]) return DCC_INVALID_USER;

    if(!strequal(User[playerid][dc_uid], "-1", true) && User[playerid][dc_uid][0] != EOS)
        return DCC_FindUserById(User[playerid][dc_uid]);

    if(!strequal(User[playerid][dc_username], "-1", true) && User[playerid][dc_username][0] != EOS)
        return DCC_FindUserByName(User[playerid][dc_username], User[playerid][dc_discriminator]);

    return DCC_INVALID_USER;
}

stock SyncPlayerDiscordRoles(playerid)
{
    new DCC_User:user = GetPlayerDiscordUser(playerid);
    if(user == DCC_INVALID_USER) return 0;

    ApplyDiscordRole(user, DISCORD_ROLE_LINKED, true);
    ApplyDiscordRole(user, DISCORD_ROLE_VIP, IsPlayerVip(playerid) != 0);
    ApplyDiscordAdminLevel(user, User[playerid][adminLevel]);

    if(guildaid == DCC_INVALID_GUILD)
        guildaid = DCC_FindGuildById(DISCORD_GUILD_ID);

    if(guildaid != DCC_INVALID_GUILD)
    {
        new currentNick[DCC_NICKNAME_SIZE];
        DCC_GetGuildMemberNickname(guildaid, user, currentNick, sizeof(currentNick));
        if(!strequal(currentNick, User[playerid][userName]))
        {
            DCC_SetGuildMemberNickname(guildaid, user, User[playerid][userName]);
        }
    }
    return 1;
}

stock ClearPlayerDiscordRoles(playerid)
{
    new DCC_User:user = GetPlayerDiscordUser(playerid);
    if(user == DCC_INVALID_USER) return 0;

    ApplyDiscordRole(user, DISCORD_ROLE_LINKED, false);
    ApplyDiscordRole(user, DISCORD_ROLE_VIP, false);
    ApplyDiscordAdminLevel(user, 0);
    return 1;
}

stock CheckDiscordUser(playerid, const username[], const uid[], Func:cb<>)
{
	Indirect_Claim(cb);

	inline const OnDiscordUserChecked()
	{
		if(cache_num_rows())
			SendClientMessage(playerid, -1, "%s", DC_INUSE);
		else
			@.cb();

		Indirect_Release(cb);
	}
	MySQL_TQueryInline(dbHandle, using inline OnDiscordUserChecked,
		"SELECT `dc_username` FROM `accounts` WHERE `dc_username` = '%e' OR `dc_uid` = '%e' LIMIT 1", username, uid);

	return 1;
}

FailValidation(playerid)
{
	format(User[playerid][dc_username], 100, "-1");
	format(User[playerid][dc_discriminator], 100, "-1");
	format(User[playerid][dc_uid], 100, "-1");
	format(User[playerid][dc_code], 100, "-1");

	SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}Discord verification failed or code expired. Please try again.");
	return true;
}

#define MAX_PENDING_DC_VERIFIES 64

enum E_PENDING_DC_VERIFY
{
    bool:pv_Active,
    pv_DiscordID[DCC_ID_SIZE],
    pv_DiscordUsername[DCC_USERNAME_SIZE],
    pv_DiscordDiscriminator[10],
    pv_Code[16],
    pv_ExpireTimestamp
};

new g_PendingDCVerify[MAX_PENDING_DC_VERIFIES][E_PENDING_DC_VERIFY];

stock CreatePendingDCVerify(const uid[], const username[], const discriminator[], const code[], expireSeconds = 600)
{
    new slot = -1;
    new now = gettime();

    for(new i = 0; i < MAX_PENDING_DC_VERIFIES; i++)
    {
        if(g_PendingDCVerify[i][pv_Active] && !strcmp(g_PendingDCVerify[i][pv_DiscordID], uid, true))
        {
            slot = i;
            break;
        }
    }

    if(slot == -1)
    {
        for(new i = 0; i < MAX_PENDING_DC_VERIFIES; i++)
        {
            if(!g_PendingDCVerify[i][pv_Active] || g_PendingDCVerify[i][pv_ExpireTimestamp] < now)
            {
                slot = i;
                break;
            }
        }
    }

    if(slot == -1) slot = random(MAX_PENDING_DC_VERIFIES);

    g_PendingDCVerify[slot][pv_Active] = true;
    strcopy(g_PendingDCVerify[slot][pv_DiscordID], uid, DCC_ID_SIZE);
    strcopy(g_PendingDCVerify[slot][pv_DiscordUsername], username, DCC_USERNAME_SIZE);
    strcopy(g_PendingDCVerify[slot][pv_DiscordDiscriminator], discriminator, 10);
    strcopy(g_PendingDCVerify[slot][pv_Code], code, 16);
    g_PendingDCVerify[slot][pv_ExpireTimestamp] = now + expireSeconds;

    return slot;
}

stock FindPendingDCVerify(const code[])
{
    new now = gettime();
    for(new i = 0; i < MAX_PENDING_DC_VERIFIES; i++)
    {
        if(g_PendingDCVerify[i][pv_Active])
        {
            if(g_PendingDCVerify[i][pv_ExpireTimestamp] < now)
            {
                g_PendingDCVerify[i][pv_Active] = false;
                continue;
            }

            if(!strcmp(g_PendingDCVerify[i][pv_Code], code, false))
            {
                return i;
            }
        }
    }
    return -1;
}

stock RemovePendingDCVerify(slot)
{
    if(0 <= slot < MAX_PENDING_DC_VERIFIES)
    {
        g_PendingDCVerify[slot][pv_Active] = false;
        g_PendingDCVerify[slot][pv_DiscordID][0] = '\0';
        g_PendingDCVerify[slot][pv_Code][0] = '\0';
        g_PendingDCVerify[slot][pv_ExpireTimestamp] = 0;
    }
    return 1;
}

forward OnDiscordVerifyDMCreated(const code[]);
public OnDiscordVerifyDMCreated(const code[])
{
    new DCC_Channel:dmChannel = DCC_GetCreatedPrivateChannel();
    if(dmChannel == DCC_INVALID_CHANNEL) return 0;

    new DCC_Embed:v_embed = DCC_CreateEmbed(.title = "Account Verification", .color = DISCORD_COLOR_INFO);
    DCC_SetEmbedFooter(v_embed, getDateStrTD());

    new desc[512];
    format(desc, sizeof(desc),
        "**Your Verification Code:** `%s`\n\n"\
        "**How to link your in-game account:**\n"\
        "1. Open SA-MP and log in to the server (`%s`)\n"\
        "2. Type the following command in-game:\n"\
        "```\n/dcode %s\n```\n"\
        "*(This code expires in 10 minutes)*",
        code, ServerHostname, code);

    DCC_SetEmbedDescription(v_embed, desc);
    DCC_SendChannelEmbedMessage(dmChannel, v_embed);
    return 1;
}

stock ProcessDiscordVerifyCmd(DCC_User:user, DCC_Channel:channel = DCC_INVALID_CHANNEL)
{
    if(_:user == 0) return 0;

    new bool:isBot;
    DCC_IsUserBot(user, isBot);
    if(isBot) return 0;

    new userid[DCC_ID_SIZE], username[DCC_USERNAME_SIZE], discriminator[10];
    DCC_GetUserId(user, userid, sizeof(userid));
    DCC_GetUserName(user, username, sizeof(username));
    DCC_GetUserDiscriminator(user, discriminator, sizeof(discriminator));

    inline const OnCheckDiscordLinked()
    {
        if(cache_num_rows())
        {
            new linkedNick[MAX_PLAYER_NAME + 1];
            cache_get_value_name(0, "username", linkedNick, sizeof(linkedNick));
            new errMsg[256];
            format(errMsg, sizeof(errMsg), "<@%s> :warning: Your Discord account is already linked to the in-game account **%s**! (Use `/udiscord` in-game to unlink first if needed).", userid, linkedNick);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, errMsg);
            else DCC_CreatePrivateChannel(user, "OnDiscordPrivateMsg", "s", errMsg);
            return 1;
        }

        new genCode[16];
        format(genCode, sizeof(genCode), "%05d", random(100000));

        CreatePendingDCVerify(userid, username, discriminator, genCode, 600);

        DCC_CreatePrivateChannel(user, "OnDiscordVerifyDMCreated", "s", genCode);
        return 1;
    }

    new q[128];
    mysql_format(dbHandle, q, sizeof(q), "SELECT `id`, `username` FROM `accounts` WHERE `dc_uid` = '%e' LIMIT 1", userid);
    MySQL_TQueryInline(dbHandle, using inline OnCheckDiscordLinked, q);
    return 1;
}

forward UpdateDiscordBotActivity();
public UpdateDiscordBotActivity()
{
    new count = 0;
    foreach(new i : Player)
    {
        if(User[i][isLogged]) count++;
    }

    new actStr[64];
    if(count == 1)
        format(actStr, sizeof(actStr), "%d player online | %s", count, SERVER_IP);
    else
        format(actStr, sizeof(actStr), "%d players online | %s", count, SERVER_IP);

    DCC_SetBotActivity(actStr);
    DCC_SetBotPresenceStatus(DCC_BotPresenceStatus:ONLINE);
    return 1;
}


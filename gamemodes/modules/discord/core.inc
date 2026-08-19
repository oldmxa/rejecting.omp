
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
    if(strequal(User[playerid][dc_uid], "-1", true)) return DCC_INVALID_USER;

    return DCC_FindUserById(User[playerid][dc_uid]);
}

stock SyncPlayerDiscordRoles(playerid)
{
    new DCC_User:user = GetPlayerDiscordUser(playerid);
    if(user == DCC_INVALID_USER) return 0;

    ApplyDiscordRole(user, DISCORD_ROLE_LINKED, true);
    ApplyDiscordRole(user, DISCORD_ROLE_VIP, IsPlayerVip(playerid) != 0);
    ApplyDiscordAdminLevel(user, User[playerid][adminLevel]);
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

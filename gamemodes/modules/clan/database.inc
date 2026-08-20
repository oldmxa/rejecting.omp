#if defined _clan_database_included
    #endinput
#endif
#define _clan_database_included

stock LoadClans()
{
    inline const OnClansLoaded()
    {
        new roles_buffer[(CLAN_ROLE_LEN * MAX_ROLES) + 16], clanid;
        new rows = cache_num_rows();

        for (new i = 0; i < rows; i++)
        {
            cache_get_value_int(i, "clan_id", clanid);

            if (clanid < 0 || clanid >= MAX_CLANS)
            {
                printf("[CLAN] clan_id %d out of range 0..%d, record ignored.", clanid, MAX_CLANS - 1);
                continue;
            }

            ClanInfo[clanid][clanID] = clanid;
            ClanInfo[clanid][clanLeader][0] = '\0';
            cache_get_value_name(i, "clan_name", ClanInfo[clanid][clanName], CLAN_NAME_LEN);
            cache_get_value_name(i, "clan_tag", ClanInfo[clanid][clanTag], CLAN_TAG_LEN);
            cache_get_value(i, "clan_roles", roles_buffer, sizeof(roles_buffer));
            sscanf(roles_buffer, "p<,>s[21]s[21]s[21]s[21]s[21]", ClanRoles[clanid][0], ClanRoles[clanid][1], ClanRoles[clanid][2], ClanRoles[clanid][3], ClanRoles[clanid][4]);
            cache_get_value_int(i, "clan_zone_points", ClanInfo[clanid][clanPoints]);
            cache_get_value_int(i, "clan_kills", ClanInfo[clanid][clanKills]);
            cache_get_value_int(i, "clan_color", ClanInfo[clanid][clanColor]);
            cache_get_value_int(i, "clan_created_at", ClanInfo[clanid][clanCreatedAt]);

            if (ClanInfo[clanid][clanColor] == -1) ClanInfo[clanid][clanColor] = CLAN_COLOR_DEFAULT;

            ClanInfo[clanid][clanIDUsed] = true;

            for (new j = 0; j < MAX_LOCATIONS; j++)
            {
                ClanInfo[clanid][clanArenaKills][j] = 0;
                ClanInfo[clanid][clanArenaDMG][j] = 0;
            }
        }

        new loaded = 0;
        for (new i = 0; i < MAX_CLANS; i++)
        {
            if (!ClanInfo[i][clanIDUsed]) continue;

            LoadClanMembers(i);
            loaded++;
        }

        g_ClansLoaded = true;
        printf("[CLAN] Loaded %d clan(s).", loaded);
    }
    MySQL_TQueryInline(dbHandle, using inline OnClansLoaded, "SELECT * FROM `clans`");

    return 1;
}

stock LoadClanMembers(clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    inline const OnClanMembersLoaded()
    {
        new rows = cache_num_rows();
        if (rows > MAX_MEMBERS) rows = MAX_MEMBERS;

        ClanMembersCount[clanid] = 0;

        for (new i = 0; i < rows; i++)
        {
            cache_get_value_int(i, "id", ClanMembers[clanid][i][member_id]);
            cache_get_value_name(i, "username", ClanMembers[clanid][i][member_name], MAX_PLAYER_NAME + 1);
            cache_get_value_int(i, "clanrole", ClanMembers[clanid][i][member_role]);
            cache_get_value_int(i, "kills", ClanMembers[clanid][i][member_score]);
            ClanMembers[clanid][i][member_slotused] = true;
            ClanMembersCount[clanid]++;

            if (ClanMembers[clanid][i][member_role] == RANK_FOUNDER)
                format(ClanInfo[clanid][clanLeader], MAX_PLAYER_NAME + 1, "%s", ClanMembers[clanid][i][member_name]);
        }
    }
    MySQL_TQueryInline(dbHandle, using inline OnClanMembersLoaded,
        "SELECT `id`, `username`, `clanrole`, `kills` FROM `accounts` WHERE `clanid` = %d ORDER BY `clanrole` DESC LIMIT %d",
        clanid, MAX_MEMBERS);

    return 1;
}

stock CheckClanText(playerid, const tag[], const name[], skipClanID, Func:cb<i>)
{
    Indirect_Claim(cb);

    inline const OnClanTextChecked()
    {
        new taken = CLAN_TEXT_FREE;

        if (cache_num_rows())
        {
            new found[CLAN_TAG_LEN];
            cache_get_value_name(0, "clan_tag", found, sizeof(found));

            taken = (!isnull(tag) && !strcmp(found, tag, true)) ? CLAN_TEXT_TAG_TAKEN : CLAN_TEXT_NAME_TAKEN;

            SendClientMessage(playerid, -1, (taken == CLAN_TEXT_TAG_TAKEN) ? (CLAN_TAG_TAKEN) : (CLAN_NAME_TAKEN));
        }

        @.cb(taken);
        Indirect_Release(cb);
    }
    MySQL_TQueryInline(dbHandle, using inline OnClanTextChecked,
        "SELECT `clan_tag`, `clan_name` FROM `clans` \
        WHERE `clan_id` <> %d AND (`clan_tag` = '%e' OR `clan_name` = '%e') LIMIT 1",
        skipClanID, tag, name);

    return 1;
}

stock InsertClan(clanid, leaderSQLID, const name[], const tag[], timestamp)
{
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "INSERT INTO `clans` (`clan_id`, `clan_leader`, `clan_name`, `clan_tag`, `clan_created_at`) VALUES (%d, %d, '%e', '%e', %d)",
        clanid, leaderSQLID, name, tag, timestamp);

    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

stock UpdateClan(clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "UPDATE `clans` SET `clan_name` = '%e', `clan_tag` = '%e', `clan_zone_points` = %d, `clan_kills` = %d, `clan_color` = %d WHERE `clan_id` = %d",
        ClanInfo[clanid][clanName], ClanInfo[clanid][clanTag], ClanInfo[clanid][clanPoints], ClanInfo[clanid][clanKills], ClanInfo[clanid][clanColor], clanid);

    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

stock UpdateRoles(clanid)
{
    if (!IsValidClanID(clanid)) return 0;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "UPDATE `clans` SET `clan_roles` = '%e,%e,%e,%e,%e' WHERE `clan_id` = %d",
        ClanRoles[clanid][0], ClanRoles[clanid][1], ClanRoles[clanid][2], ClanRoles[clanid][3], ClanRoles[clanid][4], clanid);

    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

stock SaveClanData(clanid)
{
    UpdateRoles(clanid);
    UpdateClan(clanid);
    return 1;
}

stock SaveClanMemberRank(memberSQLID, rank)
{
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "UPDATE `accounts` SET `clanRole` = %d WHERE `id` = %d", rank, memberSQLID);

    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

stock ClearClanMemberAccount(memberSQLID)
{
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "UPDATE `accounts` SET `clanID` = -1, `clanRole` = -1, `spawn_safehouse` = 0 WHERE `id` = %d", memberSQLID);

    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

stock DeleteClanRows(clanid)
{
    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "DELETE FROM `clans` WHERE `clan_id` = %d", clanid);
    mysql_tquery(dbHandle, queryGlobal);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `clanID` = -1, `clanRole` = -1, `spawn_safehouse` = 0 WHERE `clanID` = %d", clanid);
    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

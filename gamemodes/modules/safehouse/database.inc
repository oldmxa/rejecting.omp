#if defined _safehouse_database_included
    #endinput
#endif
#define _safehouse_database_included

static g_SafehouseInitTries = 0;

public InitSafehouseSystem()
{
    if (dbHandle == MYSQL_INVALID_HANDLE)
    {
        if (++g_SafehouseInitTries > 30)
        {
            print("[SAFEHOUSE ERROR] MySQL unavailable, safehouses disabled.");
            return 0;
        }

        SetTimer("InitSafehouseSystem", 1000, false);
        return 1;
    }

    g_SafehouseInitTries = 0;

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `safehouses` ( \
`id` INT(11) NOT NULL AUTO_INCREMENT, \
`zone_id` INT(11) NOT NULL, \
`clan_id` INT(11) NOT NULL DEFAULT -1, \
`name` VARCHAR(32) NOT NULL DEFAULT '', \
`password` VARCHAR(32) NOT NULL DEFAULT '', \
`level` INT(11) NOT NULL DEFAULT 1, \
`interior` INT(11) NOT NULL DEFAULT 0, \
`virtual_world` INT(11) NOT NULL DEFAULT 0, \
`ext_x` FLOAT NOT NULL DEFAULT 0, \
`ext_y` FLOAT NOT NULL DEFAULT 0, \
`ext_z` FLOAT NOT NULL DEFAULT 0, \
`int_x` FLOAT NOT NULL DEFAULT 0, \
`int_y` FLOAT NOT NULL DEFAULT 0, \
`int_z` FLOAT NOT NULL DEFAULT 0, \
`int_a` FLOAT NOT NULL DEFAULT 0, \
`exit_x` FLOAT NOT NULL DEFAULT 0, \
`exit_y` FLOAT NOT NULL DEFAULT 0, \
`exit_z` FLOAT NOT NULL DEFAULT 0, \
`vault` INT(11) NOT NULL DEFAULT 0, \
`bought_at` INT(11) NOT NULL DEFAULT 0, \
`expires_at` INT(11) NOT NULL DEFAULT 0, \
`next_charge` INT(11) NOT NULL DEFAULT 0, \
`last_charge` INT(11) NOT NULL DEFAULT 0, \
`status` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`id`), \
UNIQUE KEY `zone` (`zone_id`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `safehouse_logs` ( \
`id` INT(11) NOT NULL AUTO_INCREMENT, \
`safehouse_id` INT(11) NOT NULL DEFAULT 0, \
`zone_id` INT(11) NOT NULL DEFAULT -1, \
`clan_id` INT(11) NOT NULL DEFAULT -1, \
`user_id` INT(11) NOT NULL DEFAULT 0, \
`player_name` VARCHAR(24) NOT NULL DEFAULT '', \
`action` VARCHAR(32) NOT NULL DEFAULT '', \
`detail` VARCHAR(160) NOT NULL DEFAULT '', \
`ip` VARCHAR(46) NOT NULL DEFAULT '', \
`created_at` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`id`), \
KEY `safehouse` (`safehouse_id`), \
KEY `clan` (`clan_id`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle,
        "ALTER TABLE `accounts` ADD COLUMN IF NOT EXISTS `spawn_safehouse` TINYINT(1) NOT NULL DEFAULT 0");

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `safehouse_config` ( \
`key` VARCHAR(32) NOT NULL, \
`value` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`key`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle, "SELECT * FROM `safehouse_config`", "OnSafehouseConfigLoad");
    mysql_tquery(dbHandle, "SELECT * FROM `safehouses` ORDER BY `zone_id` ASC", "OnSafehousesLoad");
    return 1;
}

public OnSafehouseConfigLoad()
{
    for (new i = 0; i < SHCFG_MAX; i++)
        g_SafehouseConfig[i] = g_SafehouseConfigDefault[i];

    new rows = cache_num_rows();

    for (new r = 0; r < rows; r++)
    {
        new key[32], value;
        cache_get_value_name(r, "key", key, sizeof(key));
        cache_get_value_name_int(r, "value", value);

        for (new i = 0; i < SHCFG_MAX; i++)
        {
            if (strcmp(key, g_SafehouseConfigKey[i], true) != 0) continue;

            g_SafehouseConfig[i] = ClampSafehouseConfig(i, value);
            break;
        }
    }

    printf("[SAFEHOUSE] Config loaded (%d stored value(s)).", rows);
    return 1;
}

stock SaveSafehouseConfig(setting)
{
    if (setting < 0 || setting >= SHCFG_MAX) return 0;
    if (dbHandle == MYSQL_INVALID_HANDLE) return 0;

    new query[192];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `safehouse_config` (`key`, `value`) VALUES ('%e', %d) \
ON DUPLICATE KEY UPDATE `value` = %d",
        g_SafehouseConfigKey[setting], g_SafehouseConfig[setting], g_SafehouseConfig[setting]);

    mysql_tquery(dbHandle, query);
    return 1;
}

public OnSafehousesLoad()
{
    for (new i = 0; i < MAX_SAFEHOUSES; i++)
        ResetSafehouse(i);

    new rows = cache_num_rows();

    for (new r = 0; r < rows; r++)
    {
        new zoneid;
        cache_get_value_name_int(r, "zone_id", zoneid);

        if (!IsValidSafehouseIndex(zoneid))
            continue;

        cache_get_value_name_int(r, "id", SafehouseData[zoneid][sh_SQLID]);
        cache_get_value_name_int(r, "clan_id", SafehouseData[zoneid][sh_ClanID]);
        cache_get_value_name(r, "name", SafehouseData[zoneid][sh_Name], SAFEHOUSE_NAME_LEN);
        cache_get_value_name(r, "password", SafehouseData[zoneid][sh_Password], SAFEHOUSE_PASS_LEN);
        cache_get_value_name_int(r, "level", SafehouseData[zoneid][sh_Level]);
        cache_get_value_name_int(r, "interior", SafehouseData[zoneid][sh_Interior]);
        cache_get_value_name_int(r, "virtual_world", SafehouseData[zoneid][sh_VirtualWorld]);

        cache_get_value_name_float(r, "ext_x", SafehouseData[zoneid][sh_ExtX]);
        cache_get_value_name_float(r, "ext_y", SafehouseData[zoneid][sh_ExtY]);
        cache_get_value_name_float(r, "ext_z", SafehouseData[zoneid][sh_ExtZ]);
        cache_get_value_name_float(r, "int_x", SafehouseData[zoneid][sh_IntX]);
        cache_get_value_name_float(r, "int_y", SafehouseData[zoneid][sh_IntY]);
        cache_get_value_name_float(r, "int_z", SafehouseData[zoneid][sh_IntZ]);
        cache_get_value_name_float(r, "int_a", SafehouseData[zoneid][sh_IntA]);
        cache_get_value_name_float(r, "exit_x", SafehouseData[zoneid][sh_ExitX]);
        cache_get_value_name_float(r, "exit_y", SafehouseData[zoneid][sh_ExitY]);
        cache_get_value_name_float(r, "exit_z", SafehouseData[zoneid][sh_ExitZ]);

        cache_get_value_name_int(r, "vault", SafehouseData[zoneid][sh_Vault]);
        cache_get_value_name_int(r, "bought_at", SafehouseData[zoneid][sh_BoughtAt]);
        cache_get_value_name_int(r, "expires_at", SafehouseData[zoneid][sh_ExpiresAt]);
        cache_get_value_name_int(r, "next_charge", SafehouseData[zoneid][sh_NextCharge]);
        cache_get_value_name_int(r, "last_charge", SafehouseData[zoneid][sh_LastCharge]);
        cache_get_value_name_int(r, "status", SafehouseData[zoneid][sh_Status]);

        if (!IsValidSafehouseLevel(SafehouseData[zoneid][sh_Level]))
            SafehouseData[zoneid][sh_Level] = 1;


        // an owner that no longer exists leaves the safehouse dangling
        if (SafehouseData[zoneid][sh_Status] == SAFEHOUSE_STATUS_OWNED &&
            !IsValidClanID(SafehouseData[zoneid][sh_ClanID]))
        {
            ReleaseSafehouse(zoneid, "owner clan no longer exists");
        }
    }

    new eligible = 0;
    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (IsSafehouseZone(i)) eligible++;
    }

    new placed = RefreshAllSafehousePickups();

    printf("[SAFEHOUSE] Loaded %d safehouse(s), %d placed on the map, %d eligible zones free.",
        rows, placed, eligible - rows);
    return 1;
}

stock DeleteSafehouseRow(index)
{
    if (!IsValidSafehouseIndex(index) || dbHandle == MYSQL_INVALID_HANDLE) return 0;
    if (SafehouseData[index][sh_SQLID] <= 0) return 0;

    new query[128];
    mysql_format(dbHandle, query, sizeof(query),
        "DELETE FROM `safehouses` WHERE `id` = %d", SafehouseData[index][sh_SQLID]);
    mysql_tquery(dbHandle, query);
    return 1;
}

stock InsertSafehouseRow(index)
{
    if (!IsValidSafehouseIndex(index) || dbHandle == MYSQL_INVALID_HANDLE)
        return 0;

    new query[320];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `safehouses` (`zone_id`, `name`, `status`, `ext_x`, `ext_y`) VALUES (%d, '%e', %d, %f, %f) \
ON DUPLICATE KEY UPDATE `zone_id` = `zone_id`",
        index, arenaData[index][arenaName], SAFEHOUSE_STATUS_AVAILABLE,
        SafehouseData[index][sh_ExtX], SafehouseData[index][sh_ExtY]);

    mysql_tquery(dbHandle, query, "OnSafehouseInserted", "i", index);
    return 1;
}

public OnSafehouseInserted(index)
{
    if (!IsValidSafehouseIndex(index)) return 1;

    new id = cache_insert_id();
    if (id <= 0) return 1;

    SafehouseData[index][sh_SQLID] = id;

    SaveSafehouse(index);
    RefreshSafehousePickup(index);
    return 1;
}

stock SaveSafehouse(index)
{
    if (!IsValidSafehouseIndex(index) || dbHandle == MYSQL_INVALID_HANDLE)
        return 0;

    if (SafehouseData[index][sh_SQLID] <= 0) return 0;

    new query[1024];
    mysql_format(dbHandle, query, sizeof(query),
        "UPDATE `safehouses` SET \
`clan_id` = %d, `name` = '%e', `password` = '%e', \
`level` = %d, `interior` = %d, `virtual_world` = %d, \
`ext_x` = %f, `ext_y` = %f, `ext_z` = %f, \
`int_x` = %f, `int_y` = %f, `int_z` = %f, `int_a` = %f, \
`exit_x` = %f, `exit_y` = %f, `exit_z` = %f, \
`vault` = %d, `bought_at` = %d, `expires_at` = %d, \
`next_charge` = %d, `last_charge` = %d, `status` = %d \
WHERE `id` = %d",
        SafehouseData[index][sh_ClanID],
        SafehouseData[index][sh_Name],
        SafehouseData[index][sh_Password],
        SafehouseData[index][sh_Level],
        SafehouseData[index][sh_Interior],
        SafehouseData[index][sh_VirtualWorld],
        SafehouseData[index][sh_ExtX], SafehouseData[index][sh_ExtY], SafehouseData[index][sh_ExtZ],
        SafehouseData[index][sh_IntX], SafehouseData[index][sh_IntY], SafehouseData[index][sh_IntZ], SafehouseData[index][sh_IntA],
        SafehouseData[index][sh_ExitX], SafehouseData[index][sh_ExitY], SafehouseData[index][sh_ExitZ],
        SafehouseData[index][sh_Vault],
        SafehouseData[index][sh_BoughtAt],
        SafehouseData[index][sh_ExpiresAt],
        SafehouseData[index][sh_NextCharge],
        SafehouseData[index][sh_LastCharge],
        SafehouseData[index][sh_Status],
        SafehouseData[index][sh_SQLID]);

    mysql_tquery(dbHandle, query);
    return 1;
}

stock LogSafehouse(index, playerid, const action[], const detail[] = "")
{
    if (dbHandle == MYSQL_INVALID_HANDLE) return 0;
    if (!IsValidSafehouseIndex(index)) return 0;

    new userid = 0, name[MAX_PLAYER_NAME + 1] = "SERVER", ip[46] = "";

    if (playerid >= 0 && playerid < MAX_PLAYERS && IsPlayerConnected(playerid))
    {
        userid = User[playerid][user_sqlid];
        format(name, sizeof(name), "%s", User[playerid][userName]);
        GetPlayerIp(playerid, ip, sizeof(ip));
    }

    new query[512];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `safehouse_logs` \
(`safehouse_id`, `zone_id`, `clan_id`, `user_id`, `player_name`, `action`, `detail`, `ip`, `created_at`) \
VALUES (%d, %d, %d, %d, '%e', '%e', '%e', '%e', %d)",
        SafehouseData[index][sh_SQLID],
        index,
        SafehouseData[index][sh_ClanID],
        userid, name, action, detail, ip, gettime());

    mysql_tquery(dbHandle, query);
    return 1;
}

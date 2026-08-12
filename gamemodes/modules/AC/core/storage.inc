#if defined _ac_storage_included
    #endinput
#endif
#define _ac_storage_included

#define AC_KEY_LEN (32)
#define AC_LOG_RETENTION_DAYS (30)

static g_ACInitTries = 0;

new ACHitCount[MAX_AC_DETECTIONS];

forward InitACStorage();
forward OnACConfigLoad();
forward OnACHitCountLoad();

stock AC_LogDetection(playerid, code, action)
{
    if (code < 0 || code >= MAX_AC_DETECTIONS) return 0;

    ACHitCount[code]++;

    if (dbHandle == MYSQL_INVALID_HANDLE) return 0;

    new pip[18], userid = User[playerid][user_sqlid], stamp = gettime();
    GetPlayerIp(playerid, pip, sizeof(pip));

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "INSERT INTO `ac_detections` (`user_id`, `username`, `code`, `action`, `ip`, `gpci`, `created_at`) VALUES(%d, '%e', %d, %d, '%e', '%e', %d)",
        userid, User[playerid][userName], code, action, pip, User[playerid][user_gpci], stamp);
    mysql_tquery(dbHandle, queryGlobal);

    if (userid <= 0) return 1;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "INSERT INTO `ac_detection_totals` (`user_id`, `code`, `hits`, `last_seen`) VALUES(%d, %d, 1, %d) \
ON DUPLICATE KEY UPDATE `hits` = `hits` + 1, `last_seen` = %d",
        userid, code, stamp, stamp);
    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

public OnACHitCountLoad()
{
    new rows = cache_num_rows();

    for (new r; r < rows; r++)
    {
        new code, total;
        cache_get_value_name_int(r, "code", code);
        cache_get_value_name_int(r, "total", total);

        if (0 <= code < MAX_AC_DETECTIONS) ACHitCount[code] = total;
    }
    return 1;
}

stock AC_SaveSetting(const key[], value)
{
    if (dbHandle == MYSQL_INVALID_HANDLE) return 0;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "INSERT INTO `ac_config` (`key`, `value`) VALUES('%e', %d) ON DUPLICATE KEY UPDATE `value` = %d",
        key, value, value);
    mysql_tquery(dbHandle, queryGlobal);
    return 1;
}

stock AC_SaveConfig()
{
    new key[AC_KEY_LEN];

    for (new i; i < MAX_AC_DETECTIONS; i++)
    {
        format(key, sizeof(key), "code_%02d", i + 1);
        AC_SaveSetting(key, AC_DETECTION_ACTION[i]);
    }

    AC_SaveSetting("punish", _:ACPunishEnabled);
    AC_SaveSetting("no_fall_damage", _:ACNoFallDamage);
    AC_SaveSetting("no_fall_anim", _:ACNoFallAnimation);
    AC_SaveSetting("fast_stuns", _:ACFastStuns);
    return 1;
}

public OnACConfigLoad()
{
    new rows = cache_num_rows(), loaded = 0, bool:totalsBuilt = false;

    for (new r; r < rows; r++)
    {
        new key[AC_KEY_LEN], value;
        cache_get_value_name(r, "key", key, sizeof(key));
        cache_get_value_name_int(r, "value", value);

        if (!strcmp(key, "punish")) { ACPunishEnabled = bool:(value != 0); continue; }
        if (!strcmp(key, "totals_built")) { totalsBuilt = bool:(value != 0); continue; }

        if (!strcmp(key, "no_fall_damage")) { ACNoFallDamage = bool:(value != 0); continue; }
        if (!strcmp(key, "no_fall_anim")) { ACNoFallAnimation = bool:(value != 0); continue; }
        if (!strcmp(key, "fast_stuns")) { ACFastStuns = bool:(value != 0); continue; }

        if (strfind(key, "code_") != 0) continue;

        new index = strval(key[5]) - 1;
        if (index < 0 || index >= MAX_AC_DETECTIONS) continue;
        if (value < AC_ACTION_IGNORE || value > AC_ACTION_BAN) continue;

        AC_DETECTION_ACTION[index] = value;
        loaded++;
    }

    printf("[RAC] %d of %d detection settings loaded", loaded, MAX_AC_DETECTIONS);

    if (loaded < MAX_AC_DETECTIONS) AC_SaveConfig();

    if (!totalsBuilt)
    {
        mysql_tquery(dbHandle,
            "INSERT INTO `ac_detection_totals` (`user_id`, `code`, `hits`, `last_seen`) \
SELECT `user_id`, `code`, COUNT(*), MAX(`created_at`) FROM `ac_detections` WHERE `user_id` > 0 GROUP BY `user_id`, `code` \
ON DUPLICATE KEY UPDATE `hits` = VALUES(`hits`), `last_seen` = VALUES(`last_seen`)");

        AC_SaveSetting("totals_built", 1);
        print("[RAC] detection totals rebuilt from the raw log");
    }

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "DELETE FROM `ac_detections` WHERE `created_at` < %d", gettime() - AC_LOG_RETENTION_DAYS * 86400);
    mysql_tquery(dbHandle, queryGlobal);

    mysql_tquery(dbHandle, "SELECT `code`, SUM(`hits`) AS `total` FROM `ac_detection_totals` GROUP BY `code`", "OnACHitCountLoad");
    return 1;
}

public InitACStorage()
{
    if (dbHandle == MYSQL_INVALID_HANDLE)
    {
        if (++g_ACInitTries > 30)
        {
            print("[RAC] MySQL unavailable, running with default settings.");
            return 0;
        }

        SetTimer("InitACStorage", 1000, false);
        return 1;
    }

    g_ACInitTries = 0;

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `ac_config` ( \
`key` VARCHAR(32) NOT NULL, \
`value` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`key`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `ac_detections` ( \
`id` INT(11) NOT NULL AUTO_INCREMENT, \
`user_id` INT(11) NOT NULL DEFAULT 0, \
`username` VARCHAR(24) NOT NULL DEFAULT '', \
`code` INT(11) NOT NULL DEFAULT 0, \
`action` INT(11) NOT NULL DEFAULT 0, \
`ip` VARCHAR(46) NOT NULL DEFAULT '', \
`gpci` VARCHAR(41) NOT NULL DEFAULT '', \
`created_at` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`id`), \
KEY `user` (`user_id`), \
KEY `code` (`code`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `ac_detection_totals` ( \
`user_id` INT(11) NOT NULL DEFAULT 0, \
`code` INT(11) NOT NULL DEFAULT 0, \
`hits` INT(11) NOT NULL DEFAULT 0, \
`last_seen` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`user_id`, `code`), \
KEY `code` (`code`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle, "SELECT * FROM `ac_config`", "OnACConfigLoad");
    return 1;
}

#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
    InitACStorage();
    return 1;
}

#if defined _training_records_included
    #endinput
#endif
#define _training_records_included

forward InitTrainingRecords();
forward OnTrainingServerRecordsLoad();
forward OnTrainingPlayerRecordsLoad(playerid, sqlid);
forward LoadPlayerChallengeRecords(playerid);

stock GetTrainingArenaByName(const name[])
{
    for (new i = 0; i < g_TotalTrainingArenas; i++)
    {
        if (strcmp(g_TrainingArenas[i][tArena_Name], name, true) == 0)
            return i;
    }
    return -1;
}

stock ClearPlayerChallengeRecords(playerid)
{
    for (new i = 0; i < TRAINING_MAX_ARENAS; i++)
    {
        g_ChallengePlayerRecord[playerid][i] = 0;
    }
    return 1;
}

static g_TrainingRecordsInitTries = 0;

public InitTrainingRecords()
{
    if (dbHandle == MYSQL_INVALID_HANDLE)
    {
        if (++g_TrainingRecordsInitTries > 30)
        {
            print("[TRAINING ERROR] MySQL unavailable, challenge records disabled.");
            return 0;
        }

        SetTimer("InitTrainingRecords", 1000, false);
        return 1;
    }

    g_TrainingRecordsInitTries = 0;

    mysql_tquery(dbHandle,
        "CREATE TABLE IF NOT EXISTS `training_records` ( \
`id` INT(11) NOT NULL AUTO_INCREMENT, \
`user_id` INT(11) NOT NULL, \
`arena_id` INT(11) NOT NULL DEFAULT 0, \
`arena_name` VARCHAR(32) NOT NULL DEFAULT '', \
`best_score` INT(11) NOT NULL DEFAULT 0, \
`best_hits` INT(11) NOT NULL DEFAULT 0, \
`best_shots` INT(11) NOT NULL DEFAULT 0, \
`best_damage` INT(11) NOT NULL DEFAULT 0, \
`set_at` INT(11) NOT NULL DEFAULT 0, \
PRIMARY KEY (`id`), \
UNIQUE KEY `user_arena` (`user_id`, `arena_name`) \
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci");

    mysql_tquery(dbHandle,
        "ALTER TABLE `training_records` ADD COLUMN IF NOT EXISTS `best_hits` INT(11) NOT NULL DEFAULT 0 AFTER `best_score`");

    mysql_tquery(dbHandle,
        "SELECT r.`arena_name`, MAX(r.`best_score`) AS `best_score`, \
SUBSTRING_INDEX(GROUP_CONCAT(a.`username` ORDER BY r.`best_score` DESC, r.`set_at` ASC SEPARATOR '|'), '|', 1) AS `holder` \
FROM `training_records` r \
INNER JOIN `accounts` a ON a.`id` = r.`user_id` \
GROUP BY r.`arena_name`",
        "OnTrainingServerRecordsLoad");

    return 1;
}

public OnTrainingServerRecordsLoad()
{
    new rows = cache_num_rows();
    new trArenaName[TRAINING_NAME_LEN], holder[MAX_PLAYER_NAME + 1], score;

    for (new i = 0; i < rows; i++)
    {
        cache_get_value_name(i, "arena_name", trArenaName);

        new arenaid = GetTrainingArenaByName(trArenaName);
        if (arenaid == -1) continue;

        cache_get_value_name_int(i, "best_score", score);
        cache_get_value_name(i, "holder", holder);

        g_ChallengeServerRecord[arenaid] = score;
        format(g_ChallengeServerHolder[arenaid], MAX_PLAYER_NAME + 1, "%s", holder);
    }

    //printf("[TRAINING] Loaded challenge records for %d arena(s).", rows);
    return 1;
}

public LoadPlayerChallengeRecords(playerid)
{
    ClearPlayerChallengeRecords(playerid);

    if (dbHandle == MYSQL_INVALID_HANDLE) return 0;
    if (!User[playerid][isLogged] || User[playerid][user_sqlid] <= 0) return 0;

    new query[160];
    mysql_format(dbHandle, query, sizeof(query),
        "SELECT `arena_name`, `best_score` FROM `training_records` WHERE `user_id` = %d",
        User[playerid][user_sqlid]);

    mysql_tquery(dbHandle, query, "OnTrainingPlayerRecordsLoad", "ii", playerid, User[playerid][user_sqlid]);
    return 1;
}

public OnTrainingPlayerRecordsLoad(playerid, sqlid)
{
    if (!IsPlayerConnected(playerid)) return 1;
    if (!User[playerid][isLogged] || User[playerid][user_sqlid] != sqlid) return 1;

    new rows = cache_num_rows();
    new trArenaName[TRAINING_NAME_LEN], score;

    for (new i = 0; i < rows; i++)
    {
        cache_get_value_name(i, "arena_name", trArenaName);

        new arenaid = GetTrainingArenaByName(trArenaName);
        if (arenaid == -1) continue;

        cache_get_value_name_int(i, "best_score", score);
        g_ChallengePlayerRecord[playerid][arenaid] = score;
    }
    return 1;
}

stock SaveChallengeRecord(playerid, arenaid, score, hits, shots)
{
    if (!(0 <= arenaid < g_TotalTrainingArenas))
        return TRAINING_RECORD_NONE;

    new result = TRAINING_RECORD_NONE;

    if (score > g_ChallengePlayerRecord[playerid][arenaid])
    {
        result = TRAINING_RECORD_PERSONAL;
        g_ChallengePlayerRecord[playerid][arenaid] = score;

        if (score > g_ChallengeServerRecord[arenaid])
        {
            result = TRAINING_RECORD_SERVER;
            g_ChallengeServerRecord[arenaid] = score;
            format(g_ChallengeServerHolder[arenaid], MAX_PLAYER_NAME + 1, "%s", User[playerid][userName]);
        }
    }

    if (score <= 0) return result;

    if (dbHandle == MYSQL_INVALID_HANDLE) return result;
    if (!User[playerid][isLogged] || User[playerid][user_sqlid] <= 0) return result;

    new query[768];
    new timestamp = gettime();

    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `training_records` \
(`user_id`, `arena_id`, `arena_name`, `best_score`, `best_hits`, `best_shots`, `best_damage`, `set_at`) \
VALUES (%d, %d, '%e', %d, %d, %d, %d, %d) \
ON DUPLICATE KEY UPDATE \
`arena_id` = %d, \
`best_hits` = IF(%d > `best_score`, %d, `best_hits`), \
`best_shots` = IF(%d > `best_score`, %d, `best_shots`), \
`best_damage` = IF(%d > `best_score`, %d, `best_damage`), \
`set_at` = IF(%d > `best_score`, %d, `set_at`), \
`best_score` = GREATEST(`best_score`, %d)",
        User[playerid][user_sqlid], arenaid, g_TrainingArenas[arenaid][tArena_Name], score, hits, shots, score, timestamp,
        arenaid,
        score, hits,
        score, shots,
        score, score,
        score, timestamp,
        score);

    mysql_tquery(dbHandle, query);
    return result;
}

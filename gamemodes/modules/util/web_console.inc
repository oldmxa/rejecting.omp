#if defined _web_console_included
    #endinput
#endif
#define _web_console_included

#define WEB_CMD_POLL_MS      3000
#define WEB_CMD_BATCH        5
#define WEB_CMD_MAX_LEN      160
#define WEB_CMD_RESPONSE_LEN 200
#define WEB_CMD_RCON_LEVEL    5

forward WebConsolePoll();
forward OnWebCommandsFetched();

static WebConsoleFinish(id, const status[], const response[])
{
    new query[512];

    mysql_format(dbHandle, query, sizeof(query),
        "UPDATE `web_commands` SET `status` = '%e', `response` = '%e', `executed_at` = NOW() WHERE `id` = %d",
        status, response, id);

    mysql_tquery(dbHandle, query);
}

static WebConsoleRun(id, const command[], const adminName[], nivelExigido)
{
    new playerid = INVALID_PLAYER_ID;

    foreach (new i : Player)
    {
        if (!strcmp(User[i][userName], adminName, true))
        {
            playerid = i;
            break;
        }
    }

    if (playerid != INVALID_PLAYER_ID)
    {
        if (User[playerid][adminLevel] < nivelExigido)
        {
            WebConsoleFinish(id, "failed", "Admin level dropped since the command was queued.");
            return;
        }

        new cmd[WEB_CMD_MAX_LEN + 2];
        format(cmd, sizeof(cmd), "/%s", command);

        Command_ReProcess(playerid, cmd, 0);
        WebConsoleFinish(id, "done", "Executed in game as the admin.");
        return;
    }

    if (nivelExigido < WEB_CMD_RCON_LEVEL)
    {
        new recusa[WEB_CMD_RESPONSE_LEN];
        format(recusa, sizeof(recusa),
            "Admin is offline. Only level %d can run commands through RCON.", WEB_CMD_RCON_LEVEL);

        WebConsoleFinish(id, "failed", recusa);
        return;
    }

    SendRconCommand(command);
    WebConsoleFinish(id, "done", "Admin offline, sent to RCON.");
}

public OnWebCommandsFetched()
{
    new rows = cache_num_rows();

    for (new r = 0; r < rows; r++)
    {
        new id, nivel, command[WEB_CMD_MAX_LEN], adminName[MAX_PLAYER_NAME + 1];

        cache_get_value_int(r, "id", id);
        cache_get_value_int(r, "admin_level", nivel);
        cache_get_value_name(r, "command", command, sizeof(command));
        cache_get_value_name(r, "admin_name", adminName, sizeof(adminName));

        if (id <= 0 || isnull(command))
        {
            continue;
        }

        WebConsoleRun(id, command, adminName, nivel);
    }

    return 1;
}

public WebConsolePoll()
{
    new query[256];

    mysql_format(dbHandle, query, sizeof(query),
        "UPDATE `web_commands` SET `status` = 'running' WHERE `status` = 'pending' ORDER BY `id` LIMIT %d",
        WEB_CMD_BATCH);

    mysql_tquery(dbHandle, query);

    mysql_format(dbHandle, query, sizeof(query),
        "SELECT `id`, `admin_name`, `admin_level`, `command` FROM `web_commands` \
         WHERE `status` = 'running' ORDER BY `id` LIMIT %d",
        WEB_CMD_BATCH);

    mysql_tquery(dbHandle, query, "OnWebCommandsFetched");

    return 1;
}

stock WebConsoleInit()
{
    mysql_tquery(dbHandle,
        "UPDATE `web_commands` SET `status` = 'failed', \
         `response` = 'Server restarted before running.', `executed_at` = NOW() \
         WHERE `status` = 'running'");

    SetTimer("WebConsolePoll", WEB_CMD_POLL_MS, true);
}

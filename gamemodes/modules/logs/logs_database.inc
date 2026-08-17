#if defined _logs_database_included
    #endinput
#endif
#define _logs_database_included

LogServerAction(user_id, const username[], const log_category[], const action[], const details[])
{
    new query[1024];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `server_logs` (`user_id`, `username`, `category`, `action`, `details`) VALUES (%d, '%e', '%e', '%e', '%e')",
        user_id, username, log_category, action, details);

    mysql_tquery(dbHandle, query);
    return 1;
}

LogCoins(user_id, const username[], const reason[], amount, balance)
{
    if (!amount) return 0;

    new query[512];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `coins_logs` (`user_id`, `username`, `reason`, `amount`, `balance`) VALUES (%d, '%e', '%e', %d, %d)",
        user_id, username, reason, amount, balance);

    mysql_tquery(dbHandle, query);

    LogDiscordCoins(user_id, username, reason, amount, balance);
    return 1;
}

LogChatMessage(user_id, const username[], const channel[], const message[])
{
    new query[512];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `chat_logs` (`user_id`, `username`, `channel`, `message`) VALUES (%d, '%e', '%e', '%e')",
        user_id, username, channel, message);

    mysql_tquery(dbHandle, query);
    return 1;
}

forward OnIngameTicketInserted();
public OnIngameTicketInserted()
{
    TicketSkipAdd(cache_insert_id());
    return 1;
}

LogTicket(user_id, const accused_name[], const reason[])
{
    new query[1024];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `tickets` (`user_id`, `subject`, `description`, `accused_name`, `ticket_type`, `priority`) VALUES (%d, 'Player Report', '%e', '%e', 'report', 'medium')",
        user_id, reason, accused_name);

    mysql_tquery(dbHandle, query, "OnIngameTicketInserted");
    return 1;
}

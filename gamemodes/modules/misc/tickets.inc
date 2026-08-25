#if defined _tickets_watch_included
    #endinput
#endif
#define _tickets_watch_included

#define TICKET_POLL_INTERVAL   (15000)
#define TICKET_BATCH           (5)
#define TICKET_SKIP_SLOTS      (16)

new g_LastTicketID = -1;
new g_TicketSkip[TICKET_SKIP_SLOTS];
new g_TicketSkipCount;

stock TicketSkipAdd(id)
{
    if(id <= 0) return 0;

    if(g_TicketSkipCount >= TICKET_SKIP_SLOTS)
    {
        for(new i = 0; i < TICKET_SKIP_SLOTS - 1; i++)
            g_TicketSkip[i] = g_TicketSkip[i + 1];

        g_TicketSkipCount = TICKET_SKIP_SLOTS - 1;
    }

    g_TicketSkip[g_TicketSkipCount++] = id;
    return 1;
}

stock bool:TicketSkipTake(id)
{
    for(new i = 0; i < g_TicketSkipCount; i++)
    {
        if(g_TicketSkip[i] != id) continue;

        for(new j = i; j < g_TicketSkipCount - 1; j++)
            g_TicketSkip[j] = g_TicketSkip[j + 1];

        g_TicketSkipCount--;
        return true;
    }
    return false;
}

forward OnTicketBaseline();
public OnTicketBaseline()
{
    g_LastTicketID = 0;

    if(cache_num_rows())
        cache_get_value_int(0, "last_id", g_LastTicketID);

    return 1;
}

forward OnSiteTicketsFound();
public OnSiteTicketsFound()
{
    new rows = cache_num_rows();
    if(!rows) return 1;

    for(new i = 0; i < rows; i++)
    {
        new id, priority[8], ttype[51], subject[256], author[MAX_PLAYER_NAME + 1], accused[101];

        cache_get_value_int(i, "id", id);
        if(id > g_LastTicketID) g_LastTicketID = id;

        if(TicketSkipTake(id)) continue;

        cache_get_value_name(i, "priority", priority, sizeof(priority));
        cache_get_value_name(i, "ticket_type", ttype, sizeof(ttype));
        cache_get_value_name(i, "subject", subject, sizeof(subject));
        cache_get_value_name(i, "author", author, sizeof(author));
        cache_get_value_name(i, "accused_name", accused, sizeof(accused));

        if(isnull(author)) strcopy(author, "unknown", sizeof(author));

        foreach(new p : Player)
        {
            if(User[p][adminLevel] < 1 || !User[p][isLogged]) continue;

            va_SendClientMessage(p, -1, "{MAIN}[TICKET] {FFFFFF}New %s ticket {MAIN}#%d {FFFFFF}from {MAIN}%s{FFFFFF} (%s): %s",
                ttype, id, author, priority, subject);
            PlaySuccessSound(p);
        }

        if(logs_tickets != DCC_INVALID_CHANNEL)
        {
            new body[1024], encoded[1024], title[64];

            format(title, sizeof(title), "New Ticket #%d", id);
            format(body, sizeof(body),
"**Author:** %s\n**Type:** %s\n**Priority:** %s\n**Subject:** %s%s",
                author, ttype, priority, subject,
                isnull(accused) ? ("") : (fmat_return("\n**Accused:** %s", accused)));

            utf8encode(encoded, body, sizeof(encoded));
            SendDiscordEmbed(logs_tickets, title, encoded, DISCORD_COLOR_WARNING);
        }
    }
    return 1;
}

forward PollSiteTickets();
public PollSiteTickets()
{
    if(dbHandle == MYSQL_INVALID_HANDLE || g_LastTicketID < 0) return 1;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
        "SELECT t.`id`, t.`subject`, t.`priority`, t.`ticket_type`, t.`accused_name`, a.`username` AS `author` \
FROM `tickets` AS t LEFT JOIN `accounts` AS a ON a.`id` = t.`user_id` \
WHERE t.`id` > %d ORDER BY t.`id` ASC LIMIT %d", g_LastTicketID, TICKET_BATCH);

    mysql_tquery(dbHandle, queryGlobal, "OnSiteTicketsFound");
    return 1;
}

stock StartTicketWatcher()
{
    if(dbHandle == MYSQL_INVALID_HANDLE) return 0;

    mysql_tquery(dbHandle, "SELECT IFNULL(MAX(`id`), 0) AS `last_id` FROM `tickets`", "OnTicketBaseline");
    SetTimer("PollSiteTickets", TICKET_POLL_INTERVAL, true);
    return 1;
}

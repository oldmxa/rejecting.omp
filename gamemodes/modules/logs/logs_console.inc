#if defined _logs_console_included
    #endinput
#endif
#define _logs_console_included

#define CH_LOGS_CONSOLE     "1535528894726414387"
#define CONSOLE_QUEUE_SIZE      (96)
#define CONSOLE_LINE_LEN        (256)
#define CONSOLE_FLUSH_INTERVAL  (4000)
#define CONSOLE_BATCH_LEN       (1900)
#define CONSOLE_BATCH_RESERVE   (80)

new DCC_Channel:logs_console = DCC_INVALID_CHANNEL;

new g_ConsoleQueue[CONSOLE_QUEUE_SIZE][CONSOLE_LINE_LEN];
new g_ConsoleHead, g_ConsoleCount, g_ConsoleDropped;
new bool:g_ConsoleFlushing = false;

#define CONSOLE_ECHO_SIZE       (32)

new g_ConsoleEcho[CONSOLE_ECHO_SIZE][CONSOLE_LINE_LEN];
new g_ConsoleEchoHead;

stock bool:ConsoleEchoMatch(const line[])
{
    for(new i = 0; i < CONSOLE_ECHO_SIZE; i++)
    {
        if(g_ConsoleEcho[i][0] == EOS) continue;
        if(strcmp(g_ConsoleEcho[i], line)) continue;

        g_ConsoleEcho[i][0] = EOS;
        return true;
    }
    return false;
}

stock ConsoleQueuePush(const line[])
{
    if(g_ConsoleCount >= CONSOLE_QUEUE_SIZE)
    {
        g_ConsoleHead = (g_ConsoleHead + 1) % CONSOLE_QUEUE_SIZE;
        g_ConsoleCount--;
        g_ConsoleDropped++;
    }

    new slot = (g_ConsoleHead + g_ConsoleCount) % CONSOLE_QUEUE_SIZE;
    strcopy(g_ConsoleQueue[slot], line, CONSOLE_LINE_LEN);
    g_ConsoleCount++;

    return 1;
}

stock ConsolePrintLine(const line[])
{
    if(logs_console != DCC_INVALID_CHANNEL)
        print(line);

    return ConsoleLog(line);
}

stock ConsoleLog(const line[])
{
    if(logs_console == DCC_INVALID_CHANNEL)
        print(line);

    strcopy(g_ConsoleEcho[g_ConsoleEchoHead], line, CONSOLE_LINE_LEN);
    g_ConsoleEchoHead = (g_ConsoleEchoHead + 1) % CONSOLE_ECHO_SIZE;

    return ConsoleQueuePush(line);
}

forward FlushConsoleLog();
public FlushConsoleLog()
{
    if(g_ConsoleFlushing || !g_ConsoleCount) return 1;

    if(logs_console == DCC_INVALID_CHANNEL)
    {
        logs_console = DCC_FindChannelById(CH_LOGS_CONSOLE);
        if(logs_console == DCC_INVALID_CHANNEL) return 1;
    }

    g_ConsoleFlushing = true;

    new batch[CONSOLE_BATCH_LEN + 32];
    strcat(batch, "```\n", sizeof(batch));

    while(g_ConsoleCount)
    {
        new slot = g_ConsoleHead;
        if(strlen(batch) + strlen(g_ConsoleQueue[slot]) + CONSOLE_BATCH_RESERVE >= CONSOLE_BATCH_LEN) break;

        strcat(batch, g_ConsoleQueue[slot], sizeof(batch));
        strcat(batch, "\n", sizeof(batch));

        g_ConsoleHead = (g_ConsoleHead + 1) % CONSOLE_QUEUE_SIZE;
        g_ConsoleCount--;
    }

    if(g_ConsoleDropped)
    {
        new note[64];
        format(note, sizeof(note), "... %d line(s) dropped (queue full)\n", g_ConsoleDropped);
        strcat(batch, note, sizeof(batch));
        g_ConsoleDropped = 0;
    }

    strcat(batch, "```", sizeof(batch));
    DCC_SendChannelMessage(logs_console, batch);

    g_ConsoleFlushing = false;
    return 1;
}

stock StartConsoleLog()
{
    logs_console = DCC_FindChannelById(CH_LOGS_CONSOLE);
    SetTimer("FlushConsoleLog", CONSOLE_FLUSH_INTERVAL, true);
    return 1;
}

stock CB_print(const message[])
{
    return ConsoleLog(message);
}

stock CB_printf(const fmat[], va_args<>)
{
    new buffer[CONSOLE_LINE_LEN];
    va_format(buffer, sizeof(buffer), fmat, va_start<1>);

    return ConsoleLog(buffer);
}

#undef print
#undef printf

#define print CB_print
#define printf CB_printf

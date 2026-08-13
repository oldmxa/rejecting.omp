#if defined _logs_tail_included
    #endinput
#endif
#define _logs_tail_included

#define CONSOLE_TAIL_FILE       "log.txt"
#define CONSOLE_TAIL_INTERVAL   (3000)
#define CONSOLE_TAIL_MAXLINES   (12)

new g_TailOffset = -1;

static ConsoleTailStrip(const line[], dest[], len = sizeof(dest))
{
    new p = 0;

    for(new block = 0; block < 2; block++)
    {
        if(line[p] != '[') break;

        while(line[p] != EOS && line[p] != ']') p++;
        if(line[p] == ']') p++;
        while(line[p] == ' ') p++;
    }

    new end = strlen(line);
    while(end > p && (line[end - 1] == '\n' || line[end - 1] == '\r')) end--;

    strmid(dest, line, p, end, len);
    return 1;
}

forward TailConsoleLog();
public TailConsoleLog()
{
    if(g_TailOffset < 0) return 1;

    new File:handle = fopen(CONSOLE_TAIL_FILE, io_read);
    if(!handle) return 1;

    new size = flength(handle);

    if(size < g_TailOffset) g_TailOffset = 0;

    if(size == g_TailOffset)
    {
        fclose(handle);
        return 1;
    }

    fseek(handle, g_TailOffset, seek_start);

    new raw[CONSOLE_LINE_LEN], clean[CONSOLE_LINE_LEN], read, sent;

    while(sent < CONSOLE_TAIL_MAXLINES && (read = fread(handle, raw, sizeof(raw))) > 0)
    {
        g_TailOffset += read;

        ConsoleTailStrip(raw, clean);

        if(clean[0] == EOS) continue;
        if(ConsoleEchoMatch(clean)) continue;

        ConsoleQueuePush(clean);
        sent++;
    }

    fclose(handle);
    return 1;
}

stock StartConsoleTail()
{
    new File:handle = fopen(CONSOLE_TAIL_FILE, io_read);

    if(!handle)
    {
        g_TailOffset = -1;
        return 0;
    }

    g_TailOffset = flength(handle);
    fclose(handle);

    SetTimer("TailConsoleLog", CONSOLE_TAIL_INTERVAL, true);
    return 1;
}

#if defined _logs_discord_included
    #endinput
#endif
#define _logs_discord_included

#define CH_LOGS_BAN         "1461088530431348989"
#define CH_LOGS_UNBAN       "1461088687008776388"
#define CH_LOGS_KICK        "1461088734962515968"
#define CH_LOGS_REPORT      "1461088778754982080"
#define CH_LOGS_ASK         "1531481951331024936"
#define CH_LOGS_PROMOTIONS  "1461088814465552405"
#define CH_LOGS_ARENA       "1466540934689853480"
#define CH_LOGS_ADMIN_CMDS  "1477802174938615990"
#define CH_LOGS_TICKETS     "1535689360286949488"
#define CH_LOGS_AC          "1536842134156615711"
#define CH_LOGS_CONNECTIONS "1538769355687596062"
#define CH_LOGS_NEXTMAP     "1538771407310159892"
#define CH_LOGS_VIPS        "1538771961734234192"
#define CH_COMANDOS_ADMIN   "1460838135045951796"
#define CH_COMANDOS_PLAYERS "1460111666724212788"
#define CH_COMANDOS         "1460838135045951796"
#define CH_GAMECHAT         "1461088854185480296"
#define CH_ADMINCHAT        "1461088997668290581"
#define CH_LOGS_PM          "1539748119112187964"
#define CH_LOGS_CLAN_CHAT   "1539748245096366171"
#define CH_LOGS_LINKED      "1540763697721647264"
#define CH_TOURNEY_ANNOUNCE     "1539673557267644496"
#define CH_TOURNEY_REGISTER     "1460111666724212788"
#define CH_TOURNEY_ADMIN_LOGS   "1539674148752724108"
#define CH_TOURNEY_APPLICATIONS "1539674434305007627"
#define CH_LOGS_PASSWORD        "1539760216915054733"
#define CH_WEEK_RESULTS         "1540751644923527311"
#define CH_LOGS_PUNISH          "1540756249841500160"
#define CH_LOGS_TRAINING        "1540756510630477874"
#define CH_LOGS_NICK            "1540756679623319593"
#define CH_LOGS_MONEY           "1540756774229901413"
#define CH_LOGS_DUEL            "1540757017910710312"

new
    DCC_Guild:guildaid,
    DCC_Channel:logs_ban,
    DCC_Channel:logs_unban,
    DCC_Channel:logs_kick,
    DCC_Channel:logs_report,
    DCC_Channel:logs_ask,
    DCC_Channel:logs_promotions,
    DCC_Channel:logs_arena,
    DCC_Channel:comandos,
    DCC_Channel:ch_comandos_admin,
    DCC_Channel:ch_comandos_players,
    DCC_Channel:gamechat,
    DCC_Channel:adminchat,
    DCC_Channel:logs_admin_cmds,
    DCC_Channel:logs_tickets,
    DCC_Channel:logs_ac,
    DCC_Channel:logs_connections,
    DCC_Channel:logs_nextmap,
    DCC_Channel:logs_vips,
    DCC_Channel:logs_pm,
    DCC_Channel:logs_clan_chat,
    DCC_Channel:logs_linked,
    DCC_Channel:PMPlayer,
    DCC_Channel:ch_tourney_announce,
    DCC_Channel:ch_tourney_register,
    DCC_Channel:ch_tourney_admin_logs,
    DCC_Channel:ch_tourney_applications,
    DCC_Channel:logs_password,
    DCC_Channel:ch_week_results,
    DCC_Channel:logs_punish,
    DCC_Channel:logs_training,
    DCC_Channel:logs_nick,
    DCC_Channel:logs_money,
    DCC_Channel:logs_duel;

stock InitLogChannels()
{
    guildaid        = DCC_FindGuildById(DISCORD_GUILD_ID);

    logs_ban        = DCC_FindChannelById(CH_LOGS_BAN);
    logs_unban      = DCC_FindChannelById(CH_LOGS_UNBAN);
    logs_kick       = DCC_FindChannelById(CH_LOGS_KICK);
    logs_report     = DCC_FindChannelById(CH_LOGS_REPORT);
    logs_ask        = DCC_FindChannelById(CH_LOGS_ASK);
    logs_promotions = DCC_FindChannelById(CH_LOGS_PROMOTIONS);
    logs_arena      = DCC_FindChannelById(CH_LOGS_ARENA);
    logs_admin_cmds = DCC_FindChannelById(CH_LOGS_ADMIN_CMDS);
    logs_tickets    = DCC_FindChannelById(CH_LOGS_TICKETS);
    logs_ac         = DCC_FindChannelById(CH_LOGS_AC);
    logs_connections= DCC_FindChannelById(CH_LOGS_CONNECTIONS);
    logs_nextmap    = DCC_FindChannelById(CH_LOGS_NEXTMAP);
    logs_vips       = DCC_FindChannelById(CH_LOGS_VIPS);
    logs_pm         = DCC_FindChannelById(CH_LOGS_PM);
    logs_clan_chat  = DCC_FindChannelById(CH_LOGS_CLAN_CHAT);
    logs_linked     = DCC_FindChannelById(CH_LOGS_LINKED);
    ch_comandos_admin   = DCC_FindChannelById(CH_COMANDOS_ADMIN);
    ch_comandos_players = DCC_FindChannelById(CH_COMANDOS_PLAYERS);
    comandos        = ch_comandos_admin;
    logs_password   = DCC_FindChannelById(CH_LOGS_PASSWORD);
    gamechat        = DCC_FindChannelById(CH_GAMECHAT);
    adminchat       = DCC_FindChannelById(CH_ADMINCHAT);
    ch_tourney_announce     = DCC_FindChannelById(CH_TOURNEY_ANNOUNCE);
    ch_tourney_register     = DCC_FindChannelById(CH_TOURNEY_REGISTER);
    ch_tourney_admin_logs   = DCC_FindChannelById(CH_TOURNEY_ADMIN_LOGS);
    ch_tourney_applications = DCC_FindChannelById(CH_TOURNEY_APPLICATIONS);
    ch_week_results         = DCC_FindChannelById(CH_WEEK_RESULTS);
    logs_punish             = DCC_FindChannelById(CH_LOGS_PUNISH);
    logs_training           = DCC_FindChannelById(CH_LOGS_TRAINING);
    logs_nick               = DCC_FindChannelById(CH_LOGS_NICK);
    logs_money              = DCC_FindChannelById(CH_LOGS_MONEY);
    logs_duel               = DCC_FindChannelById(CH_LOGS_DUEL);

    StartConsoleLog();
    StartConsoleTail();
    return 1;
}

stock DCC_Embed:BuildDiscordEmbed(const title[], const description[], color = DISCORD_COLOR_DEFAULT)
{
    new DCC_Embed:d_embed = DCC_CreateEmbed(.title = title, .description = description, .color = color);

    DCC_SetEmbedFooter(d_embed, getDateStrTD());

    return d_embed;
}

stock SendDiscordEmbed(DCC_Channel:channel, const title[], const description[], color = DISCORD_COLOR_DEFAULT)
{
    if(channel == DCC_INVALID_CHANNEL) return 0;

    return DCC_SendChannelEmbedMessage(channel, BuildDiscordEmbed(title, description, color));
}

stock LogDiscordConnect(playerid)
{
    if(IsPlayerNPC(playerid)) return 0;

    new ip[18], serial[41], logStr[256];
    GetPlayerIp(playerid, ip, sizeof(ip));
    gpci(playerid, serial, sizeof(serial));

    if(serial[0] == EOS)
        format(serial, sizeof(serial), "N/A");

    format(logStr, sizeof(logStr),
        "**Player:** %s [ID: %d]\n**IP:** %s\n**Serial (GPCI):** %s\n**Version:** %s",
        User[playerid][userName],
        playerid,
        ip,
        serial,
        User[playerid][playerVersion]
    );

    SendDiscordEmbed(logs_connections, "Player Connected", logStr, DISCORD_COLOR_SUCCESS);
    return 1;
}

stock LogDiscordDisconnect(playerid, reason)
{
    if(IsPlayerNPC(playerid)) return 0;

    new reasonStr[32];
    switch(reason)
    {
        case 0: reasonStr = "Timeout / Crash";
        case 1: reasonStr = "Quit";
        case 2: reasonStr = "Kicked / Banned";
        default: reasonStr = "Unknown";
    }

    new ip[18], logStr[256];
    GetPlayerIp(playerid, ip, sizeof(ip));
    if(ip[0] == EOS)
        format(ip, sizeof(ip), "%s", User[playerid][user_lastip]);

    format(logStr, sizeof(logStr),
        "**Player:** %s [ID: %d]\n**Reason:** %s\n**IP:** %s\n**Ping:** %d ms",
        User[playerid][userName],
        playerid,
        reasonStr,
        ip,
        GetPlayerPing(playerid)
    );

    new color = (reason == 1) ? DISCORD_COLOR_INFO : DISCORD_COLOR_ERROR;

    SendDiscordEmbed(logs_connections, "Player Disconnected", logStr, color);
    return 1;
}

stock LogDiscordCoins(user_id, const username[], const reason[], amount, balance)
{
    new desc[512];
    format(desc, sizeof(desc),
        "**Player:** %s [SQL ID: %d]\n**Action / Reason:** %s\n**Amount:** %s%d Coins\n**New Balance:** %d Coins",
        username,
        user_id,
        reason,
        (amount > 0) ? ("+") : (""),
        amount,
        balance
    );

    new color = (amount > 0) ? DISCORD_COLOR_SUCCESS : DISCORD_COLOR_WARNING;
    SendDiscordEmbed(logs_vips, "Coins Transaction", desc, color);
    return 1;
}

stock LogDiscordVipAction(const title[], const description[], color = DISCORD_COLOR_INFO)
{
    return SendDiscordEmbed(logs_vips, title, description, color);
}

stock LogDiscordPM(senderid, targetid, const message[])
{
    if(!IsPlayerConnected(senderid) || !IsPlayerConnected(targetid)) return 0;

    if(logs_pm == DCC_INVALID_CHANNEL)
    {
        logs_pm = DCC_FindChannelById(CH_LOGS_PM);
    }
    if(logs_pm == DCC_INVALID_CHANNEL) return 0;

    new sName[MAX_PLAYER_NAME + 1], tName[MAX_PLAYER_NAME + 1];
    GetPlayerName(senderid, sName, sizeof(sName));
    GetPlayerName(targetid, tName, sizeof(tName));

    new cleanMsg[160];
    format(cleanMsg, sizeof(cleanMsg), "%s", message);
    strreplace(cleanMsg, "%", "");

    new logStr[512], encodedMsg[512];
    format(logStr, sizeof(logStr),
        "**[PM]** `%s` (ID: %d) -> `%s` (ID: %d): %s",
        sName, senderid,
        tName, targetid,
        cleanMsg
    );

    utf8encode(encodedMsg, logStr, sizeof(encodedMsg));
    DCC_SendChannelMessage(logs_pm, encodedMsg);
    return 1;
}

stock LogDiscordClanChat(playerid, const clanTag[], const clanName[], const roleName[], const message[])
{
    #pragma unused clanName
    if(!IsPlayerConnected(playerid)) return 0;

    if(logs_clan_chat == DCC_INVALID_CHANNEL)
    {
        logs_clan_chat = DCC_FindChannelById(CH_LOGS_CLAN_CHAT);
    }
    if(logs_clan_chat == DCC_INVALID_CHANNEL) return 0;

    new pName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, pName, sizeof(pName));

    new cleanMsg[160];
    format(cleanMsg, sizeof(cleanMsg), "%s", message);
    strreplace(cleanMsg, "%", "");

    new logStr[512], encodedMsg[512];
    format(logStr, sizeof(logStr),
        "**[CLAN %s]** `%s` (%s | ID: %d): %s",
        clanTag,
        pName,
        roleName,
        playerid,
        cleanMsg
    );

    utf8encode(encodedMsg, logStr, sizeof(encodedMsg));
    DCC_SendChannelMessage(logs_clan_chat, encodedMsg);
    return 1;
}

stock LogDiscordVerification(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;

    if(logs_linked == DCC_INVALID_CHANNEL)
        logs_linked = DCC_FindChannelById(CH_LOGS_LINKED);

    if(logs_linked == DCC_INVALID_CHANNEL) return 0;

    new rolesStr[256];
    format(rolesStr, sizeof(rolesStr), "<@&" DISCORD_ROLE_LINKED ">");

    if(IsPlayerVip(playerid))
    {
        format(rolesStr, sizeof(rolesStr), "%s, <@&" DISCORD_ROLE_VIP ">", rolesStr);
    }

    if(User[playerid][adminLevel] >= 1 && User[playerid][adminLevel] <= MAX_ADMIN_LEVEL)
    {
        format(rolesStr, sizeof(rolesStr), "%s, <@&%s>", rolesStr, g_DiscordAdminRoles[User[playerid][adminLevel] - 1]);
    }

    new vLog[512];
    format(vLog, sizeof(vLog),
        "**Player:** %s [ID: %d | SQL ID: %d]\n**Discord:** <@%s> (`%s`)\n**Assigned Roles:** %s",
        User[playerid][userName], playerid, User[playerid][user_sqlid],
        User[playerid][dc_uid], User[playerid][dc_username],
        rolesStr
    );

    SendDiscordEmbed(logs_linked, "Account Verified", vLog, DISCORD_COLOR_SUCCESS);
    return 1;
}




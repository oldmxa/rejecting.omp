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
#define CH_COMANDOS         "1460838135045951796"
#define CH_GAMECHAT         "1461088854185480296"
#define CH_ADMINCHAT        "1461088997668290581"

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
    DCC_Channel:gamechat,
    DCC_Channel:adminchat,
    DCC_Channel:logs_admin_cmds,
    DCC_Channel:PMPlayer;

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
    comandos        = DCC_FindChannelById(CH_COMANDOS);
    gamechat        = DCC_FindChannelById(CH_GAMECHAT);
    adminchat       = DCC_FindChannelById(CH_ADMINCHAT);

    StartConsoleLog();
    return 1;
}

stock DCC_Embed:BuildDiscordEmbed(const title[], const description[], color = DISCORD_COLOR_DEFAULT)
{
    new DCC_Embed:d_embed = DCC_CreateEmbed(.title = title, .description = description, .color = color);

    new footerStr[64];
    format(footerStr, sizeof(footerStr), "%s • %s", ServerHostname, getDateStrTD());
    DCC_SetEmbedFooter(d_embed, footerStr);

    return d_embed;
}

stock SendDiscordEmbed(DCC_Channel:channel, const title[], const description[], color = DISCORD_COLOR_DEFAULT)
{
    if(channel == DCC_INVALID_CHANNEL) return 0;

    return DCC_SendChannelEmbedMessage(channel, BuildDiscordEmbed(title, description, color));
}

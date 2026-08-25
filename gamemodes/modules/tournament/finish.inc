#if defined _tourney_finish_included
    #endinput
#endif
#define _tourney_finish_included

stock DeclareTournamentChampion(winnerSlot)
{
    if(winnerSlot < 0 || winnerSlot >= MAX_TOURNEY_SLOTS)
    {
        SendTourneyBroadcast("Tournament concluded with no clear winner.");
        ResetTournament();
        return 0;
    }

    g_TourneyData[t_State] = TOURNAMENT_STATE_FINISHED;
    g_TourneyData[t_WinnerSlot] = winnerSlot;

    new winnerDisplayName[64];
    GetParticipantDisplayName(winnerSlot, winnerDisplayName, sizeof(winnerDisplayName));
    strcopy(g_TourneyData[t_WinnerName], winnerDisplayName, MAX_PLAYER_NAME + 1);

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT CHAMPION] {FFFFFF}Congratulations to {MAIN}%s{FFFFFF}, official champion of {MAIN}%s{FFFFFF}!", winnerDisplayName, g_TourneyData[t_Name]);
    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {B4B5B7}Achievement Awarded: {FF4500}%s {B4B5B7}| Congratulations to the winner!", g_TourneyData[t_AchievementTag]);

    GiveTournamentChampionRewards(winnerSlot);

    new championEmbed[1024];
    format(championEmbed, sizeof(championEmbed),
        "**Tournament Winner**\n"\
        ":crown: **Champion:** `%s`\n\n"\
        "**Event Summary**\n"\
        "- **Tournament:** `%s`\n"\
        "- **Bracket Size:** `%d Participants`\n"\
        "- **Prizes Awarded:** `%d Coins` + `%s`\n"\
        "- **Achievement Tag:** `%s`",
        winnerDisplayName,
        g_TourneyData[t_Name],
        g_TourneyData[t_MaxSlots],
        g_TourneyData[t_PrizeCoins],
        FormatMoney(g_TourneyData[t_PrizeMoney]),
        g_TourneyData[t_AchievementTag]
    );
    SendTourneyDiscordLog(":trophy: Tournament Champion Crowned", championEmbed, DISCORD_COLOR_SUCCESS);

    if(ch_tourney_announce != DCC_INVALID_CHANNEL)
    {
        utf8encode(msgencoded, championEmbed, sizeof(msgencoded));
        SendDiscordEmbed(ch_tourney_announce, ":trophy: Tournament Champion Crowned", msgencoded, DISCORD_COLOR_SUCCESS);
    }

    SetTimer("OnTourneyFinishedDelay", 15000, false);
    return 1;
}

public OnTourneyFinishedDelay()
{
    ResetTournament();
}

stock GiveTournamentChampionRewards(winnerSlot)
{
    if(winnerSlot < 0 || winnerSlot >= MAX_TOURNEY_SLOTS) return 0;

    new p1 = g_TourneyParticipants[winnerSlot][tp_Player1];
    new p2 = g_TourneyParticipants[winnerSlot][tp_Player2];

    new prizeCoins = g_TourneyData[t_PrizeCoins];
    new prizeMoney = g_TourneyData[t_PrizeMoney];

    // Award Player 1
    if(IsPlayerLogged(p1))
    {
        if(prizeCoins > 0)
        {
            User[p1][playerCoins] += prizeCoins;
            LogCoins(User[p1][user_sqlid], User[p1][userName], "Tournament Champion Prize", prizeCoins, User[p1][playerCoins]);
        }
        if(prizeMoney > 0)
        {
            GivePlayerMoney(p1, prizeMoney);
            User[p1][playerMoney] += prizeMoney;
        }

        User[p1][customTagPerm] = true;
        SaveWonTournamentTag(User[p1][user_sqlid], g_TourneyData[t_AchievementTag], g_TourneyData[t_Name]);
        ApplyTournamentTag(p1, g_TourneyData[t_AchievementTag]);
        Ach_OnTournamentWin(p1);
        UpdatePlayerData(p1);

        va_SendClientMessage(p1, -1, "{MAIN}[REWARD] {FFFFFF}You received {FFD700}%d Coins{FFFFFF}, {23C552}%s{FFFFFF} and the {FF4500}%s{FFFFFF} Achievement & Tag!",
            prizeCoins, FormatMoney(prizeMoney), g_TourneyData[t_AchievementTag]);
        PlayerPlaySound(p1, 1057, 0.0, 0.0, 0.0);
    }

    if(p2 != INVALID_PLAYER_ID && IsPlayerLogged(p2))
    {
        if(prizeCoins > 0)
        {
            User[p2][playerCoins] += prizeCoins;
            LogCoins(User[p2][user_sqlid], User[p2][userName], "Tournament Champion Prize", prizeCoins, User[p2][playerCoins]);
        }
        if(prizeMoney > 0)
        {
            GivePlayerMoney(p2, prizeMoney);
            User[p2][playerMoney] += prizeMoney;
        }

        User[p2][customTagPerm] = true;
        SaveWonTournamentTag(User[p2][user_sqlid], g_TourneyData[t_AchievementTag], g_TourneyData[t_Name]);
        ApplyTournamentTag(p2, g_TourneyData[t_AchievementTag]);
        Ach_OnTournamentWin(p2);
        UpdatePlayerData(p2);

        va_SendClientMessage(p2, -1, "{MAIN}[REWARD] {FFFFFF}You received {FFD700}%d Coins{FFFFFF}, {23C552}%s{FFFFFF} and the {FF4500}%s{FFFFFF} Achievement & Tag!",
            prizeCoins, FormatMoney(prizeMoney), g_TourneyData[t_AchievementTag]);
        PlayerPlaySound(p2, 1057, 0.0, 0.0, 0.0);
    }

    SaveTournamentResultToDB(winnerSlot);
    return 1;
}

stock SaveTournamentResultToDB(winnerSlot)
{
    new winnerDisplayName[64];
    GetParticipantDisplayName(winnerSlot, winnerDisplayName, sizeof(winnerDisplayName));

    new query[512];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `tournaments` (`tournament_name`, `achievement_tag`, `mode`, `max_slots`, `prize_coins`, `prize_money`, `entry_fee`, `winner_name`, `started_by`, `scheduled_time`, `created_at`, `finished_at`, `status`) \
        VALUES ('%e', '%e', %d, %d, %d, %d, %d, '%e', '%e', '%e', %d, %d, 'finished')",
        g_TourneyData[t_Name],
        g_TourneyData[t_AchievementTag],
        g_TourneyData[t_Mode],
        g_TourneyData[t_MaxSlots],
        g_TourneyData[t_PrizeCoins],
        g_TourneyData[t_PrizeMoney],
        g_TourneyData[t_EntryFee],
        winnerDisplayName,
        g_TourneyData[t_StartedBy],
        g_TourneyData[t_Schedule],
        g_TourneyData[t_StartTime],
        gettime()
    );
    mysql_tquery(dbHandle, query);
}

stock CancelTournament(adminid = INVALID_PLAYER_ID, const reason[] = "Cancelled by Administrator")
{
    if(!g_TourneyData[t_Active] && g_TourneyData[t_State] == TOURNAMENT_STATE_NONE) return 0;

    if(g_TourneyData[t_EntryFee] > 0)
    {
        for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
        {
            if(g_TourneyParticipants[i][tp_Registered])
            {
                new p1 = g_TourneyParticipants[i][tp_Player1];
                new p2 = g_TourneyParticipants[i][tp_Player2];

                if(IsPlayerLogged(p1))
                {
                    GivePlayerMoney(p1, g_TourneyData[t_EntryFee]);
                    User[p1][playerMoney] += g_TourneyData[t_EntryFee];
                    SendClientMessage(p1, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your registration fee was refunded.");
                }
                if(p2 != INVALID_PLAYER_ID && IsPlayerLogged(p2))
                {
                    GivePlayerMoney(p2, g_TourneyData[t_EntryFee]);
                    User[p2][playerMoney] += g_TourneyData[t_EntryFee];
                    SendClientMessage(p2, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your registration fee was refunded.");
                }
            }
        }
    }

    new adminName[MAX_PLAYER_NAME + 1];
    if(adminid != INVALID_PLAYER_ID && IsPlayerConnected(adminid)) strcopy(adminName, User[adminid][userName], sizeof(adminName));
    else strcopy(adminName, "System");

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}The tournament was cancelled by %s. Reason: %s", adminName, reason);

    new discordLog[512];
    format(discordLog, sizeof(discordLog),
        "**Cancellation Details**\n"\
        "- **Tournament:** `%s`\n"\
        "- **Cancelled by:** `%s`\n"\
        "- **Reason:** `%s`\n\n"\
        "*All participant entry fees were fully refunded.*",
        g_TourneyData[t_Name], adminName, reason
    );
    SendTourneyDiscordLog(":x: Tournament Cancelled", discordLog, DISCORD_COLOR_ERROR);

    if(ch_tourney_announce != DCC_INVALID_CHANNEL)
    {
        utf8encode(msgencoded, discordLog, sizeof(msgencoded));
        SendDiscordEmbed(ch_tourney_announce, ":x: Tournament Cancelled", msgencoded, DISCORD_COLOR_ERROR);
    }

    ResetTournament();
    return 1;
}

stock SpectateTourneyMatch(playerid, matchid)
{
    if(!IsPlayerLogged(playerid)) return 0;

    if(IsPlayerInActiveTourneyMatch(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot spectate while playing your own match!");
        return 0;
    }

    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES || !g_TourneyMatches[matchid][tm_Active])
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Invalid or inactive tournament match!");
        return 0;
    }

    new slotA = g_TourneyMatches[matchid][tm_SlotA];
    new targetid = INVALID_PLAYER_ID;
    if(slotA != -1 && g_TourneyParticipants[slotA][tp_Registered])
    {
        targetid = g_TourneyParticipants[slotA][tp_Player1];
        if(!IsPlayerLogged(targetid))
            targetid = g_TourneyParticipants[slotA][tp_Player2];
    }

    if(targetid == INVALID_PLAYER_ID || !IsPlayerLogged(targetid))
    {
        new slotB = g_TourneyMatches[matchid][tm_SlotB];
        if(slotB != -1 && g_TourneyParticipants[slotB][tp_Registered])
        {
            targetid = g_TourneyParticipants[slotB][tp_Player1];
            if(!IsPlayerLogged(targetid))
                targetid = g_TourneyParticipants[slotB][tp_Player2];
        }
    }

    if(targetid == INVALID_PLAYER_ID || !IsPlayerLogged(targetid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}No connected players in this match to spectate.");
        return 0;
    }

    g_PlayerTourneySpectating[playerid] = matchid;
    setPlayerSpectate(playerid, targetid);

    va_SendClientMessage(playerid, -1, "{MAIN}[SPECTATE] {FFFFFF}Now spectating Tournament Match #%d. Type {MAIN}/specoff {FFFFFF}to exit.", matchid + 1);
    return 1;
}

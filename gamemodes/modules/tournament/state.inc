#if defined _tourney_state_included
    #endinput
#endif
#define _tourney_state_included

stock bool:IsPlayerInTournament(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    if(!IsPlayerLogged(playerid)) return false;
    return (g_PlayerTourneySlot[playerid] != -1);
}

stock bool:IsPlayerInActiveTourneyMatch(playerid)
{
    if(!IsPlayerInTournament(playerid)) return false;
    new matchid = g_PlayerTourneyMatch[playerid];
    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES) return false;
    return (g_TourneyMatches[matchid][tm_Active] && g_TourneyMatches[matchid][tm_State] == MATCH_STATE_ACTIVE);
}

stock bool:IsPlayerRegisteredOnDiscord(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    if(!IsPlayerLogged(playerid)) return false;

    new pName[MAX_PLAYER_NAME + 1];
    strcopy(pName, User[playerid][userName], sizeof(pName));

    for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
    {
        if(g_TourneyDiscordApps[i][tda_Active])
        {
            if(!strcmp(g_TourneyDiscordApps[i][tda_Nick], pName, true))
            {
                return true;
            }
            if(!strequal(User[playerid][dc_uid], "-1") && !strcmp(g_TourneyDiscordApps[i][tda_DiscordID], User[playerid][dc_uid], true))
            {
                return true;
            }
        }
    }
    return false;
}

stock GetParticipantDisplayName(slot, dest[], size = sizeof(dest))
{
    if(slot < 0 || slot >= MAX_TOURNEY_SLOTS || !g_TourneyParticipants[slot][tp_Registered])
    {
        strcopy(dest, "BYE / Empty", size);
        return 1;
    }

    switch(g_TourneyData[t_Mode])
    {
        case TOURNAMENT_MODE_1V1:
        {
            format(dest, size, "%s", g_TourneyParticipants[slot][tp_Player1_Name]);
        }
        case TOURNAMENT_MODE_2V2:
        {
            format(dest, size, "%s & %s", g_TourneyParticipants[slot][tp_Player1_Name], g_TourneyParticipants[slot][tp_Player2_Name]);
        }
        case TOURNAMENT_MODE_CLAN:
        {
            format(dest, size, "[%s] %s", g_TourneyParticipants[slot][tp_ClanTag], g_TourneyParticipants[slot][tp_ClanName]);
        }
    }
    return 1;
}

stock ResetTournament()
{
    for(new i = 0; i < MAX_TOURNEY_MATCHES; i++)
    {
        if(g_TourneyMatches[i][tm_TimerCountdown] != 0)
        {
            KillTimer(g_TourneyMatches[i][tm_TimerCountdown]);
            g_TourneyMatches[i][tm_TimerCountdown] = 0;
        }
        if(g_TourneyMatches[i][tm_TimerMatch] != 0)
        {
            KillTimer(g_TourneyMatches[i][tm_TimerMatch]);
            g_TourneyMatches[i][tm_TimerMatch] = 0;
        }
        if(g_TourneyMatches[i][tm_TimerWO] != 0)
        {
            KillTimer(g_TourneyMatches[i][tm_TimerWO]);
            g_TourneyMatches[i][tm_TimerWO] = 0;
        }
        g_TourneyMatches[i][tm_Active] = false;
        g_TourneyMatches[i][tm_State] = MATCH_STATE_IDLE;
    }

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerLogged(i))
        {
            if(g_PlayerTourneySpectating[i] != -1)
            {
                g_PlayerTourneySpectating[i] = -1;
                StopSpectate(i);
            }
            if(g_PlayerTourneyMatch[i] != -1)
            {
                g_PlayerTourneyMatch[i] = -1;
                ResetPlayerWeapons(i);
                SetPlayerHealth(i, 100.0);
                SetPlayerArmour(i, 100.0);
                setAfk(i);
            }
        }
        g_PlayerTourneySlot[i] = -1;
        g_PlayerTourneyMatch[i] = -1;
        g_PlayerTourneyInvite[i] = -1;
        g_PlayerTourneySpectating[i] = -1;
    }

    for(new i = 0; i < MAX_TOURNEY_SLOTS; i++)
    {
        g_TourneyParticipants[i][tp_Registered] = false;
        g_TourneyParticipants[i][tp_Player1] = INVALID_PLAYER_ID;
        g_TourneyParticipants[i][tp_Player1_SQL] = 0;
        g_TourneyParticipants[i][tp_Player1_Name][0] = '\0';
        g_TourneyParticipants[i][tp_Player2] = INVALID_PLAYER_ID;
        g_TourneyParticipants[i][tp_Player2_SQL] = 0;
        g_TourneyParticipants[i][tp_Player2_Name][0] = '\0';
        g_TourneyParticipants[i][tp_ClanID] = -1;
        g_TourneyParticipants[i][tp_ClanName][0] = '\0';
        g_TourneyParticipants[i][tp_ClanTag][0] = '\0';
        g_TourneyParticipants[i][tp_Eliminated] = false;
        g_TourneyParticipants[i][tp_Wins] = 0;
        g_TourneyParticipants[i][tp_Kills] = 0;
        g_TourneyParticipants[i][tp_Deaths] = 0;
        g_TourneyParticipants[i][tp_Seed] = 0;
    }

    // Reset global
    g_TourneyData[t_Active] = false;
    g_TourneyData[t_State] = TOURNAMENT_STATE_NONE;
    g_TourneyData[t_Mode] = TOURNAMENT_MODE_1V1;
    g_TourneyData[t_MaxSlots] = 8;
    g_TourneyData[t_RegisteredCount] = 0;
    g_TourneyData[t_PrizeCoins] = 100;
    g_TourneyData[t_PrizeMoney] = 500000;
    g_TourneyData[t_EntryFee] = 0;
    g_TourneyData[t_WeaponSet] = TOURNEY_WEAPONS_DEAGLE;
    g_TourneyData[t_ArenaCategory] = TOURNEY_ARENA_CAT_DUEL;
    g_TourneyData[t_ArenaID] = -1; // Random Duel
    g_TourneyData[t_RoundsPerMatch] = 1;
    g_TourneyData[t_RoundTimeLimit] = TOURNEY_DEFAULT_ROUND_TIME;
    g_TourneyData[t_GiveArmour] = true;
    g_TourneyData[t_RequireDiscord] = true;
    g_TourneyData[t_AutoStart] = true;
    format(g_TourneyData[t_Name], sizeof(g_TourneyData[t_Name]), "Rejecting Championship");
    format(g_TourneyData[t_AchievementTag], sizeof(g_TourneyData[t_AchievementTag]), "CHAMPION");
    format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Today at 20:00 (UTC-3)");
    g_TourneyData[t_StartedBy][0] = '\0';
    g_TourneyData[t_StartTime] = 0;
    g_TourneyData[t_CurrentRound] = 0;
    g_TourneyData[t_TotalRounds] = 0;
    g_TourneyData[t_WinnerSlot] = -1;
    g_TourneyData[t_WinnerName][0] = '\0';
    g_TourneyData[t_WinnerClanID] = -1;
    g_TourneyData[t_SQLID] = 0;

    for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
    {
        g_TourneyDiscordApps[i][tda_Active] = false;
        g_TourneyDiscordApps[i][tda_Nick][0] = '\0';
        g_TourneyDiscordApps[i][tda_UCP][0] = '\0';
        g_TourneyDiscordApps[i][tda_DiscordID][0] = '\0';
        g_TourneyDiscordApps[i][tda_DiscordTag][0] = '\0';
        g_TourneyDiscordApps[i][tda_Timestamp] = 0;
    }
    g_TourneyDiscordAppCount = 0;

    return 1;
}

stock InitTournamentDefaultConfig(adminid = INVALID_PLAYER_ID)
{
    ResetTournament();

    g_TourneyData[t_Active] = false;
    g_TourneyData[t_State] = TOURNAMENT_STATE_NONE;
    g_TourneyData[t_Mode] = TOURNAMENT_MODE_1V1;
    g_TourneyData[t_MaxSlots] = 8;
    g_TourneyData[t_RegisteredCount] = 0;
    g_TourneyData[t_PrizeCoins] = 100;
    g_TourneyData[t_PrizeMoney] = 500000;
    g_TourneyData[t_EntryFee] = 0;
    g_TourneyData[t_WeaponSet] = TOURNEY_WEAPONS_DEAGLE;
    g_TourneyData[t_ArenaCategory] = TOURNEY_ARENA_CAT_DUEL;
    g_TourneyData[t_ArenaID] = -1;
    g_TourneyData[t_RoundsPerMatch] = 1;
    g_TourneyData[t_RoundTimeLimit] = TOURNEY_DEFAULT_ROUND_TIME;
    g_TourneyData[t_GiveArmour] = true;
    g_TourneyData[t_RequireDiscord] = true;
    g_TourneyData[t_AutoStart] = true;
    format(g_TourneyData[t_Name], sizeof(g_TourneyData[t_Name]), "Rejecting Championship");
    format(g_TourneyData[t_AchievementTag], sizeof(g_TourneyData[t_AchievementTag]), "CHAMPION");
    format(g_TourneyData[t_Schedule], sizeof(g_TourneyData[t_Schedule]), "Today at 20:00 (UTC-3)");

    if(adminid != INVALID_PLAYER_ID && IsPlayerConnected(adminid))
    {
        strcopy(g_TourneyData[t_StartedBy], User[adminid][userName], MAX_PLAYER_NAME + 1);
    }
    else
    {
        strcopy(g_TourneyData[t_StartedBy], "Console / System", MAX_PLAYER_NAME + 1);
    }
    return 1;
}

stock SendTourneyBroadcast(const message[])
{
    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s", message);
}

stock SendTourneyDiscordLog(const title[], const message[], color)
{
    new string[1024];
    format(string, sizeof(string), "%s", message);
    utf8encode(msgencoded, string, sizeof(msgencoded));
    SendDiscordEmbed(logs_arena, title, msgencoded, color);
}

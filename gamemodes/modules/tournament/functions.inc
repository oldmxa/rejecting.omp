#if defined _tourney_func_included
    #endinput
#endif
#define _tourney_func_included

// Tournament Functions & Core Engine

stock bool:IsPlayerInTournament(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return false;
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
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return false;

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
        if(IsPlayerConnected(i) && User[i][isLogged])
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

stock GetTourneyCustomArenaCoords(id, &Float:x1, &Float:y1, &Float:z1, &Float:a1, &Float:x2, &Float:y2, &Float:z2, &Float:a2, &interior)
{
    interior = 0;
    switch(id)
    {
        case 0: // Chinese Garden
        {
            x1 = 3470.0; y1 = 5.0; z1 = 1001.5; a1 = 270.0;
            x2 = 3520.0; y2 = 5.0; z2 = 1001.5; a2 = 90.0;
        }
        case 1: // Desert
        {
            x1 = 3450.0; y1 = 970.0; z1 = 1001.5; a1 = 45.0;
            x2 = 3515.0; y2 = 1025.0; z2 = 1001.5; a2 = 225.0;
        }
        case 2: // Egypt
        {
            x1 = 3445.0; y1 = 1935.0; z1 = 1004.5; a1 = 45.0;
            x2 = 3540.0; y2 = 2035.0; z2 = 1004.5; a2 = 225.0;
        }
        case 3: // Urbex
        {
            x1 = -45.0; y1 = 2485.0; z1 = 993.5; a1 = 45.0;
            x2 = -5.0;  y2 = 2505.0; z2 = 993.5; a2 = 225.0;
        }
        case 4: // Zoo
        {
            x1 = 3480.0; y1 = -1005.0; z1 = 1001.5; a1 = 45.0;
            x2 = 3525.0; y2 = -985.0;  z2 = 1001.5; a2 = 225.0;
        }
        default:
        {
            x1 = 3470.0; y1 = 5.0; z1 = 1001.5; a1 = 270.0;
            x2 = 3520.0; y2 = 5.0; z2 = 1001.5; a2 = 90.0;
        }
    }
}

stock GetTourneyServerArenaCoords(id, &Float:x1, &Float:y1, &Float:z1, &Float:a1, &Float:x2, &Float:y2, &Float:z2, &Float:a2, &interior)
{
    if(id < 0 || id >= MAX_TOURNEY_SERVER_ARENAS) id = 0;

    if(IsCustomArena(id)) id = 0;

    interior = arenaData[id][arenaInterior];

    new Float:minX = arenaData[id][arenaPos][0];
    new Float:minY = arenaData[id][arenaPos][1];
    new Float:maxX = arenaData[id][arenaPos][2];
    new Float:maxY = arenaData[id][arenaPos][3];

    x1 = minX + (maxX - minX) * 0.35;
    y1 = minY + (maxY - minY) * 0.35;
    x2 = minX + (maxX - minX) * 0.65;
    y2 = minY + (maxY - minY) * 0.65;

    CA_RayCastLine(x1, y1, 1000.0, x1, y1, -1000.0, x1, y1, z1);
    CA_RayCastLine(x2, y2, 1000.0, x2, y2, -1000.0, x2, y2, z2);

    z1 += 1.0;
    z2 += 1.0;

    a1 = 45.0;
    a2 = 225.0;
}

stock GetTourneyDuelArenaCoords(id, &Float:x1, &Float:y1, &Float:z1, &Float:a1, &Float:x2, &Float:y2, &Float:z2, &Float:a2, &interior)
{
    if(id < 0 || id >= sizeof(DuelS_ArenaNames)) id = 0;

    interior = DuelS_ArenaInteriors[id];
    x1 = DuelS_Pos[id][0][0];
    y1 = DuelS_Pos[id][0][1];
    z1 = DuelS_Pos[id][0][2];
    a1 = DuelS_Pos[id][0][3];

    x2 = DuelS_Pos[id][1][0];
    y2 = DuelS_Pos[id][1][1];
    z2 = DuelS_Pos[id][1][2];
    a2 = DuelS_Pos[id][1][3];
}

stock GetTourneyArenaDisplayName(arenaCat, arenaid, dest[], size = sizeof(dest))
{
    switch(arenaCat)
    {
        case TOURNEY_ARENA_CAT_RANDOM:
        {
            strcopy(dest, "Random (Any Category)", size);
        }
        case TOURNEY_ARENA_CAT_DUEL:
        {
            if(arenaid >= 0 && arenaid < sizeof(DuelS_ArenaNames))
                format(dest, size, "Duel: %s", DuelS_ArenaNames[arenaid]);
            else
                strcopy(dest, "Duel: Random Arena", size);
        }
        case TOURNEY_ARENA_CAT_SERVER:
        {
            if(arenaid >= 0 && arenaid < MAX_TOURNEY_SERVER_ARENAS)
                format(dest, size, "Server: %s", arenaData[arenaid][arenaName]);
            else
                strcopy(dest, "Server: Random Arena", size);
        }
        case TOURNEY_ARENA_CAT_CUSTOM:
        {
            if(arenaid >= 0 && arenaid < MAX_TOURNEY_CUSTOM_ARENAS)
                format(dest, size, "Custom: %s", g_TourneyCustomArenaNames[arenaid]);
            else
                strcopy(dest, "Custom: Random Arena", size);
        }
        default:
        {
            strcopy(dest, "Random Arena", size);
        }
    }
    return 1;
}

stock PickTourneyMatchArena(matchid)
{
    new cat = g_TourneyData[t_ArenaCategory];
    new id = g_TourneyData[t_ArenaID];

    if(cat == TOURNEY_ARENA_CAT_RANDOM)
    {
        new r = random(3);
        if(r == 0)
        {
            g_TourneyMatches[matchid][tm_ArenaCategory] = TOURNEY_ARENA_CAT_DUEL;
            g_TourneyMatches[matchid][tm_ArenaID] = random(sizeof(DuelS_ArenaNames));
        }
        else if(r == 1)
        {
            g_TourneyMatches[matchid][tm_ArenaCategory] = TOURNEY_ARENA_CAT_SERVER;
            g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_SERVER_ARENAS);
        }
        else
        {
            g_TourneyMatches[matchid][tm_ArenaCategory] = TOURNEY_ARENA_CAT_CUSTOM;
            g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_CUSTOM_ARENAS);
        }
    }
    else
    {
        g_TourneyMatches[matchid][tm_ArenaCategory] = cat;
        if(id != -1)
        {
            g_TourneyMatches[matchid][tm_ArenaID] = id;
        }
        else
        {
            switch(cat)
            {
                case TOURNEY_ARENA_CAT_DUEL: g_TourneyMatches[matchid][tm_ArenaID] = random(sizeof(DuelS_ArenaNames));
                case TOURNEY_ARENA_CAT_SERVER: g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_SERVER_ARENAS);
                case TOURNEY_ARENA_CAT_CUSTOM: g_TourneyMatches[matchid][tm_ArenaID] = random(MAX_TOURNEY_CUSTOM_ARENAS);
            }
        }
    }
}

stock KickTourneyParticipant(slot, adminid = INVALID_PLAYER_ID)
{
    if(slot < 0 || slot >= MAX_TOURNEY_SLOTS || !g_TourneyParticipants[slot][tp_Registered]) return 0;
    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION) return 0;

    new p1 = g_TourneyParticipants[slot][tp_Player1];
    new p2 = g_TourneyParticipants[slot][tp_Player2];

    if(g_TourneyData[t_EntryFee] > 0)
    {
        if(p1 != INVALID_PLAYER_ID && IsPlayerConnected(p1) && User[p1][isLogged])
        {
            GivePlayerMoney(p1, g_TourneyData[t_EntryFee]);
            User[p1][playerMoney] += g_TourneyData[t_EntryFee];
            SendClientMessage(p1, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You were removed from the tournament by staff and refunded.");
        }
        if(p2 != INVALID_PLAYER_ID && IsPlayerConnected(p2) && User[p2][isLogged])
        {
            GivePlayerMoney(p2, g_TourneyData[t_EntryFee]);
            User[p2][playerMoney] += g_TourneyData[t_EntryFee];
            SendClientMessage(p2, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You were removed from the tournament by staff and refunded.");
        }
    }

    if(p1 != INVALID_PLAYER_ID && 0 <= p1 < MAX_PLAYERS) g_PlayerTourneySlot[p1] = -1;
    if(p2 != INVALID_PLAYER_ID && 0 <= p2 < MAX_PLAYERS) g_PlayerTourneySlot[p2] = -1;

    new partName[64];
    GetParticipantDisplayName(slot, partName, sizeof(partName));

    g_TourneyParticipants[slot][tp_Registered] = false;
    g_TourneyParticipants[slot][tp_Player1] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player1_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player1_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_Player2] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player2_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player2_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_ClanID] = -1;

    if(g_TourneyData[t_RegisteredCount] > 0) g_TourneyData[t_RegisteredCount]--;

    if(adminid != INVALID_PLAYER_ID && IsPlayerConnected(adminid))
    {
        va_SendClientMessage(adminid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Participant %s was removed from slot #%d.", partName, slot + 1);
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

stock RegisterParticipant(playerid, teammateid = INVALID_PLAYER_ID)
{
    if(!User[playerid][isLogged]) return -1;
    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Registrations are currently closed!");
        return -1;
    }

    if(g_PlayerTourneySlot[playerid] != -1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are already registered in the tournament!");
        return -1;
    }

    if(g_TourneyData[t_RegisteredCount] >= g_TourneyData[t_MaxSlots])
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}The tournament registration slots are already full!");
        return -1;
    }

    if(g_TourneyData[t_RequireDiscord])
    {
        if(!IsPlayerRegisteredOnDiscord(playerid))
        {
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You must complete your application on Discord first!");
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Go to Discord channel {23C552}#1539673870657921135 {FFFFFF}and use: {MAIN}/tournament <nick> <ucp_profile>");
            SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Without a Discord application, you can only spectate with {MAIN}/bracket {FFFFFF}or {MAIN}/spectournament{FFFFFF}.");
            return -1;
        }
    }

    if(g_TourneyData[t_EntryFee] > 0)
    {
        if(User[playerid][playerMoney] < g_TourneyData[t_EntryFee])
        {
            va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You need %s to pay the registration fee.", FormatMoney(g_TourneyData[t_EntryFee]));
            return -1;
        }
    }

    new slot = -1;
    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(!g_TourneyParticipants[i][tp_Registered])
        {
            slot = i;
            break;
        }
    }

    if(slot == -1) return -1;

    switch(g_TourneyData[t_Mode])
    {
        case TOURNAMENT_MODE_1V1:
        {
            g_TourneyParticipants[slot][tp_Registered] = true;
            g_TourneyParticipants[slot][tp_Player1] = playerid;
            g_TourneyParticipants[slot][tp_Player1_SQL] = User[playerid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player1_Name], User[playerid][userName], MAX_PLAYER_NAME + 1);
            g_TourneyParticipants[slot][tp_Eliminated] = false;
            g_TourneyParticipants[slot][tp_Seed] = slot + 1;
            g_PlayerTourneySlot[playerid] = slot;

            if(g_TourneyData[t_EntryFee] > 0)
            {
                GivePlayerMoney(playerid, -g_TourneyData[t_EntryFee]);
                User[playerid][playerMoney] -= g_TourneyData[t_EntryFee];
            }

            g_TourneyData[t_RegisteredCount]++;

            va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s joined the tournament! ({MAIN}%d/%d {FFFFFF}slots filled).",
                User[playerid][userName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        }
        case TOURNAMENT_MODE_2V2:
        {
            if(teammateid == INVALID_PLAYER_ID || !IsPlayerConnected(teammateid) || !User[teammateid][isLogged] || teammateid == playerid)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Usage: /jointournament [teammate id/name]");
                return -1;
            }

            if(g_PlayerTourneySlot[teammateid] != -1)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate is already registered in another team!");
                return -1;
            }

            if(g_PlayerTourneyInvite[teammateid] != playerid)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate has not accepted the invite yet.");
                return -1;
            }

            if(g_TourneyData[t_RequireDiscord] && !IsPlayerRegisteredOnDiscord(teammateid))
            {
                va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate %s has not completed their Discord application yet!", User[teammateid][userName]);
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}They must go to Discord channel {23C552}#1539673870657921135 {FFFFFF}and use: {MAIN}/tournament <nick> <ucp_profile>");
                return -1;
            }

            if(g_TourneyData[t_EntryFee] > 0 && User[teammateid][playerMoney] < g_TourneyData[t_EntryFee])
            {
                va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your teammate does not have enough money for the registration fee (%s).", FormatMoney(g_TourneyData[t_EntryFee]));
                return -1;
            }

            g_TourneyParticipants[slot][tp_Registered] = true;
            g_TourneyParticipants[slot][tp_Player1] = playerid;
            g_TourneyParticipants[slot][tp_Player1_SQL] = User[playerid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player1_Name], User[playerid][userName], MAX_PLAYER_NAME + 1);

            g_TourneyParticipants[slot][tp_Player2] = teammateid;
            g_TourneyParticipants[slot][tp_Player2_SQL] = User[teammateid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player2_Name], User[teammateid][userName], MAX_PLAYER_NAME + 1);

            g_TourneyParticipants[slot][tp_Eliminated] = false;
            g_TourneyParticipants[slot][tp_Seed] = slot + 1;
            g_PlayerTourneySlot[playerid] = slot;
            g_PlayerTourneySlot[teammateid] = slot;

            g_PlayerTourneyInvite[teammateid] = -1;

            if(g_TourneyData[t_EntryFee] > 0)
            {
                GivePlayerMoney(playerid, -g_TourneyData[t_EntryFee]);
                User[playerid][playerMoney] -= g_TourneyData[t_EntryFee];

                GivePlayerMoney(teammateid, -g_TourneyData[t_EntryFee]);
                User[teammateid][playerMoney] -= g_TourneyData[t_EntryFee];
            }

            g_TourneyData[t_RegisteredCount]++;

            va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Team {MAIN}%s & %s {FFFFFF}joined the 2v2 tournament! ({MAIN}%d/%d {FFFFFF}slots).",
                User[playerid][userName], User[teammateid][userName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        }
        case TOURNAMENT_MODE_CLAN:
        {
            new clanid = User[playerid][clanID];
            if(clanid == -1)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You must be in a clan to register in Clan War tournament!");
                return -1;
            }

            if(!HasClanRank(playerid, RANK_COFOUNDER))
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Only Clan Leader or Co-Founder can register the clan!");
                return -1;
            }

            if(GetClanMemberCount(clanid) < 2)
            {
                SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your clan must have at least 2 members to participate in Clan War!");
                return -1;
            }

            for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
            {
                if(g_TourneyParticipants[i][tp_Registered] && g_TourneyParticipants[i][tp_ClanID] == clanid)
                {
                    SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your clan is already registered!");
                    return -1;
                }
            }

            g_TourneyParticipants[slot][tp_Registered] = true;
            g_TourneyParticipants[slot][tp_Player1] = playerid;
            g_TourneyParticipants[slot][tp_Player1_SQL] = User[playerid][user_sqlid];
            strcopy(g_TourneyParticipants[slot][tp_Player1_Name], User[playerid][userName], MAX_PLAYER_NAME + 1);

            if(teammateid != INVALID_PLAYER_ID && teammateid != playerid && IsPlayerConnected(teammateid)
                && User[teammateid][isLogged] && User[teammateid][clanID] == clanid && g_PlayerTourneySlot[teammateid] == -1)
            {
                g_TourneyParticipants[slot][tp_Player2] = teammateid;
                g_TourneyParticipants[slot][tp_Player2_SQL] = User[teammateid][user_sqlid];
                strcopy(g_TourneyParticipants[slot][tp_Player2_Name], User[teammateid][userName], MAX_PLAYER_NAME + 1);
                g_PlayerTourneySlot[teammateid] = slot;
            }
            else
            {
                g_TourneyParticipants[slot][tp_Player2] = INVALID_PLAYER_ID;
                g_TourneyParticipants[slot][tp_Player2_SQL] = 0;
                g_TourneyParticipants[slot][tp_Player2_Name][0] = '\0';
            }

            g_TourneyParticipants[slot][tp_ClanID] = clanid;
            strcopy(g_TourneyParticipants[slot][tp_ClanName], ClanInfo[clanid][clanName], CLAN_NAME_LEN);
            strcopy(g_TourneyParticipants[slot][tp_ClanTag], ClanInfo[clanid][clanTag], CLAN_TAG_LEN);
            g_TourneyParticipants[slot][tp_Eliminated] = false;
            g_TourneyParticipants[slot][tp_Seed] = slot + 1;
            g_PlayerTourneySlot[playerid] = slot;

            if(g_TourneyData[t_EntryFee] > 0)
            {
                GivePlayerMoney(playerid, -g_TourneyData[t_EntryFee]);
                User[playerid][playerMoney] -= g_TourneyData[t_EntryFee];
            }

            g_TourneyData[t_RegisteredCount]++;

            va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Clan {MAIN}[%s] %s {FFFFFF}registered for Clan War tournament! ({MAIN}%d/%d {FFFFFF}slots).",
                ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        }
    }

    // Auto-start check
    if(g_TourneyData[t_AutoStart] && g_TourneyData[t_RegisteredCount] == g_TourneyData[t_MaxSlots])
    {
        SendTourneyBroadcast("All registration slots are filled! Tournament will begin in 10 seconds!");
        g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
        SetTimerEx("StartTournamentCountdown", 10000, false, "d", 0);
    }

    return slot;
}

stock SendTourneyInvite(playerid, teammateid)
{
    if(teammateid == INVALID_PLAYER_ID || !IsPlayerConnected(teammateid) || !User[teammateid][isLogged] || teammateid == playerid)
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Usage: /jointournament [teammate id/name]");

    if(g_PlayerTourneySlot[playerid] != -1)
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are already registered in the tournament!");

    if(g_PlayerTourneySlot[teammateid] != -1)
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}That player is already registered in another team!");

    if(g_TourneyData[t_RequireDiscord] && !IsPlayerRegisteredOnDiscord(teammateid))
        return va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}%s has not completed the Discord application yet.", User[teammateid][userName]);

    g_PlayerTourneyInvite[teammateid] = playerid;

    va_SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Invite sent to %s. Waiting for them to accept.", User[teammateid][userName]);
    va_SendClientMessage(teammateid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}%s invited you as their 2v2 teammate. Type {MAIN}/accepttourney {FFFFFF}to accept.", User[playerid][userName]);
    return 1;
}

stock AcceptTourneyInvite(playerid)
{
    new inviter = g_PlayerTourneyInvite[playerid];

    if(inviter == -1 || !IsPlayerConnected(inviter) || !User[inviter][isLogged])
    {
        g_PlayerTourneyInvite[playerid] = -1;
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You have no pending tournament invite.");
    }

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        g_PlayerTourneyInvite[playerid] = -1;
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Registrations are currently closed!");
    }

    if(g_PlayerTourneySlot[inviter] != -1)
    {
        g_PlayerTourneyInvite[playerid] = -1;
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}The player who invited you already registered with someone else.");
    }

    return RegisterParticipant(inviter, playerid);
}

stock UnregisterParticipant(playerid)
{
    if(!IsPlayerInTournament(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You are not registered in the tournament!");
        return 0;
    }

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot unregister once the tournament has started!");
        return 0;
    }

    new slot = g_PlayerTourneySlot[playerid];
    if(slot < 0 || slot >= MAX_TOURNEY_SLOTS) return 0;

    new p1 = g_TourneyParticipants[slot][tp_Player1];
    new p2 = g_TourneyParticipants[slot][tp_Player2];

    new bool:p1Valid = (p1 != INVALID_PLAYER_ID && 0 <= p1 < MAX_PLAYERS);
    new bool:p2Valid = (p2 != INVALID_PLAYER_ID && 0 <= p2 < MAX_PLAYERS && p2 != p1);

    if(g_TourneyData[t_EntryFee] > 0)
    {
        if(p1Valid && IsPlayerConnected(p1))
        {
            GivePlayerMoney(p1, g_TourneyData[t_EntryFee]);
            User[p1][playerMoney] += g_TourneyData[t_EntryFee];
        }
        if(p2Valid && IsPlayerConnected(p2))
        {
            GivePlayerMoney(p2, g_TourneyData[t_EntryFee]);
            User[p2][playerMoney] += g_TourneyData[t_EntryFee];
        }
    }

    if(p1Valid) g_PlayerTourneySlot[p1] = -1;
    if(p2Valid) g_PlayerTourneySlot[p2] = -1;

    g_PlayerTourneySlot[playerid] = -1;

    g_TourneyParticipants[slot][tp_Registered] = false;
    g_TourneyParticipants[slot][tp_Player1] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player1_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player1_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_Player2] = INVALID_PLAYER_ID;
    g_TourneyParticipants[slot][tp_Player2_SQL] = 0;
    g_TourneyParticipants[slot][tp_Player2_Name][0] = '\0';
    g_TourneyParticipants[slot][tp_ClanID] = -1;

    if(g_TourneyData[t_RegisteredCount] > 0) g_TourneyData[t_RegisteredCount]--;

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s left the tournament registration. ({MAIN}%d/%d {FFFFFF}slots remaining).",
        User[playerid][userName], g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);

    return 1;
}

stock StartTournamentRegistration(adminid)
{
    if(g_TourneyData[t_Active] || g_TourneyData[t_State] != TOURNAMENT_STATE_NONE)
    {
        if(IsPlayerConnected(adminid))
            SendClientMessage(adminid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}A tournament is already active or open!");
        return 0;
    }

    g_TourneyData[t_Active] = true;
    g_TourneyData[t_State] = TOURNAMENT_STATE_REGISTRATION;
    g_TourneyData[t_RegisteredCount] = 0;
    g_TourneyData[t_StartTime] = gettime();

    if(IsPlayerConnected(adminid))
        strcopy(g_TourneyData[t_StartedBy], User[adminid][userName], MAX_PLAYER_NAME + 1);

    new modeName[32];
    switch(g_TourneyData[t_Mode])
    {
        case TOURNAMENT_MODE_1V1: strcopy(modeName, "1v1 Elimination");
        case TOURNAMENT_MODE_2V2: strcopy(modeName, "2v2 Team Battle");
        case TOURNAMENT_MODE_CLAN: strcopy(modeName, "Clan War");
    }

    new feeStr[32];
    if(g_TourneyData[t_EntryFee] > 0) format(feeStr, sizeof(feeStr), "%s", FormatMoney(g_TourneyData[t_EntryFee]));
    else strcopy(feeStr, "FREE");

    new prizeStr[64];
    format(prizeStr, sizeof(prizeStr), "{FFD700}%d Coins {FFFFFF}+ {23C552}%s", g_TourneyData[t_PrizeCoins], FormatMoney(g_TourneyData[t_PrizeMoney]));

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT OPEN] {FFFFFF}Administrator {MAIN}%s {FFFFFF}opened registrations for {MAIN}%s{FFFFFF}!", g_TourneyData[t_StartedBy], g_TourneyData[t_Name]);
    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {B4B5B7}For full information, schedule, prizes and registration, check our {00B6F6}Discord{B4B5B7} server!");

    new discordAnnounce[1024];
    format(discordAnnounce, sizeof(discordAnnounce),
        "**Event Information**\n"\
        "- **Tournament:** `%s`\n"\
        "- **Schedule:** `%s`\n"\
        "- **Modality:** `%s` (%d Slots)\n\n"\
        "**Prizes & Rewards**\n"\
        "- **Champion Tag:** `%s`\n"\
        "- **Grand Prize:** `%d Coins` + `%s`\n"\
        "- **Entry Fee:** `%s`\n"\
        "- **Weapon Loadout:** `%s`\n\n"\
        "**How to Participate**\n"\
        "Use `/tournament <InGame_Nick>` in <#1539673870657921135> to apply!",
        g_TourneyData[t_Name], g_TourneyData[t_Schedule], modeName, g_TourneyData[t_MaxSlots],
        g_TourneyData[t_AchievementTag], g_TourneyData[t_PrizeCoins], FormatMoney(g_TourneyData[t_PrizeMoney]),
        feeStr, g_TourneyWeaponSets[g_TourneyData[t_WeaponSet]][twi_Name]
    );
    utf8encode(msgencoded, discordAnnounce, sizeof(msgencoded));

    new discordTitle[128];
    format(discordTitle, sizeof(discordTitle), ":trophy: %s - Registrations Open", g_TourneyData[t_Name]);
    SendDiscordEmbed(ch_tourney_announce, discordTitle, msgencoded, DISCORD_COLOR_WARNING);

    new arenaNameStr[32];
    if(g_TourneyData[t_ArenaID] >= 0 && g_TourneyData[t_ArenaID] < sizeof(DuelS_ArenaNames)) strcopy(arenaNameStr, DuelS_ArenaNames[g_TourneyData[t_ArenaID]]);
    else strcopy(arenaNameStr, "Random");

    new adminLogEmbed[1024];
    format(adminLogEmbed, sizeof(adminLogEmbed),
        "**Overview**\n"\
        "- **Tournament:** `%s`\n"\
        "- **Created by:** `%s`\n"\
        "- **Scheduled for:** `%s`\n\n"\
        "**Match Configuration**\n"\
        "- **Modality:** `%s` (%d Slots)\n"\
        "- **Format:** `%s` (%d sec/round)\n"\
        "- **Weapon Loadout:** `%s`\n"\
        "- **Arena:** `%s`\n\n"\
        "**Rewards & Settings**\n"\
        "- **Prize:** `%d Coins` + `%s`\n"\
        "- **Achievement:** `%s`\n"\
        "- **Entry Fee:** `%s`\n"\
        "- **Auto-Start:** `%s`\n"\
        "- **Created At:** `%s`",
        g_TourneyData[t_Name], g_TourneyData[t_StartedBy], g_TourneyData[t_Schedule],
        modeName, g_TourneyData[t_MaxSlots],
        (g_TourneyData[t_RoundsPerMatch] == 1) ? "Best of 1" : "Best of 3 (Bo3)", g_TourneyData[t_RoundTimeLimit],
        g_TourneyWeaponSets[g_TourneyData[t_WeaponSet]][twi_Name], arenaNameStr,
        g_TourneyData[t_PrizeCoins], FormatMoney(g_TourneyData[t_PrizeMoney]),
        g_TourneyData[t_AchievementTag], feeStr,
        g_TourneyData[t_AutoStart] ? "Enabled" : "Disabled",
        returnDate(gettime())
    );
    utf8encode(msgencoded, adminLogEmbed, sizeof(msgencoded));
    SendDiscordEmbed(ch_tourney_admin_logs, ":hammer_and_wrench: Tournament Created", msgencoded, DISCORD_COLOR_ADMIN);

    return 1;
}

stock ProcessDiscordTourneyApp(DCC_User:user, DCC_Channel:channel, const params[])
{
    if(_:user == 0) return 0;

    new bool:isBot;
    DCC_IsUserBot(user, isBot);
    if(isBot) return 0;

    new userid[DCC_ID_SIZE];
    DCC_GetUserId(user, userid, sizeof(userid));

    new username[DCC_USERNAME_SIZE];
    DCC_GetUserName(user, username, sizeof(username));

    if(!IsDiscordUserVerified(user))
    {
        new notVerifiedMsg[256];
        format(notVerifiedMsg, sizeof(notVerifiedMsg), "<@%s> :no_entry: Only **verified members** can register for tournaments!\nPlease link your in-game account using `/dvincular` to get verified.", userid);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, notVerifiedMsg);
        return 0;
    }

    if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
    {
        new failMsg[192];
        format(failMsg, sizeof(failMsg), "<@%s> :warning: There are no open tournament registrations right now.", userid);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, failMsg);
        return 0;
    }

    new cleanParams[256];
    format(cleanParams, sizeof(cleanParams), "%s", params);
    cleanParams[sizeof(cleanParams) - 1] = '\0';

    if(!strcmp(cleanParams, "/tournament", true, 11) || !strcmp(cleanParams, ".tournament", true, 11))
    {
        strdel(cleanParams, 0, 11);
    }
    else if(!strcmp(cleanParams, "/torneio", true, 8) || !strcmp(cleanParams, ".torneio", true, 8))
    {
        strdel(cleanParams, 0, 8);
    }
    else if(!strcmp(cleanParams, "!tournament", true, 11))
    {
        strdel(cleanParams, 0, 11);
    }
    else if(!strcmp(cleanParams, "!torneio", true, 8))
    {
        strdel(cleanParams, 0, 8);
    }

    new startPos = 0;
    while(cleanParams[startPos] == ' ' || cleanParams[startPos] == '\t') startPos++;
    if(startPos > 0) strdel(cleanParams, 0, startPos);

    new nick[MAX_PLAYER_NAME + 1];
    if(sscanf(cleanParams, "s[25]", nick))
    {
        new usageMsg[256];
        format(usageMsg, sizeof(usageMsg), "<@%s> :x: Invalid format!\n**Usage:** `/tournament <InGame_Nick>`\n*Example:* `/tournament Nick_Silva`", userid);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, usageMsg);
        return 0;
    }

    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(g_TourneyParticipants[i][tp_Registered])
        {
            if(!strcmp(g_TourneyParticipants[i][tp_Player1_Name], nick, true) || (g_TourneyParticipants[i][tp_Player2_Name][0] != '\0' && !strcmp(g_TourneyParticipants[i][tp_Player2_Name], nick, true)))
            {
                new alreadyMsg[256];
                format(alreadyMsg, sizeof(alreadyMsg), "<@%s> :warning: Nick **%s** is already registered in the tournament!", userid, nick);
                if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, alreadyMsg);
                return 0;
            }
        }
    }

    new slot = -1;
    for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
    {
        if(!g_TourneyParticipants[i][tp_Registered])
        {
            slot = i;
            break;
        }
    }

    if(slot == -1)
    {
        new fullMsg[192];
        format(fullMsg, sizeof(fullMsg), "<@%s> :warning: The tournament registration slots are currently full (%d/%d)!", userid, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, fullMsg);
        return 0;
    }

    new onlinePlayerId = INVALID_PLAYER_ID;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && User[i][isLogged])
        {
            if(!strcmp(User[i][userName], nick, true))
            {
                onlinePlayerId = i;
                break;
            }
        }
    }

    if(onlinePlayerId != INVALID_PLAYER_ID)
    {
        g_TourneyParticipants[slot][tp_Registered] = true;
        g_TourneyParticipants[slot][tp_Eliminated] = false;
        g_TourneyParticipants[slot][tp_Seed] = slot + 1;
        strcopy(g_TourneyParticipants[slot][tp_Player1_Name], nick, MAX_PLAYER_NAME + 1);
        g_TourneyParticipants[slot][tp_Player1] = onlinePlayerId;
        g_TourneyParticipants[slot][tp_Player1_SQL] = User[onlinePlayerId][user_sqlid];
        g_PlayerTourneySlot[onlinePlayerId] = slot;
        if(g_TourneyData[t_EntryFee] > 0 && User[onlinePlayerId][playerMoney] >= g_TourneyData[t_EntryFee])
        {
            GivePlayerMoney(onlinePlayerId, -g_TourneyData[t_EntryFee]);
            User[onlinePlayerId][playerMoney] -= g_TourneyData[t_EntryFee];
        }

        g_TourneyData[t_RegisteredCount]++;

        new appIdx = -1;
        for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
        {
            if(g_TourneyDiscordApps[i][tda_Active] && !strcmp(g_TourneyDiscordApps[i][tda_Nick], nick, true))
            {
                appIdx = i;
                break;
            }
        }
        if(appIdx == -1)
        {
            for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
            {
                if(!g_TourneyDiscordApps[i][tda_Active])
                {
                    appIdx = i;
                    break;
                }
            }
        }

        if(appIdx != -1)
        {
            g_TourneyDiscordApps[appIdx][tda_Active] = true;
            strcopy(g_TourneyDiscordApps[appIdx][tda_Nick], nick, MAX_PLAYER_NAME + 1);
            strcopy(g_TourneyDiscordApps[appIdx][tda_UCP], "Verified Discord", 128);
            strcopy(g_TourneyDiscordApps[appIdx][tda_DiscordID], userid, DCC_ID_SIZE);
            strcopy(g_TourneyDiscordApps[appIdx][tda_DiscordTag], username, DCC_USERNAME_SIZE);
            g_TourneyDiscordApps[appIdx][tda_Timestamp] = gettime();
            g_TourneyDiscordAppCount++;
        }

        new appEmbed[1024];
        format(appEmbed, sizeof(appEmbed),
            "**Applicant Details**\n"\
            "- **In-Game Nick:** `%s`\n"\
            "- **Discord Member:** <@%s> (`%s`)\n"\
            "- **Account Status:** :white_check_mark: Verified Account (Online)\n\n"\
            "**Tournament Status**\n"\
            "- **Event:** `%s`\n"\
            "- **Slot Assigned:** `#%02d`\n"\
            "- **Registered Slots:** `%d / %d`\n"\
            "- **Application Time:** `%s`",
            nick, userid, username,
            g_TourneyData[t_Name],
            slot + 1,
            g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots],
            returnDate(gettime())
        );

        utf8encode(msgencoded, appEmbed, sizeof(msgencoded));
        SendDiscordEmbed(ch_tourney_applications, ":pencil: Tournament Registration Received", msgencoded, DISCORD_COLOR_SUCCESS);

        new confirmMsg[256];
        format(confirmMsg, sizeof(confirmMsg), ":white_check_mark: <@%s> Your registration with nick **%s** is confirmed! (Slot #%02d - **%d/%d** slots filled).", userid, nick, slot + 1, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, confirmMsg);

        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s joined the tournament via Discord! ({MAIN}%d/%d {FFFFFF}slots filled).",
            nick, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);

        if(g_TourneyData[t_AutoStart] && g_TourneyData[t_RegisteredCount] >= g_TourneyData[t_MaxSlots])
        {
            g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
            StartTournamentCountdown(0);
        }
        return 1;
    }

    inline const OnTourneyCheckAccount()
    {
        if(!cache_num_rows())
        {
            new notFoundMsg[256];
            format(notFoundMsg, sizeof(notFoundMsg), "<@%s> :x: The account **%s** does not exist on the server! Please make sure you registered this account in-game.", userid, nick);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, notFoundMsg);
            return 1;
        }

        if(g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION)
        {
            new failMsg[192];
            format(failMsg, sizeof(failMsg), "<@%s> :warning: Tournament registrations closed in the meantime.", userid);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, failMsg);
            return 1;
        }

        for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
        {
            if(g_TourneyParticipants[i][tp_Registered])
            {
                if(!strcmp(g_TourneyParticipants[i][tp_Player1_Name], nick, true) || (g_TourneyParticipants[i][tp_Player2_Name][0] != '\0' && !strcmp(g_TourneyParticipants[i][tp_Player2_Name], nick, true)))
                {
                    new alreadyMsg[256];
                    format(alreadyMsg, sizeof(alreadyMsg), "<@%s> :warning: Nick **%s** is already registered in the tournament!", userid, nick);
                    if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, alreadyMsg);
                    return 1;
                }
            }
        }

        new slotIdx = -1;
        for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
        {
            if(!g_TourneyParticipants[i][tp_Registered])
            {
                slotIdx = i;
                break;
            }
        }

        if(slotIdx == -1)
        {
            new fullMsg[192];
            format(fullMsg, sizeof(fullMsg), "<@%s> :warning: The tournament registration slots are currently full (%d/%d)!", userid, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
            if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, fullMsg);
            return 1;
        }

        new acc_id = 0;
        new acc_name[MAX_PLAYER_NAME + 1];
        cache_get_value_int(0, "id", acc_id);
        cache_get_value_name(0, "username", acc_name, sizeof(acc_name));

        g_TourneyParticipants[slotIdx][tp_Registered] = true;
        g_TourneyParticipants[slotIdx][tp_Eliminated] = false;
        g_TourneyParticipants[slotIdx][tp_Seed] = slotIdx + 1;
        g_TourneyParticipants[slotIdx][tp_Player1] = INVALID_PLAYER_ID;
        g_TourneyParticipants[slotIdx][tp_Player1_SQL] = acc_id;
        strcopy(g_TourneyParticipants[slotIdx][tp_Player1_Name], acc_name, MAX_PLAYER_NAME + 1);

        g_TourneyData[t_RegisteredCount]++;

        new appIdx2 = -1;
        for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
        {
            if(g_TourneyDiscordApps[i][tda_Active] && !strcmp(g_TourneyDiscordApps[i][tda_Nick], acc_name, true))
            {
                appIdx2 = i;
                break;
            }
        }
        if(appIdx2 == -1)
        {
            for(new i = 0; i < MAX_DISCORD_TOURNEY_APPS; i++)
            {
                if(!g_TourneyDiscordApps[i][tda_Active])
                {
                    appIdx2 = i;
                    break;
                }
            }
        }

        if(appIdx2 != -1)
        {
            g_TourneyDiscordApps[appIdx2][tda_Active] = true;
            strcopy(g_TourneyDiscordApps[appIdx2][tda_Nick], acc_name, MAX_PLAYER_NAME + 1);
            strcopy(g_TourneyDiscordApps[appIdx2][tda_UCP], "Verified Discord", 128);
            strcopy(g_TourneyDiscordApps[appIdx2][tda_DiscordID], userid, DCC_ID_SIZE);
            strcopy(g_TourneyDiscordApps[appIdx2][tda_DiscordTag], username, DCC_USERNAME_SIZE);
            g_TourneyDiscordApps[appIdx2][tda_Timestamp] = gettime();
            g_TourneyDiscordAppCount++;
        }

        new appEmbed2[1024];
        format(appEmbed2, sizeof(appEmbed2),
            "**Applicant Details**\n"\
            "- **In-Game Nick:** `%s`\n"\
            "- **Discord Member:** <@%s> (`%s`)\n"\
            "- **Account Status:** :white_check_mark: Verified Account (ID: `%d`)\n\n"\
            "**Tournament Status**\n"\
            "- **Event:** `%s`\n"\
            "- **Slot Assigned:** `#%02d`\n"\
            "- **Registered Slots:** `%d / %d`\n"\
            "- **Application Time:** `%s`",
            acc_name, userid, username, acc_id,
            g_TourneyData[t_Name],
            slotIdx + 1,
            g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots],
            returnDate(gettime())
        );

        utf8encode(msgencoded, appEmbed2, sizeof(msgencoded));
        SendDiscordEmbed(ch_tourney_applications, ":pencil: Tournament Registration Received", msgencoded, DISCORD_COLOR_SUCCESS);

        new confirmMsg2[256];
        format(confirmMsg2, sizeof(confirmMsg2), ":white_check_mark: <@%s> Your registration with nick **%s** is confirmed! (Slot #%02d - **%d/%d** slots filled).", userid, acc_name, slotIdx + 1, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);
        if(channel != DCC_INVALID_CHANNEL) DCC_SendChannelMessage(channel, confirmMsg2);

        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s joined the tournament via Discord! ({MAIN}%d/%d {FFFFFF}slots filled).",
            acc_name, g_TourneyData[t_RegisteredCount], g_TourneyData[t_MaxSlots]);

        if(g_TourneyData[t_AutoStart] && g_TourneyData[t_RegisteredCount] >= g_TourneyData[t_MaxSlots])
        {
            g_TourneyData[t_State] = TOURNAMENT_STATE_STARTING;
            StartTournamentCountdown(0);
        }
        return 1;
    }

    new accQuery[128];
    mysql_format(dbHandle, accQuery, sizeof(accQuery), "SELECT `id`, `username` FROM `accounts` WHERE `username` = '%e' LIMIT 1", nick);
    MySQL_TQueryInline(dbHandle, using inline OnTourneyCheckAccount, accQuery);
    return 1;
}

stock GenerateBracket()
{
    new participantsCount = g_TourneyData[t_RegisteredCount];
    if(participantsCount < 2) return 0;

    new bracketSize = 2;
    new totalRounds = 1;
    while(bracketSize < participantsCount)
    {
        bracketSize *= 2;
        totalRounds++;
    }

    g_TourneyData[t_TotalRounds] = totalRounds;
    g_TourneyData[t_CurrentRound] = 1;

    for(new i = 0; i < MAX_TOURNEY_MATCHES; i++)
    {
        g_TourneyMatches[i][tm_Active] = false;
        g_TourneyMatches[i][tm_State] = MATCH_STATE_IDLE;
        g_TourneyMatches[i][tm_Round] = 0;
        g_TourneyMatches[i][tm_MatchIndex] = i;
        g_TourneyMatches[i][tm_SlotA] = -1;
        g_TourneyMatches[i][tm_SlotB] = -1;
        g_TourneyMatches[i][tm_WinnerSlot] = -1;
        g_TourneyMatches[i][tm_ScoreA] = 0;
        g_TourneyMatches[i][tm_ScoreB] = 0;
        g_TourneyMatches[i][tm_ArenaCategory] = TOURNEY_ARENA_CAT_DUEL;
        g_TourneyMatches[i][tm_ArenaID] = 0;
    }

    new matchCount = bracketSize / 2;
    for(new m = 0; m < matchCount; m++)
    {
        new slotA = m * 2;
        new slotB = m * 2 + 1;

        g_TourneyMatches[m][tm_Active] = true;
        g_TourneyMatches[m][tm_Round] = 1;
        g_TourneyMatches[m][tm_MatchIndex] = m;
        g_TourneyMatches[m][tm_SlotA] = (slotA < g_TourneyData[t_MaxSlots] && g_TourneyParticipants[slotA][tp_Registered]) ? slotA : -1;
        g_TourneyMatches[m][tm_SlotB] = (slotB < g_TourneyData[t_MaxSlots] && g_TourneyParticipants[slotB][tp_Registered]) ? slotB : -1;
        g_TourneyMatches[m][tm_State] = MATCH_STATE_PREPARING;
        g_TourneyMatches[m][tm_VirtualWorld] = TOURNEY_VW_BASE + m;

        // category & settings
        PickTourneyMatchArena(m);

        if(g_TourneyMatches[m][tm_SlotA] != -1 && g_TourneyMatches[m][tm_SlotB] == -1)
        {
            g_TourneyMatches[m][tm_WinnerSlot] = g_TourneyMatches[m][tm_SlotA];
            g_TourneyMatches[m][tm_State] = MATCH_STATE_FINISHED;
        }
        else if(g_TourneyMatches[m][tm_SlotA] == -1 && g_TourneyMatches[m][tm_SlotB] != -1)
        {
            g_TourneyMatches[m][tm_WinnerSlot] = g_TourneyMatches[m][tm_SlotB];
            g_TourneyMatches[m][tm_State] = MATCH_STATE_FINISHED;
        }
    }

    return 1;
}

forward StartTournamentCountdown(dummy);
public StartTournamentCountdown(dummy)
{
    #pragma unused dummy
    if(g_TourneyData[t_State] != TOURNAMENT_STATE_STARTING && g_TourneyData[t_State] != TOURNAMENT_STATE_REGISTRATION) return;

    if(g_TourneyData[t_RegisteredCount] < 2)
    {
        SendTourneyBroadcast("Tournament cancelled: not enough participants (minimum 2 required).");
        ResetTournament();
        return;
    }

    g_TourneyData[t_State] = TOURNAMENT_STATE_IN_PROGRESS;
    GenerateBracket();

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT STARTED] {FFFFFF}%s is now starting! Preparing Round 1 matches...", g_TourneyData[t_Name]);
    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {B4B5B7}Type {MAIN}/tourneybracket {B4B5B7}to follow live brackets and match progress.");

    StartTournamentRound(1);
}

stock StartTournamentRound(round)
{
    g_TourneyData[t_CurrentRound] = round;

    new roundName[32];
    if(round == g_TourneyData[t_TotalRounds]) strcopy(roundName, "GRAND FINAL");
    else if(round == g_TourneyData[t_TotalRounds] - 1) strcopy(roundName, "SEMI-FINALS");
    else if(round == g_TourneyData[t_TotalRounds] - 2) strcopy(roundName, "QUARTER-FINALS");
    else format(roundName, sizeof(roundName), "ROUND %d", round);

    va_SendClientMessageToAll(-1, "{MAIN}>>> TOURNAMENT: Starting %s <<<", roundName);

    for(new m = 0; m < MAX_TOURNEY_MATCHES; m++)
    {
        if(g_TourneyMatches[m][tm_Active] && g_TourneyMatches[m][tm_Round] == round)
        {
            if(g_TourneyMatches[m][tm_State] == MATCH_STATE_PREPARING)
            {
                StartTourneyMatch(m);
            }
        }
    }
    return 1;
}

stock SpawnTourneyParticipant(playerid, side, arenaCat, arenaid, vw)
{
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return 0;

    User[playerid][isAfk] = false;

    new Float:x1, Float:y1, Float:z1, Float:a1;
    new Float:x2, Float:y2, Float:z2, Float:a2;
    new interior = 0;

    switch(arenaCat)
    {
        case TOURNEY_ARENA_CAT_DUEL:
        {
            GetTourneyDuelArenaCoords(arenaid, x1, y1, z1, a1, x2, y2, z2, a2, interior);
        }
        case TOURNEY_ARENA_CAT_SERVER:
        {
            GetTourneyServerArenaCoords(arenaid, x1, y1, z1, a1, x2, y2, z2, a2, interior);
        }
        case TOURNEY_ARENA_CAT_CUSTOM:
        {
            GetTourneyCustomArenaCoords(arenaid, x1, y1, z1, a1, x2, y2, z2, a2, interior);
        }
        default:
        {
            GetTourneyDuelArenaCoords(0, x1, y1, z1, a1, x2, y2, z2, a2, interior);
        }
    }

    SetPlayerVirtualWorld(playerid, vw);
    SetPlayerInterior(playerid, interior);

    new Float:x = (side == 0) ? x1 : x2;
    new Float:y = (side == 0) ? y1 : y2;
    new Float:z = (side == 0) ? z1 : z2;
    new Float:a = (side == 0) ? a1 : a2;

    if(g_TourneyData[t_Mode] == TOURNAMENT_MODE_2V2 || g_TourneyData[t_Mode] == TOURNAMENT_MODE_CLAN)
    {
        new slot = g_PlayerTourneySlot[playerid];
        if(slot != -1 && g_TourneyParticipants[slot][tp_Player2] == playerid)
        {
            x += 1.5;
            y += 1.5;
        }
    }

    if(arenaCat == TOURNEY_ARENA_CAT_CUSTOM)
    {
        Streamer_UpdateEx(playerid, x, y, z, -1, interior, STREAMER_TYPE_OBJECT);
    }

    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, a);
    SetCameraBehindPlayer(playerid);

    SetPlayerHealth(playerid, 100.0);
    if(g_TourneyData[t_GiveArmour])
        SetPlayerArmour(playerid, 100.0);
    else
        SetPlayerArmour(playerid, 0.0);

    ResetPlayerWeapons(playerid);
    new wset = g_TourneyData[t_WeaponSet];
    if(wset >= 0 && wset < sizeof(g_TourneyWeaponSets))
    {
        if(g_TourneyWeaponSets[wset][twi_Weap1] > 0)
            GivePlayerWeapon(playerid, WEAPON:g_TourneyWeaponSets[wset][twi_Weap1], g_TourneyWeaponSets[wset][twi_Ammo1]);
        if(g_TourneyWeaponSets[wset][twi_Weap2] > 0)
            GivePlayerWeapon(playerid, WEAPON:g_TourneyWeaponSets[wset][twi_Weap2], g_TourneyWeaponSets[wset][twi_Ammo2]);
        if(g_TourneyWeaponSets[wset][twi_Weap3] > 0)
            GivePlayerWeapon(playerid, WEAPON:g_TourneyWeaponSets[wset][twi_Weap3], g_TourneyWeaponSets[wset][twi_Ammo3]);
        if(g_TourneyWeaponSets[wset][twi_Weap4] > 0)
            GivePlayerWeapon(playerid, WEAPON:g_TourneyWeaponSets[wset][twi_Weap4], g_TourneyWeaponSets[wset][twi_Ammo4]);

        if(g_TourneyWeaponSets[wset][twi_Weap1] > 0)
            SetPlayerArmedWeapon(playerid, WEAPON:g_TourneyWeaponSets[wset][twi_Weap1]);
    }

    TogglePlayerControllableEx(playerid, false);
    return 1;
}

stock StartTourneyMatch(matchid)
{
    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES || !g_TourneyMatches[matchid][tm_Active]) return 0;

    new slotA = g_TourneyMatches[matchid][tm_SlotA];
    new slotB = g_TourneyMatches[matchid][tm_SlotB];

    if(slotA == -1 || slotB == -1) return 0;

    new nameA[64], nameB[64];
    GetParticipantDisplayName(slotA, nameA, sizeof(nameA));
    GetParticipantDisplayName(slotB, nameB, sizeof(nameB));

    new arenaCat = g_TourneyMatches[matchid][tm_ArenaCategory];
    new arenaid = g_TourneyMatches[matchid][tm_ArenaID];
    new vw = g_TourneyMatches[matchid][tm_VirtualWorld];

    new p1A = g_TourneyParticipants[slotA][tp_Player1];
    new p2A = g_TourneyParticipants[slotA][tp_Player2];
    new p1B = g_TourneyParticipants[slotB][tp_Player1];
    new p2B = g_TourneyParticipants[slotB][tp_Player2];
    if(g_TourneyMatches[matchid][tm_TimerMatch] != 0)
    {
        KillTimer(g_TourneyMatches[matchid][tm_TimerMatch]);
        g_TourneyMatches[matchid][tm_TimerMatch] = 0;
    }
    if(g_TourneyMatches[matchid][tm_TimerCountdown] != 0)
    {
        KillTimer(g_TourneyMatches[matchid][tm_TimerCountdown]);
        g_TourneyMatches[matchid][tm_TimerCountdown] = 0;
    }
    new bool:teamA_Present = (IsPlayerConnected(p1A) && User[p1A][isLogged])
        || (p2A != INVALID_PLAYER_ID && IsPlayerConnected(p2A) && User[p2A][isLogged]);
    new bool:teamB_Present = (IsPlayerConnected(p1B) && User[p1B][isLogged])
        || (p2B != INVALID_PLAYER_ID && IsPlayerConnected(p2B) && User[p2B][isLogged]);

    if(!teamA_Present && teamB_Present)
    {
        // Start WO countdown or immediate WO
        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s is offline! %s wins by W.O.!", nameA, nameB);
        FinishTourneyMatch(matchid, slotB, true);
        return 1;
    }
    else if(teamA_Present && !teamB_Present)
    {
        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s is offline! %s wins by W.O.!", nameB, nameA);
        FinishTourneyMatch(matchid, slotA, true);
        return 1;
    }
    else if(!teamA_Present && !teamB_Present)
    {
        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Both %s and %s are offline! Double W.O.!", nameA, nameB);
        FinishTourneyMatch(matchid, -1, true);
        return 1;
    }

    // Set match tracking
    if(IsPlayerConnected(p1A)) g_PlayerTourneyMatch[p1A] = matchid;
    if(p2A != INVALID_PLAYER_ID && IsPlayerConnected(p2A)) g_PlayerTourneyMatch[p2A] = matchid;
    if(IsPlayerConnected(p1B)) g_PlayerTourneyMatch[p1B] = matchid;
    if(p2B != INVALID_PLAYER_ID && IsPlayerConnected(p2B)) g_PlayerTourneyMatch[p2B] = matchid;

    // Spawn players
    if(IsPlayerConnected(p1A))
    {
        if(GetPlayerState(p1A) == PLAYER_STATE_WASTED) SpawnPlayer(p1A);
        else SpawnTourneyParticipant(p1A, 0, arenaCat, arenaid, vw);
    }
    if(p2A != INVALID_PLAYER_ID && IsPlayerConnected(p2A))
    {
        if(GetPlayerState(p2A) == PLAYER_STATE_WASTED) SpawnPlayer(p2A);
        else SpawnTourneyParticipant(p2A, 0, arenaCat, arenaid, vw);
    }
    if(IsPlayerConnected(p1B))
    {
        if(GetPlayerState(p1B) == PLAYER_STATE_WASTED) SpawnPlayer(p1B);
        else SpawnTourneyParticipant(p1B, 1, arenaCat, arenaid, vw);
    }
    if(p2B != INVALID_PLAYER_ID && IsPlayerConnected(p2B))
    {
        if(GetPlayerState(p2B) == PLAYER_STATE_WASTED) SpawnPlayer(p2B);
        else SpawnTourneyParticipant(p2B, 1, arenaCat, arenaid, vw);
    }

    g_TourneyMatches[matchid][tm_P1_Alive] = (p1A != INVALID_PLAYER_ID && IsPlayerConnected(p1A));
    g_TourneyMatches[matchid][tm_P2_Alive] = (p2A != INVALID_PLAYER_ID && IsPlayerConnected(p2A));
    g_TourneyMatches[matchid][tm_P3_Alive] = (p1B != INVALID_PLAYER_ID && IsPlayerConnected(p1B));
    g_TourneyMatches[matchid][tm_P4_Alive] = (p2B != INVALID_PLAYER_ID && IsPlayerConnected(p2B));

    g_TourneyMatches[matchid][tm_State] = MATCH_STATE_COUNTDOWN;
    g_TourneyMatches[matchid][tm_TimeRemaining] = g_TourneyData[t_RoundTimeLimit];

    new matchArenaName[64];
    GetTourneyArenaDisplayName(arenaCat, arenaid, matchArenaName, sizeof(matchArenaName));

    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Match starting: {MAIN}%s {FFFFFF}VS {MAIN}%s {FFFFFF}(Arena: {23C552}%s{FFFFFF})!",
        nameA, nameB, matchArenaName);

    g_TourneyMatches[matchid][tm_TimerCountdown] = SetTimerEx("OnTourneyCountdownTick", 1000, false, "dd", matchid, TOURNEY_COUNTDOWN_TIME);
    return 1;
}

public OnTourneyCountdownTick(matchid, count)
{
    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES || !g_TourneyMatches[matchid][tm_Active]) return;
    if(g_TourneyMatches[matchid][tm_State] != MATCH_STATE_COUNTDOWN) return;

    new slotA = g_TourneyMatches[matchid][tm_SlotA];
    new slotB = g_TourneyMatches[matchid][tm_SlotB];

    if(count > 0)
    {
        new countStr[32];
        format(countStr, sizeof(countStr), "~r~%d", count);

        if(slotA != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player1]))
            {
                GameTextForPlayer(g_TourneyParticipants[slotA][tp_Player1], countStr, 950, 6);
                PlayerPlaySound(g_TourneyParticipants[slotA][tp_Player1], 1056, 0.0, 0.0, 0.0);
            }
            if(g_TourneyParticipants[slotA][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player2]))
            {
                GameTextForPlayer(g_TourneyParticipants[slotA][tp_Player2], countStr, 950, 6);
                PlayerPlaySound(g_TourneyParticipants[slotA][tp_Player2], 1056, 0.0, 0.0, 0.0);
            }
        }

        if(slotB != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player1]))
            {
                GameTextForPlayer(g_TourneyParticipants[slotB][tp_Player1], countStr, 950, 6);
                PlayerPlaySound(g_TourneyParticipants[slotB][tp_Player1], 1056, 0.0, 0.0, 0.0);
            }
            if(g_TourneyParticipants[slotB][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player2]))
            {
                GameTextForPlayer(g_TourneyParticipants[slotB][tp_Player2], countStr, 950, 6);
                PlayerPlaySound(g_TourneyParticipants[slotB][tp_Player2], 1056, 0.0, 0.0, 0.0);
            }
        }

        g_TourneyMatches[matchid][tm_TimerCountdown] = SetTimerEx("OnTourneyCountdownTick", 1000, false, "dd", matchid, count - 1);
    }
    else
    {
        // FIGHT!
        g_TourneyMatches[matchid][tm_TimerCountdown] = 0;
        g_TourneyMatches[matchid][tm_State] = MATCH_STATE_ACTIVE;

        if(slotA != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player1]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotA][tp_Player1], true);
                GameTextForPlayer(g_TourneyParticipants[slotA][tp_Player1], "~g~FIGHT!", 1500, 6);
                PlayerPlaySound(g_TourneyParticipants[slotA][tp_Player1], 1057, 0.0, 0.0, 0.0);
            }
            if(g_TourneyParticipants[slotA][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player2]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotA][tp_Player2], true);
                GameTextForPlayer(g_TourneyParticipants[slotA][tp_Player2], "~g~FIGHT!", 1500, 6);
                PlayerPlaySound(g_TourneyParticipants[slotA][tp_Player2], 1057, 0.0, 0.0, 0.0);
            }
        }

        if(slotB != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player1]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotB][tp_Player1], true);
                GameTextForPlayer(g_TourneyParticipants[slotB][tp_Player1], "~g~FIGHT!", 1500, 6);
                PlayerPlaySound(g_TourneyParticipants[slotB][tp_Player1], 1057, 0.0, 0.0, 0.0);
            }
            if(g_TourneyParticipants[slotB][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player2]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotB][tp_Player2], true);
                GameTextForPlayer(g_TourneyParticipants[slotB][tp_Player2], "~g~FIGHT!", 1500, 6);
                PlayerPlaySound(g_TourneyParticipants[slotB][tp_Player2], 1057, 0.0, 0.0, 0.0);
            }
        }

        g_TourneyMatches[matchid][tm_TimerMatch] = SetTimerEx("OnTourneyMatchTimer", 1000, true, "d", matchid);
    }
}

public OnTourneyMatchTimer(matchid)
{
    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES || !g_TourneyMatches[matchid][tm_Active]) return;
    if(g_TourneyMatches[matchid][tm_State] != MATCH_STATE_ACTIVE) return;

    if(g_TourneyMatches[matchid][tm_TimeRemaining] > 0)
    {
        g_TourneyMatches[matchid][tm_TimeRemaining]--;
    }
    else
    {
        new slotA = g_TourneyMatches[matchid][tm_SlotA];
        new slotB = g_TourneyMatches[matchid][tm_SlotB];

        new Float:hpA = 0.0, Float:hpB = 0.0, Float:curHp, Float:curArm;

        if(slotA != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player1]))
            {
                GetPlayerHealth(g_TourneyParticipants[slotA][tp_Player1], curHp);
                GetPlayerArmour(g_TourneyParticipants[slotA][tp_Player1], curArm);
                hpA += (curHp + curArm);
            }
            if(g_TourneyParticipants[slotA][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player2]))
            {
                GetPlayerHealth(g_TourneyParticipants[slotA][tp_Player2], curHp);
                GetPlayerArmour(g_TourneyParticipants[slotA][tp_Player2], curArm);
                hpA += (curHp + curArm);
            }
        }

        if(slotB != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player1]))
            {
                GetPlayerHealth(g_TourneyParticipants[slotB][tp_Player1], curHp);
                GetPlayerArmour(g_TourneyParticipants[slotB][tp_Player1], curArm);
                hpB += (curHp + curArm);
            }
            if(g_TourneyParticipants[slotB][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player2]))
            {
                GetPlayerHealth(g_TourneyParticipants[slotB][tp_Player2], curHp);
                GetPlayerArmour(g_TourneyParticipants[slotB][tp_Player2], curArm);
                hpB += (curHp + curArm);
            }
        }

        if(hpA >= hpB)
        {
            FinishTourneyMatch(matchid, slotA);
        }
        else
        {
            FinishTourneyMatch(matchid, slotB);
        }
    }
}

stock ProcessTourneyPlayerDeath(victimid, killerid, reason)
{
    #pragma unused reason
    if(!IsPlayerInTournament(victimid)) return 0;

    new matchid = g_PlayerTourneyMatch[victimid];
    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES || !g_TourneyMatches[matchid][tm_Active]) return 0;
    if(g_TourneyMatches[matchid][tm_State] != MATCH_STATE_ACTIVE) return 0;

    new slotA = g_TourneyMatches[matchid][tm_SlotA];
    new slotB = g_TourneyMatches[matchid][tm_SlotB];

    new victimSlot = g_PlayerTourneySlot[victimid];
    if(victimSlot == slotA)
    {
        if(victimid == g_TourneyParticipants[slotA][tp_Player1]) g_TourneyMatches[matchid][tm_P1_Alive] = false;
        if(victimid == g_TourneyParticipants[slotA][tp_Player2]) g_TourneyMatches[matchid][tm_P2_Alive] = false;
    }
    else if(victimSlot == slotB)
    {
        if(victimid == g_TourneyParticipants[slotB][tp_Player1]) g_TourneyMatches[matchid][tm_P3_Alive] = false;
        if(victimid == g_TourneyParticipants[slotB][tp_Player2]) g_TourneyMatches[matchid][tm_P4_Alive] = false;
    }

    g_TourneyParticipants[victimSlot][tp_Deaths]++;
    if(killerid != INVALID_PLAYER_ID && IsPlayerInTournament(killerid))
    {
        new killerSlot = g_PlayerTourneySlot[killerid];
        g_TourneyParticipants[killerSlot][tp_Kills]++;
    }

    new bool:teamA_Alive = (g_TourneyMatches[matchid][tm_P1_Alive] || g_TourneyMatches[matchid][tm_P2_Alive]);
    new bool:teamB_Alive = (g_TourneyMatches[matchid][tm_P3_Alive] || g_TourneyMatches[matchid][tm_P4_Alive]);

    if(!teamA_Alive && teamB_Alive)
    {
        g_TourneyMatches[matchid][tm_ScoreB]++;
        if(g_TourneyMatches[matchid][tm_ScoreB] >= (g_TourneyData[t_RoundsPerMatch] / 2 + 1))
        {
            FinishTourneyMatch(matchid, slotB);
        }
        else
        {
            StartTourneyMatch(matchid);
        }
    }
    else if(teamA_Alive && !teamB_Alive)
    {
        g_TourneyMatches[matchid][tm_ScoreA]++;
        if(g_TourneyMatches[matchid][tm_ScoreA] >= (g_TourneyData[t_RoundsPerMatch] / 2 + 1))
        {
            FinishTourneyMatch(matchid, slotA);
        }
        else
        {
            StartTourneyMatch(matchid);
        }
    }

    return 1;
}

stock FinishTourneyMatch(matchid, winnerSlot, bool:isWO = false)
{
    if(matchid < 0 || matchid >= MAX_TOURNEY_MATCHES) return 0;

    if(g_TourneyMatches[matchid][tm_TimerMatch] != 0)
    {
        KillTimer(g_TourneyMatches[matchid][tm_TimerMatch]);
        g_TourneyMatches[matchid][tm_TimerMatch] = 0;
    }
    if(g_TourneyMatches[matchid][tm_TimerCountdown] != 0)
    {
        KillTimer(g_TourneyMatches[matchid][tm_TimerCountdown]);
        g_TourneyMatches[matchid][tm_TimerCountdown] = 0;
    }
    if(g_TourneyMatches[matchid][tm_TimerWO] != 0)
    {
        KillTimer(g_TourneyMatches[matchid][tm_TimerWO]);
        g_TourneyMatches[matchid][tm_TimerWO] = 0;
    }

    g_TourneyMatches[matchid][tm_State] = MATCH_STATE_FINISHED;
    g_TourneyMatches[matchid][tm_WinnerSlot] = winnerSlot;

    new slotA = g_TourneyMatches[matchid][tm_SlotA];
    new slotB = g_TourneyMatches[matchid][tm_SlotB];

    new loserSlot = -1;

    if(winnerSlot == -1)
    {
        if(slotA != -1 && slotA < MAX_TOURNEY_SLOTS) g_TourneyParticipants[slotA][tp_Eliminated] = true;
        if(slotB != -1 && slotB < MAX_TOURNEY_SLOTS) g_TourneyParticipants[slotB][tp_Eliminated] = true;
    }
    else
    {
        loserSlot = (winnerSlot == slotA) ? slotB : slotA;

        if(loserSlot != -1 && loserSlot < MAX_TOURNEY_SLOTS)
        {
            g_TourneyParticipants[loserSlot][tp_Eliminated] = true;
        }
    }

    if(winnerSlot != -1 && winnerSlot < MAX_TOURNEY_SLOTS)
    {
        g_TourneyParticipants[winnerSlot][tp_Wins]++;
    }

    new winnerName[64], loserName[64];
    GetParticipantDisplayName(winnerSlot, winnerName, sizeof(winnerName));
    GetParticipantDisplayName(loserSlot, loserName, sizeof(loserName));

    if(isWO)
    {
        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Match finished: {MAIN}%s {FFFFFF}won against {MAIN}%s {FFFFFF}by W.O.!", winnerName, loserName);
    }
    else
    {
        va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}Match finished: {MAIN}%s {FFFFFF}defeated {MAIN}%s{FFFFFF}!", winnerName, loserName);
    }

    new pList[4];
    pList[0] = (slotA != -1) ? g_TourneyParticipants[slotA][tp_Player1] : INVALID_PLAYER_ID;
    pList[1] = (slotA != -1) ? g_TourneyParticipants[slotA][tp_Player2] : INVALID_PLAYER_ID;
    pList[2] = (slotB != -1) ? g_TourneyParticipants[slotB][tp_Player1] : INVALID_PLAYER_ID;
    pList[3] = (slotB != -1) ? g_TourneyParticipants[slotB][tp_Player2] : INVALID_PLAYER_ID;

    for(new i = 0; i < 4; i++)
    {
        new pid = pList[i];
        if(pid != INVALID_PLAYER_ID && IsPlayerConnected(pid) && User[pid][isLogged])
        {
            g_PlayerTourneyMatch[pid] = -1;
            ResetPlayerWeapons(pid);
            SetPlayerHealth(pid, 100.0);
            SetPlayerArmour(pid, 100.0);
            setAfk(pid);
        }
    }

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && User[i][isLogged] && g_PlayerTourneySpectating[i] == matchid)
        {
            g_PlayerTourneySpectating[i] = -1;
            StopSpectate(i);
            SendClientMessage(i, -1, "{MAIN}[TOURNAMENT] {FFFFFF}The tournament match you were spectating has finished.");
        }
    }

    CheckTournamentRoundFinished();
    return 1;
}

stock CheckTournamentRoundFinished()
{
    new curRound = g_TourneyData[t_CurrentRound];
    new bool:roundDone = true;

    for(new m = 0; m < MAX_TOURNEY_MATCHES; m++)
    {
        if(g_TourneyMatches[m][tm_Active] && g_TourneyMatches[m][tm_Round] == curRound)
        {
            if(g_TourneyMatches[m][tm_State] != MATCH_STATE_FINISHED)
            {
                roundDone = false;
                break;
            }
        }
    }

    if(roundDone)
    {
        if(curRound >= g_TourneyData[t_TotalRounds])
        {
            for(new m = 0; m < MAX_TOURNEY_MATCHES; m++)
            {
                if(g_TourneyMatches[m][tm_Active] && g_TourneyMatches[m][tm_Round] == curRound)
                {
                    DeclareTournamentChampion(g_TourneyMatches[m][tm_WinnerSlot]);
                    break;
                }
            }
        }
        else
        {
            SetTimerEx("StartNextTourneyRoundDelayed", 5000, false, "d", curRound + 1);
        }
    }
}

public StartNextTourneyRoundDelayed(round)
{
    new prevRound = round - 1;
    new winners[MAX_TOURNEY_SLOTS], winnerCount = 0;

    for(new m = 0; m < MAX_TOURNEY_MATCHES; m++)
    {
        if(g_TourneyMatches[m][tm_Active] && g_TourneyMatches[m][tm_Round] == prevRound)
        {
            if(g_TourneyMatches[m][tm_WinnerSlot] != -1)
            {
                winners[winnerCount++] = g_TourneyMatches[m][tm_WinnerSlot];
            }
        }
    }

    if(winnerCount <= 1)
    {
        if(winnerCount == 1) DeclareTournamentChampion(winners[0]);
        else ResetTournament();
        return;
    }

    // Pair next round matches
    new matchCount = winnerCount / 2;
    for(new m = 0; m < matchCount; m++)
    {
        new nextMatchIdx = -1;
        for(new i = 0; i < MAX_TOURNEY_MATCHES; i++)
        {
            if(!g_TourneyMatches[i][tm_Active])
            {
                nextMatchIdx = i;
                break;
            }
        }

        if(nextMatchIdx == -1) break;

        new slotA = winners[m * 2];
        new slotB = (m * 2 + 1 < winnerCount) ? winners[m * 2 + 1] : -1;

        g_TourneyMatches[nextMatchIdx][tm_Active] = true;
        g_TourneyMatches[nextMatchIdx][tm_Round] = round;
        g_TourneyMatches[nextMatchIdx][tm_MatchIndex] = nextMatchIdx;
        g_TourneyMatches[nextMatchIdx][tm_SlotA] = slotA;
        g_TourneyMatches[nextMatchIdx][tm_SlotB] = slotB;
        g_TourneyMatches[nextMatchIdx][tm_State] = MATCH_STATE_PREPARING;
        g_TourneyMatches[nextMatchIdx][tm_VirtualWorld] = TOURNEY_VW_BASE + nextMatchIdx;

        PickTourneyMatchArena(nextMatchIdx);

        if(slotA != -1 && slotB == -1)
        {
            g_TourneyMatches[nextMatchIdx][tm_WinnerSlot] = slotA;
            g_TourneyMatches[nextMatchIdx][tm_State] = MATCH_STATE_FINISHED;
        }
    }

    StartTournamentRound(round);
}

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
    if(IsPlayerConnected(p1) && User[p1][isLogged])
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

    if(p2 != INVALID_PLAYER_ID && IsPlayerConnected(p2) && User[p2][isLogged])
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

                if(IsPlayerConnected(p1) && User[p1][isLogged])
                {
                    GivePlayerMoney(p1, g_TourneyData[t_EntryFee]);
                    User[p1][playerMoney] += g_TourneyData[t_EntryFee];
                    SendClientMessage(p1, -1, "{MAIN}[TOURNAMENT] {FFFFFF}Your registration fee was refunded.");
                }
                if(p2 != INVALID_PLAYER_ID && IsPlayerConnected(p2) && User[p2][isLogged])
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
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return 0;

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
        if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
            targetid = g_TourneyParticipants[slotA][tp_Player2];
    }

    if(targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        new slotB = g_TourneyMatches[matchid][tm_SlotB];
        if(slotB != -1 && g_TourneyParticipants[slotB][tp_Registered])
        {
            targetid = g_TourneyParticipants[slotB][tp_Player1];
            if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
                targetid = g_TourneyParticipants[slotB][tp_Player2];
        }
    }

    if(targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}No connected players in this match to spectate.");
        return 0;
    }

    g_PlayerTourneySpectating[playerid] = matchid;
    setPlayerSpectate(playerid, targetid);

    va_SendClientMessage(playerid, -1, "{MAIN}[SPECTATE] {FFFFFF}Now spectating Tournament Match #%d. Type {MAIN}/specoff {FFFFFF}to exit.", matchid + 1);
    return 1;
}

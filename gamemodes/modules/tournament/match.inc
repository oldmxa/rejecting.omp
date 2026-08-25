#if defined _tourney_match_included
    #endinput
#endif
#define _tourney_match_included

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
    if(!IsPlayerLogged(playerid)) return 0;

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
    new bool:teamA_Present = (IsPlayerLogged(p1A))
        || (p2A != INVALID_PLAYER_ID && IsPlayerLogged(p2A));
    new bool:teamB_Present = (IsPlayerLogged(p1B))
        || (p2B != INVALID_PLAYER_ID && IsPlayerLogged(p2B));

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
                PlaySuccessSound(g_TourneyParticipants[slotA][tp_Player1]);
            }
            if(g_TourneyParticipants[slotA][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotA][tp_Player2]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotA][tp_Player2], true);
                GameTextForPlayer(g_TourneyParticipants[slotA][tp_Player2], "~g~FIGHT!", 1500, 6);
                PlaySuccessSound(g_TourneyParticipants[slotA][tp_Player2]);
            }
        }

        if(slotB != -1)
        {
            if(IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player1]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotB][tp_Player1], true);
                GameTextForPlayer(g_TourneyParticipants[slotB][tp_Player1], "~g~FIGHT!", 1500, 6);
                PlaySuccessSound(g_TourneyParticipants[slotB][tp_Player1]);
            }
            if(g_TourneyParticipants[slotB][tp_Player2] != INVALID_PLAYER_ID && IsPlayerConnected(g_TourneyParticipants[slotB][tp_Player2]))
            {
                TogglePlayerControllableEx(g_TourneyParticipants[slotB][tp_Player2], true);
                GameTextForPlayer(g_TourneyParticipants[slotB][tp_Player2], "~g~FIGHT!", 1500, 6);
                PlaySuccessSound(g_TourneyParticipants[slotB][tp_Player2]);
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
        if(pid != INVALID_PLAYER_ID && IsPlayerLogged(pid))
        {
            g_PlayerTourneyMatch[pid] = -1;
            ResetPlayerCombat(pid, 100.0, 100.0);
            setAfk(pid);
        }
    }

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerLogged(i) && g_PlayerTourneySpectating[i] == matchid)
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

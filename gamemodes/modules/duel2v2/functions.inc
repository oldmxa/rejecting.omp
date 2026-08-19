#if defined _duel2v2_functions_included
    #endinput
#endif
#define _duel2v2_functions_included

stock bool:IsValidDuel2v2Player(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    return (IsPlayerConnected(playerid)) ? true : false;
}

stock GetDuel2v2Players(matchid, dest[4])
{
    dest[0] = g_Duel2v2Data[matchid][d2_TeamA_Player1];
    dest[1] = g_Duel2v2Data[matchid][d2_TeamA_Player2];
    dest[2] = g_Duel2v2Data[matchid][d2_TeamB_Player1];
    dest[3] = g_Duel2v2Data[matchid][d2_TeamB_Player2];
    return 1;
}

stock SendDuel2v2Message(matchid, const message[], exceptid = INVALID_PLAYER_ID)
{
    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(players[i] == exceptid) continue;
        if(!IsValidDuel2v2Player(players[i])) continue;
        SendClientMessage(players[i], -1, "%s", message);
    }
    return 1;
}

stock FindFreeDuel2v2Slot()
{
    for(new i = 0; i < MAX_DUELS_2V2; i++)
    {
        if(!g_Duel2v2Data[i][d2_Active]) return i;
    }
    return -1;
}

stock KillDuel2v2Timers(matchid)
{
    if(g_Duel2v2Data[matchid][d2_CountdownTimer] != 0)
    {
        KillTimer(g_Duel2v2Data[matchid][d2_CountdownTimer]);
        g_Duel2v2Data[matchid][d2_CountdownTimer] = 0;
    }
    if(g_Duel2v2Data[matchid][d2_MatchTimer] != 0)
    {
        KillTimer(g_Duel2v2Data[matchid][d2_MatchTimer]);
        g_Duel2v2Data[matchid][d2_MatchTimer] = 0;
    }
    if(g_Duel2v2Data[matchid][d2_InviteExpireTimer] != 0)
    {
        KillTimer(g_Duel2v2Data[matchid][d2_InviteExpireTimer]);
        g_Duel2v2Data[matchid][d2_InviteExpireTimer] = 0;
    }
    return 1;
}

stock ResetDuel2v2Data(matchid)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2) return 0;

    KillDuel2v2Timers(matchid);

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        new pid = players[i];
        if(pid < 0 || pid >= MAX_PLAYERS) continue;
        if(g_PlayerDuel2v2ID[pid] != matchid) continue;

        g_PlayerDuel2v2ID[pid] = -1;
        g_PlayerDuel2v2Team[pid] = -1;
        DestroyDuel2v2PlayerTextDraws(pid);
    }

    g_Duel2v2Data[matchid][d2_Active] = false;
    g_Duel2v2Data[matchid][d2_State] = DUEL2V2_STATE_NONE;
    g_Duel2v2Data[matchid][d2_ArenaID] = 0;
    g_Duel2v2Data[matchid][d2_WeaponSet] = 0;
    g_Duel2v2Data[matchid][d2_MaxRounds] = 1;
    g_Duel2v2Data[matchid][d2_CurrentRound] = 1;

    g_Duel2v2Data[matchid][d2_TeamA_Player1] = INVALID_PLAYER_ID;
    g_Duel2v2Data[matchid][d2_TeamA_Player2] = INVALID_PLAYER_ID;
    g_Duel2v2Data[matchid][d2_TeamA_P1_Accepted] = false;
    g_Duel2v2Data[matchid][d2_TeamA_P2_Accepted] = false;
    g_Duel2v2Data[matchid][d2_TeamA_P1_Alive] = false;
    g_Duel2v2Data[matchid][d2_TeamA_P2_Alive] = false;
    g_Duel2v2Data[matchid][d2_TeamA_Score] = 0;

    g_Duel2v2Data[matchid][d2_TeamB_Player1] = INVALID_PLAYER_ID;
    g_Duel2v2Data[matchid][d2_TeamB_Player2] = INVALID_PLAYER_ID;
    g_Duel2v2Data[matchid][d2_TeamB_P1_Accepted] = false;
    g_Duel2v2Data[matchid][d2_TeamB_P2_Accepted] = false;
    g_Duel2v2Data[matchid][d2_TeamB_P1_Alive] = false;
    g_Duel2v2Data[matchid][d2_TeamB_P2_Alive] = false;
    g_Duel2v2Data[matchid][d2_TeamB_Score] = 0;

    g_Duel2v2Data[matchid][d2_VirtualWorld] = DUEL2V2_VW_BASE + matchid;
    return 1;
}

stock CreateDuel2v2Challenge(creatorid, teammateid, opp1, opp2, arenaid, weaponset, max_rounds)
{
    if(!IsValidDuel2v2Player(creatorid) || !IsValidDuel2v2Player(teammateid) ||
       !IsValidDuel2v2Player(opp1) || !IsValidDuel2v2Player(opp2)) return -1;

    if(arenaid < 0 || arenaid >= sizeof(DuelS_ArenaNames)) return -1;
    if(weaponset < 0 || weaponset >= sizeof(g_Duel2v2WeaponSets)) return -1;
    if(max_rounds != 1 && max_rounds != 3 && max_rounds != 5 && max_rounds != 7) return -1;

    new slot = FindFreeDuel2v2Slot();
    if(slot == -1)
    {
        SendClientMessage(creatorid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Server 2v2 arena limit reached. Try again later.");
        return -1;
    }

    ResetDuel2v2Data(slot);

    g_Duel2v2Data[slot][d2_Active] = true;
    g_Duel2v2Data[slot][d2_State] = DUEL2V2_STATE_PENDING;
    g_Duel2v2Data[slot][d2_ArenaID] = arenaid;
    g_Duel2v2Data[slot][d2_WeaponSet] = weaponset;
    g_Duel2v2Data[slot][d2_MaxRounds] = max_rounds;
    g_Duel2v2Data[slot][d2_CurrentRound] = 1;

    g_Duel2v2Data[slot][d2_TeamA_Player1] = creatorid;
    g_Duel2v2Data[slot][d2_TeamA_Player2] = teammateid;
    g_Duel2v2Data[slot][d2_TeamA_P1_Accepted] = true; // Creator auto accepts

    g_Duel2v2Data[slot][d2_TeamB_Player1] = opp1;
    g_Duel2v2Data[slot][d2_TeamB_Player2] = opp2;

    g_PlayerDuel2v2ID[creatorid]  = slot; g_PlayerDuel2v2Team[creatorid]  = 0;
    g_PlayerDuel2v2ID[teammateid] = slot; g_PlayerDuel2v2Team[teammateid] = 0;
    g_PlayerDuel2v2ID[opp1]       = slot; g_PlayerDuel2v2Team[opp1]       = 1;
    g_PlayerDuel2v2ID[opp2]       = slot; g_PlayerDuel2v2Team[opp2]       = 1;

    g_Duel2v2Data[slot][d2_InviteExpireTimer] = SetTimerEx("OnDuel2v2InviteExpired", DUEL2V2_INVITE_EXPIRE * 1000, false, "i", slot);

    new msg[192];
    format(msg, sizeof(msg),
        "{MAIN}[DUEL 2v2] {FFFFFF}%s challenges you to a 2v2 duel on {MAIN}'%s'{FFFFFF}! Type {MAIN}/accept2v2{FFFFFF} to accept.",
        getPlayerName(creatorid), DuelS_ArenaNames[arenaid]);

    SendDuel2v2Message(slot, msg, creatorid);
    va_SendClientMessage(creatorid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}Invitation sent to %s, %s, and %s!",
        getPlayerName(teammateid), getPlayerName(opp1), getPlayerName(opp2));
    return slot;
}

stock bool:HasAcceptedDuel2v2(matchid, playerid)
{
    if (matchid < 0 || matchid >= MAX_DUELS_2V2) return false;

    if (playerid == g_Duel2v2Data[matchid][d2_TeamA_Player1]) return true;
    if (playerid == g_Duel2v2Data[matchid][d2_TeamA_Player2]) return g_Duel2v2Data[matchid][d2_TeamA_P2_Accepted];
    if (playerid == g_Duel2v2Data[matchid][d2_TeamB_Player1]) return g_Duel2v2Data[matchid][d2_TeamB_P1_Accepted];
    if (playerid == g_Duel2v2Data[matchid][d2_TeamB_Player2]) return g_Duel2v2Data[matchid][d2_TeamB_P2_Accepted];

    return false;
}

stock AcceptDuel2v2Challenge(playerid)
{
    new slot = g_PlayerDuel2v2ID[playerid];
    if(slot == -1 || !g_Duel2v2Data[slot][d2_Active] || g_Duel2v2Data[slot][d2_State] != DUEL2V2_STATE_PENDING)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You do not have any pending 2v2 duel invitations.");
    }

    if(User[playerid][isSpectating] || User[playerid][isAfk] ||
       GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE ||
       g_PlayerTraining[playerid][pTrain_Active] || g_Practice[playerid][pracActive])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You are busy and cannot accept the 2v2 duel right now.");
    }

    if(playerid == g_Duel2v2Data[slot][d2_TeamA_Player1])
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You created this duel - waiting for the other players to accept.");
    }

    new bool:alreadyAccepted = false;
    if(playerid == g_Duel2v2Data[slot][d2_TeamA_Player2])
    {
        alreadyAccepted = g_Duel2v2Data[slot][d2_TeamA_P2_Accepted];
        g_Duel2v2Data[slot][d2_TeamA_P2_Accepted] = true;
    }
    else if(playerid == g_Duel2v2Data[slot][d2_TeamB_Player1])
    {
        alreadyAccepted = g_Duel2v2Data[slot][d2_TeamB_P1_Accepted];
        g_Duel2v2Data[slot][d2_TeamB_P1_Accepted] = true;
    }
    else if(playerid == g_Duel2v2Data[slot][d2_TeamB_Player2])
    {
        alreadyAccepted = g_Duel2v2Data[slot][d2_TeamB_P2_Accepted];
        g_Duel2v2Data[slot][d2_TeamB_P2_Accepted] = true;
    }
    else
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You do not have any pending 2v2 duel invitations.");
    }

    if(alreadyAccepted)
    {
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You already accepted this 2v2 duel.");
    }

    SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You accepted the 2v2 duel invitation!");

    new broadcastMsg[128];
    format(broadcastMsg, sizeof(broadcastMsg), "{MAIN}[DUEL 2v2] {FFFFFF}%s accepted the 2v2 challenge!", getPlayerName(playerid));
    SendDuel2v2Message(slot, broadcastMsg, playerid);

    if(g_Duel2v2Data[slot][d2_TeamA_P1_Accepted] && g_Duel2v2Data[slot][d2_TeamA_P2_Accepted] &&
       g_Duel2v2Data[slot][d2_TeamB_P1_Accepted] && g_Duel2v2Data[slot][d2_TeamB_P2_Accepted])
    {
        if(g_Duel2v2Data[slot][d2_InviteExpireTimer] != 0)
        {
            KillTimer(g_Duel2v2Data[slot][d2_InviteExpireTimer]);
            g_Duel2v2Data[slot][d2_InviteExpireTimer] = 0;
        }
        StartDuel2v2Round(slot);
    }
    return 1;
}

public OnDuel2v2InviteExpired(matchid)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    g_Duel2v2Data[matchid][d2_InviteExpireTimer] = 0;
    if(g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_PENDING) return 0;

    CancelDuel2v2Session(matchid, "Invitation expired (not all players accepted in time).");
    return 1;
}

stock ApplyDuel2v2RoundSpawn(playerid)
{
    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2) return 0;

    new team = g_PlayerDuel2v2Team[playerid];
    if(team != 0 && team != 1) return 0;

    new arenaid = g_Duel2v2Data[matchid][d2_ArenaID];
    if(arenaid < 0 || arenaid >= sizeof(DuelS_ArenaNames)) return 0;

    ResetPlayerWeapons(playerid);
    SetPlayerVirtualWorld(playerid, g_Duel2v2Data[matchid][d2_VirtualWorld]);
    SetPlayerInterior(playerid, DuelS_ArenaInteriors[arenaid]);

    new Float:x = DuelS_Pos[arenaid][team][0];
    new Float:y = DuelS_Pos[arenaid][team][1];
    new Float:z = DuelS_Pos[arenaid][team][2];
    new Float:a = DuelS_Pos[arenaid][team][3];

    if(playerid == g_Duel2v2Data[matchid][d2_TeamA_Player2] ||
       playerid == g_Duel2v2Data[matchid][d2_TeamB_Player2])
    {
        x += 1.5;
        y += 1.5;
    }

    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, a);
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);
    TogglePlayerControllable(playerid, false);

    new wset = g_Duel2v2Data[matchid][d2_WeaponSet];
    if(wset < 0 || wset >= sizeof(g_Duel2v2WeaponSets)) wset = 0;

    if(g_Duel2v2WeaponSets[wset][d2w_W1] > 0) GivePlayerWeapon(playerid, WEAPON:g_Duel2v2WeaponSets[wset][d2w_W1], g_Duel2v2WeaponSets[wset][d2w_A1]);
    if(g_Duel2v2WeaponSets[wset][d2w_W2] > 0) GivePlayerWeapon(playerid, WEAPON:g_Duel2v2WeaponSets[wset][d2w_W2], g_Duel2v2WeaponSets[wset][d2w_A2]);
    if(g_Duel2v2WeaponSets[wset][d2w_W3] > 0) GivePlayerWeapon(playerid, WEAPON:g_Duel2v2WeaponSets[wset][d2w_W3], g_Duel2v2WeaponSets[wset][d2w_A3]);

    CreateDuel2v2PlayerTextDraws(playerid);
    UpdateDuel2v2ScoreTextDraw(matchid);
    return 1;
}

stock ApplyDuel2v2DeadSpawn(playerid)
{
    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2) return 0;

    new team = g_PlayerDuel2v2Team[playerid];
    if(team != 0 && team != 1) return 0;

    new arenaid = g_Duel2v2Data[matchid][d2_ArenaID];
    if(arenaid < 0 || arenaid >= sizeof(DuelS_ArenaNames)) return 0;

    ResetPlayerWeapons(playerid);
    SetPlayerVirtualWorld(playerid, g_Duel2v2Data[matchid][d2_VirtualWorld]);
    SetPlayerInterior(playerid, DuelS_ArenaInteriors[arenaid]);
    SetPlayerPos(playerid,
        DuelS_Pos[arenaid][team][0],
        DuelS_Pos[arenaid][team][1],
        DuelS_Pos[arenaid][team][2] + DUEL2V2_DEAD_Z_OFFSET);
    TogglePlayerControllable(playerid, false);
    return 1;
}

stock UpdateDuel2v2ScoreTextDraw(matchid)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2) return 0;

    new str[128];
    format(str, sizeof(str), "~w~DUEL 2v2 (Rd %d/%d)~n~~b~~h~Team A: %d ~w~: ~r~~h~Team B: %d",
        g_Duel2v2Data[matchid][d2_CurrentRound],
        g_Duel2v2Data[matchid][d2_MaxRounds],
        g_Duel2v2Data[matchid][d2_TeamA_Score],
        g_Duel2v2Data[matchid][d2_TeamB_Score]);

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        UpdateDuel2v2TextDrawString(players[i], str);
    }
    return 1;
}

stock StartDuel2v2Round(matchid)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i]))
        {
            CancelDuel2v2Session(matchid, "A participant is no longer available.");
            return 0;
        }
    }

    g_Duel2v2Data[matchid][d2_State] = DUEL2V2_STATE_PREPARING;
    g_Duel2v2Data[matchid][d2_TeamA_P1_Alive] = true;
    g_Duel2v2Data[matchid][d2_TeamA_P2_Alive] = true;
    g_Duel2v2Data[matchid][d2_TeamB_P1_Alive] = true;
    g_Duel2v2Data[matchid][d2_TeamB_P2_Alive] = true;

    for(new i = 0; i < 4; i++)
    {
        SpawnPlayerEx(players[i]);
    }

    UpdateDuel2v2ScoreTextDraw(matchid);

    g_Duel2v2Data[matchid][d2_CountdownTimer] = 0;
    OnDuel2v2CountdownTick(matchid, DUEL2V2_COUNTDOWN);
    return 1;
}

public OnDuel2v2CountdownTick(matchid, count)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    g_Duel2v2Data[matchid][d2_CountdownTimer] = 0;

    if(g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_PREPARING) return 0;

    new players[4];
    GetDuel2v2Players(matchid, players);

    new strText[32];
    if(count > 0)
    {
        format(strText, sizeof(strText), "~w~%d...", count);

        for(new i = 0; i < 4; i++)
        {
            if(!IsValidDuel2v2Player(players[i])) continue;
            PlayerPlaySound(players[i], 1056, 0.0, 0.0, 0.0);
            GameTextForPlayer(players[i], strText, 1000, 6);
        }

        g_Duel2v2Data[matchid][d2_CountdownTimer] = SetTimerEx("OnDuel2v2CountdownTick", 1000, false, "ii", matchid, count - 1);
        return 1;
    }

    format(strText, sizeof(strText), "~g~~h~FIGHT!");

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        PlayerPlaySound(players[i], 1057, 0.0, 0.0, 0.0);
        GameTextForPlayer(players[i], strText, 1500, 6);
        TogglePlayerControllable(players[i], true);
    }

    g_Duel2v2Data[matchid][d2_State] = DUEL2V2_STATE_ACTIVE;
    return 1;
}

stock OnDuel2v2DeathCheck(victimid, killerid)
{
    #pragma unused killerid

    new matchid = g_PlayerDuel2v2ID[victimid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    new matchState = g_Duel2v2Data[matchid][d2_State];
    if(matchState != DUEL2V2_STATE_ACTIVE && matchState != DUEL2V2_STATE_PREPARING) return 0;

    if(victimid == g_Duel2v2Data[matchid][d2_TeamA_Player1])      g_Duel2v2Data[matchid][d2_TeamA_P1_Alive] = false;
    else if(victimid == g_Duel2v2Data[matchid][d2_TeamA_Player2]) g_Duel2v2Data[matchid][d2_TeamA_P2_Alive] = false;
    else if(victimid == g_Duel2v2Data[matchid][d2_TeamB_Player1]) g_Duel2v2Data[matchid][d2_TeamB_P1_Alive] = false;
    else if(victimid == g_Duel2v2Data[matchid][d2_TeamB_Player2]) g_Duel2v2Data[matchid][d2_TeamB_P2_Alive] = false;
    else return 0;

    new bool:teamA_wiped = (!g_Duel2v2Data[matchid][d2_TeamA_P1_Alive] && !g_Duel2v2Data[matchid][d2_TeamA_P2_Alive]);
    new bool:teamB_wiped = (!g_Duel2v2Data[matchid][d2_TeamB_P1_Alive] && !g_Duel2v2Data[matchid][d2_TeamB_P2_Alive]);

    if(teamA_wiped && teamB_wiped)
    {
        EndDuel2v2Round(matchid, -1);
    }
    else if(teamA_wiped)
    {
        EndDuel2v2Round(matchid, 1);
    }
    else if(teamB_wiped)
    {
        EndDuel2v2Round(matchid, 0);
    }
    return 1;
}

stock EndDuel2v2Round(matchid, winning_team)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    g_Duel2v2Data[matchid][d2_State] = DUEL2V2_STATE_ROUND_END;

    if(g_Duel2v2Data[matchid][d2_CountdownTimer] != 0)
    {
        KillTimer(g_Duel2v2Data[matchid][d2_CountdownTimer]);
        g_Duel2v2Data[matchid][d2_CountdownTimer] = 0;
    }

    new players[4];
    GetDuel2v2Players(matchid, players);
    for(new i = 0; i < 4; i++)
    {
        if(IsValidDuel2v2Player(players[i]))
        {
            TogglePlayerControllable(players[i], false);
        }
    }

    if(winning_team == 0)      g_Duel2v2Data[matchid][d2_TeamA_Score]++;
    else if(winning_team == 1) g_Duel2v2Data[matchid][d2_TeamB_Score]++;

    UpdateDuel2v2ScoreTextDraw(matchid);

    new requiredWins = (g_Duel2v2Data[matchid][d2_MaxRounds] / 2) + 1;

    if(g_Duel2v2Data[matchid][d2_TeamA_Score] >= requiredWins)
    {
        FinishDuel2v2Match(matchid, 0);
    }
    else if(g_Duel2v2Data[matchid][d2_TeamB_Score] >= requiredWins)
    {
        FinishDuel2v2Match(matchid, 1);
    }
    else
    {
        if(winning_team != -1) g_Duel2v2Data[matchid][d2_CurrentRound]++;
        g_Duel2v2Data[matchid][d2_MatchTimer] = SetTimerEx("StartNextRoundTimer", DUEL2V2_ROUND_DELAY, false, "i", matchid);
    }
    return 1;
}

public StartNextRoundTimer(matchid)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2) return 0;

    g_Duel2v2Data[matchid][d2_MatchTimer] = 0;

    if(!g_Duel2v2Data[matchid][d2_Active]) return 0;
    if(g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_ROUND_END) return 0;

    StartDuel2v2Round(matchid);
    return 1;
}

stock AwardDuel2v2Result(matchid, winning_team)
{
    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        new pid = players[i];
        if(pid < 0 || pid >= MAX_PLAYERS) continue;

        new team = (i < 2) ? 0 : 1;
        if(team == winning_team)
        {
            User[pid][duel2v2Wins]++;
            if(IsPlayerConnected(pid))
            {
                GivePlayerMoney(pid, DUEL2V2_WIN_REWARD);
                User[pid][playerMoney] += DUEL2V2_WIN_REWARD;
            }
        }
        else
        {
            User[pid][duel2v2Losses]++;
        }

        if(IsPlayerConnected(pid)) UpdatePlayerData(pid);
    }
    return 1;
}

stock FinishDuel2v2Match(matchid, winning_team)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    g_Duel2v2Data[matchid][d2_State] = DUEL2V2_STATE_FINISHED;
    KillDuel2v2Timers(matchid);

    new winScore = (winning_team == 0) ? g_Duel2v2Data[matchid][d2_TeamA_Score] : g_Duel2v2Data[matchid][d2_TeamB_Score];
    new loseScore = (winning_team == 0) ? g_Duel2v2Data[matchid][d2_TeamB_Score] : g_Duel2v2Data[matchid][d2_TeamA_Score];

    new winMsg[144], loseMsg[144];
    format(winMsg, sizeof(winMsg), "{MAIN}[DUEL 2v2] {FFFFFF}VICTORY! Your team won the 2v2 duel (%d vs %d)! {23C552}+%s",
        winScore, loseScore, FormatMoney(DUEL2V2_WIN_REWARD));
    format(loseMsg, sizeof(loseMsg), "{MAIN}[DUEL 2v2] {FFFFFF}DEFEAT! Your team lost the 2v2 duel (%d vs %d).",
        loseScore, winScore);

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        new team = (i < 2) ? 0 : 1;
        SendClientMessage(players[i], -1, "%s", (team == winning_team) ? winMsg : loseMsg);
    }

    AwardDuel2v2Result(matchid, winning_team);

    g_Duel2v2Data[matchid][d2_MatchTimer] = SetTimerEx("ResetMatchAfterFinish", DUEL2V2_FINISH_DELAY, false, "i", matchid);
    return 1;
}

public ResetMatchAfterFinish(matchid)
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2) return 0;

    g_Duel2v2Data[matchid][d2_MatchTimer] = 0;

    if(!g_Duel2v2Data[matchid][d2_Active]) return 0;
    if(g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_FINISHED) return 0;

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        ExitDuel2v2Player(players[i]);
    }

    ResetDuel2v2Data(matchid);
    return 1;
}

stock ExitDuel2v2Player(playerid)
{
    if(!IsValidDuel2v2Player(playerid)) return 0;

    g_PlayerDuel2v2ID[playerid] = -1;
    g_PlayerDuel2v2Team[playerid] = -1;
    DestroyDuel2v2PlayerTextDraws(playerid);

    TogglePlayerControllable(playerid, true);
    ResetPlayerWeapons(playerid);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);

    User[playerid][isAfk] = false;
    if(User[playerid][locationId] != -1 && User[playerid][locationId] != ARENA_AFK)
    {
        SpawnPlayerEx(playerid);
    }
    else
    {
        setAfk(playerid);
    }
    return 1;
}

stock ForfeitDuel2v2Session(matchid, offenderid, const reason[])
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    if(g_Duel2v2Data[matchid][d2_State] == DUEL2V2_STATE_PENDING)
    {
        return CancelDuel2v2Session(matchid, reason);
    }

    if(g_Duel2v2Data[matchid][d2_State] == DUEL2V2_STATE_FINISHED) return 0;

    new offenderTeam = (offenderid >= 0 && offenderid < MAX_PLAYERS) ? g_PlayerDuel2v2Team[offenderid] : -1;
    if(offenderTeam != 0 && offenderTeam != 1)
    {
        return CancelDuel2v2Session(matchid, reason);
    }

    g_Duel2v2Data[matchid][d2_State] = DUEL2V2_STATE_FINISHED;
    KillDuel2v2Timers(matchid);

    new winningTeam = (offenderTeam == 0) ? 1 : 0;

    new forfeitMsg[208];
    format(forfeitMsg, sizeof(forfeitMsg), "{MAIN}[DUEL 2v2] {FFFFFF}The 2v2 duel ended by forfeit. Reason: %s", reason);
    SendDuel2v2Message(matchid, forfeitMsg);

    new resultMsg[144];
    format(resultMsg, sizeof(resultMsg), "{MAIN}[DUEL 2v2] {FFFFFF}Your team wins by forfeit! {23C552}+%s", FormatMoney(DUEL2V2_WIN_REWARD));

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        if(((i < 2) ? 0 : 1) != winningTeam) continue;
        SendClientMessage(players[i], -1, "%s", resultMsg);
    }

    AwardDuel2v2Result(matchid, winningTeam);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        ExitDuel2v2Player(players[i]);
    }

    ResetDuel2v2Data(matchid);
    return 1;
}

stock CancelDuel2v2Session(matchid, const reason[])
{
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return 0;

    KillDuel2v2Timers(matchid);

    new cancelMsg[208];
    format(cancelMsg, sizeof(cancelMsg), "{MAIN}[DUEL 2v2] {FFFFFF}The 2v2 duel was cancelled. Reason: %s", reason);
    SendDuel2v2Message(matchid, cancelMsg);

    new players[4];
    GetDuel2v2Players(matchid, players);

    for(new i = 0; i < 4; i++)
    {
        if(!IsValidDuel2v2Player(players[i])) continue;
        ExitDuel2v2Player(players[i]);
    }

    ResetDuel2v2Data(matchid);
    return 1;
}

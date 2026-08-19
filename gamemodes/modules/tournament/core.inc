#if defined _tourney_core_included
    #endinput
#endif
#define _tourney_core_included

// Tournament ALS Hooks

public OnPlayerConnect(playerid)
{
    g_PlayerTourneySlot[playerid] = -1;
    g_PlayerTourneyMatch[playerid] = -1;
    g_PlayerTourneyInvite[playerid] = -1;
    g_PlayerTourneySpectating[playerid] = -1;

    #if defined TOUR_OnPlayerConnect
        return TOUR_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect TOUR_OnPlayerConnect
#if defined TOUR_OnPlayerConnect
    forward TOUR_OnPlayerConnect(playerid);
#endif

public OnPlayerSpawn(playerid)
{
    if(User[playerid][isLogged])
    {
        if(g_PlayerTourneySlot[playerid] == -1 && (g_TourneyData[t_State] == TOURNAMENT_STATE_REGISTRATION || g_TourneyData[t_State] == TOURNAMENT_STATE_IN_PROGRESS))
        {
            for(new i = 0; i < g_TourneyData[t_MaxSlots]; i++)
            {
                if(g_TourneyParticipants[i][tp_Registered])
                {
                    if(!strcmp(g_TourneyParticipants[i][tp_Player1_Name], User[playerid][userName], true))
                    {
                        g_TourneyParticipants[i][tp_Player1] = playerid;
                        g_TourneyParticipants[i][tp_Player1_SQL] = User[playerid][user_sqlid];
                        g_PlayerTourneySlot[playerid] = i;
                        break;
                    }
                    else if(g_TourneyParticipants[i][tp_Player2_Name][0] != '\0' && !strcmp(g_TourneyParticipants[i][tp_Player2_Name], User[playerid][userName], true))
                    {
                        g_TourneyParticipants[i][tp_Player2] = playerid;
                        g_TourneyParticipants[i][tp_Player2_SQL] = User[playerid][user_sqlid];
                        g_PlayerTourneySlot[playerid] = i;
                        break;
                    }
                }
            }
        }
    }

    new matchid = g_PlayerTourneyMatch[playerid];

    if(IsPlayerInTournament(playerid) && matchid >= 0 && matchid < MAX_TOURNEY_MATCHES
        && g_TourneyMatches[matchid][tm_Active])
    {
        new mstate = g_TourneyMatches[matchid][tm_State];

        if(mstate == MATCH_STATE_PREPARING || mstate == MATCH_STATE_COUNTDOWN || mstate == MATCH_STATE_ACTIVE)
        {
            new slot = g_PlayerTourneySlot[playerid];
            new side = (slot == g_TourneyMatches[matchid][tm_SlotA]) ? 0 : 1;

            SpawnTourneyParticipant(playerid, side,
                g_TourneyMatches[matchid][tm_ArenaCategory],
                g_TourneyMatches[matchid][tm_ArenaID],
                g_TourneyMatches[matchid][tm_VirtualWorld]);
        }
    }

    #if defined TOUR_OnPlayerSpawn
        return TOUR_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn TOUR_OnPlayerSpawn
#if defined TOUR_OnPlayerSpawn
    forward TOUR_OnPlayerSpawn(playerid);
#endif

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if(IsPlayerInTournament(playerid))
    {
        ProcessTourneyPlayerDeath(playerid, killerid, _:reason);
    }

    #if defined TOUR_OnPlayerDeath
        return TOUR_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath TOUR_OnPlayerDeath
#if defined TOUR_OnPlayerDeath
    forward TOUR_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    if(IsPlayerInTournament(playerid))
    {
        new dcSlot = g_PlayerTourneySlot[playerid];
        if(g_TourneyData[t_State] == TOURNAMENT_STATE_REGISTRATION)
        {
            if(dcSlot >= 0 && dcSlot < MAX_TOURNEY_SLOTS)
            {
                if(g_TourneyParticipants[dcSlot][tp_Player1] == playerid)
                    g_TourneyParticipants[dcSlot][tp_Player1] = INVALID_PLAYER_ID;
                if(g_TourneyParticipants[dcSlot][tp_Player2] == playerid)
                    g_TourneyParticipants[dcSlot][tp_Player2] = INVALID_PLAYER_ID;
            }
            g_PlayerTourneySlot[playerid] = -1;
        }
        else if(g_TourneyData[t_State] == TOURNAMENT_STATE_IN_PROGRESS)
        {
            new matchid = g_PlayerTourneyMatch[playerid];
            new mstate = (matchid != -1) ? g_TourneyMatches[matchid][tm_State] : MATCH_STATE_IDLE;

            if(matchid != -1 && g_TourneyMatches[matchid][tm_Active]
                && (mstate == MATCH_STATE_ACTIVE || mstate == MATCH_STATE_PREPARING || mstate == MATCH_STATE_COUNTDOWN))
            {
                new slot = g_PlayerTourneySlot[playerid];
                new slotA = g_TourneyMatches[matchid][tm_SlotA];
                new slotB = g_TourneyMatches[matchid][tm_SlotB];
                if(slot == slotA)
                {
                    if(playerid == g_TourneyParticipants[slotA][tp_Player1]) g_TourneyMatches[matchid][tm_P1_Alive] = false;
                    if(playerid == g_TourneyParticipants[slotA][tp_Player2]) g_TourneyMatches[matchid][tm_P2_Alive] = false;
                }
                else if(slot == slotB)
                {
                    if(playerid == g_TourneyParticipants[slotB][tp_Player1]) g_TourneyMatches[matchid][tm_P3_Alive] = false;
                    if(playerid == g_TourneyParticipants[slotB][tp_Player2]) g_TourneyMatches[matchid][tm_P4_Alive] = false;
                }

                new bool:teamA_Alive = (g_TourneyMatches[matchid][tm_P1_Alive] || g_TourneyMatches[matchid][tm_P2_Alive]);
                new bool:teamB_Alive = (g_TourneyMatches[matchid][tm_P3_Alive] || g_TourneyMatches[matchid][tm_P4_Alive]);

                if(!teamA_Alive || !teamB_Alive)
                {
                    new winnerSlot = -1;
                    if(teamA_Alive && !teamB_Alive) winnerSlot = slotA;
                    else if(!teamA_Alive && teamB_Alive) winnerSlot = slotB;

                    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s disconnected during the match! W.O. awarded!", User[playerid][userName]);
                    FinishTourneyMatch(matchid, winnerSlot, true);
                }
                else
                {
                    va_SendClientMessageToAll(-1, "{MAIN}[TOURNAMENT] {FFFFFF}%s disconnected, their teammate continues alone!", User[playerid][userName]);
                }
            }
            if(dcSlot >= 0 && dcSlot < MAX_TOURNEY_SLOTS)
            {
                if(g_TourneyParticipants[dcSlot][tp_Player1] == playerid)
                    g_TourneyParticipants[dcSlot][tp_Player1] = INVALID_PLAYER_ID;
                if(g_TourneyParticipants[dcSlot][tp_Player2] == playerid)
                    g_TourneyParticipants[dcSlot][tp_Player2] = INVALID_PLAYER_ID;
            }

            g_PlayerTourneySlot[playerid] = -1;
            g_PlayerTourneyMatch[playerid] = -1;
        }
    }

    g_PlayerTourneySpectating[playerid] = -1;
    g_PlayerTourneyInvite[playerid] = -1;

    #if defined TOUR_OnPlayerDisconnect
        return TOUR_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect TOUR_OnPlayerDisconnect
#if defined TOUR_OnPlayerDisconnect
    forward TOUR_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(hittype == BULLET_HIT_TYPE_PLAYER)
    {
        if(IsPlayerInActiveTourneyMatch(playerid) && IsPlayerInActiveTourneyMatch(hitid))
        {
            if(g_PlayerTourneySlot[playerid] == g_PlayerTourneySlot[hitid])
            {
                return 0;
            }
        }
    }

    #if defined TOUR_OnPlayerWeaponShot
        return TOUR_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot TOUR_OnPlayerWeaponShot
#if defined TOUR_OnPlayerWeaponShot
    forward TOUR_OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnGameModeInit()
{
    InitTournamentDefaultConfig();

    #if defined TOUR_OnGameModeInit
        return TOUR_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit TOUR_OnGameModeInit
#if defined TOUR_OnGameModeInit
    forward TOUR_OnGameModeInit();
#endif

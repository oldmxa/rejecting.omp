#if defined _duel2v2_definitions_included
    #endinput
#endif
#define _duel2v2_definitions_included

// Duel 2v2 Definitions

#define MAX_DUELS_2V2               (16)
#define DUEL2V2_VW_BASE             (60000)
#define DUEL2V2_INVITE_EXPIRE       (60)
#define DUEL2V2_COUNTDOWN           (3)
#define DUEL2V2_WIN_REWARD          (5000)
#define DUEL2V2_ROUND_DELAY         (2000)
#define DUEL2V2_FINISH_DELAY        (2500)
#define DUEL2V2_DEAD_Z_OFFSET       (15.0)

forward OnDuel2v2CountdownTick(matchid, count);
forward OnDuel2v2InviteExpired(matchid);
forward StartNextRoundTimer(matchid);
forward ResetMatchAfterFinish(matchid);

enum
{
    DUEL2V2_STATE_NONE = 0,
    DUEL2V2_STATE_PENDING,
    DUEL2V2_STATE_PREPARING,
    DUEL2V2_STATE_ACTIVE,
    DUEL2V2_STATE_ROUND_END,
    DUEL2V2_STATE_FINISHED
}

enum e_DUEL2V2_MATCH_DATA
{
    bool:d2_Active,
    d2_State,
    d2_ArenaID,
    d2_WeaponSet,       // 0: Deagle, 1: Deagle+M4, 2: Deagle+Shotgun, 3: Deagle+Sniper, 4: All Weapons
    d2_MaxRounds,       // 1, 3, 5, 7
    d2_CurrentRound,
    
    // Team A: Creator & Teammate
    d2_TeamA_Player1,
    d2_TeamA_Player2,
    bool:d2_TeamA_P1_Accepted,
    bool:d2_TeamA_P2_Accepted,
    bool:d2_TeamA_P1_Alive,
    bool:d2_TeamA_P2_Alive,
    d2_TeamA_Score,
    
    // Team B: Opponent 1 & Opponent 2
    d2_TeamB_Player1,
    d2_TeamB_Player2,
    bool:d2_TeamB_P1_Accepted,
    bool:d2_TeamB_P2_Accepted,
    bool:d2_TeamB_P1_Alive,
    bool:d2_TeamB_P2_Alive,
    d2_TeamB_Score,

    d2_CountdownTimer,
    d2_MatchTimer,
    d2_InviteExpireTimer,
    d2_VirtualWorld
}

new g_Duel2v2Data[MAX_DUELS_2V2][e_DUEL2V2_MATCH_DATA];

stock bool:IsPlayerBusy(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return true;
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return true;
    if(User[playerid][isSpectating] || User[playerid][isAfk] || User[playerid][isCreatingArena]) return true;
    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE) return true;
    if(g_PlayerDuel2v2ID[playerid] != -1) return true;
    if(g_PlayerTraining[playerid][pTrain_Active]) return true;
    if(g_Practice[playerid][pracActive]) return true;
    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if(pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING) return true;
    return false;
}

stock bool:IsDuel2v2PlayerAlive(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    new matchid = g_PlayerDuel2v2ID[playerid];
    if(matchid < 0 || matchid >= MAX_DUELS_2V2 || !g_Duel2v2Data[matchid][d2_Active]) return false;

    if(playerid == g_Duel2v2Data[matchid][d2_TeamA_Player1]) return g_Duel2v2Data[matchid][d2_TeamA_P1_Alive];
    if(playerid == g_Duel2v2Data[matchid][d2_TeamA_Player2]) return g_Duel2v2Data[matchid][d2_TeamA_P2_Alive];
    if(playerid == g_Duel2v2Data[matchid][d2_TeamB_Player1]) return g_Duel2v2Data[matchid][d2_TeamB_P1_Alive];
    if(playerid == g_Duel2v2Data[matchid][d2_TeamB_Player2]) return g_Duel2v2Data[matchid][d2_TeamB_P2_Alive];

    return false;
}

new g_Duel2v2Wiz_Arena[MAX_PLAYERS];
new g_Duel2v2Wiz_Weapons[MAX_PLAYERS];
new g_Duel2v2Wiz_Rounds[MAX_PLAYERS];

// Weapon configuration
enum e_DUEL2V2_WEAPON_SET
{
    d2w_Name[48],
    d2w_W1, d2w_A1,
    d2w_W2, d2w_A2,
    d2w_W3, d2w_A3
}

new const g_Duel2v2WeaponSets[][e_DUEL2V2_WEAPON_SET] =
{
    {"Desert Eagle Only",         WEAPON_DEAGLE, 500, 0, 0, 0, 0},
    {"Deagle + M4",               WEAPON_DEAGLE, 500, WEAPON_M4, 1000, 0, 0},
    {"Deagle + Shotgun",          WEAPON_DEAGLE, 500, WEAPON_SHOTGUN, 500, 0, 0},
    {"Deagle + Sniper",           WEAPON_DEAGLE, 500, WEAPON_SNIPER, 500, 0, 0},
    {"Full Pack (Deagle/M4/Snip)",WEAPON_DEAGLE, 500, WEAPON_M4, 1000, WEAPON_SNIPER, 500}
};

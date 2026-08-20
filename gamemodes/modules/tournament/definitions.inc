#if defined _tourney_def_included
    #endinput
#endif
#define _tourney_def_included

// Tournament Definitions & Constants

#define MAX_TOURNEY_SLOTS            (32)
#define MAX_TOURNEY_MATCHES          (32)
#define MAX_TOURNEY_ROUNDS           (6)
#define TOURNEY_VW_BASE              (90000)
#define TOURNEY_COUNTDOWN_TIME       (5)
#define TOURNEY_DEFAULT_ROUND_TIME   (90)
#define TOURNEY_WO_COUNTDOWN_TIME    (30)
#define TOURNEY_TAG_WINNER           "CHAMPION"

// Forward declarations
forward OnTourneyCountdownTick(matchid, count);
forward OnTourneyMatchTimer(matchid);
forward OnTourneyWOCountdownTick(matchid);
forward StartNextTourneyRoundDelayed(round);
forward OnTourneyFinishedDelay();

// Modalities
enum
{
    TOURNAMENT_MODE_1V1 = 1,
    TOURNAMENT_MODE_2V2,
    TOURNAMENT_MODE_CLAN
}

// Tournament States
enum
{
    TOURNAMENT_STATE_NONE = 0,
    TOURNAMENT_STATE_REGISTRATION,
    TOURNAMENT_STATE_STARTING,
    TOURNAMENT_STATE_IN_PROGRESS,
    TOURNAMENT_STATE_FINISHED
}

// Match States
enum
{
    MATCH_STATE_IDLE = 0,
    MATCH_STATE_PREPARING,
    MATCH_STATE_COUNTDOWN,
    MATCH_STATE_ACTIVE,
    MATCH_STATE_FINISHED,
    MATCH_STATE_WO
}

// Weapon Sets
enum
{
    TOURNEY_WEAPONS_DEAGLE = 0,
    TOURNEY_WEAPONS_DEAGLE_SHOTGUN,
    TOURNEY_WEAPONS_DEAGLE_M4,
    TOURNEY_WEAPONS_DEAGLE_SNIPER,
    TOURNEY_WEAPONS_ALL,
    TOURNEY_WEAPONS_SNIPER_ONLY,
    TOURNEY_WEAPONS_SHOTGUN_ONLY,
    TOURNEY_WEAPONS_M4_ONLY,
    TOURNEY_WEAPONS_MP5_DEAGLE
}

// Arena Categories
#define MAX_TOURNEY_CUSTOM_ARENAS (5)
#define MAX_TOURNEY_SERVER_ARENAS (MAX_ZONES - MAX_TOURNEY_CUSTOM_ARENAS)
#define MAX_TOURNEY_DUEL_ARENAS   (21)

enum
{
    TOURNEY_ARENA_CAT_RANDOM = 0,
    TOURNEY_ARENA_CAT_DUEL,
    TOURNEY_ARENA_CAT_SERVER,
    TOURNEY_ARENA_CAT_CUSTOM
}

enum e_TOURNEY_WEAPON_INFO
{
    twi_Name[32],
    twi_Weap1,
    twi_Ammo1,
    twi_Weap2,
    twi_Ammo2,
    twi_Weap3,
    twi_Ammo3,
    twi_Weap4,
    twi_Ammo4
}

new const g_TourneyWeaponSets[][e_TOURNEY_WEAPON_INFO] =
{
    {"Deagle Only",                    WEAPON_DEAGLE, 250, 0, 0, 0, 0, 0, 0},
    {"Deagle + Shotgun",               WEAPON_DEAGLE, 250, WEAPON_SHOTGUN, 100, 0, 0, 0, 0},
    {"Deagle + M4",                    WEAPON_DEAGLE, 250, WEAPON_M4, 300, 0, 0, 0, 0},
    {"Deagle + Sniper",                WEAPON_DEAGLE, 250, WEAPON_SNIPER, 50, 0, 0, 0, 0},
    {"Deagle + Shotgun + M4 + Sniper", WEAPON_DEAGLE, 250, WEAPON_SHOTGUN, 100, WEAPON_M4, 300, WEAPON_SNIPER, 50},
    {"Sniper Only",                    WEAPON_SNIPER, 100, 0, 0, 0, 0, 0, 0},
    {"Shotgun Only",                   WEAPON_SHOTGUN, 150, 0, 0, 0, 0, 0, 0},
    {"M4 Only",                        WEAPON_M4, 500, 0, 0, 0, 0, 0, 0},
    {"Deagle + MP5",                   WEAPON_DEAGLE, 250, WEAPON_MP5, 300, 0, 0, 0, 0}
};

new const g_TourneyCustomArenaNames[MAX_TOURNEY_CUSTOM_ARENAS][32] =
{
    "Chinese Garden",
    "Desert",
    "Egypt",
    "Urbex",
    "Zoo"
};

#assert MAX_TOURNEY_CUSTOM_ARENAS == sizeof(g_TourneyCustomArenaNames)
#assert MAX_TOURNEY_SERVER_ARENAS + MAX_TOURNEY_CUSTOM_ARENAS == MAX_ZONES
#assert MAX_TOURNEY_DUEL_ARENAS == sizeof(DuelS_ArenaNames)
#assert MAX_TOURNEY_SLOTS <= MAX_TOURNEY_MATCHES

// Tournament Configuration & Global State
enum e_TOURNAMENT_DATA
{
    bool:t_Active,
    t_Name[64],
    t_AchievementTag[32],
    t_State,
    t_Mode,
    t_MaxSlots,
    t_RegisteredCount,
    t_PrizeCoins,
    t_PrizeMoney,
    t_EntryFee,
    t_WeaponSet,
    t_ArenaCategory,
    t_ArenaID,
    t_RoundsPerMatch,
    t_RoundTimeLimit,
    bool:t_GiveArmour,
    bool:t_RequireDiscord,
    bool:t_AutoStart,
    t_Schedule[64],
    t_StartedBy[MAX_PLAYER_NAME + 1],
    t_StartTime,
    t_CurrentRound,
    t_TotalRounds,
    t_WinnerSlot,
    t_WinnerName[MAX_PLAYER_NAME + 1],
    t_WinnerClanID,
    t_SQLID
}

new g_TourneyData[e_TOURNAMENT_DATA];

// Participant
enum e_TOURNEY_PARTICIPANT
{
    bool:tp_Registered,
    tp_Player1,
    tp_Player1_SQL,
    tp_Player1_Name[MAX_PLAYER_NAME + 1],
    tp_Player2,
    tp_Player2_SQL,
    tp_Player2_Name[MAX_PLAYER_NAME + 1],
    tp_ClanID,
    tp_ClanName[CLAN_NAME_LEN],
    tp_ClanTag[CLAN_TAG_LEN],
    bool:tp_Eliminated,
    tp_Wins,
    tp_Kills,
    tp_Deaths,
    tp_Seed
}

new g_TourneyParticipants[MAX_TOURNEY_SLOTS][e_TOURNEY_PARTICIPANT];

// Match
enum e_TOURNEY_MATCH
{
    bool:tm_Active,
    tm_State,
    tm_Round,
    tm_MatchIndex,
    tm_SlotA,
    tm_SlotB,
    tm_WinnerSlot,
    tm_ScoreA,
    tm_ScoreB,
    tm_ArenaCategory,
    tm_ArenaID,
    tm_TimerCountdown,
    tm_TimerMatch,
    tm_TimerWO,
    tm_WOTimeRemaining,
    tm_WOForSlot,
    bool:tm_P1_Alive,
    bool:tm_P2_Alive,
    bool:tm_P3_Alive,
    bool:tm_P4_Alive,
    tm_VirtualWorld,
    tm_TimeRemaining
}

new g_TourneyMatches[MAX_TOURNEY_MATCHES][e_TOURNEY_MATCH];
new g_PlayerTourneyInvite[MAX_PLAYERS] = {-1, ...};
new g_PlayerTourneySpectating[MAX_PLAYERS] = {-1, ...};

// Discord Applications
#define MAX_DISCORD_TOURNEY_APPS (128)

enum e_TOURNEY_DISCORD_APP
{
    bool:tda_Active,
    tda_Nick[MAX_PLAYER_NAME + 1],
    tda_UCP[128],
    tda_DiscordID[DCC_ID_SIZE],
    tda_DiscordTag[DCC_USERNAME_SIZE],
    tda_Timestamp
}

new g_TourneyDiscordApps[MAX_DISCORD_TOURNEY_APPS][e_TOURNEY_DISCORD_APP];
new g_TourneyDiscordAppCount = 0;

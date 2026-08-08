#if defined _achievements_included
    #endinput
#endif
#define _achievements_included

#include <YSI_Coding/y_hooks>

#define ACH_CAT_GENERAL (0)
#define ACH_CAT_UNIQUE  (1)

#define ACH_FLUSH_INTERVAL (60000)

enum E_ACHIEVEMENT_DATA {
    ach_id,
    ach_name[64],
    ach_desc[128],
    ach_category,
    ach_target,
    ach_reward_money,
    ach_reward_coins
};

new const g_AchievementsData[][E_ACHIEVEMENT_DATA] = {
    {0,  "Welcome!",           "Join the server for the very first time",            ACH_CAT_GENERAL, 1,       77777,   50},
    {1,  "First victory",      "Win your first duel or arena match",                 ACH_CAT_GENERAL, 1,       10000,   100},
    {2,  "Maniac",             "Eliminate 500 total enemy players",                  ACH_CAT_GENERAL, 500,     25000,   250},
    {3,  "6 figures",          "Accumulate $500,000 in total cash balance",          ACH_CAT_GENERAL, 500000,  50000,   500},
    {4,  "Nemesis",            "Eliminate the same enemy player 25 times",           ACH_CAT_GENERAL, 25,      30000,   300},
    {5,  "Cash maniac",        "Accumulate $3,000,000 in total cash balance",        ACH_CAT_GENERAL, 3000000, 100000,  1000},
    {6,  "Merciless",          "Eliminate 1,000 total enemy players",                ACH_CAT_GENERAL, 1000,    50000,   500},
    {7,  "Sniper elite",       "Eliminate 1,000 enemy players using Sniper Rifle",   ACH_CAT_GENERAL, 1000,    75000,   750},
    {8,  "God of war",         "Eliminate 2,500 total enemy players",                ACH_CAT_GENERAL, 2500,    100000,  1000},
    {9,  "At what cost",       "Take 20,000 total damage from enemy attacks",        ACH_CAT_GENERAL, 20000,   35000,   350},
    {10, "2 fast",             "Perform 25 rapid double kills in combat",            ACH_CAT_GENERAL, 25,      15000,   150},
    {11, "Variative",          "Eliminate enemies using 10 different weapons",       ACH_CAT_GENERAL, 10,      20000,   200},
    {12, "Big time damage",    "Deal 150,000 total damage to enemy players",         ACH_CAT_GENERAL, 150000,  60000,   600},
    {13, "Bulletproof",        "Survive 15 minutes inside an arena without dying",   ACH_CAT_GENERAL, 1,       30000,   300},
    {14, "Undying",            "Achieve a killstreak of 25 kills without dying",     ACH_CAT_GENERAL, 25,      50000,   500},
    {15, "Headache",           "Perform 300 headshot eliminations",                  ACH_CAT_GENERAL, 300,     40000,   400},
    {16, "Cleanup",            "Eliminate 15 enemy players in a single round",       ACH_CAT_GENERAL, 15,      25000,   250},
    {17, "Chilled & relaxed",  "Spend 120 minutes relaxing in the Lounge area",      ACH_CAT_GENERAL, 120,     15000,   150},
    {18, "Call a friend",      "Join or create a clan with your teammates",          ACH_CAT_GENERAL, 1,       20000,   200},
    {19, "Honoured",           "Reach Level 10 account status on the server",        ACH_CAT_GENERAL, 10,      50000,   500},
    {20, "Still dangerous",    "Eliminate an enemy while having under 10 HP",        ACH_CAT_GENERAL, 1,       25000,   250},
    {21, "Dynamic duo",        "Capture 10 outposts together with a clanmate",       ACH_CAT_GENERAL, 10,      30000,   300},
    {22, "Wide range",         "Play matches across 10 different arenas",            ACH_CAT_GENERAL, 10,      20000,   200},
    {23, "Improvement",        "Win 50 duels against other players",                 ACH_CAT_GENERAL, 50,      35000,   350},
    {24, "Nolife",             "Eliminate 5,000 total enemy players",                ACH_CAT_GENERAL, 5000,    200000,  2000},
    {25, "Light work",         "Complete 200 total arena rounds",                    ACH_CAT_GENERAL, 200,     25000,   250},
    {26, "Highway to hell",    "Eliminate 500 players in Desert arenas",             ACH_CAT_GENERAL, 500,     50000,   500},
    {27, "Rich",               "Accumulate $10,000,000 in total cash balance",       ACH_CAT_GENERAL, 10000000,250000,  2500},
    {28, "Best support",       "Assist clanmates in capturing 50 outposts",          ACH_CAT_GENERAL, 50,      40000,   400},
    {29, "Dream team",         "Win 25 outpost captures with your clan",             ACH_CAT_GENERAL, 25,      75000,   750},
    {30, "Best fighter",       "Become the MVP player of 25 arena rounds",           ACH_CAT_GENERAL, 25,      50000,   500},
    {31, "Sharpshooter",       "Complete 50 total training challenges",              ACH_CAT_GENERAL, 50,      30000,   300},
    {32, "Target Annihilator", "Deal 50,000 total damage in training mode",          ACH_CAT_GENERAL, 50000,   60000,   600},
    {33, "Bullseye",           "Achieve 95% or higher accuracy in a challenge",      ACH_CAT_GENERAL, 1,       40000,   400},
    {34, "Record Breaker",     "Set a new personal or server record in training",    ACH_CAT_GENERAL, 1,       75000,   750},
    {35, "Rising Star",        "Reach Level 15 (Earns $50k & 1k Coins per lvl >10)", ACH_CAT_GENERAL, 15,      50000,   1000},
    {36, "Veteran Fighter",    "Reach Level 20 (Earns $50k & 1k Coins per lvl >10)", ACH_CAT_GENERAL, 20,      50000,   1000},
    {37, "Elite Operative",    "Reach Level 25 (Earns $50k & 1k Coins per lvl >10)", ACH_CAT_GENERAL, 25,      50000,   1000},
    {38, "Server Warlord",     "Reach Level 30 (Earns $50k & 1k Coins per lvl >10)", ACH_CAT_GENERAL, 30,      50000,   1000},
    {39, "Founder",            "Awarded personally by the server owner",             ACH_CAT_UNIQUE,  1,       150000,  1500},
    {40, "Legend",             "Awarded personally by the server owner",             ACH_CAT_UNIQUE,  1,       250000,  2500},
    {41, "Hall of Fame",       "Awarded personally by the server owner",             ACH_CAT_UNIQUE,  1,       500000,  5000}
};

#define ACH_COUNT (sizeof(g_AchievementsData))

// IDs
#define ACH_WELCOME             (0)
#define ACH_FIRST_VICTORY       (1)
#define ACH_MANIAC              (2)
#define ACH_6_FIGURES           (3)
#define ACH_NEMESIS             (4)
#define ACH_CASH_MANIAC         (5)
#define ACH_MERCILESS           (6)
#define ACH_SNIPER_ELITE        (7)
#define ACH_GOD_OF_WAR          (8)
#define ACH_AT_WHAT_COST        (9)
#define ACH_2_FAST              (10)
#define ACH_VARIATIVE           (11)
#define ACH_BIG_TIME_DAMAGE     (12)
#define ACH_BULLETPROOF         (13)
#define ACH_UNDYING             (14)
#define ACH_HEADACHE            (15)
#define ACH_CLEANUP             (16)
#define ACH_CHILLED             (17)
#define ACH_CALL_A_FRIEND       (18)
#define ACH_HONOURED            (19)
#define ACH_STILL_DANGEROUS     (20)
#define ACH_DYNAMIC_DUO         (21)
#define ACH_WIDE_RANGE          (22)
#define ACH_IMPROVEMENT         (23)
#define ACH_NOLIFE              (24)
#define ACH_LIGHT_WORK          (25)
#define ACH_HIGHWAY_TO_HELL     (26)
#define ACH_RICH                (27)
#define ACH_BEST_SUPPORT        (28)
#define ACH_DREAM_TEAM          (29)
#define ACH_BEST_FIGHTER        (30)
#define ACH_SHARPSHOOTER        (31)
#define ACH_TARGET_ANNIHILATOR  (32)
#define ACH_BULLSEYE            (33)
#define ACH_RECORD_BREAKER      (34)
#define ACH_LEVEL_15            (35)
#define ACH_LEVEL_20            (36)
#define ACH_LEVEL_25            (37)
#define ACH_LEVEL_30            (38)

#define ACH_DOUBLE_KILL_WINDOW  (3000)
#define ACH_BULLETPROOF_TIME    (900)
#define ACH_MAX_WEAPON_ID       (46)

#define ACH_LEVEL_BONUS_MIN     (10)
#define ACH_LEVEL_BONUS_MONEY   (50000)
#define ACH_LEVEL_BONUS_COINS   (1000)

#define ACH_UTC_OFFSET          (10800)

#define ACH_DATE_LEN            (20)

#define ACH_ICON_DONE           "{23C552}\x95"
#define ACH_ICON_LOCKED         "{F84F31}\x95"

#define ACH_BITSET_CELLS        (2)

// Arenas in the desert region (Bone County and surroundings), used for "Highway to Hell".
new const g_AchDesertArenas[] = {
    1,  // Valle Ocultado
    2,  // Las Paysadas
    3,  // Las Brujas
    4,  // Area 51
    5,  // Fort Carson
    7,  // Spiny Bed
    41, // Hunter Quarry
    43, // Green Palms
    44, // The Big Ear
    48, // Ghost Town
    49, // Las Barrancas
    50  // El Quebrados
};

#assert ACH_COUNT <= MAX_ACHIEVEMENTS
#assert MAX_ZONES <= ACH_BITSET_CELLS * 32
#assert ACH_MAX_WEAPON_ID < ACH_BITSET_CELLS * 32

new bool:g_UserAchUnlocked[MAX_PLAYERS][MAX_ACHIEVEMENTS];
new g_UserAchProgress[MAX_PLAYERS][MAX_ACHIEVEMENTS];
new g_UserAchDate[MAX_PLAYERS][MAX_ACHIEVEMENTS][ACH_DATE_LEN];

new bool:g_UserAchDirty[MAX_PLAYERS];

new g_AchKillWeapons[MAX_PLAYERS][ACH_BITSET_CELLS];
new g_AchArenasPlayed[MAX_PLAYERS][ACH_BITSET_CELLS];
new g_AchLastKillTick[MAX_PLAYERS];
new Float:g_AchDamageDealt[MAX_PLAYERS];
new Float:g_AchDamageTaken[MAX_PLAYERS];
new g_AchAliveSince[MAX_PLAYERS];
new g_AchLoungeSince[MAX_PLAYERS];

new bool:g_AchQuietGrant[MAX_PLAYERS];
new g_AchQuietCount[MAX_PLAYERS];

forward DIALOG_ACHIEVEMENTS_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_ACHIEVEMENTS_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_ACHIEVEMENTS_DETAIL(playerid, dialogid, response, listitem, string:inputtext[]);
forward FlushAchievementSaves();

stock SerializeAchievements(playerid, output[], maxlen)
{
    new len = 0, entry[48];
    output[0] = '\0';

    for(new i = 0; i < MAX_ACHIEVEMENTS; i++)
    {
        if(g_UserAchProgress[playerid][i] <= 0 && !g_UserAchUnlocked[playerid][i])
            continue;

        format(entry, sizeof(entry), "%s%d|%d|%s",
            len ? ";" : "",
            i,
            g_UserAchProgress[playerid][i],
            g_UserAchDate[playerid][i]
        );

        new entryLen = strlen(entry);
        if(len + entryLen >= maxlen)
        {
            printf("[ACHIEVEMENTS] Serialization buffer full (playerid %d, sqlid %d).", playerid, User[playerid][user_sqlid]);
            break;
        }

        strcat(output, entry, maxlen);
        len += entryLen;
    }
    return len;
}

stock DeserializeAchievements(playerid, const data[])
{
    new len = strlen(data);
    if(!len) return 0;

    new pos = 0, entry[48], token[ACH_DATE_LEN];

    while(pos < len)
    {
        new epos = pos;
        while(epos < len && data[epos] != ';') epos++;
        strmid(entry, data, pos, epos, sizeof(entry));
        pos = epos + 1;

        if(!entry[0]) continue;

        new tpos = 0, elen = strlen(entry), field = 0;
        new achid = -1, progress = 0, date[ACH_DATE_LEN];
        date[0] = '\0';

        while(tpos <= elen && field < 3)
        {
            new tend = tpos;
            while(tend < elen && entry[tend] != '|') tend++;
            strmid(token, entry, tpos, tend, sizeof(token));
            tpos = tend + 1;

            switch(field++)
            {
                case 0: achid    = strval(token);
                case 1: progress = strval(token);
                case 2: strcopy(date, token, sizeof(date));
            }
        }

        if(achid < 0 || achid >= MAX_ACHIEVEMENTS) continue;

        if(progress < 0) progress = 0;
        if(achid < ACH_COUNT && progress > g_AchievementsData[achid][ach_target])
            progress = g_AchievementsData[achid][ach_target];

        g_UserAchProgress[playerid][achid] = progress;

        if(date[0])
        {
            g_UserAchUnlocked[playerid][achid] = true;
            strcopy(g_UserAchDate[playerid][achid], date, ACH_DATE_LEN);
        }
    }
    return 1;
}

stock SavePlayerAchievements(playerid)
{
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged] || User[playerid][user_sqlid] <= 0) return 0;

    g_achSerialBuf[0] = '\0';
    SerializeAchievements(playerid, g_achSerialBuf, ACH_SERIAL_BUF_SIZE);

    mysql_format(dbHandle, g_achQueryBuf, sizeof(g_achQueryBuf),
        "UPDATE `accounts` SET `achievements_data` = '%e' WHERE `id` = %d",
        g_achSerialBuf, User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, g_achQueryBuf);

    g_UserAchDirty[playerid] = false;
    return 1;
}

stock FlushPlayerAchievements(playerid)
{
    if(!g_UserAchDirty[playerid]) return 0;
    return SavePlayerAchievements(playerid);
}

public FlushAchievementSaves()
{
    foreach(new i : Player)
    {
        if(!User[i][isLogged]) continue;

        Ach_RefreshPlayerStats(i);
        FlushPlayerAchievements(i);
    }
    return 1;
}

stock ResetPlayerAchievements(playerid)
{
    for(new i = 0; i < MAX_ACHIEVEMENTS; i++)
    {
        g_UserAchUnlocked[playerid][i] = false;
        g_UserAchProgress[playerid][i] = 0;
        g_UserAchDate[playerid][i][0] = '\0';
    }
    g_UserAchDirty[playerid] = false;

    for(new i = 0; i < ACH_BITSET_CELLS; i++)
    {
        g_AchKillWeapons[playerid][i] = 0;
        g_AchArenasPlayed[playerid][i] = 0;
    }

    g_AchLastKillTick[playerid] = 0;
    g_AchDamageDealt[playerid] = 0.0;
    g_AchDamageTaken[playerid] = 0.0;
    g_AchAliveSince[playerid] = 0;
    g_AchLoungeSince[playerid] = 0;
    g_AchQuietGrant[playerid] = false;
    g_AchQuietCount[playerid] = 0;
    return 1;
}

hook OnGameModeInit()
{
    for(new i = 0; i < ACH_COUNT; i++)
    {
        if(g_AchievementsData[i][ach_id] != i)
            printf("[ACHIEVEMENTS] Index %d declares ach_id %d, they must match or saved progress shifts.", i, g_AchievementsData[i][ach_id]);
    }

    SetTimer("FlushAchievementSaves", ACH_FLUSH_INTERVAL, true);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    ResetPlayerAchievements(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    FlushPlayerAchievements(playerid);
    ResetPlayerAchievements(playerid);
    return 1;
}

stock LoadPlayerAchievements(playerid, const achData[])
{
    DeserializeAchievements(playerid, achData);

    GivePlayerAchievement(playerid, ACH_WELCOME);

    g_AchQuietGrant[playerid] = true;
    g_AchQuietCount[playerid] = 0;

    Ach_RefreshPlayerStats(playerid);

    g_AchQuietGrant[playerid] = false;
    FlushPlayerAchievements(playerid);

    if(g_AchQuietCount[playerid] > 0)
    {
        va_SendClientMessage(playerid, -1, ACHIEVEMENT_RETRO, g_AchQuietCount[playerid]);
        PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
    }
    return 1;
}

stock bool:HasPlayerAchievement(playerid, achievement_id)
{
    if(achievement_id < 0 || achievement_id >= ACH_COUNT) return false;
    return g_UserAchUnlocked[playerid][achievement_id];
}

stock GivePlayerAchievement(playerid, achievement_id, bool:manual = false)
{
    if(!IsPlayerConnected(playerid) || User[playerid][user_sqlid] <= 0) return 0;
    if(achievement_id < 0 || achievement_id >= ACH_COUNT) return 0;
    if(g_UserAchUnlocked[playerid][achievement_id]) return 0;

    if(g_AchievementsData[achievement_id][ach_category] == ACH_CAT_UNIQUE && !manual)
        return 0;

    g_UserAchUnlocked[playerid][achievement_id] = true;
    g_UserAchProgress[playerid][achievement_id] = g_AchievementsData[achievement_id][ach_target];
    strcopy(g_UserAchDate[playerid][achievement_id], returnDate(gettime() - ACH_UTC_OFFSET), ACH_DATE_LEN);
    g_UserAchDirty[playerid] = true;

    new money = g_AchievementsData[achievement_id][ach_reward_money];
    new coins = g_AchievementsData[achievement_id][ach_reward_coins];

    if(money > 0)
    {
        GivePlayerMoney(playerid, money);
        User[playerid][playerMoney] += money;
    }

    if(coins > 0)
        User[playerid][playerCoins] += coins;

    if(g_AchQuietGrant[playerid])
    {
        g_AchQuietCount[playerid]++;
        return 1;
    }

    FlushPlayerAchievements(playerid);

    va_SendClientMessage(playerid, -1, ACHIEVEMENT_UNLOCKED, g_AchievementsData[achievement_id][ach_name]);

    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
    return 1;
}

stock UpdatePlayerAchievementProgress(playerid, achievement_id, amount = 1)
{
    if(!IsPlayerConnected(playerid) || User[playerid][user_sqlid] <= 0) return 0;
    if(achievement_id < 0 || achievement_id >= ACH_COUNT) return 0;
    if(amount <= 0) return 0;
    if(g_UserAchUnlocked[playerid][achievement_id]) return 0;

    if(g_AchievementsData[achievement_id][ach_category] == ACH_CAT_UNIQUE)
        return 0;

    g_UserAchProgress[playerid][achievement_id] += amount;

    if(g_UserAchProgress[playerid][achievement_id] >= g_AchievementsData[achievement_id][ach_target])
        GivePlayerAchievement(playerid, achievement_id);
    else
        g_UserAchDirty[playerid] = true;

    return 1;
}

stock SetPlayerAchievementProgress(playerid, achievement_id, value)
{
    if(!IsPlayerConnected(playerid) || User[playerid][user_sqlid] <= 0) return 0;
    if(achievement_id < 0 || achievement_id >= ACH_COUNT) return 0;
    if(g_UserAchUnlocked[playerid][achievement_id]) return 0;

    if(g_AchievementsData[achievement_id][ach_category] == ACH_CAT_UNIQUE)
        return 0;

    if(value < 0) value = 0;
    if(value <= g_UserAchProgress[playerid][achievement_id]) return 0;

    g_UserAchProgress[playerid][achievement_id] = value;

    if(value >= g_AchievementsData[achievement_id][ach_target])
        GivePlayerAchievement(playerid, achievement_id);
    else
        g_UserAchDirty[playerid] = true;

    return 1;
}

static stock Ach_BitCount(const bits[], size)
{
    new count = 0;
    for(new c = 0; c < size; c++)
    {
        new v = bits[c];
        while(v)
        {
            v &= (v - 1);
            count++;
        }
    }
    return count;
}

static stock bool:Ach_IsPlayerActive(playerid)
{
    if(!User[playerid][isLogged] || User[playerid][isSpectating]) return false;

    switch(GetPlayerState(playerid))
    {
        case PLAYER_STATE_ONFOOT, PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER: return true;
    }
    return false;
}

static stock bool:Ach_IsDesertArena(zoneid)
{
    for(new i = 0; i < sizeof(g_AchDesertArenas); i++)
    {
        if(g_AchDesertArenas[i] == zoneid) return true;
    }
    return false;
}

static stock Ach_SyncKills(playerid, kills)
{
    SetPlayerAchievementProgress(playerid, ACH_MANIAC,     kills);
    SetPlayerAchievementProgress(playerid, ACH_MERCILESS,  kills);
    SetPlayerAchievementProgress(playerid, ACH_GOD_OF_WAR, kills);
    SetPlayerAchievementProgress(playerid, ACH_NOLIFE,     kills);
    return 1;
}

static stock Ach_SyncLevel(playerid, level)
{
    SetPlayerAchievementProgress(playerid, ACH_HONOURED, level);
    SetPlayerAchievementProgress(playerid, ACH_LEVEL_15, level);
    SetPlayerAchievementProgress(playerid, ACH_LEVEL_20, level);
    SetPlayerAchievementProgress(playerid, ACH_LEVEL_25, level);
    SetPlayerAchievementProgress(playerid, ACH_LEVEL_30, level);
    return 1;
}

stock Ach_OnPlayerKill(killerid, victimid, reason)
{
    if(!IsPlayerConnected(killerid) || User[killerid][user_sqlid] <= 0) return 0;

    Ach_SyncKills(killerid, User[killerid][playerKills]);

    if(reason == WEAPON_SNIPER)
        UpdatePlayerAchievementProgress(killerid, ACH_SNIPER_ELITE, 1);

    if(GetPVarInt(victimid, "LastBodyPart") == 9)
        UpdatePlayerAchievementProgress(killerid, ACH_HEADACHE, 1);

    if(0 <= victimid < MAX_PLAYERS)
        SetPlayerAchievementProgress(killerid, ACH_NEMESIS, g_PlayerKillsAgainst[killerid][victimid]);

    SetPlayerAchievementProgress(killerid, ACH_UNDYING, GetPVarInt(killerid, "Spree"));

    new tick = GetTickCount();
    if(g_AchLastKillTick[killerid] != 0 && (tick - g_AchLastKillTick[killerid]) <= ACH_DOUBLE_KILL_WINDOW)
        UpdatePlayerAchievementProgress(killerid, ACH_2_FAST, 1);
    g_AchLastKillTick[killerid] = tick;

    if(0 <= reason <= ACH_MAX_WEAPON_ID)
    {
        new cell = reason / 32, bit = 1 << (reason % 32);
        if(!(g_AchKillWeapons[killerid][cell] & bit))
        {
            g_AchKillWeapons[killerid][cell] |= bit;
            SetPlayerAchievementProgress(killerid, ACH_VARIATIVE, Ach_BitCount(g_AchKillWeapons[killerid], ACH_BITSET_CELLS));
        }
    }

    new Float:hp;
    GetPlayerHealth(killerid, hp);
    if(hp < 10.0)
        GivePlayerAchievement(killerid, ACH_STILL_DANGEROUS);

    new loc = User[killerid][locationId];
    if(0 <= loc < MAX_LOCATIONS)
    {
        SetPlayerAchievementProgress(killerid, ACH_CLEANUP, playerArenaInfo[loc][killerid][arenaKills]);

        if(Ach_IsDesertArena(arenaInfo[loc][arenaId]))
            UpdatePlayerAchievementProgress(killerid, ACH_HIGHWAY_TO_HELL, 1);
    }
    return 1;
}

stock Ach_OnPlayerDied(playerid)
{
    g_AchLastKillTick[playerid] = 0;
    g_AchAliveSince[playerid] = 0;
    return 1;
}

stock Ach_OnPlayerDamage(issuerid, victimid, Float:amount)
{
    if(amount <= 0.0) return 0;

    if(IsPlayerConnected(issuerid) && User[issuerid][user_sqlid] > 0)
    {
        g_AchDamageDealt[issuerid] += amount;
        new whole = floatround(g_AchDamageDealt[issuerid], floatround_floor);
        if(whole > 0)
        {
            g_AchDamageDealt[issuerid] -= float(whole);
            UpdatePlayerAchievementProgress(issuerid, ACH_BIG_TIME_DAMAGE, whole);
        }
    }

    if(IsPlayerConnected(victimid) && User[victimid][user_sqlid] > 0)
    {
        g_AchDamageTaken[victimid] += amount;
        new whole = floatround(g_AchDamageTaken[victimid], floatround_floor);
        if(whole > 0)
        {
            g_AchDamageTaken[victimid] -= float(whole);
            UpdatePlayerAchievementProgress(victimid, ACH_AT_WHAT_COST, whole);
        }
    }
    return 1;
}

stock Ach_OnDuelWin(playerid)
{
    GivePlayerAchievement(playerid, ACH_FIRST_VICTORY);
    SetPlayerAchievementProgress(playerid, ACH_IMPROVEMENT, User[playerid][duelWins]);
    return 1;
}

stock Ach_OnArenaMVP(playerid)
{
    GivePlayerAchievement(playerid, ACH_FIRST_VICTORY);
    SetPlayerAchievementProgress(playerid, ACH_BEST_FIGHTER, User[playerid][wonArenas]);
    return 1;
}

stock Ach_OnArenaRoundEnd(playerid)
{
    UpdatePlayerAchievementProgress(playerid, ACH_LIGHT_WORK, 1);
    return 1;
}

stock Ach_OnOutpostCaptured(playerid, participants)
{
    UpdatePlayerAchievementProgress(playerid, ACH_DREAM_TEAM, 1);

    if(participants >= 2)
    {
        UpdatePlayerAchievementProgress(playerid, ACH_DYNAMIC_DUO, 1);
        UpdatePlayerAchievementProgress(playerid, ACH_BEST_SUPPORT, 1);
    }
    return 1;
}

stock Ach_OnTrainingChallengeFinish(playerid, Float:damage, Float:accuracy, recordType)
{
    if(!IsPlayerConnected(playerid) || User[playerid][user_sqlid] <= 0) return 0;

    UpdatePlayerAchievementProgress(playerid, ACH_SHARPSHOOTER, 1);
    UpdatePlayerAchievementProgress(playerid, ACH_TARGET_ANNIHILATOR, floatround(damage));

    if(accuracy >= 95.0)
        GivePlayerAchievement(playerid, ACH_BULLSEYE);

    if(recordType > 0)
        GivePlayerAchievement(playerid, ACH_RECORD_BREAKER);

    return 1;
}

stock CheckPlayerLevelUp(playerid)
{
    if(!IsPlayerConnected(playerid) || User[playerid][user_sqlid] <= 0) return 0;

    new lvl = User[playerid][playerLevel];

    if(lvl > ACH_LEVEL_BONUS_MIN)
    {
        GivePlayerMoney(playerid, ACH_LEVEL_BONUS_MONEY);
        User[playerid][playerMoney] += ACH_LEVEL_BONUS_MONEY;
        User[playerid][playerCoins] += ACH_LEVEL_BONUS_COINS;

        va_SendClientMessage(playerid, -1, LEVEL_UP_BONUS,
            lvl, FormatMoney(ACH_LEVEL_BONUS_MONEY), FormatCoins(ACH_LEVEL_BONUS_COINS));
    }

    Ach_SyncLevel(playerid, lvl);
    return 1;
}

stock Ach_RefreshPlayerStats(playerid)
{
    if(!IsPlayerConnected(playerid) || User[playerid][user_sqlid] <= 0) return 0;

    new money = User[playerid][playerMoney];
    SetPlayerAchievementProgress(playerid, ACH_6_FIGURES,   money);
    SetPlayerAchievementProgress(playerid, ACH_CASH_MANIAC, money);
    SetPlayerAchievementProgress(playerid, ACH_RICH,        money);

    Ach_SyncKills(playerid, User[playerid][playerKills]);
    Ach_SyncLevel(playerid, User[playerid][playerLevel]);

    SetPlayerAchievementProgress(playerid, ACH_UNDYING,      User[playerid][maxStreak]);
    SetPlayerAchievementProgress(playerid, ACH_IMPROVEMENT,  User[playerid][duelWins]);
    SetPlayerAchievementProgress(playerid, ACH_BEST_FIGHTER, User[playerid][wonArenas]);

    if(User[playerid][duelWins] > 0 || User[playerid][wonArenas] > 0)
        GivePlayerAchievement(playerid, ACH_FIRST_VICTORY);

    if(User[playerid][clanID] != -1)
        GivePlayerAchievement(playerid, ACH_CALL_A_FRIEND);

    new loc = User[playerid][locationId];
    new bool:active = Ach_IsPlayerActive(playerid);
    new bool:inArena = (active && 0 <= loc < MAX_LOCATIONS && loc != ARENA_AFK && !User[playerid][isAfk]);

    if(inArena)
    {
        new zone = arenaInfo[loc][arenaId];
        if(0 <= zone < MAX_ZONES)
        {
            new cell = zone / 32, bit = 1 << (zone % 32);
            if(!(g_AchArenasPlayed[playerid][cell] & bit))
            {
                g_AchArenasPlayed[playerid][cell] |= bit;
                SetPlayerAchievementProgress(playerid, ACH_WIDE_RANGE, Ach_BitCount(g_AchArenasPlayed[playerid], ACH_BITSET_CELLS));
            }
        }

        if(g_AchAliveSince[playerid] == 0)
            g_AchAliveSince[playerid] = gettime();
        else if((gettime() - g_AchAliveSince[playerid]) >= ACH_BULLETPROOF_TIME)
            GivePlayerAchievement(playerid, ACH_BULLETPROOF);
    }
    else
    {
        g_AchAliveSince[playerid] = 0;
    }

    if(active && loc == ARENA_AFK)
    {
        if(g_AchLoungeSince[playerid] == 0)
        {
            g_AchLoungeSince[playerid] = gettime();
        }
        else
        {
            new mins = (gettime() - g_AchLoungeSince[playerid]) / 60;
            if(mins > 0)
            {
                g_AchLoungeSince[playerid] += mins * 60;
                UpdatePlayerAchievementProgress(playerid, ACH_CHILLED, mins);
            }
        }
    }
    else
    {
        g_AchLoungeSince[playerid] = 0;
    }
    return 1;
}

stock Ach_GetViewTarget(playerid)
{
    new targetid = GetPVarInt(playerid, "ach_target");

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        targetid = playerid;
        SetPVarInt(playerid, "ach_target", playerid);
    }
    return targetid;
}

stock Ach_CountUnlocked(targetid, ach_cat, &total)
{
    new count = 0;
    total = 0;
    for(new i = 0; i < ACH_COUNT; i++)
    {
        if(g_AchievementsData[i][ach_category] != ach_cat) continue;
        total++;
        if(g_UserAchUnlocked[targetid][i]) count++;
    }
    return count;
}

stock ShowAchievementsMainMenu(playerid, targetid = INVALID_PLAYER_ID)
{
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", ACHIEVEMENT_NOT_LOGGED);

    if(targetid == INVALID_PLAYER_ID) targetid = playerid;

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    SetPVarInt(playerid, "ach_target", targetid);

    new title[96], menuStr[320], line[128];
    new genTotal, genDone = Ach_CountUnlocked(targetid, ACH_CAT_GENERAL, genTotal);
    new vipTotal, vipDone = Ach_CountUnlocked(targetid, ACH_CAT_UNIQUE, vipTotal);

    if(targetid == playerid)
        strcopy(title, "{MAIN}Achievements System", sizeof(title));
    else
        format(title, sizeof(title), "{MAIN}Achievements: {FFFFFF}%s", User[targetid][userName]);

    format_MainColor(playerid, title);

    strcopy(menuStr, "{MAIN}Category\t{MAIN}Unlocked\n", sizeof(menuStr));

    format(line, sizeof(line), "{MAIN}\xBB {FFFFFF}General Achievements\t{FFFFFF}%d{BEBEBE}/%d\n", genDone, genTotal);
    strcat(menuStr, line);

    format(line, sizeof(line), "{MAIN}\xBB {FFFFFF}Exclusive Achievements\t{FFFFFF}%d{BEBEBE}/%d", vipDone, vipTotal);
    strcat(menuStr, line);

    format_MainColor(playerid, menuStr);

    Dialog_ShowCallback(playerid, using public DIALOG_ACHIEVEMENTS_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, menuStr, "Select", "Close");
    return 1;
}

public DIALOG_ACHIEVEMENTS_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0: ShowAchievementsCategoryList(playerid, ACH_CAT_GENERAL);
            case 1: ShowAchievementsCategoryList(playerid, ACH_CAT_UNIQUE);
        }
    }
    return 1;
}

stock ShowAchievementsCategoryList(playerid, ach_cat)
{
    if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return 0;

    if(ach_cat == ACH_CAT_UNIQUE && !IsServerOwner(playerid))
        return SendClientMessage(playerid, -1, "%s", ACHIEVEMENT_EXCLUSIVE);

    SetPVarInt(playerid, "ach_current_cat", ach_cat);

    new targetid = Ach_GetViewTarget(playerid);
    new title[96];

    if(targetid == playerid)
    {
        strcopy(title, (ach_cat == ACH_CAT_GENERAL)
            ? ("{MAIN}\xBB {FFFFFF}General Achievements")
            : ("{MAIN}\xBB {FFFFFF}Exclusive Achievements"), sizeof(title));
    }
    else
    {
        format(title, sizeof(title), "{MAIN}%s: {FFFFFF}%s",
            (ach_cat == ACH_CAT_GENERAL) ? ("General") : ("Exclusive"), User[targetid][userName]);
    }

    format_MainColor(playerid, title);

    g_achDialogBuf[0] = '\0';
    strcat(g_achDialogBuf, "{MAIN}Achievement\t{MAIN}Status\n", sizeof(g_achDialogBuf));

    new count = 0, line[192], nameStr[128];

    for(new i = 0; i < ACH_COUNT; i++)
    {
        if(g_AchievementsData[i][ach_category] != ach_cat) continue;

        new bool:unlocked = HasPlayerAchievement(targetid, i);

        if(g_AchievementsData[i][ach_target] > 1 && !unlocked)
        {
            format(nameStr, sizeof(nameStr), "{FFFFFF}%s  {BEBEBE}%d/%d",
                g_AchievementsData[i][ach_name], g_UserAchProgress[targetid][i], g_AchievementsData[i][ach_target]);
        }
        else
        {
            format(nameStr, sizeof(nameStr), "{FFFFFF}%s", g_AchievementsData[i][ach_name]);
        }

        format(line, sizeof(line), "%s\t%s\n", nameStr, unlocked ? (ACH_ICON_DONE) : (ACH_ICON_LOCKED));
        strcat(g_achDialogBuf, line, sizeof(g_achDialogBuf));
        count++;
    }

    if(count == 0)
        strcat(g_achDialogBuf, "{BEBEBE}No achievements in this category\t{BEBEBE}None\n", sizeof(g_achDialogBuf));

    format_MainColor(playerid, g_achDialogBuf, sizeof(g_achDialogBuf));

    Dialog_ShowCallback(playerid, using public DIALOG_ACHIEVEMENTS_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, g_achDialogBuf, "Select", "Back");
    return 1;
}

public DIALOG_ACHIEVEMENTS_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new target_cat = GetPVarInt(playerid, "ach_current_cat");
        new matchCount = 0;

        for(new i = 0; i < ACH_COUNT; i++)
        {
            if(g_AchievementsData[i][ach_category] != target_cat) continue;

            if(matchCount == listitem)
            {
                ShowAchievementDetail(playerid, i);
                return 1;
            }

            matchCount++;
        }
    }

    ShowAchievementsMainMenu(playerid, Ach_GetViewTarget(playerid));
    return 1;
}

stock ShowAchievementDetail(playerid, ach_index)
{
    if(!IsPlayerConnected(playerid) || ach_index < 0 || ach_index >= ACH_COUNT) return 0;

    new targetid = Ach_GetViewTarget(playerid);

    new title[96], body[512], statusStr[128];
    format(title, sizeof(title), "{MAIN}Achievement: {FFFFFF}%s", g_AchievementsData[ach_index][ach_name]);
    format_MainColor(playerid, title);

    new bool:unlocked = HasPlayerAchievement(targetid, ach_index);

    if(unlocked)
    {
        format(statusStr, sizeof(statusStr), ACH_ICON_DONE" {BEBEBE}(%s)", g_UserAchDate[targetid][ach_index]);
    }
    else if(g_AchievementsData[ach_index][ach_category] == ACH_CAT_UNIQUE)
    {
        strcopy(statusStr, ACH_ICON_LOCKED" {BEBEBE}(granted by the server owner)", sizeof(statusStr));
    }
    else if(g_AchievementsData[ach_index][ach_target] > 1)
    {
        format(statusStr, sizeof(statusStr), ACH_ICON_LOCKED" {BEBEBE}(%d/%d)",
            g_UserAchProgress[targetid][ach_index], g_AchievementsData[ach_index][ach_target]);
    }
    else
    {
        strcopy(statusStr, ACH_ICON_LOCKED, sizeof(statusStr));
    }

    format(body, sizeof(body),
        "{BEBEBE}Status: %s\n\n" \
        "{BEBEBE}Objective:\n{FFFFFF}%s\n\n" \
        "{BEBEBE}Reward:\n{23C552}%s {FFFFFF}| {FFFF00}%s",
        statusStr,
        g_AchievementsData[ach_index][ach_desc],
        FormatMoney(g_AchievementsData[ach_index][ach_reward_money]),
        FormatCoins(g_AchievementsData[ach_index][ach_reward_coins])
    );
    format_MainColor(playerid, body);

    Dialog_ShowCallback(playerid, using public DIALOG_ACHIEVEMENTS_DETAIL<iiiis>, DIALOG_STYLE_MSGBOX, title, body, "Back", "Close");
    return 1;
}

public DIALOG_ACHIEVEMENTS_DETAIL(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
        ShowAchievementsCategoryList(playerid, GetPVarInt(playerid, "ach_current_cat"));

    return 1;
}

@cmd(.alts = {"darconquista"}) giveach(playerid, params[], help)
{
    if(!IsServerOwner(playerid))
        return SendClientMessage(playerid, -1, "%s", ACHIEVEMENT_GIVE_OWNER);

    new targetid, achid;
    if(sscanf(params, "ui", targetid, achid))
    {
        SendClientMessage(playerid, -1, "%s", ACHIEVEMENT_GIVE_HELP);

        g_achDialogBuf[0] = '\0';
        strcat(g_achDialogBuf, "{MAIN}ID\t{MAIN}Achievement\t{MAIN}Reward\n", sizeof(g_achDialogBuf));

        new line[192];
        for(new i = 0; i < ACH_COUNT; i++)
        {
            if(g_AchievementsData[i][ach_category] != ACH_CAT_UNIQUE) continue;

            format(line, sizeof(line), "{BEBEBE}%d\t{FFFFFF}%s\t{BEBEBE}%s / %s\n",
                i, g_AchievementsData[i][ach_name],
                FormatMoney(g_AchievementsData[i][ach_reward_money]), FormatCoins(g_AchievementsData[i][ach_reward_coins]));
            strcat(g_achDialogBuf, line, sizeof(g_achDialogBuf));
        }

        new title[64] = "{MAIN}Exclusive Achievements";
        format_MainColor(playerid, title);
        format_MainColor(playerid, g_achDialogBuf, sizeof(g_achDialogBuf));

        Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, g_achDialogBuf, "Close", "");
        return true;
    }

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(achid < 0 || achid >= ACH_COUNT)
        return SendClientMessage(playerid, -1, "%s", ACHIEVEMENT_GIVE_INVALID);

    if(HasPlayerAchievement(targetid, achid))
    {
        return va_SendClientMessage(playerid, -1, ACHIEVEMENT_GIVE_HAS,
            User[targetid][userName], g_AchievementsData[achid][ach_name]);
    }

    GivePlayerAchievement(targetid, achid, true);

    va_SendClientMessage(playerid, -1, ACHIEVEMENT_GIVE_DONE,
        g_AchievementsData[achid][ach_name], User[targetid][userName]);

    va_SendClientMessage(targetid, -1, ACHIEVEMENT_GIVE_GOT,
        User[playerid][userName], g_AchievementsData[achid][ach_name]);

    new log_details[192];
    format(log_details, sizeof(log_details), "%s granted achievement '%s' to %s",
        User[playerid][userName], g_AchievementsData[achid][ach_name], User[targetid][userName]);
    LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "achievements", "Grant", log_details);
    return true;
}

@cmd(.alts = {"saldo"}) coins(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    va_SendClientMessage(playerid, -1, COINS_BALANCE, FormatCoins(User[playerid][playerCoins]));
    return true;
}

@cmd(.alts = {"conquistas", "achievements"}) ach(playerid, params[], help)
{
    ShowAchievementsMainMenu(playerid);
    return true;
}

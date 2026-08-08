#if defined _clan_enums_included
    #endinput
#endif
#define _clan_enums_included

forward RewardClanTerritories();
forward GetPlayerIDBySQLID(sqlid);

forward OnClanDeleted(clanid);

forward DIALOG_CLANMENU(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLANS_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_RECRUIT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLANMEMBERS_PAGED(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_EDITMEMBER(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_SETTINGS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_ROLES(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_EDIT_ROLE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_EDIT_NAME(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_EDIT_TAG(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_COLOR_TYPE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_DEFAULT_COLORS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_EDIT_COLOR(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_CLAN_LOGS_BACK(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_TERRITORIES(playerid, dialogid, response, listitem, string:inputtext[]);

enum e_CLAN
{
    clanID,
    bool:clanIDUsed,
    clanLeader[MAX_PLAYER_NAME + 1],
    clanName[CLAN_NAME_LEN],
    clanTag[CLAN_TAG_LEN],
    clanKills,
    clanArenaKills[MAX_LOCATIONS],
    clanArenaDMG[MAX_LOCATIONS],
    clanPoints,
    clanColor,
    clanCreatedAt
};

enum e_ClanMembers
{
    member_id,
    member_name[MAX_PLAYER_NAME + 1],
    member_role,
    member_score,
    bool:member_slotused
};

enum e_ClanRankEntry
{
    rank_clanID,
    rank_clanPoints
};

new ClanInfo[MAX_CLANS][e_CLAN];
new ClanRank[MAX_CLANS][e_ClanRankEntry];
new ClanRoles[MAX_CLANS][MAX_ROLES][CLAN_ROLE_LEN];
new ClanMembers[MAX_CLANS][MAX_MEMBERS][e_ClanMembers];
new ClanMembersCount[MAX_CLANS] = 0;
new bool:g_ClansLoaded = false;

new const ClanDefaultRoles[MAX_ROLES][CLAN_ROLE_LEN] =
{
    "Trial", "Member", "Recruter", "Co Founder", "Founder"
};

new clanTop3Color[3][7] = {
    "2a2a2a",//black
    "E63114",//Orange
    "14e6e0"//cyan
};

new const DefaultClanColors[] = {
    0x00FF00AA, // Green
    0xFF0000AA, // Red
    0x0000FFAA, // Blue
    0x804000AA, // Brown
    0xFFFFFFAA, // White
    0xFF8000AA, // Orange
    0x80FFFFAA, // Navy Blue
    0xC0C0C0AA, // Gray
    0xFF80FFAA, // Pink
    0x80BFFFAA, // Light Blue
    0x007138AA, // Dark Green
    0xFFFF00AA, // Yellow
    0x9F009FAA  // Purple
};

new const DefaultClanColorNames[][] = {
    "Green",
    "Red",
    "Blue",
    "Brown",
    "White",
    "Orange",
    "Navy Blue",
    "Gray",
    "Pink",
    "Light Blue",
    "Dark Green",
    "Yellow",
    "Purple"
};

// clans listed in the last /clans dialog, indexed by list item
new g_PlayerClansList[MAX_PLAYERS][MAX_CLANS];

// clan whose recruitment dialog the player is looking at, -1 when none
new g_PlayerSelectedClan[MAX_PLAYERS] = {-1, ...};

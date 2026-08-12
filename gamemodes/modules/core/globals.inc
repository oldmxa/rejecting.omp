//Arena
enum Arena
{
	arenaId,
	roundMints,
	maxPlayers,
	bool:isRunningArena,
	totalArenaKills
}

new arenaInfo[MAX_LOCATIONS][Arena];
new ServerHostname[64] = "rejecting.files";

enum
{
    ARENA_DM_1,
    ARENA_DM_2,
    ARENA_AFK,
    ARENA_CUSTOM
}

enum playerArena
{
    Float:arenaDMG,
    firedShots,
    hitedShots,
    arenaKills,
    arenaDeaths,
    arenaExp,
    arenaHeadshots
}

new playerArenaInfo[MAX_LOCATIONS][MAX_PLAYERS + 1][playerArena];

Float:GetPlayerAccuracy(playerid, location)
{
    if(location < 0 || location >= MAX_LOCATIONS) return 0.0;
    if(playerArenaInfo[location][playerid][firedShots] <= 0) return 0.0;
    
    new hits = playerArenaInfo[location][playerid][hitedShots];
    new shots = playerArenaInfo[location][playerid][firedShots];
    
    if(hits > shots) hits = shots;
    
    return (float(hits) / float(shots)) * 100.0;
}

enum
{
    ARENA_CATEGORY_SMALL,
    ARENA_CATEGORY_MEDIUM,
    ARENA_CATEGORY_BIG,
    ARENA_CATEGORY_CUSTOM,
    MAX_ARENA_CATEGORIES
}

enum arenaa{
	arenaName[30 + 1],
	Float:arenaPos[4],
    category,
    Float:arenaFloorZ, // 0.0 = arena on the real map
    arenaInterior
};


new OWNER_COMMANDS_DISCORD[][] = {
    "1184577106332631074",  // oldmxa
    "1460839538640551946"  // rejecting.files
};

//Global Variables


enum admNames {
    ingame[22],
    discord[14]
};

new adminnames[6][admNames] =
{
    {"{B2ACAC}No admin", "No admin"},
	{"{9A2EFE}Operator", "Operator"},
	{"{FF8000}Moderator", "Moderator"},
	{"{3098ff}Administrator", "Administrator"},
	{"{C00101}Coordinator", "Coordinator"},
	{"{2e5b26}Developer", "Developer"}
};

// VIP Tag Color Presets
stock const vipColorListHex[] = {
    0x0022FFFF, // 0: {0022FF}
    0x008CFFFF, // 1: {008CFF}
    0x09FF00FF, // 2: {09FF00}
    0xFF7700FF, // 3: {FF7700}
    0xFF0000FF, // 4: {FF0000}
    0xFFFB00FF, // 5: {FFFB00}
    0xFF00FBFF, // 6: {FF00FB}
    0x4D3F4CFF, // 7: {4D3F4C}
    0x00FFA2FF, // 8: {00FFA2}
    0x7300FFFF, // 9: {7300FF}
    0x00000000  // 10: Hidden
};

stock const vipColorListStr[][] = {
    "0022FF",
    "008CFF",
    "09FF00",
    "FF7700",
    "FF0000",
    "FFFB00",
    "FF00FB",
    "4D3F4C",
    "00FFA2",
    "7300FF"
};
new const g_DiscordAdminRoles[MAX_ADMIN_LEVEL][] = {
	DISCORD_ROLE_OPERATOR,
	DISCORD_ROLE_MODERATOR,
	DISCORD_ROLE_ADMIN,
	DISCORD_ROLE_COORD,
	DISCORD_ROLE_DEV
};

new wName[55][18] = {
    "none",
    "fist",
    "Golf Club",
    "Nightstick",
    "Knife",
    "Baseball Bat",
    "Shovel",
    "Pool Cue",
    "Katana",
    "Chainsaw",
    "Dildo",
    "Dildo",
    "Vibrator",
    "Vibrator",
    "Flowers",
    "Cane",
    "Grenade",
    "Tear Gas",
    "Molotov",
    "NONE", //20
    "NONE",
    "NONE",
    "9mm",
    "Silenced",
    "Deagle",
    "Shotgun",
    "Sawnoff",
    "Spas-12",
    "Micro SMG",
    "MP5",
    "AK-47",
    "M4",
    "Tec-9",
    "Country Rifle",
    "Sniper",
    "RPG",
    "HS",
    "Flame",
    "Minigun",
    "Satchel",
    "Detonator",
    "Spray Can",
    "Fire Extinguisher",
    "Camera",
    "Night Vis",
    "Thermal",
    "Parachute",
    "47",
    "48",
    "49",
    "50",
    "Explosion",
    "52",
    "53",
    "Explosion"
};

new VehicleNames[212][18] = {
    "Landstalker",
    "Bravura",
    "Buffalo",
    "Linerunner",
    "Pereniel",
    "Sentinel",
    "Dumper",
    "Firetruck",
    "Trashmaster",
    "Stretch",
    "Manana",
    "Infernus",
    "Voodoo",
    "Pony",
    "Mule",
    "Cheetah",
    "Ambulance",
    "Leviathan",
    "Moonbeam",
    "Esperanto",
    "Taxi",
    "Washington",
    "Bobcat",
    "Mr Whoopee",
    "BF Injection",
    "Hunter",
    "Premier",
    "Enforcer",
    "Securicar",
    "Banshee",
    "Predator",
    "Bus",
    "Rhino",
    "Barracks",
    "Hotknife",
    "Trailer",
    "Previon",
    "Coach",
    "Cabbie",
    "Stallion",
    "Rumpo",
    "RC Bandit",
    "Romero",
    "Packer",
    "Monster",
    "Admiral",
    "Squalo",
    "Seasparrow",
    "Pizzaboy",
    "Tram",
    "Trailer",
    "Turismo",
    "Speeder",
    "Reefer",
    "Tropic",
    "Flatbed",
    "Yankee",
    "Caddy",
    "Solair",
    "Berkley's RC Van",
    "Skimmer",
    "PCJ-600",
    "Faggio",
    "Freeway",
    "RC Baron",
    "RC Raider",
    "Glendale",
    "Oceanic",
    "Sanchez",
    "Sparrow",
    "Patriot",
    "Quad",
    "Coastguard",
    "Dinghy",
    "Hermes",
    "Sabre",
    "Rustler",
    "ZR3 50",
    "Walton",
    "Regina",
    "Comet",
    "BMX",
    "Burrito",
    "Camper",
    "Marquis",
    "Baggage",
    "Dozer",
    "Maverick",
    "News Chopper",
    "Rancher",
    "FBI Rancher",
    "Virgo",
    "Greenwood",
    "Jetmax",
    "Hotring",
    "Sandking",
    "Blista Compact",
    "Police Maverick",
    "Boxville",
    "Benson",
    "Mesa",
    "RC Goblin",
    "Hotring Racer A",
    "Hotring Racer B",
    "Bloodring Banger",
    "Rancher",
    "Super GT",
    "Elegant",
    "Journey",
    "Bike",
    "Mountain Bike",
    "Beagle",
    "Cropdust",
    "Stunt",
    "Tanker",
    "RoadTrain",
    "Nebula",
    "Majestic",
    "Buccaneer",
    "Shamal",
    "Hydra",
    "FCR-900",
    "NRG-500",
    "HPV1000",
    "Cement Truck",
    "Tow Truck",
    "Fortune",
    "Cadrona",
    "FBI Truck",
    "Willard",
    "Forklift",
    "Tractor",
    "Combine",
    "Feltzer",
    "Remington",
    "Slamvan",
    "Blade",
    "Freight",
    "Streak",
    "Vortex",
    "Vincent",
    "Bullet",
    "Clover",
    "Sadler",
    "Firetruck",
    "Hustler",
    "Intruder",
    "Primo",
    "Cargobob",
    "Tampa",
    "Sunrise",
    "Merit",
    "Utility",
    "Nevada",
    "Yosemite",
    "Windsor",
    "Monster A",
    "Monster B",
    "Uranus",
    "Jester",
    "Sultan",
    "Stratum",
    "Elegy",
    "Raindance",
    "RC Tiger",
    "Flash",
    "Tahoma",
    "Savanna",
    "Bandito",
    "Freight",
    "Trailer",
    "Kart",
    "Mower",
    "Duneride",
    "Sweeper",
    "Broadway",
    "Tornado",
    "AT-400",
    "DFT-30",
    "Huntley",
    "Stafford",
    "BF-400",
    "Newsvan",
    "Tug",
    "Trailer A",
    "Emperor",
    "Wayfarer",
    "Euros",
    "Hotdog",
    "Club",
    "Trailer B",
    "Trailer C",
    "Andromada",
    "Dodo",
    "RC Cam",
    "Launch",
    "Police Car (LSPD)",
    "Police Car (SFPD)",
    "Police Car (LVPD)",
    "Police Ranger",
    "Picador",
    "S.W.A.T. Van",
    "Alpha",
    "Phoenix",
    "Glendale",
    "Sadler",
    "Luggage Trailer A",
    "Luggage Trailer B",
    "Stair Trailer",
    "Boxville",
    "Farm Plow",
    "Utility Trailer"
};

new pVehicle[MAX_PLAYERS] = {999, ...};
// INVALID_OBJECT_ID e nao 0: object id 0 e valido no SA-MP, entao 0 como
new objecthporarmour[MAX_PLAYERS] = {INVALID_OBJECT_ID, ...};
new hpandarmourtimer[MAX_PLAYERS];

new playerSpecID[MAX_PLAYERS + 1];

// CLAN: modules/clan/enums.inc

enum e_USER
{
    user_sqlid,
    user_pass[64 + 1],
    user_regip[18],
    user_lastip[18],
    user_gpci[41],
    user_regdate[32],
    FPS,
    dLlast,
    userName[MAX_PLAYER_NAME + 1],
    playerKills,
    kills_semanais,
    playerDeaths,
    playerExp,
    playerLevel,
    playerSkin,
    fightStyle,
    adminLevel,
    playerMoney,
    playerCoins,
    maxDamage,
    hudColor,
    chatColor,
    chatIDColor,
    vipLevel,
    vipExpire,
    vipHeadTag,
    hitmarkColor,
    time,
    weather,
    locationId,
    lastLocation,
    hitmarkTimer,
    timerBox,
    muteTimestamp,
    lastLogin,
    loginTime,
    lastTimeUpdated,
    spreeTime,
    playerWeapons[4],
    lastDamagerId,
    lastDamageTime,
    currentZone,
    totalOnTime,
    maxStreak,
    wonArenas,
    duelWins,
    duelLosses,
    duel2v2Wins,
    duel2v2Losses,
    warnings,
    killsSession,
    deathsSession,
    Float:dmgSession,
    dmgSemanal,
	playerVersion[24],

    // Respawn

    timerRespawn,

    //CLAN

    clanID,
    clanRole,
    clanIDInvite,
    clanInviteTime,
    clanSlotID,
    outpostWarnCooldown,

    // Booleans

    bool:ForcedR2,
    bool:showHeadshot,
    bool:showConnectMsg,
    bool:hideGlobalMsg,
    bool:hideWelcomeMsg,
    bool:hideVipChat,
    bool:hideVipTag,
    bool:hideClanTag,
    bool:hideAdminTag,
    bool:spawnSafehouse,
    bool:hideCustomTag,
    bool:customTagPerm,
    customTag[33],
    bool:show3DDeathTag,
    bool:showRandomDeathTags,
    bool:isLogged,
    bool:isAlreadySpawned,
    dmgInformerStyle,
    hitSoundID,
    killSoundID,
    hudType,
    lifeType,
    bool:showPlayerHud,
    bool:showClanHud,
    bool:blockPM,
    bool:isShowingHud,
    bool:isSpectating,
    bool:isPiscando,
    bool:isAfk,
    bool:isFrozen,
    bool:isShowingKSTD,
    bool:isMobile,
    bool:OnTempREC,
    bool:adminCamu,
    bool:autoLogin,
    // Lounge
    Float:loungeX,
    Float:loungeY,
    Float:loungeZ,
    Float:loungeA,
    loungeVW,
    loungeINT,

    // Anti-VPN

    City[64],
    Region[64],
    RegionCode[3],
    Country[64],
    CountryCode[3],
    Latitude[15],
    Longitude[15],
    TimeZone[30],
    Network[25],
    IspNumber[25],
    Isp[64],
    bool:Proxy,
    bool:VPN,

    // /newarena
    bool:isCreatingArena,
    newArenaStep,
    Float:newArenaPos[4],
    newArenaZone,
    VisualArea:newArenaVisualArea,
    newArenaTimer,

    // DISCORD SYNC
    dc_code[101],
    dc_username[101],
    dc_discriminator[101],
    dc_uid[101],
    dc_codetimer,

    // ADMIN CONFIG
    bool:acfg_pm,
    bool:acfg_clanpm,
    bool:acfg_cmds,
    bool:acfg_local,

    // Admin Auth System
    bool:isAdminLogged,
    adminAuthCode,
    adminAuthTimer,
    adminAuthType,
    adminLoginTimer,
    adminAuthAttempts,
    adminPass[BCRYPT_HASH_LENGTH],

    // Login System
    loginAttempts,
    loginTimer,

    // Interior System
    int_InteriorSelecionado,
    Float:int_PosX,
    Float:int_PosY,
    Float:int_PosZ,
    Float:int_PosFA,
    int_VW,
    int_PInterior
};

new User[MAX_PLAYERS + 1][e_USER];

new pickupID[MAX_PICKUPSS] = {-1, ...};
new pickupData[MAX_PICKUPSS];
new pickupTime[MAX_PICKUPSS];

new pickupWeapon[MAX_PICKUPSS];


new msgdiscord[512], msgencoded[1024], stringdiscord[512]; // strings discord

new StringToCat[MAX_PLAYERS][MAX_LINES][64+1];
new lines[MAX_PLAYERS];

enum coloringHud {
    colorOne[7],
    colorTwo[7],
    lineInfo[15]
};

new hudPersonalization[30][coloringHud] =
{
    {"~w~~h~",      "~g~",      "white green"},
    {"~w~~h~",      "~r~~h~",   "white red"},
    {"~w~~h~",      "~b~~h~",   "white blue"},
    {"~w~~h~",      "~y~~h~",   "white yellow"},
    {"~w~~h~",      "~p~~h~",   "white pink"},

    {"~r~~h~",    "~w~~h~",  "red white"},
    {"~r~~h~",    "~g~",     "red green"},
    {"~r~~h~",    "~b~~h~",  "red blue"},
    {"~r~~h~",    "~y~~h~",  "red yellow"},
    {"~r~~h~",    "~p~~h~",  "red pink"},

    {"~g~",      "~w~~h~",   "green white"},
    {"~g~",      "~r~~h~",   "green red"},
    {"~g~",      "~b~~h~",   "green blue"},
    {"~g~",      "~y~~h~",   "green yellow"},
    {"~g~",      "~p~~h~",   "green pink"},

    {"~b~~h~",    "~w~~h~",   "blue white"},
    {"~b~~h~",    "~g~",     "blue green"},
    {"~b~~h~",    "~r~~h~",  "blue red"},
    {"~b~~h~",    "~y~~h~",  "blue yellow"},
    {"~b~~h~",    "~p~~h~",  "blue pink"},

    {"~y~~h~",    "~w~~h~",  "yellow white"},
    {"~y~~h~",    "~g~",     "yellow green"},
    {"~y~~h~",    "~r~~h~",  "yellow red"},
    {"~y~~h~",    "~b~~h~",  "yellow blue"},
    {"~y~~h~",    "~p~~h~",  "yellow pink"},

    {"~p~~h~",    "~w~~h~",  "pink white"},
    {"~p~~h~",    "~g~",     "pink green"},
    {"~p~~h~",    "~r~~h~",  "pink red"},
    {"~p~~h~",    "~b~~h~",  "pink blue"},
    {"~p~~h~",    "~y~~h~",  "pink yellow"}
};

#define HUD_COLOR_DEFAULT   (0) // "white green"

enum chatColorStruct {
    chatColorHex[7],
    chatColorName[24]
};

new chatPersonalization[16][chatColorStruct] =
{
    {"FFFFFF", "White Default"},
    {"00FF00", "Bright Green"},
    {"FF0000", "Bright Red"},
    {"00BFFF", "Sky Blue"},
    {"FFFF00", "Yellow"},
    {"FF00FF", "Pink / Magenta"},
    {"00FFFF", "Cyan"},
    {"FFA500", "Orange"},
    {"A020F0", "Purple"},
    {"FFD700", "Gold"},
    {"32CD32", "Lime Green"},
    {"FF69B4", "Hot Pink"},
    {"1E90FF", "Dodger Blue"},
    {"FF4500", "Orange Red"},
    {"CCCCCC", "Light Grey"},
    {"404EA4", "Royal Blue"}
};

enum gunmenuEnum {
    weaponID,
    weaponAmmo
};

new CoresDoBglh[30][7] = 
{
	"2e5b26", "F3230E", "404ea4", "dcdd81", "db93cb", // 0-4 (White base)
	"F3230E", "F3230E", "F3230E", "F3230E", "F3230E", // 5-9 (Red base)
	"2e5b26", "2e5b26", "2e5b26", "2e5b26", "2e5b26", // 10-14 (Green base)
	"404ea4", "404ea4", "404ea4", "404ea4", "404ea4", // 15-19 (Blue base)
	"dcdd81", "dcdd81", "dcdd81", "dcdd81", "dcdd81", // 20-24 (Yellow base)
	"db93cb", "db93cb", "db93cb", "db93cb", "db93cb"  // 25-29 (Pink base)
};

new gunmenuWInfo[5][gunmenuEnum] =
{
    {0, 0},
    {24, 900},
    {25, 200},
    {34, 100},
    {31, 250}
};

#if !defined FIGHT_STYLE
    #define FIGHT_STYLE: _:
#endif

enum styles {
    fightname[10],
    FIGHT_STYLE:styleid
}

new fightStyles[6][styles] =
{
    {"Normal", FIGHT_STYLE_NORMAL},
    {"Boxing", FIGHT_STYLE_BOXING},
    {"Kung fu", FIGHT_STYLE_KUNGFU},
    {"Knee head", FIGHT_STYLE_KNEEHEAD},
    {"Grab kick", FIGHT_STYLE_GRABKICK},
    {"Elbow", FIGHT_STYLE_ELBOW}
};

enum playerVote{
    bool:alreadyVoted,
    bool:voteChoice
}

new playerVoteInfo[MAX_PLAYERS][playerVote];

enum globalVote{
    bool:showingVoteTD,
    bool:isRunningVote,
    votesYes,
    votesNo,
    votingTime
}

new votingInfo[globalVote];

new piscaTimer[MAX_PLAYERS + 1];

new Text3D:ClanLabel[MAX_PLAYERS] = {Text3D:INVALID_3DTEXT_ID, ...};
new Text3D:VipLabel[MAX_PLAYERS] = {Text3D:INVALID_3DTEXT_ID, ...};


new Iterator:ArenaQueue<MAX_ARENA_CATEGORIES, MAX_ZONES>;
new changeArenaDelay = -1;

new g_Object;

new bool:R2KickEnabled = false;
forward IsR2KickEnabled();

new bool:ACPunishEnabled = true;

new bool:ACNoFallDamage = true;
new bool:ACNoFallAnimation = true;
new bool:ACFastStuns = false;

new bool:ServerLocked = false;
new ServerMaxPing = MAX_PING;
new Float:ServerMaxPacketLoss = MAX_PL;
new ServerMinFPS = MIN_FPS;
new ServerMaxFPS = MAX_FPS;
new ServerDefaultHud = 1; // 0: hud_rejecting, 1: hud_underline
new ServerDefaultHudColor = 0;

new MySQL: dbHandle;
new queryGlobal[4096];

// global buffers (avoids stack overflow)
#define ACH_SERIAL_BUF_SIZE (MAX_ACHIEVEMENTS * 48 + 16)
new g_achSerialBuf[ACH_SERIAL_BUF_SIZE];
new g_achQueryBuf[ACH_SERIAL_BUF_SIZE + 80];
new g_achDialogBuf[4096];

new randomcolor[500] =
{
    0xFF8C13AA,0xC715FFAA,0x20B2AAAA,0xDC143CAA,0x6495EDAA,0xf0e68cAA,0x778899AA,0xFF1493AA,0xF4A460AA,0xEE82EEAA,0xFFD720AA,
    0x8b4513AA,0x4949A0AA,0x148b8bAA,0x14ff7fAA,0x556b2fAA,0x0FD9FAAA,0x10DC29AA,0x534081AA,0x0495CDAA,0xEF6CE8AA,0xBD34DAAA,
    0x247C1BAA,0x0C8E5DAA,0x635B03AA,0xCB7ED3AA,0x65ADEBAA,0x5C1ACCAA,0xF2F853AA,0x11F891AA,0x7B39AAAA,0x53EB10AA,0x54137DAA,
    0x275222AA,0xF09F5BAA,0x3D0A4FAA,0x22F767AA,0xD63034AA,0x9A6980AA,0xDFB935AA,0x3793FAAA,0x90239DAA,0xE9AB2FAA,0xAF2FF3AA,
    0x057F94AA,0xB98519AA,0x388EEAAA,0x028151AA,0xA55043AA,0x0DE018AA,0x93AB1CAA,0x95BAF0AA,0x369976AA,0x18F71FAA,0x4B8987AA,
    0x491B9EAA,0x829DC7AA,0xBCE635AA,0xCEA6DFAA,0x20D4ADAA,0x2D74FDAA,0x3C1C0DAA,0x12D6D4AA,0x48C000AA,0x2A51E2AA,0xE3AC12AA,
    0xFC42A8AA,0x2FC827AA,0x1A30BFAA,0xB740C2AA,0x42ACF5AA,0x2FD9DEAA,0xFAFB71AA,0x05D1CDAA,0xC471BDAA,0x94436EAA,0xC1F7ECAA,
    0xCE79EEAA,0xBD1EF2AA,0x93B7E4AA,0x3214AAAA,0x184D3BAA,0xAE4B99AA,0x7E49D7AA,0x4C436EAA,0xFA24CCAA,0xCE76BEAA,0xA04E0AAA,
    0x9F945CAA,0xDCDE3DAA,0x10C9C5AA,0x70524DAA,0x0BE472AA,0x8A2CD7AA,0x6152C2AA,0xCF72A9AA,0xE59338AA,0xEEDC2DAA,0xD8C762AA,
    0x3FE65CAA,0xFF8C13AA,0xC715FFAA,0x20B2AAAA,0xDC143CAA,0x6495EDAA,0xf0e68cAA,0x778899AA,0xFF1493AA,0xF4A460AA,0xEE82EEAA,
    0xFFD720AA,0x8b4513AA,0x4949A0AA,0x148b8bAA,0x14ff7fAA,0x556b2fAA,0x0FD9FAAA,0x10DC29AA,0x534081AA,0x0495CDAA,0xEF6CE8AA,
    0xBD34DAAA,0x247C1BAA,0x0C8E5DAA,0x635B03AA,0xCB7ED3AA,0x65ADEBAA,0x5C1ACCAA,0xF2F853AA,0x11F891AA,0x7B39AAAA,0x53EB10AA,
    0x54137DAA,0x275222AA,0xF09F5BAA,0x3D0A4FAA,0x22F767AA,0xD63034AA,0x9A6980AA,0xDFB935AA,0x3793FAAA,0x90239DAA,0xE9AB2FAA,
    0xAF2FF3AA,0x057F94AA,0xB98519AA,0x388EEAAA,0x028151AA,0xA55043AA,0x0DE018AA,0x93AB1CAA,0x95BAF0AA,0x369976AA,0x18F71FAA,
    0x4B8987AA,0x491B9EAA,0x829DC7AA,0xBCE635AA,0xCEA6DFAA,0x20D4ADAA,0x2D74FDAA,0x3C1C0DAA,0x12D6D4AA,0x48C000AA,0x2A51E2AA,
    0xE3AC12AA,0xFC42A8AA,0x2FC827AA,0x1A30BFAA,0xB740C2AA,0x42ACF5AA,0x2FD9DEAA,0xFAFB71AA,0x05D1CDAA,0xC471BDAA,0x94436EAA,
    0xC1F7ECAA,0xCE79EEAA,0xBD1EF2AA,0x93B7E4AA,0x3214AAAA,0x184D3BAA,0xAE4B99AA,0x7E49D7AA,0x4C436EAA,0xFA24CCAA,0xCE76BEAA,
    0xA04E0AAA,0x9F945CAA,0xDCDE3DAA,0x10C9C5AA,0x70524DAA,0x0BE472AA,0x8A2CD7AA,0x6152C2AA,0xCF72A9AA,0xE59338AA,0xEEDC2DAA,
    0xD8C762AA,0x3FE65CAA,0xFF8C13AA,0xC715FFAA,0x20B2AAAA,0xDC143CAA,0x6495EDAA,0xf0e68cAA,0x778899AA,0xFF1493AA,0xF4A460AA,
    0xEE82EEAA,0xFFD720AA,0x8b4513AA,0x4949A0AA,0x148b8bAA,0x14ff7fAA,0x556b2fAA,0x0FD9FAAA,0x10DC29AA,0x534081AA,0x0495CDAA,
    0xEF6CE8AA,0xBD34DAAA,0x247C1BAA,0x0C8E5DAA,0x635B03AA,0xCB7ED3AA,0x65ADEBAA,0x5C1ACCAA,0xF2F853AA,0x11F891AA,0x7B39AAAA,
    0x53EB10AA,0x54137DAA,0x275222AA,0xF09F5BAA,0x3D0A4FAA,0x22F767AA,0xD63034AA,0x9A6980AA,0xDFB935AA,0x3793FAAA,0x90239DAA,
    0xE9AB2FAA,0xAF2FF3AA,0x057F94AA,0xB98519AA,0x388EEAAA,0x028151AA,0xA55043AA,0x0DE018AA,0x93AB1CAA,0x95BAF0AA,0x369976AA,
    0x18F71FAA,0x4B8987AA,0x491B9EAA,0x829DC7AA,0xBCE635AA,0xCEA6DFAA,0x20D4ADAA,0x2D74FDAA,0x3C1C0DAA,0x12D6D4AA,0x48C000AA,
    0x2A51E2AA,0xE3AC12AA,0xFC42A8AA,0x2FC827AA,0x1A30BFAA,0xB740C2AA,0x42ACF5AA,0x2FD9DEAA,0xFAFB71AA,0x05D1CDAA,0xC471BDAA,
    0x94436EAA,0xC1F7ECAA,0xCE79EEAA,0xBD1EF2AA,0x93B7E4AA,0x3214AAAA,0x184D3BAA,0xAE4B99AA,0x7E49D7AA,0x4C436EAA,0xFA24CCAA,
    0xCE76BEAA,0xA04E0AAA,0x9F945CAA,0xDCDE3DAA,0x10C9C5AA,0x70524DAA,0x0BE472AA,0x8A2CD7AA,0x6152C2AA,0xCF72A9AA,0xE59338AA,
    0xEEDC2DAA,0xD8C762AA,0x3FE65CAA,0xFF8C13AA,0xC715FFAA,0x20B2AAAA,0xDC143CAA,0x6495EDAA,0xf0e68cAA,0x778899AA,0xFF1493AA,
    0xF4A460AA,0xEE82EEAA,0xFFD720AA,0x8b4513AA,0x4949A0AA,0x148b8bAA,0x14ff7fAA,0x556b2fAA,0x0FD9FAAA,0x10DC29AA,0x534081AA,
    0x0495CDAA,0xEF6CE8AA,0xBD34DAAA,0x247C1BAA,0x0C8E5DAA,0x635B03AA,0xCB7ED3AA,0x65ADEBAA,0x5C1ACCAA,0xF2F853AA,0x11F891AA,
    0x7B39AAAA,0x53EB10AA,0x54137DAA,0x275222AA,0xF09F5BAA,0x3D0A4FAA,0x22F767AA,0xD63034AA,0x9A6980AA,0xDFB935AA,0x3793FAAA,
    0x90239DAA,0xE9AB2FAA,0xAF2FF3AA,0x057F94AA,0xB98519AA,0x388EEAAA,0x028151AA,0xA55043AA,0x0DE018AA,0x93AB1CAA,0x95BAF0AA,
    0x369976AA,0x18F71FAA,0x4B8987AA,0x491B9EAA,0x829DC7AA,0xBCE635AA,0xCEA6DFAA,0x20D4ADAA,0x2D74FDAA,0x3C1C0DAA,0x12D6D4AA,
    0xFC42A8AA,0x2FC827AA,0x1A30BFAA,0xB740C2AA,0x42ACF5AA,0x2FD9DEAA,0xFAFB71AA,0x05D1CDAA,0xC471BDAA,0x94436EAA,0xC1F7ECAA,
    0xCE79EEAA,0xBD1EF2AA,0x93B7E4AA,0x3214AAAA,0x184D3BAA,0xAE4B99AA,0x7E49D7AA,0x4C436EAA,0xFA24CCAA,0xCE76BEAA,0xA04E0AAA,
    0x9F945CAA,0xDCDE3DAA,0x10C9C5AA,0x70524DAA,0x0BE472AA,0x8A2CD7AA,0x6152C2AA,0xCF72A9AA,0xE59338AA,0xEEDC2DAA,0xD8C762AA,
    0x3FE65CAA,0xFF8C13AA,0xC715FFAA,0x20B2AAAA,0xDC143CAA,0x6495EDAA,0xf0e68cAA,0x778899AA,0xFF1493AA,0xF4A460AA,0xEE82EEAA,
    0xFFD720AA,0x8b4513AA,0x4949A0AA,0x148b8bAA,0x14ff7fAA,0x556b2fAA,0x0FD9FAAA,0x10DC29AA,0x534081AA,0x0495CDAA,0xEF6CE8AA,
    0xBD34DAAA,0x247C1BAA,0x0C8E5DAA,0x635B03AA,0xCB7ED3AA,0x65ADEBAA,0x5C1ACCAA,0xF2F853AA,0x11F891AA,0x7B39AAAA,0x53EB10AA,
    0x54137DAA,0x275222AA,0xF09F5BAA,0x3D0A4FAA,0x22F767AA,0xD63034AA,0x9A6980AA,0xDFB935AA,0x3793FAAA,0x90239DAA,0xE9AB2FAA,
    0xAF2FF3AA,0x057F94AA,0xB98519AA,0x388EEAAA,0x028151AA,0xA55043AA,0x0DE018AA,0x93AB1CAA,0x95BAF0AA,0x369976AA,0x18F71FAA,
    0x4B8987AA,0x491B9EAA,0x829DC7AA,0xBCE635AA,0xCEA6DFAA,0x20D4ADAA,0x2D74FDAA,0x3C1C0DAA,0x12D6D4AA,0x48C000AA,0x2A51E2AA,
    0xE3AC12AA,0xFC42A8AA,0x2FC827AA,0x1A30BFAA,0xB740C2AA,0x42ACF5AA,0x2FD9DEAA,0xFAFB71AA,0x05D1CDAA,0xC471BDAA,0x94436EAA,
    0xC1F7ECAA,0xCE79EEAA,0xBD1EF2AA,0x93B7E4AA,0x3214AAAA,0x184D3BAA,0xAE4B99AA,0x7E49D7AA,0x4C436EAA,0xFA24CCAA,0xCE76BEAA,
    0xA04E0AAA,0x9F945CAA,0xDCDE3DAA,0x10C9C5AA,0x70524DAA,0x0BE472AA,0x8A2CD7AA,0x6152C2AA,0xCF72A9AA,0xE59338AA,0xEEDC2DAA,
    0xD8C762AA,0x3FE65CAA,0x48C000AA,0x2A51E2AA,0xE3AC12AA
};

enum DATE_TIME
{
    Day,
    Month,
    Year,
    Second,
    Minute,
    Hour,
}

new ClockTime[DATE_TIME];

//TOP SYSTEM
new Top5KillsActor[5];
new Text3D:Top5Kills[5];
new Text3D:Top5KillsNome[5];

new Float:TOP5Kills[5][4] = {
    { 1184.2400,-2032.3656,69.0078,123.0535 },
    { 1182.5326,-2029.8873,69.0078,123.0753 },
    { 1180.1112,-2027.8939,69.0078,148.8109 },
    { 1169.2753,-2028.4543,69.0006,230.1996 },
    { 1165.3832,-2032.8639,69.0078,235.5392 }
};

new ActorLounge[8];
new Top5DamageActor[5];
new Text3D:Top5Damage[5];
new Text3D:Top5DamageNome[5];

new Float:TOP5Damage[5][4] = {
    { 1184.4551,-2041.2717,69.0078,46.4216 },
    { 1182.9928,-2043.0415,69.0078,56.8577 },
    { 1180.6578,-2045.7148,69.0078,34.9736 },
    { 1169.7711,-2045.6019,69.0006,346.1958 },
    { 1165.5803,-2041.0651,69.0078,320.7744 }
};

new Float:ActorLoungeCoords[8][4] = {
    {1131.4117,-2044.2993,69.0078,273.5329},
    {1125.3280,-2037.1938,69.8815,269.0763},
    {1131.4326,-2029.6290,69.0078,269.8611},
    {1136.8027,-2007.7343,69.0006,157.7136},
    {1136.7711,-2066.1260,69.0078,36.9320},
    {1125.1427,-2029.8063,75.8093,270.0000},
    {1125.1427,-2044.1575,75.8093,270.0000},
    {1117.7090,-2036.9181,78.7500,268.6331}
};

new LoaderFreezer = 1;
new TeleLockTimer[MAX_PLAYERS];

new ZoneColour[MAX_ZONES] = {DEFAULT_ZONE_COLOUR, ...};
new ZoneOwnerClan[MAX_ZONES] = {-1, ...};

new RoundTime = ROUND_TIME_LOW_PLAYERS_COUNT * 60;

enum E_ROOM_NAME
{
    E_ROOM_NAME_SERVER[128],
    E_ROOM_NAME_DISCORD[16]
}

new const ROOM_NAME[MAX_LOCATIONS][E_ROOM_NAME] =
{
    {"{B4B5B7}DM Room",         "DM Room 1"},
    {"{B4B5B7}DM Room",         "DM Room 2"},
    {"{B4B5B7}Lounge Room",     "Lounge Room"},
    {"{B4B5B7}Custom Room",     "Custom Room"}
};

new const LOCATION_DISPLAY_ORDER[MAX_LOCATIONS] =
{
    ARENA_DM_1,
    ARENA_DM_2,
    ARENA_CUSTOM,
    ARENA_AFK
};

#define AC_REASON_LEN (64)
#define AC_WEAPON_LEN (32)

enum e_AC_SUSPECT
{
    ac_rejectedHits,
    ac_lastReason[AC_REASON_LEN],
    ac_weaponFlags,
    ac_lastWeapon[AC_WEAPON_LEN],
    ac_lastTime,

    ac_cheatFlags,
    ac_lastCheat[AC_REASON_LEN],
    ac_lastCheatTime
};

new ACSuspect[MAX_PLAYERS][e_AC_SUSPECT];

new bool:MsgCheat[MAX_PLAYERS];
new g_PlayerKillsAgainst[MAX_PLAYERS][MAX_PLAYERS];

enum E_SOUND_LIST
{
    soundID,
    soundName[32]
}

new const g_SoundList[][E_SOUND_LIST] = 
{
    {17802, "Classic Bell 1"},
    {17803, "High Pitch Bell 2"},
    {1056,  "Countdown Beep"},
    {1057,  "High Beep"},
    {1085,  "Menu Select Blip"},
    {1052,  "Mission Pass Chime"},
    {1149,  "Cash Register"},
    {1137,  "Weapon Click"},
    {1138,  "Weapon Purchase"},
    {1139,  "Ammo Reload"},
    {1190,  "Teleport Sound"},
    {1058,  "Garage Door Open"},
    {1083,  "Keypress Click"},
    {1084,  "Keypress Error"},
    {1145,  "Heavy Click"},
    {1150,  "Slot Machine Win"},
    {1053,  "Chime Alert"},
    {1054,  "Mission Failed"},
    {0,     "Disabled / Off"}
};

new PlayerText:FPSPingPacket[MAX_PLAYERS];
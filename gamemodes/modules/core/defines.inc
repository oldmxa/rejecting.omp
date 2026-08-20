#define NT_DISTANCE 40.0

#define SCOREBOARD_SCORE (1337)
#define SCOREBOARD_PING  (1337)

//Colors

#define COLOR_WHITE (0xFFFFFFFF)
#define COL_WHITE "{FFFFFF}"

#define COLOR_RED (0xF3230EFF)
#define COL_RED "{F3230E}"

#define COLOR_GRAY (0xBEBEBEFF)
#define COL_GRAY "{BEBEBE}"

#define COLOR_NGRAY (0Xa4aba8ff)
#define COL_NGRAY "{a4aba8}"

#define COLOR_DSAND (0xb09385FF)
#define COL_DSAND "{b09385}"

#define COLOR_BEIGE (0xa29582ff)
#define COL_BEIGE "{a29582}"

#define COLOR_GREEN (0x026440ff)
#define COL_GREEN "{026440}"

#define COLOR_KGREEN (0x32CD32ff)
#define COL_KGREEN  "{32CD32}"

#define COLOR_BLUE (0x2f4048ff)
#define COL_BLUE "{2f4048}"

#define COLOR_LIGHT_BLUE (0x418cb0ff)
#define COL_LIGHT_BLUE "{418cb0}"

#define COLOR_IRED (0x9b2f25ff)
#define COL_IRED "{9b2f25}"

#define COLOR_TOMATO (0xFF6347FF)

#define COLOR_PINK (0xf58169ff)
#define COL_PINK "{f58169}"

#define COLOR_GYELLOW (0xdead10ff)
#define COL_GYELLOW "{dead10}"

#define COLOR_DISCORD (0x7289daff)
#define COL_DISCORD "{7289da}"

#define COLOR_DEFAULT (0xa9c4e4ff)
#define COL_DEFAULT   "{a9c4e4}"

#define COLOR_ORANGE  (0xffa500ff)
#define COL_ORANGE 	  "{ffa500}"

#define COLOR_ABACATE (0x98A578ff)
#define COL_ABACATE   "{98A578}"

#define COLOR_CORANGE (0xf1802dff)
#define COL_CORANGE "{f1802d}"

#define COLOR_MONEY (0x23C552ff)
#define COL_MONEY "{23C552}"

// Admin Colors
#define COLOR_ADM_CMD     (0x4287f5FF)
#define COL_ADM_CMD       "{4287f5}"
#define COLOR_ADM_PUNISH  (0xe83333FF)
#define COL_ADM_PUNISH    "{e83333}"

// Discord Embed Colors
#define DISCORD_COLOR_INFO     0x3498DB
#define DISCORD_COLOR_SUCCESS  0x2ECC71
#define DISCORD_COLOR_WARNING  0xF1C40F
#define DISCORD_COLOR_ERROR    0xE74C3C
#define DISCORD_COLOR_ADMIN    0x9B59B6
#define DISCORD_COLOR_ARENA    0x1ABC9C
#define DISCORD_COLOR_DEFAULT  3037990 // 0x2E5B26

#define ADM_TAG_NORM      COL_ADM_CMD"[Admin] "
#define ADM_TAG_PUN       COL_ADM_PUNISH"[Admin] "

#define RDM_TAG           "{MAIN}[RDM] "

#define ban_message_color "{e83333}"

#define MAIN_BACKGROUND_COLOUR (0x0000004B)


#define LOUNGEVW             909
#define LOUNGEINT            1

#if !defined DUELS_STATE_NONE
	#define DUELS_STATE_NONE    998
#endif
#if !defined DUELS_STATE_WAITING
	#define DUELS_STATE_WAITING 999
#endif

#define MIN_PASSWORD_LENGTH 4
#define MAX_PASSWORD_LENGTH 45

#define APPEARANCE_SRC_MENU     (0)   // /menu
#define APPEARANCE_SRC_PERSONAL (1)   // /help > My Account > Personalization
#define APPEARANCE_SRC_SETTINGS (2)   // /help > My Account > Settings
#define APPEARANCE_SRC_NONE     (3)   // chat command: there is no menu to go back to

#define GIVE_MONEY_ONDEATH		300
#define VOTEKICK_MIN_VOTES		3

#define LOST_MONEY_ONDEATH		100
#define LOST_MONEY_ONSUICIDE	200

#define NULL 0

#define MAX_LOGIN_ATTEMPTS 3
#define MAX_ACCOUNT_LOCKTIME 2

#define MAX_ZONES 72

#define MAX_LINES 			6
#define DEATH_LINE_LEN		128
#define BACKGROUND_COLOR	"30"

#define	ROUND_TIME_LOW_PLAYERS_COUNT	5
#define	ROUND_TIME_HIGH_PLAYERS_COUNT	8
#define	HIGH_PLAYERS_COUNT				8

#define MEDIUM_ARENA_PLAYERS_COUNT 5
#define HIGH_ARENA_PLAYERS_COUNT 12

#define MAX_FPS 110
#define MIN_FPS 30
#define MAX_PING 500
#define MAX_PL (Float:4.0)

#define BLOCKED_SKIN_ID		0
#define MAX_STRING 255
#define MAX_PICKUPSS	30

#define randomEx(%0,%1) (random(%1 - %0 + 1) + %0)
#define floor(%0) floatround(%0, floatround_floor)

#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))


#define MAX_PLAYERS_LOCATION	25
#define MAX_PLAYERS_AFK	MAX_PLAYERS

#define MAX_LOCATIONS		7

#define ARENA_FPS_LOCK_VALUE	(102)


#define IsPlayerAndroid(%0)                 GetPVarInt(%0, "NotAndroid") == 0

#define IsPlayerFakingNick(%0)              (strcmp(getPlayerName(%0), User[%0][userName], false) != 0)

#define MOBILE		"MOBILE"

#define	DEFAULT_ZONE_COLOUR		0x7a747465

// CLAN: modules/clan/config.inc

// ACHIEVEMENTS

#define MAX_ACHIEVEMENTS (60)

// funcs
#define	getPlayerName	ReturnPlayerName


// configs

#define         SERVER_RCON			"rejectingflles"
#define         SERVER_MAP			"San Andreas"
#define         SERVER_WEBSITE		"rejectingflles.com"
#define         SERVER_MODE			"Build 2.0.0"
#define         SERVER_LANGUAGE     "ENG"
//access restricted to owner
#define         SERVER_OWNER        "oldmxa"

// Discord guild
#define         DISCORD_GUILD_ID    "1460103805290151946"

// discord Roles 

#define         DISCORD_ROLE_LINKED    "1467351645745840182"
#define         DISCORD_ROLE_VIP       "1531410032753770597"

#define         DISCORD_ROLE_OPERATOR  "1532823418213503167"   // adminLevel 1
#define         DISCORD_ROLE_MODERATOR "1532823261308518530"   // adminLevel 2
#define         DISCORD_ROLE_ADMIN     "1460104459811033361"   // adminLevel 3
#define         DISCORD_ROLE_COORD     "1532822549811957841"   // adminLevel 4
#define         DISCORD_ROLE_DEV       "1460119782627213353"   // adminLevel 5

#define         MAX_ADMIN_LEVEL        (5)

#define         GUNMENU_VW          (20000)

// gunmenu vw
#define         SPAWN_VW            (30000)

stock ShowPersonalizationMenu(playerid) return CallLocalFunction("Account_ShowPersonalizationMenu", "d", playerid);
stock ShowSettingsMenu(playerid) return CallLocalFunction("Account_ShowSettingsMenu", "d", playerid);
stock ChangeSkin(playerid, skinid) return CallLocalFunction("Stats_ChangeSkin", "dd", playerid, skinid);
stock CanSpawnLoungeVehicle(playerid) return CallLocalFunction("Lounge_CanSpawnLoungeVehicle", "d", playerid);
stock SpawnLoungeVehicle(playerid, vehicle, color1 = -1, color2 = -1) return CallLocalFunction("Lounge_SpawnLoungeVehicle", "dddd", playerid, vehicle, color1, color2);
stock ChangeTime(playerid, timeid) return CallLocalFunction("Stats_ChangeTime", "dd", playerid, timeid);
stock ChangeWeather(playerid, weatherid) return CallLocalFunction("Stats_ChangeWeather", "dd", playerid, weatherid);
stock showMapListDialog(playerid) return CallLocalFunction("Stats_showMapListDialog", "d", playerid);
stock ShowStats(playerid, targetid) return CallLocalFunction("Stats_ShowStats", "dd", playerid, targetid);
stock ShowPlayerClanInfo(playerid, targetid) return CallLocalFunction("Stats_ShowPlayerClanInfo", "dd", playerid, targetid);
stock ShowNameHistory(playerid, targetid, page = 0) return CallLocalFunction("Invest_ShowNameHistory", "ddd", playerid, targetid, page);
stock ShowPenaltyHistory(playerid, const targetname[], page = 0) return CallLocalFunction("Punish_ShowPenaltyHistory", "dsd", playerid, targetname, page);
stock ShowAdminConfig(playerid) return CallLocalFunction("Stats_ShowAdminConfig", "d", playerid);
stock SendDiscordResponse(DCC_Channel:channel, const message[]) return CallLocalFunction("Discord_SendDiscordResponse", "ds", _:channel, message);
stock SendDiscordEmbedReply(const title[], const description[], color = DISCORD_COLOR_DEFAULT) return CallLocalFunction("Discord_SendDiscordEmbedReply", "ssd", title, description, color);
stock ShowBanDialog(playerid, const adminname[], const reason[], const banDate[], const unbanDate[]) return CallLocalFunction("Stats_ShowBanDialog", "dssss", playerid, adminname, reason, banDate, unbanDate);
stock ShowSecurityMenu(playerid) return CallLocalFunction("Account_ShowSecurityMenu", "d", playerid);
stock ShowCoinsMenu(playerid) return CallLocalFunction("Coinshop_ShowCoinsMenu", "d", playerid);
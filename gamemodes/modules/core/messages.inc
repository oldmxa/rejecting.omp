
#if defined _MESSAGES_INC_
    #endinput
#endif
#define _MESSAGES_INC_

// GENERAL SERVER PREFIXES & SYSTEM NOTIFICATIONS

#define INFO                        ""COL_IRED"[INFO] "
#define HELP                        ""COL_GREEN"[HELP] "

#define WELCOME_MSG                 "{FFFFFF}Welcome to {MAIN}%s{FFFFFF} server"
#define REGISTER_WRITE_MSG          "{MAIN}[INFO] {FFFFFF}Your account has been created. {MAIN}Welcome to %s!"
#define AUTO_LOGIN_SUCCESS          "{MAIN}[SECURITY] {FFFFFF}Auto-logged in successfully (IP & Serial verified)."
#define ADMIN_LOGIN_WELCOME         "{MAIN}[ADMIN] {FFFFFF}Welcome %s, your administrator commands have been successfully loaded!"

#define INFO_PREFERENCES            "{MAIN}[INFO] {FFFFFF}Changes saved successfully!"
#define INFO_ADMINCONFIG            "{MAIN}[INFO] {FFFFFF}Your admin settings have been changed successfully!"
#define NEXT_LEVEL_MSG              "{MAIN}[INFO] {FFFFFF}Congratulations, you have moved to the next level!"

// CHAT FORMAT MODELS & SPY NOTIFICATIONS

#define GLOBAL_CHAT_MODEL           "%s{%06x}%s {%s}[%d]: {%s}%s"
#define GLOBAL_CHAT_MODEL_CLAN      "%s%s{%06x}[%s] {%06x}%s {%s}[%d]: {%s}%s"
#define LOCAL_CHAT_MODEL            ""COL_ORANGE"%s[%d]: %s"
#define ADMIN_CHAT_MODEL            ""COL_DSAND".: ADMIN | %s {%06x}%s [%d]"COL_WHITE": %s"

#define ADMIN_SEE_PMS               "{787878}PM SENT FROM %s[%d] TO %s[%d]: %s"
#define ADMIN_SEE_CLANPM            "{787878}CLAN %s[%d] * %s[%s]: %s"
#define ADMIN_SEE_CMDS              "{787878}CMD * %s[%d]: %s"
#define ADMIN_SEE_LOCAL             "{787878}LOCAL * %s[%d]: %s"

// COMMON VALIDATION & CHECKER ERRORS

#define UNKNOWN_COMMAND_MSG         "{MAIN} << {FFFFFF}Unknown command. Use /help /cmds {MAIN}>>"
#define IS_NOT_CONNECTED            "{MAIN}[ERROR] {FFFFFF} The user is not logged in."
#define IS_NOT_LOGGED               "{MAIN}[ERROR] {FFFFFF} The player is not logged yet."
#define NOT_LOGGED_ERROR            "{MAIN}[ERROR] {FFFFFF} You must be logged in to use commands!"
#define NOT_SPAWNED_MSG             "{MAIN}[ERROR] {FFFFFF} You are not spawned."
#define CANNOT_DO_THIS              "{MAIN}[ERROR] {FFFFFF} You cannot do this now."
#define CANNOT_RESPAWN              "{MAIN}**{FFFFFF} You cannot respawn."
#define IN_VEHICLE_ERROR            "{MAIN}**{FFFFFF}You cannot use this while inside a vehicle."
#define IS_NOT_NUMERIC              "{MAIN}**{FFFFFF}Enter a valid number."
#define KEEP_CALM_MSG               "{MAIN}[ERROR] {FFFFFF}Please wait %d seconds."
#define HEALTH_ERROR_MSG            "{MAIN}[ERROR] {FFFFFF} You need at least %d health"
#define LOW_HEALTH_ERROR            "{MAIN}[ERROR] {FFFFFF} You need to have full health to change mode."
#define RECENT_DAMAGE_ERROR         "{MAIN}[ERROR] {FFFFFF} You cannot change mode while in combat or having taken damage recently."
#define RETARD                      "{MAIN}**{FFFFFF} YOU ARE A FUCKING RETARD!!!"

// PLAYER COMMANDS & PREFERENCES

#define STATS_HELP                  "{MAIN}[ERROR] {FFFFFF}/stats [ID]"
#define AKA_HELP                    "{MAIN}[ERROR] {FFFFFF}/Aka [ID] - Check if player has other accounts."
#define NAMES_HELP                  "{MAIN}[ERROR] {FFFFFF}/names [ID] - Check the player's name history."
#define DANCE_HELP                  "{MAIN}[ERROR] {FFFFFF} /dance [1-4]"

#define TIME_HELP                   "{MAIN}[ERROR] {FFFFFF}/time [id time]"
#define TIME_INFO                   "{MAIN}[INFO] {FFFFFF}You have set the time for {2275FF}%d:00"

#define WEATHER_HELP                "{MAIN}[ERROR] {FFFFFF}/weather [id weather]"
#define WEATHER_INFO                "{MAIN}[INFO] {FFFFFF}Weather changed to {2275FF}%d"

#define SKIN_HELP2                  "{MAIN}**{FFFFFF} Select a number between 2 and 311."
#define SKIN_INFO                   "{MAIN}[INFO] {FFFFFF}You changed your skin to {2275FF}%d"

#define NICK_CHANGE_WHILE_FAKE      "{MAIN}[ERROR] {FFFFFF}You cannot change your nickname with a fake nick active. Use {MAIN}/fake {FFFFFF}with no arguments to reset it first."

#define PM_HELP                     "{MAIN}[ERROR] {FFFFFF}/pm [name/id] [Message]"
#define PM_SEND                     "[PM] Message sent to %s (%d): %s"
#define PM_RECIVED                  "[PM] Message received from %s (%d): %s"

#define REPORT_HELP                 "{MAIN}[ERROR] {FFFFFF} /report [id] [reason]"
#define CANNOT_REPORT_SELF          "{MAIN}[ERROR] {FFFFFF} You cannot report yourself."
#define CANNOT_REPORT_ADMIN         "{MAIN}[ERROR] {FFFFFF} You cannot report an administrator."

// ECONOMY & MONEY TRANSFERS

#define ERROR_TRANSFERMONEY         "{MAIN}[ERROR] {FFFFFF}/transfer [ID/name] [money]"
#define TRANSFERMONEY_SELF          "{MAIN}[ERROR] {FFFFFF}You cannot transfer money to yourself."
#define TRANSFERMONEY_AMOUNT        "{MAIN}[ERROR] {FFFFFF}The amount must be greater than zero."
#define TRANSFERMONEY_NOMONEY       "{MAIN}[ERROR] {FFFFFF}You don't have enough money."
#define TRANSFERMONEY_SENT          "{MAIN}**{FFFFFF} You sent "COL_GREEN"%d {FFFFFF}to "COL_GREEN"%s."
#define TRANSFERMONEY_GIVEN         "{MAIN}**{FFFFFF} You received "COL_GREEN"%d {FFFFFF}from "COL_GREEN"%s."

// LOUNGE

#define AFK_MSG_FIRST               ""COL_GRAY"[LNG] %s went to rest their fingers (/lounge)"
#define LEFT_LOUNGE_MSG             ""COL_GRAY"[LNG] You left the lounge."
#define AFK_MSG_2                   ""COL_GRAY"[LNG] You can teleport. Use {b4b3b3}ESC > maps"
#define AFK_MSG_3                   ""COL_GRAY"[LNG] Lounge commands (get gun, car etc): {b4b3b3}/lcmds"
#define AFK_MSG_4                   ""COL_GRAY"[LNG] Server commands /cmds"
#define AFK_ERROR_MSG               "{MAIN}**{FFFFFF} To leave the lounge type {MAIN}/lounge"
#define NEED_BE_AFK                 "{MAIN}**{FFFFFF} You need to be in the lounge to use this command"
#define TARGET_AFK                  "{MAIN}**{FFFFFF} You can't spectate players in lounge."

// 7. WEAPONS, RESPAWN & COMBAT

#define GETGUN_HELP                 "{MAIN}[ERROR] {FFFFFF}/getgun [Weapon Name [Max 18 characters]] [Ammo]"
#define INVALID_AMMO_MSG            "{MAIN}**{FFFFFF} Invalid Ammo."
#define INVALID_ID_MSG              "{MAIN}**{FFFFFF} Invalid Weapon ID."
#define CHOOSE_WEAPON_MSG           "{MAIN}**{FFFFFF} You didn't take a weapon. Please choose something."

#define ERROR_RESPAWN               "{MAIN}[ERROR] {FFFFFF}After doing /respawn you need to wait 2 seconds to respawn again."
#define ERROR_RESPAWN_DUEL          "{MAIN}[ERROR] {FFFFFF}You cannot respawn while in a duel."

#define HITMARK_HELP                "{MAIN}[ERROR] {FFFFFF}8 Characters are needed to proceed!"
#define HITMARK_HELP_2              "{MAIN}[ERROR] {FFFFFF}The value needs to be hexadecimal!"
#define UPDATE_DAMAGE               "[DAMAGE] Your damage record has been updated, from "COL_GYELLOW"%d {FFFFFF}to now {MAIN}%d{FFFFFF}!"

// ARENAS & ROUND MANAGEMENT

#define CHANGED_ARENA_MSG           ADM_TAG_NORM"%s changed to arena %s (ID: "COL_BEIGE"%d"COL_ADM_CMD")"
#define RANDOM_CHANGED_ARENA_MSG    ADM_TAG_NORM"%s randomly changed arena to %s (ID: "COL_BEIGE"%d"COL_ADM_CMD")"
#define CHANGEARENA_DELAY           "{MAIN}**{FFFFFF} Change arena command is delayed, please wait %s or use "COL_GYELLOW"/queuearena."
#define QUEUED_ARENA                "{MAIN}**{FFFFFF}Arena {MAIN}%s (ID: %d) {FFFFFF}was queued successfully, please wait for the current arena to end."
#define NOT_VALID_ARENA             "{MAIN}[ERROR] {FFFFFF}The specified value exceeds the maximum value %d"
#define ARENA_ENDED_MSG             "{MAIN}[LOCATION %d] {FFFFFF}Current arena {MAIN}%s (ID: %d) {FFFFFF}has ended, the next one is {MAIN}%s (ID: %d)."
#define ARENA_START_MSG             "{MAIN}[INFO] {FFFFFF}Arena {MAIN}%s #%d {FFFFFF}has started!"
#define ARENA_CLAN_DOMINATED        "{MAIN}[INFO] {FFFFFF}The arena {MAIN}%s #%d {FFFFFF}is dominated by the clan %s."
#define ARENA_JUST_PLAYED           "{MAIN}[ERROR] {FFFFFF}%s {FFFFFF}was the last arena played. Wait for the current one to finish."
#define ARENA_R2_REQUIRED_ERROR     "{MAIN}[ERROR] {FFFFFF}This arena requires an R2+ client (SA-MP R2/R3/R4/R5 or open.mp). Update your client to enter."
#define ARENA_FPS_LOCK_ERROR        "{MAIN}[ERROR] {FFFFFF}This arena requires your FPS locked at exactly %d. Your current FPS is %d."
#define NOT_EMPTY_SPACES            "{MAIN}**{FFFFFF} There are no empty spaces in this location"
#define JOINED_ROOM_MSG             "{MAIN}**{FFFFFF} You have chosen room #%d"

// SPREES & COMBAT BROADCASTS

#define FIRST_BLOOD                 RDM_TAG"{FFFFFF}First blood! {%06x}%s {FFFFFF}opened the round."
#define END_KILLSPREE               RDM_TAG"{FFFFFF}Your spree is over. You killed {MAIN}%d {FFFFFF}players in {MAIN}%s {FFFFFF}seconds."
#define KILL_MACHINE                RDM_TAG"{%06x}%s {FFFFFF}is on a {MAIN}killing machine {FFFFFF}run!"
#define REACHED_RAMPAGE             RDM_TAG"{%06x}%s {FFFFFF}reached a {MAIN}rampage{FFFFFF}!"
#define DOWN_SPREE_MSG              RDM_TAG"{%06x}%s {FFFFFF}has ended the kill-streak of {%06x}%s {MAIN}(%d kills)"

// WEEKLY RANKINGS

#define WEEKLY_KILLS                ""COL_PINK"[Kills Rank] {MAIN}%s {FFFFFF}placed {MAIN}1st {FFFFFF}in the weekly rank with {MAIN}%d kills {FFFFFF}and was rewarded."
#define WEEKLY_DAMAGE               ""COL_PINK"[Damage Rank] {MAIN}%s {FFFFFF}placed {MAIN}1st {FFFFFF}in the weekly rank with {MAIN}%d damage {FFFFFF}and was rewarded."
#define WEEKLY_RESET                ""COL_PINK"[Weekly Ranks] {FFFFFF}Weekly ranks have been reset!"

// ACHIEVEMENTS & LEVEL PROGRESSION

#define ACHIEVEMENT_UNLOCKED        "{MAIN}[ACH] {FFFFFF}You unlocked {MAIN}%s{FFFFFF}! Type {MAIN}/ach {FFFFFF}to check your reward."
#define ACHIEVEMENT_RETRO           "{MAIN}[ACH] {FFFFFF}You unlocked {MAIN}%d achievements {FFFFFF}from your history! Type {MAIN}/ach {FFFFFF}to check your rewards."
#define ACHIEVEMENT_NOT_LOGGED      "{MAIN}[ERROR] {FFFFFF}You must be logged in to view achievements."
#define ACHIEVEMENT_GIVE_HELP       "{MAIN}[USAGE] {FFFFFF}/giveach [id/name] [achievement id]"
#define ACHIEVEMENT_GIVE_OWNER      "{MAIN}[ERROR] {FFFFFF}Only {MAIN}"SERVER_OWNER"{FFFFFF} can grant achievements."
#define ACHIEVEMENT_GIVE_INVALID    "{MAIN}[ERROR] {FFFFFF}Invalid achievement ID. Use {MAIN}/giveach {FFFFFF}to see the list."
#define ACHIEVEMENT_GIVE_HAS        "{MAIN}[ERROR] {FFFFFF}%s {FFFFFF}already has {MAIN}%s{FFFFFF}."
#define ACHIEVEMENT_GIVE_DONE       "{MAIN}[ACH] {FFFFFF}You granted {MAIN}%s {FFFFFF}to {MAIN}%s{FFFFFF}."
#define ACHIEVEMENT_GIVE_GOT        "{MAIN}[ACH] {FFFFFF}%s {FFFFFF}granted you {MAIN}%s{FFFFFF}! Type {MAIN}/ach {FFFFFF}to check your reward."
#define LEVEL_UP_BONUS              "{MAIN}[LEVEL UP] {FFFFFF}Congratulations on reaching {MAIN}Level %d{FFFFFF}! Level-Up Bonus: {23C552}+%s{FFFFFF} and {FFFF00}+%s{FFFFFF}!"

// COINS & VIP PACKAGES

#define VIP_NO_MONEY                "{MAIN}[ERROR] {FFFFFF}You need {00FF00}%s {FFFFFF}more to activate this VIP package."
#define VIP_NO_COINS                "{MAIN}[ERROR] {FFFFFF}You need {FFFF00}%s {FFFFFF}more to activate this VIP package. Earn Coins by unlocking achievements ({MAIN}/ach{FFFFFF})."
#define COINS_BALANCE               "{MAIN}[COINS] {FFFFFF}You have {FFFF00}%s{FFFFFF}. Earn more by unlocking achievements ({MAIN}/ach{FFFFFF})."
#define COINS_SET_HELP              "{MAIN}[USAGE] {FFFFFF}/setcoins [id/name] [amount] {BEBEBE}- works on offline accounts too."
#define COINS_SET_NEGATIVE          "{MAIN}[ERROR] {FFFFFF}The Coins amount cannot be negative."
#define COINS_SET_NOACC             "{MAIN}[ERROR] {FFFFFF}No account found with the name {MAIN}%s{FFFFFF}."
#define COINS_SET_DONE              "{MAIN}[COINS] {FFFFFF}You set the Coins of {MAIN}%s {FFFFFF}from {FFFF00}%s {FFFFFF}to {FFFF00}%s{FFFFFF}."
#define COINS_SET_TARGET            "{MAIN}[COINS] {FFFFFF}Admin {MAIN}%s {FFFFFF}set your Coins to {FFFF00}%s{FFFFFF}."
#define COINS_SET_OFFLINE           "{MAIN}[COINS] {FFFFFF}Offline account {MAIN}%s {FFFFFF}now has {FFFF00}%s{FFFFFF}."

// DISCORD

#define DC_COOLDOWN                 "{MAIN}[ERROR] {FFFFFF}You are blocked from using this command. Please wait a few minutes."
#define DC_LINKSUCCESS              "{MAIN}[DISCORD] {FFFFFF}Your account is now linked to Discord!"
#define DC_LINKED                   "{MAIN}[ERROR] {FFFFFF}Your account is already linked to a Discord user (%s#%s)."
#define DC_NOTLINKED                "{MAIN}[ERROR] {FFFFFF}Your account is not linked to a Discord user."
#define DC_UNLINKSUCESS             "{MAIN}[DISCORD] {FFFFFF}You unlinked your Discord account successfully."
#define DC_INUSE                    "{MAIN}[ERROR] {FFFFFF}That Discord user is already linked to another account."
#define DC_LEFT                     "{MAIN}[DISCORD] {FFFFFF}You left our Discord server and your account was unlinked."

// ACCOUNT SECURITY

#define ERROR_CHANGEPASS            "{MAIN}[ERROR] {FFFFFF}/changepass [pass]"
#define ERROR_INVALIDPASS           "{MAIN}[ERROR] {FFFFFF}Invalid password. Use only letters and numbers, between %d and %d characters."

// ADMINISTRATION

#define NOT_ADMIN_MSG               "{MAIN}[ERROR] {FFFFFF} You need to be admin to use this command!"
#define NEED_ADMIN_LEVEL            "{MAIN}[ERROR] {FFFFFF} You need to be a higher admin level."
#define SUPERIOR_RIGHTS             "{MAIN}**{FFFFFF} You can't use admin commands against an administrator with a level equal to or higher than yours."
#define SUPERIOR_RIGHTS_DISCORD     "** You can't use admin commands against an administrator with a level equal to or higher than yours."

// --- Staff Promotion / Demotion ---
#define CREATE_ADMIN_HELP           "{MAIN}[ERROR] {FFFFFF}/setadmin [name/id] [level]"
#define CREATE_ADMIN_HELP2          "{MAIN}[ERROR] {FFFFFF}The admin level needs to be between 0 and 5."
#define CREATE_ADMIN_HELP3          "{MAIN}[ERROR] {FFFFFF}The level cannot be equal to or higher than yours."
#define REMOVED_ADMIN_MSG           ADM_TAG_NORM"You removed admin privileges from %s!"
#define REMOVED_ADMIN_MSG_PLAYER    ADM_TAG_NORM"%s removed your admin privileges!"
#define CREATED_ADMIN_MSG           ADM_TAG_NORM"You promoted %s to admin level %d."
#define CREATED_ADMIN_MSG_PLAYER    ADM_TAG_NORM"%s promoted you to admin level %d."

// --- Staff Teleports & Actions ---
#define SPAWN_HELP                  "{MAIN}[ERROR] {FFFFFF}/spawn(ar) [ID]"
#define SPAWN_MSG                   ADM_TAG_NORM"Admin %s spawned you."
#define SLAP_HELP                   "{MAIN}[ERROR] {FFFFFF} /slap [name/id]"
#define SLAP_MSG                    ADM_TAG_PUN"Admin %s slapped you."
#define FREEZE_HELP                 "{MAIN}[ERROR] {FFFFFF} /freeze [name/id]"
#define FREEZE_MSG                  ADM_TAG_PUN"Admin %s froze you."
#define UNFREEZE_HELP               "{MAIN}[ERROR] {FFFFFF}  /unfreeze [name/id]"
#define UNFREEZE_MSG_ADM            ADM_TAG_NORM"Admin %s unfroze you."
#define GOON_HELP                   "{MAIN}[ERROR] {FFFFFF} /goon [name/id] Go to player position"
#define GET_HELP                    "{MAIN}[ERROR] {FFFFFF} /get [name/id] Bring the player to your position"

// --- Spectate ---
#define SPEC_HELP                   "{MAIN}[ERROR] {FFFFFF}/spec [name/id]"
#define SPEC_HELP2                  "{MAIN}**{FFFFFF} You can't watch yourself"
#define NO_MORE_PLAYERS             "{MAIN}**{FFFFFF}No more players to spectate in this direction."
#define NO_SPEC_MODE                "{MAIN}**{FFFFFF}You're not spectating anyone."
#define IN_SPEC_MODE                "{MAIN}**{FFFFFF}You're in spectate mode. Type {MAIN}/specoff {FFFFFF}to leave."

// --- Mute ---
#define MUTE_HELP                   "{MAIN}[ERROR] {FFFFFF}/mute [name/id] [minutes] [reason]"
#define MUTE_HELP2                  "{MAIN}[ERROR] {FFFFFF}Enter a number between 1 and 500."
#define MUTE_REASON                 ADM_TAG_PUN"%s (%d) muted %s (%d) for %d minutes. Reason: %s"
#define MUTED_PLAYER                ADM_TAG_PUN"%s (%d) muted you for %d minutes"
#define UNMUTED_MSG                 ADM_TAG_NORM"Admin %s (%d) has unmuted you. Be more careful when using the chat!"
#define UNMUTED_AUTO                ADM_TAG_NORM"** Your mute time is up. Be more careful when using the chat!"
#define UNMUTED_PLAYER              ADM_TAG_NORM"Admin %s (%d) unmuted player %s (%d)."
#define IS_MUTED                    "{MAIN}**{FFFFFF} You are blocked from using chat."
#define UNMUTE_HELP                 "{MAIN}[ERROR] {FFFFFF}/unmute [name/id]"

// --- Kick ---
#define KICK_HELP                   "{MAIN}[ERROR] {FFFFFF}/kick [ID] [reason] - Kick player from the server."
#define KICK_HELP2                  "{MAIN}[ERROR] {FFFFFF} You can't kick an administrator with a level equal to or higher than yours."
#define KICK_NO_REASON              ADM_TAG_PUN"Admin %s kicked %s (%d)."
#define KICK_REASON                 ADM_TAG_PUN"Admin %s kicked %s (%d). Reason: %s"

// --- Ban ---
#define BAN_BANNED                  ""COL_ADM_PUNISH"[ACCOUNT] You are banned. Open an appeal on our Discord."
#define BAN_WHOBANNED               ""COL_ADM_PUNISH"The account %s was banned by admin %s, reason: %s, on %s, your unban was scheduled for %s. (GMT-03:00)"
#define BAN_MISTAKE                 ""COL_ADM_PUNISH"If you think this is a mistake, take a screenshot and open an unban appeal on our Discord."
#define BAN_DCLINK                  ""COL_DISCORD"rejectingflles.com"
#define BAN_HELP                    "{MAIN}[ERROR] {FFFFFF}/ban [id] [hours] [reason] - Ban player."
#define BAN_HELP2                   "{MAIN}**{FFFFFF} Enter a number between 1 and 9999."
#define BAN_HELP3                   "{MAIN}**{FFFFFF} You can't ban an administrator with a level equal to or higher than yours."
#define BAN_NO_REASON               ADM_TAG_PUN"Admin %s banned %s (%d)."
#define BAN_REASON                  ADM_TAG_PUN"Admin %s (%d) banned %s for %d hours. Reason: %s"
#define BANOFF_HELP                 "{MAIN}[ERROR] {FFFFFF}/offban [name] [hours] [reason] - Ban player."
#define UNBAN_HELP                  "{MAIN}[ERROR] {FFFFFF}/unban [player/ip]"
#define UNBAN_MESSAGE               ADM_TAG_NORM"Admin "COL_WHITE"%s "COL_ADM_CMD"unbanned player "COL_WHITE"%s"COL_ADM_CMD"."
#define BAN_TEMPREC_REASON          ADM_TAG_PUN"%s banned %s for %d hours. Reason: %s"
#define BAN_TEMPREC_MINUTES         ADM_TAG_PUN"5 minutes have passed. Post your recording on our Discord."
#define BAN_TEMPREC_WHOBANNED       ADM_TAG_PUN"You were banned by %s, reason: %s. Your IP: %s."

// --- open.mp launcher force ---
#define FORCE_OMP_HELP              "{MAIN}[ERROR] {FFFFFF}/banomp [id] - Force player to open.mp launcher."
#define FORCED_OMP_BAN              ADM_TAG_PUN"Admin %s forced open.mp launcher on %s"
#define OMP_UNBAN                   ADM_TAG_NORM"The player "COL_WHITE"%s "COL_ADM_CMD"is no longer locked to the open.mp launcher."
#define UNBAN_OMP_HELP              "{MAIN}[ERROR] {FFFFFF}/unbanomp [nick] - Unban a player that has been forced to play with open.mp launcher."

// --- Network & Client Limits ---
#define TOO_HIGH_FPS                "FPS TOO HIGH (%d/%d)"
#define TOO_LOW_FPS                 "FPS TOO LOW (%d/%d)"
#define TOO_HIGH_PING               "PING TOO HIGH (%d/%d)"

// --- Vehicle Admin Spawn ---
#define VEHICLE_SPAWN_HELP          "{MAIN}[ERROR] {FFFFFF}/car [veh name] [color 1; -1 = random] [color 2; -1 = random]"
#define INVALID_VEHICLE_MSG         "{MAIN}[ERROR] {FFFFFF}Invalid car name!"

// PLAYER WELCOME 

#define PLAYER_WELCOME_MSG1         "{MAIN}**{FFFFFF} Welcome again, %s{FFFFFF}!"
#define PLAYER_WELCOME_MSG2         "{MAIN}**{FFFFFF} Your account is that of a %s."
#define PLAYER_WELCOME_MSG3         "{MAIN}**{FFFFFF} You are a member of the %s{FFFFFF} clan."
#define PLAYER_WELCOME_MSG4         "{MAIN}**{FFFFFF} Your last login was at: {MAIN}%s."
#define PLAYER_WELCOME_MSG5         "{MAIN}**{FFFFFF} VIP Status: {MAIN}VIP {FFFFFF}%dd %dh left."
#define PLAYER_WELCOME_MSG_VIP_PERM "{MAIN}**{FFFFFF} VIP Status: {MAIN}VIP Permanent."

// SERVER MESSAGES FUNCTIONS

stock SendServerInfoMessage(playerid, const fmat[], va_args<>)
{
    if(!IsPlayerConnected(playerid)) return 0;
    if(User[playerid][hideGlobalMsg]) return 0;

    new buffer[256];
    va_format(buffer, sizeof(buffer), fmat, va_start<2>);

    return SendWrappedClientMessage(playerid, -1, buffer);
}

forward SendMSG(playerid);
public SendMSG(playerid)
{
    if(User[playerid][hideGlobalMsg]) return 1;

    new stringe[24][160];
    format(stringe[0], 160, "{MAIN}[INFO] {FFFFFF}Have a question for staff? Use {MAIN}'/ask [question]'{FFFFFF} to get help from admins.");
    format(stringe[1], 160, "{MAIN}[INFO] {FFFFFF}Suspect someone of cheating or breaking rules? Report them: {MAIN}'/report [id] [reason]'{FFFFFF}.");
    format(stringe[2], 160, "{MAIN}[INFO] {FFFFFF}Press {MAIN}'N'{FFFFFF} anytime to open your quick menu (Settings, My Account, Security & Help).");
    format(stringe[3], 160, "{MAIN}[INFO] {FFFFFF}Explore all our arenas: RP Walk, FPS Lock, FPS Unlock & Custom using {MAIN}'/changemode'{FFFFFF}.");
    format(stringe[4], 160, "{MAIN}[INFO] {FFFFFF}Want to train your aim and break server records? Use {MAIN}'/training'{FFFFFF} for timed challenges!");
    format(stringe[5], 160, "{MAIN}[INFO] {FFFFFF}Practice against configurable moving NPC bots anytime using {MAIN}'/prac'{FFFFFF} (or {MAIN}'/practice'{FFFFFF})!");
    format(stringe[6], 160, "{MAIN}[INFO] {FFFFFF}Challenge players to 1v1/2v2 duels: {MAIN}'/duel'{FFFFFF} & {MAIN}'/duel2v2'{FFFFFF}.");
    format(stringe[7], 160, "{MAIN}[INFO] {FFFFFF}Gather your clan, unlock extra member slots and spawn at your Clan Safehouse with {MAIN}'/safemenu'{FFFFFF}!");
    format(stringe[8], 160, "{MAIN}[INFO] {FFFFFF}Capture floating Outpost torches with your Clan to receive {MAIN}$5,000 & 50 points{FFFFFF} every 5 minutes!");
    format(stringe[9], 160, "{MAIN}[INFO] {FFFFFF}Manage your clan, invite members and customize colors using {MAIN}'/clan'{FFFFFF}.");
    format(stringe[10], 160, "{MAIN}[INFO] {FFFFFF}Need to take a break or chat in peace? Go to the Lounge using {MAIN}'/lounge'{FFFFFF} or {MAIN}'/afk'{FFFFFF}.");
    format(stringe[11], 160, "{MAIN}[INFO] {FFFFFF}Select and customize your favorite weapon loadout in {MAIN}'/gunmenu'{FFFFFF}.");
    format(stringe[12], 160, "{MAIN}[INFO] {FFFFFF}Customize your change weather ({MAIN}'/weather [id]'{FFFFFF}) and world time ({MAIN}'/t [0-23]'{FFFFFF}).");
    format(stringe[13], 160, "{MAIN}[INFO] {FFFFFF}Change your skin anytime using {MAIN}'/setskin (/ss) [skin id]'{FFFFFF}.");
    format(stringe[14], 160, "{MAIN}[INFO] {FFFFFF}Change your melee fighting style using {MAIN}'/fightstyle'{FFFFFF}.");
    format(stringe[15], 160, "{MAIN}[INFO] {FFFFFF}Customize your hit sound (Bell), Headshot text, and HUD in Settings ({MAIN}'N'{FFFFFF} key).");
    format(stringe[16], 160, "{MAIN}[INFO] {FFFFFF}Check your K/D ratio, achievements, clan rank and registration date with {MAIN}'/stats [id]'{FFFFFF}.");
    format(stringe[17], 160, "{MAIN}[INFO] {FFFFFF}Configure your security, Auto-Login & Admin Auth in {MAIN}'/help'{FFFFFF} -> Security.");
    format(stringe[18], 160, "{MAIN}[INFO] {FFFFFF}Join our Discord community to participate in tournaments, claim roles, and submit feedback!");
    format(stringe[19], 160, "{MAIN}[INFO] {FFFFFF}To change your nickname, password, or gameplay toggles, press {MAIN}'N'{FFFFFF}.");
    format(stringe[20], 160, "{MAIN}[INFO] {FFFFFF}Try our {MAIN}RP Walk Arenas{FFFFFF} for authentic, tactical foot combat movement!");
    format(stringe[21], 160, "{MAIN}[INFO] {FFFFFF}Looking for max smoothness? Play in our {MAIN}FPS Unlock Arenas{FFFFFF} via {MAIN}'/changemode'{FFFFFF}.");
    format(stringe[22], 160, "{MAIN}[INFO] {FFFFFF}Toggle PMs, hit sound, HUD and connect messages in Settings ({MAIN}'N'{FFFFFF} key).");
    format(stringe[23], 160, "{MAIN}[INFO] {FFFFFF}Need a complete list of commands? Check {MAIN}'/help'{FFFFFF} and {MAIN}'/cmds'{FFFFFF}.");

    SendClientMessage(playerid, -1, "%s", stringe[randomEx(0, sizeof(stringe) - 1)]);
    return 1;
}

// SERVER RULES ARRAYS

new const g_ServerRules[][] = {
    "The use of cheat programs that give any form of advantage over other players (any type of software) is prohibited.",
    "The use of malicious software that may negatively affect the server's performance is prohibited.",
    "The use of VPNs and Proxies is prohibited (exceptions with admin approval only).",
    "Texture modifications to gain advantage (removing poles, trees, etc.) are prohibited.",
    "Reduced or sped-up animations via third-party programs are prohibited.",
    "Replacing stun animations via third-party programs is prohibited.",
    "Rude or offensive messages (insulting family, ethnic hatred) are prohibited.",
    "Provoking players negatively (calling noob, low skill, or 0) is prohibited.",
    "Neutral swearing is allowed within reason.",
    "Off-topic reports are prohibited.",
    "Advertising third-party resources or servers is prohibited.",
    "Flooding or spamming chat is prohibited.",
    "Maximum of 1 account per player.",
    "Maximum allowed frame rate (FPS) is 102."
};

new const g_ServerRulesNote[] =
    "We strongly recommend resolving any issues with your gameplay recording software in advance.\n" \
    "At any time, the administration may request a recording of your gameplay session. Recording the entire \n" \
    "session is not required; a few clips of 4-5 minutes per session will suffice.";

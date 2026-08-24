// Constants, globals and hooks. Everything depends on this block, nothing enters before it.
#include "modules/core/defines.inc"
#include "modules/core/globals.inc"
#include "modules/core/hooks.inc"

// anti cheat: hooks ALS
#tryinclude "modules/AC/early.inc"
#tryinclude "modules/AC/detect/nops.inc"

// logs
#include "modules/logs/logs_entry.inc"

// Interface: HUD, textdraws and the login screen.
#include "modules/ui/style_picker.inc"
#include "modules/ui/hud_styles.inc"
#include "modules/ui/life_styles.inc"
#include "modules/ui/skin_picker.inc"
#include "modules/ui/vehicle_picker.inc"
#include "modules/ui/preview_dialog.inc"
#include "modules/ui/textdraws.inc"
#include "modules/ui/scoreboard.inc"
#include "modules/core/initgame.inc"
#include "modules/ui/login.inc"

// texts and set design
#include "modules/core/messages.inc"
#include "modules/util/buildings.inc"

// Arenas: the arenaData table is used by the dialogs.
#include "modules/arena/data.inc"

// object-built arenas: each file self-registers through hook OnGameModeInit()
#include "modules/arena/arena_object.inc"
#include "modules/arena/maps/chinese-garden.inc"
#include "modules/arena/maps/desert.inc"
#include "modules/arena/maps/egypt.inc"
#include "modules/arena/maps/urbex.inc"
#include "modules/arena/maps/zoo.inc"

// Clan: its enums and globals are used by almost every module below.
#include "modules/clan/clan.inc"

// dialogs. they all go here, in the position the single file previously occupied,
// so as not to alter the resolution order of the `using public` statements further down.
#include "modules/players/dialogs.inc"
#include "modules/arena/dialogs.inc"
#include "modules/admins/dialogs.inc"
#include "modules/discord/dialogs.inc"

// database and anti-vpn
#include "modules/database/entry.inc"
#include "modules/database/match_history.inc"
#include "modules/misc/antivpn.inc"

// utilities and functions
#include "modules/util/utils.inc"
#include "modules/util/server.inc"
#include "modules/util/web_console.inc"
#include "modules/players/session.inc"
#include "modules/players/spectate.inc"
#include "modules/players/combat.inc"
#include "modules/ui/hud.inc"
#include "modules/arena/rounds.inc"
#include "modules/misc/rank.inc"
#include "modules/discord/core.inc"
#include "modules/admins/lookup.inc"

#include "modules/admins/auth.inc"
#include "modules/misc/top5.inc"
#include "modules/misc/tickets.inc"
#include "modules/vip/core.inc"
#include "modules/outposts/core.inc"
#include "modules/interiors/core.inc"

// anti cheat
#tryinclude "modules/AC/entry.inc"
#include "modules/training/training.pwn"
#include "modules/practice/practice.inc"

// duel, safehouse and random_death_tag hook OnPlayerDeath via ALS:
// they must come before the base.pwn callbacks.
#include "modules/safehouse/safehouse.inc"
#include "modules/duel/core.inc"
#include "modules/duel2v2/entry.inc"
#include "modules/misc/random_death_tag.inc"
#include "modules/achievements/core.inc"
#include "modules/tournament/entry.inc"

// commands
#include "modules/players/commands.inc"
#include "modules/players/social.inc"
#include "modules/players/appearance.inc"
#include "modules/players/lounge.inc"
#include "modules/players/account.inc"
#include "modules/players/stats.inc"
#include "modules/players/coinslog.inc"
#include "modules/players/coinshop.inc"

// admin
#include "modules/admins/commands.inc"
#include "modules/admins/punish.inc"
#include "modules/admins/players.inc"
#include "modules/admins/system.inc"
#include "modules/admins/investigate.inc"
#include "modules/admins/eaccount.inc"

#include "modules/discord/commands.inc"

// maps
#include "modules/util/lounge_map.inc"
#include "modules/util/custom_arenas.inc"

// discord
#include "modules/discord/callbacks.inc"
#include "modules/core/callbacks.inc"

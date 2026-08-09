// constantes, variaveis globais e hooks everything depends on this block. Nothing enters before it.
#include "modules/core/defines.inc"
#include "modules/core/globals.inc"
#include "modules/core/hooks.inc"

// logs
#include "modules/logs/logs_entry.inc"

// interface: HUD, textdraws e tela de login ---
#include "modules/ui/style_picker.inc"
#include "modules/ui/hud_styles.inc"
#include "modules/ui/life_styles.inc"
#include "modules/ui/skin_picker.inc"
#include "modules/ui/vehicle_picker.inc"
#include "modules/ui/preview_dialog.inc"
#include "modules/ui/textdraws.inc"
#include "modules/ui/login.inc"

// texts and set design
#include "modules/core/messages.inc"
#include "modules/util/buildings.inc"

// arenas the arenaData table is used by the dialogs.
#include "modules/arena/data.inc"

// clan: enums e globais sao usados por quase todos os modulos abaixo.
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
#include "modules/players/session.inc"
#include "modules/players/spectate.inc"
#include "modules/players/combat.inc"
#include "modules/ui/hud.inc"
#include "modules/arena/rounds.inc"
#include "modules/misc/rank.inc"
#include "modules/discord/core.inc"
#include "modules/admins/lookup.inc"

// game systems
#include "modules/misc/top5.inc"
#include "modules/misc/tickets.inc"
#include "modules/vip/core.inc"
#include "modules/outposts/core.inc"
#include "modules/interiors/core.inc"
#include "modules/admins/auth.inc"
#include "modules/training/training.pwn"

// duel and random_death_tag hook OnPlayerDeath via ALS:
// they must come before the base.pwn callbacks.
#include "modules/duel/core.inc"
#include "modules/misc/random_death_tag.inc"
#include "modules/achievements/core.inc"

// commands
#include "modules/players/commands.inc"
#include "modules/players/social.inc"
#include "modules/players/appearance.inc"
#include "modules/players/lounge.inc"
#include "modules/players/account.inc"
#include "modules/players/stats.inc"

// admin
#include "modules/admins/commands.inc"
#include "modules/admins/punish.inc"
#include "modules/admins/players.inc"
#include "modules/admins/system.inc"
#include "modules/admins/investigate.inc"

#include "modules/discord/commands.inc"

#include "modules/duel2v2/entry.inc"
#include "modules/safehouse/safehouse.inc"

// maps
#include "modules/util/lounge_map.inc"
#include "modules/util/custom_arenas.inc"

// discord
#include "modules/discord/callbacks.inc"
#include "modules/core/callbacks.inc"

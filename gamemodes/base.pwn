#pragma dynamic 16384

#include <a_samp>

#define YSI_NO_MODE_CACHE
#define YSI_NO_CACHE_MESSAGE
#define YSI_NO_OPTIMISATION_MESSAGE
#define YSI_NO_HEAP_MALLOC

#undef MAX_PLAYERS
#define MAX_PLAYERS 88

#include <sscanf2>
#include <samp_bcrypt>
#include <a_mysql>
#include <streamer>
#include <crashdetect>
#include <colandreas>
#include <requests>
#include <discord-connector>

#include <Pawn.RakNet>

#include <YSI_Coding/y_va>
#include <YSI_Coding/y_inline>
#include <YSI_Extra/y_inline_timers>
#include <YSI_Extra/y_inline_mysql>
#include <YSI_Visual/y_dialog>
#include <YSI_Visual/y_commands>
#include <YSI_Data/y_iterate>

#include <weapon-config>
#include <zmessage>
#include <sort-inline>
#include <md-sort/md-sort>
#include <strlib>
#include <a_zone>
#include <alpha-fix>
// #include <lagcomp>
#include <visual-areas>

new RequestsClient:client;

#include "header.inc"

main()
{
}

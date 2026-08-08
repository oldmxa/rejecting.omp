#if defined _clan_module_included
    #endinput
#endif
#define _clan_module_included

#include <YSI_Coding/y_hooks>

#include "modules/clan/config.inc"
#include "modules/clan/messages.inc"
#include "modules/clan/enums.inc"
#include "modules/clan/functions.inc"
#include "modules/clan/database.inc"
#include "modules/clan/dialogs.inc"
#include "modules/clan/commands.inc"

hook OnPlayerConnect(playerid)
{
    g_PlayerSelectedClan[playerid] = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    g_PlayerSelectedClan[playerid] = -1;
    return 1;
}

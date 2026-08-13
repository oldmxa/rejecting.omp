#if defined _practice_module_incl
    #endinput
#endif
#define _practice_module_incl

#include <YSI_Coding/y_hooks>

#include "modules/practice/definitions.inc"
#include "modules/practice/core.inc"
#include "modules/practice/npc.inc"
#include "modules/practice/dialogs.inc"
#include "modules/practice/commands.inc"

hook OnPlayerConnect(playerid)
{
    ResetPracticeState(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    DestroyPracticeTarget(playerid);
    ResetPracticeState(playerid);
    return 1;
}

hook OnGameModeExit()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        DestroyPracticeTarget(i);

    return 1;
}

hook OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (g_Practice[playerid][pracActive])
        DestroyPracticeTarget(playerid);

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    if (!g_Practice[playerid][pracActive]) return 1;

    if (!IsPlayerInLounge(playerid))
    {
        EndPracticeSession(playerid);
        return 1;
    }

    SetPlayerVirtualWorld(playerid, PRACTICE_VW_BASE + playerid);
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);

    CreatePracticeTarget(playerid);
    return 1;
}

hook OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (!PracticeValidateWorld(playerid)) return 1;

    // conta o disparo para os dois tipos de alvo; o acerto no NPC vem por callback
    g_Practice[playerid][pracShots]++;

    HandlePracticeShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);

    return 1;
}

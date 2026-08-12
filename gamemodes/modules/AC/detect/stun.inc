#if defined _ac_stun_included
    #endinput
#endif
#define _ac_stun_included

#include <YSI_Coding\y_hooks>

#define AC_STUN_WINDOW   (400)
#define AC_STUN_STRIKES  (6)
#define AC_ANIM_SAMPLE   (500)

static
    ACStunTick[MAX_PLAYERS],
    ACStunAnim[MAX_PLAYERS],
    bool:ACStunSeen[MAX_PLAYERS],
    ACStunStrike[MAX_PLAYERS],
    ACLastAnim[MAX_PLAYERS],
    ACAnimSample[MAX_PLAYERS];

static AC_ClearStun(playerid)
{
    ACStunTick[playerid] = ACStunAnim[playerid] = ACStunStrike[playerid] = 0;
    ACLastAnim[playerid] = 0;
    ACStunSeen[playerid] = false;
    return 1;
}

stock AC_NoteStun(playerid)
{
    if (ACFastStuns || !User[playerid][isLogged]) return 0;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;

    ACStunTick[playerid] = GetTickCount();
    ACStunAnim[playerid] = ACLastAnim[playerid];
    ACStunSeen[playerid] = false;
    return 1;
}

stock AC_SampleAnim(playerid, animationId, Float:vx, Float:vy)
{
    if (!AC_DebugActive()) return 0;
    if (GetTickCount() - ACAnimSample[playerid] < AC_ANIM_SAMPLE) return 0;

    ACAnimSample[playerid] = GetTickCount();

    AC_Debug(playerid, AC_PIZDA_WALK, "anim %d at speed %.1f u/s",
        animationId, floatsqroot(vx * vx + vy * vy) * 179.28);

    return 1;
}

stock AC_CheckStun(playerid, animationId)
{
    ACLastAnim[playerid] = animationId;

    if (!ACStunTick[playerid]) return 0;

    if (animationId != ACStunAnim[playerid]) ACStunSeen[playerid] = true;

    if (GetTickCount() - ACStunTick[playerid] < AC_STUN_WINDOW) return 0;

    ACStunTick[playerid] = 0;

    AC_Debug(playerid, AC_ANTI_STUN, "hit reaction %s, anim %d to %d (strike %d/%d)",
        ACStunSeen[playerid] ? ("shown") : ("missing"),
        ACStunAnim[playerid], animationId, ACStunStrike[playerid], AC_STUN_STRIKES);

    if (ACStunSeen[playerid])
    {
        ACStunStrike[playerid] = 0;
        return 0;
    }

    if (++ACStunStrike[playerid] < AC_STUN_STRIKES) return 0;

    ACStunStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_ANTI_STUN);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    AC_ClearStun(playerid);
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    AC_ClearStun(playerid);
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    AC_ClearStun(playerid);
    return 1;
}

#if defined _ac_upside_included
    #endinput
#endif
#define _ac_upside_included

#define AC_UPSIDE_LIMIT (-0.5)
#define AC_UPSIDE_HITS  (5)

static ACUpsideStrike[MAX_PLAYERS];

stock AC_CheckUpsideDown(playerid, Float:qx, Float:qy)
{
    new Float:up = 1.0 - 2.0 * ((qx * qx) + (qy * qy));

    if (up < 0.5)
        AC_Debug(playerid, AC_UPSIDE_DOWN, "vertical axis %.2f (limit %.2f)", up, AC_UPSIDE_LIMIT);

    if (up != up || up >= AC_UPSIDE_LIMIT)
    {
        ACUpsideStrike[playerid] = 0;
        return 1;
    }

    if (++ACUpsideStrike[playerid] < AC_UPSIDE_HITS) return 1;

    ACUpsideStrike[playerid] = 0;
    CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_UPSIDE_DOWN);
    return 1;
}

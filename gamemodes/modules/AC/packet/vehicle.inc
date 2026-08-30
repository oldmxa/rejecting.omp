#if defined _ac_vehicle_included
    #endinput
#endif
#define _ac_vehicle_included

const AC_PM_VEHICLE_SYNC = 200;

#define AC_CAR_FLOAT_MAX (100000.0)
#define AC_CAR_STRIKES   (2)

static ACCarStrike[MAX_PLAYERS];

static bool:AC_CarFloatIsSane(Float:value)
{
    if (IsNaN(value)) return false;
    if (value < -AC_CAR_FLOAT_MAX || value > AC_CAR_FLOAT_MAX) return false;

    return true;
}

static bool:AC_CarSyncIsSane(const data[PR_InCarSync])
{
    if (!AC_CarFloatIsSane(data[PR_trainSpeed])) return false;
    if (!AC_CarFloatIsSane(data[PR_vehicleHealth])) return false;

    for (new i; i < 4; i++)
    {
        if (!AC_CarFloatIsSane(data[PR_quaternion][i])) return false;
    }

    for (new i; i < 3; i++)
    {
        if (!AC_CarFloatIsSane(data[PR_position][i])) return false;
        if (!AC_CarFloatIsSane(data[PR_velocity][i])) return false;
    }
    return true;
}

IPacket:AC_PM_VEHICLE_SYNC(playerid, BitStream:bs)
{
    if (IsPlayerNPC(playerid)) return 1;

    new inCarData[PR_InCarSync];

    BS_IgnoreBits(bs, 8);
    BS_ReadInCarSync(bs, inCarData);

    if (AC_CarSyncIsSane(inCarData))
    {
        ACCarStrike[playerid] = 0;
        return 1;
    }

    AC_Debug(playerid, AC_FAKE_CAR_SYNC, "vehicle %d sync carries an impossible float (strike %d/%d)",
        inCarData[PR_vehicleId], ACCarStrike[playerid] + 1, AC_CAR_STRIKES);

    if (++ACCarStrike[playerid] >= AC_CAR_STRIKES)
    {
        ACCarStrike[playerid] = 0;
        CallRemoteFunction("OnRejectingACDetection", "ii", playerid, AC_FAKE_CAR_SYNC);
    }
    return 0;
}

#if defined _ac_parkour_included
    #endinput
#endif
#define _ac_parkour_included

const AC_PM_PLAYER_SYNC = 207;

IPacket:AC_PM_PLAYER_SYNC(playerid, BitStream:bs)
{
    if (IsPlayerNPC(playerid))
    {
        return 1;
    }

    new onFootData[PR_OnFootSync];

    BS_IgnoreBits(bs, 8);
    BS_ReadOnFootSync(bs, onFootData);

    if (onFootData[PR_specialAction] == _:SPECIAL_ACTION_NONE)
    {
        new const Float:angle = atan2(2.0 * ((onFootData[PR_quaternion][2] * onFootData[PR_quaternion][2]) + (onFootData[PR_quaternion][0] * onFootData[PR_quaternion][1])), (onFootData[PR_quaternion][0] * onFootData[PR_quaternion][0]) - (onFootData[PR_quaternion][1] * onFootData[PR_quaternion][1]) - (onFootData[PR_quaternion][2] * onFootData[PR_quaternion][2]) + (onFootData[PR_quaternion][2] * onFootData[PR_quaternion][2]));

        if ((angle > 85.0 || angle < -85.0) && !IsNaN(angle))
        {
            CallRemoteFunction("OnRejectingACDetection", "ii", playerid, E_REJECTING_AC_PARKOUR_MOD);
            return 0;
        }
    }

    AC_CheckUpsideDown(playerid, onFootData[PR_quaternion][1], onFootData[PR_quaternion][2]);
    AC_CheckStun(playerid, onFootData[PR_animationId]);
    AC_CheckHealthSync(playerid, onFootData[PR_health]);

    AC_CheckNullVelocity(playerid,
        onFootData[PR_position][0], onFootData[PR_position][1], onFootData[PR_position][2],
        onFootData[PR_velocity][0], onFootData[PR_velocity][1], onFootData[PR_velocity][2]);

    AC_SampleAnim(playerid, onFootData[PR_animationId],
        onFootData[PR_velocity][0], onFootData[PR_velocity][1], onFootData[PR_keys]);

    return 1;
}
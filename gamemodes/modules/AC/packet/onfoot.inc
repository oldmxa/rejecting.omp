#if defined _ac_parkour_included
    #endinput
#endif
#define _ac_parkour_included

stock AC_CheckNoFallAnim(playerid, animationId)
{
    if (!ACNoFallAnimation || !IsPlayerLogged(playerid) || IsPlayerAFK(playerid)) return 0;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;

    switch (animationId)
    {
        // Ground landing and impact animations after a fall (FALL_land, FALL_collapse, FALL_tumble)
        case 1128, 1131, 1137, 1138, 1139, 1140:
        {
            ClearAnimations(playerid, SYNC_ALL);
            return 1;
        }
    }
    return 0;
}

#define AC_SURF_OFFSET_MAX   (50.0)
#define AC_SURF_OFFSET_MIN_Z (-35.0)

static bool:AC_SurfFloatIsSane(Float:value)
{
    if (IsNaN(value)) return false;
    if (value < -AC_SURF_OFFSET_MAX || value > AC_SURF_OFFSET_MAX) return false;

    return true;
}

static bool:AC_SanitizeSurfOffsets(playerid, data[PR_OnFootSync])
{
    if (AC_SurfFloatIsSane(data[PR_surfingOffsets][0])
     && AC_SurfFloatIsSane(data[PR_surfingOffsets][1])
     && AC_SurfFloatIsSane(data[PR_surfingOffsets][2])
     && data[PR_surfingOffsets][2] > AC_SURF_OFFSET_MIN_Z) return false;

    AC_Debug(playerid, AC_INVISIBLE_SURF, "surf offsets %.1f %.1f %.1f (vehicle %d) zeroed",
        data[PR_surfingOffsets][0], data[PR_surfingOffsets][1], data[PR_surfingOffsets][2],
        data[PR_surfingVehicleId]);

    data[PR_surfingOffsets][0] = 0.0;
    data[PR_surfingOffsets][1] = 0.0;
    data[PR_surfingOffsets][2] = 0.0;
    return true;
}

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

    if (AC_SanitizeSurfOffsets(playerid, onFootData))
    {
        BS_SetWriteOffset(bs, 8);
        BS_WriteOnFootSync(bs, onFootData);
    }

    if (onFootData[PR_specialAction] == _:SPECIAL_ACTION_NONE)
    {
        new const Float:angle = atan2(2.0 * ((onFootData[PR_quaternion][2] * onFootData[PR_quaternion][2]) + (onFootData[PR_quaternion][0] * onFootData[PR_quaternion][1])), (onFootData[PR_quaternion][0] * onFootData[PR_quaternion][0]) - (onFootData[PR_quaternion][1] * onFootData[PR_quaternion][1]) - (onFootData[PR_quaternion][2] * onFootData[PR_quaternion][2]) + (onFootData[PR_quaternion][2] * onFootData[PR_quaternion][2]));

        if ((angle > 85.0 || angle < -85.0) && !IsNaN(angle))
        {
            CallRemoteFunction("OnRejectingACDetection", "ii", playerid, E_REJECTING_AC_PARKOUR_MOD);
            return 0;
        }
    }

    AC_CheckHologram(playerid, onFootData[PR_specialAction]);
    AC_CheckUpsideDown(playerid, onFootData[PR_quaternion][1], onFootData[PR_quaternion][2]);
    AC_CheckStun(playerid, onFootData[PR_animationId]);
    AC_CheckNoFallAnim(playerid, onFootData[PR_animationId]);
    AC_CheckHealthSync(playerid, onFootData[PR_health]);
    AC_CheckArmourSync(playerid, onFootData[PR_armour]);
    AC_CheckMegaJump(playerid, onFootData[PR_velocity][2]);

    AC_CheckNullVelocity(playerid,
        onFootData[PR_position][0], onFootData[PR_position][1], onFootData[PR_position][2],
        onFootData[PR_velocity][0], onFootData[PR_velocity][1], onFootData[PR_velocity][2]);

    AC_CheckPizdaWalk(playerid, onFootData[PR_animationId], onFootData[PR_keys],
        onFootData[PR_velocity][0], onFootData[PR_velocity][1]);

    AC_SampleAnim(playerid, onFootData[PR_animationId],
        onFootData[PR_velocity][0], onFootData[PR_velocity][1], onFootData[PR_keys]);

    return 1;
}
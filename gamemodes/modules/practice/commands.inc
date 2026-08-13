#if defined _practice_commands_incl
    #endinput
#endif
#define _practice_commands_incl

static SetPracticePoint(playerid, slot)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (slot == 1)
    {
        g_Practice[playerid][pracPos1][0] = x;
        g_Practice[playerid][pracPos1][1] = y;
        g_Practice[playerid][pracPos1][2] = z;
        g_Practice[playerid][pracHasPos1] = true;
    }
    else
    {
        g_Practice[playerid][pracPos2][0] = x;
        g_Practice[playerid][pracPos2][1] = y;
        g_Practice[playerid][pracPos2][2] = z;
        g_Practice[playerid][pracHasPos2] = true;
    }

    va_SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Point {MAIN}%d {FFFFFF}set at your position.", slot);

    if (g_Practice[playerid][pracMoving] && !PracticeCanMove(playerid))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Now set point {MAIN}%d {FFFFFF}to start the movement.", (slot == 1) ? 2 : 1);
    }

    CreatePracticeTarget(playerid);
    return 1;
}

@cmd(.alts = {"practice"}) prac(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (User[playerid][playerLevel] < PRACTICE_MIN_LEVEL)
    {
        return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need level {MAIN}%d {FFFFFF}to use the practice mode.", PRACTICE_MIN_LEVEL);
    }

    PracticeValidateWorld(playerid);

    new sub[12];

    if (sscanf(params, "s[12]", sub))
    {
        if (!IsPracticeActive(playerid))
        {
            if (!IsPlayerInLounge(playerid))
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The practice mode can only be started from the lounge.");

            if (g_PlayerTraining[playerid][pTrain_Active])
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Leave the training arena first.");

            if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use the practice mode during a duel.");

            if (playerSpecID[playerid] != INVALID_PLAYER_ID)
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Stop spectating first.");

            if (User[playerid][isCreatingArena])
                return SendClientMessage(playerid, -1, "%s", IN_ARENA_EDITOR);

            if (User[playerid][isFrozen])
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot do this while frozen.");

            EnterPractice(playerid);
        }
        return ShowPracticeDialog(playerid);
    }

    if (!strcmp(sub, "exit", true))
    {
        if (!IsPracticeActive(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}You are not in the practice world.");

        ExitPractice(playerid);
        return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}You left the practice world.");
    }

    if (!strcmp(sub, "rec", true))
    {
        if (!IsPracticeActive(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Use {MAIN}/prac {FFFFFF}first to enter the practice world.");

        if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Recording only works with the NPC target.");

        if (g_Practice[playerid][pracRecPending])
            return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Wait, the last recording is still being processed.");

        if (g_Practice[playerid][pracRecording]) return PracticeStopRecording(playerid);
        return PracticeStartRecording(playerid);
    }

    if (!strcmp(sub, "pos1", true) || !strcmp(sub, "pos2", true))
    {
        if (!IsPracticeActive(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Use {MAIN}/prac {FFFFFF}first to enter the practice world.");

        return SetPracticePoint(playerid, (sub[3] == '1') ? 1 : 2);
    }

    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac {B4B5B7}- open the settings dialog");
    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac pos1 {B4B5B7}/ {FFFFFF}/prac pos2 {B4B5B7}- set the two movement points");
    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac rec {B4B5B7}- record / stop recording the NPC movement");
    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac exit {B4B5B7}- leave the practice world");
    return 1;
}

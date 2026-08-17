#if defined _practice_commands_incl
    #endinput
#endif
#define _practice_commands_incl

@cmd(.alts = {"practice"}) prac(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot use practice mode while in a 2v2 duel.");

    if (User[playerid][playerLevel] < PRACTICE_MIN_LEVEL)
    {
        return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need level {MAIN}%d {FFFFFF}to use the practice mode.", PRACTICE_MIN_LEVEL);
    }

    if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use practice mode while inside a vehicle.");

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_SPECTATING)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use practice mode right now.");

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
        if (PracticeHasRecord(playerid)) return PracticeClearRecord(playerid);
        return PracticeStartRecording(playerid);
    }

    if (!strcmp(sub, "pos1", true) || !strcmp(sub, "pos2", true))
    {
        if (!IsPracticeActive(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Use {MAIN}/prac {FFFFFF}first to enter the practice world.");

        if (PracticeRecordLocks(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The recording drives the NPC. Clear it first to use the movement points.");

        return SetPracticePoint(playerid, (sub[3] == '1') ? 1 : 2);
    }

    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac {B4B5B7}- open the settings dialog");
    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac pos1 {B4B5B7}/ {FFFFFF}/prac pos2 {B4B5B7}- set the two movement points");
    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac rec {B4B5B7}- record, stop, then delete the NPC movement");
    SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/prac exit {B4B5B7}- leave the practice world");
    return 1;
}

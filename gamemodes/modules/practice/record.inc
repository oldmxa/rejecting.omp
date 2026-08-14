#if defined _practice_record_incl
    #endinput
#endif
#define _practice_record_incl

forward OnPracticeRecordReady(playerid);
forward OnPracticeRecordLoop(playerid);

stock PracticeRecordName(playerid, dest[], len = sizeof(dest))
{
    format(dest, len, "prac_rec_%d", playerid);
    return 1;
}

stock PracticeDropRecordFile(playerid)
{
    new name[24], file[32];
    PracticeRecordName(playerid, name);
    format(file, sizeof(file), "%s.rec", name);

    if (fexist(file)) fremove(file);
    return 1;
}

stock PracticeDropRecord(playerid)
{
    new npcid = g_Practice[playerid][pracNpcId];

    if (npcid != INVALID_NPC_ID && NPC_IsValid(npcid) && NPC_IsPlayingPlayback(npcid))
        NPC_StopPlayback(npcid);

    if (g_Practice[playerid][pracRecordId] != -1)
    {
        if (NPC_IsValidRecord(g_Practice[playerid][pracRecordId]))
            NPC_UnloadRecord(g_Practice[playerid][pracRecordId]);

        g_Practice[playerid][pracRecordId] = -1;
    }

    g_Practice[playerid][pracRecPending] = false;

    PracticeDropRecordFile(playerid);
    return 1;
}

stock bool:PracticeHasRecord(playerid)
{
    return (g_Practice[playerid][pracRecordId] != -1);
}

stock PracticeClearRecord(playerid)
{
    if (!PracticeHasRecord(playerid)) return 0;

    PracticeDropRecord(playerid);

    SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Recording deleted. The NPC is back to the movement points.");

    CreatePracticeTarget(playerid);
    return 1;
}

stock bool:PracticeRecordLocks(playerid)
{
    if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC) return false;

    return (g_Practice[playerid][pracRecording] ||
            g_Practice[playerid][pracRecPending] ||
            PracticeHasRecord(playerid));
}

stock PracticeStartRecording(playerid)
{
    if (g_Practice[playerid][pracRecording]) return 0;

    PracticeDropRecord(playerid);

    new npcid = g_Practice[playerid][pracNpcId];
    if (npcid != INVALID_NPC_ID && NPC_IsValid(npcid)) NPC_StopMove(npcid);

    new name[24];
    PracticeRecordName(playerid, name);

    StartRecordingPlayerData(playerid, PLAYER_RECORDING_TYPE_ONFOOT, name);
    g_Practice[playerid][pracRecording] = true;

    SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Recording your movement. Walk the path you want the NPC to repeat.");
    SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Stop it in the dialog or with {MAIN}/prac rec{FFFFFF}.");
    return 1;
}

stock PracticeStopRecording(playerid)
{
    if (!g_Practice[playerid][pracRecording]) return 0;

    StopRecordingPlayerData(playerid);

    g_Practice[playerid][pracRecording] = false;
    g_Practice[playerid][pracRecPending] = true;

    SetTimerEx("OnPracticeRecordReady", 400, false, "i", playerid);

    SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Recording stopped, preparing the playback...");
    return 1;
}

public OnPracticeRecordReady(playerid)
{
    if (!IsPlayerConnected(playerid) || !g_Practice[playerid][pracActive]) return 1;
    if (!g_Practice[playerid][pracRecPending]) return 1;

    g_Practice[playerid][pracRecPending] = false;

    new name[24], file[32];
    PracticeRecordName(playerid, name);
    format(file, sizeof(file), "%s.rec", name);

    new path[48], recordid = -1;

    for (new i; i < 3 && recordid < 0; i++)
    {
        switch (i)
        {
            case 0: format(path, sizeof(path), "scriptfiles/%s", file);
            case 1: format(path, sizeof(path), "%s", file);
            case 2: format(path, sizeof(path), "npcmodes/recordings/%s", file);
        }

        recordid = NPC_LoadRecord(path);
        if (!NPC_IsValidRecord(recordid)) recordid = -1;
    }

    if (recordid < 0)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Could not load the recording. Nothing to play back.");

        new log[96];
        format(log, sizeof(log), "[PRACTICE] NPC_LoadRecord failed for %s (player %d)", file, playerid);
        ConsoleLog(log);
        return 1;
    }

    g_Practice[playerid][pracRecordId] = recordid;

    SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Recording ready. The NPC repeats it in a loop until you leave the practice.");

    PracticePlayRecord(playerid);
    return 1;
}

stock PracticePlayRecord(playerid)
{
    if (g_Practice[playerid][pracRecordId] == -1) return 0;
    if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC) return 0;

    new npcid = g_Practice[playerid][pracNpcId];
    if (npcid == INVALID_NPC_ID || !NPC_IsValid(npcid)) return 0;
    if (NPC_IsDead(npcid)) return 0;

    NPC_StopMove(npcid);
    NPC_StartPlaybackEx(npcid, g_Practice[playerid][pracRecordId]);
    return 1;
}

public OnPracticeRecordLoop(playerid)
{
    if (!IsPlayerConnected(playerid) || !g_Practice[playerid][pracActive]) return 1;

    return PracticePlayRecord(playerid);
}

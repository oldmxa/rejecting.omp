#if defined _training_functions_included
    #endinput
#endif
#define _training_functions_included

forward bool:IsValidTrainingArena(arenaid);
forward OnTrainingCountdownTimer(playerid, count);
forward DestroyTrainingCountdownTimer(playerid);
forward RestoreTrainingSpawn(playerid);
forward TrainingOnSetAfk(playerid);
forward OnTrainingChallengeTick(playerid);
forward OnTrainingWaypointTimer(playerid);

stock bool:IsPlayerInTraining(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return false;
    return g_PlayerTraining[playerid][pTrain_Active];
}

stock RegisterTrainingArena(
    const name[],
    const type[],
    Float:spawnX, Float:spawnY, Float:spawnZ, Float:spawnA,
    Float:startX, Float:startY, Float:startZ, Float:startA,
    Float:endX, Float:endY, Float:endZ, Float:endA,
    Float:speed,
    targetModel,
    const weapons[] = {24, 25},
    weaponCount = 2,
    Float:targetHP = TRAINING_TARGET_HP,
    weaponsSize = sizeof(weapons)
)
{
    if (g_TotalTrainingArenas >= TRAINING_MAX_ARENAS)
    {
        printf("[TRAINING ERROR] Max training arenas reached (%d).", TRAINING_MAX_ARENAS);
        return -1;
    }

    new id = g_TotalTrainingArenas;
    g_TrainingArenas[id][tArena_ID] = id;
    format(g_TrainingArenas[id][tArena_Name], TRAINING_NAME_LEN, "%s", name);
    format(g_TrainingArenas[id][tArena_Type], TRAINING_NAME_LEN, "%s", type);

    g_TrainingArenas[id][tArena_SpawnX] = spawnX;
    g_TrainingArenas[id][tArena_SpawnY] = spawnY;
    g_TrainingArenas[id][tArena_SpawnZ] = spawnZ;
    g_TrainingArenas[id][tArena_SpawnA] = spawnA;

    g_TrainingArenas[id][tArena_TargetStartX] = startX;
    g_TrainingArenas[id][tArena_TargetStartY] = startY;
    g_TrainingArenas[id][tArena_TargetStartZ] = startZ;
    g_TrainingArenas[id][tArena_TargetStartA] = startA;

    g_TrainingArenas[id][tArena_TargetEndX] = endX;
    g_TrainingArenas[id][tArena_TargetEndY] = endY;
    g_TrainingArenas[id][tArena_TargetEndZ] = endZ;
    g_TrainingArenas[id][tArena_TargetEndA] = endA;

    g_TrainingArenas[id][tArena_TargetSpeed] = (speed > 0.0) ? speed : 1.0;
    g_TrainingArenas[id][tArena_TargetModel] = (targetModel > 0) ? targetModel : TRAINING_TARGET_MODEL;
    g_TrainingArenas[id][tArena_TargetHP] = (targetHP > 0.0) ? targetHP : TRAINING_TARGET_HP;

    if (weaponCount > weaponsSize) weaponCount = weaponsSize;
    if (weaponCount > TRAINING_MAX_WEAPONS) weaponCount = TRAINING_MAX_WEAPONS;
    if (weaponCount < 0) weaponCount = 0;

    g_TrainingArenas[id][tArena_WeaponCount] = weaponCount;
    for (new w = 0; w < weaponCount; w++)
    {
        g_TrainingArenas[id][tArena_Weapons][w] = weapons[w];
    }

    g_TrainingArenas[id][tArena_WaypointCount] = 2;
    g_TrainingArenas[id][tArena_WaypointX][0] = startX;
    g_TrainingArenas[id][tArena_WaypointY][0] = startY;
    g_TrainingArenas[id][tArena_WaypointZ][0] = startZ;
    g_TrainingArenas[id][tArena_WaypointX][1] = endX;
    g_TrainingArenas[id][tArena_WaypointY][1] = endY;
    g_TrainingArenas[id][tArena_WaypointZ][1] = endZ;
    g_TrainingArenas[id][tArena_IsRandomSpawning] = false;

    g_TotalTrainingArenas++;
    //printf("[TRN-DEBUG] registrada id=%d nome=%s total=%d", id, g_TrainingArenas[id][tArena_Name], g_TotalTrainingArenas);
    return id;
}

stock SetTrainingArenaRandomSpawning(arenaid, bool:randomSpawn = true)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    g_TrainingArenas[arenaid][tArena_IsRandomSpawning] = randomSpawn;
    return 1;
}

stock SetTrainingArenaWaypoints(arenaid, const Float:waypoints[][3], count)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    if (count > TRAINING_MAX_WAYPOINTS)
        count = TRAINING_MAX_WAYPOINTS;

    if (count < 2)
        return 0;

    g_TrainingArenas[arenaid][tArena_WaypointCount] = count;
    for (new i = 0; i < count; i++)
    {
        g_TrainingArenas[arenaid][tArena_WaypointX][i] = waypoints[i][0];
        g_TrainingArenas[arenaid][tArena_WaypointY][i] = waypoints[i][1];
        g_TrainingArenas[arenaid][tArena_WaypointZ][i] = waypoints[i][2];
    }

    g_TrainingArenas[arenaid][tArena_TargetStartX] = waypoints[0][0];
    g_TrainingArenas[arenaid][tArena_TargetStartY] = waypoints[0][1];
    g_TrainingArenas[arenaid][tArena_TargetStartZ] = waypoints[0][2];

    g_TrainingArenas[arenaid][tArena_TargetEndX] = waypoints[count - 1][0];
    g_TrainingArenas[arenaid][tArena_TargetEndY] = waypoints[count - 1][1];
    g_TrainingArenas[arenaid][tArena_TargetEndZ] = waypoints[count - 1][2];

    return 1;
}

stock bool:IsValidTrainingArena(arenaid)
{
    return (0 <= arenaid < g_TotalTrainingArenas);
}

stock Float:GetTrainingTargetRadius(arenaid)
{
    if (!IsValidTrainingArena(arenaid))
        return TRAINING_TARGET_RADIUS_FALL;

    if (g_TrainingArenas[arenaid][tArena_TargetRadius] > 0.0)
        return g_TrainingArenas[arenaid][tArena_TargetRadius];

    new Float:offX, Float:offY, Float:offZ, Float:radius;
    new Float:result = TRAINING_TARGET_RADIUS_FALL;

    if (CA_GetModelBoundingSphere(g_TrainingArenas[arenaid][tArena_TargetModel], offX, offY, offZ, radius) && radius > 0.0)
    {
        result = radius * TRAINING_TARGET_RADIUS_SCALE;
    }

    if (result < TRAINING_TARGET_RADIUS_MIN)
        result = TRAINING_TARGET_RADIUS_MIN;

    g_TrainingArenas[arenaid][tArena_TargetRadius] = result;

    printf("[TRAINING] Arena %d (%s): target model %d hit radius %.2f.",
        arenaid, g_TrainingArenas[arenaid][tArena_Name], g_TrainingArenas[arenaid][tArena_TargetModel], result);

    return result;
}

stock KillTrainingTimer(&timerid)
{
    if (timerid != 0)
    {
        KillTimer(timerid);
        timerid = 0;
    }
    return 1;
}

stock DestroyTrainingTargetObject(playerid)
{
    if (g_PlayerTraining[playerid][pTrain_TargetObject] != INVALID_STREAMER_ID)
    {
        if (IsValidDynamicObject(g_PlayerTraining[playerid][pTrain_TargetObject]))
        {
            DestroyDynamicObject(g_PlayerTraining[playerid][pTrain_TargetObject]);
        }
        g_PlayerTraining[playerid][pTrain_TargetObject] = INVALID_STREAMER_ID;
    }
    return 1;
}

stock Float:GetTrainingAccuracy(playerid)
{
    new shots = g_PlayerTraining[playerid][pTrain_Shots];
    if (shots <= 0) return 0.0;

    return (float(g_PlayerTraining[playerid][pTrain_Hits]) / float(shots)) * 100.0;
}

stock GetTrainingHudColors(playerid, colorOne_[], colorTwo_[], size1 = sizeof(colorOne_), size2 = sizeof(colorTwo_))
{
    format(colorOne_, size1, "%s", GetPlayerHudTag(playerid, false));
    format(colorTwo_, size2, "%s", GetPlayerHudTag(playerid, true));
    return 1;
}

stock bool:IsTrainingArenaWeapon(arenaid, weaponid)
{
    if (!IsValidTrainingArena(arenaid))
        return false;

    new count = g_TrainingArenas[arenaid][tArena_WeaponCount];
    for (new i = 0; i < count; i++)
    {
        if (g_TrainingArenas[arenaid][tArena_Weapons][i] == weaponid)
            return true;
    }
    return false;
}

stock GetTrainingMsLeft(playerid)
{
    new msLeft = g_PlayerTraining[playerid][pTrain_ChallengeEndTick] - GetTickCount();

    if (msLeft > TRAINING_CHALLENGE_TIME * 1000)
        return 0;

    return msLeft;
}

stock ShowTrainingHitmark(playerid, Float:amount, weaponid)
{
    WC_ShowDamageFeedGiven(playerid, playerid, amount, WEAPON:weaponid);
    return 1;
}

// challenge timer textdraw

stock CreateChallengeTimerTD(playerid)
{
    if (g_TrainingTimerTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
        return 1;

    g_TrainingTimerTD[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 415.000000, "~w~00:00");
    PlayerTextDrawFont(playerid, g_TrainingTimerTD[playerid], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, g_TrainingTimerTD[playerid], 0.220000, 0.950000);
    PlayerTextDrawSetOutline(playerid, g_TrainingTimerTD[playerid], 1);
    PlayerTextDrawSetShadow(playerid, g_TrainingTimerTD[playerid], 0);
    PlayerTextDrawAlignment(playerid, g_TrainingTimerTD[playerid], TEXT_DRAW_ALIGN_CENTRE);
    PlayerTextDrawColor(playerid, g_TrainingTimerTD[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, g_TrainingTimerTD[playerid], MAIN_BACKGROUND_COLOUR);
    PlayerTextDrawSetProportional(playerid, g_TrainingTimerTD[playerid], true);
    return 1;
}

stock DestroyChallengeTimerTD(playerid)
{
    if (g_TrainingTimerTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_TrainingTimerTD[playerid]);
        g_TrainingTimerTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }
    return 1;
}

stock ShowTrainingCountdownTD(playerid, const text[])
{
    if (g_TrainingCountdownTD[playerid] == PlayerText:INVALID_TEXT_DRAW)
    {
        g_TrainingCountdownTD[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 160.000000, text);
        PlayerTextDrawFont(playerid, g_TrainingCountdownTD[playerid], TEXT_DRAW_FONT_1);
        PlayerTextDrawLetterSize(playerid, g_TrainingCountdownTD[playerid], 0.400000, 1.600000);
        PlayerTextDrawAlignment(playerid, g_TrainingCountdownTD[playerid], TEXT_DRAW_ALIGN_CENTRE);
        PlayerTextDrawColor(playerid, g_TrainingCountdownTD[playerid], -1);
        PlayerTextDrawSetOutline(playerid, g_TrainingCountdownTD[playerid], 1);
        PlayerTextDrawSetShadow(playerid, g_TrainingCountdownTD[playerid], 0);
        PlayerTextDrawBackgroundColor(playerid, g_TrainingCountdownTD[playerid], MAIN_BACKGROUND_COLOUR);
        PlayerTextDrawSetProportional(playerid, g_TrainingCountdownTD[playerid], true);
    }
    else
    {
        PlayerTextDrawSetString(playerid, g_TrainingCountdownTD[playerid], text);
    }
    PlayerTextDrawShow(playerid, g_TrainingCountdownTD[playerid]);
    return 1;
}

stock DestroyTrainingCountdownTD(playerid)
{
    if (g_TrainingCountdownTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, g_TrainingCountdownTD[playerid]);
        g_TrainingCountdownTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }
    return 1;
}

public DestroyTrainingCountdownTimer(playerid)
{
    if (IsPlayerConnected(playerid) && !g_PlayerTraining[playerid][pTrain_IsOutOfBounds])
    {
        DestroyTrainingCountdownTD(playerid);
    }
    return 1;
}

stock UpdateTrainingHud(playerid, secondsLeft)
{
    if (!User[playerid][showPlayerHud])
        return 0;

    new colorOne_[20], colorTwo_[20];
    GetTrainingHudColors(playerid, colorOne_, colorTwo_);

    if (secondsLeft < 0) secondsLeft = 0;

    new bool:lowTime = (secondsLeft <= 5);
    new fString[512];

    if (User[playerid][hudType] == HUD_UNDERLINE || User[playerid][hudType] == HUD_RL)
    {
        new netLine[384];

        if (User[playerid][hudType] == HUD_RL)
            format(netLine, sizeof(netLine), "%sFPS: %s%d %sPing: %s%d~n~%sPL: %s%.1f%% %sSpeed: %s%d",
                colorOne_, colorTwo_, User[playerid][FPS],
                colorOne_, colorTwo_, GetPlayerPing(playerid),
                colorOne_, colorTwo_, NetStats_PacketLossPercent(playerid),
                colorOne_, colorTwo_, GetPlayerSpeed(playerid));
        else
            format(netLine, sizeof(netLine), "%sFPS: %s%d %sPL: %s%.1f",
                colorOne_, colorTwo_, User[playerid][FPS],
                colorOne_, colorTwo_, NetStats_PacketLossPercent(playerid));

        if (User[playerid][hudType] == HUD_RL)
        {
            format(fString, sizeof(fString), "%s~n~%sTraining: %s%d:%s%02d",
                netLine,
                colorOne_,
                lowTime ? ("~r~~h~") : (colorOne_), secondsLeft / 60,
                lowTime ? ("~r~~h~") : (colorTwo_), secondsLeft % 60
            );
        }
        else
        {
            format(fString, sizeof(fString), "%s~n~%s~n~%sTraining: %s%d:%s%02d",
                getDateStrTDInfo(colorOne_, colorTwo_), netLine,
                colorOne_,
                lowTime ? ("~r~~h~") : (colorOne_), secondsLeft / 60,
                lowTime ? ("~r~~h~") : (colorTwo_), secondsLeft % 60
            );
        }
        PlayerTextDrawSetString(playerid, FPSPingPacket[playerid], fString);
    }
    else
    {
        FormatNetworkDataForHud(User[playerid][hudType], colorOne_, colorTwo_, User[playerid][FPS], GetPlayerPing(playerid), NetStats_PacketLossPercent(playerid), fString, sizeof(fString));
        PlayerTextDrawSetString(playerid, FPSPingPacket[playerid], fString);

        format(fString, sizeof(fString), "~n~%sTraining: %s%d:%s%02d",
            colorOne_,
            lowTime ? ("~r~~h~") : (colorOne_), secondsLeft / 60,
            lowTime ? ("~r~~h~") : (colorTwo_), secondsLeft % 60
        );
        PlayerTextDrawSetString(playerid, InfoStats[playerid][1], fString);
        PlayerTextDrawShow(playerid, InfoStats[playerid][1]);
    }
    return 1;
}

stock UpdateChallengeTimerTD(playerid, secondsLeft)
{
    if (g_TrainingTimerTD[playerid] == PlayerText:INVALID_TEXT_DRAW)
        return 0;

    if (!IsValidTrainingArena(g_PlayerTraining[playerid][pTrain_ArenaID]))
        return 0;

    new colorOne_[20], colorTwo_[20];
    GetTrainingHudColors(playerid, colorOne_, colorTwo_);

    new str[144];
    format(
        str, sizeof(str),
        "%s%s~n~%s%d:%02d ~w~- DMG: %s%.0f",
        colorOne_,
        g_TrainingArenas[g_PlayerTraining[playerid][pTrain_ArenaID]][tArena_Name],
        colorTwo_,
        secondsLeft / 60,
        secondsLeft % 60,
        colorOne_,
        g_PlayerTraining[playerid][pTrain_Damage]
    );

    PlayerTextDrawSetString(playerid, g_TrainingTimerTD[playerid], str);
    PlayerTextDrawShow(playerid, g_TrainingTimerTD[playerid]);
    return 1;
}

// session handling

stock ResetPlayerTrainingData(playerid)
{
    KillTrainingTimer(g_PlayerTraining[playerid][pTrain_CountdownTimer]);
    KillTrainingTimer(g_PlayerTraining[playerid][pTrain_ChallengeTimer]);
    KillTrainingTimer(g_PlayerTraining[playerid][pTrain_WaypointTimer]);

    DestroyChallengeTimerTD(playerid);
    DestroyTrainingCountdownTD(playerid);

    g_PlayerTraining[playerid][pTrain_ChallengeOn] = false;

    g_PlayerTraining[playerid][pTrain_Active] = false;
    g_PlayerTraining[playerid][pTrain_ArenaID] = -1;
    g_PlayerTraining[playerid][pTrain_TargetObject] = INVALID_STREAMER_ID;
    g_PlayerTraining[playerid][pTrain_TargetHealth] = 0.0;
    g_PlayerTraining[playerid][pTrain_MovingToEnd] = false;
    g_PlayerTraining[playerid][pTrain_CurrentWaypoint] = 0;
    g_PlayerTraining[playerid][pTrain_Hits] = 0;
    g_PlayerTraining[playerid][pTrain_Shots] = 0;
    g_PlayerTraining[playerid][pTrain_Damage] = 0.0;
    g_PlayerTraining[playerid][pTrain_ChallengeEndTick] = 0;
    g_PlayerTraining[playerid][pTrain_SecondShown] = 0;
    g_PlayerTraining[playerid][pTrain_IsOutOfBounds] = false;
    g_PlayerTraining[playerid][pTrain_OutOfBoundsTime] = 0;
    g_PlayerTraining[playerid][pTrain_OutOfBoundsLastSec] = 0;

    if (IsPlayerLogged(playerid))
    {
        updatePlayerHud(playerid);
    }
    return 1;
}

stock EndTrainingSession(playerid)
{
    DestroyTrainingTargetObject(playerid);
    ResetPlayerTrainingData(playerid);
    return 1;
}

public TrainingOnSetAfk(playerid)
{
    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        if (g_PlayerTraining[playerid][pTrain_ChallengeOn])
        {
            SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}Challenge cancelled by the server: {MAIN}you were sent to the lounge{FFFFFF}. Nothing was saved.");
        }

        EndTrainingSession(playerid);
    }
    return 1;
}

stock ExitTrainingArena(playerid)
{
    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 0;

    EndTrainingSession(playerid);

    setAfk(playerid);

    if (User[playerid][showPlayerHud])
        showPlayerTextDraw(playerid);
    else
        hidePlayerTextdraw(playerid);

    return 1;
}

stock GiveArenaWeapons(playerid, arenaid)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    ResetPlayerWeapons(playerid);

    new count = g_TrainingArenas[arenaid][tArena_WeaponCount];
    for (new i = 0; i < count; i++)
    {
        new wep = g_TrainingArenas[arenaid][tArena_Weapons][i];
        if (wep > 0)
        {
            GivePlayerWeapon(playerid, WEAPON:wep, 9999);
        }
    }
    return 1;
}

stock ApplyTrainingEnvironment(playerid)
{
    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 0;

    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];
    if (!IsValidTrainingArena(arenaid))
        return 0;

    hidePlayerTextdraw(playerid);
    if (User[playerid][showPlayerHud])
    {
        PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
    }

    SetPlayerVirtualWorld(playerid, TRAINING_VW_BASE + playerid);
    SetPlayerInterior(playerid, 0);

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);

    GiveArenaWeapons(playerid, arenaid);

    SetPlayerPos(
        playerid,
        g_TrainingArenas[arenaid][tArena_SpawnX],
        g_TrainingArenas[arenaid][tArena_SpawnY],
        g_TrainingArenas[arenaid][tArena_SpawnZ]
    );
    SetPlayerFacingAngle(playerid, g_TrainingArenas[arenaid][tArena_SpawnA]);
    SetCameraBehindPlayer(playerid);
    return 1;
}

public RestoreTrainingSpawn(playerid)
{
    if (!IsPlayerConnected(playerid))
        return 1;

    ApplyTrainingEnvironment(playerid);
    return 1;
}

static const g_RandomOrbModels[] = { 19059, 19060, 19061, 19062, 19063 };

stock CreateTrainingTarget(playerid, arenaid)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    KillTrainingTimer(g_PlayerTraining[playerid][pTrain_WaypointTimer]);
    DestroyTrainingTargetObject(playerid);

    new targetModel = g_TrainingArenas[arenaid][tArena_TargetModel];
    new Float:startX = g_TrainingArenas[arenaid][tArena_TargetStartX];
    new Float:startY = g_TrainingArenas[arenaid][tArena_TargetStartY];
    new Float:startZ = g_TrainingArenas[arenaid][tArena_TargetStartZ];
    new wpIdx = 0;

    if (g_TrainingArenas[arenaid][tArena_IsRandomSpawning])
    {
        new wpCount = g_TrainingArenas[arenaid][tArena_WaypointCount];
        if (wpCount > 0)
        {
            new prevWp = g_PlayerTraining[playerid][pTrain_CurrentWaypoint];
            if (wpCount > 1 && (0 <= prevWp < wpCount))
            {
                wpIdx = (prevWp + 1 + random(wpCount - 1)) % wpCount;
            }
            else
            {
                wpIdx = random(wpCount);
            }

            startX = g_TrainingArenas[arenaid][tArena_WaypointX][wpIdx];
            startY = g_TrainingArenas[arenaid][tArena_WaypointY][wpIdx];
            startZ = g_TrainingArenas[arenaid][tArena_WaypointZ][wpIdx];
        }
        targetModel = g_RandomOrbModels[random(sizeof(g_RandomOrbModels))];
    }

    g_PlayerTraining[playerid][pTrain_TargetObject] = CreateDynamicObject(
        targetModel,
        startX, startY, startZ,
        0.0, 0.0,
        g_TrainingArenas[arenaid][tArena_TargetStartA],
        TRAINING_VW_BASE + playerid, 0, playerid, 300.0
    );

    g_PlayerTraining[playerid][pTrain_TargetHealth] = g_TrainingArenas[arenaid][tArena_TargetHP];
    g_PlayerTraining[playerid][pTrain_MovingToEnd] = false;
    g_PlayerTraining[playerid][pTrain_CurrentWaypoint] = wpIdx;
    return 1;
}

stock MoveTrainingTargetNext(playerid, arenaid)
{
    KillTrainingTimer(g_PlayerTraining[playerid][pTrain_WaypointTimer]);

    if (!IsValidTrainingArena(arenaid))
        return 0;

    if (g_TrainingArenas[arenaid][tArena_IsRandomSpawning])
    {
        return 1;
    }

    new targetObj = g_PlayerTraining[playerid][pTrain_TargetObject];

    if (targetObj == INVALID_STREAMER_ID || !IsValidDynamicObject(targetObj))
        return 0;

    new wpCount = g_TrainingArenas[arenaid][tArena_WaypointCount];
    if (wpCount <= 1)
        return 0;

    new prevWp = g_PlayerTraining[playerid][pTrain_CurrentWaypoint];
    new curr = prevWp;

    if (g_PlayerTraining[playerid][pTrain_MovingToEnd])
    {
        curr++;
        if (curr >= wpCount)
        {
            curr = wpCount - 2;
            g_PlayerTraining[playerid][pTrain_MovingToEnd] = false;
        }
    }
    else
    {
        curr--;
        if (curr < 0)
        {
            curr = 1;
            g_PlayerTraining[playerid][pTrain_MovingToEnd] = true;
        }
    }

    g_PlayerTraining[playerid][pTrain_CurrentWaypoint] = curr;

    new Float:speed = g_TrainingArenas[arenaid][tArena_TargetSpeed];
    if (speed <= 0.0) speed = 1.0;

    new Float:targetX = g_TrainingArenas[arenaid][tArena_WaypointX][curr];
    new Float:targetY = g_TrainingArenas[arenaid][tArena_WaypointY][curr];
    new Float:targetZ = g_TrainingArenas[arenaid][tArena_WaypointZ][curr];

    MoveDynamicObject(
        targetObj,
        targetX, targetY, targetZ,
        speed
    );

    new Float:prevX = g_TrainingArenas[arenaid][tArena_WaypointX][prevWp];
    new Float:prevY = g_TrainingArenas[arenaid][tArena_WaypointY][prevWp];
    new Float:prevZ = g_TrainingArenas[arenaid][tArena_WaypointZ][prevWp];

    new Float:dist = VectorSize(targetX - prevX, targetY - prevY, targetZ - prevZ);
    new travelMs = floatround((dist / speed) * 1000.0, floatround_round);

    if (travelMs < 50) travelMs = 50;

    g_PlayerTraining[playerid][pTrain_WaypointTimer] = SetTimerEx("OnTrainingWaypointTimer", travelMs, false, "i", playerid);
    return 1;
}

public OnTrainingWaypointTimer(playerid)
{
    g_PlayerTraining[playerid][pTrain_WaypointTimer] = 0;

    if (!IsPlayerConnected(playerid))
        return 1;

    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 1;

    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];
    if (!IsValidTrainingArena(arenaid))
        return 1;

    MoveTrainingTargetNext(playerid, arenaid);
    return 1;
}

stock MoveTrainingTarget(playerid, arenaid, bool:toEnd)
{
    if (IsValidTrainingArena(arenaid) && g_TrainingArenas[arenaid][tArena_IsRandomSpawning])
    {
        return 1;
    }

    g_PlayerTraining[playerid][pTrain_CurrentWaypoint] = 0;
    g_PlayerTraining[playerid][pTrain_MovingToEnd] = toEnd;
    return MoveTrainingTargetNext(playerid, arenaid);
}

stock EnterTrainingArena(playerid, arenaid)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    if (!IsPlayerLogged(playerid))
        return 0;

    if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot enter a training arena while in a duel.");
        return 0;
    }

    if (IsPlayerInDuel2v2(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot enter a training arena while in a 2v2 duel.");
        return 0;
    }

    if (IsPracticeActive(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}Leave the practice world first.");
        return 0;
    }

    if (IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot enter a training arena while in a vehicle.");
        return 0;
    }

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot enter a training arena right now.");
        return 0;
    }

    if (IsPlayerSpectating(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot enter a training arena while spectating.");
        return 0;
    }

    if (IsPlayerFrozen(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You cannot enter a training arena right now.");
        return 0;
    }

    if (!g_PlayerTraining[playerid][pTrain_Active] && !IsPlayerInLounge(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}The training mode can only be started from the lounge.");
        return 0;
    }

    if (g_PlayerTraining[playerid][pTrain_ChallengeOn])
    {
        SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}Your previous run was discarded because you restarted the challenge.");
    }

    EndTrainingSession(playerid);

    g_PlayerTraining[playerid][pTrain_Active] = true;
    g_PlayerTraining[playerid][pTrain_ArenaID] = arenaid;
    User[playerid][locationId] = ARENA_AFK;
    User[playerid][isAfk] = false;

    ApplyTrainingEnvironment(playerid);
    UpdateTrainingHud(playerid, TRAINING_CHALLENGE_TIME);

    CreateTrainingTarget(playerid, arenaid);

    va_SendClientMessage(
        playerid,
        -1,
        "{MAIN}[TRAINING] {FFFFFF}You entered arena {MAIN}%s{FFFFFF}! Shoot the moving target to train.",
        g_TrainingArenas[arenaid][tArena_Name]
    );

    ShowTrainingCountdownTD(playerid, "~r~3");
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    g_PlayerTraining[playerid][pTrain_CountdownTimer] = SetTimerEx("OnTrainingCountdownTimer", 1000, false, "ii", playerid, 2);
    return 1;
}

stock RestartTrainingChallenge(playerid)
{
    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 0;

    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];
    if (!IsValidTrainingArena(arenaid))
        return 0;

    return EnterTrainingArena(playerid, arenaid);
}

public OnTrainingCountdownTimer(playerid, count)
{
    g_PlayerTraining[playerid][pTrain_CountdownTimer] = 0;

    if (!IsPlayerConnected(playerid))
        return 1;

    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 1;

    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];
    if (!IsValidTrainingArena(arenaid))
        return 1;

    if (count == 2)
    {
        ShowTrainingCountdownTD(playerid, "~y~2");
        PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
        g_PlayerTraining[playerid][pTrain_CountdownTimer] = SetTimerEx("OnTrainingCountdownTimer", 1000, false, "ii", playerid, 1);
    }
    else if (count == 1)
    {
        ShowTrainingCountdownTD(playerid, "~g~1");
        PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
        g_PlayerTraining[playerid][pTrain_CountdownTimer] = SetTimerEx("OnTrainingCountdownTimer", 1000, false, "ii", playerid, 0);
    }
    else if (count == 0)
    {
        new goStr[24];
        format(goStr, sizeof(goStr), "%sGO!", GetPlayerHudTag(playerid, false));
        ShowTrainingCountdownTD(playerid, goStr);

        PlaySuccessSound(playerid);
        SetTimerEx("DestroyTrainingCountdownTimer", 1000, false, "i", playerid);

        MoveTrainingTarget(playerid, arenaid, true);
        StartTrainingChallenge(playerid, arenaid);
    }
    return 1;
}

stock HandleTrainingTargetMove(objectid)
{
    if (objectid == INVALID_STREAMER_ID)
        return 1;

    foreach (new i : Player)
    {
        if (!g_PlayerTraining[i][pTrain_Active]) continue;
        if (g_PlayerTraining[i][pTrain_TargetObject] != objectid) continue;

        new arenaid = g_PlayerTraining[i][pTrain_ArenaID];
        if (!IsValidTrainingArena(arenaid)) break;

        if (g_PlayerTraining[i][pTrain_WaypointTimer] != 0) break;

        MoveTrainingTargetNext(i, arenaid);
        break;
    }
    return 1;
}

// challenge

stock StartTrainingChallenge(playerid, arenaid)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    g_PlayerTraining[playerid][pTrain_Hits] = 0;
    g_PlayerTraining[playerid][pTrain_Shots] = 0;
    g_PlayerTraining[playerid][pTrain_Damage] = 0.0;

    g_PlayerTraining[playerid][pTrain_ChallengeOn] = true;
    g_PlayerTraining[playerid][pTrain_ChallengeEndTick] = GetTickCount() + (TRAINING_CHALLENGE_TIME * 1000);
    g_PlayerTraining[playerid][pTrain_SecondShown] = TRAINING_CHALLENGE_TIME;

    UpdateTrainingHud(playerid, TRAINING_CHALLENGE_TIME);

    CreateChallengeTimerTD(playerid);
    UpdateChallengeTimerTD(playerid, TRAINING_CHALLENGE_TIME);

    if (g_PlayerTraining[playerid][pTrain_ChallengeTimer] != 0)
        KillTimer(g_PlayerTraining[playerid][pTrain_ChallengeTimer]);

    g_PlayerTraining[playerid][pTrain_ChallengeTimer] = SetTimerEx("OnTrainingChallengeTick", TRAINING_CHALLENGE_TICK, true, "i", playerid);

    va_SendClientMessage(
        playerid, -1,
        "{MAIN}[TRAINING] {FFFFFF}You started the {MAIN}'%s'{FFFFFF} challenge! (Ping: {MAIN}%d{FFFFFF} | FPS: {MAIN}%d{FFFFFF})",
        g_TrainingArenas[arenaid][tArena_Name],
        GetPlayerPing(playerid),
        User[playerid][FPS]
    );

    va_SendClientMessage(
        playerid, -1,
        "{MAIN}[CHALLENGE] {FFFFFF}Time: {MAIN}%ds{FFFFFF} | Your Record: {MAIN}%d dmg{FFFFFF} | Server: {MAIN}%d dmg{FFFFFF}.",
        TRAINING_CHALLENGE_TIME,
        g_ChallengePlayerRecord[playerid][arenaid],
        g_ChallengeServerRecord[arenaid]
    );
    return 1;
}

stock CancelTrainingChallenge(playerid, const reason[], bool:teleport = true)
{
    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 0;

    va_SendClientMessage(
        playerid, -1,
        "{MAIN}[TRAINING] {FFFFFF}Training cancelled by the server: {MAIN}%s{FFFFFF}. Nothing was saved.",
        reason
    );

    if (teleport)
        ExitTrainingArena(playerid);
    else
        EndTrainingSession(playerid);

    return 1;
}

stock FinishTrainingChallenge(playerid)
{
    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];

    if (!IsValidTrainingArena(arenaid))
    {
        ExitTrainingArena(playerid);
        return 0;
    }

    new hits = g_PlayerTraining[playerid][pTrain_Hits];
    new shots = g_PlayerTraining[playerid][pTrain_Shots];
    new Float:damage = g_PlayerTraining[playerid][pTrain_Damage];
    new Float:accuracy = GetTrainingAccuracy(playerid);

    new score = floatround(damage, floatround_round);
    new previous = g_ChallengePlayerRecord[playerid][arenaid];

    new trArenaName[TRAINING_NAME_LEN];
    format(trArenaName, sizeof(trArenaName), "%s", g_TrainingArenas[arenaid][tArena_Name]);

    new record = SaveChallengeRecord(playerid, arenaid, score, hits, shots);

    PlaySuccessSound(playerid);

    va_SendClientMessage(
        playerid, -1,
        "{MAIN}[TRAINING] {FFFFFF}Time is up on {MAIN}'%s'{FFFFFF}! Damage: {MAIN}%d{FFFFFF} | Hits: %d/%d | Accuracy: {MAIN}%.1f%%",
        trArenaName, score, hits, shots, accuracy
    );

    switch (record)
    {
        case TRAINING_RECORD_SERVER:
        {
            va_SendClientMessage(
                playerid, -1,
                "{MAIN}[CHALLENGE] {FFFFFF}NEW SERVER RECORD on {MAIN}'%s'{FFFFFF} with {MAIN}%d dmg{FFFFFF}! (your previous: %d)",
                trArenaName, score, previous
            );
        }
        case TRAINING_RECORD_PERSONAL:
        {
            va_SendClientMessage(
                playerid, -1,
                "{MAIN}[CHALLENGE] {FFFFFF}New personal record on {MAIN}'%s'{FFFFFF}: {MAIN}%d dmg{FFFFFF} (previous: %d).",
                trArenaName, score, previous
            );
        }
        default:
        {
            va_SendClientMessage(
                playerid, -1,
                "{MAIN}[CHALLENGE] {FFFFFF}Your record on {MAIN}'%s'{FFFFFF} is still {MAIN}%d dmg{FFFFFF} (server: %d by %s).",
                trArenaName,
                previous,
                g_ChallengeServerRecord[arenaid],
                (g_ChallengeServerHolder[arenaid][0] != EOS) ? (g_ChallengeServerHolder[arenaid]) : ("nobody")
            );
        }
    }

    Ach_OnTrainingChallengeFinish(playerid, damage, accuracy, record);

    new trLog[256], trStatus[32];
    if(record == TRAINING_RECORD_SERVER) format(trStatus, sizeof(trStatus), "New Server Record");
    else if(record == TRAINING_RECORD_PERSONAL) format(trStatus, sizeof(trStatus), "New Personal Record");
    else format(trStatus, sizeof(trStatus), "Completed");

    format(trLog, sizeof(trLog), "**Player:** %s\n**Arena:** %s\n**Damage:** %s dmg\n**Hits:** %d/%d (%.1f%% Accuracy)\n**Result:** %s",
        User[playerid][userName], trArenaName, FormatMoney(score), hits, shots, accuracy, trStatus);
    SendDiscordEmbed(logs_training, "Training Challenge", trLog, (record == TRAINING_RECORD_SERVER) ? DISCORD_COLOR_SUCCESS : DISCORD_COLOR_INFO);

    ExitTrainingArena(playerid);
    SendClientMessage(playerid, -1, "{MAIN}[TRAINING] {FFFFFF}You were sent back to the lounge.");
    return 1;
}

public OnTrainingChallengeTick(playerid)
{
    if (!g_PlayerTraining[playerid][pTrain_Active] || !g_PlayerTraining[playerid][pTrain_ChallengeOn])
    {
        if (g_PlayerTraining[playerid][pTrain_ChallengeTimer] != 0)
        {
            KillTimer(g_PlayerTraining[playerid][pTrain_ChallengeTimer]);
            g_PlayerTraining[playerid][pTrain_ChallengeTimer] = 0;
        }
        return 1;
    }

    if (!IsPlayerConnected(playerid))
        return EndTrainingSession(playerid);

    if (gettime() - User[playerid][lastTimeUpdated] >= TRAINING_PAUSE_TIMEOUT)
        return CancelTrainingChallenge(playerid, "the game was paused");

    if (GetPlayerVirtualWorld(playerid) != TRAINING_VW_BASE + playerid)
        return CancelTrainingChallenge(playerid, "you left the arena");

    if (IsPlayerInAnyVehicle(playerid))
        return CancelTrainingChallenge(playerid, "you entered a vehicle");

    if (IsPlayerSpectating(playerid))
        return CancelTrainingChallenge(playerid, "you started spectating");

    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];
    if (IsValidTrainingArena(arenaid))
    {
        new Float:dist = GetPlayerDistanceFromPoint(
            playerid,
            g_TrainingArenas[arenaid][tArena_SpawnX],
            g_TrainingArenas[arenaid][tArena_SpawnY],
            g_TrainingArenas[arenaid][tArena_SpawnZ]
        );

        if (dist > TRAINING_MAX_DISTANCE)
        {
            if (!g_PlayerTraining[playerid][pTrain_IsOutOfBounds])
            {
                g_PlayerTraining[playerid][pTrain_IsOutOfBounds] = true;
                g_PlayerTraining[playerid][pTrain_OutOfBoundsTime] = gettime();
                g_PlayerTraining[playerid][pTrain_OutOfBoundsLastSec] = 3;

                ShowTrainingCountdownTD(playerid, "~r~RETURN TO AREA! (3s)");
                PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            }
            else
            {
                new elapsed = gettime() - g_PlayerTraining[playerid][pTrain_OutOfBoundsTime];
                new secondsRemaining = 3 - elapsed;

                if (secondsRemaining <= 0)
                {
                    DestroyTrainingCountdownTD(playerid);
                    g_PlayerTraining[playerid][pTrain_IsOutOfBounds] = false;
                    return CancelTrainingChallenge(playerid, "you left the training area for 3s");
                }
                else if (secondsRemaining != g_PlayerTraining[playerid][pTrain_OutOfBoundsLastSec])
                {
                    g_PlayerTraining[playerid][pTrain_OutOfBoundsLastSec] = secondsRemaining;
                    new strMsg[48];
                    format(strMsg, sizeof(strMsg), "~r~RETURN TO AREA! (%ds)", secondsRemaining);
                    ShowTrainingCountdownTD(playerid, strMsg);
                    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
                }
            }
        }
        else
        {
            if (g_PlayerTraining[playerid][pTrain_IsOutOfBounds])
            {
                g_PlayerTraining[playerid][pTrain_IsOutOfBounds] = false;
                g_PlayerTraining[playerid][pTrain_OutOfBoundsTime] = 0;
                g_PlayerTraining[playerid][pTrain_OutOfBoundsLastSec] = 0;
                DestroyTrainingCountdownTD(playerid);
            }
        }
    }

    new msLeft = GetTrainingMsLeft(playerid);

    if (msLeft <= 0)
        return FinishTrainingChallenge(playerid);

    new secondsLeft = (msLeft + 999) / 1000;

    if (secondsLeft != g_PlayerTraining[playerid][pTrain_SecondShown])
    {
        g_PlayerTraining[playerid][pTrain_SecondShown] = secondsLeft;
        UpdateChallengeTimerTD(playerid, secondsLeft);
        UpdateTrainingHud(playerid, secondsLeft);

        if (secondsLeft <= 5)
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    }
    return 1;
}

// bullet hit detection

stock Float:GetTrainingShotDistance(
    Float:x1, Float:y1, Float:z1,
    Float:x2, Float:y2, Float:z2,
    Float:px, Float:py, Float:pz,
    &Float:closeX, &Float:closeY, &Float:closeZ
)
{
    new Float:dx = x2 - x1, Float:dy = y2 - y1, Float:dz = z2 - z1;
    new Float:lenSq = (dx * dx) + (dy * dy) + (dz * dz);
    new Float:t = 0.0;

    if (lenSq > 0.0)
    {
        t = (((px - x1) * dx) + ((py - y1) * dy) + ((pz - z1) * dz)) / lenSq;

        if (t < 0.0) t = 0.0;
        else if (t > 1.0) t = 1.0;
    }

    closeX = x1 + (dx * t);
    closeY = y1 + (dy * t);
    closeZ = z1 + (dz * t);

    return VectorSize(px - closeX, py - closeY, pz - closeZ);
}

stock bool:IsTrainingShotAngleValid(
    Float:originX, Float:originY, Float:originZ,
    Float:endX, Float:endY, Float:endZ,
    Float:targetX, Float:targetY, Float:targetZ,
    Float:minDot = 0.50
)
{
    new Float:sX = endX - originX, Float:sY = endY - originY, Float:sZ = endZ - originZ;
    new Float:sLen = VectorSize(sX, sY, sZ);
    if (sLen <= 0.001) return false;

    new Float:tX = targetX - originX, Float:tY = targetY - originY, Float:tZ = targetZ - originZ;
    new Float:tLen = VectorSize(tX, tY, tZ);
    if (tLen <= 0.001) return true;

    new Float:dot = ((sX * tX) + (sY * tY) + (sZ * tZ)) / (sLen * tLen);
    return (dot >= minDot);
}

stock GetTrainingLagPos(playerid, arenaid, Float:objX, Float:objY, Float:objZ, &Float:lagX, &Float:lagY, &Float:lagZ)
{
    lagX = objX;
    lagY = objY;
    lagZ = objZ;

    if (!IsValidTrainingArena(arenaid))
        return 0;

    new ping = GetPlayerPing(playerid);
    if (ping <= 0) return 0;
    if (ping > TRAINING_LAG_PING_CAP) ping = TRAINING_LAG_PING_CAP;

    new currWp = g_PlayerTraining[playerid][pTrain_CurrentWaypoint];
    if (currWp < 0 || currWp >= g_TrainingArenas[arenaid][tArena_WaypointCount])
        currWp = 0;

    new Float:destX = g_TrainingArenas[arenaid][tArena_WaypointX][currWp];
    new Float:destY = g_TrainingArenas[arenaid][tArena_WaypointY][currWp];
    new Float:destZ = g_TrainingArenas[arenaid][tArena_WaypointZ][currWp];

    new Float:dirX = destX - objX, Float:dirY = destY - objY, Float:dirZ = destZ - objZ;
    new Float:len = VectorSize(dirX, dirY, dirZ);

    if (len <= 0.01) return 0;

    new Float:back = g_TrainingArenas[arenaid][tArena_TargetSpeed] * (float(ping) / 2000.0);

    lagX = objX - ((dirX / len) * back);
    lagY = objY - ((dirY / len) * back);
    lagZ = objZ - ((dirZ / len) * back);
    return 1;
}

stock Float:GetTrainingWeaponDamage(weaponid)
{
    switch (weaponid)
    {
        case WEAPON_DEAGLE:  return TRAINING_DMG_DEAGLE;
        case WEAPON_SHOTGUN: return TRAINING_DMG_SHOTGUN;
        case WEAPON_M4:      return TRAINING_DMG_M4;
        case WEAPON_SNIPER:  return TRAINING_DMG_SNIPER;
    }
    return TRAINING_DMG_DEFAULT;
}

stock DestroyTrainingTarget(playerid, arenaid)
{
    if (!IsValidTrainingArena(arenaid))
        return 0;

    new Float:accuracy = GetTrainingAccuracy(playerid);

    if (!g_TrainingArenas[arenaid][tArena_IsRandomSpawning])
    {
        va_SendClientMessage(
            playerid,
            -1,
            "{MAIN}[TRAINING] {FFFFFF}Target destroyed! Hits: {MAIN}%d{FFFFFF}/%d ({MAIN}%.1f%%{FFFFFF}) - Damage: {MAIN}%.0f",
            g_PlayerTraining[playerid][pTrain_Hits],
            g_PlayerTraining[playerid][pTrain_Shots],
            accuracy,
            g_PlayerTraining[playerid][pTrain_Damage]
        );
    }

    PlaySuccessSound(playerid);

    CreateTrainingTarget(playerid, arenaid);
    MoveTrainingTarget(playerid, arenaid, true);
    return 1;
}

stock HandleTrainingWeaponShot(playerid, weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (!g_PlayerTraining[playerid][pTrain_Active])
        return 1;

    if (!g_PlayerTraining[playerid][pTrain_ChallengeOn])
        return 1;

    new arenaid = g_PlayerTraining[playerid][pTrain_ArenaID];
    if (!IsValidTrainingArena(arenaid))
        return 1;

    if (!IsTrainingArenaWeapon(arenaid, weaponid))
        return 1;

    new targetObj = g_PlayerTraining[playerid][pTrain_TargetObject];
    if (targetObj == INVALID_STREAMER_ID || !IsValidDynamicObject(targetObj))
        return 1;

    g_PlayerTraining[playerid][pTrain_Shots]++;

    new Float:objX, Float:objY, Float:objZ;
    GetDynamicObjectPos(targetObj, objX, objY, objZ);

    new bool:isTargetHit = false;

    if (hittype == BULLET_HIT_TYPE_OBJECT || hittype == BULLET_HIT_TYPE_PLAYER_OBJECT)
    {
        if (Streamer_GetItemStreamerID(playerid, STREAMER_TYPE_OBJECT, hitid) == targetObj)
        {
            isTargetHit = true;
        }
    }

    if (!isTargetHit)
    {
        new Float:originX, Float:originY, Float:originZ;
        new Float:endX, Float:endY, Float:endZ;

        GetPlayerLastShotVectors(playerid, originX, originY, originZ, endX, endY, endZ);

        if (endX == 0.0 && endY == 0.0 && endZ == 0.0)
        {
            if (hittype != BULLET_HIT_TYPE_NONE)
                return 1;

            endX = fX;
            endY = fY;
            endZ = fZ;
            GetPlayerPos(playerid, originX, originY, originZ);
        }

        new Float:closeX, Float:closeY, Float:closeZ;
        new Float:dist = GetTrainingShotDistance(
            originX, originY, originZ,
            endX, endY, endZ,
            objX, objY, objZ,
            closeX, closeY, closeZ
        );

        new Float:bestTargetX = objX, Float:bestTargetY = objY, Float:bestTargetZ = objZ;
        new Float:lagX, Float:lagY, Float:lagZ;

        if (GetTrainingLagPos(playerid, arenaid, objX, objY, objZ, lagX, lagY, lagZ))
        {
            new Float:lagCloseX, Float:lagCloseY, Float:lagCloseZ;
            new Float:lagDist = GetTrainingShotDistance(
                originX, originY, originZ,
                endX, endY, endZ,
                lagX, lagY, lagZ,
                lagCloseX, lagCloseY, lagCloseZ
            );

            if (lagDist < dist)
            {
                dist = lagDist;
                closeX = lagCloseX;
                closeY = lagCloseY;
                closeZ = lagCloseZ;
                bestTargetX = lagX;
                bestTargetY = lagY;
                bestTargetZ = lagZ;
            }
        }

        if (dist <= GetTrainingTargetRadius(arenaid) && IsTrainingShotAngleValid(originX, originY, originZ, endX, endY, endZ, bestTargetX, bestTargetY, bestTargetZ, 0.50))
        {
            isTargetHit = true;
        }
    }

    if (!isTargetHit)
        return 1;

    new Float:dmgAmount = GetTrainingWeaponDamage(weaponid);

    // increment player training stats
    g_PlayerTraining[playerid][pTrain_Hits]++;
    g_PlayerTraining[playerid][pTrain_Damage] += dmgAmount;
    g_PlayerTraining[playerid][pTrain_TargetHealth] -= dmgAmount;

    if (g_PlayerTraining[playerid][pTrain_ChallengeOn])
    {
        UpdateChallengeTimerTD(playerid, g_PlayerTraining[playerid][pTrain_SecondShown]);
    }

    ShowTrainingHitmark(playerid, dmgAmount, weaponid);

    new snd = User[playerid][hitSoundID];
    if (snd == 0) snd = TRAINING_HIT_SOUND;
    PlayerPlaySound(playerid, snd, 0.0, 0.0, 0.0);

    if (g_PlayerTraining[playerid][pTrain_TargetHealth] <= 0.0)
    {
        DestroyTrainingTarget(playerid, arenaid);
    }
    return 1;
}

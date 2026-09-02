// server configuration, GMX, statistics, logs, and automatic punishments

forward PrintServerBanner();
public PrintServerBanner()
{
	ConsolePrintLine("  +------------------------------------------------------------+");
	ConsolePrintLine("  |                                                            |");
	ConsolePrintLine("  |    _     _                                                 |");
	ConsolePrintLine("  |   (c).-.(c)                                                |");
	ConsolePrintLine("  |    / ._. \\     __________  _____  _________                |");
	ConsolePrintLine("  |  __\\( Y )/__   \\______   \\ /  _  \\ \\_   ___ \\              |");
	ConsolePrintLine("  | (_.-/'-'\\-._)   |       _/ /  /_\\  \\/    \\  \\/             |");
	ConsolePrintLine("  |    ||   ||      |    |   \\ /    |    \\     \\____           |");
	ConsolePrintLine("  |  _.' `-' '._    |____|_  / \\____|__  /\\______  /           |");
	ConsolePrintLine("  | (.-./`-'\\.-.)          \\/          \\/        \\/            |");
	ConsolePrintLine("  |   `-'     `-'                                              |");
	ConsolePrintLine("  |                                                            |");
	ConsolePrintLine("  |  _____          __   __        __                   __     |");
	ConsolePrintLine("  | /  _  \\   _____/  |_|__| ____ |  |__   __________ _/  |_   |");
	ConsolePrintLine("  | /  /_\\  \\ /    \\   __|  _/ ___\\|  |  \\_/ __ \\___  \\\\   __\\ |");
	ConsolePrintLine("  | /    |    \\   |  \\  | |  \\  |___|   Y  \\  ___/ / __ \\|  |  |");
	ConsolePrintLine("  | \\____|__  /___|  /__| |__|\\___  |___|  /\\___ \\(____  /__|  |");
	ConsolePrintLine("  |         \\/     \\/             \\/     \\/     \\/     \\/      |");
	ConsolePrintLine("  |                                                            |");
	ConsolePrintLine("  +------------------------------------------------------------+");
	return 1;
}

stock LoadServerConfig()
{
    if(!fexist("server_config.ini"))
    {
        SaveServerConfig();
        return 1;
    }

    new File:file = fopen("server_config.ini", io_read);
    if(!file) return 0;

    new line[128], key[32], val[64];
    while(fread(file, line))
    {
        for(new i = strlen(line) - 1; i >= 0; i--)
        {
            if(line[i] == '\r' || line[i] == '\n') line[i] = EOS;
            else break;
        }

        if(sscanf(line, "p<=>s[32]s[64]", key, val)) continue;

        if(!strcmp(key, "MaxPing", true)) ServerMaxPing = strval(val);
        else if(!strcmp(key, "MaxPacketLoss", true)) ServerMaxPacketLoss = floatstr(val);
        else if(!strcmp(key, "MinFPS", true)) ServerMinFPS = strval(val);
        else if(!strcmp(key, "MaxFPS", true)) ServerMaxFPS = strval(val);
        else if(!strcmp(key, "R2Kick", true)) R2KickEnabled = (strval(val) != 0);
        else if(!strcmp(key, "OMPKick", true)) OMPKickEnabled = (strval(val) != 0);
        else if(!strcmp(key, "MaxWarnings", true)) ServerMaxWarnings = strval(val);
        else if(!strcmp(key, "ACCooldown", true)) ServerACCooldown = strval(val);
        else if(!strcmp(key, "MaxLoginAttempts", true)) ServerMaxLoginAttempts = strval(val);
        else if(!strcmp(key, "MaxAccountsPerIP", true)) ServerMaxAccountsPerIP = strval(val);
        else if(!strcmp(key, "AntiSpam", true)) ServerAntiSpam = (strval(val) != 0);
        else if(!strcmp(key, "AntiCapslock", true)) ServerAntiCapslock = (strval(val) != 0);
        else if(!strcmp(key, "AdminsCmd", true)) ServerAdminsCmd = (strval(val) != 0);
        else if(!strcmp(key, "HpArmourText", true)) ServerHpArmourText = (strval(val) != 0);
        else if(!strcmp(key, "SprintMacro", true)) ACSprintMacro = (strval(val) != 0);
        else if(!strcmp(key, "MaxAccuracy", true)) ServerMaxAccuracy = strval(val);
        else if(!strcmp(key, "DefaultHud", true))
        {
            ServerDefaultHud = strval(val);
            if(ServerDefaultHud < 0 || ServerDefaultHud >= MAX_HUD_TYPES) ServerDefaultHud = 1;
        }
        else if(!strcmp(key, "DefaultHudColor", true))
        {
            ServerDefaultHudColor = strval(val);
            if(ServerDefaultHudColor < 0 || ServerDefaultHudColor >= sizeof(hudPersonalization)) ServerDefaultHudColor = 0;
        }
        else if(!strcmp(key, "DefaultHudShadeOne", true))
        {
            ServerDefaultHudShadeOne = strval(val);
            if(ServerDefaultHudShadeOne < -1 || ServerDefaultHudShadeOne >= sizeof(hudShades)) ServerDefaultHudShadeOne = -1;
        }
        else if(!strcmp(key, "DefaultHudShadeTwo", true))
        {
            ServerDefaultHudShadeTwo = strval(val);
            if(ServerDefaultHudShadeTwo < -1 || ServerDefaultHudShadeTwo >= sizeof(hudShades)) ServerDefaultHudShadeTwo = -1;
        }
        else if(!strcmp(key, "DefaultChatMainColor", true))
        {
            ServerDefaultChatMainColor = strval(val);
            if(ServerDefaultChatMainColor < 0 || ServerDefaultChatMainColor > sizeof(hudShades)) ServerDefaultChatMainColor = 0;
        }
        else if(!strcmp(key, "DefaultHpBarColor", true))
        {
            ServerDefaultHpBarColor = strval(val);
            if(ServerDefaultHpBarColor < -1 || ServerDefaultHpBarColor >= sizeof(hudShades)) ServerDefaultHpBarColor = -1;
        }
    }
    fclose(file);
    return 1;
}

stock SaveServerConfig()
{
    new File:file = fopen("server_config.ini", io_write);
    if(!file) return 0;

    new str[128];
    format(str, sizeof(str), "MaxPing=%d\r\n", ServerMaxPing);
    fwrite(file, str);

    format(str, sizeof(str), "MaxPacketLoss=%.2f\r\n", ServerMaxPacketLoss);
    fwrite(file, str);

    format(str, sizeof(str), "MinFPS=%d\r\n", ServerMinFPS);
    fwrite(file, str);

    format(str, sizeof(str), "MaxFPS=%d\r\n", ServerMaxFPS);
    fwrite(file, str);

    format(str, sizeof(str), "R2Kick=%d\r\n", _:R2KickEnabled);
    fwrite(file, str);

    format(str, sizeof(str), "OMPKick=%d\r\n", _:OMPKickEnabled);
    fwrite(file, str);

    format(str, sizeof(str), "DefaultHud=%d\r\n", ServerDefaultHud);
    fwrite(file, str);

    format(str, sizeof(str), "DefaultHudColor=%d\r\n", ServerDefaultHudColor);
    fwrite(file, str);

    format(str, sizeof(str), "DefaultHudShadeOne=%d\r\nDefaultHudShadeTwo=%d\r\n", ServerDefaultHudShadeOne, ServerDefaultHudShadeTwo);
    fwrite(file, str);

    format(str, sizeof(str), "DefaultChatMainColor=%d\r\n", ServerDefaultChatMainColor);
    fwrite(file, str);

    format(str, sizeof(str), "DefaultHpBarColor=%d\r\n", ServerDefaultHpBarColor);
    fwrite(file, str);

    format(str, sizeof(str), "MaxWarnings=%d\r\nMaxLoginAttempts=%d\r\nMaxAccountsPerIP=%d\r\nACCooldown=%d\r\n",
        ServerMaxWarnings, ServerMaxLoginAttempts, ServerMaxAccountsPerIP, ServerACCooldown);
    fwrite(file, str);

    format(str, sizeof(str), "AntiSpam=%d\r\nAntiCapslock=%d\r\nAdminsCmd=%d\r\n",
        _:ServerAntiSpam, _:ServerAntiCapslock, _:ServerAdminsCmd);
    fwrite(file, str);

    format(str, sizeof(str), "SprintMacro=%d\r\n", _:ACSprintMacro);
    fwrite(file, str);

    format(str, sizeof(str), "HpArmourText=%d\r\n", _:ServerHpArmourText);
    fwrite(file, str);

    format(str, sizeof(str), "MaxAccuracy=%d\r\n", ServerMaxAccuracy);
    fwrite(file, str);

    fclose(file);
    printf("[SERVER CONFIG] Saved: MaxPing=%d, MaxPL=%.2f, MinFPS=%d, MaxFPS=%d, MaxAcc=%d, R2Kick=%d, DefaultHud=%d, DefaultHudColor=%d, DefaultChatMainColor=%d, DefaultHpBarColor=%d", ServerMaxPing, ServerMaxPacketLoss, ServerMinFPS, ServerMaxFPS, ServerMaxAccuracy, _:R2KickEnabled, ServerDefaultHud, ServerDefaultHudColor, ServerDefaultChatMainColor, ServerDefaultHpBarColor);
    return 1;
}

ReturnCI(playerid)
{
	new szSerial[41];
	gpci(playerid, szSerial, sizeof(szSerial));

	return szSerial;
}

StartVoteKick(targetid)
{
	votingInfo[isRunningVote]  = true;
	votingInfo[showingVoteTD]  = true;
	votingInfo[votingTime]     = gettime() + (2 * 60);
	votingInfo[votesYes]       = 0;
	votingInfo[votesNo]        = 0;

	for(new i; i < MAX_PLAYERS; i++)
	{
		playerVoteInfo[i][alreadyVoted] = false;
		playerVoteInfo[i][voteChoice]   = false;
	}

	SetPVarInt(targetid, "VoteKickTarget", 1);

	TextDrawSetString(votekickTextdraws[2], "0 (Y) - 0 (N)");

	for(new i; i < 3; i++)
		TextDrawShowForAll(votekickTextdraws[i]);

	return 1;
}

GetVoteKickTarget()
{
	foreach(new i : Player)
	{
		if(GetPVarInt(i, "VoteKickTarget") == 1)
			return i;
	}
	return INVALID_PLAYER_ID;
}

FinishVoteKick()
{
	new targetid = GetVoteKickTarget();

	for(new i; i < 3; i++)
		TextDrawHideForAll(votekickTextdraws[i]);

	if(votingInfo[votesYes] < VOTEKICK_MIN_VOTES)
	{
		va_SendClientMessageToAll(COLOR_GREEN, "The votekick ended without enough votes (%d of %d needed). Nothing will happen.", votingInfo[votesYes], VOTEKICK_MIN_VOTES);
	}
	else if(votingInfo[votesYes] > votingInfo[votesNo])
	{
		SendClientMessageToAll(COLOR_GREEN, "The votekick ended with more YES than NOs. The player will be kicked");

		if(targetid != INVALID_PLAYER_ID)
			KickEx(targetid, 1000);
	}
	else if(votingInfo[votesNo] > votingInfo[votesYes])
	{
		SendClientMessageToAll(COLOR_GREEN, "The votekick ended with more NOs than YES. The player will not be kicked");
	}
	else
	{
		SendClientMessageToAll(COLOR_GREEN, "The votekick ended with equal YES and NOs. Nothing will happen, try again later");
	}

	if(targetid != INVALID_PLAYER_ID)
		DeletePVar(targetid, "VoteKickTarget");

	votingInfo[showingVoteTD] = false;
	votingInfo[isRunningVote] = false;
	votingInfo[votingTime]    = 0;
	votingInfo[votesYes]      = 0;
	votingInfo[votesNo]       = 0;

	for(new i; i < MAX_PLAYERS; i++)
		playerVoteInfo[i][alreadyVoted] = false;

	return 1;
}

RegisterVote(playerid, bool:choice)
{
	if(!votingInfo[isRunningVote])
		return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}There isn't a votekick ongoing.");

	if(playerVoteInfo[playerid][alreadyVoted])
		return SendClientMessage(playerid, COLOR_RED, "{MAIN}[ERROR] {FFFFFF}You already voted.");

	playerVoteInfo[playerid][alreadyVoted] = true;
	playerVoteInfo[playerid][voteChoice]   = choice;

	if(choice) votingInfo[votesYes]++;
	else       votingInfo[votesNo]++;

	new string[50];
	format(string, sizeof(string), "%d (Y) - %d (N)", votingInfo[votesYes], votingInfo[votesNo]);
	TextDrawSetString(votekickTextdraws[2], string);

	return SendClientMessage(playerid, COLOR_GREEN, "{MAIN}[VOTEKICK] {FFFFFF}Your vote has been registered.");
}

KickEx(playerid, delayms = 1000)
{
	if (IsPlayerNPC(playerid)) return false;

	PlayerPlaySound(playerid, 12605, 0.0, 0.0, 0.0);
	SetTimerEx("KickPlayerEx", delayms, false, "i", playerid);

	return true;
}

AntiSpam(playerid, const var_name[], const flood_var[])
{
	if(!ServerAntiSpam) return true;

	if (!IsAdmin(playerid, 1))
	{
		if(gettime() <= GetPVarInt(playerid, var_name))
		{
			new flood = GetPVarInt(playerid, flood_var)+1;
			SetPVarInt(playerid, flood_var, flood);

			if(flood == 5)
			{
				SendClientMessage(playerid, COLOR_ABACATE, "{MAIN}[ANTI-SPAM] {FFFFFF}Calm down or you will be kicked!");
			}
			else if(flood > 7)
			{
				SendClientMessage(playerid, COLOR_ABACATE, "{MAIN}[ANTI-SPAM] {FFFFFF}You have been warned!");
				KickEx(playerid, 2000);
			}
			else{
				new samp = GetPVarInt(playerid, var_name) - gettime()+1;
				va_SendClientMessage(playerid, -1, KEEP_CALM_MSG, samp);
			}
			return false;
		}
		return true;
	}
	return true;
}

forward UpdateServerStats();
public UpdateServerStats()
{
	new playersInLocation[MAX_LOCATIONS];
	new mostPopulatedLocation = -1;
	new maxPlayersInLocation = 0;

	foreach(new i : Player)
	{
		UpdatePlayerData(i);
		if(User[i][locationId] >= 0 && User[i][locationId] < MAX_LOCATIONS)
		{
			if(User[i][locationId] == ARENA_AFK) continue; 

			playersInLocation[User[i][locationId]]++;
			if(playersInLocation[User[i][locationId]] > maxPlayersInLocation)
			{
				maxPlayersInLocation = playersInLocation[User[i][locationId]];
				mostPopulatedLocation = User[i][locationId];
			}
		}
	}

	new szArenaName[64] = "Lounge";
	new arenaIdVal = -1;
	
	if(mostPopulatedLocation != -1) {
		arenaIdVal = arenaInfo[mostPopulatedLocation][arenaId];
		if(arenaIdVal >= 0 && arenaIdVal < MAX_ZONES) {
			format(szArenaName, sizeof(szArenaName), "%s", arenaData[arenaIdVal][arenaName]);
		}
	}

	new query[512];
	mysql_format(dbHandle, query, sizeof(query), 
		"UPDATE `server_status` SET `current_arena_id` = %d, `current_arena_name` = '%e', `players_online` = %d, `last_updated` = NOW() WHERE `id` = 1",
		arenaIdVal, szArenaName, Iter_Count(Player));
	mysql_tquery(dbHandle, query);

	updateKillRank();
	UpdateTOP3Kills();
	UpdateTOP3Damage();
	//CLAN
	for(new i = 0; i != MAX_CLANS; i++)
	{
		if(ClanInfo[i][clanIDUsed])
		{
			SaveClanData(i);
		}
	}
	return true;
}


forward KickPlayerEx(playerid);
public KickPlayerEx(playerid)
{
	if (IsPlayerNPC(playerid)) return 1;
	Kick(playerid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(pVehicle[i] == vehicleid)
			pVehicle[i] = 999;
	}

	DestroyVehicle(vehicleid);
	return 1;
}

stock UpdatePlayerFPS(playerid)
{
	if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return 0;
	if(!IsPlayerLogged(playerid) || !User[playerid][isAlreadySpawned]) return 0;

	new drunk = GetPlayerDrunkLevel(playerid);
	if(drunk < 100)
	{
		SetPlayerDrunkLevel(playerid, 2000);
		User[playerid][dLlast] = 2000;
		User[playerid][lastFpsTick] = GetTickCount();
		return 0;
	}

	new curTick = GetTickCount();
	new delta = curTick - User[playerid][lastFpsTick];
	if(delta >= 250)
	{
		if(User[playerid][dLlast] > drunk)
		{
			new diff = User[playerid][dLlast] - drunk;
			new fps = (diff * 1000) / delta;
			if(fps >= 5 && fps <= 1000)
			{
				new currentFps = User[playerid][FPS];
				if(currentFps <= 0 || (currentFps - fps) >= 15 || (fps - currentFps) >= 15)
				{
					User[playerid][FPS] = fps;
				}
				else
				{
					User[playerid][FPS] = (fps * 4 + currentFps) / 5;
				}
			}
			User[playerid][dLlast] = drunk;
			User[playerid][lastFpsTick] = curTick;
		}
		else if(delta >= 1500)
		{
			User[playerid][dLlast] = drunk;
			User[playerid][lastFpsTick] = curTick;
		}
	}
	return 1;
}

forward OnScriptUpdate();
public OnScriptUpdate()
{
	UpdateTOP10Clans();
	new strm[500];

	for(new i; i < sizeof(arenaInfo); i++)
	{
		if(arenaInfo[i][isRunningArena])
		{
			arenaInfo[i][roundMints]--;

			if(arenaInfo[i][roundMints] < 0)
			{
				new nextArena = determineArenaID(i);

				foreach(new p : Player)
				{
					if(User[p][locationId] != i || GetPVarInt(p, "DuelS_State") != DUELS_STATE_NONE)
						continue;

					format(strm, sizeof(strm), ARENA_ENDED_MSG , GetLocationDisplaySlot(i)+1, arenaData[arenaInfo[i][arenaId]][arenaName], arenaInfo[i][arenaId],	arenaData[nextArena][arenaName], nextArena);
					SendClientMessage(p, -1, "%s", strm);
					GangZoneStopPulseForPlayer(p, e_ZONE_PULSE_ZONE, arenaInfo[i][arenaId]);
				}

				GetRoundRank(i);
                new oldArenaId = arenaInfo[i][arenaId];
				arenaInfo[i][arenaId] = nextArena;
				PrepareChangeArena(i, oldArenaId);
			}
		}
	}

	for(new i; i < MAX_PICKUPSS; i++)
	{
		if(pickupID[i] != -1 && pickupTime[i] != 0 && pickupTime[i] <= gettime())
		{
			DestroyPickup(pickupID[i]);
			pickupTime[i] = 0;
			pickupID[i] = -1;
			pickupWeapon[i] = 0;
		}
	}

	if(votingInfo[isRunningVote] && votingInfo[votingTime] != 0 && gettime() >= votingInfo[votingTime])
		FinishVoteKick();

	foreach(new i : Player)
	{
		if (IsPlayerNPC(i)) continue;

		if(User[i][lastDamagerId] != INVALID_PLAYER_ID && User[i][lastDamageTime] != -1 && gettime() >= User[i][lastDamageTime])
			User[i][lastDamagerId] = INVALID_PLAYER_ID, User[i][lastDamageTime] = -1;

		if(gettime() - User[i][lastTimeUpdated] >= 10 && User[i][isAlreadySpawned])
			setAfkReason(i, "pause");

		if(gettime() >= User[i][timerBox] && User[i][isShowingKSTD])
			UpdateKillstreakBox(i);

		if(User[i][muteTimestamp] != 0 && IsPlayerLogged(i))
		{
			if(gettime() >= User[i][muteTimestamp])
			{
				User[i][muteTimestamp] = 0;
				SendClientMessage(i, -1, "%s", UNMUTED_AUTO);
			}
		}

		if(IsPlayerSpectating(i))
			updateSpecInfo(i);

		if(IsPlayerLogged(i) && User[i][isAlreadySpawned] && !IsPlayerAFK(i))
		{
			if(IsFpsFreeArena(User[i][locationId]))
			{
				DeletePVar(i, "highFpsTicks");
				DeletePVar(i, "lowFpsTicks");
			}
			else
			{
				if(ServerMaxFPS > 0 && User[i][FPS] > ServerMaxFPS)
				{
					new count = GetPVarInt(i, "highFpsTicks") + 1;
					SetPVarInt(i, "highFpsTicks", count);
					if(count >= 5)
					{
						DeletePVar(i, "highFpsTicks");
						setAfkReason(i, "highfps");
					}
				}
				else DeletePVar(i, "highFpsTicks");

				if(ServerMinFPS > 0 && User[i][FPS] < ServerMinFPS && User[i][FPS] > 5)
				{
					new count = GetPVarInt(i, "lowFpsTicks") + 1;
					SetPVarInt(i, "lowFpsTicks", count);
					if(count >= 5)
					{
						DeletePVar(i, "lowFpsTicks");
						setAfkReason(i, "lowfps");
					}
				}
				else DeletePVar(i, "lowFpsTicks");
			}

			if(ArenaRequiresFpsLock(User[i][locationId]) && User[i][FPS] > ARENA_FPS_LOCK_VALUE)
			{
				new count = GetPVarInt(i, "lock102Ticks") + 1;
				SetPVarInt(i, "lock102Ticks", count);
				if(count >= 5)
				{
					DeletePVar(i, "lock102Ticks");
					setAfkReason(i, "fps102");
				}
			}
			else DeletePVar(i, "lock102Ticks");

			if(ServerMaxPing > 0 && GetPlayerPing(i) > ServerMaxPing)
			{
				new count = GetPVarInt(i, "highPingTicks") + 1;
				SetPVarInt(i, "highPingTicks", count);
				if(count >= 5)
				{
					DeletePVar(i, "highPingTicks");
					setAfkReason(i, "highping");
				}
			}
			else DeletePVar(i, "highPingTicks");

			if(ServerMaxPacketLoss > 0.0 && NetStats_PacketLossPercent(i) > ServerMaxPacketLoss)
			{
				new count = GetPVarInt(i, "highPlTicks") + 1;
				SetPVarInt(i, "highPlTicks", count);
				if(count >= 5)
				{
					DeletePVar(i, "highPlTicks");
					setAfkReason(i, "highpl");
				}
			}
			else DeletePVar(i, "highPlTicks");
		}

		if((GetPlayerWeapon(i) == WEAPON_SAWEDOFF || GetPlayerWeapon(i) == WEAPON_ROCKETLAUNCHER || GetPlayerWeapon(i) == WEAPON_MINIGUN) && !IsPlayerAFK(i))
		{
			KickEx(i, 300);
		}
	}
	return true;
}

forward CheckTime();
public CheckTime()
{
    getdate(ClockTime[Year], ClockTime[Month], ClockTime[Day]);
    gettime(ClockTime[Hour], ClockTime[Minute], ClockTime[Second]);

	CheckDay();
	return true;
}
static LastWeeklyCheck;
static LastGmxCheck;

forward CheckDay();
public CheckDay()
{
	new string[50];
	format(string, 50, "%s", GetWeekDay());
	if(isequal(string,"Monday"))
	{
		if(gettime()-LastWeeklyCheck > 120)
		{
			LastWeeklyCheck = gettime();
			if(ClockTime[Minute] == 00)
			{
				CheckTOP1Kills();
				CheckTOP1Damage();
				SendWeeklyResultsDiscord(true);
			}
		}
	}
	return 1;
}

stock RestartServerSafe()
{
	for(new i; i < 20; i++)
		SendClientMessageToAll(randomcolor[random(sizeof(randomcolor))], "Server restarting, back in a few seconds.");

	foreach(new i : Player)
	{
		if(IsPlayerLogged(i))
			UpdatePlayerData(i);
	}

	SendRconCommand("gmx");
	return 1;
}

forward AutomaticGMX();
public AutomaticGMX()
{
	if(gettime()-LastGmxCheck > 60)
	{
		LastGmxCheck = gettime();
		if(ClockTime[Hour] == 3 && ClockTime[Minute] == 00)
		{
			RestartServerSafe();
		}
	}
}

initializeServer(const gamemodeName[])
{
	SetNameTagDrawDistance(NT_DISTANCE);

	Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 1000);
	SetGameModeText(gamemodeName);

	DisableInteriorEnterExits();
	UsePlayerPedAnims();
	EnableStuntBonusForAll(false);
	SetVehiclePassengerDamage(true);
	SetDisableSyncBugs(true);
	SetTimer("OnScriptUpdate", 1000, true);
	SetTimer("UpdateFastHUD", 200, true);
	SetTimer("UpdateLifeLabels", 400, true);
	SetTimer("UpdateServerStats", 120000, true);
	WebConsoleInit();
	SetTimer("RewardClanTerritories", (5 * 60000), true);
	SetTimer("deleteDuplicateRows", (30*60000), true);
	SetTimer("CheckTime", 1000, true); // DATE/TIME SYSTEM
	SetTimer("WarmCustomSpawns", 1000, false);
	SetTimer("AutomaticGMX", 1000, true);
	LastWeeklyCheck = LastGmxCheck = gettime();
	SetWeaponDamage(WEAPON_SNIPER, DAMAGE_TYPE_STATIC, 40.0);
	SetWeaponDamage(WEAPON_M4, DAMAGE_TYPE_STATIC, 8.0);
	gmtextdraws();
	createZones();
	initArena();
	initTop3();
	updateKillRank();
	SetTimer("updateKillRank", 300000, true);
	LoadClans();
	UpdateTOP10Clans();
	CreateObject(19911, 1786.4516, -1301.2150, 119.3707, -89.8999, 0.0000, 90.0999);
	AddStaticPickup(1314, 0, 2068.8516, -1556.8025, 13.4260, 909);

	for (new i = 0, rand; i < 30; i++)
	{
		do{
			rand = random(200);
		}
		while (rand == 0 || rand == 1 || rand == 74 || rand == 86);

		AddPlayerClass(rand, 0, 0, 0, 0, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
	}

	SetTimer("SendGlobalMSG", 360000, true);
}

forward SendGlobalMSG();
public SendGlobalMSG()
{
	foreach(new i : Player)
	{
		if(IsPlayerLogged(i)) SendMSG(i);
	}
	return 1;
}

stock UpdateServerName(const newname[])
{
	format(ServerHostname, sizeof(ServerHostname), "%s", newname);
	new rconStr[128];
	format(rconStr, sizeof(rconStr), "hostname %s", newname);
	SendRconCommand(rconStr);

	foreach(new i : Player)
	{
		if(IsPlayerLogged(i))
		{
			if(LoginRC[i][10] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawSetString(i, LoginRC[i][10], ServerHostname);
			}
		}
	}
	return 1;
}

stock ClearChat(playerid, amount)
{
	for(new b = 0; b <= amount; b++) SendClientMessage(playerid, -1, "");
}


#define REJECTED_HIT_COOLDOWN (10)

static rejectedHitCooldown[MAX_PLAYERS];
static rejectedHitSkipped[MAX_PLAYERS];


public OnGameModeInit()
{
	SetTimer("PrintServerBanner", 1200, false);

	CA_Init();
	SetWeaponMaxRange(WEAPON_SNIPER, 450.0);
	SetWeaponShootRate(WEAPON_SNIPER, 750);
	DisableCrashDetectLongCall();
	Command_SetDeniedReturn(true);
	mysql_log(ERROR | WARNING);

	SetRespawnTime(2000);

	InitLogChannels();

	getdate(ClockTime[Year], ClockTime[Month], ClockTime[Day]);
	gettime(ClockTime[Hour], ClockTime[Minute], ClockTime[Second]);
	printf("%s - %d/%d/%d - %d:%d:%d",
				GetWeekDay(),
				ClockTime[Day],
				ClockTime[Month],
				ClockTime[Year],
				ClockTime[Hour],
				ClockTime[Minute],
				ClockTime[Second]
			);

	initDatabase();
	StartTicketWatcher();
	LoadServerConfig();
	LoadCustomArenasMaps();

	// websocket
	SetTimer("RegisterSlashCommands", 15000, false);
	SetTimer("UpdateDiscordBotActivity", 5000, false);
	SetTimer("UpdateDiscordBotActivity", 30000, true);

	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
		{
			playertextdraws(i);
		}
	}

	return true;
}

public OnGameModeExit()
{
	for(new i = 0; i != MAX_CLANS; i++)
	{
		if(ClanInfo[i][clanIDUsed])
		{
			SaveClanData(i);
		}
	}
	mysql_close(dbHandle);
	return true;
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
	printf("SQL ERROR %d on handle %d: %s", errorid, _:handle, error);
	printf("callback: %s | query: %s", callback, query);
}


public OnPlayerConnect(playerid)
{
	if (IsPlayerNPC(playerid)) return true;

	User[playerid] = User[MAX_PLAYERS];
	StopSpectate(playerid);

	GetPlayerIp(playerid, User[playerid][user_lastip], 18);
	GetPlayerName(playerid, User[playerid][userName], MAX_PLAYER_NAME + 1);
	GetPlayerVersion(playerid, User[playerid][playerVersion]);

	LogDiscordConnect(playerid);

	CheckISP(playerid);
	CheckAccountLimit(playerid);

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
	"SELECT `username`, `forced_omp` FROM `accounts` WHERE `username` = '%e' and `forced_omp` = 1", User[playerid][userName]);
	mysql_tquery(dbHandle, queryGlobal, "ForcedOMPData", "i", playerid);

	PreloadAnims(playerid);

	pVehicle[playerid] = 999;
	objecthporarmour[playerid] = INVALID_OBJECT_ID;
	rejectedHitCooldown[playerid] = 0;
	rejectedHitSkipped[playerid] = 0;

	ACSuspect[playerid][ac_rejectedHits] = 0;
	ACSuspect[playerid][ac_weaponFlags] = 0;
	ACSuspect[playerid][ac_lastTime] = 0;
	ACSuspect[playerid][ac_lastReason][0] = EOS;
	ACSuspect[playerid][ac_lastWeapon][0] = EOS;

	SetPlayerVirtualWorld(playerid, playerid);
	SetSpawnInfo(playerid, 0, 1, User[playerid][loungeX], User[playerid][loungeY], User[playerid][loungeZ], User[playerid][loungeA], WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
	SetPVarInt(playerid, "firstSpawn", 1);
	SetPVarInt(playerid, "CurrentShotId", 0);
	SetPVarInt(playerid, "LastHitedShotId", 0);

	playertextdraws(playerid);

	return true;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (IsPlayerNPC(playerid)) return true;

	LogDiscordDisconnect(playerid, reason);

	UpdatePlayerData(playerid, 1);
	hidePlayerTextdraw(playerid);
	UpdateDiscordBotActivity();

	if(pVehicle[playerid] != 999)
		DestroyVehicle(pVehicle[playerid]);

	pVehicle[playerid] = 999;
	User[playerid][isAfk] = false;

	CleanupPlayerTimers(playerid);
	if(SpawnGuardTimer[playerid])
	{
		KillTimer(SpawnGuardTimer[playerid]);
		SpawnGuardTimer[playerid] = 0;
	}
	if(SpawnProtectTimer[playerid])
	{
		KillTimer(SpawnProtectTimer[playerid]);
		SpawnProtectTimer[playerid] = 0;
	}
	if(DuelS_Timer[playerid])
	{
		KillTimer(DuelS_Timer[playerid]);
		DuelS_Timer[playerid] = 0;
	}
	if(BodyDeathTimer[playerid])
	{
		KillTimer(BodyDeathTimer[playerid]);
		BodyDeathTimer[playerid] = 0;
	}
	if(Timer3DText[playerid])
	{
		KillTimer(Timer3DText[playerid]);
		Timer3DText[playerid] = 0;
	}

	for(new i; i < MAX_ZONES; i++)
		HideZoneForPlayer(playerid,i);

	new szString[64], name[25];

	GetPlayerName(playerid, name, sizeof(name));
	format(szString, sizeof szString, "{92B5B8}*** {979E9E} %s left disconnected ", name);
    
    foreach(new i : Player)
    {
        if(User[i][showConnectMsg])
        {
            SendClientMessage(i, 0xC4C4C4FF, "%s", szString);
        }
    }

	foreach (new i : Player)
	{
		if (playerSpecID[i] == playerid)
			StopSpectate(i);
	}

	for (new i; i < MAX_LOCATIONS; i ++)
	{
		playerArenaInfo[i][playerid][arenaDMG] = 0;
		playerArenaInfo[i][playerid][firedShots] = 0;
		playerArenaInfo[i][playerid][hitedShots] = 0;
		playerArenaInfo[i][playerid][arenaKills] = 0;
		playerArenaInfo[i][playerid][arenaDeaths] = 0;
		playerArenaInfo[i][playerid][arenaExp] = 0;
		playerArenaInfo[i][playerid][arenaHeadshots] = 0;
	}

	if(User[playerid][OnTempREC])
	{
		new gTempIP[18], gTempUser[MAX_PLAYER_NAME + 1], gTempCountry[50 + 1], gTempISP[100 + 1];
		GetPlayerIp(playerid, gTempIP, sizeof(gTempIP));
		format(gTempUser, sizeof(gTempUser), "%s", User[playerid][userName]);
		format(gTempCountry, sizeof(gTempCountry), "%s", User[playerid][Country]);
		format(gTempISP, sizeof(gTempISP), "%s", User[playerid][Isp]);

		new
            bantimestamp = gettime(),
            hours = 9999,
            unbants = gettime()+hours*3600,
            ban_reason[32] = "Quit - temprec";

        mysql_format(dbHandle, queryGlobal, sizeof queryGlobal,
            "INSERT INTO `banimentos` (`username`, `ip`, `admin`, `reason`, `country`, `isp`, `ban_timestamp`, `unban_timestamp`) VALUES('%e', '%e', '%e', '%e', '%e', '%e', %d, %d)",
            gTempUser, gTempIP, "Server", ban_reason, gTempCountry, gTempISP, bantimestamp, unbants);
        mysql_tquery(dbHandle, queryGlobal);

        va_SendClientMessageToAll(-1, BAN_TEMPREC_REASON, "Server", gTempUser, hours, ban_reason);
		User[playerid][OnTempREC] = false;
	}

	lines[playerid] = 0;
	for(new i; i < MAX_LINES; i++)
		StringToCat[playerid][i] = "_";

	return true;
}

public OnPlayerPrepareDeath(playerid, animlib[32], animname[32], &anim_lock, &respawn_time)
{
	if(User[playerid][lastDamagerId] != INVALID_PLAYER_ID)
	{
		CallLocalFunction("OnPlayerDeath", "ddd", playerid, User[playerid][lastDamagerId], 53);
		User[playerid][lastDamagerId] = INVALID_PLAYER_ID;
		User[playerid][lastDamageTime] = -1;
		return true;
	}
	return true;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	if (IsPlayerNPC(playerid) || IsPracticeActive(playerid)) return true;

    SetClanColor(playerid);
	new iString[100];
	new oldMoney = GetPlayerMoney(playerid);
	if(reason == REASON_SUICIDE) reason = WEAPON_DROWN;

	Ach_OnPlayerDied(playerid);

	if(killerid == INVALID_PLAYER_ID && !IsPlayerAFK(playerid))
	{
		SendDeathMessageToRoom(INVALID_PLAYER_ID, playerid, reason, User[playerid][locationId]);
		User[playerid][playerDeaths]++;

		#if LOST_MONEY_ONSUICIDE != 0
		{
			new lost = (oldMoney > LOST_MONEY_ONSUICIDE) ? LOST_MONEY_ONSUICIDE : oldMoney;
			GivePlayerCash(playerid, -lost);
			ShowMoneyTextDraw(playerid, sprintf("~r~~h~-%s!", FormatMoney(lost)), 615.0, 56.0);
		}
		#endif
	}
	else if(killerid != INVALID_PLAYER_ID && IsPlayerConnected(killerid))
	{
		#if LOST_MONEY_ONDEATH != 0
			if(oldMoney > LOST_MONEY_ONDEATH)
			{
				GivePlayerCash(playerid, -LOST_MONEY_ONDEATH);
				ShowMoneyTextDraw(playerid, sprintf("~r~~h~-%s!", FormatMoney(LOST_MONEY_ONDEATH)), 615.0, 56.0);
			}
			else GivePlayerCash(playerid, -oldMoney);
		#endif

		new givemoney;
		switch(reason)
		{
			case 24: givemoney = 300; // Deagle
			case 25: givemoney = 500; // Shotgun
			case 34: givemoney = 200; // Sniper
			default: givemoney = 300; // Other weapons
		}

		if(IsPlayerVip(killerid))
		{
			givemoney *= 2;
		}

		if(GetPVarInt(killerid, "Spree") + 1 > 4)
		{
			givemoney *= 2;
			format(iString, sizeof(iString), "~y~~h~+%s! ~r~~h~2x!", FormatMoney(givemoney));
			GivePlayerCash(killerid, givemoney);
			ShowMoneyTextDraw(killerid, iString, 604.0, 56.0);
		}
		else
		{
			format(iString, sizeof(iString), "~y~~h~+%s!", FormatMoney(givemoney));
			GivePlayerCash(killerid, givemoney);
			ShowMoneyTextDraw(killerid, iString, 615.0, 56.0);
		}

		SendDeathMessageToRoom(killerid, playerid, reason, User[playerid][locationId]);

		new giveexp;
		switch(reason)
		{
			case 24: giveexp = 6+random(10);
			case 25: giveexp = 10+random(10);
			case 31: giveexp = 11+random(10);
			case 34: giveexp = 15+random(10);
			default: giveexp = 5+random(5);
		}

		if(IsPlayerInClan(killerid)) {
			giveexp = floatround(float(giveexp) * GetOutpostRespectMultiplier(GetPlayerClan(killerid)));
		}

		User[killerid][playerExp] += giveexp;
		User[killerid][playerKills]++;
		User[killerid][kills_semanais]++;
		User[playerid][playerDeaths]++;

		new killerLoc = User[killerid][locationId];
		new victimLoc = User[playerid][locationId];
		new bool:killerLocOk = (0 <= killerLoc < MAX_LOCATIONS);
		new bool:victimLocOk = (0 <= victimLoc < MAX_LOCATIONS);

		if(IsPlayerInClan(killerid) && GetPlayerClan(killerid) != GetPlayerClan(playerid))
		{
			ClanInfo[User[killerid][clanID]][clanKills]++;

			if(killerLocOk)
				ClanInfo[User[killerid][clanID]][clanArenaKills][killerLoc]++;
		}

		if(killerLocOk)
		{
			playerArenaInfo[killerLoc][killerid][arenaKills]++;
			playerArenaInfo[killerLoc][killerid][arenaExp] += giveexp;

			if(GetPVarInt(playerid, "LastBodyPart") == 9)
				playerArenaInfo[killerLoc][killerid][arenaHeadshots]++;
		}

		if(victimLocOk)
			playerArenaInfo[victimLoc][playerid][arenaDeaths]++;

		g_PlayerKillsAgainst[killerid][playerid]++;
		ShowBody3DDeathTag(killerid, playerid, givemoney);

		if(User[killerid][killSoundID] != 0)
			PlayerPlaySound(killerid, User[killerid][killSoundID], 0.0, 0.0, 0.0);

        if(GetPVarInt(playerid, "LastBodyPart") == 9 && User[killerid][showHeadshot])
        {
        //    GameTextForPlayer(killerid, "~r~HEADSHOT", 2000, 5);
        }

		dropPlayerSniper(playerid);

		if(GetPVarInt(playerid, "Spree") >= 5)
		{
			new spreetime = gettime() - User[playerid][spreeTime];
			new timestr[32];

			if(spreetime < 0) spreetime = 0;

			if(spreetime >= 60)
			{
				format(timestr, sizeof(timestr), "%d minutes and %d", spreetime / 60, spreetime % 60);
			}
			else {
				format(timestr, sizeof(timestr), "%d", spreetime);
			}

			new spreeKiller[MAX_PLAYER_NAME + 1], spreeVictim[MAX_PLAYER_NAME + 1];
			GetPlayerName(killerid, spreeKiller, sizeof(spreeKiller));
			GetPlayerName(playerid, spreeVictim, sizeof(spreeVictim));

			va_SendClientMessageToAll(-1, DOWN_SPREE_MSG, (GetPlayerColor(killerid) >>> 8), spreeKiller, (GetPlayerColor(playerid) >>> 8) ,spreeVictim, GetPVarInt(playerid, "Spree"));
			va_SendClientMessage(playerid, -1, END_KILLSPREE, GetPVarInt(playerid, "Spree"), timestr);
		}

		if(GetPVarInt(playerid, "Spree") > User[playerid][maxStreak])
		{
			User[playerid][maxStreak] = GetPVarInt(playerid, "Spree");
			SendClientMessage(playerid, COLOR_ORANGE, "{MAIN}[INFO] {FFFFFF}Your killing streak has been successfully updated!");
		}

		DeletePVar(playerid, "Spree");
		User[playerid][spreeTime] = 0;

		SetPVarInt(killerid, "Spree", GetPVarInt(killerid, "Spree")+1);

		if(GetPVarInt(killerid, "Spree") == 1)
			User[killerid][spreeTime] = gettime();

		rSpree(killerid, playerid);

		if(killerLocOk)
			arenaInfo[killerLoc][totalArenaKills] ++;

		RandomDeathMessages(killerid, playerid, reason);

		User[playerid][timerBox] = gettime()+2;
		format(iString, sizeof(iString), "%sKilled by %s%s",
					GetPlayerHudTag(playerid, false),
					GetPlayerHudTag(playerid, true),
					getPlayerName(killerid));

		PlayerTextDrawSetString(playerid, DeathInfo[playerid][1], iString);
		if(User[playerid][showPlayerHud]) PlayerTextDrawShow(playerid, DeathInfo[playerid][1]);
		SetTimerEx("DeathMessage", 4000, false, "i", playerid);

		if(levelGoal(User[killerid][playerLevel]) <= User[killerid][playerKills])
		{
			SendClientMessage(killerid, -1, "%s", NEXT_LEVEL_MSG);
			User[killerid][playerLevel]++;
			new strLevel[64];
			format(strLevel, sizeof (strLevel), "~y~~h~congratulations!~n~~w~you achieved ~y~~h~level %d", User[killerid][playerLevel]);
			GameTextForPlayer(killerid, strLevel, 2000, 4);
			CheckPlayerLevelUp(killerid);
		}

		Ach_OnPlayerKill(killerid, playerid, reason);

		if(objecthporarmour[playerid] != INVALID_OBJECT_ID)
		{
			DestroyObject(objecthporarmour[playerid]), KillTimer(hpandarmourtimer[playerid]);
			objecthporarmour[playerid] = INVALID_OBJECT_ID;
		}

		SetPlayerArmour(killerid, 100.0);
		SetPlayerHealth(killerid, 100.0);
	}

	return true;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	if(IsPlayerDying(playerid))
		return false;

	new armedweapo = GetPlayerWeapon(playerid);
	for(new i; i < MAX_PICKUPSS; i++)
	{
		if(pickupid == pickupID[i] && pickupID[i] != -1)
		{
			GivePlayerWeapon(playerid, WEAPON:pickupWeapon[i], pickupData[i]);
			SetPlayerArmedWeapon(playerid, WEAPON:armedweapo);
			DestroyPickup(pickupID[i]);
			pickupID[i] = -1;
			pickupTime[i] = 0;
			pickupWeapon[i] = 0;
			break;
		}
	}
	return true;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerInterior(playerid, 0);
    SetPlayerPos(playerid, -1504.1549,595.7162,42.6563);
	SetPlayerFacingAngle(playerid, 200.5717);
	User[playerid][isAlreadySpawned] = false;

	if(User[playerid][playerSkin] != 0 && User[playerid][playerSkin] != 1)
	{
		SetPlayerSkin(playerid, User[playerid][playerSkin]);
	}
 	return true;
}

public OnPlayerRequestSpawn(playerid)
{
	if(IsPlayerLogged(playerid))
	{
		User[playerid][locationId] = -1;
		SetSpawnInfo(playerid, 0, GetPlayerSkin(playerid), User[playerid][loungeX], User[playerid][loungeY], User[playerid][loungeZ], User[playerid][loungeA], WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
		showLocationSelectDialog(playerid);
		return false;
	}

	SendClientMessage(playerid, -1, "%s", CANNOT_DO_THIS);
	return false;
}

public OnPlayerSpawn(playerid)
{
	if (IsPlayerNPC(playerid)) return true;

	new bool:inTourneyMatch = (IsPlayerInTournament(playerid) && g_PlayerTourneyMatch[playerid] != -1);
	new bool:inDuel = (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE);
	new bool:inDuel2v2 = (IsPlayerInDuel2v2(playerid) != 0);

	SetPlayerWorldBounds(playerid, 20000.0, -20000.0, 20000.0, -20000.0);
    SetPlayerFightingStyle(playerid, User[playerid][fightStyle]);

	if(User[playerid][playerSkin] > 0)
	{
		SetPlayerSkin(playerid, User[playerid][playerSkin]);
	}

	if(votingInfo[showingVoteTD])
	{
		for(new i; i < sizeof(votekickTextdraws); i++)
			TextDrawShowForPlayer(playerid, votekickTextdraws[i]);
	}

	foreach (new i : Player)
	{
		if (playerSpecID[i] == playerid)
			SetTimerEx("reSpectate", 1500, false, "ii", i, playerid);
	}

	if(IsPlayerInAnyVehicle(playerid))
		RemovePlayerFromVehicle(playerid);

	if(!inTourneyMatch && !inDuel && !inDuel2v2)
	{
		SetPlayerInterior(playerid, 0);
		givePlayerWeapons(playerid);

		if(IsPlayerAFK(playerid))
			setAfk(playerid);
		else
			spawnOnLocation(playerid);
	}

	SetPlayerTime(playerid, User[playerid][time], 0);
	SetPlayerWeather(playerid, User[playerid][weather]);

	if(GetPVarInt(playerid, "firstSpawn") == 1)
	{
		for (new i; i < MAX_ZONES; i ++)
		{
			ShowZoneForPlayer(playerid, i, ZoneColour[i], 0xFFFFFFFF, 0xFFFFFFFF);
		}

		hideSpecTD(playerid);

		if(User[playerid][showPlayerHud] == false)
			hidePlayerTextdraw(playerid);
		else
			showPlayerTextDraw(playerid);

		if (User[playerid][locationId] != -1)
		{
			new zoneid = arenaInfo[User[playerid][locationId]][arenaId];
			// GangZonePulseForPlayer(playerid, e_ZONE_PULSE_ZONE, zoneid, ZoneColour[zoneid] | 101, ZoneColour[zoneid], 1000);
			ZoneNumberFlashForPlayer(playerid, zoneid, 0xCC0000FF);
		}

		SetPlayerScore(playerid, SCOREBOARD_SCORE);

		CancelSelectTextDraw(playerid);
		DeletePVar(playerid, "firstSpawn");
	}

    User[playerid][isAlreadySpawned] = true;

	User[playerid][isMobile] = (IsPlayerAndroid(playerid)) ? true : false;

	if(!inTourneyMatch)
	{
		SetPlayerArmour(playerid, 100.0);
		SetPlayerHealth(playerid, 100.0);
	}

    User[playerid][timerRespawn] = gettime();
    if(!IsPlayerInLounge(playerid))
    {
        if(!HasPlayerConfiguredWeapons(playerid))
        {
            SetPVarInt(playerid, "showGunmenuFirst", 1);
            ShowGunMenu(playerid);
        }
        else
        {
            SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}You can change your guns in the next 15 seconds. Use {MAIN}/gunmenu");
        }
    }

    SetClanColor(playerid);

	return true;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
    if(!IsPlayerLogged(playerid)) return false;
    if(!IsPlayerLogged(clickedplayerid)) return false;

    SetPVarInt(playerid, "profile_target", clickedplayerid);
    
    new menu_str[2048], actions[24], count = 0;
    strcat(menu_str, "Option\tDescription\n");
    
    if(clickedplayerid == playerid)
    {
        format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}My Profile\t{BEBEBE}View your comprehensive statistics.\n", menu_str);
        actions[count++] = 1; // Stats

        format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Achievements\t{BEBEBE}View your unlocked achievements and progress.\n", menu_str);
        actions[count++] = 16; // Achievements

        format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Ask Question\t{BEBEBE}Send a question to the administration.\n", menu_str);
        actions[count++] = 15; // Ask Question

        if(IsPlayerInClan(clickedplayerid))
        {
            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Clan Information\t{BEBEBE}View details about your clan.\n", menu_str);
            actions[count++] = 3; // Clan Info
        }
    }
    else
    {
        format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}General stats\t{BEBEBE}Comprehensive player statistics.\n", menu_str);
        actions[count++] = 1; // Stats

        format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Achievements\t{BEBEBE}View this player's unlocked achievements.\n", menu_str);
        actions[count++] = 16; // Achievements

        format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Reports\t{BEBEBE}Report this player for misconduct.\n", menu_str);
        actions[count++] = 2; // Report

        if(IsPlayerInClan(clickedplayerid))
        {
            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Clan Information\t{BEBEBE}View details about this player's clan.\n", menu_str);
            actions[count++] = 3; // Clan Info
        }
        
        if (IsAdmin(playerid, 1))
        {
            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Name History\t{BEBEBE}View player nickname history.\n", menu_str);
            actions[count++] = 4; // Names

            if (IsAdmin(playerid, 3))
            {
                format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Go\t{BEBEBE}Teleport to this player's position.\n", menu_str);
                actions[count++] = 6;

                format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Get\t{BEBEBE}Teleport this player to your position.\n", menu_str);
                actions[count++] = 7;
            }

            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Slap\t{BEBEBE}Apply a slap to this player.\n", menu_str);
            actions[count++] = 8;

            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Mute\t{BEBEBE}Mute this player for a specific reason.\n", menu_str);
            actions[count++] = 9;

            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Unmute\t{BEBEBE}Restore this player's ability to speak.\n", menu_str);
            actions[count++] = 10;

            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Kick\t{BEBEBE}Remove this player from the server.\n", menu_str);
            actions[count++] = 11;

            if (IsAdmin(playerid, 2))
            {
                format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Ban\t{BEBEBE}Permanently block this player from the server.\n", menu_str);
                actions[count++] = 12;
            }

            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Spec\t{BEBEBE}Spectate this player's movements.\n", menu_str);
            actions[count++] = 13;

            if (IsAdmin(playerid, 4))
            {
                format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Gtemp\t{BEBEBE}Force a temporary record requirement.\n", menu_str);
                actions[count++] = 14;
            }
        }
        
        if (IsAdmin(playerid, 4))
        {
            format(menu_str, sizeof(menu_str), "%s{MAIN}> {FFFFFF}Penalty History\t{BEBEBE}View administrative penalty logs.", menu_str);
            actions[count++] = 5; // Penalties
        }
    }

    for(new i = 0; i < count; i++)
    {
        new pvar[20];
        format(pvar, sizeof(pvar), "profile_act_%d", i);
        SetPVarInt(playerid, pvar, actions[i]);
    }
    SetPVarInt(playerid, "profile_act_count", count);

    new title[64];
    format(title, sizeof(title), "{MAIN}Profile: {FFFFFF}%s", getPlayerName(clickedplayerid));

    format_MainColor(playerid, menu_str);
    format_MainColor(playerid, title);
    Dialog_ShowCallback(playerid, using public DIALOG_PROFILE_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, menu_str, "Select", "Close");
	return true;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (!IsPlayerAFK(playerid))
		return false;

	SetPlayerInterior(playerid, 0);
	CA_FindZ_For2DCoord(fX, fY, fZ);
	SetPlayerPos(playerid, fX, fY, fZ + 2.0);
	return true;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
	if (R2KickEnabled && newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_NONE && !GetPVarInt(playerid, "NotAndroid"))
	{
		SendClientMessage(playerid, 0xFF000000, "{MAIN}[ERROR] {FFFFFF}Restart your game to join. Tell your client developer to fix his broken code.");
		return KickEx(playerid);
	}

	if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER)
	{
		if(User[playerid][showPlayerHud]) PlayerTextDrawShow(playerid, speedometer[playerid]);

		if(pVehicle[playerid] != 999)
			AddVehicleComponent(pVehicle[playerid], 1010);
	}
	else
		PlayerTextDrawHide(playerid, speedometer[playerid]);

	if(oldstate == PLAYER_STATE_DRIVER && newstate == PLAYER_STATE_ONFOOT)
	{
		if(pVehicle[playerid] != 999)
		{
			DestroyVehicle(pVehicle[playerid]);
			pVehicle[playerid]  = 999;
		}
	}
	return true;
}

public OnPlayerGiveDamageDynamicActor(playerid, STREAMER_TAG_ACTOR:actorid, Float:amount, weaponid, bodypart)
{
	PlayerPlaySound(playerid, 17802, 0, 0, 0);
	return true;
}

public OnPlayerDamage(&playerid, &Float:amount, &issuerid, &WEAPON:weapon, &bodypart)
{
	if (playerid == INVALID_PLAYER_ID || !IsPlayerLogged(playerid))
	{
		return false;
	}

	if(GetPVarInt(playerid, "firstSpawn") == 1)
		return false;

	if(weapon == WEAPON_COLLISION)
	{
		if(ACNoFallAnimation) ClearAnimations(playerid, SYNC_ALL);
		if(ACNoFallDamage) return false;
		return true;
	}

	if(!IsPlayerLogged(issuerid))
	{
		return false;
	}

	if(ACFastStuns) ClearAnimations(playerid, SYNC_ALL);

	new bool:pInDuel = (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE && GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_WAITING);
	new bool:iInDuel = (GetPVarInt(issuerid, "DuelS_State") != DUELS_STATE_NONE && GetPVarInt(issuerid, "DuelS_State") != DUELS_STATE_WAITING);

	new bool:pInDuel2v2 = (IsPlayerInDuel2v2(playerid) != 0);
	new bool:iInDuel2v2 = (IsPlayerInDuel2v2(issuerid) != 0);

	new bool:pInTourney = IsPlayerInActiveTourneyMatch(playerid);
	new bool:iInTourney = IsPlayerInActiveTourneyMatch(issuerid);

	if (pInTourney || iInTourney)
	{
		if (!pInTourney || !iInTourney) return false;

		new matchid = g_PlayerTourneyMatch[playerid];
		if (matchid < 0 || matchid >= MAX_TOURNEY_MATCHES) return false;
		if (g_PlayerTourneyMatch[issuerid] != matchid) return false;
		if (!g_TourneyMatches[matchid][tm_Active] || g_TourneyMatches[matchid][tm_State] != MATCH_STATE_ACTIVE) return false;

		if (g_PlayerTourneySlot[playerid] == g_PlayerTourneySlot[issuerid]) return false;
	}
	else if (pInDuel2v2 || iInDuel2v2)
	{
		if (!pInDuel2v2 || !iInDuel2v2) return false;

		new matchid = g_PlayerDuel2v2ID[playerid];
		if (matchid < 0 || matchid >= MAX_DUELS_2V2) return false;
		if (g_PlayerDuel2v2ID[issuerid] != matchid) return false;
		if (!g_Duel2v2Data[matchid][d2_Active]) return false;
		if (g_Duel2v2Data[matchid][d2_State] != DUEL2V2_STATE_ACTIVE) return false;

		if (g_PlayerDuel2v2Team[playerid] == g_PlayerDuel2v2Team[issuerid]) return false;
		if (!IsDuel2v2PlayerAlive(playerid) || !IsDuel2v2PlayerAlive(issuerid)) return false;
	}
	else if (pInDuel || iInDuel)
	{
		if (GetPVarInt(playerid, "DuelS_State") != issuerid || GetPVarInt(issuerid, "DuelS_State") != playerid)
		{
			return false;
		}
	}
	else if (
		(User[issuerid][locationId] == -1 && User[playerid][locationId] == -1) ||
		(User[issuerid][locationId] != User[playerid][locationId])
	)
	{
		return false;
	}

	if(weapon != WEAPON_FIST && weapon != WEAPON_DEAGLE && weapon != WEAPON_SHOTGUN && weapon != WEAPON_SNIPER && weapon != WEAPON_M4 && weapon != WEAPON_GRENADE && weapon != WEAPON_KATANA)
		return false;

	User[playerid][lastDamagerId] = issuerid;
	User[playerid][lastDamageTime] = gettime()+10;

	if(IsPlayerAFK(playerid) || IsPlayerAFK(issuerid))
		return false;

	if(weapon == WEAPON_DEAGLE)
	{
		switch(bodypart)
		{
			case 3: amount = 35.0;
			case 4: amount = 35.0;
			case 5: amount = 30.0;
			case 6: amount = 30.0;
			case 7: amount = 30.0;
			case 8: amount = 30.0;
			case 9: 
            {
                amount = 40.0;
                if(User[issuerid][showHeadshot]) GameTextForPlayer(issuerid, "~r~~h~HEADSHOT~n~~g~~h~DEAGLE", 2000, 1);
            }
		}
	}
    
    if(weapon == WEAPON_SNIPER && bodypart == 9)
    {
        amount = 48.0;
        if(User[issuerid][showHeadshot]) GameTextForPlayer(issuerid, "~r~~h~HEADSHOT~n~~g~~h~SNIPER", 2000, 1);
    }

    if(User[issuerid][hitSoundID] != 0)
		PlayerPlaySound(issuerid, User[issuerid][hitSoundID], 0.0, 0.0, 0.0);

    SetPVarInt(playerid, "LastBodyPart", bodypart);

	new loc_issuer = User[issuerid][locationId];
	new bool:loc_issuer_ok = (0 <= loc_issuer < MAX_LOCATIONS) && loc_issuer != ARENA_AFK && !pInDuel && !iInDuel && !pInDuel2v2 && !iInDuel2v2 && !pInTourney && !iInTourney && !g_PlayerTraining[issuerid][pTrain_Active] && !g_Practice[issuerid][pracActive];

	if(loc_issuer_ok && (weapon == WEAPON_DEAGLE || weapon == WEAPON_SHOTGUN || weapon == WEAPON_M4 || weapon == WEAPON_SNIPER))
	{
		if(GetPVarInt(issuerid, "LastHitedShotId") != GetPVarInt(issuerid, "CurrentShotId"))
		{
			playerArenaInfo[loc_issuer][issuerid][hitedShots]++;
			SetPVarInt(issuerid, "LastHitedShotId", GetPVarInt(issuerid, "CurrentShotId"));
			
			if(playerArenaInfo[loc_issuer][issuerid][firedShots] < playerArenaInfo[loc_issuer][issuerid][hitedShots])
				playerArenaInfo[loc_issuer][issuerid][firedShots] = playerArenaInfo[loc_issuer][issuerid][hitedShots];
		}
	}

	if(loc_issuer_ok)
		playerArenaInfo[loc_issuer][issuerid][arenaDMG] += amount;

	Ach_OnPlayerDamage(issuerid, playerid, amount);

	User[issuerid][dmgSession] += amount;
	User[issuerid][dmgSemanal] += floatround(amount, floatround_round);

	new clanid = User[issuerid][clanID];
	if (clanid != -1 && clanid != User[playerid][clanID] && loc_issuer_ok)
	{
		ClanInfo[clanid][clanArenaDMG][loc_issuer] += floatround(amount, floatround_round);
	}

	if(objecthporarmour[playerid] != INVALID_OBJECT_ID)
	{
		DestroyObject(objecthporarmour[playerid]), KillTimer(hpandarmourtimer[playerid]);
		objecthporarmour[playerid] = INVALID_OBJECT_ID;
	}

	new Float:arm;
	GetPlayerArmour(playerid, arm);

	if(arm > 1)
	{
		objecthporarmour[playerid] = CreateObject(1242, 0, 0, 0, 0, 0, 0);
		AttachObjectToPlayer(objecthporarmour[playerid], playerid, 0, 0, 1.60, 0, 0, 0);
	}
	else
	{
		objecthporarmour[playerid] = CreateObject(1240, 0, 0, 0, 0, 0, 0);
		AttachObjectToPlayer(objecthporarmour[playerid], playerid, 0, 0, 1.60, 0, 0, 0);
	}

	hpandarmourtimer[playerid] = SetTimerEx("HideWeaponObject", 1500, false, "i", playerid);

	// hitmark
	new
		Float:fOriginX, Float:fOriginY, Float:fOriginZ,
		Float:fHitPosX, Float:fHitPosY, Float:fHitPosZ,
		PlayerText3D:hitbar, string[51];

    switch(User[issuerid][dmgInformerStyle])
    {
        case 2: format(string, sizeof(string), "*");
        case 3: format(string, sizeof(string), "X");
        default: format(string, sizeof(string), "%.0f", amount);
    }

	GetPlayerLastShotVectors(issuerid, fOriginX, fOriginY, fOriginZ, fHitPosX, fHitPosY, fHitPosZ);

	if(User[issuerid][dmgInformerStyle] == 0)
	{
		hitbar = CreatePlayer3DTextLabel(issuerid, string, User[issuerid][hitmarkColor], fHitPosX, fHitPosY, fHitPosZ, 70);
		User[issuerid][hitmarkTimer] = SetTimerEx("updateDynamicHitmark", 30, false, "dfdffff", issuerid, amount, _:hitbar, fHitPosX, fHitPosY, fHitPosZ, 0.03);
	}
	else
	{
		hitbar = CreatePlayer3DTextLabel(issuerid, string, User[issuerid][hitmarkColor],fHitPosX, fHitPosY, fHitPosZ, 70);
		SetTimerEx("destroyHidBar", 2000, false, "dd", issuerid, _:hitbar);
	}

	foreach (new specid : Player)
	{
		if (playerSpecID[specid] != issuerid) continue;

		hitbar = CreatePlayer3DTextLabel(specid, string, User[specid][hitmarkColor], fHitPosX, fHitPosY, fHitPosZ, 70);

		if(User[issuerid][dmgInformerStyle] == 0)
			SetTimerEx("updateDynamicHitmark", 30, false, "dfdffff", specid, amount, _:hitbar, fHitPosX, fHitPosY, fHitPosZ, 0.03);
		else
			SetTimerEx("destroyHidBar", 2000, false, "dd", specid, _:hitbar);
	}

	return true;
}

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(IsPlayerLogged(playerid) && !IsPlayerAFK(playerid))
	{
		new loc = User[playerid][locationId];
		new bool:pInDuel = (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE && GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_WAITING);
		new bool:pInDuel2v2 = (IsPlayerInDuel2v2(playerid) != 0);

		if(0 <= loc < MAX_LOCATIONS && loc != ARENA_AFK && !pInDuel && !pInDuel2v2 && !g_PlayerTraining[playerid][pTrain_Active] && !g_Practice[playerid][pracActive])
		{
			if(weaponid == WEAPON_DEAGLE || weaponid == WEAPON_SHOTGUN || weaponid == WEAPON_M4 || weaponid == WEAPON_SNIPER)
			{
				playerArenaInfo[loc][playerid][firedShots]++;
				SetPVarInt(playerid, "CurrentShotId", GetPVarInt(playerid, "CurrentShotId") + 1);
				
				// Logic safety: hits cannot exceed shots
				if(playerArenaInfo[loc][playerid][hitedShots] > playerArenaInfo[loc][playerid][firedShots])
					playerArenaInfo[loc][playerid][hitedShots] = playerArenaInfo[loc][playerid][firedShots];
			}
		}
		return true;
	}

	return false;
}

public OnPlayerText(playerid, text[])
{
	if(!IsPlayerLogged(playerid))
	{
		SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
		return false;
	}

	if(IsPlayerMuted(playerid))
	{
		va_SendClientMessage(playerid, -1, IS_MUTED);
		return false;
	}

	if(ServerAntiCapslock) StripCapslock(text);

	if(votingInfo[isRunningVote] && !playerVoteInfo[playerid][alreadyVoted] && strlen(text) == 1)
	{
		if(text[0] == 'y' || text[0] == 'Y')
			return RegisterVote(playerid, true), false;

		if(text[0] == 'n' || text[0] == 'N')
			return RegisterVote(playerid, false), false;
	}

	if(!AntiSpam(playerid, "flood_chat", "flood_chat_count"))
	{
		return false;
	}
	else {
		SetPVarInt(playerid, "flood_chat", gettime()+1);
		DeletePVar(playerid, "flood_chat_count");
	}

	if(text[0] == '@' && User[playerid][adminLevel] > 0)
	{
		foreach(new i : Player)
			if(User[i][adminLevel] > 0 && IsPlayerLogged(i))
				va_SendClientMessage(i, -1, ADMIN_CHAT_MODEL, adminnames[User[playerid][adminLevel]][ingame], (GetPlayerColor(playerid) >>> 8), User[playerid][userName], playerid, strtodown(text[1]));


		format(msgdiscord, sizeof(msgdiscord), "> [%s] %s[%d]: %s", adminnames[User[playerid][adminLevel]][discord], User[playerid][userName], playerid, strtodown(text[1]));
        utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
        DCC_SendChannelMessage(adminchat , msgencoded);
        
        LogChatMessage(User[playerid][user_sqlid], User[playerid][userName], "admin", text[1]);
		return false;
	}

	if(text[0] == '!')
    {
		if(IsPlayerInClan(playerid))
		{
			new clanid = User[playerid][clanID];
			new roleid = User[playerid][clanRole];
			new string[256];
        	format(string, sizeof(string), "{B4B5B7}| CHAT %s | {%06x}%s {CCCCCC}[{90C3D4}%s{CCCCCC}]: %s", ClanInfo[clanid][clanTag], GetPlayerColor(playerid) >>> 8, getPlayerName(playerid), ClanRoles[clanid][roleid], text[1]);
        	SendMessageToClan(clanid, -1, string);

			foreach(new i : Player)
				if(User[i][adminLevel] > 2 && IsPlayerLogged(i) && User[i][acfg_clanpm])
					va_SendClientMessage(i, -1, ADMIN_SEE_CLANPM, ClanInfo[clanid][clanTag], User[playerid][userName], playerid, ClanRoles[clanid][roleid], text[1]);

            LogChatMessage(User[playerid][user_sqlid], User[playerid][userName], "clan", text[1]);
            LogDiscordClanChat(playerid, ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName], ClanRoles[clanid][roleid], text[1]);
			return false;
		}
	}

	if(text[0] == ';')
	{
		// local buffer: strtodown also returns array, and it is not worth risking
		new localName[MAX_PLAYER_NAME + 1];
		GetPlayerName(playerid, localName, sizeof(localName));

		va_SendClientMessage(playerid, -1, LOCAL_CHAT_MODEL, localName, playerid, strtodown(text[1]));
		PlayerPlaySound(playerid, 1139, 0.0, 0.0, 0.0);

		new Float:pp[3], closePlayers;
		GetPlayerPos(playerid, pp[0], pp[1], pp[2]);

		foreach(new i : Player)
		{
			if(IsPlayerInRangeOfPoint(i, 20, pp[0], pp[1], pp[2]) && i != playerid)
			{
				va_SendClientMessage(i, -1, LOCAL_CHAT_MODEL, localName, playerid, strtodown(text[1]));
				PlayerPlaySound(i, 1139, 0.0, 0.0, 0.0);
				closePlayers++;
			}
		}
		if(closePlayers == 0)
			SendClientMessage(playerid, 0x71797eff, "{MAIN}[ERROR] {FFFFFF}There is no one near you, use general chat, fucking schizophrenic!");

		foreach(new i : Player)
		{
			if(User[i][adminLevel] > 2 && IsPlayerLogged(i) && User[i][acfg_local])
				va_SendClientMessage(i, -1, ADMIN_SEE_LOCAL, User[playerid][userName], playerid, strtodown(text[1]));
		}

        LogChatMessage(User[playerid][user_sqlid], User[playerid][userName], "local", text[1]);
		return false;
	}

    LogChatMessage(User[playerid][user_sqlid], User[playerid][userName], "global", text);

	format(msgdiscord, sizeof(msgdiscord), "> %s[%d]: %s", User[playerid][userName], playerid, text);
	utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));
	DCC_SendChannelMessage(gamechat , msgencoded);

	new fullTagStr[128] = "";
	if(IsAdmin(playerid) && !User[playerid][hideAdminTag] && !IsAdminCamouflaged(playerid))
	{
		format(fullTagStr, sizeof(fullTagStr), "{FFFFFF}[{FF4500}%s{FFFFFF}] ", adminnames[User[playerid][adminLevel]][ingame]);
	}

	if(!User[playerid][hideCustomTag] && User[playerid][customTag][0] != '\0')
	{
		format(fullTagStr, sizeof(fullTagStr), "%s{FF4500}%s ", fullTagStr, User[playerid][customTag]);
	}

	new vipTagStr[32] = "";
	GetVipTagString(playerid, vipTagStr, sizeof(vipTagStr));
	if(vipTagStr[0] != '\0')
	{
		format(fullTagStr, sizeof(fullTagStr), "%s%s", fullTagStr, vipTagStr);
	}

	new chatName[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, chatName, sizeof(chatName));

	new idHex[7], textHex[7];
	GetPlayerChatIDColorHex(playerid, idHex);
	GetPlayerChatTextColorHex(playerid, textHex);

	if(IsPlayerInClan(playerid) && !User[playerid][hideClanTag])
	{
		new topString[50];
		topString[0] = EOS;

		for(new i = 0; i < 5; i++)
		{
			if(ClanInfo[User[playerid][clanID]][clanID] == ClanRank[i][rank_clanID])
			{
				format(topString, sizeof(topString), "{%s}#%d ", i < 3 ? clanTop3Color[i] : "ffffff", i + 1);
				break;
			}
		}

		va_SendClientMessageToAll(-1, GLOBAL_CHAT_MODEL_CLAN,
			topString,
			fullTagStr,
			(ClanInfo[User[playerid][clanID]][clanColor] >>> 8),
			ClanInfo[User[playerid][clanID]][clanName],
			(GetPlayerColor(playerid) >>> 8),
			chatName,
			idHex,
			playerid,
			textHex,
			strtodown(text)
		);

		return false;
	}

	va_SendClientMessageToAll(-1, GLOBAL_CHAT_MODEL,
		fullTagStr,
		(GetPlayerColor(playerid) >>> 8),
		chatName,
		idHex,
		playerid,
		textHex,
		strtodown(text)
	);
	//SetPlayerChatBubble(playerid, text, 0xFFFFFFFF, 70.0, 4000);
	return false;
}

public OnPlayerUpdate(playerid)
{
	if (IsPlayerNPC(playerid)) return true;

	new Float:health, Float:armour, fullHealth;
	GetPlayerHealth(playerid, health);
	GetPlayerArmour(playerid, armour);
	fullHealth = floatround(health+armour);

	if(IsPlayerAFK(playerid) && fullHealth != 200)
	{
		SetPlayerHealth(playerid, 100.0);
		SetPlayerArmour(playerid, 100.0);
	}

	new PLAYER_STATE:playerState = GetPlayerState(playerid);

	if(playerState == PLAYER_STATE_DRIVER)
	{
		new strv[128], tagOne[20], tagTwo[20];
		new vehicleid = GetPlayerVehicleID(playerid);
		new Float:vhealth;
		GetVehicleHealth(vehicleid, vhealth);

		tagOne = GetPlayerHudTag(playerid, false);
		tagTwo = GetPlayerHudTag(playerid, true);

		format(strv, sizeof(strv), "%s%s~w~_%0.1f~n~%s%02d ~w~km/h",
			tagOne, getVehicleName(vehicleid), vhealth,
			tagTwo, GetVehicleSpeed(vehicleid));

		PlayerTextDrawSetString(playerid, speedometer[playerid], strv);
	}

	if(User[playerid][isAlreadySpawned])
		User[playerid][lastTimeUpdated] = gettime();

	// MONEY AC
	if(GetPlayerMoney(playerid) != User[playerid][playerMoney])
		LoadPlayerMoney(playerid);

	// WEAPON ANTI-CHEAT ALERT (OUTSIDE LOUNGE)
	if(User[playerid][isAlreadySpawned] && !IsPlayerInLounge(playerid))
	{
		if(!MsgCheat[playerid])
		{
			new weaponName[64] = "";
			switch(GetPlayerWeapon(playerid))
			{
				case 9:     weaponName = "a chainsaw";
				case 35,36: weaponName = "a rocket launcher";
				case 37:    weaponName = "a flamethrower";
				case 38:    weaponName = "a minigun";
				case 39:    weaponName = "remote explosives";
				case 40:    weaponName = "a detonator";
				case 43:    weaponName = "a camera";
				case 44,45: weaponName = "night/thermal vision goggles";
			}

			if(weaponName[0] != '\0')
			{
				MsgCheat[playerid] = true;

				ACSuspect[playerid][ac_weaponFlags]++;
				ACSuspect[playerid][ac_lastTime] = gettime();
				format(ACSuspect[playerid][ac_lastWeapon], AC_WEAPON_LEN, "%s", weaponName);
				new alertMsg[192];
				format(alertMsg, sizeof(alertMsg), "{MAIN}[ANTI-CHEAT] {FFFFFF}Player %s [ID: %d] is using %s. Use {MAIN}/spec {FFFFFF}to check.", User[playerid][userName], playerid, weaponName);

				foreach(new i : Player)
				{
					if(User[i][adminLevel] >= 2 && IsPlayerLogged(i))
					{
						SendClientMessage(i, -1, "%s", alertMsg);
					}
				}
				SetTimerEx("EnviarMsgCheat", 60000, false, "i", playerid);
			}
		}
	}

	return true;
}

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
	if(!IsPlayerLogged(playerid))
	{
		SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must log in before executing commands.");
		return COMMAND_DENIED;
	}

	if(!AntiSpam(playerid, "flood_cmd", "flood_cmd_count"))
	{
		return COMMAND_DENIED;
	}
	else {
		SetPVarInt(playerid, "flood_cmd", gettime()+1);
		DeletePVar(playerid, "flood_cmd_count");
	}

	if(User[playerid][adminLevel] < 1 && GetTickCount() < playerCmdCooldown[playerid])
	{
		SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Please wait before using another command.");
		return COMMAND_DENIED;
	}

	new spyText[144];
	RedactCommandText(spyText, cmdtext, sizeof(spyText));

	foreach(new i : Player)
			if(User[i][adminLevel] > 2 && IsPlayerLogged(i) && User[i][acfg_cmds])
				va_SendClientMessage(i, -1, ADMIN_SEE_CMDS, User[playerid][userName], playerid, spyText);

	if(!WC_IsPlayerSpawned(playerid) && !IsPlayerSpectating(playerid))
	{
		SendClientMessage(playerid, -1, "%s", NOT_SPAWNED_MSG);
		return COMMAND_DENIED;
	}

	if(success == COMMAND_UNDEFINED)
	{
		return SendClientMessage(playerid, -1, "%s", UNKNOWN_COMMAND_MSG), COMMAND_DENIED;
	}

    return COMMAND_OK;
}

public e_COMMAND_ERRORS:OnPlayerCommandPerformed(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
	if (!IsAdmin(playerid, 1))
		playerCmdCooldown[playerid] = GetTickCount() + 500;
	else if(success == COMMAND_OK && logs_admin_cmds != DCC_INVALID_CHANNEL)
	{
		new logText[144];
		RedactCommandText(logText, cmdtext, sizeof(logText));

		format(msgdiscord, sizeof(msgdiscord), "**Admin:** %s (Level %d)\n**Command:** %s",
			User[playerid][userName], User[playerid][adminLevel], logText);
		utf8encode(msgencoded, msgdiscord, sizeof(msgencoded));

		SendDiscordEmbed(logs_admin_cmds, "Admin Command", msgencoded, DISCORD_COLOR_ADMIN);
	}

    return COMMAND_OK;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(!IsPlayerLogged(playerid)) return 1;

	if(PRESSED(KEY_SECONDARY_ATTACK) || PRESSED(KEY_JUMP) || PRESSED(KEY_SUBMISSION))
	{
		if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
		{
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		}
	}

	if(PRESSED(KEY_NO))
    {
        if(g_PlayerTraining[playerid][pTrain_Active])
        {
            RestartTrainingChallenge(playerid);
        }
        else
        {
            new cmd[10] = "/help";
            Command_ReProcess(playerid, cmd, 0);
        }
    }

	if(PRESSED(KEY_FIRE) && GetPVarInt(playerid, "isShowingTableTD") != 0)
	{
		HideRoundStatsTD(playerid);

		DeletePVar(playerid, "isShowingTableTD");
	}

	new Float:health, Float:armour, fullHealth;
	GetPlayerHealth(playerid, health);
	GetPlayerArmour(playerid, armour);
	fullHealth = floatround(health+armour);

	if(PRESSED(KEY_CTRL_BACK))
	{
		new timestampRespawn = gettime();
		if(fullHealth == 200 && WC_IsPlayerSpawned(playerid))
			Respawn(playerid, timestampRespawn);
		else
			return SendClientMessage(playerid, -1, "%s", CANNOT_RESPAWN);
	}

	if(newkeys == KEY:160 && (GetPlayerWeapon(playerid) == WEAPON_FIST || GetPlayerWeapon(playerid) == WEAPON_BRASSKNUCKLE) && !IsPlayerInAnyVehicle(playerid) && !(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING))
	{
		SyncPlayer(playerid);
		return true;
	}
	return true;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	if (success)
	{
		new
			playerid = INVALID_PLAYER_ID,
			playerIp[16];

		foreach (new i : Player)
		{
			GetPlayerIp(i, playerIp);

			if (strequal(ip, playerIp))
			{
				playerid = i;
				break;
			}
		}

		if (playerid != INVALID_PLAYER_ID)
		{
			static const ALLOWED_RCONS[][] = {SERVER_OWNER};

			for (new i; i < sizeof(ALLOWED_RCONS); i ++)
			{
				if (strequal(User[playerid][userName], ALLOWED_RCONS[i]))
				{
					va_SendClientMessage(playerid, COLOR_ABACATE, "{MAIN}[RCON] {FFFFFF}Login successful. Welcome {MAIN}%s{FFFFFF}!", getPlayerName(playerid));
					return true;
				}
			}

			SendClientMessage(playerid, COLOR_IRED, "{MAIN}[ERROR] {FFFFFF}Access denied.");
			KickEx(playerid, 300);
		}
	}
	return true;
}

static stock bool:IsSuspiciousRejectedHit(reason)
{
	switch(reason)
	{
		case HIT_INVALID_WEAPON,
			 HIT_MULTIPLE_PLAYERS,
			 HIT_INVALID_HITTYPE,
			 SHOOTING_RATE_TOO_FAST,
			 SHOOTING_RATE_TOO_FAST_MULTIPLE,
			 HIT_RATE_TOO_FAST,
			 HIT_RATE_TOO_FAST_MULTIPLE,
			 HIT_INVALID_DAMAGE:
		{
			return true;
		}
	}
	return false;
}

public OnRejectedHit(playerid, hit[E_REJECTED_HIT])
{
	if(playerid < 0 || playerid >= MAX_PLAYERS)
		return true;

	new reason = hit[e_Reason];

	if(reason < 0 || reason >= sizeof(g_HitRejectReasons))
		return true;

	if(!IsSuspiciousRejectedHit(reason))
		return true;

	new WEAPON:weapon = hit[e_Weapon];
	new detail[160], output[256], extra[48];

	format(detail, sizeof(detail), g_HitRejectReasons[reason], hit[e_Info1], hit[e_Info2], hit[e_Info3]);
	format(output, sizeof(output), "(%s -> %s) %s", ReturnWeaponName(weapon), hit[e_Name], detail);

	ACSuspect[playerid][ac_rejectedHits]++;
	ACSuspect[playerid][ac_lastTime] = gettime();
	format(ACSuspect[playerid][ac_lastReason], AC_REASON_LEN, "%s", detail);

	if(gettime() < rejectedHitCooldown[playerid])
	{
		rejectedHitSkipped[playerid]++;
		return true;
	}

	extra[0] = EOS;
	if(rejectedHitSkipped[playerid] > 0)
	{
		format(extra, sizeof(extra), " {BEBEBE}(+%d in %ds)", rejectedHitSkipped[playerid], REJECTED_HIT_COOLDOWN);
		rejectedHitSkipped[playerid] = 0;
	}

	foreach(new i : Player)
	{
		if(i == playerid) continue;
		if(!IsPlayerLogged(i) || User[i][adminLevel] < 2) continue;

		va_SendClientMessage(i, 0xcc0000ff, "{MAIN}[ANTI-CHEAT] {FFFFFF}%s [%d] rejected hit: %s%s",
			User[playerid][userName], playerid, output, extra);
	}

	rejectedHitCooldown[playerid] = gettime() + REJECTED_HIT_COOLDOWN;
	return true;
}

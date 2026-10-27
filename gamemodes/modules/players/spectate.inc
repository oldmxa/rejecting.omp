
#define SPEC_MAX_WATCHERS	(15)
#define SPEC_WATCHER_STR	(16 + (SPEC_MAX_WATCHERS * 30))

bool:IsValidSpecTarget(playerid, targetid)
{
	if(targetid == playerid || targetid == INVALID_PLAYER_ID) return false;
	if(!IsPlayerLogged(targetid)) return false;
	if(IsPlayerSpectating(targetid)) return false;
	if(IsPlayerAFK(targetid) && User[playerid][adminLevel] < 1) return false;

	return true;
}

initPlayerSpectate(playerid)
{
	Iter_Clear(specWatchers[playerid]);
	playerSpecID[playerid] = INVALID_PLAYER_ID;
	User[playerid][isSpectating] = false;
}

refreshWatcherList(targetid)
{
	if(targetid == INVALID_PLAYER_ID) return;

	new list[SPEC_WATCHER_STR], visible;
	format(list, sizeof(list), "~y~~h~Watching~n~");

	foreach(new i : specWatchers[targetid])
	{
		if(User[i][adminLevel] > 0) continue;

		format(list, sizeof(list), "%s~w~%s~n~", list, getPlayerName(i));
		visible++;
	}

	foreach(new i : specWatchers[targetid])
	{
		if(visible < 2)
		{
			PlayerTextDrawHide(i, specTextDraw[i][7]);
			continue;
		}

		PlayerTextDrawSetString(i, specTextDraw[i][7], list);
		PlayerTextDrawShow(i, specTextDraw[i][7]);
	}
}

forgetSpecWatcher(playerid)
{
	new old = playerSpecID[playerid];

	if(old == INVALID_PLAYER_ID) return;

	if(Iter_Contains(specWatchers[old], playerid))
		Iter_Remove(specWatchers[old], playerid);

	refreshWatcherList(old);
}

attachSpecCamera(playerid, targetid)
{
	new PLAYER_STATE:targetState = GetPlayerState(targetid);

	if(targetState == PLAYER_STATE_DRIVER || targetState == PLAYER_STATE_PASSENGER)
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(targetid), SPECTATE_MODE_NORMAL);
	else
		PlayerSpectatePlayer(playerid, targetid, SPECTATE_MODE_NORMAL);
}
findSpecTarget(playerid, exclude = INVALID_PLAYER_ID, bool:ascending = true)
{
	new id = playerSpecID[playerid];

	if(id == INVALID_PLAYER_ID) id = 0;

	for(new n; n < MAX_PLAYERS; n++)
	{
		id = ascending ? (id + 1) : (id - 1);

		if(id >= MAX_PLAYERS) id = 0;
		else if(id < 0) id = MAX_PLAYERS - 1;

		if(id == exclude) continue;
		if(!IsValidSpecTarget(playerid, id)) continue;

		return id;
	}

	return INVALID_PLAYER_ID;
}

StopSpectate(playerid)
{
	forgetSpecWatcher(playerid);

	hideSpecTD(playerid);
	playerSpecID[playerid] = INVALID_PLAYER_ID;
	User[playerid][isSpectating] = false;
	User[playerid][isAfk] = IsPlayerInLounge(playerid);
	TogglePlayerSpectating(playerid, false);

	if(User[playerid][showPlayerHud])
		showPlayerTextDraw(playerid);
}

setPlayerSpectate(playerid, playertargetid)
{
	if(playerid == INVALID_PLAYER_ID || playertargetid == INVALID_PLAYER_ID)
		return false;

	if(playerid == playertargetid || !IsPlayerConnected(playertargetid))
		return false;

	if(playerSpecID[playerid] != playertargetid && Iter_Count(specWatchers[playertargetid]) >= SPEC_MAX_WATCHERS)
	{
		SendClientMessage(playerid, -1, "%s", SPEC_TOO_MANY);
		return false;
	}

	forgetSpecWatcher(playerid);

	playerSpecID[playerid] = playertargetid;
	User[playerid][isSpectating] = true;
	User[playerid][isAfk] = false;

	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(playertargetid));
	SetPlayerInterior(playerid, GetPlayerInterior(playertargetid));

	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
	{
		hidePlayerTextdraw(playerid);
		showSpecTD(playerid);
		TogglePlayerSpectating(playerid, true);
	}

	attachSpecCamera(playerid, playertargetid);

	new stringa[30];
	format(stringa, sizeof (stringa), "%s [%d]", getPlayerName(playertargetid), playertargetid);
	PlayerTextDrawSetString(playerid, specTextDraw[playerid][1], stringa);

	Iter_Add(specWatchers[playertargetid], playerid);
	refreshWatcherList(playertargetid);

	updateSpecInfo(playerid);
	return true;
}

specCycle(playerid, bool:ascending)
{
	if(!IsPlayerSpectating(playerid)) return false;

	new next = findSpecTarget(playerid, INVALID_PLAYER_ID, ascending);

	if(next == INVALID_PLAYER_ID)
	{
		SendClientMessage(playerid, -1, "%s", SPEC_NO_TARGET);
		return false;
	}

	return setPlayerSpectate(playerid, next);
}

handleSpecTargetLeft(targetid)
{
	new watchers[MAX_PLAYERS], count;

	foreach(new i : specWatchers[targetid])
		watchers[count++] = i;

	Iter_Clear(specWatchers[targetid]);

	for(new i; i < count; i++)
	{
		new next = findSpecTarget(watchers[i], targetid, true);

		if(next == INVALID_PLAYER_ID)
		{
			StopSpectate(watchers[i]);
			SendClientMessage(watchers[i], -1, "%s", SPEC_TARGET_LEFT);
			continue;
		}

		setPlayerSpectate(watchers[i], next);
	}
}
hook OnPlayerDamageDone(playerid, Float:amount, issuerid, WEAPON:weapon, bodypart)
{
	new Float:hp;
	GetPlayerHealth(playerid, hp);

	new bool:fatal = (hp <= 0.0005);

	if(issuerid != INVALID_PLAYER_ID)
	{
		foreach(new i : specWatchers[issuerid])
			WC_ShowDamageFeedGiven(i, playerid, amount, weapon, fatal);
	}

	foreach(new i : specWatchers[playerid])
		WC_ShowDamageFeedTaken(i, issuerid, amount, weapon, fatal);

	return 1;
}

// publics

forward reSpectate(playerid, specid);
public reSpectate(playerid, specid)
{
	if(playerSpecID[playerid] == INVALID_PLAYER_ID || playerid == INVALID_PLAYER_ID || specid == INVALID_PLAYER_ID)
		return false;

	setPlayerSpectate(playerid, specid);
	return true;
}

showSpecTD(playerid)
{
	for(new i; i < 7; i++)
		PlayerTextDrawShow(playerid, specTextDraw[playerid][i]);
}

hideSpecTD(playerid)
{
	for(new i; i < 8; i++)
		PlayerTextDrawHide(playerid, specTextDraw[playerid][i]);
}

stock updateSpecInfo(playerid)
{
	new specBy = playerSpecID[playerid];

	if(specBy == INVALID_PLAYER_ID) return 1;

	if(!IsPlayerLogged(specBy))
	{
		StopSpectate(playerid);
		return 1;
	}

	new targetWorld = GetPlayerVirtualWorld(specBy);
	new targetInterior = GetPlayerInterior(specBy);
	if(GetPlayerVirtualWorld(playerid) != targetWorld)
		SetPlayerVirtualWorld(playerid, targetWorld);
	if(GetPlayerInterior(playerid) != targetInterior)
		SetPlayerInterior(playerid, targetInterior);

	new
		stringg[512],
		Float:playerHealth,
		Float:playerArmour,
		HP,
		ping,
		fps,
		Float:packetLoss,
		sampVERSION[24],
		weapon_idd[13],
		weapon_ammoo[13];

	GetPlayerHealth(specBy, playerHealth);
	GetPlayerArmour(specBy, playerArmour);
	HP = floatround(playerHealth+playerArmour);
	ping = GetPlayerPing(specBy);
	fps = User[specBy][FPS];
	packetLoss = NetStats_PacketLossPercent(specBy);
	GetPlayerVersion(specBy, sampVERSION, sizeof(sampVERSION));

	for(new i; i < 12; i++)
	{
		GetPlayerWeaponData(specBy, WEAPON_SLOT:i, WEAPON:weapon_idd[i], weapon_ammoo[i]);
	}

	format(stringg, sizeof (stringg), "ID: %d~n~\
		HP: (~r~~h~%s~w~)~n~\
		PING: %s~n~\
		FPS: %s~n~\
		PL: %s~n~\
		Version: ~w~%s~n~\
		Mobile: %s",
		specBy,
		(HP > 50) ? (sprintf("~w~%d", HP)) : (sprintf("~r~~h~%d", HP)),
		(ping < 400) ? (sprintf("~w~%d", ping)) : (sprintf("~r~~h~%d", ping)),
		(fps > 60) ? (sprintf("~w~%d", fps)) : (sprintf("~r~~h~%d", fps)),
		(packetLoss < 1.5) ? (sprintf("~w~%.1f%%", packetLoss)) : (sprintf("~r~~h~%.1f%%", packetLoss)),
		sampVERSION,
		(User[specBy][isMobile]) ? ("~g~~h~Yes") : ("~w~No"));

	PlayerTextDrawSetString(playerid, specTextDraw[playerid][2], stringg);

	stringg = "";
	new stringtemp[60], strLines;

	for(new i; i < 12; i++)
	{
		if(weapon_idd[i] == 0 || strLines >= 4) continue;
		format(stringtemp, sizeof(stringtemp), "~y~~h~%s ~w~(~r~~h~%d~w~)~n~", wName[weapon_idd[i]], weapon_ammoo[i]);
		strcat(stringg, stringtemp);
		strLines++;
	}

	PlayerTextDrawSetString(playerid, specTextDraw[playerid][4], stringg);

	new arenaStats[220];
	new loc = User[specBy][locationId];

	if(loc != -1 && loc != ARENA_AFK)
	{
		format(arenaStats, sizeof(arenaStats), "Kills: ~g~~h~%d~n~\
			Deaths: ~r~~h~%d~n~\
			Ratio: ~w~%.2f~n~\
			Damage: ~w~%.0f~n~\
			Accuracy: ~w~%.1f%%",
			playerArenaInfo[loc][specBy][arenaKills],
			playerArenaInfo[loc][specBy][arenaDeaths],
			ratio(playerArenaInfo[loc][specBy][arenaKills], playerArenaInfo[loc][specBy][arenaDeaths]),
			playerArenaInfo[loc][specBy][arenaDMG],
			GetPlayerAccuracy(specBy, loc));
	}
	else
	{
		format(arenaStats, sizeof(arenaStats), "~w~In the Lounge");
	}

	PlayerTextDrawSetString(playerid, specTextDraw[playerid][6], arenaStats);
	return 1;
}

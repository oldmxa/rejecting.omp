
StopSpectate(playerid)
{
	hideSpecTD(playerid);
	playerSpecID[playerid] = INVALID_PLAYER_ID;
	User[playerid][isSpectating] = false;
	if(!IsPlayerInLounge(playerid))
	{
		User[playerid][isAfk] = false;
	}
	else
	{
		User[playerid][isAfk] = true;
	}
	TogglePlayerSpectating(playerid, false);
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

setPlayerSpectate(playerid, playertargetid)
{
	if(playerid != INVALID_PLAYER_ID && playertargetid != INVALID_PLAYER_ID)
	{
		playerSpecID[playerid] = playertargetid;
		SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(playertargetid));
		SetPlayerInterior(playerid, GetPlayerInterior(playertargetid));
		User[playerid][isSpectating] = true;
		User[playerid][isAfk] = false;

		hidePlayerTextdraw(playerid);
		showSpecTD(playerid);

		new stringa[30];
		format(stringa, sizeof (stringa), "%s [%d]", getPlayerName(playertargetid), playertargetid);
		PlayerTextDrawSetString(playerid, specTextDraw[playerid][1], stringa);
		TogglePlayerSpectating(playerid, true);
		PlayerSpectatePlayer(playerid, playertargetid, SPECTATE_MODE_NORMAL);
		updateSpecInfo(playerid);
	}
}

showSpecTD(playerid)
{
	for(new i; i < 7; i++)
		PlayerTextDrawShow(playerid, specTextDraw[playerid][i]);
}

hideSpecTD(playerid)
{
	for(new i; i < 7; i++)
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
		sampVERSION[24],
		weapon_idd[13],
		weapon_ammoo[13];

	GetPlayerHealth(specBy, playerHealth);
	GetPlayerArmour(specBy, playerArmour);
	HP = floatround(playerHealth+playerArmour);
	ping = GetPlayerPing(specBy);
	fps = User[specBy][FPS];
	GetPlayerVersion(specBy, sampVERSION, sizeof(sampVERSION));

	for(new i; i < 12; i++)
	{
		GetPlayerWeaponData(specBy, WEAPON_SLOT:i, WEAPON:weapon_idd[i], weapon_ammoo[i]);
	}

	format(stringg, sizeof (stringg), "ID: %d~n~\
		HP: (~r~~h~%s~w~)~n~\
		PING: %s~n~\
		FPS: %s~n~\
		Version: ~w~%s~n~\
		Mobile: %s",
		specBy,
		(HP > 50) ? (sprintf("~w~%d", HP)) : (sprintf("~r~~h~%d", HP)),
		(ping < 400) ? (sprintf("~w~%d", ping)) : (sprintf("~r~~h~%d", ping)),
		(fps > 60) ? (sprintf("~w~%d", fps)) : (sprintf("~r~~h~%d", fps)),
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

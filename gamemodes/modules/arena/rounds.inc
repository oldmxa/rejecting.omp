
stock GetLocationDisplaySlot(location)
{
	for (new slot; slot < sizeof(LOCATION_DISPLAY_ORDER); slot ++)
		if (LOCATION_DISPLAY_ORDER[slot] == location) return slot;

	return location;
}

showLocationSelectDialog(playerid)
{
	new dialogBody[1500] = "ID\tArena Mode\tCurrent Arena\tPlayers\n";

	for (new slot; slot < sizeof(LOCATION_DISPLAY_ORDER); slot ++)
	{
        new i = LOCATION_DISPLAY_ORDER[slot];

        new mapName[48], slots[32], pCount = getLocationPlayers(i), mCount = arenaInfo[i][maxPlayers];
        new zoneid = arenaInfo[i][arenaId];

        if(i == ARENA_AFK)
            format(mapName, sizeof(mapName), "{BEBEBE}-");
        else if(!arenaInfo[i][isRunningArena] || !(0 <= zoneid < MAX_ZONES))
            format(mapName, sizeof(mapName), "{BEBEBE}Waiting round");
        else
            format(mapName, sizeof(mapName), "%s%s",
                (User[playerid][locationId] == i) ? ("{23C552}") : ("{FFFFFF}"),
                arenaData[zoneid][arenaName]);

        format(slots, sizeof(slots), "{FFFFFF}%d %s\x95 {FFFFFF}%d",
            pCount, (pCount >= mCount) ? ("{F84F31}") : ("{23C552}"), mCount);

		strcat(dialogBody, fmat_return("{MAIN}#%02d\t%s\t%s\t%s\n",
            slot + 1, ROOM_NAME[i][E_ROOM_NAME_SERVER], mapName, slots), sizeof(dialogBody));
	}

    new dialogTitle[64] = "{MAIN}Changemode - Arena Selection";
    format_MainColor(playerid, dialogTitle);
    format_MainColor(playerid, dialogBody, sizeof(dialogBody));

	Dialog_ShowCallback(playerid, using public Location<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, dialogTitle, dialogBody, "Select", "Close");
}

stock IsCustomArena(zone)
{
    return (0 <= zone < MAX_ZONES && _:arenaData[zone][category] == ARENA_CATEGORY_CUSTOM);
}

stock GetCustomArenaByIndex(index)
{
    if (index < 0) return -1;

    new count = 0;

    for (new i; i < MAX_ZONES; i++)
    {
        if (!IsCustomArena(i)) continue;
        if (count == index) return i;

        count++;
    }

    return -1;
}

stock ApplyArenaChange(playerid, location, zone)
{
    foreach(new i : Player)
    {
        if(User[i][locationId] != location || GetPVarInt(i, "DuelS_State") != DUELS_STATE_NONE) continue;
        GangZoneStopPulseForPlayer(i, e_ZONE_PULSE_ZONE, arenaInfo[location][arenaId]);
    }

    GetRoundRank(location);

    new oldArenaId = arenaInfo[location][arenaId];
    arenaInfo[location][arenaId] = zone;
    PrepareChangeArena(location, oldArenaId);

    foreach(new i : Player)
    {
        if((User[i][locationId] == location && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE) || User[i][adminLevel] > 0)
            va_SendClientMessage(i, -1, CHANGED_ARENA_MSG, User[playerid][userName], arenaData[zone][arenaName], zone);
    }

    if(User[playerid][adminLevel] < 4)
        changeArenaDelay = gettime()+(3*60);

    return 1;
}

determineArenaID(location)
{
	new const LOCATION_PLAYERS = getLocationPlayers(location);
	new type, rand;

	if (location == ARENA_CUSTOM)
	{
		type = ARENA_CATEGORY_CUSTOM;
	}
	else if (LOCATION_PLAYERS >= HIGH_ARENA_PLAYERS_COUNT)
	{
		type = ARENA_CATEGORY_BIG;
	}
	else if (LOCATION_PLAYERS >= MEDIUM_ARENA_PLAYERS_COUNT)
	{
		type = ARENA_CATEGORY_MEDIUM;
	}
	else
	{
		type = ARENA_CATEGORY_SMALL;
	}

	if (Iter_IsEmpty(ArenaQueue<type>))
	{
		foreach (new i : ArenaCategory<type>)
		{
			if (arenaInfo[location][arenaId] != i)
			{
				Iter_Add(ArenaQueue<type>, i);
			}
		}
	}

	rand = Iter_Random(ArenaQueue<type>);
	Iter_Remove(ArenaQueue<type>, rand);

	return rand;
}

forward ChangeArena(location);
public ChangeArena(location)
{
	new iString[150], clanString[300];
	clanString[0] = EOS;

	format(iString, sizeof(iString), ARENA_START_MSG, arenaData[arenaInfo[location][arenaId]][arenaName], arenaInfo[location][arenaId]);

	inline const ZoneClanMessage()
	{
		new rows = cache_num_rows();
		if (rows)
		{
			new clanid, clanNameColor[100];
			cache_get_value_int(0, "clanid", clanid);
            
            if(ClanInfo[clanid][clanIDUsed])
                format(clanNameColor, sizeof(clanNameColor), "{%06x}[%s] %s{FFFFFF}", ClanInfo[clanid][clanColor] >>> 8, ClanInfo[clanid][clanTag], ClanInfo[clanid][clanName]);
            else
                format(clanNameColor, sizeof(clanNameColor), "None");

			format(clanString, sizeof(clanString), ARENA_CLAN_DOMINATED, arenaData[arenaInfo[location][arenaId]][arenaName], arenaInfo[location][arenaId], clanNameColor);

			foreach(new i : Player)
			{
				if(clanString[0] != EOS && User[i][locationId] == location)
				{
					SendClientMessage(i, -1, "%s", clanString);
				}
			}

		}
	}

	MySQL_TQueryInline(dbHandle, using inline ZoneClanMessage, "SELECT `clanid` FROM `zones` WHERE `zoneid` = %d", arenaInfo[location][arenaId]);

	foreach(new i : Player)
	{
		if (User[i][locationId] == location && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE)
		{
			if(playerArenaInfo[User[i][locationId]][i][arenaDMG] > User[i][maxDamage])
			{
				va_SendClientMessage(i, -1, UPDATE_DAMAGE, User[i][maxDamage], floatround(playerArenaInfo[User[i][locationId]][i][arenaDMG]));
				User[i][maxDamage] = floatround(playerArenaInfo[User[i][locationId]][i][arenaDMG]);
			}

			UpdatePlayerData(i);
			SendClientMessage(i, -1, "%s", iString);

			new zoneid = arenaInfo[location][arenaId];
			ZoneNumberFlashForPlayer(i, zoneid, 0xCC0000FF);

			DeletePVar(i, "Spree");
			User[i][spreeTime] = 0;

			HideRoundStatsTD(i);

			if(!User[i][isAfk])
				SpawnPlayerEx(i);
		}

		playerArenaInfo[location][i][arenaDMG] = 0;
		playerArenaInfo[location][i][arenaKills] = 0;
		playerArenaInfo[location][i][arenaExp] = 0;
		playerArenaInfo[location][i][arenaDeaths] = 0;
		playerArenaInfo[location][i][firedShots] = 0;
		playerArenaInfo[location][i][hitedShots] = 0;
		playerArenaInfo[location][i][arenaHeadshots] = 0;
	}

	arenaInfo[location][roundMints] = RoundTime;
	arenaInfo[location][isRunningArena] = true;
	return true;
}


forward initArena();
public initArena()
{
	for (new i; i < sizeof(arenaInfo); i++)
	{
		arenaInfo[i][maxPlayers] = MAX_PLAYERS_LOCATION;
		arenaInfo[i][roundMints] = RoundTime;
		arenaInfo[i][isRunningArena] = true;

		new poolCategory = (i == ARENA_CUSTOM) ? ARENA_CATEGORY_CUSTOM : ARENA_CATEGORY_SMALL;

		arenaInfo[i][arenaId] = Iter_Random(ArenaQueue<poolCategory>);
		Iter_Remove(ArenaQueue<poolCategory>, arenaInfo[i][arenaId]);
	}

	arenaInfo[ARENA_AFK][isRunningArena] = false;
	arenaInfo[ARENA_AFK][maxPlayers] = MAX_PLAYERS_AFK;
	return true;
}

getLocationPlayers(location)
{
	new playerCounter = 0;
	foreach(new i : Player)
	{
		if(User[i][isLogged] == false || User[i][locationId] != location) continue;
		playerCounter++;
	}
	return playerCounter;
}

// minimum distance between spawn and other players in the arena
#define SPAWN_MIN_DISTANCE (25.0)
// how many points to test before accepting the best one found
#define SPAWN_MAX_ATTEMPTS (15)

// Default raycast start for arenas on the real map; overridden per arena by
// arenaSpawnCeiling.
#define SPAWN_RAYCAST_CEILING (700.0)

// How far under the ceiling the arena floor can still be. Anything lower is
// something else entirely - sea floor, terrain under the map.
#define SPAWN_ARENA_DEPTH (250.0)

// Drop height used when no ground was found at all, so the player lands on the
// arena instead of being placed under it.
#define SPAWN_FALLBACK_DROP (30.0)

// synchronize with other players 500ms
#define SPAWN_PROTECT_TIME (500)

new SpawnProtectTimer[MAX_PLAYERS];

forward RestoreSpawnWorld(playerid);

static stock Float:GetSpawnClearance(playerid, Float:x, Float:y, Float:z)
{
    new Float:closest = 99999.0;

    foreach(new i : Player)
    {
        if(i == playerid) continue;
        if(!IsPlayerConnected(i) || !User[i][isLogged] || User[i][isAfk]) continue;
        if(User[i][locationId] != User[playerid][locationId]) continue;

        new Float:d = GetPlayerDistanceFromPoint(i, x, y, z);
        if(d < closest) closest = d;
    }
    return closest;
}

spawnOnLocation(playerid)
{
	if(User[playerid][locationId] == -1) return true;

	new zone = arenaInfo[User[playerid][locationId]][arenaId];
	new Float:posX, Float:posY, Float:posZ;
	new Float:bestX, Float:bestY, Float:bestZ, Float:bestClearance = -1.0;

	new customArena = IsCustomArena(zone);

	new Float:ceiling = arenaData[zone][arenaSpawnCeiling];
	if(ceiling <= 0.0) ceiling = SPAWN_RAYCAST_CEILING;

	new Float:floorLimit = ceiling - SPAWN_ARENA_DEPTH;

	for(new attempt = 0; attempt < SPAWN_MAX_ATTEMPTS; attempt++)
	{
		posX = RandomFloat(arenaData[zone][arenaPos][0], arenaData[zone][arenaPos][2]);
		posY = RandomFloat(arenaData[zone][arenaPos][1], arenaData[zone][arenaPos][3]);

		new modelid = CA_RayCastLine(posX, posY, ceiling, posX, posY, -1000.0, posX, posY, posZ);

		if(!modelid || modelid == WATER_OBJECT) continue;

		if(customArena)
		{
			if(posZ < floorLimit) continue;
		}
		else if(IsModelBuilding(modelid)) continue;

		new Float:clearance = GetSpawnClearance(playerid, posX, posY, posZ);

		if(clearance > bestClearance)
		{
			bestClearance = clearance;
			bestX = posX;
			bestY = posY;
			bestZ = posZ;
		}

		if(clearance >= SPAWN_MIN_DISTANCE) break;
	}

	if(bestClearance < 0.0)
	{
		bestX = (arenaData[zone][arenaPos][0] + arenaData[zone][arenaPos][2]) / 2.0;
		bestY = (arenaData[zone][arenaPos][1] + arenaData[zone][arenaPos][3]) / 2.0;

		new fallbackModel = CA_RayCastLine(bestX, bestY, ceiling, bestX, bestY, -1000.0, bestX, bestY, bestZ);

		if(!fallbackModel || fallbackModel == WATER_OBJECT || (customArena && bestZ < floorLimit))
			bestZ = ceiling - SPAWN_FALLBACK_DROP;
	}

	SetPlayerInterior(playerid, arenaData[zone][arenaInterior]);
	SetPlayerPos(playerid, bestX, bestY, bestZ + 1.0);
	SetPlayerFacingAngle(playerid, RandomFloat(0.0, 360.0));
	SetCameraBehindPlayer(playerid);

	DeletePVar(playerid, "gunmenu_world");

	if(GetPVarInt(playerid, "showGunmenuFirst") == 0)
	{
		SetPlayerVirtualWorld(playerid, SPAWN_VW + playerid);
		SetPVarInt(playerid, "spawn_protect", 1);

		KillTimer(SpawnProtectTimer[playerid]);
		SpawnProtectTimer[playerid] = SetTimerEx("RestoreSpawnWorld", SPAWN_PROTECT_TIME, false, "i", playerid);
	}
	return true;
}

public RestoreSpawnWorld(playerid)
{
	DeletePVar(playerid, "spawn_protect");

	if(!IsPlayerConnected(playerid) || !User[playerid][isLogged]) return 1;

	if(GetPVarInt(playerid, "gunmenu_world")) return 1;

	if(User[playerid][isAfk]) return 1;
	if(User[playerid][locationId] == -1 || User[playerid][locationId] == ARENA_AFK) return 1;

	SetPlayerVirtualWorld(playerid, User[playerid][locationId] * 99);
	return 1;
}

SendDeathMessageToRoom(killer, killed, weapon, roomid)
{
	foreach(new i : Player)
	{
		if(User[i][isLogged] && User[i][locationId] == roomid)
			SendDeathMessageToPlayer(i, killer, killed, weapon);
	}
}

PrepareChangeArena(location, finishingArenaId)
{
	new g_PlayerArray<playersTop>;

	sortPlayersInline playersTop => (R = l > r)
	{
		new lc = IsPlayerConnected(l) && !IsPlayerNPC(l) && User[l][locationId] == location && GetPVarInt(l, "DuelS_State") == DUELS_STATE_NONE;
		new rc = IsPlayerConnected(r) && !IsPlayerNPC(r) && User[r][locationId] == location && GetPVarInt(r, "DuelS_State") == DUELS_STATE_NONE;

		if (lc != rc)
		{
			R = lc > rc;
		}
		else
		{
			R = playerArenaInfo[location][l][arenaDMG] > playerArenaInfo[location][r][arenaDMG];
		}
	}

	if(IsPlayerConnected(playersTop[0]))
	{
		if(User[playersTop[0]][locationId] == location)
		{
			User[playersTop[0]][wonArenas]++;
			Ach_OnArenaMVP(playersTop[0]);
			foreach(new i : Player)
			{
				if(User[i][locationId] == location)
				{
					va_SendClientMessage(i, -1, "{MAIN}[MVP]{FFFFFF} %s won this round!", getPlayerName(playersTop[0]));
				}
			}

            new log_winner[MAX_PLAYER_NAME + 1], log_clan[MAX_CLAN_NAME] = "None", log_arena[64];
            strcopy(log_winner, User[playersTop[0]][userName], sizeof(log_winner));

            if(User[playersTop[0]][clanID] != -1 && ClanInfo[User[playersTop[0]][clanID]][clanIDUsed]) {
                strcopy(log_clan, ClanInfo[User[playersTop[0]][clanID]][clanName], sizeof(log_clan));
            }

            strcopy(log_arena, arenaData[finishingArenaId][arenaName], sizeof(log_arena));

            new log_kills = playerArenaInfo[location][playersTop[0]][arenaKills];
            new log_dmg = floatround(playerArenaInfo[location][playersTop[0]][arenaDMG]);

            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO `arena_logs` (`arena_id`, `arena_name`, `winner_name`, `winner_clan`, `total_kills`, `total_damage`, `timestamp`) VALUES (%d, '%e', '%e', '%e', %d, %d, %d)",
                finishingArenaId, log_arena, log_winner, log_clan, log_kills, log_dmg, gettime());
            mysql_tquery(dbHandle, queryGlobal);

            new d_embed_str[1024];
            format(d_embed_str, sizeof(d_embed_str), "**Arena:** %s (ID: %d)\n**Winner:** %s\n**Clan:** %s\n**Kills:** %d\n**Damage:** %d",
            log_arena, finishingArenaId, log_winner, log_clan, log_kills, log_dmg);

            SendDiscordEmbed(logs_arena, "Arena Results", d_embed_str, DISCORD_COLOR_ARENA);
		}
	}

	SaveArenaMatchHistory(location, finishingArenaId, playersTop, sizeof(playersTop));

	foreach(new i : Player)
	{
		if(User[i][isLogged] && User[i][locationId] == location && !User[i][isAfk] && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE)
			Ach_OnArenaRoundEnd(i);
	}

	new bestClanID = -1;
	new highestKills = 0;
	new highestDamage = 0;

	for(new i = 0; i < MAX_CLANS; i++)
	{
		if(!ClanInfo[i][clanIDUsed]) continue;

		new cKills = ClanInfo[i][clanArenaKills][location];
		new cDamage = ClanInfo[i][clanArenaDMG][location];

		if(cKills == 0 && cDamage == 0) continue;

		if(cKills > highestKills || (cKills == highestKills && cDamage > highestDamage))
		{
			highestKills = cKills;
			highestDamage = cDamage;
			bestClanID = i;
		}
	}

	new mvpName[MAX_PLAYER_NAME + 1] = "None";
	if(IsPlayerConnected(playersTop[0]))
	{
		strcopy(mvpName, getPlayerName(playersTop[0]), sizeof(mvpName));
	}

	new namesList[512], damageList[512], killsList[512], ratioList[512];
	new base = (location * 20) + 26;

	new clanInfoStr[192];
	new bClanName[MAX_CLAN_NAME] = "None";
	new bClanKills = 0;
	new bClanDmg = 0;

	if(bestClanID != -1)
	{
		format(bClanName, sizeof(bClanName), "%s", ClanInfo[bestClanID][clanName]);
		bClanKills = ClanInfo[bestClanID][clanArenaKills][location];
		bClanDmg = ClanInfo[bestClanID][clanArenaDMG][location];
	}

	format(clanInfoStr, sizeof(clanInfoStr), "Best_clan_on_Arena_#%d_-_%s~n~Damage:_%d_/_Kills:_%d~n~MVP:_%s", location, bClanName, bClanDmg, bClanKills, mvpName);

	for (new i = 0; i < 5; i++)
	{
		new name[64], damage[64], kills[64], kdr[64];
		if(IsPlayerConnected(playersTop[i]) && User[playersTop[i]][locationId] == location)
		{
			new playerId = playersTop[i];
			format(name,sizeof(name),"%s", getPlayerName(playerId));
			format(damage, sizeof(damage), "%.0f", playerArenaInfo[location][playerId][arenaDMG]);
			format(kills, sizeof(kills), "%d", playerArenaInfo[location][playerId][arenaKills]);
			FormatRatio(playerArenaInfo[location][playerId][arenaKills], playerArenaInfo[location][playerId][arenaDeaths], kdr, sizeof(kdr));
		}
		else
		{
			name = "N/A";
			damage = "N/A";
			kills = "N/A";
			kdr = "N/A";
		}

		if(i == 0)
		{
			format(namesList, sizeof(namesList), "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~%s", name);
			format(damageList, sizeof(damageList), "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~%s", damage);
			format(killsList, sizeof(killsList), "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~%s", kills);
			format(ratioList, sizeof(ratioList), "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~%s", kdr);
		}
		else
		{
			strcat(namesList, fmat_return("~n~%s", name), sizeof(namesList));
			strcat(damageList, fmat_return("~n~%s", damage), sizeof(damageList));
			strcat(killsList, fmat_return("~n~%s", kills), sizeof(killsList));
			strcat(ratioList, fmat_return("~n~%s", kdr), sizeof(ratioList));
		}
	}


	foreach(new i : Player)
	{
		if(User[i][isAfk] || GetPVarInt(i, "DuelS_State") != DUELS_STATE_NONE)
			continue;

		if(User[i][locationId] == location)
		{
			TogglePlayerControllable(i, false);

			new playerColorStr[10];
			new hudIdx = GetPlayerHudColorIndex(i);
			if(strfind(hudPersonalization[hudIdx][colorOne], "~w~") != -1)
			{
				format(playerColorStr, sizeof(playerColorStr), "%s", hudPersonalization[hudIdx][colorTwo]);
			}
			else
			{
				format(playerColorStr, sizeof(playerColorStr), "%s", hudPersonalization[hudIdx][colorOne]);
			}

			format(clanInfoStr, sizeof(clanInfoStr), "Best_clan_on_Arena_#%d_-_%s%s~n~Damage:_%s%d~w~_/_Kills:_%s%d~n~MVP:_%s%s", location, playerColorStr, bClanName, playerColorStr, bClanDmg, playerColorStr, bClanKills, playerColorStr, mvpName);

            PlayerTextDrawSetString(i, topLocationTable[i][base], clanInfoStr);
            PlayerTextDrawSetString(i, topLocationTable[i][base + 1], namesList);
            PlayerTextDrawSetString(i, topLocationTable[i][base + 2], damageList);
            PlayerTextDrawSetString(i, topLocationTable[i][base + 3], killsList);
            PlayerTextDrawSetString(i, topLocationTable[i][base + 4], ratioList);

			for(new a = 0; a <= 5; a++)
				PlayerTextDrawShow(i, topLocationTable[i][a]);
			for(new a = 7; a <= 9; a++) // Headers
				PlayerTextDrawShow(i, topLocationTable[i][a]);

			for(new b = 0; b <= 4; b++)
				PlayerTextDrawShow(i, topLocationTable[i][base + b]);

			GameTextForPlayer(i, "~r~ROUND FINISHED", 2000, 1);
		}
	}

	for(new i = 0; i != MAX_CLANS; i++)
	{
		if(ClanInfo[i][clanIDUsed])
		{
			ClanInfo[i][clanArenaKills][location] = 0;
			ClanInfo[i][clanArenaDMG][location] = 0;
		}
	}

	arenaInfo[location][roundMints] = 0;
	arenaInfo[location][totalArenaKills] = 0;
	arenaInfo[location][isRunningArena] = false;
	SetTimerEx("ChangeArena", 6000, false, "i", location);

	return true;
}

ClearPlayerNewArenaData(playerid)
{
	if (!IsPlayerConnected(playerid))
	{
		return false;
	}

	User[playerid][newArenaStep] = 0;
	User[playerid][newArenaPos] = Float:{0.0, 0.0, 0.0, 0.0};

	GangZoneDestroy(User[playerid][newArenaZone]);
	User[playerid][newArenaZone] = INVALID_GANG_ZONE;

	DestroyVisualArea(User[playerid][newArenaVisualArea]);
	User[playerid][newArenaVisualArea] = INVALID_VISUAL_AREA_ID;

	if(User[playerid][newArenaTimer] != 0)
	{
		KillTimer(User[playerid][newArenaTimer]);
		User[playerid][newArenaTimer] = 0;
	}

	return true;
}

forward CameraDelay(playerid, i, Float:playerX, Float:playerY, Float:playerZ, Float:x, Float:y, Float:z);
public CameraDelay(playerid, i, Float:playerX, Float:playerY, Float:playerZ, Float:x, Float:y, Float:z)
{
	if (!IsPlayerConnected(playerid))
	{
		return 0;
	}

	switch (i ++)
	{
		case 0:
		{
			TogglePlayerControllable(playerid, false);

			x = playerX;
			y = playerY;
			z = playerZ + 125.0;
		}
		case 1:
		{
			x = User[playerid][newArenaPos][0] + 3.0; // (User[playerid][newArenaPos][0] + User[playerid][newArenaPos][2]) / 2.0;
			y = User[playerid][newArenaPos][1] + 3.0;
			z = GetVisualAreaZ(User[playerid][newArenaVisualArea]) + 125.0;
		}
		case 2:
		{
			x = User[playerid][newArenaPos][0] + 3.0;
			y = User[playerid][newArenaPos][3] - 3.0; // (User[playerid][newArenaPos][1] + User[playerid][newArenaPos][3]) / 2.0;
		}
		case 3:
		{
			x = User[playerid][newArenaPos][2] - 3.0; // (User[playerid][newArenaPos][0] + User[playerid][newArenaPos][2]) / 2.0;
			y = User[playerid][newArenaPos][3] - 3.0;
		}
		case 4:
		{
			x = User[playerid][newArenaPos][2] - 3.0;
			y = User[playerid][newArenaPos][1] + 3.0; // (User[playerid][newArenaPos][1] + User[playerid][newArenaPos][3]) / 2.0;
		}
		case 5:
		{
			Dialog_ShowCallback(playerid, using public NewArena<iiiis>, DIALOG_STYLE_MSGBOX, "New arena", "Do you want to save this arena?", "Yes", "No");
			return 1;
		}
	}

	SetPlayerCameraPos(playerid, x, y, z);
	SetPlayerCameraLookAt(playerid, playerX, playerY, playerZ, CAMERA_MOVE);

	User[playerid][newArenaTimer] = SetTimerEx("CameraDelay", 3000, false, "iiffffff", playerid, i, playerX, playerY, playerZ, x, y, z);

	return 1;
}

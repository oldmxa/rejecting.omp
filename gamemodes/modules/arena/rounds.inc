
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

        new mapName[48], slots[48], pCount = getLocationPlayers(i), mCount = arenaInfo[i][maxPlayers];
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
        if(User[i][locationId] != location || GetPVarInt(i, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(i)) continue;
        GangZoneStopPulseForPlayer(i, e_ZONE_PULSE_ZONE, arenaInfo[location][arenaId]);
    }

    GetRoundRank(location);

    new oldArenaId = arenaInfo[location][arenaId];
    arenaInfo[location][arenaId] = zone;
    PrepareChangeArena(location, oldArenaId);

    foreach(new i : Player)
    {
        if((User[i][locationId] == location && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE && !IsPlayerInDuel2v2(i)) || User[i][adminLevel] > 0)
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

	if (Iter_Count(ArenaQueue<type>) > 1 && Iter_Contains(ArenaQueue<type>, lastArenaId[location]))
	{
		Iter_Remove(ArenaQueue<type>, lastArenaId[location]);
	}

	rand = Iter_Random(ArenaQueue<type>);
	Iter_Remove(ArenaQueue<type>, rand);

	return rand;
}

stock LogDiscordNextMap(location, oldArenaId, newArenaId)
{
    if(!(0 <= location < MAX_LOCATIONS)) return 0;
    if(location == ARENA_AFK) return 0;

    new oldName[64], newName[64], mapStr[512];

    if(0 <= oldArenaId < MAX_ZONES)
        format(oldName, sizeof(oldName), "%s (ID: %d)", arenaData[oldArenaId][arenaName], oldArenaId);
    else
        format(oldName, sizeof(oldName), "None");

    if(0 <= newArenaId < MAX_ZONES)
        format(newName, sizeof(newName), "%s (ID: %d)", arenaData[newArenaId][arenaName], newArenaId);
    else
        format(newName, sizeof(newName), "Unknown");

    new pCount = getLocationPlayers(location);

    format(mapStr, sizeof(mapStr),
        "**Room:** %s\n**New Map:** %s\n**Previous Map:** %s\n**Players Online:** %d\n**Round Duration:** %d minutes",
        ROOM_NAME[location][E_ROOM_NAME_DISCORD],
        newName,
        oldName,
        pCount,
        RoundTime / 60
    );

    SendDiscordEmbed(logs_nextmap, "Next Map", mapStr, DISCORD_COLOR_ARENA);
    return 1;
}

forward ChangeArena(location);
public ChangeArena(location)
{
	new iString[150], clanString[300];
	clanString[0] = EOS;

	format(iString, sizeof(iString), ARENA_START_MSG, arenaData[arenaInfo[location][arenaId]][arenaName], arenaInfo[location][arenaId]);
	LogDiscordNextMap(location, lastArenaId[location], arenaInfo[location][arenaId]);

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

	arenaInfo[location][roundMints] = RoundTime;
	arenaInfo[location][isRunningArena] = true;

	foreach(new i : Player)
	{
		if (User[i][locationId] == location && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE && !IsPlayerInDuel2v2(i))
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

		if (i == ARENA_AFK) continue;

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

#define SPAWN_RAYCAST_CEILING (700.0)

#define SPAWN_CEILING_ABOVE_FLOOR (120.0)
#define SPAWN_FLOOR_TOLERANCE     (25.0)

#define SPAWN_GUARD_INTERVAL (150)
#define SPAWN_GUARD_TICKS    (20)

#define MAX_CUSTOM_ARENAS   (8)
#define CUSTOM_SPAWN_GRID   (12)
#define MAX_CUSTOM_SPAWNS   (CUSTOM_SPAWN_GRID * CUSTOM_SPAWN_GRID)
#define CUSTOM_SPAWN_TOP    (60.0)
#define CUSTOM_SPAWN_DOWN   (30.0)
#define CUSTOM_SPAWN_LEVEL  (8.0)
#define CUSTOM_SPAWN_BUCKET (2.0)
#define CUSTOM_SPAWN_SLOTS  (46)
#define CUSTOM_SPAWN_HEAD   (2.2)
#define CUSTOM_SPAWN_LAYERS (3)
#define CUSTOM_SPAWN_STEPS  (10)

// synchronize with other players 500ms
#define SPAWN_PROTECT_TIME (500)

new SpawnProtectTimer[MAX_PLAYERS];
new SpawnGuardTimer[MAX_PLAYERS];

static
    Float:CustomSpawnPos[MAX_CUSTOM_ARENAS][MAX_CUSTOM_SPAWNS][3],
    Float:CustomSpawnFloor[MAX_CUSTOM_ARENAS],
    CustomSpawnCount[MAX_CUSTOM_ARENAS] = {-1, ...};

forward RestoreSpawnWorld(playerid);
forward SpawnFloorGuard(playerid);
forward WarmCustomSpawns();

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

static stock GetCustomArenaIndex(zone)
{
    if (!IsCustomArena(zone)) return -1;

    new count = 0;

    for (new i; i < MAX_ZONES; i++)
    {
        if (!IsCustomArena(i)) continue;
        if (i == zone) return (count < MAX_CUSTOM_ARENAS) ? count : -1;

        count++;
    }
    return -1;
}

// Standing surfaces under (x, y), from the highest down: a roof answers first,
// so the walk has to keep going to reach the floor the round is played on.
static stock CollectSurfaces(Float:x, Float:y, Float:top, Float:bottom, Float:out[], size)
{
    new count = 0;
    new Float:z = top;

    for (new step; step < CUSTOM_SPAWN_STEPS && count < size; step++)
    {
        new Float:hx, Float:hy, Float:hz, Float:cx, Float:cy, Float:cz;

        new modelid = CA_RayCastLine(x, y, z, x, y, bottom, hx, hy, hz);

        if (!modelid || modelid == WATER_OBJECT) break;

        if (!CA_RayCastLine(x, y, hz + 0.6, x, y, hz + CUSTOM_SPAWN_HEAD, cx, cy, cz))
            out[count++] = hz;

        z = hz - 0.2;
        if (z <= bottom) break;
    }
    return count;
}

// Spawns are measured once against the ColAndreas copy of the map: the grid
// keeps every surface a player fits on, and the height shared by most of them
// is taken as the arena floor, so a wrong Z in the table cannot drop anyone.
static stock BuildCustomSpawns(index, zone)
{
    new Float:floorZ = arenaData[zone][arenaFloorZ];
    new Float:top    = floorZ + CUSTOM_SPAWN_TOP;
    new Float:bottom = floorZ - CUSTOM_SPAWN_DOWN;

    new Float:stepX = (arenaData[zone][arenaPos][2] - arenaData[zone][arenaPos][0]) / (CUSTOM_SPAWN_GRID + 1);
    new Float:stepY = (arenaData[zone][arenaPos][3] - arenaData[zone][arenaPos][1]) / (CUSTOM_SPAWN_GRID + 1);

    static Float:surfaceZ[CUSTOM_SPAWN_GRID * CUSTOM_SPAWN_GRID][CUSTOM_SPAWN_LAYERS];
    static surfaceCount[CUSTOM_SPAWN_GRID * CUSTOM_SPAWN_GRID];

    new buckets[CUSTOM_SPAWN_SLOTS];

    for (new gx; gx < CUSTOM_SPAWN_GRID; gx++)
    {
        for (new gy; gy < CUSTOM_SPAWN_GRID; gy++)
        {
            new point = gx * CUSTOM_SPAWN_GRID + gy;

            new Float:x = arenaData[zone][arenaPos][0] + stepX * (gx + 1);
            new Float:y = arenaData[zone][arenaPos][1] + stepY * (gy + 1);

            surfaceCount[point] = CollectSurfaces(x, y, top, bottom, surfaceZ[point], CUSTOM_SPAWN_LAYERS);

            for (new s; s < surfaceCount[point]; s++)
            {
                new slot = floatround((surfaceZ[point][s] - bottom) / CUSTOM_SPAWN_BUCKET, floatround_floor);
                if (0 <= slot < sizeof(buckets)) buckets[slot]++;
            }
        }
    }

    new best = -1;
    for (new slot; slot < sizeof(buckets); slot++)
        if (buckets[slot] > ((best == -1) ? 0 : buckets[best])) best = slot;

    if (best == -1)
    {
        CustomSpawnFloor[index] = floorZ;
        CustomSpawnCount[index] = 0;
        return 0;
    }

    new Float:mainZ = bottom + (best + 0.5) * CUSTOM_SPAWN_BUCKET;
    new count = 0;

    for (new point; point < sizeof(surfaceCount) && count < MAX_CUSTOM_SPAWNS; point++)
    {
        for (new s; s < surfaceCount[point]; s++)
        {
            if (floatabs(surfaceZ[point][s] - mainZ) > CUSTOM_SPAWN_LEVEL) continue;

            CustomSpawnPos[index][count][0] = arenaData[zone][arenaPos][0] + stepX * ((point / CUSTOM_SPAWN_GRID) + 1);
            CustomSpawnPos[index][count][1] = arenaData[zone][arenaPos][1] + stepY * ((point % CUSTOM_SPAWN_GRID) + 1);
            CustomSpawnPos[index][count][2] = surfaceZ[point][s];
            count++;
            break;
        }
    }

    CustomSpawnFloor[index] = mainZ;
    CustomSpawnCount[index] = count;

    printf("[arena] %s: %d spawns, floor %.2f", arenaData[zone][arenaName], count, mainZ);

    return count;
}

static stock Float:GetCustomArenaFloor(zone)
{
    new index = GetCustomArenaIndex(zone);

    if (index == -1 || CustomSpawnCount[index] <= 0) return arenaData[zone][arenaFloorZ];

    return CustomSpawnFloor[index];
}

public WarmCustomSpawns()
{
    for (new index; index < MAX_CUSTOM_ARENAS; index++)
    {
        new zone = GetCustomArenaByIndex(index);
        if (zone == -1) break;

        if (CustomSpawnCount[index] == -1) BuildCustomSpawns(index, zone);
    }
    return 1;
}

static stock PickCustomSpawn(playerid, zone, &Float:outX, &Float:outY, &Float:outZ)
{
    new index = GetCustomArenaIndex(zone);
    if (index == -1) return 0;

    if (CustomSpawnCount[index] == -1) BuildCustomSpawns(index, zone);
    if (CustomSpawnCount[index] <= 0) return 0;

    new Float:bestClearance = -1.0;

    for (new attempt = 0; attempt < SPAWN_MAX_ATTEMPTS; attempt++)
    {
        new slot = random(CustomSpawnCount[index]);

        new Float:clearance = GetSpawnClearance(playerid,
            CustomSpawnPos[index][slot][0],
            CustomSpawnPos[index][slot][1],
            CustomSpawnPos[index][slot][2]
        );

        if (clearance > bestClearance)
        {
            bestClearance = clearance;
            outX = CustomSpawnPos[index][slot][0];
            outY = CustomSpawnPos[index][slot][1];
            outZ = CustomSpawnPos[index][slot][2];
        }

        if (clearance >= SPAWN_MIN_DISTANCE) break;
    }
    return 1;
}

spawnOnLocation(playerid)
{
	if(User[playerid][locationId] == -1) return true;

	new zone = arenaInfo[User[playerid][locationId]][arenaId];
	new Float:posX, Float:posY, Float:posZ;
	new Float:bestX, Float:bestY, Float:bestZ, Float:bestClearance = -1.0;

	new customArena = IsCustomArena(zone);

	new Float:floorZ = arenaData[zone][arenaFloorZ];

	new Float:ceiling    = customArena ? (floorZ + SPAWN_CEILING_ABOVE_FLOOR) : SPAWN_RAYCAST_CEILING;
	new Float:floorLimit = floorZ - SPAWN_FLOOR_TOLERANCE;

	if(customArena && PickCustomSpawn(playerid, zone, bestX, bestY, bestZ))
	{
		bestClearance = 0.0;
		floorLimit = GetCustomArenaFloor(zone) - SPAWN_FLOOR_TOLERANCE;
	}

	for(new attempt = 0; bestClearance < 0.0 && attempt < SPAWN_MAX_ATTEMPTS; attempt++)
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

		if(customArena && (!fallbackModel || fallbackModel == WATER_OBJECT || bestZ < floorLimit))
			bestZ = floorZ;
	}

	SetPlayerInterior(playerid, arenaData[zone][arenaInterior]);

	bestZ += 1.0;

	// Must run before the teleport: the floor is streamed objects, and arriving
	// first means falling through while they are still being created.
	if(customArena)
		Streamer_UpdateEx(playerid, bestX, bestY, bestZ, -1, arenaData[zone][arenaInterior], STREAMER_TYPE_OBJECT);

	SetPlayerPos(playerid, bestX, bestY, bestZ);
	SetPlayerFacingAngle(playerid, RandomFloat(0.0, 360.0));
	SetCameraBehindPlayer(playerid);

	KillTimer(SpawnGuardTimer[playerid]);
	SpawnGuardTimer[playerid] = 0;

	if(customArena)
	{
		SetPVarFloat(playerid, "spawn_guard_x", bestX);
		SetPVarFloat(playerid, "spawn_guard_y", bestY);
		SetPVarFloat(playerid, "spawn_guard_z", bestZ);
		SetPVarFloat(playerid, "spawn_guard_min", floorLimit);
		SetPVarInt(playerid, "spawn_guard_loc", User[playerid][locationId]);
		SetPVarInt(playerid, "spawn_guard_ticks", SPAWN_GUARD_TICKS);

		SpawnGuardTimer[playerid] = SetTimerEx("SpawnFloorGuard", SPAWN_GUARD_INTERVAL, true, "i", playerid);
	}

	DeletePVar(playerid, "gunmenu_world");

	if(GetPVarInt(playerid, "showGunmenuFirst") == 0)
	{
		SetPlayerVirtualWorld(playerid, SPAWN_VW + playerid);
		SetPVarInt(playerid, "spawn_protect", 1);

		KillTimer(SpawnProtectTimer[playerid]);
		SpawnProtectTimer[playerid] = SetTimerEx("RestoreSpawnWorld", SPAWN_PROTECT_TIME, false, "i", playerid);
	}

	new location = User[playerid][locationId];

	if(location != ARENA_AFK && !arenaInfo[location][isRunningArena])
	{
		TogglePlayerControllable(playerid, false);
		GameTextForPlayer(playerid, "~r~ROUND FINISHED", 2000, 1);
	}

	return true;
}

static StopSpawnFloorGuard(playerid)
{
	KillTimer(SpawnGuardTimer[playerid]);
	SpawnGuardTimer[playerid] = 0;

	DeletePVar(playerid, "spawn_guard_x");
	DeletePVar(playerid, "spawn_guard_y");
	DeletePVar(playerid, "spawn_guard_z");
	DeletePVar(playerid, "spawn_guard_min");
	DeletePVar(playerid, "spawn_guard_loc");
	DeletePVar(playerid, "spawn_guard_ticks");
	return 1;
}

// The arena floor is made of streamed objects: if the client has not created
// them yet the player drops through it, so the first seconds after the spawn
// put anyone who falls below the floor back on the spawn point.
public SpawnFloorGuard(playerid)
{
	new ticks = GetPVarInt(playerid, "spawn_guard_ticks") - 1;

	if(ticks <= 0
	|| !IsPlayerConnected(playerid)
	|| !User[playerid][isLogged]
	|| User[playerid][isAfk]
	|| User[playerid][locationId] != GetPVarInt(playerid, "spawn_guard_loc")
	|| GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		return StopSpawnFloorGuard(playerid);

	SetPVarInt(playerid, "spawn_guard_ticks", ticks);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if(z < GetPVarFloat(playerid, "spawn_guard_min"))
	{
		SetPlayerPos(playerid,
			GetPVarFloat(playerid, "spawn_guard_x"),
			GetPVarFloat(playerid, "spawn_guard_y"),
			GetPVarFloat(playerid, "spawn_guard_z")
		);
	}
	return 1;
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
	lastArenaId[location] = finishingArenaId;

	new g_PlayerArray<playersTop>;

	sortPlayersInline playersTop => (R = l > r)
	{
		new lc = IsPlayerConnected(l) && !IsPlayerNPC(l) && User[l][locationId] == location && GetPVarInt(l, "DuelS_State") == DUELS_STATE_NONE && !IsPlayerInDuel2v2(l);
		new rc = IsPlayerConnected(r) && !IsPlayerNPC(r) && User[r][locationId] == location && GetPVarInt(r, "DuelS_State") == DUELS_STATE_NONE && !IsPlayerInDuel2v2(r);

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
		if(User[i][isLogged] && User[i][locationId] == location && !User[i][isAfk] && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE && !IsPlayerInDuel2v2(i))
		{
			if(GetPVarInt(i, "Spree") > User[i][maxStreak])
			{
				User[i][maxStreak] = GetPVarInt(i, "Spree");
				SendClientMessage(i, COLOR_ORANGE, "{MAIN}[INFO] {FFFFFF}Your killing streak has been successfully updated!");
			}
			DeletePVar(i, "Spree");
			User[i][spreeTime] = 0;
			Ach_OnArenaRoundEnd(i);
		}
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

	new clanInfoStr[288];
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
		if(User[i][isAfk] || GetPVarInt(i, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(i))
			continue;

		if(User[i][locationId] == location)
		{
			TogglePlayerControllable(i, false);

			new playerColorStr[20];
			if(strfind(GetPlayerHudTag(i, false), "~w~") != -1)
			{
				format(playerColorStr, sizeof(playerColorStr), "%s", GetPlayerHudTag(i, true));
			}
			else
			{
				format(playerColorStr, sizeof(playerColorStr), "%s", GetPlayerHudTag(i, false));
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

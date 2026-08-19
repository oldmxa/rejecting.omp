
SyncPlayer(playerid, bool:playercalled = true)
{
	if(User[playerid][isFrozen])
		return SendClientMessage(playerid, -1, "%s", CANNOT_DO_THIS);

	if(User[playerid][lastDamageTime] > gettime() + 5)
		return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot sync if you took damage in the last 5 seconds.");

	if(gettime() <= GetPVarInt(playerid, "player_flood_tick_sync"))
	{
		new syncflood = GetPVarInt(playerid, "player_flood_tick_sync") - gettime() + 1;
		va_SendClientMessage(playerid, -1, KEEP_CALM_MSG, syncflood);
		return false;
	}

	if(GetPlayerVehicleID(playerid))
		return false;

	ClearAnimations(playerid, SYNC_ALL);

	if(playercalled)
		SetPVarInt(playerid, "player_flood_tick_sync", gettime() + 1);

	return true;
}

forward LoadMap(playerid, loadtime);
public LoadMap(playerid, loadtime)
{
	if(LoaderFreezer == 0) return 1;

	if(loadtime <= 0)
	{
		if(GetPlayerPing(playerid) < 2000) loadtime = 1000+GetPlayerPing(playerid); else loadtime = 2000;
	}

	ClearAnimations(playerid);
	TogglePlayerControllable(playerid,false);

	KillTimer(TeleLockTimer[playerid]);

	TeleLockTimer[playerid] = SetTimerEx("Unfreeze",loadtime,false, "i", playerid);
	return true;
}

forward Unfreeze(playerid);
public Unfreeze(playerid)
{
	if(!IsPlayerConnected(playerid)) return true;
	if(User[playerid][isFrozen]) return true;

	TogglePlayerControllable(playerid,true);
	return true;
}

forward LoadPlayerMoney(playerid);
public LoadPlayerMoney(playerid)
{
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, User[playerid][playerMoney]);
	return true;
}

forward HideWeaponObject(playerid);
public HideWeaponObject(playerid)
{
	if(objecthporarmour[playerid] != INVALID_OBJECT_ID)
	{
		DestroyObject(objecthporarmour[playerid]);
		objecthporarmour[playerid] = INVALID_OBJECT_ID;
	}
}

// stocks

TogglePlayerControllableEx(playerid, bool:toggle)
{
	TogglePlayerControllable(playerid, toggle);
	User[playerid][isFrozen] = !toggle;
}

setAfkReason(playerid, const reasonn[])
{
	new reasonid;
	if(!strcmp(reasonn, "pause", true)) reasonid = 1;
	if(!strcmp(reasonn, "highfps", true)) reasonid = 2;
	if(!strcmp(reasonn, "lowfps", true)) reasonid = 3;
	if(!strcmp(reasonn, "highping", true)) reasonid = 4;
	if(!strcmp(reasonn, "highpl", true)) reasonid = 5;
	if(!strcmp(reasonn, "fps102", true)) reasonid = 6;

	if(User[playerid][isAfk] == false)
	{
		switch(reasonid)
		{
			case 1:
			{
				SendClientMessage(playerid, -1, ""COL_GRAY"[LNG] You have been teleported to the lounge. Reason: PAUSED");
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ""COL_GRAY"[LNG] %s Was teleported to the lounge. Reason: PAUSED", getPlayerName(playerid));
				}

			}
			case 2:
			{
				va_SendClientMessage(playerid, -1, ""COL_GRAY"[LNG] You have been teleported to the lounge. Reason: TOO HIGH FPS %d/%d", User[playerid][FPS], ServerMaxFPS);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ""COL_GRAY"[LNG] %s Was teleported to the lounge. Reason: HIGH FPS", getPlayerName(playerid));
				}
			}
			case 3:
			{
				va_SendClientMessage(playerid, -1, ""COL_GRAY"[LNG] You have been teleported to the lounge. Reason: TOO LOW FPS %d/%d", User[playerid][FPS], ServerMinFPS);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ""COL_GRAY"[LNG] %s Was teleported to the lounge. Reason: LOW FPS", getPlayerName(playerid));
				}
			}
			case 4:
			{
				va_SendClientMessage(playerid, -1, ""COL_GRAY"[LNG] You have been teleported to the lounge. Reason: TOO HIGH PING %d/%d", GetPlayerPing(playerid), ServerMaxPing);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ""COL_GRAY"[LNG] %s Was teleported to the lounge. Reason: HIGH PING", getPlayerName(playerid));
				}
			}
			case 5:
			{
				va_SendClientMessage(playerid, -1, ""COL_GRAY"[LNG] You have been teleported to the lounge. Reason: TOO HIGH PL %.2f/%.2f", NetStats_PacketLossPercent(playerid), ServerMaxPacketLoss);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ""COL_GRAY"[LNG] %s Was teleported to the lounge. Reason: HIGH PL", getPlayerName(playerid));
				}
			}
			case 6:
			{
				va_SendClientMessage(playerid, -1, ""COL_GRAY"[LNG] You have been teleported to the lounge. Reason: FPS DRIFTED FROM %d (yours: %d)", ARENA_FPS_LOCK_VALUE, User[playerid][FPS]);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ""COL_GRAY"[LNG] %s Was teleported to the lounge. Reason: FPS LOCK", getPlayerName(playerid));
				}
			}
		}
		setAfk(playerid);
	}
}

forward ReleaseAsk(playerid);
public ReleaseAsk(playerid)
{
    DeletePVar(playerid, "ask_cooldown");
    return 1;
}

forward ReleaseReport(playerid);
public ReleaseReport(playerid)
{
    DeletePVar(playerid, "report_cooldown");
    return 1;
}

gunMenu(playerid)
{
	if(!User[playerid][isAfk] && User[playerid][locationId] != -1 && User[playerid][locationId] != ARENA_AFK)
	{
		if(!GetPVarInt(playerid, "gunmenu_world"))
		{
			SetPVarInt(playerid, "gunmenu_world", 1);
			SetPlayerVirtualWorld(playerid, GUNMENU_VW + playerid);
		}
	}

	new weaponName[4][20], strr[500];
	for(new i, weapon; i < 4; i++)
	{
		if (!(0 <= (weapon = User[playerid][playerWeapons][i]) < sizeof(gunmenuWInfo)))
		{
			weapon = User[playerid][playerWeapons][i] = 0;
		}

		weaponName[i] =  wName[gunmenuWInfo[weapon][weaponID]];
	}

	switch(User[playerid][playerLevel])
	{
		case 0..4: format(strr, sizeof(strr), "Slot #\tGun\nOne\t{229954}%s\nTwo\t{229954}%s\nThree {ff3c00}*\t{229954}%s\nFourth {ff3c00}*\t{229954}%s",
			weaponName[0], weaponName[1], weaponName[2], weaponName[3]);

		case 5..9: format(strr, sizeof(strr), "Slot #\tGun\nOne\t{229954}%s\nTwo\t{229954}%s\nThree \t{229954}%s\nFourth{ff3c00}*\t{229954}%s",
			weaponName[0], weaponName[1], weaponName[2], weaponName[3]);

		default: format(strr, sizeof(strr), "Slot #\tGun\nOne\t{229954}%s\nTwo\t{229954}%s\nThree \t{229954}%s\nFourth\t{229954}%s",
			weaponName[0], weaponName[1], weaponName[2], weaponName[3]);
	}
	Dialog_ShowCallback(playerid, using public CHANGE<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{ff3c00}Selection guns", strr, "Select", "Play");
	return true;
}

setAfk(playerid)
{
	if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
	{
		CallLocalFunction("CancelPlayerDuel", "i", playerid);
	}
	CallLocalFunction("ForfeitDuel2v2OnAfk", "i", playerid);
	CallLocalFunction("TrainingOnSetAfk", "i", playerid);
	CallLocalFunction("PracticeOnSetAfk", "i", playerid);
	if((User[playerid][loungeX] == 0.0 && User[playerid][loungeY] == 0.0) || User[playerid][loungeX] == -1504.1549 || User[playerid][loungeVW] == 0)
	{
		User[playerid][loungeX] = 1209.1161;
		User[playerid][loungeY] = -2036.4353;
		User[playerid][loungeZ] = 69.0078;
		User[playerid][loungeA] = 91.1024;
		User[playerid][loungeVW] = LOUNGEVW;
		User[playerid][loungeINT] = 0;
	}
	SetPlayerVirtualWorld(playerid, User[playerid][loungeVW]);
	SetPlayerPos(playerid, User[playerid][loungeX], User[playerid][loungeY], User[playerid][loungeZ]);
	SetPlayerInterior(playerid, User[playerid][loungeINT]);
	SetPlayerFacingAngle(playerid, User[playerid][loungeA]);
	SetCameraBehindPlayer(playerid);
	SetPlayerArmour(playerid, 100);
	SetPlayerHealth(playerid, 100);
	givePlayerWeapons(playerid);

	HideRoundStatsTD(playerid);

	User[playerid][isAfk] = true;
	User[playerid][locationId] = ARENA_AFK;

	GangZoneStopPulseForPlayer(playerid, e_ZONE_PULSE_ZONE, arenaInfo[ARENA_AFK][arenaId]);
	LoadMap(playerid, 0);
	updatePlayerHud(playerid);
	CallLocalFunction("ResetPlayerOutpostCapture", "i", playerid);

	// modules/safehouse: while the toggle is on the base replaces the lounge,
	// custom or not, so the move happens after everything else is applied
	if(User[playerid][spawnSafehouse])
	{
		new safehouse = GetPlayerSafehouse(playerid);
		if(safehouse != -1) EnterSafehouse(playerid, safehouse);
	}
}

SpawnPlayerEx(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
		RemovePlayerFromVehicle(playerid);

	SpawnPlayer(playerid);
	return 1;
}

#define SNIPER_DROP_AMMO (30)

#define M4_DROP_AMMO     (50)

#define PICKUP_MODEL_M4     (356)
#define PICKUP_MODEL_SNIPER (358)

// time for the weapon to disappear from the ground
#define WEAPON_DROP_LIFETIME (15)

static stock CreateWeaponDrop(playerid, WEAPON:weap, ammo, model)
{
	if(ammo < 1) return 0;

	new Float:ppos[3];
	GetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]);

	new Float:dropAngle = RandomFloat(0.0, 360.0);

	new Float:dropDist = float(random(2) + 1);

	for(new i; i < MAX_PICKUPSS; i++)
	{
		if(pickupID[i] != -1 || pickupTime[i] != 0) continue;

		pickupID[i] = CreatePickup(model,
			1,
			 ppos[0] + dropDist * floatsin(dropAngle, degrees),
			 ppos[1] + dropDist * floatcos(dropAngle, degrees),
			 ppos[2],
			 -1);

		pickupWeapon[i] = _:weap;
		pickupData[i] = ammo;
		pickupTime[i] = gettime() + WEAPON_DROP_LIFETIME;
		return 1;
	}
	return 0;
}

dropPlayerSniper(playerid)
{
	new WEAPON:weap, weapammo;

	GetPlayerWeaponData(playerid, WEAPON_SLOT:6, WEAPON:weap, weapammo);

	if(weap == WEAPON_SNIPER && weapammo > 0)
	{
		if(weapammo > SNIPER_DROP_AMMO)
			weapammo = SNIPER_DROP_AMMO;

		CreateWeaponDrop(playerid, WEAPON_SNIPER, weapammo, PICKUP_MODEL_SNIPER);
	}

	GetPlayerWeaponData(playerid, WEAPON_SLOT:5, WEAPON:weap, weapammo);

	if(weap == WEAPON_M4 && weapammo > 0)
	{
		if(weapammo > M4_DROP_AMMO)
			weapammo = M4_DROP_AMMO;

		CreateWeaponDrop(playerid, WEAPON_M4, weapammo, PICKUP_MODEL_M4);
	}
}

showAvailableWeapons()
{
	new string[128];
	for(new i = 1; i < sizeof ( gunmenuWInfo ); i ++)
			strcat(string, fmat_return("%s\n", wName[gunmenuWInfo[i][weaponID]]), sizeof(string));
	strcat(string, fmat_return("%s\n", wName[gunmenuWInfo[0][weaponID]]), sizeof(string));
	return string;
}

givePlayerWeapons(playerid)
{
	new playerWeapon[4], weapAmmo[4];
	for(new i, weapon; i < 4; i++)
	{
		if (!(0 <= (weapon = User[playerid][playerWeapons][i]) < sizeof(gunmenuWInfo)))
		{
			weapon = User[playerid][playerWeapons][i] = 0;
		}

		playerWeapon[i] =  gunmenuWInfo[weapon][weaponID];
		weapAmmo[i]   =  gunmenuWInfo[weapon][weaponAmmo];
	}

	ResetPlayerWeapons(playerid);
	for(new i = 0; i < 4; i++)
	{
		if(playerWeapon[i] != 0)
			GivePlayerWeapon(playerid, WEAPON:playerWeapon[i], weapAmmo[i]);
	}

	if(playerWeapon[0] != 0)
		SetPlayerArmedWeapon(playerid, WEAPON:playerWeapon[0]);
	else if(playerWeapon[1] != 0)
		SetPlayerArmedWeapon(playerid, WEAPON:playerWeapon[1]);
	else if(playerWeapon[2] != 0)
		SetPlayerArmedWeapon(playerid, WEAPON:playerWeapon[2]);
	else if(playerWeapon[3] != 0)
		SetPlayerArmedWeapon(playerid, WEAPON:playerWeapon[3]);
}

levelGoal(level)
{
	new lgoal;
	switch(level)
	{
		case 0: lgoal = 20;
		case 1: lgoal = 75;
		case 2: lgoal = 150;
		case 3: lgoal = 250;
		case 4: lgoal = 300;
		case 5: lgoal = 400;
		case 6: lgoal = 550;
		case 7: lgoal = 700;
		case 8: lgoal = 850;
		case 9: lgoal = 950;
		case 10: lgoal = 5000;
		case 11..19: lgoal = (level-10)*7500;
		case 20: lgoal = 75000;
		default: lgoal = (level > 20) ? ((level-10)*10000) : 20;
	}
	return lgoal;
}

PreloadAnims(playerid)
{
	ApplyAnimation(playerid, "ped", "null", 0.0, 0, 0, 0, 0, 0);
	return true;
}


SyncPlayer(playerid, bool:playercalled = true)
{
	if(IsPlayerFrozen(playerid))
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
	if(IsPlayerFrozen(playerid)) return true;

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

	if(!IsPlayerAFK(playerid))
	{
		new coloredNick[MAX_PLAYER_NAME + 18];
		format(coloredNick, sizeof(coloredNick), "{%06x}%s{FFFFFF}", GetPlayerDisplayColour(playerid), getPlayerName(playerid));

		switch(reasonid)
		{
			case 1:
			{
				static const afk_self[][] = {
					"{MAIN}[LNG] {FFFFFF}You have been teleported to the lounge. Reason: {MAIN}PAUSED / lounge{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Sent to the lounge due to inactivity: {MAIN}lounge / Paused{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Teleported to lounge: {MAIN}Player Inactive lounge{FFFFFF}."
				};
				static const afk_all[][] = {
					"{MAIN}[LNG] {FFFFFF}%s was teleported to the lounge. Reason: {MAIN}PAUSED / lounge{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}%s was moved to the lounge due to inactivity {MAIN}lounge{FFFFFF}."
				};
				SendClientMessage(playerid, -1, afk_self[random(sizeof(afk_self))]);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, afk_all[random(sizeof(afk_all))], coloredNick);
				}
			}
			case 2:
			{
				static const fps_self[][] = {
					"{MAIN}[LNG] {FFFFFF}You have been teleported to the lounge. Reason: {MAIN}High FPS (%d/%d){FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Teleported to lounge: {MAIN}FPS exceeded server limit (%d/%d FPS){FFFFFF}."
				};
				static const fps_all[][] = {
					"{MAIN}[LNG] {FFFFFF}%s was teleported to the lounge. Reason: {MAIN}HIGH FPS{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}%s was moved to the lounge. Reason: {MAIN}High Framerate{FFFFFF}."
				};
				va_SendClientMessage(playerid, -1, fps_self[random(sizeof(fps_self))], User[playerid][FPS], ServerMaxFPS);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, fps_all[random(sizeof(fps_all))], coloredNick);
				}
			}
			case 3:
			{
				static const lowfps_self[][] = {
					"{MAIN}[LNG] {FFFFFF}You have been teleported to the lounge. Reason: {MAIN}Low FPS (%d/%d){FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Teleported to lounge: {MAIN}FPS below minimum limit (%d/%d FPS){FFFFFF}."
				};
				static const lowfps_all[][] = {
					"{MAIN}[LNG] {FFFFFF}%s was teleported to the lounge. Reason: {MAIN}LOW FPS{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}%s was moved to the lounge. Reason: {MAIN}Low Framerate{FFFFFF}."
				};
				va_SendClientMessage(playerid, -1, lowfps_self[random(sizeof(lowfps_self))], User[playerid][FPS], ServerMinFPS);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, lowfps_all[random(sizeof(lowfps_all))], coloredNick);
				}
			}
			case 4:
			{
				static const ping_self[][] = {
					"{MAIN}[LNG] {FFFFFF}You have been teleported to the lounge. Reason: {MAIN}High Ping (%d/%d ms){FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Teleported to lounge: {MAIN}Ping exceeded maximum limit (%d/%d ms){FFFFFF}."
				};
				static const ping_all[][] = {
					"{MAIN}[LNG] {FFFFFF}%s was teleported to the lounge. Reason: {MAIN}HIGH PING{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}%s was moved to the lounge. Reason: {MAIN}High Latency{FFFFFF}."
				};
				va_SendClientMessage(playerid, -1, ping_self[random(sizeof(ping_self))], GetPlayerPing(playerid), ServerMaxPing);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, ping_all[random(sizeof(ping_all))], coloredNick);
				}
			}
			case 5:
			{
				static const pl_self[][] = {
					"{MAIN}[LNG] {FFFFFF}You have been teleported to the lounge. Reason: {MAIN}High Packet Loss (%.2f%%/%.2f%%){FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Teleported to lounge: {MAIN}Network connection unstable (%.2f%% Packet Loss){FFFFFF}."
				};
				static const pl_all[][] = {
					"{MAIN}[LNG] {FFFFFF}%s was teleported to the lounge. Reason: {MAIN}HIGH PL{FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}%s was moved to the lounge. Reason: {MAIN}Unstable Connection{FFFFFF}."
				};
				va_SendClientMessage(playerid, -1, pl_self[random(sizeof(pl_self))], NetStats_PacketLossPercent(playerid), ServerMaxPacketLoss);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, pl_all[random(sizeof(pl_all))], coloredNick);
				}
			}
			case 6:
			{
				static const lock_self[][] = {
					"{MAIN}[LNG] {FFFFFF}You have been teleported to the lounge. Reason: {MAIN}FPS exceeded arena limit of %d FPS (Yours: %d FPS){FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}Teleported to lounge: {MAIN}Current arena requires %d FPS lock (Yours: %d FPS){FFFFFF}."
				};
				static const lock_all[][] = {
					"{MAIN}[LNG] {FFFFFF}%s was teleported to the lounge. Reason: {MAIN}Arena FPS Limit (%d FPS){FFFFFF}.",
					"{MAIN}[LNG] {FFFFFF}%s was moved to the lounge: {MAIN}Exceeded Arena %d FPS Limit{FFFFFF}."
				};
				va_SendClientMessage(playerid, -1, lock_self[random(sizeof(lock_self))], ARENA_FPS_LOCK_VALUE, User[playerid][FPS]);
				foreach(new i : Player)
				{
					if(i != playerid)
						va_SendClientMessage(i, -1, lock_all[random(sizeof(lock_all))], coloredNick, ARENA_FPS_LOCK_VALUE);
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
	if(!IsPlayerInLounge(playerid))
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
	new bool:fromPersonal = bool:GetPVarInt(playerid, "gunmenu_from_personalization");
	Dialog_ShowCallback(playerid, using public CHANGE<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "{ff3c00}Selection guns", strr, "Select", fromPersonal ? "Back" : "Play");
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
		User[playerid][loungeX] = LOUNGE_SPAWN_X;
		User[playerid][loungeY] = LOUNGE_SPAWN_Y;
		User[playerid][loungeZ] = LOUNGE_SPAWN_Z;
		User[playerid][loungeA] = LOUNGE_SPAWN_A;
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

	ApplyArenaRpWalk(playerid, User[playerid][locationId], ARENA_AFK);

	User[playerid][isAfk] = true;
	User[playerid][locationId] = ARENA_AFK;

	if(GetPVarInt(playerid, "Spree") > User[playerid][maxStreak])
	{
		User[playerid][maxStreak] = GetPVarInt(playerid, "Spree");
	}
	DeletePVar(playerid, "Spree");
	User[playerid][spreeTime] = 0;

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
		else
		{
			User[playerid][spawnSafehouse] = false;
			UpdatePlayerData(playerid);
		}
	}
}

SpawnPlayerEx(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
		RemovePlayerFromVehicle(playerid);

	SpawnPlayer(playerid);
	return 1;
}

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

stock CleanupPlayerTimers(playerid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;

	if(TeleLockTimer[playerid])
	{
		KillTimer(TeleLockTimer[playerid]);
		TeleLockTimer[playerid] = 0;
	}
	if(piscaTimer[playerid])
	{
		KillTimer(piscaTimer[playerid]);
		piscaTimer[playerid] = 0;
	}
	if(objecthporarmour[playerid] != INVALID_OBJECT_ID)
	{
		DestroyObject(objecthporarmour[playerid]);
		objecthporarmour[playerid] = INVALID_OBJECT_ID;
	}
	if(hpandarmourtimer[playerid])
	{
		KillTimer(hpandarmourtimer[playerid]);
		hpandarmourtimer[playerid] = 0;
	}
	if(User[playerid][hitmarkTimer] != -1)
	{
		KillTimer(User[playerid][hitmarkTimer]);
		User[playerid][hitmarkTimer] = -1;
	}
	if(User[playerid][loginTimer])
	{
		KillTimer(User[playerid][loginTimer]);
		User[playerid][loginTimer] = 0;
	}
	if(User[playerid][adminLoginTimer])
	{
		KillTimer(User[playerid][adminLoginTimer]);
		User[playerid][adminLoginTimer] = 0;
	}
	if(User[playerid][adminAuthTimer])
	{
		KillTimer(User[playerid][adminAuthTimer]);
		User[playerid][adminAuthTimer] = 0;
	}
	return 1;
}

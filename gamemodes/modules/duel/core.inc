
#if defined _included_duel
	#endinput
#endif
#define _included_duel

#include <a_samp>

#if !defined COLOR_DUELS
	#define COLOR_DUELS 0x9771F5FF
#endif

#if !defined COLOR_TOMATO
	#define COLOR_TOMATO 0xFF6347FF
#endif

#if !defined COLOR_LRED
	#define COLOR_LRED 0xFF0000FF
#endif

#if !defined DIALOG_DUEL_ID
	#define DIALOG_DUEL_ID 9850
#endif

#if !defined DIALOG_DUEL_ACCEPT_ID
	#define DIALOG_DUEL_ACCEPT_ID 9851
#endif

#define DUELS_STATE_NONE    998
#define DUELS_STATE_WAITING 999

enum e_DUELS_DATA
{
	duelArena,
	duelInterior
}

new DuelS_Data[MAX_PLAYERS][e_DUELS_DATA];
new DuelS_Timer[MAX_PLAYERS];
new PlayerText:TDTimerDuelS[MAX_PLAYERS];

new Float:DuelS_Pos[21][2][4] =
{
	{
		{1775.3969,-1303.1936,114.8198,288.8297}, // Apartment LS
		{1814.2803,-1288.9517,114.8125,118.9101}
	},
	{
		{1695.5968,1337.3281,2516.7527,72.8742}, // Aurora Side
		{1661.0576,1350.9073,2516.7527,241.9861} 
	},
	{
		{-2452.4236,2327.3235,4.8359,79.7799}, // Bayside
		{-2506.2734,2342.5085,4.9751,247.8223}
	},
	{
		{-2433.1855,2520.1936,14.2304,133.9307}, // Bayside Trailer 
		{-2469.1418,2490.4148,17.3914,307.8120} 
	},
	{
		{-2347.4185,614.6311,42.0712,271.3934}, // Chinatown Roof
		{-2300.3157,617.5727,42.0700,96.7319}
	},
	{
		{1517.3495,-1649.1044,13.5469,346.9775}, // Commerce
		{1443.0592,-1680.6833,13.7780,272.2744}
	},
	{
		{649.9054,-587.4417,16.3359,347.4542}, // Dillimore Gas Station #1
		{664.1398,-546.1418,16.3359,152.9227} 
	},
	{
		{-1706.4758,28.1099,3.5547,171.5327}, // Easter Basin
		{-1706.5782,-0.2012,3.5489,354.5626}
	},
	{
		{-286.0168,-2172.5645,28.6722,214.4911}, // Flint Country
		{-258.9764,-2219.3127,28.6681,16.7994}
	},
	{
		{-154.0167,1125.4529,19.7422,172.0182}, // Fort Carson
		{-171.5257,1070.5386,19.7422,305.2254}
	},
	{
		{2367.4360,-1691.0743,13.6273,247.8865}, // Ganton #1
		{2415.6848,-1702.9259,13.7789,54.0988}  // Ganton #2
	},
	{
		{2490.2253,-1675.7072,13.3359,152.6216}, // Grove Street
		{2472.6755,-1730.2418,13.3828,339.5356}
	},
	{
		{1038.4053,-322.8241,73.9922,276.4315}, // Hilltop
		{1076.9969,-316.8098,73.9922,98.8545}
	},
	{
		{2001.9990,-1411.2988,16.9922,230.5547}, // Hospital LS
		{2032.6636,-1443.5309,17.2310,47.8302}
	},
	{
		{804.1825,-1345.0295,-0.5078,226.1350}, // Market Station #1
		{837.8577,-1375.5645,-0.5015,45.5471}
	},
	{
		{1964.0901,2372.6880,23.8469,229.6046}, // Redsands East Parking #1
		{2001.3853,2342.4189,23.8516,60.0664}
	},
	{
		{1019.2365,-1194.8635,54.9063,87.2607}, // Roof
		{981.0176,-1194.6741,54.9063,267.2485}
	},
	{
		{1161.8090,-1498.1881,22.7520,74.7942}, // Shopping
		{1091.8494,-1480.7130,22.7553,245.8758}
	},
	{
		{-873.3672,2763.7246,46.0000,245.7787}, // Tee Pee Motel
		{-834.2608,2746.0322,45.8516,57.0454}
	},
	{
		{-780.6841,-110.3776,64.7954,250.7670}, // The Panopticon
		{-738.8654,-129.9909,67.0670,68.3274}
	},
	{
		{1403.4952,-20.6579,1000.9114,87.5119}, // Warehouse
		{1361.2015,-20.5966,1000.9219,267.4032}
	}
};

new DuelS_ArenaNames[21][30] =
{
	"Apartment LS",
	"Aurora Side",
	"Bayside",
	"Bayside Trailer",
	"Chinatown Roof",
	"Commerce",
	"Dillimore Gas Station",
	"Easter Basin",
	"Flint Country",
	"Fort Carson",
	"Ganton",
	"Grove Street",
	"Hilltop",
	"Hospital LS",
	"Market Station",
	"Redsands East Parking",
	"Roof",
	"Shopping",
	"Tee Pee Motel",
	"The Panopticon",
	"Warehouse"
};

new DuelS_ArenaInteriors[21] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

#assert sizeof(DuelS_Pos) == sizeof(DuelS_ArenaNames)
#assert sizeof(DuelS_ArenaInteriors) == sizeof(DuelS_ArenaNames)

stock DuelS_ConvertTime(number)
{
	new smalltext[20];

	if(number < 0) number = 0;

	format(smalltext, sizeof(smalltext), "%d:%02d", number / 60, number % 60);
	return smalltext;
}

stock DuelS_CreateTextDraw(playerid)
{
	TDTimerDuelS[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 415.000000, "~w~00:00~n~~b~~h~200 ~w~: ~r~~h~200");
	PlayerTextDrawFont(playerid, TDTimerDuelS[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, TDTimerDuelS[playerid], 0.220000, 0.950000);
	PlayerTextDrawSetOutline(playerid, TDTimerDuelS[playerid], 1);
	PlayerTextDrawSetShadow(playerid, TDTimerDuelS[playerid], 0);
	PlayerTextDrawAlignment(playerid, TDTimerDuelS[playerid], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, TDTimerDuelS[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, TDTimerDuelS[playerid], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawSetProportional(playerid, TDTimerDuelS[playerid], true);
}

stock DuelS_DestroyTextDraw(playerid)
{
	if(TDTimerDuelS[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, TDTimerDuelS[playerid]);
		TDTimerDuelS[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
}

stock DuelS_StartCountdown(playerid, challengerid)
{
	ResetPlayerWeapons(playerid);
	ResetPlayerWeapons(challengerid);

	TogglePlayerControllable(playerid, 0);
	TogglePlayerControllable(challengerid, 0);

	GameTextForPlayer(playerid, "~y~3", 1000, 4);
	GameTextForPlayer(challengerid, "~y~3", 1000, 4);

	PlayerPlaySound(playerid, 1056, 0, 0, 0);
	PlayerPlaySound(challengerid, 1056, 0, 0, 0);

	GivePlayerWeapon(playerid, WEAPON_SNIPER, 1000);
	GivePlayerWeapon(challengerid, WEAPON_SNIPER, 1000);
	GivePlayerWeapon(playerid, WEAPON_SHOTGUN, 1000);
	GivePlayerWeapon(challengerid, WEAPON_SHOTGUN, 1000);
	GivePlayerWeapon(playerid, WEAPON_DEAGLE, 1000);
	GivePlayerWeapon(challengerid, WEAPON_DEAGLE, 1000);

	SetTimerEx("DuelS_Countdown", 1000, false, "iii", playerid, challengerid, 2);
}

forward DuelS_Countdown(playerid, challengerid, pos);
public DuelS_Countdown(playerid, challengerid, pos)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(challengerid)) return 1;
	if(GetPVarInt(playerid, "DuelS_State") != challengerid || GetPVarInt(challengerid, "DuelS_State") != playerid) return 1;

	switch(pos)
	{
		case 2:
		{
			GameTextForPlayer(playerid, "~b~2", 1000, 4);
			GameTextForPlayer(challengerid, "~b~2", 1000, 4);
			PlayerPlaySound(playerid, 1056, 0, 0, 0);
			PlayerPlaySound(challengerid, 1056, 0, 0, 0);
			SetTimerEx("DuelS_Countdown", 1000, false, "iii", playerid, challengerid, 1);
			SetCameraBehindPlayer(playerid);
			SetCameraBehindPlayer(challengerid);
		}
		case 1:
		{
			GameTextForPlayer(playerid, "~g~1", 1000, 4);
			GameTextForPlayer(challengerid, "~g~1", 1000, 4);
			PlayerPlaySound(playerid, 1056, 0, 0, 0);
			PlayerPlaySound(challengerid, 1056, 0, 0, 0);
			SetTimerEx("DuelS_Countdown", 1000, false, "iii", playerid, challengerid, 0);
			SetCameraBehindPlayer(playerid);
			SetCameraBehindPlayer(challengerid);
		}
		case 0:
		{
			GameTextForPlayer(playerid, "~r~DUEL!!", 1000, 4);
			GameTextForPlayer(challengerid, "~r~DUEL!!", 1000, 4);
			PlayerPlaySound(playerid, 1057, 0, 0, 0);
			PlayerPlaySound(challengerid, 1057, 0, 0, 0);
			TogglePlayerControllable(playerid, 1);
			TogglePlayerControllable(challengerid, 1);
			SetPlayerHealth(playerid, 100.0);
			SetPlayerArmour(playerid, 100.0);
			SetPlayerHealth(challengerid, 100.0);
			SetPlayerArmour(challengerid, 100.0);

			SendClientMessage(playerid, -1, "{MAIN}[DUEL] {FFFFFF}Combat started! You can use {MAIN}/backduel{FFFFFF} at any time to leave.");
			SendClientMessage(challengerid, -1, "{MAIN}[DUEL] {FFFFFF}Combat started! You can use {MAIN}/backduel{FFFFFF} at any time to leave.");

			SetPVarInt(playerid, "DuelS_Start", GetTickCount());
			SetPVarInt(challengerid, "DuelS_Start", GetTickCount());

			if(DuelS_Timer[playerid]) KillTimer(DuelS_Timer[playerid]);
			if(DuelS_Timer[challengerid] && DuelS_Timer[challengerid] != DuelS_Timer[playerid]) KillTimer(DuelS_Timer[challengerid]);

			// um unico timer atende os dois lados; ambos guardam o mesmo id
			DuelS_Timer[playerid] = SetTimerEx("DuelS_UpdateInfo", 1000, true, "ii", playerid, challengerid);
			DuelS_Timer[challengerid] = DuelS_Timer[playerid];
		}
	}
	return 1;
}

forward DuelS_UpdateInfo(playerid, challengerid);
public DuelS_UpdateInfo(playerid, challengerid)
{
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(challengerid)) return 1;
	if(GetPVarInt(playerid, "DuelS_State") != challengerid || GetPVarInt(challengerid, "DuelS_State") != playerid) return 1;

	new Float:pHealth, Float:cHealth, Float:pArmour, Float:cArmour, pAmount, cAmount;
	new strum[50], strdos[50];

	GetPlayerHealth(playerid, pHealth);
	GetPlayerHealth(challengerid, cHealth);
	GetPlayerArmour(playerid, pArmour);
	GetPlayerArmour(challengerid, cArmour);
	pAmount = floatround(pHealth + pArmour, floatround_round);
	cAmount = floatround(cHealth + cArmour, floatround_round);

	format(strum, sizeof(strum), "~w~%s~n~~b~~h~%d ~w~: ~r~~h~%d", DuelS_ConvertTime((GetTickCount() - GetPVarInt(playerid, "DuelS_Start")) / 1000), pAmount, cAmount);
	format(strdos, sizeof(strdos), "~w~%s~n~~b~~h~%d ~w~: ~r~~h~%d", DuelS_ConvertTime((GetTickCount() - GetPVarInt(challengerid, "DuelS_Start")) / 1000), pAmount, cAmount);

	PlayerTextDrawSetString(playerid, TDTimerDuelS[playerid], strum);
	PlayerTextDrawSetString(challengerid, TDTimerDuelS[challengerid], strdos);
	return 1;
}

stock DuelS_End(winner, loser, bool:forfeited = false, bool:disconnected = false)
{
	new Float:Result, Float:Health, Float:Armour, msgStr[200];

	GetPlayerArmour(winner, Armour);
	GetPlayerHealth(winner, Health);
	Result = Health + Armour;

	SetPVarInt(winner, "DuelS_State", DUELS_STATE_NONE);
	SetPVarInt(loser, "DuelS_State", DUELS_STATE_NONE);

	// winner e loser compartilham o mesmo timer: matar duas vezes o mesmo id
	if(DuelS_Timer[winner])
		KillTimer(DuelS_Timer[winner]);
	else if(DuelS_Timer[loser])
		KillTimer(DuelS_Timer[loser]);

	DuelS_Timer[winner] = 0;
	DuelS_Timer[loser] = 0;

	PlayerTextDrawHide(winner, TDTimerDuelS[winner]);
	PlayerTextDrawHide(loser, TDTimerDuelS[loser]);

	if(IsPlayerConnected(winner))
	{
		User[winner][duelWins]++;
		Ach_OnDuelWin(winner);
		GivePlayerMoney(winner, 2000);
		User[winner][playerMoney] += 2000;
		GameTextForPlayer(winner, sprintf("~g~~h~YOU WON THE DUEL!~n~~w~+%s", FormatMoney(2000)), 3000, 1);
		va_SendClientMessage(winner, -1, "{MAIN}[DUEL] {FFFFFF}You won the duel and earned {00FF00}+%s{FFFFFF}!", FormatMoney(2000));
		UpdatePlayerData(winner);
	}

	if(IsPlayerConnected(loser))
	{
		User[loser][duelLosses]++;
		GivePlayerMoney(loser, -2000);
		User[loser][playerMoney] -= 2000;
		if(User[loser][playerMoney] < 0) User[loser][playerMoney] = 0;
		GameTextForPlayer(loser, sprintf("~r~~h~YOU LOST THE DUEL!~n~~w~-%s", FormatMoney(2000)), 3000, 1);
		va_SendClientMessage(loser, -1, "{MAIN}[DUEL] {FFFFFF}You lost the duel and lost {FF0000}-%s{FFFFFF}.", FormatMoney(2000));
		UpdatePlayerData(loser);
	}

	new duration = (GetTickCount() - GetPVarInt(winner, "DuelS_Start")) / 1000;
	if(duration < 0) duration = 0;

	new winnerName[MAX_PLAYER_NAME + 1], loserName[MAX_PLAYER_NAME + 1];

	if(IsPlayerConnected(winner)) GetPlayerName(winner, winnerName, sizeof(winnerName));
	else format(winnerName, sizeof(winnerName), "%s", User[winner][userName]);

	if(IsPlayerConnected(loser)) GetPlayerName(loser, loserName, sizeof(loserName));
	else format(loserName, sizeof(loserName), "%s", User[loser][userName]);

	if(forfeited)
	{
		format(msgStr, sizeof(msgStr), "{MAIN}[DUEL] {FFFFFF}%s won the duel! %s ran away from combat.", winnerName, loserName);
	}
	else if(disconnected)
	{
		format(msgStr, sizeof(msgStr), "{MAIN}[DUEL] {FFFFFF}%s won the duel! %s disconnected during combat.", winnerName, loserName);
	}
	else if(Result >= 200.0)
	{
		format(msgStr, sizeof(msgStr), "{MAIN}[DUEL] {FFFFFF}%s achieved a PERFECT victory against %s in %d seconds!", winnerName, loserName, duration);
	}
	else
	{
		format(msgStr, sizeof(msgStr), "{MAIN}[DUEL] {FFFFFF}%s won the duel against %s with %d HP/Armour in %d seconds!", winnerName, loserName, floatround(Result), duration);
	}

	foreach(new i : Player)
	{
		if(User[i][isLogged])
		{
			new formattedMsg[256];
			strcopy(formattedMsg, msgStr, sizeof(formattedMsg));
			format_MainColor(i, formattedMsg);
			SendClientMessage(i, COLOR_LRED, "%s", formattedMsg);
		}
	}

	if(IsPlayerConnected(winner))
	{
		User[winner][isAfk] = false;
		if(User[winner][locationId] != -1 && User[winner][locationId] != ARENA_AFK)
		{
			SpawnPlayerEx(winner);
		}
		else
		{
			setAfk(winner);
		}
	}

	if(!disconnected && IsPlayerConnected(loser))
	{
		User[loser][isAfk] = false;
		if(User[loser][locationId] != -1 && User[loser][locationId] != ARENA_AFK)
		{
			new Float:lHP;
			GetPlayerHealth(loser, lHP);
			if(lHP > 0.0)
			{
				SpawnPlayerEx(loser);
			}
		}
		else
		{
			setAfk(loser);
		}
	}
	return 1;
}

stock DuelS_Host(playerid)
{
	if(User[playerid][isAfk] || User[playerid][locationId] == ARENA_AFK)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot create a duel while in the lounge.");

	if(User[playerid][isSpectating])
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot use this while spectating.");

	if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You are already in a duel! Use {MAIN}/backduel{FFFFFF} to leave.");

	if(IsPlayerInDuel2v2(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot create a duel while in a 2v2 duel.");

	new Float:Armour, Float:Health, Float:Result;
	GetPlayerHealth(playerid, Health);
	GetPlayerArmour(playerid, Armour);
	Result = (Health + Armour);

	if(Result < 190.0)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You need at least 190 total HP + Armour to start a duel.");

	SetPVarInt(playerid, "DuelS_State", DUELS_STATE_WAITING);

	new selectedArena = DuelS_Data[playerid][duelArena];
	SetPlayerPos(playerid, DuelS_Pos[selectedArena][0][0], DuelS_Pos[selectedArena][0][1], DuelS_Pos[selectedArena][0][2]);
	SetPlayerFacingAngle(playerid, DuelS_Pos[selectedArena][0][3]);
	SetPlayerVirtualWorld(playerid, playerid + 10000);
	SetPlayerInterior(playerid, DuelS_Data[playerid][duelInterior]);

	PlayerTextDrawShow(playerid, TDTimerDuelS[playerid]);
	PlayerTextDrawSetString(playerid, TDTimerDuelS[playerid], "~y~~h~WAITING FOR OPPONENT...");

	SendClientMessage(playerid, -1, "{MAIN}[DUEL] {FFFFFF}Duel created! Type {MAIN}/backduel{FFFFFF} at any time to leave the queue.");

	new dString[180];
	format(dString, sizeof(dString), "{MAIN}[DUEL] {FFFFFF}%s created a duel! (Arena: %s) Type {MAIN}/acceptduel %d{FFFFFF} to challenge them.", getPlayerName(playerid), DuelS_ArenaNames[selectedArena], playerid);

	foreach(new i : Player)
	{
		if(User[i][isLogged])
		{
			new formattedMsg[256];
			strcopy(formattedMsg, dString, sizeof(formattedMsg));
			format_MainColor(i, formattedMsg);
			SendClientMessage(i, COLOR_LRED, "%s", formattedMsg);
		}
	}
	return 1;
}

@cmd() duel(playerid, params[], help)
{
	if(!User[playerid][isLogged]) return 1;

	if(isnull(params))
	{
		if(User[playerid][isAfk] || User[playerid][locationId] == ARENA_AFK)
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot create a duel while in the lounge.");

		if(User[playerid][isSpectating])
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot use this while spectating.");

		if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You are already in a duel! Use {MAIN}/backduel{FFFFFF} to leave.");

		if(IsPlayerInDuel2v2(playerid))
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot create a duel while in a 2v2 duel.");

		new Float:Armour, Float:Health, Float:Result;
		GetPlayerHealth(playerid, Health);
		GetPlayerArmour(playerid, Armour);
		Result = (Health + Armour);

		if(Result < 190.0)
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You need at least 190 total HP + Armour to create a duel.");

		new stre[1500] = "ID\tArena Name\tEnvironment\tStatus\n";
		for(new i = 0; i < sizeof(DuelS_ArenaNames); i++)
		{
			new envStr[32], statusStr[32], waitingCount = 0;

			if(DuelS_ArenaInteriors[i] > 0)
			{
				format(envStr, sizeof(envStr), "{418CB0}Interior (%d)", DuelS_ArenaInteriors[i]);
			}
			else
			{
				format(envStr, sizeof(envStr), "{23C552}Exterior");
			}

			foreach(new p : Player)
			{
				if(User[p][isLogged] && GetPVarInt(p, "DuelS_State") == DUELS_STATE_WAITING && DuelS_Data[p][duelArena] == i)
				{
					waitingCount++;
				}
			}

			if(waitingCount > 0)
			{
				format(statusStr, sizeof(statusStr), "{FFFF00}%d Waiting", waitingCount);
			}
			else
			{
				format(statusStr, sizeof(statusStr), "{00FF00}Available");
			}

			strcat(stre, fmat_return("{MAIN}#%02d\t{FFFFFF}%s\t%s\t%s\n", i + 1, DuelS_ArenaNames[i], envStr, statusStr), sizeof(stre));
		}

		new title[64] = "{MAIN}Duel Arenas - Selection";
		format_MainColor(playerid, title);
		format_MainColor(playerid, stre, sizeof(stre));
		ShowPlayerDialog(playerid, DIALOG_DUEL_ID, DIALOG_STYLE_TABLIST_HEADERS, title, stre, "Select", "Cancel");
		return 1;
	}

	new subcmd[32], rest[64];
	sscanf(params, "s[32]S()[64]", subcmd, rest);

	if(!strcmp(subcmd, "accept", true) || !strcmp(subcmd, "go", true))
	{
		new cmdBuf[128];
		format(cmdBuf, sizeof(cmdBuf), "/acceptduel %s", rest);
		Command_ReProcess(playerid, cmdBuf, 0);
		return 1;
	}
	else if(!strcmp(subcmd, "leave", true) || !strcmp(subcmd, "exit", true) || !strcmp(subcmd, "cancel", true) || !strcmp(subcmd, "back", true))
	{
		new cmdBuf[32];
		strcopy(cmdBuf, "/backduel", sizeof(cmdBuf));
		Command_ReProcess(playerid, cmdBuf, 0);
		return 1;
	}
	return 1;
}

@cmd(.alts = {"duelaccept", "goduel"}) acceptduel(playerid, params[], help)
{
	if(!User[playerid][isLogged]) return 1;

	if(User[playerid][isAfk] || User[playerid][locationId] == ARENA_AFK)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot enter a duel while in the lounge.");

	if(User[playerid][isSpectating])
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot use this while spectating.");

	if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You are already in a duel! Use {MAIN}/backduel{FFFFFF} to leave.");

	if(IsPlayerInDuel2v2(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot enter a duel while in a 2v2 duel.");

	new Float:Armour, Float:Health, Float:Result;
	GetPlayerHealth(playerid, Health);
	GetPlayerArmour(playerid, Armour);
	Result = Health + Armour;

	if(Result < 190.0)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You need at least 190 total HP + Armour to enter a duel.");

	new id = -1;
	if(!isnull(params))
	{
		#if defined sscanf
			sscanf(params, "u", id);
		#else
			id = strval(params);
		#endif
	}

	if(id == -1 || !IsPlayerConnected(id))
	{
		new waitingCount = 0, lastWaitingId = -1;
		new pWaitingList[MAX_PLAYERS];

		foreach(new i : Player)
		{
			if(User[i][isLogged] && GetPVarInt(i, "DuelS_State") == DUELS_STATE_WAITING)
			{
				if(i == playerid) continue;
				pWaitingList[waitingCount++] = i;
				lastWaitingId = i;
			}
		}

		if(waitingCount == 0)
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}No open duel found or player is not connected.");
		}
		else if(waitingCount == 1)
		{
			id = lastWaitingId;
		}
		else
		{
			new dialogBody[1000] = "Host Player\tArena Name\tEnvironment\n";
			new targetPVarName[32];
			for(new w = 0; w < waitingCount; w++)
			{
				new targetP = pWaitingList[w];
				new selArena = DuelS_Data[targetP][duelArena];
				new envStr[32];
				if(DuelS_ArenaInteriors[selArena] > 0) format(envStr, sizeof(envStr), "{418CB0}Interior (%d)", DuelS_ArenaInteriors[selArena]);
				else format(envStr, sizeof(envStr), "{23C552}Exterior");

				strcat(dialogBody, fmat_return("{MAIN}%s (ID: %d)\t{FFFFFF}%s\t%s\n",
					getPlayerName(targetP), targetP, DuelS_ArenaNames[selArena], envStr), sizeof(dialogBody));

				format(targetPVarName, sizeof(targetPVarName), "DuelS_AccList_%d", w);
				SetPVarInt(playerid, targetPVarName, targetP);
			}

			SetPVarInt(playerid, "DuelS_AccCount", waitingCount);

			new title[64] = "{MAIN}Open Duels - Challenge Queue";
			format_MainColor(playerid, title);
			format_MainColor(playerid, dialogBody, sizeof(dialogBody));
			ShowPlayerDialog(playerid, DIALOG_DUEL_ACCEPT_ID, DIALOG_STYLE_TABLIST_HEADERS, title, dialogBody, "Accept", "Cancel");
			return 1;
		}
	}

	if(id == -1 || !IsPlayerConnected(id))
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}No open duel found or player is not connected.");

	if(GetPVarInt(id, "DuelS_State") != DUELS_STATE_WAITING)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}Duel has already started or does not exist.");

	if(id == playerid)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You cannot accept your own duel.");

	if(GetPVarInt(playerid, "DuelS_AllowAccept") > 0)
	{
		if(gettime() < GetPVarInt(playerid, "DuelS_AllowAccept"))
			return 1;
		else
			SetPVarInt(playerid, "DuelS_AllowAccept", 0);
	}

	SetPVarInt(id, "DuelS_AllowAccept", gettime() + 3);
	SetPVarInt(playerid, "DuelS_AllowAccept", gettime() + 3);

	SetPVarInt(playerid, "DuelS_State", id);
	SetPVarInt(id, "DuelS_State", playerid);

	new selectedArena = DuelS_Data[id][duelArena];
	SetPlayerVirtualWorld(playerid, id + 10000);

	new aString[180];
	// Dois nomes no mesmo format: buffers locais, nao getPlayerName duplicado.
	new accName[MAX_PLAYER_NAME + 1], oppName[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, accName, sizeof(accName));
	GetPlayerName(id, oppName, sizeof(oppName));

	format(aString, sizeof(aString), "{MAIN}[DUEL] {FFFFFF}%s accepted the duel against %s!", accName, oppName);

	foreach(new i : Player)
	{
		if(User[i][isLogged])
		{
			new formattedMsg[256];
			strcopy(formattedMsg, aString, sizeof(formattedMsg));
			format_MainColor(i, formattedMsg);
			SendClientMessage(i, COLOR_LRED, "%s", formattedMsg);
		}
	}

	SetPlayerPos(playerid, DuelS_Pos[selectedArena][0][0], DuelS_Pos[selectedArena][0][1], DuelS_Pos[selectedArena][0][2]);
	SetPlayerPos(id, DuelS_Pos[selectedArena][1][0], DuelS_Pos[selectedArena][1][1], DuelS_Pos[selectedArena][1][2]);

	SetPlayerFacingAngle(playerid, DuelS_Pos[selectedArena][0][3]);
	SetPlayerFacingAngle(id, DuelS_Pos[selectedArena][1][3]);

	SetPlayerInterior(playerid, DuelS_Data[id][duelInterior]);

	SetPlayerColor(id, 0x0000FFFF);
	SetPlayerColor(playerid, 0xFF0000FF);

	PlayerTextDrawShow(playerid, TDTimerDuelS[playerid]);
	PlayerTextDrawShow(id, TDTimerDuelS[id]);

	PlayerTextDrawSetString(id, TDTimerDuelS[id], "~w~00:00~n~~b~~h~200 ~w~: ~r~~h~200");
	PlayerTextDrawSetString(playerid, TDTimerDuelS[playerid], "~w~00:00~n~~b~~h~200 ~w~: ~r~~h~200");

	DuelS_StartCountdown(id, playerid);
	return 1;
}

@cmd(.alts = {"leaveduel", "exitduel", "cancelduel", "back"}) backduel(playerid, params[], help)
{
	new duelState = GetPVarInt(playerid, "DuelS_State");
	if(duelState == DUELS_STATE_NONE)
		return SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}You are not in a duel.");

	if(duelState == DUELS_STATE_WAITING)
	{
		SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
		PlayerTextDrawHide(playerid, TDTimerDuelS[playerid]);
		if(User[playerid][locationId] != -1 && User[playerid][locationId] != ARENA_AFK)
		{
			User[playerid][isAfk] = false;
			SpawnPlayerEx(playerid);
		}
		else
		{
			setAfk(playerid);
		}
		SendClientMessage(playerid, COLOR_KGREEN, "{MAIN}[DUEL] {FFFFFF}You left the duel queue.");
		return 1;
	}

	new opponent = duelState;
	if(IsPlayerConnected(opponent) && GetPVarInt(opponent, "DuelS_State") == playerid)
	{
		DuelS_End(opponent, playerid, .forfeited = true);
	}
	else
	{
		SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
		PlayerTextDrawHide(playerid, TDTimerDuelS[playerid]);
		if(User[playerid][locationId] != -1 && User[playerid][locationId] != ARENA_AFK)
		{
			User[playerid][isAfk] = false;
			SpawnPlayerEx(playerid);
		}
		else
		{
			setAfk(playerid);
		}
	}
	return 1;
}

forward CancelPlayerDuel(playerid);
public CancelPlayerDuel(playerid)
{
	new cState = GetPVarInt(playerid, "DuelS_State");
	if(cState == DUELS_STATE_NONE) return 0;

	if(cState == DUELS_STATE_WAITING)
	{
		SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
		PlayerTextDrawHide(playerid, TDTimerDuelS[playerid]);
		SendClientMessage(playerid, COLOR_KGREEN, "{MAIN}[DUEL] {FFFFFF}You left the duel queue.");
		return 1;
	}

	new opp = cState;
	if(IsPlayerConnected(opp) && GetPVarInt(opp, "DuelS_State") == playerid)
	{
		DuelS_End(opp, playerid, .forfeited = true);
	}
	else
	{
		SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
		PlayerTextDrawHide(playerid, TDTimerDuelS[playerid]);
	}
	return 1;
}

public OnPlayerConnect(playerid)
{
	SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
	SetPVarInt(playerid, "DuelS_AllowAccept", 0);
	DuelS_CreateTextDraw(playerid);

	#if defined DuelS_OnPlayerConnect
		return DuelS_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect DuelS_OnPlayerConnect
#if defined DuelS_OnPlayerConnect
	forward DuelS_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	new duelState = GetPVarInt(playerid, "DuelS_State");
	if(duelState != DUELS_STATE_NONE)
	{
		if(duelState != DUELS_STATE_WAITING && IsPlayerConnected(duelState))
		{
			DuelS_End(duelState, playerid, .disconnected = true);
		}
	}

	SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
	DuelS_DestroyTextDraw(playerid);

	#if defined DuelS_OnPlayerDisconnect
		return DuelS_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect DuelS_OnPlayerDisconnect
#if defined DuelS_OnPlayerDisconnect
	forward DuelS_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerSpawn(playerid)
{
	if(GetPVarInt(playerid, "DuelS_State") == DUELS_STATE_NONE)
	{
		if(TDTimerDuelS[playerid] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawHide(playerid, TDTimerDuelS[playerid]);
		}
	}

	#if defined DuelS_OnPlayerSpawn
		return DuelS_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn DuelS_OnPlayerSpawn
#if defined DuelS_OnPlayerSpawn
	forward DuelS_OnPlayerSpawn(playerid);
#endif

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	new duelState = GetPVarInt(playerid, "DuelS_State");
	if(duelState != DUELS_STATE_NONE && duelState != DUELS_STATE_WAITING)
	{
		new opponent = duelState;
		if(IsPlayerConnected(opponent) && GetPVarInt(opponent, "DuelS_State") == playerid)
		{
			DuelS_End(opponent, playerid);
		}
		else
		{
			SetPVarInt(playerid, "DuelS_State", DUELS_STATE_NONE);
			PlayerTextDrawHide(playerid, TDTimerDuelS[playerid]);
		}
	}

	#if defined DuelS_OnPlayerDeath
		return DuelS_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath DuelS_OnPlayerDeath
#if defined DuelS_OnPlayerDeath
	forward DuelS_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_DUEL_ID)
	{
		if(response)
		{
			if(listitem >= 0 && listitem < sizeof(DuelS_ArenaNames))
			{
				DuelS_Data[playerid][duelArena] = listitem;
				DuelS_Data[playerid][duelInterior] = DuelS_ArenaInteriors[listitem];
				DuelS_Host(playerid);
			}
		}
		return 1;
	}

	if(dialogid == DIALOG_DUEL_ACCEPT_ID)
	{
		new count = GetPVarInt(playerid, "DuelS_AccCount");
		if(response)
		{
			if(listitem >= 0 && listitem < count)
			{
				new pVarName[32];
				format(pVarName, sizeof(pVarName), "DuelS_AccList_%d", listitem);
				new targetP = GetPVarInt(playerid, pVarName);

				for(new w = 0; w < count; w++)
				{
					format(pVarName, sizeof(pVarName), "DuelS_AccList_%d", w);
					DeletePVar(playerid, pVarName);
				}
				DeletePVar(playerid, "DuelS_AccCount");

				if(IsPlayerConnected(targetP) && GetPVarInt(targetP, "DuelS_State") == DUELS_STATE_WAITING)
				{
					new cmdBuf[64];
					format(cmdBuf, sizeof(cmdBuf), "/acceptduel %d", targetP);
					Command_ReProcess(playerid, cmdBuf, 0);
				}
				else
				{
					SendClientMessage(playerid, COLOR_TOMATO, "{MAIN}[ERROR] {FFFFFF}That duel is no longer available.");
				}
			}
		}
		else
		{
			for(new w = 0; w < count; w++)
			{
				new pVarName[32];
				format(pVarName, sizeof(pVarName), "DuelS_AccList_%d", w);
				DeletePVar(playerid, pVarName);
			}
			DeletePVar(playerid, "DuelS_AccCount");
		}
		return 1;
	}

	#if defined DuelS_OnDialogResponse
		return DuelS_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse DuelS_OnDialogResponse
#if defined DuelS_OnDialogResponse
	forward DuelS_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
#endif


#define MAX_PLAYERS 88

#include <a_samp>

enum e_Player
{
	playerVersion[24],
	updateCount,
	bool:ResponseType[2]
}

new ePlayer[MAX_PLAYERS][e_Player];

public OnFilterScriptInit()
{
	for (new i; i < MAX_PLAYERS; i ++) // prevents all players from being kicked on reload
	{
		ePlayer[i][updateCount] = -1;
	}
}

public OnPlayerConnect(playerid)
{
	ePlayer[playerid][updateCount] = -1;
	ePlayer[playerid][ResponseType] = bool:{false, false};

	GetPlayerVersion(playerid, ePlayer[playerid][playerVersion]);

	if (!strequal(ePlayer[playerid][playerVersion], "0.3.7"))
	{
		ePlayer[playerid][updateCount] = 0;
		SendClientCheck(playerid, 5, 0x400000, 0, 4);
		SendClientCheck(playerid, 69, 0x0, 0, 4);
	}

	SendClientCheck(playerid, 72, 0x0, 0, 2);

	return 1;
}

public OnPlayerUpdate(playerid)
{
	if (ePlayer[playerid][updateCount] != -1 && ++ePlayer[playerid][updateCount] == 10) // 10 (6 og) number of sync packets sent by the player without a client check response.
	{
		if (!CallRemoteFunction("IsR2KickEnabled", ""))
			return 1;

		SendClientMessage(playerid, 0xB81A1AFF, "[FALSE R2] You are using a fake version of R2+. Remove it.");
		return KickEx(playerid);
	}

	return 1;
}

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	if (actionid == 5 || actionid == 69)
	{
		ePlayer[playerid][ResponseType][actionid == 69] = true;

		if (ePlayer[playerid][ResponseType][0] && ePlayer[playerid][ResponseType][1])
		{
			ePlayer[playerid][updateCount] = -1;
		}
	}
	else if (actionid == 72)
	{
		SetPVarInt(playerid, "NotAndroid", 1);
	}

	return true;
}

KickEx(playerid)
{
	SetTimerEx("KickPlayerEx", 1500, false, "i", playerid);
	return true;
}

forward KickPlayerEx(playerid);
public KickPlayerEx(playerid)
{
	Kick(playerid);
	return true;
}

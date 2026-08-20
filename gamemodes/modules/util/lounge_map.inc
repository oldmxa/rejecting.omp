#if defined _lounge_map_included
    #endinput
#endif
#define _lounge_map_included

public OnGameModeInit()
{
    CreateLoungeMap();

    #if defined LMAP_OnGameModeInit
        return LMAP_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit LMAP_OnGameModeInit
#if defined LMAP_OnGameModeInit
    forward LMAP_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
    if(!IsPlayerNPC(playerid))
    {
        RemoveObjectsLounge(playerid);
    }

    #if defined LMAP_OnPlayerConnect
        return LMAP_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect LMAP_OnPlayerConnect
#if defined LMAP_OnPlayerConnect
    forward LMAP_OnPlayerConnect(playerid);
#endif

CreateLoungeMap()
{
}

RemoveObjectsLounge(playerid)
{
    RemoveBuildingForPlayer(playerid, 1441, -822.6953, 490.8984, 1358.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 1441, -824.8438, 491.8047, 1358.3047, 0.25);
	RemoveBuildingForPlayer(playerid, 1438, -822.0156, 492.5625, 1357.8281, 0.25);
	RemoveBuildingForPlayer(playerid, 1441, -820.1328, 490.9531, 1358.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, -828.9922, 496.0000, 1357.9766, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, -827.6328, 494.7344, 1358.0938, 0.25);
	RemoveBuildingForPlayer(playerid, 1415, -826.2813, 495.1172, 1357.8594, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, -826.6953, 496.7734, 1358.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1372, -827.2656, 498.1953, 1357.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 1372, -829.0313, 498.1953, 1357.5938, 0.25);
	RemoveBuildingForPlayer(playerid, 3903, -815.6797, 534.2422, 1359.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3948, -834.0234, 526.4766, 1363.7188, 0.25);
	RemoveBuildingForPlayer(playerid, 1438, -814.2656, 493.3281, 1358.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -813.8047, 495.0000, 1359.0547, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -815.3438, 495.8281, 1359.0000, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -813.8047, 495.9141, 1359.1172, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, -814.1641, 497.2734, 1359.1250, 0.25);
	RemoveBuildingForPlayer(playerid, 944, -811.2578, 492.5547, 1359.4219, 0.25);
	RemoveBuildingForPlayer(playerid, 944, -811.2578, 494.2031, 1359.5313, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -811.5625, 496.9531, 1359.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, -810.4063, 497.4219, 1359.4297, 0.25);
	RemoveBuildingForPlayer(playerid, 1438, -818.0000, 507.1406, 1358.5703, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -820.1328, 508.2734, 1358.8828, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -818.7188, 508.6953, 1359.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 1431, -816.5625, 508.6953, 1359.2500, 0.25);
	RemoveBuildingForPlayer(playerid, 1338, -815.7188, 507.6797, 1359.4375, 0.25);
	RemoveBuildingForPlayer(playerid, 933, -826.8750, 510.4844, 1357.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 933, -825.0547, 509.7891, 1357.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 923, -838.2734, 511.6250, 1358.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 1430, -800.9453, 500.8984, 1360.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 1430, -800.9453, 501.7422, 1360.0313, 0.25);

	// Remove buildings for garbage dump
	RemoveBuildingForPlayer(playerid, 5357, 2177.9922, -2006.7578, 23.2891, 0.25);
	RemoveBuildingForPlayer(playerid, 5291, 2177.9922, -2006.7578, 23.2891, 0.25);

    return true;
}

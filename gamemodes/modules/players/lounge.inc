
stock SetupPlayerVehicle(vehicleid)
{
    if (vehicleid != INVALID_VEHICLE_ID && vehicleid != 999)
    {
        SetVehicleNumberPlate(vehicleid, "oldmxa");
        new engine, lights, alarm, doors, bonnet, boot, objective;
        GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
    }
    return 1;
}

forward Lounge_CanSpawnLoungeVehicle(playerid);
public Lounge_CanSpawnLoungeVehicle(playerid)
{
    if (User[playerid][isLogged] == false)
    {
        SendClientMessage(playerid, -1, "%s", CANNOT_DO_THIS);
        return 0;
    }

    if (User[playerid][isAfk] == false)
    {
        SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);
        return 0;
    }

    if (GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle during a duel.");
        return 0;
    }

    if (IsPlayerInDuel2v2(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle during a 2v2 duel.");
        return 0;
    }

    if (IsPlayerInTournamentMatch(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle during a tournament match.");
        return 0;
    }

    if (g_PlayerTraining[playerid][pTrain_Active])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle in training.");
        return 0;
    }

    if (IsPracticeActive(playerid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle in practice.");
        return 0;
    }

    if (User[playerid][isFrozen])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle right now.");
        return 0;
    }

    if (playerSpecID[playerid] != INVALID_PLAYER_ID || User[playerid][isSpectating])
    {
        SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);
        return 0;
    }

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if (pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot spawn a vehicle right now.");
        return 0;
    }

    return 1;
}

@cmd(.alts = {"b"}) bike(playerid, params[], help)
{
    if (!CanSpawnLoungeVehicle(playerid))
        return true;

    if (pVehicle[playerid] != 999)
        DestroyVehicle(pVehicle[playerid]);

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    pVehicle[playerid] = CreateVehicle(481, x, y, z, 0, random(100), random(100), -1);
    SetupPlayerVehicle(pVehicle[playerid]);

    SetVehicleVirtualWorld(pVehicle[playerid], GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(pVehicle[playerid], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, pVehicle[playerid], 0);

    return true;
}

forward Lounge_SpawnLoungeVehicle(playerid, vehicle, color1, color2);
public Lounge_SpawnLoungeVehicle(playerid, vehicle, color1, color2)
{
    if (vehicle < VEHICLE_ID_MIN || vehicle > VEHICLE_ID_MAX)
        return SendClientMessage(playerid, -1, "%s", INVALID_VEHICLE_MSG);

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (pVehicle[playerid] != 999)
        DestroyVehicle(pVehicle[playerid]);

    pVehicle[playerid] = CreateVehicle(vehicle, x, y, z, 0, color1, color2, -1);
    SetupPlayerVehicle(pVehicle[playerid]);
    SetVehicleVirtualWorld(pVehicle[playerid], GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(pVehicle[playerid], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, pVehicle[playerid], 0);

    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}When you exit the vehicle it will be automatically deleted!");

    va_SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}You created the vehicle {FFFFFF}'{0CB80C}%s{FFFFFF}' {13A9F6}ID {FFFFFF}'{0CB80C}%d{FFFFFF}' !", VehicleNames[vehicle - VEHICLE_ID_MIN], vehicle);

    return true;
}

@cmd() car(playerid, params[], help)
{
    if (!CanSpawnLoungeVehicle(playerid))
        return true;

    if (IsNull(params))
        return ShowVehicleDialog(playerid);

    new vehicle, vehiclestr[30], color1 = -1, color2 = -1;
    if (sscanf(params, "s[30]D(-1)D(-1)", vehiclestr, color1, color2))
        return SendClientMessage(playerid, -1, "%s", VEHICLE_SPAWN_HELP);

    if (IsNumeric(vehiclestr))
    {
        vehicle = strval(vehiclestr);
    }
    else
    {
        vehicle = GetVehicleModelIDFromName(vehiclestr);
    }

    return SpawnLoungeVehicle(playerid, vehicle, color1, color2);
}

@cmd() getgun(playerid, params[], help)
{
    if(User[playerid][isAfk] == false)
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use /getgun during a duel.");

    if(g_PlayerTraining[playerid][pTrain_Active] || IsPracticeActive(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use /getgun in training or practice.");

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if(pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use /getgun right now.");

    new weaponStr[18], weapid,Ammo;

    if(sscanf(params, "s[18]d", weaponStr, Ammo))
        return SendClientMessage(playerid, -1, "%s", GETGUN_HELP);
    weapid = GetWeaponModelIDFromName(weaponStr);
    if(weapid == -1)
        return SendClientMessage(playerid, -1, "%s", GETGUN_HELP);
    if(Ammo < 0 || Ammo > 9999)
        return SendClientMessage(playerid, -1, "%s", INVALID_AMMO_MSG);

    if(weapid < 1 || weapid > 46 || weapid == 19 || weapid == 20 || weapid == 21 || weapid == 22)
        return SendClientMessage(playerid, -1, "%s", INVALID_ID_MSG);
    if(weapid == 44 || weapid == 45)
        return SendClientMessage(playerid, -1, "%s", RETARD);

    GivePlayerWeapon(playerid, WEAPON:weapid, Ammo);

    return true;
}

@cmd(.alts = {"l", "afk"}) lounge(playerid, params[], help)
{
    new String[150];
    new Float:Armour, Float:Health;
    GetPlayerArmour(playerid, Armour);
    GetPlayerHealth(playerid, Health);

    if (floatround(Health + Armour) < 200)
        return va_SendClientMessage(playerid, -1, HEALTH_ERROR_MSG, 200);

    new PLAYER_STATE:playerState = GetPlayerState(playerid);
    if(playerState == PLAYER_STATE_SPECTATING || playerSpecID[playerid] != INVALID_PLAYER_ID)
        return SendClientMessage(playerid, -1, "%s", IN_SPEC_MODE);
    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE)
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL] {FFFFFF}You cannot use /lounge while in a duel.");
    if (IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[DUEL 2v2] {FFFFFF}You cannot use /lounge while in a 2v2 duel. Use /cancel2v2 first.");

    if (IsPlayerInTournamentMatch(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[TOURNAMENT] {FFFFFF}You cannot use /lounge while playing a tournament match.");

    if (User[playerid][isAfk])
    {
        if(User[playerid][locationId] == -1)
        {
            return SendClientMessage(playerid, -1, "{MAIN}** {FFFFFF}You need to enter an arena first using /changemode.");
        }

        if(User[playerid][lastLocation] < 0 || User[playerid][lastLocation] >= MAX_LOCATIONS || User[playerid][lastLocation] == ARENA_AFK)
        {
            return SendClientMessage(playerid, -1, "{MAIN}** {FFFFFF}You need to enter an arena first.");
        }
        else
        {
            new oldLocation = User[playerid][locationId];

            User[playerid][isAfk] = false;
            User[playerid][locationId] = User[playerid][lastLocation];

            ApplyArenaRpWalk(playerid, oldLocation, User[playerid][locationId]);
        }

        SpawnPlayerEx(playerid);
        SendClientMessage(playerid, -1, "%s", GetRandomLeaveLoungeMsg());
    }
    else
    {
        new coloredNick[MAX_PLAYER_NAME + 18];
        format(coloredNick, sizeof(coloredNick), "{%06x}%s{FFFFFF}", GetPlayerDisplayColour(playerid), getPlayerName(playerid));
        format(String, sizeof(String), GetRandomEnterLoungeMsg(), coloredNick);

        SendClientMessageToAll(-1, "%s", String);
        SendClientMessage(playerid, -1, "%s", AFK_MSG_2);
        SendClientMessage(playerid, -1, "%s", AFK_MSG_3);
        SendClientMessage(playerid, -1, "%s", AFK_MSG_4);

        setAfk(playerid);
        updatePlayerHud(playerid);
    }
    return true;
}

new const LoungeCmdsList[][2][] = {
    {"/car", "Spawn a vehicle"},
    {"/bike", "Spawn a bike"},
    {"/jetpack", "Get a jetpack"},
    {"/getgun", "Get weapons"},
    {"/tune", "Tune your current vehicle"},
    {"/fix", "Repair your current vehicle"},
    {"/flip", "Flip your vehicle back"},
    {"/spec", "Spectate a player"},
    {"/specoff", "Stop spectating"},
    {"/lounge", "Exit from lounge room"},
    {"/changelounge", "Change your lounge spawn location"}
};

ShowLoungeCommands(playerid, page)
{
    new cmdsstr[1024], title[64];

    format(title, sizeof(title), "{MAIN}Lounge Commands - Page %d", page + 1);
    format_MainColor(playerid, title);

    format(cmdsstr, sizeof(cmdsstr), "{MAIN}Command\t{MAIN}Description\n");

    new start = page * 10;
    new end = start + 10;
    if(end > sizeof(LoungeCmdsList)) end = sizeof(LoungeCmdsList);

    for(new i = start; i < end; i++)
    {
        strcat(cmdsstr, "{BEBEBE}");
        strcat(cmdsstr, LoungeCmdsList[i][0]);
        strcat(cmdsstr, "\t"COL_WHITE"");
        strcat(cmdsstr, LoungeCmdsList[i][1]);
        strcat(cmdsstr, "\n");
    }

    if(end < sizeof(LoungeCmdsList)) strcat(cmdsstr, ""COL_KGREEN">>> Next Page\tGo next\n");

    format_MainColor(playerid, cmdsstr, sizeof(cmdsstr));

    SetPVarInt(playerid, "LcmdsPage", page);
    Dialog_ShowCallback(playerid, using public DIALOG_LCMDS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, cmdsstr, "Select", (page > 0) ? ("<") : ("Close"));
    return 1;
}

forward DIALOG_LCMDS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_LCMDS(playerid, dialogid, response, listitem, string:inputtext[])
{
    new page = GetPVarInt(playerid, "LcmdsPage");

    if(!response)
    {
        if(page > 0)
            return ShowLoungeCommands(playerid, page - 1);
        return 1;
    }

    if(strfind(inputtext, "Next Page") != -1)
        return ShowLoungeCommands(playerid, page + 1);

    return 1;
}

@cmd() lcmds(playerid, params[], help)
{
    if(!User[playerid][isAfk])
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    ShowLoungeCommands(playerid, 0);
    return true;
}

@cmd() fix(playerid, params[], help)
{
    if(!User[playerid][isAfk])
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be in a vehicle.");

    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be the driver.");

    new vehicleid = GetPlayerVehicleID(playerid);
    RepairVehicle(vehicleid);
    SetVehicleHealth(vehicleid, 1000.0);
    
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle repaired!");
    return true;
}

@cmd(.alts = {"msp"}) changelounge(playerid, params[], help)
{
    if(!User[playerid][isAfk])
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot change lounge during a duel.");

    if(g_PlayerTraining[playerid][pTrain_Active] || IsPracticeActive(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot change lounge inside training or practice.");

    if(g_PlayerInSafehouse[playerid] != -1)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot change lounge inside a safehouse.");

    if(User[playerid][spawnSafehouse])
    {
        if(GetPlayerSafehouse(playerid) == -1)
        {
            User[playerid][spawnSafehouse] = false;
            UpdatePlayerData(playerid);
        }
        else
        {
            SendClientMessage(playerid, -1, "{MAIN}[LNG] {FFFFFF}You spawn at your clan safehouse, so a custom lounge would be ignored.");
            return SendClientMessage(playerid, -1, "{MAIN}[LNG] {FFFFFF}Disable {MAIN}Safehouse Spawn{FFFFFF} in {MAIN}/clan{FFFFFF} first.");
        }
    }

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if(pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot change lounge right now.");

    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    User[playerid][loungeX] = x;
    User[playerid][loungeY] = y;
    User[playerid][loungeZ] = z;
    User[playerid][loungeA] = a;
    User[playerid][loungeVW] = GetPlayerVirtualWorld(playerid);
    User[playerid][loungeINT] = GetPlayerInterior(playerid);

    UpdatePlayerData(playerid);
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Your lounge spawn location has been updated!");
    return true;
}

@cmd() jetpack(playerid, params[], help)
{
    if(!User[playerid][isAfk] && User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot get a jetpack during a duel.");

    if(g_PlayerTraining[playerid][pTrain_Active] || IsPracticeActive(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot get a jetpack in training or practice.");

    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot get a jetpack while in a vehicle.");

    new PLAYER_STATE:pState = GetPlayerState(playerid);
    if(pState == PLAYER_STATE_NONE || pState == PLAYER_STATE_WASTED || pState == PLAYER_STATE_SPECTATING)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot get a jetpack right now.");

    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}You received a jetpack.");
    return true;
}

@cmd() flip(playerid, params[], help)
{
    if(!User[playerid][isAfk] && User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot flip a vehicle during a duel.");

    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are not in a vehicle.");

    new vehicleid = GetPlayerVehicleID(playerid);
    new Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(vehicleid, x, y, z);
    GetVehicleZAngle(vehicleid, a);
    SetVehiclePos(vehicleid, x, y, z + 2.0);
    SetVehicleZAngle(vehicleid, a);

    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle flipped.");
    return true;
}

@cmd() tune(playerid, params[], help)
{
    if(!User[playerid][isAfk] && User[playerid][adminLevel] < 1)
        return SendClientMessage(playerid, -1, "%s", NEED_BE_AFK);

    if(GetPVarInt(playerid, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot tune a vehicle during a duel.");

    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You are not in a vehicle.");

    new vehicleid = GetPlayerVehicleID(playerid);
    AddVehicleComponent(vehicleid, 1010);
    AddVehicleComponent(vehicleid, 1087);
    
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle tuned (Nitro + Hydraulics).");
    return true;
}

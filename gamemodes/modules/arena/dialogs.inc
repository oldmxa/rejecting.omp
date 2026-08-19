
forward Location(playerid, dialogid, response, listitem, string:inputtext[]);
public Location(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        if(User[playerid][locationId] == -1)
        {
            User[playerid][locationId] = ARENA_AFK;
            User[playerid][lastLocation] = ARENA_AFK;
            setAfk(playerid);
            SpawnPlayerEx(playerid);
            SendClientMessage(playerid, -1, "{MAIN}** {FFFFFF}You cancelled the selection and were sent to the Lounge.");
            return true;
        }
		SetPlayerVirtualWorld(playerid, User[playerid][locationId]*99);
        return false;
    }

    if(listitem < 0 || listitem >= sizeof(LOCATION_DISPLAY_ORDER))
        return true;

    listitem = LOCATION_DISPLAY_ORDER[listitem];

    if(listitem == User[playerid][locationId])
    {
        SendClientMessage(playerid, COLOR_ABACATE, "{MAIN}[ERROR] {FFFFFF}You're already in this location!");
        return true;
    }

    if(getLocationPlayers(listitem) >= arenaInfo[listitem][maxPlayers])
    {
        SendClientMessage(playerid, 0xa4aba7ff, "%s", NOT_EMPTY_SPACES);
        showLocationSelectDialog(playerid);
        return true;
    }

    if(User[playerid][isCreatingArena])
        return SendClientMessage(playerid, -1, "%s", IN_ARENA_EDITOR);

    if(IsR2OnlyArena(listitem) && !IsPlayerR2Client(playerid))
    {
        SendClientMessage(playerid, -1, "%s", ARENA_R2_REQUIRED_ERROR);
        showLocationSelectDialog(playerid);
        return true;
    }

    if(ArenaRequiresFpsLock(listitem) && User[playerid][FPS] != ARENA_FPS_LOCK_VALUE)
    {
        va_SendClientMessage(playerid, -1, ARENA_FPS_LOCK_ERROR, ARENA_FPS_LOCK_VALUE, User[playerid][FPS]);
        showLocationSelectDialog(playerid);
        return true;
    }

    new oldLocation = User[playerid][lastLocation];

    User[playerid][locationId] = listitem;
    User[playerid][lastLocation] = listitem;

    if (listitem != ARENA_AFK)
    {
        new bool:hasWeapons = false;
        for(new i = 0; i < 4; i++)
        {
            if(User[playerid][playerWeapons][i] != 0)
            {
                hasWeapons = true;
                break;
            }
        }

        if(!hasWeapons)
        {
            SetPVarInt(playerid, "showGunmenuFirst", 1);
        }

        User[playerid][isAfk] = false;

        new zoneid = arenaInfo[User[playerid][locationId]][arenaId];
        // GangZonePulseForPlayer(playerid, e_ZONE_PULSE_ZONE, zoneid, ZoneColour[zoneid] | 101, ZoneColour[zoneid], 1000);
        ZoneNumberFlashForPlayer(playerid, zoneid, 0xCC0000FF);
    }

    new selectMessage[64];
    format(selectMessage, sizeof(selectMessage), JOINED_ROOM_MSG, GetLocationDisplaySlot(listitem) + 1);
    SendClientMessage(playerid, 0xa4aba7ff, "%s", selectMessage);

    playerArenaInfo[listitem][playerid][firedShots] = 0;
    playerArenaInfo[listitem][playerid][hitedShots] = 0;
    SetPVarInt(playerid, "CurrentShotId", 0);
    SetPVarInt(playerid, "LastHitedShotId", 0);

    ApplyArenaRpWalk(playerid, oldLocation, listitem);

    SpawnPlayerEx(playerid);

    if(listitem == ARENA_AFK)
        setAfk(playerid);

    return true;
}
ShowNewArenaNameDialog(playerid)
{
    return Dialog_ShowCallback(playerid, using public NewArenaName<iiiis>, DIALOG_STYLE_INPUT, "New arena - name", "Give your suggestion for the name of the new arena.", "Save", "Cancel");
}

forward NewArena(playerid, dialogid, response, listitem, string:inputtext[]);
public NewArena(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (response)
    {
        ShowNewArenaNameDialog(playerid);
    }
    else
    {
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);

        SendClientMessage(playerid, -1, "%s", NEW_ARENA_ANOTHER);
        ClearPlayerNewArenaData(playerid);
    }

    return true;
}

forward NewArenaName(playerid, dialogid, response, listitem, string:inputtext[]);
public NewArenaName(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (response)
    {
        if (isnull(inputtext))
        {
            SendClientMessage(playerid, -1, "%s", NEW_ARENA_EMPTY_NAME);
            return ShowNewArenaNameDialog(playerid);
        }
        if (strlen(inputtext) > 30)
        {
            SendClientMessage(playerid, -1, "%s", NEW_ARENA_NAME_TOO_LONG);
            return ShowNewArenaNameDialog(playerid);
        }

        mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
            "INSERT INTO `new_arena` (`authorid`, `name`, `minx`, `miny`, `maxx`, `maxy`) VALUES (%d, '%e', %f, %f, %f, %f)",
            User[playerid][user_sqlid], inputtext, User[playerid][newArenaPos][0], User[playerid][newArenaPos][1], User[playerid][newArenaPos][2], User[playerid][newArenaPos][3]);
        mysql_tquery(dbHandle, queryGlobal);

        SendClientMessage(playerid, -1, "%s", NEW_ARENA_SAVED);
    }

    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);

    SendClientMessage(playerid, -1, "%s", NEW_ARENA_ANOTHER);
    return ClearPlayerNewArenaData(playerid);
}

ShowCustomMapListDialog(playerid)
{
    new dialogStr[768];
    strcat(dialogStr, "ID\tArena Name\tArea\n");

    for(new i; i < MAX_ZONES; i++)
    {
        if(!IsCustomArena(i)) continue;

        new Float:arenaarea = (arenaData[i][arenaPos][0] - arenaData[i][arenaPos][2]) * (arenaData[i][arenaPos][1] - arenaData[i][arenaPos][3]);
        if(arenaarea < 0) arenaarea *= -1;

        strcat(dialogStr, sprintf("%d\t%s%s\t%0.1fm\n",
            i,
            (arenaInfo[ARENA_CUSTOM][arenaId] == i) ? ("{23C552}") : ("{FFFFFF}"),
            arenaData[i][arenaName],
            arenaarea));
    }

    Dialog_ShowCallback(playerid, using public DIALOG_CUSTOM_MAPLIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "SELECT CUSTOM MAP", dialogStr, "Select", "Close");
    return 1;
}

forward DIALOG_CUSTOM_MAPLIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_CUSTOM_MAPLIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return true;

    if(User[playerid][adminLevel] < 2)
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    if(User[playerid][locationId] != ARENA_CUSTOM)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be in the Custom Room to use this command.");

    if(changeArenaDelay > gettime() && User[playerid][adminLevel] < 4)
        return va_SendClientMessage(playerid, -1, CHANGEARENA_DELAY, convertTime(changeArenaDelay - gettime()));

    new zone = GetCustomArenaByIndex(listitem);
    if(zone == -1)
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid custom arena.");

    return ApplyArenaChange(playerid, ARENA_CUSTOM, zone);
}

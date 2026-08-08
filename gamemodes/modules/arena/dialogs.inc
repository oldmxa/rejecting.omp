
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

    if(listitem < 0 || listitem >= MAX_LOCATIONS)
        return true;

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
    format(selectMessage, sizeof(selectMessage), JOINED_ROOM_MSG, listitem + 1);
    SendClientMessage(playerid, 0xa4aba7ff, "%s", selectMessage);

    if(User[playerid][locationId] != ARENA_AFK)
        SetPlayerVirtualWorld(playerid, playerid*50);

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


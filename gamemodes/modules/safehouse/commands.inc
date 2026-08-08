#if defined _safehouse_commands_included
    #endinput
#endif
#define _safehouse_commands_included

forward DIALOG_SAFE_EDIT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_EDIT_VALUE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_EDIT_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_ADMIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_CONFIG(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_CONFIG_SET(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_FREEZONES(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_DELETE(playerid, dialogid, response, listitem, string:inputtext[]);

enum
{
    SAFE_EDIT_EXTERIOR,
    SAFE_EDIT_SPAWN,
    SAFE_EDIT_EXIT,
    SAFE_EDIT_INTERIOR,
    SAFE_EDIT_VW,
    SAFE_EDIT_GOTO,
    SAFE_EDIT_PICKUP,
    SAFE_EDIT_DELETE
}

stock IsSafehouseAdmin(playerid)
{
    return (User[playerid][adminLevel] >= 5 || IsPlayerAdmin(playerid)) ? 1 : 0;
}

stock ShowSafehouseAdminMenu(playerid)
{
    if (!IsSafehouseAdmin(playerid)) return 0;

    new title[64] = "{MAIN}Safehouse System";
    format_MainColor(playerid, title);

    new placed = 0, owned = 0, eligible = 0, created = 0;
    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (!IsSafehouseZone(i)) continue;

        eligible++;
        if (!SafehouseExists(i)) continue;

        created++;
        if (SafehouseData[i][sh_ExtZ] != 0.0) placed++;
        if (SafehouseData[i][sh_Status] == SAFEHOUSE_STATUS_OWNED) owned++;
    }

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new here = GetSafehouseZoneAtPosition(px, py);
    new hereLabel[96];

    if (here == -1)
        format(hereLabel, sizeof(hereLabel), "{F3230E}Not inside a medium or large zone");
    else if (SafehouseExists(here))
        format(hereLabel, sizeof(hereLabel), "{F3230E}%s already has one", arenaData[here][arenaName]);
    else
        format(hereLabel, sizeof(hereLabel), "{23C552}Create in %s", arenaData[here][arenaName]);

    new buf[640];
    format(buf, sizeof(buf),
        "{MAIN}Option\t{MAIN}Status\n\
{FFFFFF}Create safehouse here\t%s\n\
{FFFFFF}Safehouses\t{BEBEBE}%d created, %d placed, %d owned\n\
{FFFFFF}System settings\t{BEBEBE}Prices, durations and view distances\n\
{FFFFFF}Rebuild all\t{BEBEBE}Recreate every pickup, icon and label\n\
{BEBEBE}Free zones\t{BEBEBE}%d of %d still without a safehouse\n",
        hereLabel, created, placed, owned, eligible - created, eligible);

    format_MainColor(playerid, buf);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_ADMIN<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Select", "Close");
}

public DIALOG_SAFE_ADMIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    if (!IsSafehouseAdmin(playerid)) return 1;

    switch (listitem)
    {
        case 0: return CreateSafehouseHere(playerid);
        case 1: return ShowSafehouseEditList(playerid);
        case 2: return ShowSafehouseConfigMenu(playerid);
        case 3:
        {
            new count = RefreshAllSafehousePickups();
            va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Rebuilt %d safehouse(s).", count);
            return ShowSafehouseAdminMenu(playerid);
        }
        case 4: return ShowSafehouseFreeZones(playerid);
    }
    return 1;
}

stock CreateSafehouseHere(playerid)
{
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    new index = GetSafehouseZoneAtPosition(x, y);

    if (index == -1)
    {
        SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You are not inside a medium or large arena zone.");
        return ShowSafehouseAdminMenu(playerid);
    }

    if (SafehouseExists(index))
    {
        va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"%s already has a safehouse. Edit it instead.", arenaData[index][arenaName]);
        return ShowSafehouseAdminMenu(playerid);
    }

    ResetSafehouse(index);

    SafehouseData[index][sh_ExtX] = x;
    SafehouseData[index][sh_ExtY] = y;
    SafehouseData[index][sh_ExtZ] = z;
    SafehouseData[index][sh_ExitX] = x;
    SafehouseData[index][sh_ExitY] = y;
    SafehouseData[index][sh_ExitZ] = z;

    InsertSafehouseRow(index);

    va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Safehouse created in {MAIN}%s{FFFFFF} at your position.", arenaData[index][arenaName]);
    SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Interior and exit use the level defaults. Adjust them in the edit menu if needed.");

    g_PlayerSafehouseEdit[playerid] = index;
    LogSafehouse(index, playerid, "admin_create", arenaData[index][arenaName]);
    return 1;
}

stock ShowSafehouseFreeZones(playerid)
{
    new title[64] = "{MAIN}Zones Without Safehouse";
    format_MainColor(playerid, title);

    new buf[128 + (48 * MAX_SAFEHOUSES)];
    format(buf, sizeof(buf), "{MAIN}Zone\t{MAIN}Size\n");

    new found = 0;
    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (!IsSafehouseZone(i) || SafehouseExists(i)) continue;

        new line[96];
        format(line, sizeof(line), "#%02d %s\t{BEBEBE}%s\n",
            i, arenaData[i][arenaName],
            (arenaData[i][category] == ARENA_CATEGORY_BIG) ? ("Large") : ("Medium"));
        strcat(buf, line);
        found++;
    }

    if (!found) strcat(buf, "{23C552}Every eligible zone already has a safehouse\t \n");

    format_MainColor(playerid, buf);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_FREEZONES<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Back", "Close");
}

public DIALOG_SAFE_FREEZONES(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    return ShowSafehouseAdminMenu(playerid);
}

stock ShowSafehouseConfigMenu(playerid)
{
    new title[64] = "{MAIN}Safehouse Settings";
    format_MainColor(playerid, title);

    new buf[128 + (96 * SHCFG_MAX)];
    format(buf, sizeof(buf), "{MAIN}Setting\t{MAIN}Value\t{MAIN}Range\n");

    for (new i = 0; i < SHCFG_MAX; i++)
    {
        new line[128];
        format(line, sizeof(line), "{FFFFFF}%s\t{23C552}%d\t{BEBEBE}%d - %d\n",
            g_SafehouseConfigLabel[i], g_SafehouseConfig[i],
            g_SafehouseConfigMin[i], g_SafehouseConfigMax[i]);
        strcat(buf, line);
    }

    format_MainColor(playerid, buf);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_CONFIG<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Change", "Back");
}

public DIALOG_SAFE_CONFIG(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseAdminMenu(playerid);
    if (!IsSafehouseAdmin(playerid)) return 1;

    if (listitem < 0 || listitem >= SHCFG_MAX)
        return ShowSafehouseConfigMenu(playerid);

    g_PlayerSafehouseCfg[playerid] = listitem;

    new title[80];
    format(title, sizeof(title), "{MAIN}%s", g_SafehouseConfigLabel[listitem]);
    format_MainColor(playerid, title);

    new body[256];
    format(body, sizeof(body),
        "{FFFFFF}Current value: {23C552}%d\n{FFFFFF}Allowed range: {BEBEBE}%d to %d\n\n\
{FFFFFF}Enter the new value:",
        g_SafehouseConfig[listitem], g_SafehouseConfigMin[listitem], g_SafehouseConfigMax[listitem]);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_CONFIG_SET<iiiis>,
        DIALOG_STYLE_INPUT, title, body, "Save", "Back");
}

public DIALOG_SAFE_CONFIG_SET(playerid, dialogid, response, listitem, string:inputtext[])
{
    new setting = g_PlayerSafehouseCfg[playerid];

    if (!response) return ShowSafehouseConfigMenu(playerid);
    if (!IsSafehouseAdmin(playerid)) return 1;
    if (setting < 0 || setting >= SHCFG_MAX) return ShowSafehouseConfigMenu(playerid);

    new value = ClampSafehouseConfig(setting, strval(inputtext));
    new previous = g_SafehouseConfig[setting];

    g_SafehouseConfig[setting] = value;
    SaveSafehouseConfig(setting);

    if (setting == SHCFG_ICON_DISTANCE || setting == SHCFG_LABEL_DISTANCE)
        RefreshAllSafehousePickups();

    va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"%s changed from {BEBEBE}%d{FFFFFF} to {23C552}%d{FFFFFF}.",
        g_SafehouseConfigLabel[setting], previous, value);

    return ShowSafehouseConfigMenu(playerid);
}

stock ShowSafehouseEditList(playerid)
{
    new title[64] = "{MAIN}Safehouses";
    format_MainColor(playerid, title);

    new buf[128 + (64 * MAX_SAFEHOUSES)];
    format(buf, sizeof(buf), "{MAIN}Zone\t{MAIN}Owner\t{MAIN}Placed\n");

    g_SafehouseEditCount[playerid] = 0;

    for (new i = 0; i < MAX_SAFEHOUSES; i++)
    {
        if (!IsSafehouseZone(i) || !SafehouseExists(i)) continue;

        new owner[MAX_CLAN_TAG + 12] = "{BEBEBE}Free";
        if (SafehouseData[i][sh_Status] == SAFEHOUSE_STATUS_OWNED)
        {
            new clanid = SafehouseData[i][sh_ClanID];
            new clanLabel[MAX_CLAN_TAG + 1] = "?";

            if (IsValidClanID(clanid))
                format(clanLabel, sizeof(clanLabel), "%s", ClanInfo[clanid][clanTag]);

            format(owner, sizeof(owner), "{E67E22}%s", clanLabel);
        }

        new line[128];
        format(line, sizeof(line), "{FFFFFF}#%02d %s\t%s\t%s\n",
            i, arenaData[i][arenaName], owner,
            (SafehouseData[i][sh_ExtZ] != 0.0) ? ("{23C552}Yes") : ("{F3230E}No"));
        strcat(buf, line);

        g_SafehouseEditList[playerid][g_SafehouseEditCount[playerid]++] = i;
    }

    format_MainColor(playerid, buf);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_EDIT_LIST<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Edit", "Back");
}

public DIALOG_SAFE_EDIT_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseAdminMenu(playerid);
    if (!IsSafehouseAdmin(playerid)) return 1;

    if (listitem < 0 || listitem >= g_SafehouseEditCount[playerid])
        return ShowSafehouseEditList(playerid);

    g_PlayerSafehouseEdit[playerid] = g_SafehouseEditList[playerid][listitem];
    return ShowSafehouseEditMenu(playerid);
}

@cmd(.alts = {"safe", "basemenu"}) safemenu(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index))
        return SendClientMessage(playerid, -1, "%s", SH_NO_SAFEHOUSE);

    if (g_PlayerInSafehouse[playerid] != index)
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You need to be inside your safehouse to open this menu.");

    return ShowSafehouseMenu(playerid);
}

@cmd(.alts = {"leavesafe", "sairbase"}) exitsafehouse(playerid, params[], help)
{
    if (!IsValidSafehouseIndex(g_PlayerInSafehouse[playerid]))
        return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You are not inside a safehouse.");

    ExitSafehouse(playerid);
    return SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"You left the safehouse.");
}

@cmd() editsafehouse(playerid, params[], help)
{
    if (!User[playerid][isLogged])
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if (!IsSafehouseAdmin(playerid))
        return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need Admin Level 5 or RCON to use this command.");

    new index = -1;
    if (isnull(params))
    {
        index = IsValidSafehouseIndex(g_PlayerInSafehouse[playerid]) ?
            g_PlayerInSafehouse[playerid] : GetSafehouseAtPosition(playerid, 5.0);

        // nothing nearby: open the system panel instead of failing
        if (!IsValidSafehouseIndex(index))
            return ShowSafehouseAdminMenu(playerid);
    }
    else
    {
        index = strval(params);
        if (!IsValidSafehouseIndex(index))
            return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Zone id must be between 0 and %d.", MAX_SAFEHOUSES - 1);

        if (!IsSafehouseZone(index))
            return va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}%s is a small zone and cannot host a safehouse.", arenaData[index][arenaName]);
    }

    g_PlayerSafehouseEdit[playerid] = index;
    return ShowSafehouseEditMenu(playerid);
}

stock ShowSafehouseEditMenu(playerid)
{
    new index = g_PlayerSafehouseEdit[playerid];
    if (!IsValidSafehouseIndex(index)) return 1;

    new title[80];
    format(title, sizeof(title), "{MAIN}Edit Safehouse - %s", SafehouseData[index][sh_Name]);
    format_MainColor(playerid, title);

    new buf[1024], line[192];
    format(buf, sizeof(buf), "{MAIN}Field\t{MAIN}Current\n");

    format(line, sizeof(line), "{FFFFFF}Exterior position\t{BEBEBE}%.1f, %.1f, %.1f\n",
        SafehouseData[index][sh_ExtX], SafehouseData[index][sh_ExtY], SafehouseData[index][sh_ExtZ]);
    strcat(buf, line);

    format(line, sizeof(line), "{FFFFFF}Interior spawn\t{BEBEBE}%.1f, %.1f, %.1f\n",
        SafehouseData[index][sh_IntX], SafehouseData[index][sh_IntY], SafehouseData[index][sh_IntZ]);
    strcat(buf, line);

    format(line, sizeof(line), "{FFFFFF}Exit position\t{BEBEBE}%.1f, %.1f, %.1f\n",
        SafehouseData[index][sh_ExitX], SafehouseData[index][sh_ExitY], SafehouseData[index][sh_ExitZ]);
    strcat(buf, line);

    format(line, sizeof(line), "{FFFFFF}Interior id\t{BEBEBE}%d\n", SafehouseData[index][sh_Interior]);
    strcat(buf, line);

    format(line, sizeof(line), "{FFFFFF}Virtual world\t{BEBEBE}%d\n", SafehouseData[index][sh_VirtualWorld]);
    strcat(buf, line);

    strcat(buf, "{23C552}Teleport here\t{BEBEBE}Go to the exterior position\n");
    strcat(buf, "{FFFFFF}Rebuild pickup\t{BEBEBE}Recreate pickup, icon and label\n");

    format_MainColor(playerid, buf);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_EDIT<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Select", "Back");
}

public DIALOG_SAFE_EDIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseEditList(playerid);

    new index = g_PlayerSafehouseEdit[playerid];
    if (!IsValidSafehouseIndex(index)) return 1;

    if (!IsSafehouseAdmin(playerid)) return 1;

    g_PlayerSafehouseEditField[playerid] = listitem;

    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    switch (listitem)
    {
        case SAFE_EDIT_EXTERIOR:
        {
            SafehouseData[index][sh_ExtX] = x;
            SafehouseData[index][sh_ExtY] = y;
            SafehouseData[index][sh_ExtZ] = z;
            SaveSafehouse(index);
            RefreshSafehousePickup(index);
            LogSafehouse(index, playerid, "admin_edit", "exterior position");
            SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Exterior position set to your location.");
        }
        case SAFE_EDIT_SPAWN:
        {
            SafehouseData[index][sh_IntX] = x;
            SafehouseData[index][sh_IntY] = y;
            SafehouseData[index][sh_IntZ] = z;
            SafehouseData[index][sh_IntA] = a;
            SafehouseData[index][sh_Interior] = GetPlayerInterior(playerid);
            SaveSafehouse(index);
            LogSafehouse(index, playerid, "admin_edit", "interior spawn");
            SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Interior spawn set to your location.");
        }
        case SAFE_EDIT_EXIT:
        {
            SafehouseData[index][sh_ExitX] = x;
            SafehouseData[index][sh_ExitY] = y;
            SafehouseData[index][sh_ExitZ] = z;
            SaveSafehouse(index);
            LogSafehouse(index, playerid, "admin_edit", "exit position");
            SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Exit position set to your location.");
        }
        case SAFE_EDIT_INTERIOR, SAFE_EDIT_VW:
        {
            new title[64], body[128];
            format(title, sizeof(title), "{MAIN}%s", (listitem == SAFE_EDIT_INTERIOR) ? ("Interior id") : ("Virtual world"));
            format(body, sizeof(body), "{FFFFFF}Enter the new %s:",
                (listitem == SAFE_EDIT_INTERIOR) ? ("interior id") : ("virtual world"));
            format_MainColor(playerid, title);

            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_EDIT_VALUE<iiiis>,
                DIALOG_STYLE_INPUT, title, body, "Save", "Back");
        }
        case SAFE_EDIT_GOTO:
        {
            if (SafehouseData[index][sh_ExtZ] == 0.0)
            {
                SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"This safehouse has no exterior position yet.");
                return ShowSafehouseEditMenu(playerid);
            }

            SetPlayerInterior(playerid, 0);
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerPos(playerid,
                SafehouseData[index][sh_ExtX],
                SafehouseData[index][sh_ExtY],
                SafehouseData[index][sh_ExtZ] + 1.0);
            SetCameraBehindPlayer(playerid);

            va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Teleported to {MAIN}%s{FFFFFF}.", SafehouseData[index][sh_Name]);
            return 1;
        }
        case SAFE_EDIT_PICKUP:
        {
            RefreshSafehousePickup(index);
            SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Pickup, icon and label rebuilt.");
        }
        case SAFE_EDIT_DELETE:
        {
            if (SafehouseData[index][sh_Status] == SAFEHOUSE_STATUS_OWNED)
            {
                SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"This safehouse belongs to a clan. Wait for it to expire or have the leader sell it first.");
                return ShowSafehouseEditMenu(playerid);
            }

            new title2[80], body[256];
            format(title2, sizeof(title2), "{MAIN}Delete Safehouse");
            format(body, sizeof(body),
                "{FFFFFF}Delete the safehouse in {MAIN}%s{FFFFFF}?\n\n\
{BEBEBE}The zone becomes free again and can be recreated anywhere inside it.",
                arenaData[index][arenaName]);

            format_MainColor(playerid, title2);
            format_MainColor(playerid, body);

            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_DELETE<iiiis>,
                DIALOG_STYLE_MSGBOX, title2, body, "Delete", "Back");
        }
    }
    return ShowSafehouseEditMenu(playerid);
}

public DIALOG_SAFE_EDIT_VALUE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseEditMenu(playerid);

    new index = g_PlayerSafehouseEdit[playerid];
    if (!IsValidSafehouseIndex(index)) return 1;

    if (!IsSafehouseAdmin(playerid)) return 1;

    new value = strval(inputtext);

    if (g_PlayerSafehouseEditField[playerid] == SAFE_EDIT_INTERIOR)
    {
        SafehouseData[index][sh_Interior] = value;
        LogSafehouse(index, playerid, "admin_edit", "interior id");
    }
    else
    {
        SafehouseData[index][sh_VirtualWorld] = value;
        LogSafehouse(index, playerid, "admin_edit", "virtual world");
    }

    SaveSafehouse(index);
    return ShowSafehouseEditMenu(playerid);
}

public DIALOG_SAFE_DELETE(playerid, dialogid, response, listitem, string:inputtext[])
{
    new index = g_PlayerSafehouseEdit[playerid];

    if (!response) return ShowSafehouseEditMenu(playerid);
    if (!IsSafehouseAdmin(playerid)) return 1;
    if (!IsValidSafehouseIndex(index) || !SafehouseExists(index)) return ShowSafehouseAdminMenu(playerid);

    if (SafehouseData[index][sh_Status] == SAFEHOUSE_STATUS_OWNED)
        return ShowSafehouseEditMenu(playerid);

    LogSafehouse(index, playerid, "admin_delete", arenaData[index][arenaName]);
    DeleteSafehouseRow(index);
    ResetSafehouse(index);

    va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Safehouse in {MAIN}%s{FFFFFF} deleted, the zone is free again.", arenaData[index][arenaName]);

    g_PlayerSafehouseEdit[playerid] = -1;
    return ShowSafehouseAdminMenu(playerid);
}

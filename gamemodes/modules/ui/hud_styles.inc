
#if defined _custom_hud_styles_included
    #endinput
#endif
#define _custom_hud_styles_included

enum
{
    HUD_REJECTING = 0,
    HUD_UNDERLINE,
    
    MAX_HUD_TYPES
}

#define HUD_DEFAULT HUD_REJECTING

#if !defined DIALOG_HUD_TYPE
    #define DIALOG_HUD_TYPE 14500
#endif

new const HUD_TYPES[MAX_HUD_TYPES][] =
{
    "hud_rejecting",
    "hud_underline"
};

static const g_monthNameTD[13][7] = {
    "zero", "Jan.", "Feb.", "Mar.", "Apr.", "May", "June",
    "July", "Aug.", "Sept.", "Oct.", "Nov.", "Dec."
};

stock getDateStrTDInfo(const primary_col[], const secondary_col[])
{
    new dString[48];
    new year, month, day;
    getdate(year, month, day);
    format(dString, sizeof(dString), "%s%02d %s%s %s%04d", primary_col, day, secondary_col, g_monthNameTD[month], primary_col, year);
    return dString;
}

stock RecreatePlayerFpsPingTd(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    if (FPSPingPacket[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, FPSPingPacket[playerid]);
        FPSPingPacket[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    if (User[playerid][hudType] == HUD_UNDERLINE)
    {
        FPSPingPacket[playerid] = CreatePlayerTextDraw(playerid, 578.000000, 16.000000, "_");
        PlayerTextDrawFont(playerid, FPSPingPacket[playerid], 1);
        PlayerTextDrawLetterSize(playerid, FPSPingPacket[playerid], 0.200000, 0.900000);
        PlayerTextDrawSetShadow(playerid, FPSPingPacket[playerid], 0);
        PlayerTextDrawSetOutline(playerid, FPSPingPacket[playerid], 1);
        PlayerTextDrawAlignment(playerid, FPSPingPacket[playerid], 2);
        PlayerTextDrawColor(playerid, FPSPingPacket[playerid], -1);
        PlayerTextDrawBackgroundColor(playerid, FPSPingPacket[playerid], MAIN_BACKGROUND_COLOUR);
        PlayerTextDrawSetProportional(playerid, FPSPingPacket[playerid], true);
    }
    else // HUD_REJECTING
    {
        FPSPingPacket[playerid] = CreatePlayerTextDraw(playerid, 557.000000, 1.399999, "_");
        PlayerTextDrawFont(playerid, FPSPingPacket[playerid], 1);
        PlayerTextDrawLetterSize(playerid, FPSPingPacket[playerid], 0.210795, 0.843375);
        PlayerTextDrawSetShadow(playerid, FPSPingPacket[playerid], 0);
        PlayerTextDrawSetOutline(playerid, FPSPingPacket[playerid], 1);
        PlayerTextDrawAlignment(playerid, FPSPingPacket[playerid], 2);
        PlayerTextDrawColor(playerid, FPSPingPacket[playerid], -1);
        PlayerTextDrawBackgroundColor(playerid, FPSPingPacket[playerid], MAIN_BACKGROUND_COLOUR);
    }
    return 1;
}

stock ShowPlayerHudTypesList(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    new string[1024];
    strcopy(string, "ID\tHUD Style\tStatus\n");
    for (new i = 0; i < sizeof(HUD_TYPES); i++)
    {
        strcat(string, fmat_return("%d\t%s\t%s\n", i, HUD_TYPES[i], (User[playerid][hudType] == i) ? ("{00FF00}Selected") : ("{FFFFFF}Select")));
    }

    return ShowPlayerDialog(playerid, DIALOG_HUD_TYPE, DIALOG_STYLE_TABLIST_HEADERS, "{MAIN}HUD Styles", string, "Select", "Cancel");
}

#include <YSI_Coding/y_hooks>

forward updatePlayerHud(playerid);

hook OnDialogResponse(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (dialogid == DIALOG_HUD_TYPE)
    {
        if (response && 0 <= listitem < sizeof(HUD_TYPES))
        {
            if (User[playerid][hudType] == listitem)
            {
                SendClientMessage(playerid, -1, fmat_return("{MAIN}[HUD] {FFFFFF}You are already using the {MAIN}%s{FFFFFF} style.", HUD_TYPES[listitem]));
                return 1;
            }

            User[playerid][hudType] = listitem;
            RecreatePlayerFpsPingTd(playerid);
            SendClientMessage(playerid, -1, fmat_return("{MAIN}[HUD] {FFFFFF}HUD style set to: {MAIN}%s", HUD_TYPES[listitem]));
            if (User[playerid][showPlayerHud])
            {
                updatePlayerHud(playerid);
                PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
            }
        }
        return 1;
    }
    return 0;
}

stock ToggleCustomHudTextDraws(playerid, hud_type, bool:toggle)
{
    if (!IsPlayerConnected(playerid)) return 0;

    if (hud_type < 0 || hud_type >= MAX_HUD_TYPES) hud_type = HUD_REJECTING;

    switch (hud_type)
    {
        case HUD_REJECTING, HUD_UNDERLINE:
        {
        }
    }

    return 1;
}

stock FormatNetworkDataForHud(hud_type, const primary_col[], const secondary_col[], fps, ping, Float:packet_loss, output[], len = sizeof(output))
{
    switch (hud_type)
    {
        case HUD_REJECTING:
        {
            format(output, len, "%sFPS: %s%d %sPing: %s%d %sPacketLoss: %s%.2f%%",
                primary_col, secondary_col, fps,
                primary_col, secondary_col, ping,
                primary_col, secondary_col, packet_loss
            );
        }
        case HUD_UNDERLINE:
        {
            format(output, len, "%s~n~%sFPS: %s%d %sPL: %s%.2f%%",
                getDateStrTDInfo(primary_col, secondary_col),
                primary_col, secondary_col, fps,
                primary_col, secondary_col, packet_loss
            );
        }
        default:
        {
            format(output, len, "%sFPS: %s%d %sPing: %s%d %sPL: %s%.2f%%",
                primary_col, secondary_col, fps,
                primary_col, secondary_col, ping,
                primary_col, secondary_col, packet_loss
            );
        }
    }
    return 1;
}

stock FormatDeathMessageForHud(hud_type, const primary_col[], const secondary_col[], const killer_name[], output[], len = sizeof(output))
{
    switch (hud_type)
    {
        case HUD_REJECTING, HUD_UNDERLINE:
            format(output, len, "%sKilled by %s%s", primary_col, secondary_col, killer_name);
        default:
            format(output, len, "%sKilled by %s%s", primary_col, secondary_col, killer_name);
    }
    return 1;
}

stock GetCustomClockString(hud_type, timestamp = 0, utc = -3, bool:is_pt_br = true)
{
    #pragma unused timestamp, utc, is_pt_br
    new string[64];

    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);

    switch (hud_type)
    {
        case HUD_REJECTING:
            format(string, sizeof(string), "%02d.%02d.%02d - %02d:%02d", day, month, year, hour, minute);
        case HUD_UNDERLINE:
            string[0] = '\0';
        default:
            format(string, sizeof(string), "%02d.%02d.%02d - %02d:%02d", day, month, year, hour, minute);
    }

    return string;
}

stock GetHudTypeName(hud_type, destination[], len = sizeof(destination))
{
    if (0 <= hud_type < MAX_HUD_TYPES)
    {
        format(destination, len, "%s", HUD_TYPES[hud_type]);
    }
    else
    {
        format(destination, len, "%s", HUD_TYPES[HUD_REJECTING]);
    }
    return 1;
}

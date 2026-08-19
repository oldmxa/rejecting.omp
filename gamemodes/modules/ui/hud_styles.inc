
#if defined _custom_hud_styles_included
    #endinput
#endif
#define _custom_hud_styles_included

enum
{
    HUD_REJECTING = 0,
    HUD_UNDERLINE,
    HUD_REJECTING_01,
    
    MAX_HUD_TYPES
}

#define HUD_DEFAULT HUD_REJECTING

#if !defined DIALOG_HUD_TYPE
    #define DIALOG_HUD_TYPE 14500
#endif

new const HUD_TYPES[MAX_HUD_TYPES][] =
{
    "Rejecting",
    "Underline",
    "RLOW"
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

stock RecreatePlayerHudTextDraws(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    if (FPSPingPacket[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, FPSPingPacket[playerid]);
        FPSPingPacket[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    for (new i = 0; i < 3; i++)
    {
        if (InfoStats[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, InfoStats[playerid][i]);
            InfoStats[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }
    }

    switch (User[playerid][hudType])
    {
        case HUD_UNDERLINE:
        {
            FPSPingPacket[playerid] = CreatePlayerTextDraw(playerid, 578.000000, 16.000000, "_");
            PlayerTextDrawFont(playerid, FPSPingPacket[playerid], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, FPSPingPacket[playerid], 0.200000, 0.900000);
            PlayerTextDrawSetShadow(playerid, FPSPingPacket[playerid], 0);
            PlayerTextDrawSetOutline(playerid, FPSPingPacket[playerid], 1);
            PlayerTextDrawAlignment(playerid, FPSPingPacket[playerid], TEXT_DRAW_ALIGN_CENTRE);
            PlayerTextDrawColor(playerid, FPSPingPacket[playerid], -1);
            PlayerTextDrawBackgroundColor(playerid, FPSPingPacket[playerid], MAIN_BACKGROUND_COLOUR);
            PlayerTextDrawSetProportional(playerid, FPSPingPacket[playerid], true);

            InfoStats[playerid][0] = CreatePlayerTextDraw(playerid, 3.200005, 390.257629, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][0], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][0], 0.180000, 0.799998);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][0], 1280.000000, 1280.000000);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][0], true);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][0], 1);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][0], 0);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][0], TEXT_DRAW_ALIGN_LEFT);
            PlayerTextDrawColor(playerid, InfoStats[playerid][0], -1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][0], MAIN_BACKGROUND_COLOUR);

            InfoStats[playerid][1] = CreatePlayerTextDraw(playerid, 625.767273, 397.214905, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][1], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][1], 0.216999, 0.766222);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][1], 1280.000000, 1280.000000);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][1], true);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][1], 1);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][1], 0);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][1], TEXT_DRAW_ALIGN_RIGHT);
            PlayerTextDrawColor(playerid, InfoStats[playerid][1], -1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][1], MAIN_BACKGROUND_COLOUR);

            InfoStats[playerid][2] = CreatePlayerTextDraw(playerid, 578.000000, 56.000000, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][2], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][2], 0.180000, 0.800000);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][2], true);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][2], 0);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][2], 1);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][2], TEXT_DRAW_ALIGN_CENTRE);
            PlayerTextDrawColor(playerid, InfoStats[playerid][2], -1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][2], MAIN_BACKGROUND_COLOUR);
        }
        case HUD_REJECTING_01:
        {
            // FPS / Ping / PL (rlow textdraw #2)
            FPSPingPacket[playerid] = CreatePlayerTextDraw(playerid, 554.400024, 2.986654, "_");
            PlayerTextDrawFont(playerid, FPSPingPacket[playerid], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, FPSPingPacket[playerid], 0.210795, 0.843375);
            PlayerTextDrawAlignment(playerid, FPSPingPacket[playerid], TEXT_DRAW_ALIGN_CENTRE);
            PlayerTextDrawColor(playerid, FPSPingPacket[playerid], -1);
            PlayerTextDrawSetProportional(playerid, FPSPingPacket[playerid], true);
            PlayerTextDrawTextSize(playerid, FPSPingPacket[playerid], 1280.000000, 1280.000000);
            PlayerTextDrawSetShadow(playerid, FPSPingPacket[playerid], 0);
            PlayerTextDrawSetOutline(playerid, FPSPingPacket[playerid], 1);
            PlayerTextDrawBackgroundColor(playerid, FPSPingPacket[playerid], MAIN_BACKGROUND_COLOUR);
            PlayerTextDrawSetSelectable(playerid, FPSPingPacket[playerid], false);

            // Left HUD (rlow textdraw #8)
            InfoStats[playerid][0] = CreatePlayerTextDraw(playerid, 3.200005, 390.257629, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][0], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][0], 0.180000, 0.799998);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][0], TEXT_DRAW_ALIGN_LEFT);
            PlayerTextDrawColor(playerid, InfoStats[playerid][0], -1);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][0], true);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][0], 1280.000000, 1280.000000);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][0], 0);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][0], 1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][0], MAIN_BACKGROUND_COLOUR);
            PlayerTextDrawSetSelectable(playerid, InfoStats[playerid][0], false);

            // Right HUD (rlow textdraw #3 / #6)
            InfoStats[playerid][1] = CreatePlayerTextDraw(playerid, 625.767273, 397.214905, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][1], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][1], 0.216999, 0.766222);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][1], TEXT_DRAW_ALIGN_RIGHT);
            PlayerTextDrawColor(playerid, InfoStats[playerid][1], -1);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][1], true);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][1], 1280.000000, 1280.000000);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][1], 0);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][1], 1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][1], MAIN_BACKGROUND_COLOUR);
            PlayerTextDrawSetSelectable(playerid, InfoStats[playerid][1], false);

            // Time and server name (rlow textdraw #11)
            InfoStats[playerid][2] = CreatePlayerTextDraw(playerid, 577.468811, 25.515415, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][2], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][2], 0.186127, 0.816041);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][2], TEXT_DRAW_ALIGN_CENTRE);
            PlayerTextDrawColor(playerid, InfoStats[playerid][2], -1);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][2], true);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][2], 1280.000000, 1280.000000);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][2], 0);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][2], 1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][2], MAIN_BACKGROUND_COLOUR);
            PlayerTextDrawSetSelectable(playerid, InfoStats[playerid][2], false);
        }
        default: // HUD_REJECTING
        {
            FPSPingPacket[playerid] = CreatePlayerTextDraw(playerid, 557.000000, 1.399999, "_");
            PlayerTextDrawFont(playerid, FPSPingPacket[playerid], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, FPSPingPacket[playerid], 0.210795, 0.843375);
            PlayerTextDrawSetShadow(playerid, FPSPingPacket[playerid], 0);
            PlayerTextDrawSetOutline(playerid, FPSPingPacket[playerid], 1);
            PlayerTextDrawAlignment(playerid, FPSPingPacket[playerid], TEXT_DRAW_ALIGN_CENTRE);
            PlayerTextDrawColor(playerid, FPSPingPacket[playerid], -1);
            PlayerTextDrawBackgroundColor(playerid, FPSPingPacket[playerid], MAIN_BACKGROUND_COLOUR);
            PlayerTextDrawSetProportional(playerid, FPSPingPacket[playerid], true);

            InfoStats[playerid][0] = CreatePlayerTextDraw(playerid, 3.200005, 390.257629, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][0], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][0], 0.180000, 0.799998);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][0], 1280.000000, 1280.000000);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][0], true);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][0], 1);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][0], 0);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][0], TEXT_DRAW_ALIGN_LEFT);
            PlayerTextDrawColor(playerid, InfoStats[playerid][0], -1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][0], MAIN_BACKGROUND_COLOUR);

            InfoStats[playerid][1] = CreatePlayerTextDraw(playerid, 625.767273, 397.214905, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][1], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][1], 0.216999, 0.766222);
            PlayerTextDrawTextSize(playerid, InfoStats[playerid][1], 1280.000000, 1280.000000);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][1], true);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][1], 1);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][1], 0);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][1], TEXT_DRAW_ALIGN_RIGHT);
            PlayerTextDrawColor(playerid, InfoStats[playerid][1], -1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][1], MAIN_BACKGROUND_COLOUR);

            InfoStats[playerid][2] = CreatePlayerTextDraw(playerid, 578.000000, 56.000000, "_");
            PlayerTextDrawFont(playerid, InfoStats[playerid][2], TEXT_DRAW_FONT_1);
            PlayerTextDrawLetterSize(playerid, InfoStats[playerid][2], 0.180000, 0.800000);
            PlayerTextDrawSetProportional(playerid, InfoStats[playerid][2], true);
            PlayerTextDrawSetShadow(playerid, InfoStats[playerid][2], 0);
            PlayerTextDrawSetOutline(playerid, InfoStats[playerid][2], 1);
            PlayerTextDrawAlignment(playerid, InfoStats[playerid][2], TEXT_DRAW_ALIGN_CENTRE);
            PlayerTextDrawColor(playerid, InfoStats[playerid][2], -1);
            PlayerTextDrawBackgroundColor(playerid, InfoStats[playerid][2], MAIN_BACKGROUND_COLOUR);
        }
    }
    return 1;
}

stock RecreatePlayerFpsPingTd(playerid)
{
    return RecreatePlayerHudTextDraws(playerid);
}

stock ShowPlayerHudTypesList(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    new string[1024];
    string[0] = '\0';

    for (new i = 0; i < sizeof(HUD_TYPES); i++)
        AppendStyleRow(string, HUD_TYPES[i], (User[playerid][hudType] == i));

    return ShowStylePicker(playerid, DIALOG_HUD_TYPE, "{MAIN}HUD Styles", string);
}

forward updatePlayerHud(playerid);
forward showPlayerTextDraw(playerid);
forward hidePlayerTextdraw(playerid);

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DIALOG_HUD_TYPE)
    {
        if (response && 0 <= listitem < sizeof(HUD_TYPES))
        {
            if (User[playerid][hudType] == listitem)
            {
                SendClientMessage(playerid, -1, fmat_return("{MAIN}[HUD] {FFFFFF}You are already using the {MAIN}%s{FFFFFF} style.", HUD_TYPES[listitem]));
            }
            else
            {
                User[playerid][hudType] = listitem;
                RecreatePlayerHudTextDraws(playerid);
                SendClientMessage(playerid, -1, fmat_return("{MAIN}[HUD] {FFFFFF}HUD style set to: {MAIN}%s", HUD_TYPES[listitem]));
                if (User[playerid][showPlayerHud])
                {
                    updatePlayerHud(playerid);
                    showPlayerTextDraw(playerid);
                }
            }
        }
        ShowPersonalizationMenu(playerid);
        return 1;
    }

    #if defined HSTY_OnDialogResponse
        return HSTY_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 0;
    #endif
}
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse HSTY_OnDialogResponse
#if defined HSTY_OnDialogResponse
    forward HSTY_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

stock ToggleCustomHudTextDraws(playerid, hud_type, bool:toggle)
{
    #pragma unused playerid, hud_type, toggle
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
        case HUD_REJECTING_01:
        {
            format(output, len, "%sFPS %s%d %sPing %s%d %sPacketLoss %s%.2f%%",
                primary_col, secondary_col, fps,
                primary_col, secondary_col, ping,
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
        case HUD_REJECTING, HUD_UNDERLINE, HUD_REJECTING_01:
            format(output, len, "%sKilled by %s%s", primary_col, secondary_col, killer_name);
        default:
            format(output, len, "%sKilled by %s%s", primary_col, secondary_col, killer_name);
    }
    return 1;
}

stock GetCustomClockString(hud_type, const primary_col[] = "", const secondary_col[] = "", timestamp = 0, utc = -3, bool:is_pt_br = true)
{
    #pragma unused timestamp, utc, is_pt_br
    new string[128];

    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);

    switch (hud_type)
    {
        case HUD_REJECTING:
            format(string, sizeof(string), "%02d.%02d.%04d - %02d:%02d", day, month, year, hour, minute);
        case HUD_UNDERLINE:
            string[0] = '\0';
        case HUD_REJECTING_01:
        {
            new secCol[20];
            if (secondary_col[0] != '\0') format(secCol, sizeof(secCol), "%s", secondary_col);
            else format(secCol, sizeof(secCol), "~w~~h~");

            format(string, sizeof(string), "%sR~w~~h~LOW_%sD~w~~h~EATHMATCH~n~%02d.%02d.%04d - %02d:%02d", primary_col, secCol, day, month, year, hour, minute);
        }
        default:
            format(string, sizeof(string), "%02d.%02d.%04d - %02d:%02d", day, month, year, hour, minute);
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


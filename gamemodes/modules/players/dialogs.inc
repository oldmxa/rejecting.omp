
// Dialogs Settings e Personalization.

SetAppearanceSource(playerid, source)
{
    SetPVarInt(playerid, "appearance_source", source);
    return 1;
}

GetAppearanceSource(playerid)
{
    return GetPVarInt(playerid, "appearance_source");
}

ReturnToAppearanceSource(playerid)
{
    switch(GetPVarInt(playerid, "appearance_source"))
    {
        case APPEARANCE_SRC_PERSONAL, APPEARANCE_SRC_MENU: ShowPersonalizationMenu(playerid);
        case APPEARANCE_SRC_PRACTICE: CallLocalFunction("ShowPracticeDialog", "i", playerid);
        case APPEARANCE_SRC_FEED: ShowDamageFeedMainMenu(playerid, APPEARANCE_SRC_PERSONAL);
        case APPEARANCE_SRC_DMG: ShowDamageStyleDialog(playerid, APPEARANCE_SRC_PERSONAL);
        case APPEARANCE_SRC_NONE: {} // opened from chat, nothing to reopen
        default: ShowPersonalizationMenu(playerid);
    }
    return 1;
}

ShowHudColorDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new body[3500];
    BuildHudColorDialogList(body, sizeof(body), User[playerid][hudColor]);

    new title[64] = "{MAIN}Personalization - HUD Color";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public HUD_COLORS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Accept", "Cancel");
}

ShowHudShadeDialog(playerid, source, bool:secondary)
{
    SetAppearanceSource(playerid, source);

    new selected = GetPlayerShadeIndex(playerid, secondary);

    new presetColor = GetPlayerHudColorIndex(playerid);
    new hex[7];
    if (secondary) hex = GetColorHexTag(hudPersonalization[presetColor][colorTwo]);
    else hex = GetColorHexTag(hudPersonalization[presetColor][colorOne]);

    new body[3500] = "{BEBEBE}Name\t{BEBEBE}Preview\n";

    strcat(body, fmat_return("%s{BEBEBE}Use interface preset\t{%s}Preview\n",
        STYLE_MARK(selected == -1), hex), sizeof(body));

    for(new i; i < sizeof(hudShades); i++)
    {
        strcat(body, fmat_return("%s{%06x}%s\t{%06x}Preview\n",
            STYLE_MARK(selected == i), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
    }

    new title[64];
    format(title, sizeof(title), "{MAIN}Personalization - %s Color", secondary ? ("Secondary") : ("Primary"));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    if(secondary)
        return Dialog_ShowCallback(playerid, using public HUD_SHADE_TWO<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Accept", "Cancel");

    return Dialog_ShowCallback(playerid, using public HUD_SHADE_ONE<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Accept", "Cancel");
}

ShowChatColorDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new mainHex[7], idHex[7], textHex[7];
    GetPlayerChatMainColorHex(playerid, mainHex);
    GetPlayerChatIDColorHex(playerid, idHex);
    GetPlayerChatTextColorHex(playerid, textHex);

    new body[768];
    format(body, sizeof(body),
"{BEBEBE}Option\t{BEBEBE}Preview\n\
{B4B5B7}\xBB {FFFFFF}Main Accent Color ({MAIN})\t{%s}[MAIN] Sample Message\n\
{B4B5B7}\xBB {FFFFFF}Color ID in chat\t{%s}[%d] Sample ID\n\
{B4B5B7}\xBB {FFFFFF}Text color in chat\t{%s}Sample Chat Text",
        mainHex, idHex, playerid, textHex);

    new title[64] = "{MAIN}Personalization - Chat Color";

    format_MainColor(playerid, title);
    return Dialog_ShowCallback(playerid, using public CHAT_COLOR_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "<");
}

forward CHAT_COLOR_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
public CHAT_COLOR_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
        return ReturnToAppearanceSource(playerid);

    if(listitem == 0)
    {
        ShowChatColorSelectionDialog(playerid, 2, GetAppearanceSource(playerid));
    }
    else if(listitem == 1)
    {
        ShowChatColorSelectionDialog(playerid, 0, GetAppearanceSource(playerid));
    }
    else if(listitem == 2)
    {
        ShowChatColorSelectionDialog(playerid, 1, GetAppearanceSource(playerid));
    }

    return 1;
}

ShowChatColorSelectionDialog(playerid, color_type, source)
{
    SetAppearanceSource(playerid, source);
    SetPVarInt(playerid, "ChatColorType", color_type);

    new curHex[7];
    if(color_type == 0) GetPlayerChatIDColorHex(playerid, curHex);
    else if(color_type == 1) GetPlayerChatTextColorHex(playerid, curHex);
    else GetPlayerChatMainColorHex(playerid, curHex);

    new body[3500];
    format(body, sizeof(body), "{BEBEBE}Name\t{BEBEBE}Preview\n");

    if(color_type == 2)
    {
        new bool:isDef = (User[playerid][chatMainColor] <= 0);
        strcat(body, fmat_return("%s{FFFFFF}Use Interface Color (Default)\t{BEBEBE}Follows your HUD color\n",
            isDef ? ("{23C552}> ") : ("{B4B5B7}\x95 ")), sizeof(body));

        new bool:isCustom = (User[playerid][chatMainColor] & 0x10000000) != 0;
        strcat(body, fmat_return("%s{FFFFFF}Custom HEX Color\t{%s}#%s\n",
            isCustom ? ("{23C552}> ") : ("{B4B5B7}\x95 "), curHex, curHex), sizeof(body));

        for(new i; i < sizeof(hudShades); i++)
        {
            new bool:isSelected = (User[playerid][chatMainColor] == (i + 1));
            strcat(body, fmat_return("%s{%06x}%s\t{%06x}[MAIN] Sample Message\n",
                isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
        }
    }
    else
    {
        new bool:isDef = false;
        if(color_type == 0 && User[playerid][chatIDColor] <= 0) isDef = true;
        if(color_type == 1 && User[playerid][chatColor] <= 0) isDef = true;

        new bool:isCustom = false;
        if(color_type == 0 && (User[playerid][chatIDColor] & 0x10000000)) isCustom = true;
        if(color_type == 1 && (User[playerid][chatColor] & 0x10000000)) isCustom = true;

        strcat(body, fmat_return("%s{FFFFFF}Default White (Chat)\t{FFFFFF}Sample Text\n",
            isDef ? ("{23C552}> ") : ("{B4B5B7}\x95 ")), sizeof(body));

        strcat(body, fmat_return("%s{FFFFFF}Custom HEX Color\t{%s}#%s\n",
            isCustom ? ("{23C552}> ") : ("{B4B5B7}\x95 "), curHex, curHex), sizeof(body));

        for(new i; i < sizeof(hudShades); i++)
        {
            new bool:isSelected = false;
            if(color_type == 0)
            {
                if(User[playerid][chatIDColor] == (i + 1)) isSelected = true;
            }
            else
            {
                if(User[playerid][chatColor] == (i + 1)) isSelected = true;
            }

            strcat(body, fmat_return("%s{%06x}%s\t{%06x}Sample Chat Text\n",
                isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
        }
    }

    new title[64];
    if(color_type == 2) format(title, sizeof(title), "{MAIN}Chat Color - Main Accent ({MAIN})");
    else format(title, sizeof(title), "{MAIN}Chat Color - %s", (color_type == 0) ? ("Color ID") : ("Text Color"));

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public CHAT_COLORS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "*", "<");
}

ShowChatColorCustomHexDialog(playerid, color_type, source)
{
    SetAppearanceSource(playerid, source);
    SetPVarInt(playerid, "ChatColorType", color_type);

    new curHex[7];
    if(color_type == 0) GetPlayerChatIDColorHex(playerid, curHex);
    else if(color_type == 1) GetPlayerChatTextColorHex(playerid, curHex);
    else GetPlayerChatMainColorHex(playerid, curHex);

    new body[512];
    new typeName[16];
    if(color_type == 2) format(typeName, sizeof(typeName), "Main {MAIN}");
    else format(typeName, sizeof(typeName), "%s", (color_type == 0) ? ("ID") : ("Text"));

    format(body, sizeof(body),
"{FFFFFF}Current %s Color: {%s}#%s\n\n\
{BEBEBE}Enter your new 6-digit HEX color code below (e.g. {23C552}23C552{BEBEBE} for Green, {F84F31}F84F31{BEBEBE} for Red).\n\
{BEBEBE}Valid characters: 0-9, A-F.",
        typeName, curHex, curHex);

    new title[64];
    format(title, sizeof(title), "{MAIN}Custom HEX Color - %s", typeName);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public CHAT_COLOR_CUSTOM_HEX<iiiis>, DIALOG_STYLE_INPUT, title, body, "*", "<");
}

ShowDamageStyleDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new const damageStyles[][] = {
        "Dynamic (Moving numbers)",
        "Numbers (Static numbers)",
        "Dot (*)",
        "X (X)"
    };

    new hitHex = (User[playerid][hitmarkColor] >>> 8);
    new hitColorName[48];
    new bool:foundPreset = false;
    for(new i; i < sizeof(hudShades); i++)
    {
        if(User[playerid][hitmarkColor] == ((hudShades[i][shadeHex] << 8) | DEFAULT_HITMARK_ALPHA))
        {
            format(hitColorName, sizeof(hitColorName), "{%06x}%s", hudShades[i][shadeHex], hudShades[i][shadeName]);
            foundPreset = true;
            break;
        }
    }
    if(!foundPreset)
    {
        if(User[playerid][hitmarkColor] == DEFAULT_HITMARK_COLOR) format(hitColorName, sizeof(hitColorName), "{FFFFFF}Default White");
        else format(hitColorName, sizeof(hitColorName), "{%06x}#%08x", hitHex, User[playerid][hitmarkColor]);
    }

    new body[768] = "{BEBEBE}Option\t{BEBEBE}Status / Format\n";
    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Damage Informer State\t%s\n", (User[playerid][showDamageInformer]) ? ("{23C552}Visible") : ("{F84F31}Hidden")), sizeof(body));
    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Damage Informer Color\t%s\n", hitColorName), sizeof(body));

    for(new i; i < sizeof(damageStyles); i++)
    {
        new bool:isSelected = (User[playerid][dmgInformerStyle] == i && User[playerid][showDamageInformer]);
        strcat(body, fmat_return("%s{FFFFFF}%d\t{FFFFFF}%s\n",
            isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), i, damageStyles[i]), sizeof(body));
    }

    new title[64] = "{MAIN}Personalization - Damage Style";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_DMG_STYLE<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "<");
}

ShowDamageFeedMainMenu(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new fcL[48] = "{BEBEBE}Default", fcR[48] = "{BEBEBE}Default";
    if(0 <= User[playerid][dmgFeedColorLeft] < sizeof(hudShades))  format(fcL, sizeof(fcL), "{%06x}%s", hudShades[User[playerid][dmgFeedColorLeft]][shadeHex], hudShades[User[playerid][dmgFeedColorLeft]][shadeName]);
    if(0 <= User[playerid][dmgFeedColorRight] < sizeof(hudShades)) format(fcR, sizeof(fcR), "{%06x}%s", hudShades[User[playerid][dmgFeedColorRight]][shadeHex], hudShades[User[playerid][dmgFeedColorRight]][shadeName]);

    new fcAll[96];
    if(User[playerid][dmgFeedColorLeft] == User[playerid][dmgFeedColorRight])
    {
        format(fcAll, sizeof(fcAll), "%s", fcL);
    }
    else
    {
        format(fcAll, sizeof(fcAll), "%s {BEBEBE}/ %s", fcL, fcR);
    }

    new body[512] = "{BEBEBE}Option\t{BEBEBE}Current Setting\n";
    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Damage Feed State\t%s\n", (User[playerid][showDamageFeed]) ? ("{23C552}Visible") : ("{F84F31}Hidden")), sizeof(body));
    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Display Style\t{FFFFFF}%s\n", GetDamageFeedStyleName(User[playerid][dmgFeedStyle])), sizeof(body));
    strcat(body, fmat_return("{B4B5B7}\xBB {FFFFFF}Colors (Left & Right)\t%s", fcAll), sizeof(body));

    new title[64] = "{MAIN}Personalization - Damage Feed";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_DMG_FEED_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "<");
}

ShowDamageFeedDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new const feedStyles[][] = {
        "Name - Weapon +dmg",
        "Name (HP) Weapon +dmg",
        "Name - Weapon +dmg (HP)",
        "Name - Weapon +dmg - KILL"
    };

    new body[512] = "{BEBEBE}Style ID\t{BEBEBE}Format\n";
    for(new i; i < sizeof(feedStyles); i++)
    {
        new bool:isSelected = (User[playerid][dmgFeedStyle] == i);
        strcat(body, fmat_return("%s{FFFFFF}%d\t{FFFFFF}%s\n",
            isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), i, feedStyles[i]), sizeof(body));
    }

    new title[64] = "{MAIN}Personalization - Damage Feed";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_DMG_FEED<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "<");
}

ShowDamageFeedColorMenu(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new fcL[48] = "{BEBEBE}Default", fcR[48] = "{BEBEBE}Default";
    if(0 <= User[playerid][dmgFeedColorLeft] < sizeof(hudShades))  format(fcL, sizeof(fcL), "{%06x}%s", hudShades[User[playerid][dmgFeedColorLeft]][shadeHex], hudShades[User[playerid][dmgFeedColorLeft]][shadeName]);
    if(0 <= User[playerid][dmgFeedColorRight] < sizeof(hudShades)) format(fcR, sizeof(fcR), "{%06x}%s", hudShades[User[playerid][dmgFeedColorRight]][shadeHex], hudShades[User[playerid][dmgFeedColorRight]][shadeName]);

    new fcAll[96];
    if(User[playerid][dmgFeedColorLeft] == User[playerid][dmgFeedColorRight])
    {
        format(fcAll, sizeof(fcAll), "%s", fcL);
    }
    else
    {
        format(fcAll, sizeof(fcAll), "%s {BEBEBE}/ %s", fcL, fcR);
    }

    new body[512] = "{BEBEBE}Target\t{BEBEBE}Current Color\n";
    strcat(body, fmat_return("{FFFFFF}\x95 {B4B5B7}Whole feed (All)\t%s\n", fcAll), sizeof(body));
    strcat(body, fmat_return("{FFFFFF}\x95 {B4B5B7}Left side (Name & Weapon)\t%s\n", fcL), sizeof(body));
    strcat(body, fmat_return("{FFFFFF}\x95 {B4B5B7}Right side (Damage value)\t%s", fcR), sizeof(body));

    new title[64] = "{MAIN}Personalization - Feed Color";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_DMG_FEED_COLMENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "<");
}

ShowDamageFeedColorDialog(playerid, source, side)
{
    SetAppearanceSource(playerid, source);
    SetPVarInt(playerid, "dmgFeedColSide", side);

    new selected;
    switch(side)
    {
        case 0:  selected = User[playerid][dmgFeedColorLeft];
        case 1:  selected = User[playerid][dmgFeedColorRight];
        case 2:  selected = (User[playerid][dmgFeedColorLeft] == User[playerid][dmgFeedColorRight]) ? User[playerid][dmgFeedColorLeft] : -2;
        default: selected = User[playerid][dmgFeedColorLeft];
    }
    new body[3500] = "{BEBEBE}Name\t{BEBEBE}Preview\n";

    strcat(body, fmat_return("%s{BEBEBE}Default\t{BEBEBE}Preview\n",
        STYLE_MARK(selected == -1)), sizeof(body));

    for(new i; i < sizeof(hudShades); i++)
    {
        strcat(body, fmat_return("%s{%06x}%s\t{%06x}Preview\n",
            STYLE_MARK(selected == i), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
    }

    new sideName[16];
    switch(side)
    {
        case 0:  sideName = "Left Side";
        case 1:  sideName = "Right Side";
        default: sideName = "Whole Feed";
    }

    new title[64];
    format(title, sizeof(title), "{MAIN}Damage Feed - %s Color", sideName);

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_DMG_FEED_COLOR<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "<");
}

ShowHitmarkColorDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new curHex = (User[playerid][hitmarkColor] >>> 8);
    new body[3500];
    format(body, sizeof(body), "{BEBEBE}Name\t{BEBEBE}Preview\n");

    new bool:isDef = (User[playerid][hitmarkColor] == DEFAULT_HITMARK_COLOR);
    strcat(body, fmat_return("%s{FFFFFF}Default White (Semi-Transparent)\t{%06x}Sample (45)\n",
        isDef ? ("{23C552}> ") : ("{B4B5B7}\x95 "), (DEFAULT_HITMARK_COLOR >>> 8)), sizeof(body));

    new bool:isCustom = true;
    if(isDef) isCustom = false;
    for(new i; i < sizeof(hudShades); i++)
    {
        if(User[playerid][hitmarkColor] == ((hudShades[i][shadeHex] << 8) | DEFAULT_HITMARK_ALPHA))
        {
            isCustom = false;
            break;
        }
    }

    strcat(body, fmat_return("%s{FFFFFF}Custom HEX Color\t{%06x}#%08x\n",
        isCustom ? ("{23C552}> ") : ("{B4B5B7}\x95 "), curHex, User[playerid][hitmarkColor]), sizeof(body));

    for(new i; i < sizeof(hudShades); i++)
    {
        new bool:isSelected = (User[playerid][hitmarkColor] == ((hudShades[i][shadeHex] << 8) | DEFAULT_HITMARK_ALPHA));
        strcat(body, fmat_return("%s{%06x}%s\t{%06x}Sample (45)\n",
            isSelected ? ("{23C552}> ") : ("{B4B5B7}\x95 "), hudShades[i][shadeHex], hudShades[i][shadeName], hudShades[i][shadeHex]), sizeof(body));
    }

    new title[64] = "{MAIN}Damage Informer - Color";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_HITMARK<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "<");
}

ShowHitmarkCustomHexDialog(playerid, source)
{
    SetAppearanceSource(playerid, source);

    new curHex = (User[playerid][hitmarkColor] >>> 8);
    new body[512];
    format(body, sizeof(body),
"{FFFFFF}Current Damage Informer Color: {%06x}#%08x\n\n\
{BEBEBE}Enter your HEX color code below:\n\
{FFFFFF}\x95 {BEBEBE}6 digits ({MAIN}RRGGBB{BEBEBE}): Applies color with default transparency (e.g. {23C552}23C552{BEBEBE}, {F84F31}F84F31{BEBEBE})\n\
{FFFFFF}\x95 {BEBEBE}8 digits ({MAIN}RRGGBBAA{BEBEBE}): Custom color & transparency/alpha (e.g. {MAIN}FFFFFF60{BEBEBE}, {MAIN}FF0000AA{BEBEBE})\n\n\
{BEBEBE}Valid characters: 0-9, A-F (prefix # or 0x is optional).",
        curHex, User[playerid][hitmarkColor]);

    new title[64] = "{MAIN}Custom HEX Color - Damage Informer";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_HITMARK_CUSTOM_HEX<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "<");
}

static ApplyHudShade(playerid, listitem, bool:secondary)
{
    new value = (listitem == 0) ? -1 : (listitem - 1);

    if(value >= sizeof(hudShades)) return 0;

    if(secondary) User[playerid][hudShadeTwo] = value;
    else User[playerid][hudShadeOne] = value;

    UpdateRoundStatsColors(playerid);
    updatePlayerHud(playerid);
    SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);

    if(User[playerid][showPlayerHud])
    {
        PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
        PlayerTextDrawShow(playerid, InfoStats[playerid][0]);
        PlayerTextDrawShow(playerid, InfoStats[playerid][1]);
    }
    return 1;
}

forward HUD_SHADE_ONE(playerid, dialogid, response, listitem, string:inputtext[]);
public HUD_SHADE_ONE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response) ApplyHudShade(playerid, listitem, false);
    return ReturnToAppearanceSource(playerid);
}

forward HUD_SHADE_TWO(playerid, dialogid, response, listitem, string:inputtext[]);
public HUD_SHADE_TWO(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response) ApplyHudShade(playerid, listitem, true);
    return ReturnToAppearanceSource(playerid);
}

forward HUD_COLORS(playerid, dialogid, response, listitem, string:inputtext[]);
public HUD_COLORS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
        return ReturnToAppearanceSource(playerid);

    if(0 <= listitem < sizeof(hudPersonalization))
    {
        new bool:hadCustom = (GetPlayerShadeIndex(playerid, false) != -1 || GetPlayerShadeIndex(playerid, true) != -1);

        User[playerid][hudColor] = listitem;
        User[playerid][hudShadeOne] = -1;
        User[playerid][hudShadeTwo] = -1;

        if(hadCustom)
            SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Primary and secondary colors were reset to the interface preset.");

        UpdateRoundStatsColors(playerid);
        updatePlayerHud(playerid);
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);

        if(User[playerid][showPlayerHud])
        {
            PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
            PlayerTextDrawShow(playerid, InfoStats[playerid][0]);
            PlayerTextDrawShow(playerid, InfoStats[playerid][1]);
        }
    }

    return ReturnToAppearanceSource(playerid);
}

forward CHAT_COLORS(playerid, dialogid, response, listitem, string:inputtext[]);
public CHAT_COLORS(playerid, dialogid, response, listitem, string:inputtext[])
{
    new color_type = GetPVarInt(playerid, "ChatColorType");
    if (!response)
    {
        return ShowChatColorDialog(playerid, GetAppearanceSource(playerid));
    }

    if(listitem == 0)
    {
        if(color_type == 2) User[playerid][chatMainColor] = 0;
        else if(color_type == 0) User[playerid][chatIDColor] = 0;
        else User[playerid][chatColor] = 0;

        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
    }
    else if(listitem == 1)
    {
        return ShowChatColorCustomHexDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new selectedIdx = listitem - 2;
    if(0 <= selectedIdx < sizeof(hudShades))
    {
        if(color_type == 2)
        {
            User[playerid][chatMainColor] = selectedIdx + 1;
        }
        else if(color_type == 0)
        {
            User[playerid][chatIDColor] = selectedIdx + 1;
        }
        else
        {
            User[playerid][chatColor] = selectedIdx + 1;
        }
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    }

    return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
}

forward CHAT_COLOR_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[]);
public CHAT_COLOR_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[])
{
    new color_type = GetPVarInt(playerid, "ChatColorType");
    if (!response)
    {
        return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new colorVal;
    if(!IsValidHexColor(inputtext, colorVal))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX format! Please enter 6 characters (e.g. FF0000).");
        return ShowChatColorCustomHexDialog(playerid, color_type, GetAppearanceSource(playerid));
    }

    new storedVal = 0x10000000 | (colorVal & 0xFFFFFF);
    if(color_type == 0)
    {
        User[playerid][chatIDColor] = storedVal;
    }
    else if(color_type == 1)
    {
        User[playerid][chatColor] = storedVal;
    }
    else
    {
        User[playerid][chatMainColor] = storedVal;
    }

    SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    return ShowChatColorSelectionDialog(playerid, color_type, GetAppearanceSource(playerid));
}

forward DIALOG_DMG_STYLE(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DMG_STYLE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ReturnToAppearanceSource(playerid);

    if(listitem == 0)
    {
        User[playerid][showDamageInformer] = !User[playerid][showDamageInformer];
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        return ShowDamageStyleDialog(playerid, GetAppearanceSource(playerid));
    }
    else if(listitem == 1)
    {
        return ShowHitmarkColorDialog(playerid, APPEARANCE_SRC_DMG);
    }
    else if(2 <= listitem <= 5)
    {
        User[playerid][dmgInformerStyle] = listitem - 2;
        User[playerid][showDamageInformer] = true;
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        return ShowDamageStyleDialog(playerid, GetAppearanceSource(playerid));
    }

    return ReturnToAppearanceSource(playerid);
}

forward DIALOG_DMG_FEED_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DMG_FEED_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ReturnToAppearanceSource(playerid);

    switch(listitem)
    {
        case 0:
        {
            User[playerid][showDamageFeed] = !User[playerid][showDamageFeed];
            SetDamageFeedForPlayer(playerid, User[playerid][showDamageFeed] ? 1 : 0);
            SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
            ShowDamageFeedMainMenu(playerid, GetAppearanceSource(playerid));
        }
        case 1: ShowDamageFeedDialog(playerid, APPEARANCE_SRC_FEED);
        case 2: ShowDamageFeedColorMenu(playerid, APPEARANCE_SRC_FEED);
    }
    return 1;
}

forward DIALOG_DMG_FEED(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DMG_FEED(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response && 0 <= listitem <= 3)
    {
        User[playerid][dmgFeedStyle] = listitem;
        SetDamageFeedStyle(playerid, User[playerid][dmgFeedStyle]);
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    }

    return ReturnToAppearanceSource(playerid);
}

forward DIALOG_DMG_FEED_COLMENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DMG_FEED_COLMENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ReturnToAppearanceSource(playerid);

    switch(listitem)
    {
        case 0:  ShowDamageFeedColorDialog(playerid, GetAppearanceSource(playerid), 2);
        case 1:  ShowDamageFeedColorDialog(playerid, GetAppearanceSource(playerid), 0);
        case 2:  ShowDamageFeedColorDialog(playerid, GetAppearanceSource(playerid), 1);
        default: ShowDamageFeedColorDialog(playerid, GetAppearanceSource(playerid), 2);
    }
    return 1;
}

forward DIALOG_DMG_FEED_COLOR(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_DMG_FEED_COLOR(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        DeletePVar(playerid, "dmgFeedColSide");
        return ShowDamageFeedColorMenu(playerid, GetAppearanceSource(playerid));
    }

    if(0 <= listitem <= sizeof(hudShades))
    {
        new idx = listitem - 1;

        switch(GetPVarInt(playerid, "dmgFeedColSide"))
        {
            case 0: User[playerid][dmgFeedColorLeft] = idx;
            case 1: User[playerid][dmgFeedColorRight] = idx;
            case 2:
            {
                User[playerid][dmgFeedColorLeft] = idx;
                User[playerid][dmgFeedColorRight] = idx;
            }
            default: User[playerid][dmgFeedColorLeft] = idx;
        }

        ApplyDamageFeedColors(playerid);
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    }

    DeletePVar(playerid, "dmgFeedColSide");
    return ShowDamageFeedColorMenu(playerid, GetAppearanceSource(playerid));
}

forward DIALOG_HITMARK(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HITMARK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
        return ReturnToAppearanceSource(playerid);

    if(listitem == 0)
    {
        User[playerid][hitmarkColor] = DEFAULT_HITMARK_COLOR;
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        return ShowHitmarkColorDialog(playerid, GetAppearanceSource(playerid));
    }
    else if(listitem == 1)
    {
        return ShowHitmarkCustomHexDialog(playerid, GetAppearanceSource(playerid));
    }

    new selectedIdx = listitem - 2;
    if(0 <= selectedIdx < sizeof(hudShades))
    {
        User[playerid][hitmarkColor] = (hudShades[selectedIdx][shadeHex] << 8) | DEFAULT_HITMARK_ALPHA;
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
    }

    return ShowHitmarkColorDialog(playerid, GetAppearanceSource(playerid));
}

forward DIALOG_HITMARK_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_HITMARK_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        return ShowHitmarkColorDialog(playerid, GetAppearanceSource(playerid));
    }

    new parsedColor;
    if(ParseHexColorRGBA(inputtext, parsedColor, DEFAULT_HITMARK_ALPHA))
    {
        User[playerid][hitmarkColor] = parsedColor;
        SendClientMessage(playerid, -1, "%s", INFO_PREFERENCES);
        return ShowHitmarkColorDialog(playerid, GetAppearanceSource(playerid));
    }
    else
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX format! Enter 6 digits (RRGGBB) or 8 digits with alpha (RRGGBBAA).");
        return ShowHitmarkCustomHexDialog(playerid, GetAppearanceSource(playerid));
    }
}


forward CHANGE(playerid, dialogid, response, listitem, string:inputtext[]);
public CHANGE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        new bool:fromPersonal = bool:GetPVarInt(playerid, "gunmenu_from_personalization");
        DeletePVar(playerid, "gunmenu_from_personalization");

        if(!HasPlayerConfiguredWeapons(playerid) && User[playerid][isAlreadySpawned])
        {
            SendClientMessage(playerid, -1, "%s", CHOOSE_WEAPON_MSG);
            if(fromPersonal) SetPVarInt(playerid, "gunmenu_from_personalization", 1);
            gunMenu(playerid);
            DeletePVar(playerid, "slotName");
            return false;
        }

        if(GetPVarInt(playerid, "showGunmenuFirst") == 1)
        {
        	SetPlayerVirtualWorld(playerid, User[playerid][locationId]*99);
        	DeletePVar(playerid, "showGunmenuFirst");
        }

        if(!IsPlayerInLounge(playerid))
            givePlayerWeapons(playerid);

        if(GetPVarInt(playerid, "gunmenu_world"))
        {
        	DeletePVar(playerid, "gunmenu_world");
        	if(User[playerid][locationId] != -1)
        		SetPlayerVirtualWorld(playerid, User[playerid][locationId]*99);
        }

        if(fromPersonal)
        {
            ShowPersonalizationMenu(playerid);
            return true;
        }

        return true;
    }

    switch(listitem)
    {
        case 0:
        {
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for first slot", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }


        case 1:
        {
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for second slot", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }

        case 2:
        {
            if(User[playerid][playerLevel] < 5)
                return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}The third slot will be available only after reaching level {MAIN}5{FFFFFF}."), gunMenu(playerid);
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for third slot", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }
        case 3:
        {
            if(User[playerid][playerLevel] < 10) return SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}The fourth slot will be available only after reaching level {MAIN}10{FFFFFF}."), gunMenu(playerid);
            Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, "Select weapon for slot four", showAvailableWeapons(), "Select", "Back");
            SetPVarInt(playerid, "slotId", listitem);
        }

        default:
            gunMenu(playerid);
    }

    if(!IsPlayerInLounge(playerid) && !IsPlayerAFK(playerid))
        givePlayerWeapons(playerid);
    return true;
}

forward CHANGE1(playerid, dialogid, response, listitem, string:inputtext[]);
public CHANGE1(playerid, dialogid, response, listitem, string:inputtext[])
{
	if(!response || !(0 <= listitem < sizeof(gunmenuWInfo)))
	{
		return gunMenu(playerid);
	}

	new slotid = GetPVarInt(playerid, "slotId");
	new listitemm = listitem+1;
	new slotNum[4][6] =
	{
		"one",
		"two",
		"three",
		"four"
	};
	switch(listitem)
	{
		case 0, 1: User[playerid][playerWeapons][slotid] = listitemm;
		case 2:
		{
			if(User[playerid][playerLevel] < 5)
				return Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, fmat_return("Select weapon for slot %s", slotNum[slotid]), showAvailableWeapons(), "Select", "Back"), SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}This weapon will be available upon reaching level {MAIN}5{FFFFFF}.");
			User[playerid][playerWeapons][slotid] = listitemm;
		}
		case 3:
		{
			if(User[playerid][playerLevel] < 10)
				return Dialog_ShowCallback(playerid, using public CHANGE1<iiiis>, DIALOG_STYLE_LIST, fmat_return("Select weapon for slot %s", slotNum[slotid]), showAvailableWeapons(), "Select", "Back"), SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}This weapon will be available upon reaching level {MAIN}10{FFFFFF}.");

			User[playerid][playerWeapons][slotid] = listitemm;
		}
		case 4:	User[playerid][playerWeapons][slotid] = 0;
	}

	if(!IsPlayerInLounge(playerid) && !IsPlayerAFK(playerid))
		givePlayerWeapons(playerid);

	gunMenu(playerid);

    return true;
}

forward FIGHT(playerid, dialogid, response, listitem, string:inputtext[]);
public FIGHT(playerid, dialogid, response, listitem, string:inputtext[])
{
	if(response)
    {
		if(0 <= listitem < sizeof(fightStyles))
        {
            User[playerid][fightStyle] = fightStyles[listitem][styleid];
            SetPlayerFightingStyle(playerid, User[playerid][fightStyle]);
            UpdatePlayerData(playerid);
            va_SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Your fighting style has been updated to: {MAIN}%s", fightStyles[listitem][fightname]);
            ChangeFight(playerid);
            return 1;
        }
	}
    else
    {
        ShowPersonalizationMenu(playerid);
    }

	return true;
}

forward CHANGE_NICK(playerid, dialogid, response, listitem, string:inputtext[]);
public CHANGE_NICK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response)
    {
        return true;
    }
    if (!(3 <= strlen(inputtext) <= 20))
    {
        return SendClientMessage(playerid, -1, "{MAIN}**{FFFFFF} Invalid nickname entered, try another one");
    }

    if(IsPlayerFakingNick(playerid))
        return SendClientMessage(playerid, -1, "%s", NICK_CHANGE_WHILE_FAKE);

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `id` FROM `accounts` WHERE `username` = '%e'", inputtext);
    mysql_tquery(dbHandle, queryGlobal, "OnPlayerTryChangeNick", "iss", playerid, inputtext, User[playerid][userName]);

	return true;
}

forward DIALOG_RANK(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_RANK(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        // training challenges: served from the training module caches, no query
        if(listitem == 10)
        {
            CallLocalFunction("ShowTrainingChallengeDialog", "i", playerid);
            return true;
        }

        new query[256], header[128];
        switch(listitem)
        {
            case 0: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `played_time` FROM `accounts` ORDER BY `played_time` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Played Time"); }
            case 1: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `kills` FROM `accounts` ORDER BY `kills` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Total Kills"); }
            case 2: { mysql_format(dbHandle, query, sizeof(query), "SELECT `clan_name`, `clan_kills`, `clan_zone_points` FROM `clans` ORDER BY `clan_zone_points` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Dominant Clans"); }
            case 3: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `kills_semanais` FROM `accounts` ORDER BY `kills_semanais` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Weekly Kills"); }
            case 4: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `damage_semanal` FROM `accounts` ORDER BY `damage_semanal` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Weekly Damage"); }
            case 5: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `deaths` FROM `accounts` ORDER BY `deaths` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Total Deaths"); }
            case 6: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `max_streak` FROM `accounts` ORDER BY `max_streak` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Highest Killstreaks"); }
            case 7: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `max_damage` FROM `accounts` ORDER BY `max_damage` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Session Peak Damage"); }
            case 8: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `won_arenas` FROM `accounts` ORDER BY `won_arenas` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Most Arena Wins"); }
            case 9: { mysql_format(dbHandle, query, sizeof(query), "SELECT `username`, `duel_wins` FROM `accounts` ORDER BY `duel_wins` DESC LIMIT 10"); format(header, sizeof(header), "{MAIN}Top 10: {FFFFFF}Duel Victories"); }
        }

        inline const OnRankLoad()
        {
            new liststr[2048], usernamee[24], value, clanName_[20], clankills, clanzp;
            new rows = cache_num_rows();
            
            if(rows)
            {
                if(listitem == 2) format(liststr, sizeof(liststr), "{MAIN}#\t{MAIN}Clan Name\t{MAIN}Score Points\t{MAIN}Frags\n");
                else if(listitem == 0) format(liststr, sizeof(liststr), "{MAIN}#\t{MAIN}Player\t{MAIN}Time Spent Online\n");
                else format(liststr, sizeof(liststr), "{MAIN}#\t{MAIN}Player\t{MAIN}Record / Value\n");

                for(new i; i < rows; i++)
                {
                    new pos_color[16], trophy[8];
                    switch(i)
                    {
                        case 0: { pos_color = "{FFFF00}"; trophy = " (G)"; } // Gold
                        case 1: { pos_color = "{C0C0C0}"; trophy = " (S)"; } // Silver
                        case 2: { pos_color = "{CD7F32}"; trophy = " (B)"; } // Bronze
                        default: { pos_color = "{B4B5B7}"; trophy = ""; }
                    }

                    if(listitem == 2) // Clan Rank
                    {
                        cache_get_value_name(i, "clan_name", clanName_);
                        cache_get_value_name_int(i, "clan_zone_points", clanzp);
                        cache_get_value_name_int(i, "clan_kills", clankills);
                        strcat(liststr, fmat_return("%s%02d.\t{FFFFFF}%s%s\t{23C552}%d pts\t{FFFFFF}%d kills\n", pos_color, i+1, clanName_, trophy, clanzp, clankills));
                    }
                    else // Player Ranks
                    {
                        cache_get_value_name(i, "username", usernamee);
                        
                        if(listitem == 0) // Played Time
                        {
                            cache_get_value_name_int(i, "played_time", value);
                            new hour = value/3600, minute = (value%3600)/60, second = value%60;
                            strcat(liststr, fmat_return("%s%02d.\t{FFFFFF}%s%s\t{FFFFFF}%02dh %02dm %02ds\n", pos_color, i+1, usernamee, trophy, hour, minute, second));
                        }
                        else
                        {
                            new col_name[32], unit[16] = "";
                            switch(listitem)
                            {
                                case 1: col_name = "kills";
                                case 3: col_name = "kills_semanais";
                                case 4: { col_name = "damage_semanal"; unit = " dmg"; }
                                case 5: col_name = "deaths";
                                case 6: col_name = "max_streak";
                                case 7: { col_name = "max_damage"; unit = " dmg"; }
                                case 8: { col_name = "won_arenas"; unit = " wins"; }
                                case 9: { col_name = "duel_wins"; unit = " wins"; }
                            }
                            cache_get_value_name_int(i, col_name, value);
                            strcat(liststr, fmat_return("%s%02d.\t{FFFFFF}%s%s\t{FFFFFF}%d%s\n", pos_color, i+1, usernamee, trophy, value, unit));
                        }
                    }
                }
                

                format_MainColor(playerid, header);
                format_MainColor(playerid, liststr);
                Dialog_ShowCallback(playerid, using public DIALOG_RANK1<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, header, liststr, "<", "Exit");
            }
            else
            {
                SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}No data found for this rank.");
                Dialog_ShowCallback(playerid, using public DIALOG_RANK1<iiiis>, DIALOG_STYLE_MSGBOX, "Rankings", "No records found yet.", "Back", "");
            }
        }
        MySQL_TQueryInline(dbHandle, using inline OnRankLoad, query);
    }
    return true;
}

forward DIALOG_RANK1(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_RANK1(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        new rankstr[1024];
        strcat(rankstr, "{MAIN}Category\t{MAIN}Description\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Played Time\t{FFFFFF}Ranking of the most active community members\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Total Kills\t{FFFFFF}Global statistics of confirmed frags\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Dominant Clans\t{FFFFFF}Clans with highest points and dominance\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Weekly Kills\t{FFFFFF}Top killers of the current week\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Weekly Damage\t{FFFFFF}Top damage dealers of the current week\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Total Deaths\t{FFFFFF}Statistics of players with most casualties\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Highest Killstreaks\t{FFFFFF}Best combat records without dying\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Session Peak Damage\t{FFFFFF}Highest damage achieved in a single session\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Most Arena Wins\t{FFFFFF}Elite players with most round victories\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Duel Victories\t{FFFFFF}Top duel champions of the server\n");
        strcat(rankstr, "{MAIN}\xBB {B4B5B7}Training Challenges\t{FFFFFF}Personal & server challenge records");

        new header[64] = "{MAIN}Server Statistics & Rankings";

        format_MainColor(playerid, header);
        format_MainColor(playerid, rankstr);
        Dialog_ShowCallback(playerid, using public DIALOG_RANK<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, header, rankstr, "View Rank", "Close");
    }
    return true;
}

forward DIALOG_MAPLIST(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_MAPLIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    new maxitems, listitemm, currentpage;
    currentpage = GetPVarInt(playerid, "playerCurrentPage");
    maxitems = GetPVarInt(playerid, "pageItems");
    listitemm = listitem+(currentpage*16);

    if(!response)
    {
        if(currentpage > 0)
        {
            SetPVarInt(playerid, "playerCurrentPage", currentpage - 1);
            showMapListDialog(playerid);
            return true;
        }
        return true;
    }

    if(strfind(inputtext, "Next Page") != -1 || listitem == maxitems)
    {
        if((currentpage + 1) * 16 >= MAX_ZONES)
        {
            showMapListDialog(playerid);
            return true;
        }
        SetPVarInt(playerid, "playerCurrentPage", currentpage+1);
        showMapListDialog(playerid);
        return true;
    }

    if(listitem < maxitems)
    {
        if(changeArenaDelay > gettime() &&  User[playerid][adminLevel] < 4)
            return va_SendClientMessage(playerid, -1, CHANGEARENA_DELAY, convertTime(changeArenaDelay - gettime()));

        new location = User[playerid][locationId];
        if(location < 0 || location >= MAX_LOCATIONS || location == ARENA_AFK)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You need to be inside an arena to use this command.");

        if(location == ARENA_CUSTOM)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The Custom Room has its own map list. Use {MAIN}/clist{FFFFFF}.");

        if(listitemm < 0 || listitemm >= MAX_ZONES || IsCustomArena(listitemm))
            return va_SendClientMessage(playerid, -1, NOT_VALID_ARENA, MAX_ZONES-1);

        if(listitemm == lastArenaId[location])
            return va_SendClientMessage(playerid, -1, ARENA_JUST_PLAYED, arenaData[listitemm][arenaName]);

        foreach(new i : Player)
        {
            if(User[i][locationId] != location || GetPVarInt(i, "DuelS_State") != DUELS_STATE_NONE || IsPlayerInDuel2v2(i)) continue;
            GangZoneStopPulseForPlayer(i, e_ZONE_PULSE_ZONE, arenaInfo[location][arenaId]);
        }

        GetRoundRank(location);
        new oldArenaId = arenaInfo[location][arenaId];
        arenaInfo[location][arenaId] = listitemm;
        PrepareChangeArena(location, oldArenaId);

        foreach(new i : Player)
        {
            if((User[i][locationId] == location && GetPVarInt(i, "DuelS_State") == DUELS_STATE_NONE && !IsPlayerInDuel2v2(i)) || User[i][adminLevel] > 0)
                va_SendClientMessage(i, -1, CHANGED_ARENA_MSG , User[playerid][userName] , arenaData[listitemm][arenaName], listitemm);
        }

        if (!IsAdmin(playerid, 4))
            changeArenaDelay = gettime()+(3*60);
    }
    return true;
}

// Clan

forward DIALOG_PROFILE_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PROFILE_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    new action = 0, targetid = INVALID_PLAYER_ID;

    if(response)
    {
        targetid = GetPVarInt(playerid, "profile_target");

        new pvar[20];
        format(pvar, sizeof(pvar), "profile_act_%d", listitem);
        action = GetPVarInt(playerid, pvar);
    }

    new act_count = GetPVarInt(playerid, "profile_act_count");
    for(new i = 0; i < act_count; i++)
    {
        new pvar_clean[20];
        format(pvar_clean, sizeof(pvar_clean), "profile_act_%d", i);
        DeletePVar(playerid, pvar_clean);
    }
    DeletePVar(playerid, "profile_act_count");

    if(response)
    {
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

        switch(action)
        {
            case 1: // Geral stats
            {
                ShowStats(playerid, targetid);
            }
            case 2: // Reports
            {
                if(targetid == playerid) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_SELF);
                if(IsAdmin(targetid)) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_ADMIN);

                new header[64];
                format(header, sizeof(header), "{MAIN}Submit Report: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_REPORT_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for reporting this player:", "Submit", "Cancel");
            }
            case 3: // Clan Information
            {
                if(!IsPlayerInClan(targetid)) return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}This player is no longer in a clan.");
                ShowPlayerClanInfo(playerid, targetid);
            }
            case 4: // Names changes
            {
                if(!IsAdmin(playerid)) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);
                ShowNameHistory(playerid, targetid);
            }
            case 5: // Penality historico
            {
                if (!IsAdmin(playerid, 4)) return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);
                ShowPenaltyHistory(playerid, User[targetid][userName]);
            }
            case 6: // Go
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/go %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 7: // Get
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/get %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 8: // Slap
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/slap %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 9: // Mute
            {
                new header[64];
                format(header, sizeof(header), "{MAIN}Mute Player: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_MUTE_REASON<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for muting this player:", "Submit", "Cancel");
            }
            case 10: // Unmute
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/unmute %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 11: // Kick
            {
                new header[64];
                format(header, sizeof(header), "{MAIN}Kick Player: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_KICK_REASON<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for kicking this player:", "Submit", "Cancel");
            }
            case 12: // Ban
            {
                new header[64];
                format(header, sizeof(header), "{MAIN}Ban Player: {FFFFFF}%s", User[targetid][userName]);
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ADM_BAN_REASON<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter the reason for banning this player:", "Submit", "Cancel");
            }
            case 13: // Spec
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/spec %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 14: // Gtemp
            {
                new cmd[32];
                format(cmd, sizeof(cmd), "/gtemp %d", targetid);
                Command_ReProcess(playerid, cmd, 0);
            }
            case 15: // Ask Question
            {
                new header[64] = "{MAIN}Submit Question";
                format_MainColor(playerid, header);
                Dialog_ShowCallback(playerid, using public DIALOG_ASK_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{FFFFFF}Please enter your question for the administration:", "Submit", "Cancel");
            }
            case 16: // Achievements
            {
                ShowAchievementsMainMenu(playerid, targetid);
            }
        }
    }

    return 1;
}

forward DIALOG_ASK_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_ASK_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if (IsAdmin(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[ASK] {FFFFFF}Administrators cannot submit questions via /ask.");

        if(GetPVarInt(playerid, "ask_cooldown"))
            return SendClientMessage(playerid, -1, "{MAIN}[ASK] {FFFFFF}Please wait 30 seconds before sending another question.");

        if(isnull(inputtext)) 
        {
            new header[64] = "{MAIN}Submit Question";
            format_MainColor(playerid, header);
            return Dialog_ShowCallback(playerid, using public DIALOG_ASK_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{F3230E}Question cannot be empty!{FFFFFF}\n\nPlease enter your question for the administration:", "Submit", "Cancel");
        }

        new cmd[256];
        format(cmd, sizeof(cmd), "/ask %s", inputtext);
        Command_ReProcess(playerid, cmd, 0);
    }
    return 1;
}

forward DIALOG_REPORT_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_REPORT_SUBMIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(response)
    {
        if (IsAdmin(playerid))
            return SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Administrators cannot use player reports.");

        if(GetPVarInt(playerid, "report_cooldown"))
            return SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Please wait 30 seconds before sending another report.");

        new targetid = GetPVarInt(playerid, "profile_target");
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);
        if(targetid == playerid) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_SELF);
        if(IsAdmin(targetid)) return SendClientMessage(playerid, -1, "%s", CANNOT_REPORT_ADMIN);
        if(isnull(inputtext)) 
        {
            new header[64];
            format(header, sizeof(header), "{MAIN}Submit Report: {FFFFFF}%s", User[targetid][userName]);
            format_MainColor(playerid, header);
            return Dialog_ShowCallback(playerid, using public DIALOG_REPORT_SUBMIT<iiiis>, DIALOG_STYLE_INPUT, header, "{F3230E}Reason cannot be empty!{FFFFFF}\n\nPlease enter the reason for reporting this player:", "Submit", "Cancel");
        }

        SetPVarInt(playerid, "report_cooldown", 1);
        SetTimerEx("ReleaseReport", 30000, false, "i", playerid);

        foreach(new i : Player)
        {
            if(IsAdmin(i))
            {
                if(IsPlayerVip(playerid))
                {
                    va_SendClientMessage(i, 0xFF7700FF, "{MAIN}[VIP REPORT] {FFFFFF}Player %s[%d] reported: %s[%d] | Reason: %s", User[playerid][userName], playerid, User[targetid][userName], targetid, inputtext);
                    GameTextForPlayer(i, "~y~~h~VIP REPORT", 3000, 1);
                }
                else
                {
                    va_SendClientMessage(i, 0xEDA909AA, "{MAIN}[REPORT] {FFFFFF}Player %s[%d] reported: %s[%d] | Reason: %s", User[playerid][userName], playerid, User[targetid][userName], targetid, inputtext);
                    GameTextForPlayer(i, "~r~~h~NEW REPORT", 3000, 1);
                }
                PlaySuccessSound(i);
            }
        }
        va_SendClientMessage(playerid, -1, "{MAIN}[REPORT] {FFFFFF}Your report against {MAIN}%s {FFFFFF}has been sent successfully to the Administration!", User[targetid][userName]);
        PlayErrorSound(playerid);
        
        LogTicket(User[playerid][user_sqlid], User[targetid][userName], inputtext);

        new log_details[256];
        format(log_details, sizeof(log_details), "Player %s reported %s. Reason: %s", User[playerid][userName], User[targetid][userName], inputtext);
        LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "reports", "Report", log_details);

        new string[1024];
        format(string, sizeof(string), "**Reporter:** %s\n**Target:** %s\n**Reason:** %s",
            User[playerid][userName],
            User[targetid][userName],
            inputtext);

        SendDiscordEmbed(logs_report, "New Report", string, DISCORD_COLOR_WARNING);
    }
    return 1;
}

enum
{
    TAG_TYPE_VIP,
    TAG_TYPE_CLAN,
    TAG_TYPE_ADMIN,
    TAG_TYPE_CUSTOM,
    TAG_TYPE_EDIT_CUSTOM,
    TAG_TYPE_TOURNAMENT
};

new const g_PredefinedTags[][] = {
    "PRO PLAYER",
    "LEGEND",
    "LEGENDARY",
    "DM KING",
    "MONSTER",
    "HEADSHOOTER",
    "MYTH",
    "BAD BOY",
    "SUPREME",
    "DOMINATOR",
    "IMMORTAL",
    "EXECUTOR"
};

static g_PlayerTagMenu[MAX_PLAYERS][8];
static g_PlayerTagMenuCount[MAX_PLAYERS];

#define CUSTOM_TAG_MIN_LEN  (2)
#define CUSTOM_TAG_MAX_LEN  (16)
#define CUSTOM_TAG_NO_PERM  "{MAIN}[ERROR] {FFFFFF}You must be VIP, Admin Level 5 or have special permission to use Custom Tags."

stock ResetPlayerTagMenu(playerid)
{
    g_PlayerTagMenuCount[playerid] = 0;
    return 1;
}

stock CanPlayerUseCustomTag(playerid)
{
    if(IsPlayerVip(playerid) || User[playerid][adminLevel] >= 5 || User[playerid][customTagPerm]) return true;
    return false;
}

stock bool:SanitizeCustomTag(const input[], dest[], size = sizeof(dest))
{
    dest[0] = '\0';

    new end = strlen(input) - 1, start = 0;
    while(start <= end && input[start] == ' ') start++;
    while(end >= start && input[end] == ' ') end--;

    new outLen = end - start + 1;
    if(outLen < CUSTOM_TAG_MIN_LEN || outLen > CUSTOM_TAG_MAX_LEN) return false;
    if(outLen >= size) return false;

    for(new i = start; i <= end; i++)
    {
        new c = input[i];
        if(c < ' ' || c > '~') return false;
        if(c == '{' || c == '}') return false;
        if(c == '~') return false;
    }

    strmid(dest, input, start, end + 1, size);
    return true;
}

stock SaveCustomTag(playerid)
{
    new query[192];
    mysql_format(dbHandle, query, sizeof(query),
        "UPDATE accounts SET custom_tag = '%e', hide_custom_tag = %d WHERE id = %d",
        User[playerid][customTag], User[playerid][hideCustomTag], User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, query);
    return 1;
}

stock ApplyCustomTag(playerid, const tag[])
{
    format(User[playerid][customTag], 32, "%s", tag);
    User[playerid][hideCustomTag] = false;

    SaveCustomTag(playerid);
    SetClanColor(playerid);
    va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Your Custom Tag has been set to {FF4500}%s{FFFFFF}!", User[playerid][customTag]);
    return 1;
}

stock ApplyTournamentTag(playerid, const tag[])
{
    if(playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)) return 0;

    new cleanTag[32];
    format(cleanTag, sizeof(cleanTag), "%s", tag);
    if(cleanTag[0] == '[') strdel(cleanTag, 0, 1);
    new len = strlen(cleanTag);
    if(len > 0 && cleanTag[len - 1] == ']') cleanTag[len - 1] = '\0';

    format(User[playerid][customTag], 32, "%s", cleanTag);
    User[playerid][hideCustomTag] = false;
    User[playerid][customTagPerm] = true;

    SaveCustomTag(playerid);
    SetClanColor(playerid);
    va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Equipped tournament tag: {FF4500}%s{FFFFFF}!", User[playerid][customTag]);
    return 1;
}

stock ClearCustomTag(playerid)
{
    User[playerid][customTag][0] = '\0';
    User[playerid][hideCustomTag] = true;

    SaveCustomTag(playerid);
    SetClanColor(playerid);
    SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Your Tag has been unequipped.");
    return 1;
}

stock ShowCustomTagOptionsMenu(playerid)
{
    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    new body[512];
    format(body, sizeof(body), "{BEBEBE}Option\t{BEBEBE}Description\t{BEBEBE}Action\n");
    strcat(body, "{B4B5B7}\x95 {FFFFFF}Predefined Tags\t{BEBEBE}Choose from server preset tags\t{3CCFFA}[Select]\n");
    strcat(body, "{B4B5B7}\x95 {FFFFFF}Custom Tag Text\t{BEBEBE}Enter your own custom tag (2-16 chars)\t{3CCFFA}[Enter]\n");
    strcat(body, "{B4B5B7}\x95 {FFFFFF}Remove Tag\t{BEBEBE}Unequip current custom tag\t{F84F31}[Remove]\n");

    new title[64] = "{MAIN}Custom Tag - Options";

    format_MainColor(playerid, title);
    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_CUSTOM_TAG_OPTIONS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

static stock HandleCustomTagRemoval(playerid)
{
    if(User[playerid][customTag][0] == '\0')
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You do not have a Custom Tag set to remove.");
        return ShowTagMenu(playerid);
    }

    ClearCustomTag(playerid);
    return ShowTagMenu(playerid);
}

stock ShowPredefinedTagsDialog(playerid)
{
    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    new listStr[1024];
    format(listStr, sizeof(listStr), "{BEBEBE}#\t{BEBEBE}Tag Preview\t{BEBEBE}Status\n");

    for(new i = 0; i < sizeof(g_PredefinedTags); i++)
    {
        new isEquipped = (!User[playerid][hideCustomTag] && !strcmp(User[playerid][customTag], g_PredefinedTags[i], true));
        format(listStr, sizeof(listStr), "%s{B4B5B7}\x95 {FFFFFF}%02d.\t{3CCFFA}%s\t%s\n",
            listStr,
            i + 1,
            g_PredefinedTags[i],
            isEquipped ? "{23C552}[Equipped]" : "{3CCFFA}[Select]");
    }

    new title[64] = "{MAIN}Predefined Tags";

    format_MainColor(playerid, title);
    format_MainColor(playerid, listStr);
    return Dialog_ShowCallback(playerid, using public DIALOG_PREDEFINED_TAGS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, listStr, "Select", "Back");
}

forward DIALOG_CUSTOM_TAG_OPTIONS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_CUSTOM_TAG_OPTIONS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTagMenu(playerid);

    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    switch(listitem)
    {
        case 0: // Predefined list
        {
            return ShowPredefinedTagsDialog(playerid);
        }
        case 1: // Custom input
        {
            new title[64] = "{MAIN}Custom Tag - Enter Text";
            new body[512];
            format(body, sizeof(body),
                "{FFFFFF}Enter your desired {MAIN}Custom Tag{FFFFFF} text below:\n\n"\
                "{MAIN}\x95 {B4B5B7}Length: {FFFFFF}%d to %d characters\n"\
                "{MAIN}\x95 {B4B5B7}Rules: {FFFFFF}Letters, numbers and basic symbols only\n"\
                "{MAIN}\x95 {B4B5B7}Formatting: {FFFFFF}Brackets [] and colors are applied automatically\n\n"\
                "{BEBEBE}Type {FF6347}none{BEBEBE} to unequip your current tag.",
                CUSTOM_TAG_MIN_LEN, CUSTOM_TAG_MAX_LEN);


            format_MainColor(playerid, title);
            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_EDIT_CUSTOM_TAG<iiiis>, DIALOG_STYLE_INPUT, title, body, "Save", "Back");
        }
        case 2: // Remove
        {
            return HandleCustomTagRemoval(playerid);
        }
    }
    return 1;
}

forward DIALOG_PREDEFINED_TAGS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PREDEFINED_TAGS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowCustomTagOptionsMenu(playerid);

    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    if(listitem < 0 || listitem >= sizeof(g_PredefinedTags)) return ShowCustomTagOptionsMenu(playerid);

    ApplyCustomTag(playerid, g_PredefinedTags[listitem]);
    return ShowTagMenu(playerid);
}

forward DIALOG_EDIT_CUSTOM_TAG(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_EDIT_CUSTOM_TAG(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowCustomTagOptionsMenu(playerid);

    if(!CanPlayerUseCustomTag(playerid))
    {
        SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
        return ShowTagMenu(playerid);
    }

    if(isnull(inputtext) || strcmp(inputtext, "none", true) == 0)
    {
        return HandleCustomTagRemoval(playerid);
    }

    new cleanTag[32];
    if(!SanitizeCustomTag(inputtext, cleanTag, sizeof(cleanTag)))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid Custom Tag. Use %d to %d normal characters - colour codes and special symbols are not allowed.",
            CUSTOM_TAG_MIN_LEN, CUSTOM_TAG_MAX_LEN);
        return ShowCustomTagOptionsMenu(playerid);
    }

    ApplyCustomTag(playerid, cleanTag);
    return ShowTagMenu(playerid);
}

stock SaveWonTournamentTag(userid, const tag[], const tourneyName[])
{
    new cleanTag[32];
    format(cleanTag, sizeof(cleanTag), "%s", tag);
    if(cleanTag[0] == '[') strdel(cleanTag, 0, 1);
    new len = strlen(cleanTag);
    if(len > 0 && cleanTag[len - 1] == ']') cleanTag[len - 1] = '\0';

    new query[256];
    mysql_format(dbHandle, query, sizeof(query),
        "INSERT INTO `player_tournament_tags` (`user_id`, `tag_name`, `tournament_name`, `won_at`) VALUES (%d, '%e', '%e', UNIX_TIMESTAMP()) \
        ON DUPLICATE KEY UPDATE `tournament_name` = '%e', `won_at` = UNIX_TIMESTAMP()",
        userid, cleanTag, tourneyName, tourneyName);
    mysql_tquery(dbHandle, query);
    return 1;
}

forward OnPlayerTournamentTagsLoaded(playerid);
public OnPlayerTournamentTagsLoaded(playerid)
{
    if(!IsPlayerLogged(playerid)) return 1;

    new rows = cache_num_rows();
    if(rows == 0)
    {
        new title[64] = "{MAIN}Tournament Tags";
        new body[384] =
            "{FFFFFF}You have not won any tournament tags yet!\n\n"\
            "{MAIN}\x95 {B4B5B7}Competitions: {FFFFFF}Compete in official server tournaments\n"\
            "{MAIN}\x95 {B4B5B7}Rewards: {FFFFFF}Earn exclusive champion tags and prizes";

        format_MainColor(playerid, title);
        format_MainColor(playerid, body);
        return Dialog_ShowCallback(playerid, using public DIALOG_NO_TOURNEY_TAGS<iiiis>, DIALOG_STYLE_MSGBOX, title, body, "Back", "");
    }

    new listStr[2048];
    format(listStr, sizeof(listStr), "{MAIN}Tag\t{MAIN}Tournament Won\t{MAIN}Status\n");
    strcat(listStr, "{FF6347}None (Unequip)\t{BEBEBE}Do not display tournament tag\t{FF6347}[Unequip]\n");

    for(new i = 0; i < rows; i++)
    {
        new tTag[32], tTourney[64];
        cache_get_value_name(i, "tag_name", tTag, sizeof(tTag));
        cache_get_value_name(i, "tournament_name", tTourney, sizeof(tTourney));

        new isEquipped = (!User[playerid][hideCustomTag] && !strcmp(User[playerid][customTag], tTag, true));
        format(listStr, sizeof(listStr), "%s{FF4500}%s\t{FFFFFF}%s\t%s\n",
            listStr,
            tTag,
            tTourney,
            isEquipped ? "{23C552}[Equipped]" : "{90C3D4}[Equip]");
    }

    new title[64] = "{MAIN}My Tournament Tags";

    format_MainColor(playerid, title);
    format_MainColor(playerid, listStr);
    return Dialog_ShowCallback(playerid, using public DIALOG_PLAYER_TOURNEY_TAGS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, listStr, "Select", "Back");
}

stock ShowPlayerTournamentTagsDialog(playerid)
{
    if(!IsPlayerLogged(playerid)) return 0;

    new query[192];
    mysql_format(dbHandle, query, sizeof(query), "SELECT `tag_name`, `tournament_name`, `won_at` FROM `player_tournament_tags` WHERE `user_id` = %d ORDER BY `won_at` DESC", User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, query, "OnPlayerTournamentTagsLoaded", "d", playerid);
    return 1;
}

forward DIALOG_NO_TOURNEY_TAGS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_NO_TOURNEY_TAGS(playerid, dialogid, response, listitem, string:inputtext[])
{
    return ShowTagMenu(playerid);
}

forward DIALOG_PLAYER_TOURNEY_TAGS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_PLAYER_TOURNEY_TAGS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowTagMenu(playerid);

    if(listitem == 0)
    {
        ClearCustomTag(playerid);
        return ShowTagMenu(playerid);
    }

    new targetIndex = listitem - 1;
    new query[192];
    mysql_format(dbHandle, query, sizeof(query), "SELECT `tag_name` FROM `player_tournament_tags` WHERE `user_id` = %d ORDER BY `won_at` DESC LIMIT %d, 1", User[playerid][user_sqlid], targetIndex);

    inline const OnTagSelected()
    {
        if(cache_num_rows())
        {
            new selTag[32];
            cache_get_value_name(0, "tag_name", selTag, sizeof(selTag));
            ApplyTournamentTag(playerid, selTag);
        }
        ShowTagMenu(playerid);
    }
    MySQL_TQueryInline(dbHandle, using inline OnTagSelected, query);
    return 1;
}

stock ShowTagMenu(playerid)
{
    if(!IsPlayerLogged(playerid)) return 0;

    new tagList[1024];
    format(tagList, sizeof(tagList), "{MAIN}Tag\t{MAIN}Display\t{MAIN}Status\n");

    new count = 0;

    // 1. VIP Tag
    if(IsPlayerVip(playerid))
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_VIP;
        new vipPreview[32] = "{FFD700}[VIP]";
        new tagIdx = User[playerid][vipHeadTag];
        if(0 <= tagIdx < 10) format(vipPreview, sizeof(vipPreview), "{%s}[VIP]", vipColorListStr[tagIdx]);
        else if(tagIdx >= 0x10000000) format(vipPreview, sizeof(vipPreview), "{%06x}[VIP]", tagIdx & 0xFFFFFF);

        format(tagList, sizeof(tagList), "%s{FFFFFF}VIP Tag\t%s\t%s\n",
            tagList,
            vipPreview,
            User[playerid][hideVipTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");
    }

    // 2. Clan Tag
    if(IsPlayerInClan(playerid))
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_CLAN;
        new clanid = User[playerid][clanID];
        format(tagList, sizeof(tagList), "%s{FFFFFF}Clan Tag\t{%06x}[%s]\t%s\n",
            tagList,
            (ClanInfo[clanid][clanColor] >>> 8),
            ClanInfo[clanid][clanName],
            User[playerid][hideClanTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");
    }

    // 3. Admin Tag
    if(IsAdmin(playerid))
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_ADMIN;
        format(tagList, sizeof(tagList), "%s{FFFFFF}Admin Tag\t{FFFFFF}[{FF4500}%s{FFFFFF}]\t%s\n",
            tagList,
            adminnames[User[playerid][adminLevel]][ingame],
            User[playerid][hideAdminTag] ? "{FF6347}[Hidden]" : "{23C552}[Visible]");
    }

    // 4. Custom Tag
    g_PlayerTagMenu[playerid][count++] = TAG_TYPE_CUSTOM;
    new customPreview[64];
    if(User[playerid][customTag][0] != '\0')
        format(customPreview, sizeof(customPreview), "{FF4500}%s", User[playerid][customTag]);
    else
        format(customPreview, sizeof(customPreview), "{BEBEBE}(None)");

    new customStatus[32];
    if(!CanPlayerUseCustomTag(playerid) && User[playerid][customTag][0] == '\0')
        format(customStatus, sizeof(customStatus), "{B4B5B7}[No Access]");
    else if(User[playerid][customTag][0] == '\0')
        format(customStatus, sizeof(customStatus), "{3CCFFA}[Configure]");
    else
        format(customStatus, sizeof(customStatus), "%s", User[playerid][hideCustomTag] ? "{F84F31}[Hidden]" : "{23C552}[Visible]");

    format(tagList, sizeof(tagList), "%s{B4B5B7}\x95 {FFFFFF}Custom Tag\t%s\t%s\n",
        tagList,
        customPreview,
        customStatus);

    // 5. Configure Custom Tag
    if(CanPlayerUseCustomTag(playerid))
    {
        g_PlayerTagMenu[playerid][count++] = TAG_TYPE_EDIT_CUSTOM;
        format(tagList, sizeof(tagList), "%s{B4B5B7}\x95 {FFFFFF}Configure Custom Tag\t{BEBEBE}Presets or Custom Text\t{3CCFFA}[Configure]\n", tagList);
    }

    // 6. Manage Tournament Tags
    g_PlayerTagMenu[playerid][count++] = TAG_TYPE_TOURNAMENT;
    format(tagList, sizeof(tagList), "%s{B4B5B7}\x95 {FFFFFF}Tournament Tags\t{BEBEBE}Manage won champion tags\t{3CCFFA}[Select]\n", tagList);

    g_PlayerTagMenuCount[playerid] = count;

    new title[64] = "{MAIN}Tag Manager";

    format_MainColor(playerid, title);
    format_MainColor(playerid, tagList);
    return Dialog_ShowCallback(playerid, using public DIALOG_TAG_MANAGER<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, tagList, "Select", "Close");
}

forward DIALOG_TAG_MANAGER(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_TAG_MANAGER(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;

    if(listitem < 0 || listitem >= g_PlayerTagMenuCount[playerid]) return 1;
    new tagType = g_PlayerTagMenu[playerid][listitem];

    switch(tagType)
    {
        case TAG_TYPE_VIP:
        {
            if(!IsPlayerVip(playerid)) return ShowTagMenu(playerid);

            User[playerid][hideVipTag] = !User[playerid][hideVipTag];
            if(User[playerid][hideVipTag])
            {
                SetPVarInt(playerid, "LastVipHeadTag", User[playerid][vipHeadTag]);
                User[playerid][vipHeadTag] = 10;
            }
            else
            {
                new lastTag = GetPVarInt(playerid, "LastVipHeadTag");
                if(lastTag == 10) lastTag = 0;
                User[playerid][vipHeadTag] = lastTag;
            }

            SetClanColor(playerid);

            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET hide_vip_tag = %d, vip_head_tag = %d WHERE id = %d", User[playerid][hideVipTag], User[playerid][vipHeadTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}VIP Tag is now %s{FFFFFF}.", User[playerid][hideVipTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_CLAN:
        {
            if(!IsPlayerInClan(playerid)) return ShowTagMenu(playerid);

            User[playerid][hideClanTag] = !User[playerid][hideClanTag];
            SetClanColor(playerid);

            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET hide_clan_tag = %d WHERE id = %d", User[playerid][hideClanTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Clan Tag is now %s{FFFFFF}.", User[playerid][hideClanTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_ADMIN:
        {
            if(User[playerid][adminLevel] <= 0) return ShowTagMenu(playerid);

            User[playerid][hideAdminTag] = !User[playerid][hideAdminTag];
            SetClanColor(playerid);

            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET hide_admin_tag = %d WHERE id = %d", User[playerid][hideAdminTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Admin Tag is now %s{FFFFFF}.", User[playerid][hideAdminTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_CUSTOM:
        {
            if(!CanPlayerUseCustomTag(playerid) && User[playerid][customTag][0] == '\0')
            {
                SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
                return ShowTagMenu(playerid);
            }

            if(User[playerid][customTag][0] == '\0')
            {
                return ShowCustomTagOptionsMenu(playerid);
            }

            User[playerid][hideCustomTag] = !User[playerid][hideCustomTag];
            SaveCustomTag(playerid);
            SetClanColor(playerid);

            va_SendClientMessage(playerid, -1, "{MAIN}[TAG] {FFFFFF}Custom Tag is now %s{FFFFFF}.", User[playerid][hideCustomTag] ? "{FF6347}Hidden" : "{23C552}Visible");
            ShowTagMenu(playerid);
        }
        case TAG_TYPE_EDIT_CUSTOM:
        {
            if(!CanPlayerUseCustomTag(playerid))
            {
                SendClientMessage(playerid, -1, "%s", CUSTOM_TAG_NO_PERM);
                return ShowTagMenu(playerid);
            }
            return ShowCustomTagOptionsMenu(playerid);
        }
        case TAG_TYPE_TOURNAMENT:
        {
            return ShowPlayerTournamentTagsDialog(playerid);
        }
    }
    return 1;
}


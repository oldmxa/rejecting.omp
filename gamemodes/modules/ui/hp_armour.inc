#if defined _hp_armour_text_included
    #endinput
#endif
#define _hp_armour_text_included

#include <YSI_Coding\y_hooks>

#define HPTEXT_X       (577.000000)
#define HPTEXT_Y       (67.100000)
#define ARMOURTEXT_X   (577.000000)
#define ARMOURTEXT_Y   (45.100000)

new PlayerText:HpValueTD[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:ArmourValueTD[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

static HpValueLast[MAX_PLAYERS];
static ArmourValueLast[MAX_PLAYERS];

stock bool:HpArmourTextAllowed(playerid)
{
    if(!ServerHpArmourText) return false;
    if(!User[playerid][showHpArmour]) return false;
    if(!IsPlayerLogged(playerid)) return false;
    if(IsPlayerSpectating(playerid)) return false;

    return true;
}

stock DestroyHpArmourText(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;

    if(HpValueTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, HpValueTD[playerid]);
        HpValueTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    if(ArmourValueTD[playerid] != PlayerText:INVALID_TEXT_DRAW)
    {
        PlayerTextDrawDestroy(playerid, ArmourValueTD[playerid]);
        ArmourValueTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    }

    HpValueLast[playerid] = -1;
    ArmourValueLast[playerid] = -1;
    return 1;
}

stock GetHpTextColour(playerid)
{
    new idx = User[playerid][hpTextColor];

    if(idx < 0 || idx >= sizeof(hudShades)) return -1;

    return (hudShades[idx][shadeHex] << 8) | 0xFF;
}

static StyleHpArmourText(playerid, PlayerText:td)
{
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, td, 0.260000, 0.800000);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTRE);
    PlayerTextDrawColor(playerid, td, GetHpTextColour(playerid));
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetProportional(playerid, td, true);
    PlayerTextDrawBackgroundColor(playerid, td, MAIN_BACKGROUND_COLOUR);
    return 1;
}

stock CreateHpArmourText(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(!IsPlayerConnected(playerid)) return 0;

    DestroyHpArmourText(playerid);

    HpValueTD[playerid] = CreatePlayerTextDraw(playerid, HPTEXT_X, HPTEXT_Y, "_");
    StyleHpArmourText(playerid, HpValueTD[playerid]);

    ArmourValueTD[playerid] = CreatePlayerTextDraw(playerid, ARMOURTEXT_X, ARMOURTEXT_Y, "_");
    StyleHpArmourText(playerid, ArmourValueTD[playerid]);
    return 1;
}

stock UpdateHpArmourText(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;

    if(!HpArmourTextAllowed(playerid))
        return DestroyHpArmourText(playerid);

    if(HpValueTD[playerid] == PlayerText:INVALID_TEXT_DRAW)
        CreateHpArmourText(playerid);

    if(HpValueTD[playerid] == PlayerText:INVALID_TEXT_DRAW) return 0;

    new Float:health, Float:armour;
    GetPlayerHealth(playerid, health);
    GetPlayerArmour(playerid, armour);

    new hp = floatround(health), ap = floatround(armour);
    if(hp < 0) hp = 0;
    if(ap < 0) ap = 0;

    new text[16];

    if(hp != HpValueLast[playerid])
    {
        HpValueLast[playerid] = hp;
        format(text, sizeof(text), "%d", hp);
        PlayerTextDrawSetString(playerid, HpValueTD[playerid], text);
        PlayerTextDrawShow(playerid, HpValueTD[playerid]);
    }

    if(ap != ArmourValueLast[playerid])
    {
        ArmourValueLast[playerid] = ap;

        if(ap > 0)
        {
            format(text, sizeof(text), "%d", ap);
            PlayerTextDrawSetString(playerid, ArmourValueTD[playerid], text);
            PlayerTextDrawShow(playerid, ArmourValueTD[playerid]);
        }
        else PlayerTextDrawHide(playerid, ArmourValueTD[playerid]);
    }
    return 1;
}

stock RefreshHpArmourText(playerid)
{
    DestroyHpArmourText(playerid);
    return UpdateHpArmourText(playerid);
}

stock RefreshHpArmourTextForAll()
{
    foreach(new i : Player)
    {
        if(IsPlayerNPC(i)) continue;
        RefreshHpArmourText(i);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    HpValueTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    ArmourValueTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    HpValueLast[playerid] = -1;
    ArmourValueLast[playerid] = -1;
    return 1;
}

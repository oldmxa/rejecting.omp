
#if defined _life_styles_included
    #endinput
#endif
#define _life_styles_included

enum
{
    LIFE_NATIVE = 0,
    LIFE_DOTS,
    LIFE_BLOCKS,
    LIFE_NUMERIC,

    MAX_LIFE_TYPES
}

#define LIFE_DEFAULT LIFE_NATIVE

#if !defined DIALOG_LIFE_TYPE
    #define DIALOG_LIFE_TYPE 14501
#endif

#define LIFE_LABEL_Z_DEFAULT 0.20
#define LIFE_LABEL_DISTANCE  NT_DISTANCE

#define LIFE_COL_HEALTH  "{FFFFFF}"
#define LIFE_COL_ARMOUR  "{FF4444}"
#define LIFE_COL_EMPTY   "{3A3A3A}"

enum e_LIFE_STYLE
{
    lsName[16],
    lsLabel[24],
    lsGlyph[4],
    lsSegments,
    lsArmourRow
}

new const LifeStyle[MAX_LIFE_TYPES][e_LIFE_STYLE] =
{
    {"native",  "SA-MP default", "",  0,  0},
    {"dots",    "Dots",          ".", 16, 1},
    {"blocks",  "Solid bar",     "|", 20, 1},
    {"numeric", "Numeric",       "",  0,  1}
};

new Float:g_LifeLabelZ = LIFE_LABEL_Z_DEFAULT;

static PlayerText3D:LifeLabel[MAX_PLAYERS][MAX_PLAYERS];
static LifeLastHealth[MAX_PLAYERS];
static LifeLastArmour[MAX_PLAYERS];

stock bool:IsValidLifeStyle(style)
{
    return (0 <= style < MAX_LIFE_TYPES);
}

stock bool:UsesCustomLifeStyle(playerid)
{
    return (IsValidLifeStyle(User[playerid][lifeType]) && User[playerid][lifeType] != LIFE_NATIVE);
}

stock bool:CanShowLifeLabel(targetid)
{
    if (!User[targetid][isLogged]) return false;
    if (GetPVarInt(targetid, "HideTab") == 1) return false;

    new PLAYER_STATE:playerState = GetPlayerState(targetid);

    return (playerState != PLAYER_STATE_WASTED && playerState != PLAYER_STATE_NONE);
}

stock GetLifeStyleName(style, destination[], len = sizeof(destination))
{
    format(destination, len, "%s", LifeStyle[IsValidLifeStyle(style) ? style : LIFE_DEFAULT][lsLabel]);
    return 1;
}

static BuildLifeBar(style, Float:value, Float:maximum, const filledColor[], output[], len = sizeof(output))
{
    output[0] = '\0';

    new segments = LifeStyle[style][lsSegments];
    if (segments <= 0 || maximum <= 0.0) return 0;

    if (value < 0.0) value = 0.0;
    if (value > maximum) value = maximum;

    new filled = floatround((value / maximum) * float(segments));
    if (filled < 0) filled = 0;
    if (filled > segments) filled = segments;
    if (filled == 0 && value > 0.0) filled = 1;

    new filledStr[64], emptyStr[64];
    filledStr[0] = '\0';
    emptyStr[0]  = '\0';

    for (new i = 0; i < filled; i++)        strcat(filledStr, LifeStyle[style][lsGlyph]);
    for (new i = filled; i < segments; i++) strcat(emptyStr,  LifeStyle[style][lsGlyph]);

    format(output, len, "%s%s%s%s", filledColor, filledStr, LIFE_COL_EMPTY, emptyStr);
    return 1;
}

static AppendLifeBar(style, Float:value, Float:maximum, const filledColor[], output[], len)
{
    new bar[160];
    if (!BuildLifeBar(style, value, maximum, filledColor, bar, sizeof(bar)))
        return 0;

    strcat(output, "\n", len);
    strcat(output, bar, len);
    return 1;
}

static BuildLifeLabelText(targetid, style, output[], len = sizeof(output))
{
    output[0] = '\0';

    new name[MAX_PLAYER_NAME];
    GetPlayerName(targetid, name, sizeof(name));

    new Float:health, Float:armour;
    GetPlayerHealth(targetid, health);
    GetPlayerArmour(targetid, armour);

    new hp = floatround(health), ap = floatround(armour);
    if (hp < 0) hp = 0;
    if (ap < 0) ap = 0;

    new bool:withArmour = (LifeStyle[style][lsArmourRow] && ap > 0);

    new nameColor = (GetPlayerColor(targetid) >>> 8) & 0xFFFFFF;
    if (!nameColor) nameColor = 0xFFFFFF;

    if (style == LIFE_NUMERIC)
    {
        if (withArmour)
            format(output, len, "{%06x}%s (%d)\n" LIFE_COL_HEALTH "%d " LIFE_COL_ARMOUR "%d", nameColor, name, targetid, hp, ap);
        else
            format(output, len, "{%06x}%s (%d)\n" LIFE_COL_HEALTH "%d", nameColor, name, targetid, hp);

        return 1;
    }

    format(output, len, "{%06x}%s (%d)", nameColor, name, targetid);

    AppendLifeBar(style, health, 100.0, LIFE_COL_HEALTH, output, len);

    if (withArmour)
        AppendLifeBar(style, armour, 100.0, LIFE_COL_ARMOUR, output, len);

    return 1;
}

stock DestroyLifeLabel(viewerid, targetid)
{
    if (LifeLabel[viewerid][targetid] != PlayerText3D:INVALID_3DTEXT_ID)
    {
        DeletePlayer3DTextLabel(viewerid, LifeLabel[viewerid][targetid]);
        LifeLabel[viewerid][targetid] = PlayerText3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

stock RefreshLifeLabel(viewerid, targetid)
{
    if (viewerid == targetid) return 0;
    if (!IsPlayerConnected(viewerid) || !IsPlayerConnected(targetid)) return 0;

    if (!UsesCustomLifeStyle(viewerid) || !CanShowLifeLabel(targetid))
        return DestroyLifeLabel(viewerid, targetid);

    new text[256];
    BuildLifeLabelText(targetid, User[viewerid][lifeType], text, sizeof(text));

    if (LifeLabel[viewerid][targetid] == PlayerText3D:INVALID_3DTEXT_ID)
    {
        LifeLabel[viewerid][targetid] = CreatePlayer3DTextLabel(
            viewerid, text, -1, 0.0, 0.0, g_LifeLabelZ, LIFE_LABEL_DISTANCE,
            targetid, INVALID_VEHICLE_ID, true
        );
    }
    else
    {
        UpdatePlayer3DTextLabelText(viewerid, LifeLabel[viewerid][targetid], -1, text);
    }

    return 1;
}

stock ApplyLifeStyle(viewerid)
{
    if (!IsPlayerConnected(viewerid)) return 0;

    new bool:custom = UsesCustomLifeStyle(viewerid);

    foreach (new targetid : Player)
    {
        if (targetid == viewerid) continue;

        ShowPlayerNameTagForPlayer(viewerid, targetid, !custom);

        if (custom) RefreshLifeLabel(viewerid, targetid);
        else        DestroyLifeLabel(viewerid, targetid);
    }

    return 1;
}

stock RefreshLifeLabelForAll(targetid)
{
    foreach (new viewerid : Player)
    {
        if (viewerid == targetid) continue;
        if (!UsesCustomLifeStyle(viewerid)) continue;

        RefreshLifeLabel(viewerid, targetid);
    }
    return 1;
}

stock SyncLifeLabelTarget(targetid)
{
    foreach (new viewerid : Player)
    {
        if (viewerid == targetid) continue;
        if (!UsesCustomLifeStyle(viewerid)) continue;

        ShowPlayerNameTagForPlayer(viewerid, targetid, false);
        RefreshLifeLabel(viewerid, targetid);
    }
    return 1;
}

stock RebuildAllLifeLabels()
{
    foreach (new viewerid : Player)
    {
        foreach (new targetid : Player)
        {
            if (viewerid == targetid) continue;
            DestroyLifeLabel(viewerid, targetid);
        }

        ApplyLifeStyle(viewerid);
    }
    return 1;
}

stock ShowPlayerLifeTypesList(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    new title[64] = "{MAIN}\xA4 {FFFFFF}Life Bar Styles";
    format_MainColor(playerid, title);

    new body[512];
    body[0] = '\0';

    for (new i = 0; i < MAX_LIFE_TYPES; i++)
    {
        if (User[playerid][lifeType] == i)
            strcat(body, fmat_return("{MAIN}> {FFFFFF}%s\n", LifeStyle[i][lsLabel]), sizeof(body));
        else
            strcat(body, fmat_return("{MAIN}\xA4 {FFFFFF}%s\n", LifeStyle[i][lsLabel]), sizeof(body));
    }

    format_MainColor(playerid, body, sizeof(body));

    return ShowPlayerDialog(playerid, DIALOG_LIFE_TYPE, DIALOG_STYLE_LIST, title, body, "Select", "Back");
}

public OnGameModeInit()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        LifeLastHealth[i] = -1;
        LifeLastArmour[i] = -1;

        for (new j = 0; j < MAX_PLAYERS; j++)
            LifeLabel[i][j] = PlayerText3D:INVALID_3DTEXT_ID;
    }

    #if defined LSTY_OnGameModeInit
        return LSTY_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit LSTY_OnGameModeInit
#if defined LSTY_OnGameModeInit
    forward LSTY_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
    LifeLastHealth[playerid] = -1;
    LifeLastArmour[playerid] = -1;

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        LifeLabel[playerid][i] = PlayerText3D:INVALID_3DTEXT_ID;
        LifeLabel[i][playerid] = PlayerText3D:INVALID_3DTEXT_ID;
    }

    #if defined LSTY_OnPlayerConnect
        return LSTY_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect LSTY_OnPlayerConnect
#if defined LSTY_OnPlayerConnect
    forward LSTY_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    foreach (new viewerid : Player)
    {
        if (viewerid == playerid) continue;
        DestroyLifeLabel(viewerid, playerid);
    }

    for (new i = 0; i < MAX_PLAYERS; i++)
        LifeLabel[playerid][i] = PlayerText3D:INVALID_3DTEXT_ID;

    #if defined LSTY_OnPlayerDisconnect
        return LSTY_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect LSTY_OnPlayerDisconnect
#if defined LSTY_OnPlayerDisconnect
    forward LSTY_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerSpawn(playerid)
{
    ApplyLifeStyle(playerid);
    SyncLifeLabelTarget(playerid);

    #if defined LSTY_OnPlayerSpawn
        return LSTY_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn LSTY_OnPlayerSpawn
#if defined LSTY_OnPlayerSpawn
    forward LSTY_OnPlayerSpawn(playerid);
#endif

public OnPlayerStreamIn(playerid, forplayerid)
{
    if (UsesCustomLifeStyle(forplayerid))
    {
        ShowPlayerNameTagForPlayer(forplayerid, playerid, false);
        RefreshLifeLabel(forplayerid, playerid);
    }

    #if defined LSTY_OnPlayerStreamIn
        return LSTY_OnPlayerStreamIn(playerid, forplayerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerStreamIn
    #undef OnPlayerStreamIn
#else
    #define _ALS_OnPlayerStreamIn
#endif
#define OnPlayerStreamIn LSTY_OnPlayerStreamIn
#if defined LSTY_OnPlayerStreamIn
    forward LSTY_OnPlayerStreamIn(playerid, forplayerid);
#endif

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    foreach (new viewerid : Player)
    {
        if (viewerid == playerid) continue;
        DestroyLifeLabel(viewerid, playerid);
    }

    #if defined LSTY_OnPlayerDeath
        return LSTY_OnPlayerDeath(playerid, killerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath LSTY_OnPlayerDeath
#if defined LSTY_OnPlayerDeath
    forward LSTY_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DIALOG_LIFE_TYPE)
    {
        if (response && IsValidLifeStyle(listitem))
        {
            if (User[playerid][lifeType] == listitem)
            {
                SendClientMessage(playerid, -1, fmat_return("{MAIN}[LIFE] {FFFFFF}You are already using the {MAIN}%s{FFFFFF} style.", LifeStyle[listitem][lsLabel]));
            }
            else
            {
                User[playerid][lifeType] = listitem;
                ApplyLifeStyle(playerid);
                SendClientMessage(playerid, -1, fmat_return("{MAIN}[LIFE] {FFFFFF}Life style set to: {MAIN}%s", LifeStyle[listitem][lsLabel]));
            }
        }
        ShowPersonalizationMenu(playerid);
        return 1;
    }

    #if defined LSTY_OnDialogResponse
        return LSTY_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 0;
    #endif
}
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse LSTY_OnDialogResponse
#if defined LSTY_OnDialogResponse
    forward LSTY_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

forward UpdateLifeLabels();
public UpdateLifeLabels()
{
    foreach (new targetid : Player)
    {
        if (!User[targetid][isLogged]) continue;

        new Float:health, Float:armour;
        GetPlayerHealth(targetid, health);
        GetPlayerArmour(targetid, armour);

        new hp = floatround(health), ap = floatround(armour);

        if (hp == LifeLastHealth[targetid] && ap == LifeLastArmour[targetid]) continue;

        LifeLastHealth[targetid] = hp;
        LifeLastArmour[targetid] = ap;

        RefreshLifeLabelForAll(targetid);
    }
    return 1;
}

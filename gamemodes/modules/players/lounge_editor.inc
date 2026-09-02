#if defined _lounge_editor_included
    #endinput
#endif
#define _lounge_editor_included

/*
    Lounge Editor Module (/elounge)
    Allows authorized administrators to customize lounge spawn, blackboard rank board (with height & 3D editing),
    and TOP 5 ranking actors (Kills & Damage) dynamically in-game.
    Persists configuration into lounge_config.ini.
*/

#define LOUNGE_CONFIG_FILE "lounge_config.ini"

// Forward declarations
forward updateKillRank();
forward UpdateTOP5Kills();
forward UpdateTOP5Damage();
forward InitLoungeTipSystem();

// Re-creates / repositions the lounge blackboard ranking board
stock RespawnLoungeBoard()
{
    new Float:frontX = g_LoungeBoardPos[0] + 0.015 * floatsin(-g_LoungeBoardPos[3], degrees);
    new Float:frontY = g_LoungeBoardPos[1] + 0.015 * floatcos(-g_LoungeBoardPos[3], degrees);

    if (!IsValidDynamicObject(g_BoardBaseObject))
    {
        g_BoardBaseObject = CreateDynamicObject(
            3077,
            g_LoungeBoardPos[0],
            g_LoungeBoardPos[1],
            g_LoungeBoardPos[2],
            0.0000, 0.0000,
            g_LoungeBoardPos[3],
            LOUNGEVW, 0, -1, 300.0, 300.0
        );
    }
    else
    {
        SetDynamicObjectPos(g_BoardBaseObject, g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2]);
        SetDynamicObjectRot(g_BoardBaseObject, 0.0000, 0.0000, g_LoungeBoardPos[3]);
    }

    if (!IsValidDynamicObject(g_Object))
    {
        g_Object = CreateDynamicObject(
            3077,
            frontX,
            frontY,
            g_LoungeBoardPos[2],
            0.0000, 0.0000,
            g_LoungeBoardPos[3],
            LOUNGEVW, 0, -1, 300.0, 300.0
        );
    }
    else
    {
        SetDynamicObjectPos(g_Object, frontX, frontY, g_LoungeBoardPos[2]);
        SetDynamicObjectRot(g_Object, 0.0000, 0.0000, g_LoungeBoardPos[3]);
    }

    if (g_LoungeRankStr[0] != '\0')
    {
        SetDynamicObjectMaterialText(g_Object, 0, g_LoungeRankStr, OBJECT_MATERIAL_SIZE_512x256, "Lucida Console", 18, false, 0xFFD9A0A9, 0, OBJECT_MATERIAL_TEXT_ALIGN_LEFT);
    }
    else
    {
        updateKillRank();
    }
    return 1;
}

// Loads lounge configuration from lounge_config.ini
stock LoadLoungeConfig()
{
    if(!fexist(LOUNGE_CONFIG_FILE))
    {
        SaveLoungeConfig();
        return 1;
    }

    new File:file = fopen(LOUNGE_CONFIG_FILE, io_read);
    if(!file) return 0;

    new line[128], key[32], val[64];
    while(fread(file, line))
    {
        for(new i = strlen(line) - 1; i >= 0; i--)
        {
            if(line[i] == '\r' || line[i] == '\n') line[i] = EOS;
            else break;
        }

        if(sscanf(line, "p<=>s[32]s[64]", key, val)) continue;

        // Spawn
        if(!strcmp(key, "SpawnX", true)) g_LoungeSpawnPos[0] = floatstr(val);
        else if(!strcmp(key, "SpawnY", true)) g_LoungeSpawnPos[1] = floatstr(val);
        else if(!strcmp(key, "SpawnZ", true)) g_LoungeSpawnPos[2] = floatstr(val);
        else if(!strcmp(key, "SpawnA", true)) g_LoungeSpawnPos[3] = floatstr(val);

        // Board
        else if(!strcmp(key, "BoardX", true)) g_LoungeBoardPos[0] = floatstr(val);
        else if(!strcmp(key, "BoardY", true)) g_LoungeBoardPos[1] = floatstr(val);
        else if(!strcmp(key, "BoardZ", true)) g_LoungeBoardPos[2] = floatstr(val);
        else if(!strcmp(key, "BoardA", true)) g_LoungeBoardPos[3] = floatstr(val);

        // Info / Tips NPC
        else if(!strcmp(key, "TipNPC_X", true)) g_LoungeTipPos[0] = floatstr(val);
        else if(!strcmp(key, "TipNPC_Y", true)) g_LoungeTipPos[1] = floatstr(val);
        else if(!strcmp(key, "TipNPC_Z", true)) g_LoungeTipPos[2] = floatstr(val);
        else if(!strcmp(key, "TipNPC_A", true)) g_LoungeTipPos[3] = floatstr(val);

        // Top 5 Kills Actors
        for(new k = 0; k < 5; k++)
        {
            new tempKey[32];
            format(tempKey, sizeof(tempKey), "TopKills%d_X", k);
            if(!strcmp(key, tempKey, true)) { TOP5Kills[k][0] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopKills%d_Y", k);
            if(!strcmp(key, tempKey, true)) { TOP5Kills[k][1] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopKills%d_Z", k);
            if(!strcmp(key, tempKey, true)) { TOP5Kills[k][2] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopKills%d_A", k);
            if(!strcmp(key, tempKey, true)) { TOP5Kills[k][3] = floatstr(val); break; }
        }

        // Top 5 Damage Actors
        for(new d = 0; d < 5; d++)
        {
            new tempKey[32];
            format(tempKey, sizeof(tempKey), "TopDmg%d_X", d);
            if(!strcmp(key, tempKey, true)) { TOP5Damage[d][0] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopDmg%d_Y", d);
            if(!strcmp(key, tempKey, true)) { TOP5Damage[d][1] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopDmg%d_Z", d);
            if(!strcmp(key, tempKey, true)) { TOP5Damage[d][2] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopDmg%d_A", d);
            if(!strcmp(key, tempKey, true)) { TOP5Damage[d][3] = floatstr(val); break; }
        }
    }

    fclose(file);
    return 1;
}

// Saves lounge configuration to lounge_config.ini
stock SaveLoungeConfig()
{
    new File:file = fopen(LOUNGE_CONFIG_FILE, io_write);
    if(!file) return 0;

    new str[128];
    // Spawn
    format(str, sizeof(str), "SpawnX=%.4f\r\nSpawnY=%.4f\r\nSpawnZ=%.4f\r\nSpawnA=%.4f\r\n",
        g_LoungeSpawnPos[0], g_LoungeSpawnPos[1], g_LoungeSpawnPos[2], g_LoungeSpawnPos[3]);
    fwrite(file, str);

    // Board
    format(str, sizeof(str), "BoardX=%.4f\r\nBoardY=%.4f\r\nBoardZ=%.4f\r\nBoardA=%.4f\r\n",
        g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2], g_LoungeBoardPos[3]);
    fwrite(file, str);

    // Info / Tips NPC
    format(str, sizeof(str), "TipNPC_X=%.4f\r\nTipNPC_Y=%.4f\r\nTipNPC_Z=%.4f\r\nTipNPC_A=%.4f\r\n",
        g_LoungeTipPos[0], g_LoungeTipPos[1], g_LoungeTipPos[2], g_LoungeTipPos[3]);
    fwrite(file, str);

    // Top 5 Kills
    for(new k = 0; k < 5; k++)
    {
        format(str, sizeof(str), "TopKills%d_X=%.4f\r\nTopKills%d_Y=%.4f\r\nTopKills%d_Z=%.4f\r\nTopKills%d_A=%.4f\r\n",
            k, TOP5Kills[k][0], k, TOP5Kills[k][1], k, TOP5Kills[k][2], k, TOP5Kills[k][3]);
        fwrite(file, str);
    }

    // Top 5 Damage
    for(new d = 0; d < 5; d++)
    {
        format(str, sizeof(str), "TopDmg%d_X=%.4f\r\nTopDmg%d_Y=%.4f\r\nTopDmg%d_Z=%.4f\r\nTopDmg%d_A=%.4f\r\n",
            d, TOP5Damage[d][0], d, TOP5Damage[d][1], d, TOP5Damage[d][2], d, TOP5Damage[d][3]);
        fwrite(file, str);
    }

    fclose(file);
    return 1;
}

forward DIALOG_EL_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_SPAWN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_BOARD(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP5KILLS_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP5KILLS_OPT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP5DMG_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP5DMG_OPT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TIPNPC(playerid, dialogid, response, listitem, string:inputtext[]);

stock ShowLoungeEditorMenu(playerid)
{
    if(!IsAdmin(playerid, 5))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    new body[512], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor - Main Menu");
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Option\t{MAIN}Description\n"\
        "{FFFFFF}1. Main Spawn\t{B4B5B7}Configure lounge spawn position\n"\
        "{FFFFFF}2. Kills Board\t{B4B5B7}Position, height and 3D mode for rank board\n"\
        "{FFFFFF}3. TOP 5 Kills Actors\t{B4B5B7}Position the 5 top kills actors\n"\
        "{FFFFFF}4. TOP 5 Damage Actors\t{B4B5B7}Position the 5 top damage actors\n"\
        "{FFFFFF}5. Info / Tips NPC\t{B4B5B7}Position server tips & info NPC\n"\
        "{23C552}6. Save & Reload All\t{FFFFFF}Save configuration and reload all entities"
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_EL_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Close");
    return 1;
}

public DIALOG_EL_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return 1;

    switch(listitem)
    {
        case 0: // Main Spawn
        {
            new body[256], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor - Main Spawn");
            format_MainColor(playerid, title);

            format(body, sizeof(body),
                "{MAIN}Action\t{MAIN}Details\n"\
                "{23C552}Set Spawn Here\t{FFFFFF}Use current position and facing angle\n"\
                "{FFFFFF}Teleport to Spawn\t{B4B5B7}Teleport to current lounge spawn\n"\
                "{F3230E}Reset Default Spawn\t{B4B5B7}Restore default coordinates"
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_SPAWN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Confirm", "Back");
        }
        case 1: // Board
        {
            new body[256], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor - Kills Board");
            format_MainColor(playerid, title);

            format(body, sizeof(body),
                "{MAIN}Action\t{MAIN}Details\n"\
                "{MAIN}3D Edit Mode (GUI)\t{FFFFFF}Adjust position/height/rotation with arrows\n"\
                "{23C552}Move Board to Me\t{FFFFFF}Position board at current location and angle\n"\
                "{FFFFFF}Teleport to Board\t{B4B5B7}Teleport to current board position"
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_BOARD<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Confirm", "Back");
        }
        case 2: // TOP 5 Kills
        {
            new body[512], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor - TOP 5 Kills Actors");
            format_MainColor(playerid, title);

            format(body, sizeof(body), "{MAIN}Rank\t{MAIN}Coordinates\n");
            for(new k = 0; k < 5; k++)
            {
                new line[96];
                format(line, sizeof(line), "{FFFFFF}Top Kills #%d\t{BEBEBE}X: %.1f | Y: %.1f | Z: %.1f\n",
                    k + 1, TOP5Kills[k][0], TOP5Kills[k][1], TOP5Kills[k][2]);
                strcat(body, line);
            }
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP5KILLS_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Edit", "Back");
        }
        case 3: // TOP 5 Damage
        {
            new body[512], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor - TOP 5 Damage Actors");
            format_MainColor(playerid, title);

            format(body, sizeof(body), "{MAIN}Rank\t{MAIN}Coordinates\n");
            for(new d = 0; d < 5; d++)
            {
                new line[96];
                format(line, sizeof(line), "{FFFFFF}Top Damage #%d\t{BEBEBE}X: %.1f | Y: %.1f | Z: %.1f\n",
                    d + 1, TOP5Damage[d][0], TOP5Damage[d][1], TOP5Damage[d][2]);
                strcat(body, line);
            }
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP5DMG_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Edit", "Back");
        }
        case 4: // Info / Tips NPC
        {
            new body[256], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor - Info / Tips NPC");
            format_MainColor(playerid, title);

            format(body, sizeof(body),
                "{MAIN}Action\t{MAIN}Details\n"\
                "{23C552}Move NPC to Me\t{FFFFFF}Position tips NPC at current location and angle\n"\
                "{FFFFFF}Teleport to NPC\t{B4B5B7}Teleport to current tips NPC position\n"\
                "{F3230E}Reset Default Position\t{B4B5B7}Restore default coordinates"
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_TIPNPC<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Confirm", "Back");
        }
        case 5: // Save & Reload All
        {
            SaveLoungeConfig();
            RespawnLoungeBoard();
            CreateTOP5Kills();
            UpdateTOP5Kills();
            CreateTOP5Damage();
            UpdateTOP5Damage();
            InitLoungeTipSystem();

            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}All lounge settings saved to {MAIN}lounge_config.ini{FFFFFF} and reloaded!");
            ShowLoungeEditorMenu(playerid);
        }
    }
    return 1;
}

// SPAWN

public DIALOG_EL_SPAWN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // Set Spawn Here
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            g_LoungeSpawnPos[0] = x;
            g_LoungeSpawnPos[1] = y;
            g_LoungeSpawnPos[2] = z;
            g_LoungeSpawnPos[3] = a;

            User[playerid][loungeX] = x;
            User[playerid][loungeY] = y;
            User[playerid][loungeZ] = z;
            User[playerid][loungeA] = a;
            User[playerid][loungeVW] = LOUNGEVW;
            User[playerid][loungeINT] = 0;
            UpdatePlayerData(playerid);

            SaveLoungeConfig();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}New default server spawn set: {MAIN}%.2f, %.2f, %.2f (Angle: %.1f)", x, y, z, a);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 0, empty);
        }
        case 1: // Teleport to Spawn
        {
            SetPlayerPos(playerid, g_LoungeSpawnPos[0], g_LoungeSpawnPos[1], g_LoungeSpawnPos[2]);
            SetPlayerFacingAngle(playerid, g_LoungeSpawnPos[3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to the Lounge Spawn.");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 0, empty);
        }
        case 2: // Reset Default Spawn
        {
            g_LoungeSpawnPos[0] = 934.4449;
            g_LoungeSpawnPos[1] = -1086.8077;
            g_LoungeSpawnPos[2] = 24.2891;
            g_LoungeSpawnPos[3] = 90.5514;

            SaveLoungeConfig();

            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Lounge Spawn reset to server default.");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 0, empty);
        }
    }
    return ShowLoungeEditorMenu(playerid);
}

//  BOARD HANDLER
public DIALOG_EL_BOARD(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // 3D Edit Mode
        {
            if(!IsValidDynamicObject(g_BoardBaseObject)) RespawnLoungeBoard();
            EditDynamicObject(playerid, g_BoardBaseObject);
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}3D Edit Mode enabled! Drag arrows to move/raise/rotate. Click the save icon to confirm.");
            return 1;
        }
        case 1: // Move Board to Me
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            g_LoungeBoardPos[0] = x;
            g_LoungeBoardPos[1] = y;
            g_LoungeBoardPos[2] = z;
            g_LoungeBoardPos[3] = a;

            SaveLoungeConfig();
            RespawnLoungeBoard();
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board repositioned at: {MAIN}%.2f, %.2f, %.2f (Rot: %.1f)", x, y, z, a);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 1, empty);
        }
        case 2: // Teleport to Board
        {
            SetPlayerPos(playerid, g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2]);
            SetPlayerFacingAngle(playerid, g_LoungeBoardPos[3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to the Board position.");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 1, empty);
        }
    }
    return ShowLoungeEditorMenu(playerid);
}

// TOP 5 KILLS HANDLER
public DIALOG_EL_TOP5KILLS_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);
    if(listitem < 0 || listitem >= 5) return ShowLoungeEditorMenu(playerid);

    SetPVarInt(playerid, "EL_SelectedTopKill", listitem);

    new body[256], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor - Top Kills #%d", listitem + 1);
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Action\t{MAIN}Details\n"\
        "{23C552}Move Actor to Me\t{FFFFFF}Position actor #%d at current position and angle\n"\
        "{FFFFFF}Teleport to Actor\t{B4B5B7}Teleport to current actor position",
        listitem + 1
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP5KILLS_OPT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Confirm", "Back");
    return 1;
}

public DIALOG_EL_TOP5KILLS_OPT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        new empty[1];
        return DIALOG_EL_MAIN(playerid, 0, 1, 2, empty);
    }

    new kIdx = GetPVarInt(playerid, "EL_SelectedTopKill");
    if(kIdx < 0 || kIdx >= 5) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // Move Actor to Me
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            TOP5Kills[kIdx][0] = x;
            TOP5Kills[kIdx][1] = y;
            TOP5Kills[kIdx][2] = z;
            TOP5Kills[kIdx][3] = a;

            SaveLoungeConfig();
            CreateTOP5Kills();
            UpdateTOP5Kills();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}TOP Kills Actor #{MAIN}%d{FFFFFF} repositioned successfully!", kIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 2, empty);
        }
        case 1: // Teleport to Actor
        {
            SetPlayerPos(playerid, TOP5Kills[kIdx][0], TOP5Kills[kIdx][1], TOP5Kills[kIdx][2]);
            SetPlayerFacingAngle(playerid, TOP5Kills[kIdx][3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to TOP Kills Actor #{MAIN}%d{FFFFFF}.", kIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 2, empty);
        }
    }
    return 1;
}

// TOP 5 DAMAGE HANDLER

public DIALOG_EL_TOP5DMG_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);
    if(listitem < 0 || listitem >= 5) return ShowLoungeEditorMenu(playerid);

    SetPVarInt(playerid, "EL_SelectedTopDmg", listitem);

    new body[256], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor - Top Damage #%d", listitem + 1);
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Action\t{MAIN}Details\n"\
        "{23C552}Move Actor to Me\t{FFFFFF}Position actor #%d at current position and angle\n"\
        "{FFFFFF}Teleport to Actor\t{B4B5B7}Teleport to current actor position",
        listitem + 1
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP5DMG_OPT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Confirm", "Back");
    return 1;
}

public DIALOG_EL_TOP5DMG_OPT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        new empty[1];
        return DIALOG_EL_MAIN(playerid, 0, 1, 3, empty);
    }

    new dIdx = GetPVarInt(playerid, "EL_SelectedTopDmg");
    if(dIdx < 0 || dIdx >= 5) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // Move Actor to Me
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            TOP5Damage[dIdx][0] = x;
            TOP5Damage[dIdx][1] = y;
            TOP5Damage[dIdx][2] = z;
            TOP5Damage[dIdx][3] = a;

            SaveLoungeConfig();
            CreateTOP5Damage();
            UpdateTOP5Damage();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}TOP Damage Actor #{MAIN}%d{FFFFFF} repositioned successfully!", dIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 3, empty);
        }
        case 1: // Teleport to Actor
        {
            SetPlayerPos(playerid, TOP5Damage[dIdx][0], TOP5Damage[dIdx][1], TOP5Damage[dIdx][2]);
            SetPlayerFacingAngle(playerid, TOP5Damage[dIdx][3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to TOP Damage Actor #{MAIN}%d{FFFFFF}.", dIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 3, empty);
        }
    }
    return 1;
}

// INFO / TIPS NPC HANDLER
public DIALOG_EL_TIPNPC(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // Move NPC to Me
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            g_LoungeTipPos[0] = x;
            g_LoungeTipPos[1] = y;
            g_LoungeTipPos[2] = z;
            g_LoungeTipPos[3] = a;

            SaveLoungeConfig();
            InitLoungeTipSystem();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Info / Tips NPC repositioned at: {MAIN}%.2f, %.2f, %.2f (Angle: %.1f)", x, y, z, a);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 4, empty);
        }
        case 1: // Teleport to NPC
        {
            SetPlayerPos(playerid, g_LoungeTipPos[0], g_LoungeTipPos[1], g_LoungeTipPos[2]);
            SetPlayerFacingAngle(playerid, g_LoungeTipPos[3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to Info / Tips NPC position.");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 4, empty);
        }
        case 2: // Reset Default Position
        {
            g_LoungeTipPos[0] = 919.0244;
            g_LoungeTipPos[1] = -1067.8599;
            g_LoungeTipPos[2] = 24.2992;
            g_LoungeTipPos[3] = 270.0235;

            SaveLoungeConfig();
            InitLoungeTipSystem();

            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Info / Tips NPC reset to default position.");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 4, empty);
        }
    }
    return ShowLoungeEditorMenu(playerid);
}

// /elounge
@cmd(.alts = {"editlounge", "loungeedit"}) elounge(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(!IsAdmin(playerid, 5))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    return ShowLoungeEditorMenu(playerid);
}

public OnGameModeInit()
{
    LoadLoungeConfig();
    RespawnLoungeBoard();

    #if defined LEDT_OnGameModeInit
        return LEDT_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit LEDT_OnGameModeInit
#if defined LEDT_OnGameModeInit
    forward LEDT_OnGameModeInit();
#endif

public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    if(objectid == g_BoardBaseObject || objectid == g_Object)
    {
        if(response == _:EDIT_RESPONSE_FINAL)
        {
            g_LoungeBoardPos[0] = x;
            g_LoungeBoardPos[1] = y;
            g_LoungeBoardPos[2] = z;
            g_LoungeBoardPos[3] = rz;

            SaveLoungeConfig();
            RespawnLoungeBoard();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board saved successfully: {MAIN}X: %.2f | Y: %.2f | Z: %.2f (Rot: %.1f)", x, y, z, rz);
            ShowLoungeEditorMenu(playerid);
        }
        else if(response == _:EDIT_RESPONSE_CANCEL)
        {
            RespawnLoungeBoard();
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}3D board edit canceled.");
            ShowLoungeEditorMenu(playerid);
        }
    }

    #if defined LEDT_OnPlayerEditDynamicObject
        return LEDT_OnPlayerEditDynamicObject(playerid, objectid, response, x, y, z, rx, ry, rz);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerEditDynamicObject
    #undef OnPlayerEditDynamicObject
#else
    #define _ALS_OnPlayerEditDynamicObject
#endif
#define OnPlayerEditDynamicObject LEDT_OnPlayerEditDynamicObject
#if defined LEDT_OnPlayerEditDynamicObject
    forward LEDT_OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
#endif

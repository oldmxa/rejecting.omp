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
forward updateDamageRank();
forward UpdateTOP3Kills();
forward UpdateTOP3Damage();
forward InitLoungeTipSystem();


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
        else if(!strcmp(key, "BoardSpacing", true)) g_LoungeBoardSpacing = floatstr(val);
        else if(!strcmp(key, "BoardHeight", true)) g_LoungeBoardHeight = floatstr(val);
        else if(!strcmp(key, "BoardDepth", true)) g_LoungeBoardDepth = floatstr(val);

        // Info / Tips NPC
        else if(!strcmp(key, "TipNPC_X", true)) g_LoungeTipPos[0] = floatstr(val);
        else if(!strcmp(key, "TipNPC_Y", true)) g_LoungeTipPos[1] = floatstr(val);
        else if(!strcmp(key, "TipNPC_Z", true)) g_LoungeTipPos[2] = floatstr(val);
        else if(!strcmp(key, "TipNPC_A", true)) g_LoungeTipPos[3] = floatstr(val);

        // Top 3 Kills Actors
        for(new k = 0; k < 3; k++)
        {
            new tempKey[32];
            format(tempKey, sizeof(tempKey), "TopKills%d_X", k);
            if(!strcmp(key, tempKey, true)) { TOP3Kills[k][0] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopKills%d_Y", k);
            if(!strcmp(key, tempKey, true)) { TOP3Kills[k][1] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopKills%d_Z", k);
            if(!strcmp(key, tempKey, true)) { TOP3Kills[k][2] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopKills%d_A", k);
            if(!strcmp(key, tempKey, true)) { TOP3Kills[k][3] = floatstr(val); break; }
        }

        // Top 3 Damage Actors
        for(new d = 0; d < 3; d++)
        {
            new tempKey[32];
            format(tempKey, sizeof(tempKey), "TopDmg%d_X", d);
            if(!strcmp(key, tempKey, true)) { TOP3Damage[d][0] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopDmg%d_Y", d);
            if(!strcmp(key, tempKey, true)) { TOP3Damage[d][1] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopDmg%d_Z", d);
            if(!strcmp(key, tempKey, true)) { TOP3Damage[d][2] = floatstr(val); break; }

            format(tempKey, sizeof(tempKey), "TopDmg%d_A", d);
            if(!strcmp(key, tempKey, true)) { TOP3Damage[d][3] = floatstr(val); break; }
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

    new str[256];
    // Spawn
    format(str, sizeof(str), "SpawnX=%.4f\r\nSpawnY=%.4f\r\nSpawnZ=%.4f\r\nSpawnA=%.4f\r\n",
        g_LoungeSpawnPos[0], g_LoungeSpawnPos[1], g_LoungeSpawnPos[2], g_LoungeSpawnPos[3]);
    fwrite(file, str);

    // Board
    format(str, sizeof(str), "BoardX=%.4f\r\nBoardY=%.4f\r\nBoardZ=%.4f\r\nBoardA=%.4f\r\nBoardSpacing=%.4f\r\nBoardHeight=%.4f\r\nBoardDepth=%.4f\r\n",
        g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2], g_LoungeBoardPos[3],
        g_LoungeBoardSpacing, g_LoungeBoardHeight, g_LoungeBoardDepth);
    fwrite(file, str);

    // Info / Tips NPC
    format(str, sizeof(str), "TipNPC_X=%.4f\r\nTipNPC_Y=%.4f\r\nTipNPC_Z=%.4f\r\nTipNPC_A=%.4f\r\n",
        g_LoungeTipPos[0], g_LoungeTipPos[1], g_LoungeTipPos[2], g_LoungeTipPos[3]);
    fwrite(file, str);

    // Top 3 Kills
    for(new k = 0; k < 3; k++)
    {
        format(str, sizeof(str), "TopKills%d_X=%.4f\r\nTopKills%d_Y=%.4f\r\nTopKills%d_Z=%.4f\r\nTopKills%d_A=%.4f\r\n",
            k, TOP3Kills[k][0], k, TOP3Kills[k][1], k, TOP3Kills[k][2], k, TOP3Kills[k][3]);
        fwrite(file, str);
    }

    // Top 3 Damage
    for(new d = 0; d < 3; d++)
    {
        format(str, sizeof(str), "TopDmg%d_X=%.4f\r\nTopDmg%d_Y=%.4f\r\nTopDmg%d_Z=%.4f\r\nTopDmg%d_A=%.4f\r\n",
            d, TOP3Damage[d][0], d, TOP3Damage[d][1], d, TOP3Damage[d][2], d, TOP3Damage[d][3]);
        fwrite(file, str);
    }

    fclose(file);
    return 1;
}

forward DIALOG_EL_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_SPAWN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_BOARD(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_SPACING_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_SPACING_INPUT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP3KILLS_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP3KILLS_OPT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP3DMG_LIST(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TOP3DMG_OPT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_EL_TIPNPC(playerid, dialogid, response, listitem, string:inputtext[]);

stock ShowLoungeEditorMenu(playerid)
{
    if(!IsAdmin(playerid, 5))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    new body[512], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Main Menu");
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Option\t{MAIN}Description\n"\
        "{B4B5B7}\xBB {FFFFFF}Main Spawn\t{BEBEBE}Configure lounge spawn position\n"\
        "{B4B5B7}\xBB {FFFFFF}Top 10 Boards\t{BEBEBE}Position, height & spacing for boards\n"\
        "{B4B5B7}\xBB {FFFFFF}TOP 3 Kills Actors\t{BEBEBE}Position the 3 top kills actors\n"\
        "{B4B5B7}\xBB {FFFFFF}TOP 3 Damage Actors\t{BEBEBE}Position the 3 top damage actors\n"\
        "{B4B5B7}\xBB {FFFFFF}Info / Tips NPC\t{BEBEBE}Position server tips & info NPC\n"\
        "{23C552}\xBB {FFFFFF}Save & Reload All\t{BEBEBE}Save and reload all settings"
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
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Main Spawn");
            format_MainColor(playerid, title);

            format(body, sizeof(body),
                "{MAIN}Action\t{MAIN}Details\n"\
                "{23C552}\xBB {FFFFFF}Set Spawn Here\t{BEBEBE}Use current position and facing angle\n"\
                "{B4B5B7}\xBB {FFFFFF}Teleport to Spawn\t{BEBEBE}Teleport to current lounge spawn\n"\
                "{F84F31}\xBB {FFFFFF}Reset Default Spawn\t{BEBEBE}Restore default spawn coordinates"
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_SPAWN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        }
        case 1: // Board
        {
            new body[450], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Top 10 Boards");
            format_MainColor(playerid, title);

            format(body, sizeof(body),
                "{MAIN}Action\t{MAIN}Details\n"\
                "{00EEFE}\xBB {FFFFFF}3D Edit Mode (GUI)\t{BEBEBE}Adjust position/height/rotation with arrows\n"\
                "{23C552}\xBB {FFFFFF}Move Boards to Me\t{BEBEBE}Position both boards at current location\n"\
                "{B4B5B7}\xBB {FFFFFF}Teleport to Boards\t{BEBEBE}Teleport to current boards position\n"\
                "{00EEFE}\xBB {FFFFFF}Reload Top 10 Data\t{BEBEBE}Refresh Top 10 Kills & Damage from database\n"\
                "{MAIN}\xBB {FFFFFF}Edit Spacing & Offsets\t{BEBEBE}Adjust distance between boards, height & depth"
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_BOARD<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        }
        case 2: // TOP 3 Kills
        {
            new body[512], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}TOP 3 Kills Actors");
            format_MainColor(playerid, title);

            format(body, sizeof(body), "{MAIN}Rank\t{MAIN}Coordinates\n");
            for(new k = 0; k < 3; k++)
            {
                new line[128];
                format(line, sizeof(line), "{B4B5B7}\xBB {FFFFFF}#%d Top Kills\t{BEBEBE}X: %.1f | Y: %.1f | Z: %.1f (A: %.1f)\n",
                    k + 1, TOP3Kills[k][0], TOP3Kills[k][1], TOP3Kills[k][2], TOP3Kills[k][3]);
                strcat(body, line);
            }
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP3KILLS_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        }
        case 3: // TOP 3 Damage
        {
            new body[512], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}TOP 3 Damage Actors");
            format_MainColor(playerid, title);

            format(body, sizeof(body), "{MAIN}Rank\t{MAIN}Coordinates\n");
            for(new d = 0; d < 3; d++)
            {
                new line[128];
                format(line, sizeof(line), "{B4B5B7}\xBB {FFFFFF}#%d Top Damage\t{BEBEBE}X: %.1f | Y: %.1f | Z: %.1f (A: %.1f)\n",
                    d + 1, TOP3Damage[d][0], TOP3Damage[d][1], TOP3Damage[d][2], TOP3Damage[d][3]);
                strcat(body, line);
            }
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP3DMG_LIST<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        }
        case 4: // Info / Tips NPC
        {
            new body[256], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Info / Tips NPC");
            format_MainColor(playerid, title);

            format(body, sizeof(body),
                "{MAIN}Action\t{MAIN}Details\n"\
                "{23C552}\xBB {FFFFFF}Move NPC to Me\t{BEBEBE}Position tips NPC at current location and angle\n"\
                "{B4B5B7}\xBB {FFFFFF}Teleport to NPC\t{BEBEBE}Teleport to current tips NPC position\n"\
                "{F84F31}\xBB {FFFFFF}Reset Default Position\t{BEBEBE}Restore default coordinates"
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_TIPNPC<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
        }
        case 5: // Save & Reload All
        {
            SaveLoungeConfig();
            RespawnLoungeBoard();
            CreateTOP3Kills();
            UpdateTOP3Kills();
            CreateTOP3Damage();
            UpdateTOP3Damage();
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
            if(!IsValidDynamicObject(g_BoardKillsFront) && !IsValidDynamicObject(g_BoardBaseObject)) RespawnLoungeBoard();
            new STREAMER_TAG_OBJECT:editObj = (IsValidDynamicObject(g_BoardKillsFront)) ? (g_BoardKillsFront) : (g_BoardBaseObject);
            EditDynamicObject(playerid, editObj);
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}3D Edit Mode enabled for Top 10 Boards! Drag arrows to move/raise/rotate. Click the save icon to confirm.");
            return 1;
        }
        case 1: // Move Boards to Me
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
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Top 10 Boards (Kills & Damage) repositioned at: {MAIN}%.2f, %.2f, %.2f (Rot: %.1f)", x, y, z, a);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 1, empty);
        }
        case 2: // Teleport to Boards
        {
            SetPlayerPos(playerid, g_LoungeBoardPos[0], g_LoungeBoardPos[1], g_LoungeBoardPos[2]);
            SetPlayerFacingAngle(playerid, g_LoungeBoardPos[3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to Top 10 Boards position.");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 1, empty);
        }
        case 3: // Reload Top 10 Data
        {
            updateKillRank();
            updateDamageRank();
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Top 10 Kills and Top 10 Damage ranks reloaded from database!");
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 1, empty);
        }
        case 4: // Edit Spacing & Offsets
        {
            return ShowBoardSpacingDialog(playerid);
        }
    }
    return ShowLoungeEditorMenu(playerid);
}

// BOARD SPACING & OFFSETS HANDLERS
stock ShowBoardSpacingDialog(playerid)
{
    if(!IsAdmin(playerid, 5))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    new body[600], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Board Spacing & Offsets");
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Setting\t{MAIN}Current Value\t{MAIN}Default\n"\
        "{B4B5B7}\xBB {FFFFFF}Lateral Spacing (Between Boards)\t{MAIN}%.2f m\t{BEBEBE}3.30 m\n"\
        "{B4B5B7}\xBB {FFFFFF}Board Height Offset (On Iron Bar)\t{MAIN}%.2f m\t{BEBEBE}2.00 m\n"\
        "{B4B5B7}\xBB {FFFFFF}Board Depth Offset (Encaixe)\t{MAIN}%.3f m\t{BEBEBE}0.045 m\n"\
        "{F84F31}\xBB {FFFFFF}Reset to Default Spacing\t{BEBEBE}Restore defaults\t{BEBEBE}-\n"\
        "{23C552}\xBB {FFFFFF}Save & Apply Changes\t{BEBEBE}Save to INI & reload\t{BEBEBE}-",
        g_LoungeBoardSpacing, g_LoungeBoardHeight, g_LoungeBoardDepth
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_EL_SPACING_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

public DIALOG_EL_SPACING_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        new empty[1];
        return DIALOG_EL_MAIN(playerid, 0, 1, 1, empty);
    }

    switch(listitem)
    {
        case 0: // Lateral Spacing
        {
            SetPVarInt(playerid, "EL_SpacingType", 1);
            new body[300], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Lateral Spacing");
            format_MainColor(playerid, title);
            format(body, sizeof(body),
                "{FFFFFF}Enter new lateral spacing between the boards/iron bars in meters:\n\n"\
                "{FFFFFF}\xBB {B4B5B7}Current: {MAIN}%.2f m\n"\
                "{FFFFFF}\xBB {B4B5B7}Default: {MAIN}3.30 m\n"\
                "{FFFFFF}\xBB {B4B5B7}Valid Range: {MAIN}0.5 - 20.0 m",
                g_LoungeBoardSpacing
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_SPACING_INPUT<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Back");
        }
        case 1: // Height Offset
        {
            SetPVarInt(playerid, "EL_SpacingType", 2);
            new body[300], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Board Height Offset");
            format_MainColor(playerid, title);
            format(body, sizeof(body),
                "{FFFFFF}Enter new height offset of the board on the iron bar in meters:\n\n"\
                "{FFFFFF}\xBB {B4B5B7}Current: {MAIN}%.2f m\n"\
                "{FFFFFF}\xBB {B4B5B7}Default: {MAIN}2.00 m\n"\
                "{FFFFFF}\xBB {B4B5B7}Valid Range: {MAIN}-5.0 - 10.0 m",
                g_LoungeBoardHeight
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_SPACING_INPUT<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Back");
        }
        case 2: // Depth Offset
        {
            SetPVarInt(playerid, "EL_SpacingType", 3);
            new body[300], title[64];
            format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Board Depth Offset");
            format_MainColor(playerid, title);
            format(body, sizeof(body),
                "{FFFFFF}Enter new depth / encaixe offset of the board relative to the iron bar:\n\n"\
                "{FFFFFF}\xBB {B4B5B7}Current: {MAIN}%.3f m\n"\
                "{FFFFFF}\xBB {B4B5B7}Default: {MAIN}0.045 m\n"\
                "{FFFFFF}\xBB {B4B5B7}Valid Range: {MAIN}-2.0 - 5.0 m",
                g_LoungeBoardDepth
            );
            format_MainColor(playerid, body);
            Dialog_ShowCallback(playerid, using public DIALOG_EL_SPACING_INPUT<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Back");
        }
        case 3: // Reset to Default
        {
            g_LoungeBoardSpacing = 3.3;
            g_LoungeBoardHeight = 2.0;
            g_LoungeBoardDepth = 0.045;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board spacing and offsets reset to default values (Spacing: 3.30m, Height: 2.00m, Depth: 0.045m)!");
            return ShowBoardSpacingDialog(playerid);
        }
        case 4: // Save & Apply
        {
            SaveLoungeConfig();
            RespawnLoungeBoard();
            SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board spacing & offsets saved and applied successfully!");
            return ShowBoardSpacingDialog(playerid);
        }
    }
    return 1;
}

public DIALOG_EL_SPACING_INPUT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        DeletePVar(playerid, "EL_SpacingType");
        return ShowBoardSpacingDialog(playerid);
    }

    new type = GetPVarInt(playerid, "EL_SpacingType");
    DeletePVar(playerid, "EL_SpacingType");

    if(IsNull(inputtext))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must enter a valid numeric value.");
        return ShowBoardSpacingDialog(playerid);
    }

    new Float:val;
    if(sscanf(inputtext, "f", val))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must enter a valid numeric value.");
        return ShowBoardSpacingDialog(playerid);
    }

    switch(type)
    {
        case 1: // Lateral Spacing
        {
            if(val < 0.1 || val > 50.0)
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Spacing must be between 0.1 and 50.0 meters.");
                return ShowBoardSpacingDialog(playerid);
            }
            g_LoungeBoardSpacing = val;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Lateral board spacing updated to {MAIN}%.2f m{FFFFFF}.", val);
        }
        case 2: // Height Offset
        {
            if(val < -10.0 || val > 20.0)
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Height offset must be between -10.0 and 20.0 meters.");
                return ShowBoardSpacingDialog(playerid);
            }
            g_LoungeBoardHeight = val;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board height offset updated to {MAIN}%.2f m{FFFFFF}.", val);
        }
        case 3: // Depth Offset
        {
            if(val < -5.0 || val > 10.0)
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Depth offset must be between -5.0 and 10.0 meters.");
                return ShowBoardSpacingDialog(playerid);
            }
            g_LoungeBoardDepth = val;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board depth offset updated to {MAIN}%.3f m{FFFFFF}.", val);
        }
    }

    return ShowBoardSpacingDialog(playerid);
}

// TOP 3 KILLS HANDLER
public DIALOG_EL_TOP3KILLS_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);
    if(listitem < 0 || listitem >= 3) return ShowLoungeEditorMenu(playerid);

    SetPVarInt(playerid, "EL_SelectedTopKill", listitem);

    new body[256], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Top Kills #%d", listitem + 1);
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Action\t{MAIN}Details\n"\
        "{23C552}\xBB {FFFFFF}Move Actor to Me\t{BEBEBE}Position actor #%d at current position and angle\n"\
        "{B4B5B7}\xBB {FFFFFF}Teleport to Actor\t{BEBEBE}Teleport to current actor position",
        listitem + 1
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP3KILLS_OPT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

public DIALOG_EL_TOP3KILLS_OPT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        new empty[1];
        return DIALOG_EL_MAIN(playerid, 0, 1, 2, empty);
    }

    new kIdx = GetPVarInt(playerid, "EL_SelectedTopKill");
    if(kIdx < 0 || kIdx >= 3) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // Move Actor to Me
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            TOP3Kills[kIdx][0] = x;
            TOP3Kills[kIdx][1] = y;
            TOP3Kills[kIdx][2] = z;
            TOP3Kills[kIdx][3] = a;

            SaveLoungeConfig();
            CreateTOP3Kills();
            UpdateTOP3Kills();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}TOP Kills Actor #{MAIN}%d{FFFFFF} repositioned successfully!", kIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 2, empty);
        }
        case 1: // Teleport to Actor
        {
            SetPlayerPos(playerid, TOP3Kills[kIdx][0], TOP3Kills[kIdx][1], TOP3Kills[kIdx][2]);
            SetPlayerFacingAngle(playerid, TOP3Kills[kIdx][3]);
            SetPlayerVirtualWorld(playerid, LOUNGEVW);
            SetPlayerInterior(playerid, 0);
            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Teleported to TOP Kills Actor #{MAIN}%d{FFFFFF}.", kIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 2, empty);
        }
    }
    return 1;
}

// TOP 3 DAMAGE HANDLER

public DIALOG_EL_TOP3DMG_LIST(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response) return ShowLoungeEditorMenu(playerid);
    if(listitem < 0 || listitem >= 3) return ShowLoungeEditorMenu(playerid);

    SetPVarInt(playerid, "EL_SelectedTopDmg", listitem);

    new body[256], title[64];
    format(title, sizeof(title), "{MAIN}Lounge Editor: {FFFFFF}Top Damage #%d", listitem + 1);
    format_MainColor(playerid, title);

    format(body, sizeof(body),
        "{MAIN}Action\t{MAIN}Details\n"\
        "{23C552}\xBB {FFFFFF}Move Actor to Me\t{BEBEBE}Position actor #%d at current position and angle\n"\
        "{B4B5B7}\xBB {FFFFFF}Teleport to Actor\t{BEBEBE}Teleport to current actor position",
        listitem + 1
    );

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_EL_TOP3DMG_OPT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

public DIALOG_EL_TOP3DMG_OPT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        new empty[1];
        return DIALOG_EL_MAIN(playerid, 0, 1, 3, empty);
    }

    new dIdx = GetPVarInt(playerid, "EL_SelectedTopDmg");
    if(dIdx < 0 || dIdx >= 3) return ShowLoungeEditorMenu(playerid);

    switch(listitem)
    {
        case 0: // Move Actor to Me
        {
            new Float:x, Float:y, Float:z, Float:a;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            TOP3Damage[dIdx][0] = x;
            TOP3Damage[dIdx][1] = y;
            TOP3Damage[dIdx][2] = z;
            TOP3Damage[dIdx][3] = a;

            SaveLoungeConfig();
            CreateTOP3Damage();
            UpdateTOP3Damage();

            va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}TOP Damage Actor #{MAIN}%d{FFFFFF} repositioned successfully!", dIdx + 1);
            new empty[1];
            return DIALOG_EL_MAIN(playerid, 0, 1, 3, empty);
        }
        case 1: // Teleport to Actor
        {
            SetPlayerPos(playerid, TOP3Damage[dIdx][0], TOP3Damage[dIdx][1], TOP3Damage[dIdx][2]);
            SetPlayerFacingAngle(playerid, TOP3Damage[dIdx][3]);
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

// /boardspacing
@cmd(.alts = {"boardoffset", "quadrolounge", "editboardspacing", "espacamentoquadro", "espacamentolounge"}) boardspacing(playerid, params[], help)
{
    if(!IsPlayerLogged(playerid))
        return SendClientMessage(playerid, -1, "%s", IS_NOT_CONNECTED);

    if(!IsAdmin(playerid, 5))
        return SendClientMessage(playerid, -1, "%s", NOT_ADMIN_MSG);

    CheckAdminAuth(playerid);

    if(IsNull(params))
    {
        return ShowBoardSpacingDialog(playerid);
    }

    new subcmd[32], Float:val1, Float:val2, Float:val3;
    if(!strcmp(params, "reset", true))
    {
        g_LoungeBoardSpacing = 3.3;
        g_LoungeBoardHeight = 2.0;
        g_LoungeBoardDepth = 0.045;
        SaveLoungeConfig();
        RespawnLoungeBoard();
        return SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board spacing and offsets reset to default (Spacing: 3.30m, Height: 2.00m, Depth: 0.045m)!");
    }

    if(!sscanf(params, "s[32]f", subcmd, val1))
    {
        if(!strcmp(subcmd, "lateral", true) || !strcmp(subcmd, "dist", true) || !strcmp(subcmd, "distancia", true) || !strcmp(subcmd, "spacing", true))
        {
            if(val1 < 0.1 || val1 > 50.0)
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Spacing must be between 0.1 and 50.0 meters.");

            g_LoungeBoardSpacing = val1;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            return va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Lateral board spacing updated to {MAIN}%.2f m{FFFFFF}.", val1);
        }
        else if(!strcmp(subcmd, "altura", true) || !strcmp(subcmd, "z", true) || !strcmp(subcmd, "height", true))
        {
            if(val1 < -10.0 || val1 > 20.0)
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Height offset must be between -10.0 and 20.0 meters.");

            g_LoungeBoardHeight = val1;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            return va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board height offset updated to {MAIN}%.2f m{FFFFFF}.", val1);
        }
        else if(!strcmp(subcmd, "profundidade", true) || !strcmp(subcmd, "depth", true) || !strcmp(subcmd, "offset", true) || !strcmp(subcmd, "encaixe", true))
        {
            if(val1 < -5.0 || val1 > 10.0)
                return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Depth offset must be between -5.0 and 10.0 meters.");

            g_LoungeBoardDepth = val1;
            SaveLoungeConfig();
            RespawnLoungeBoard();
            return va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board depth offset updated to {MAIN}%.3f m{FFFFFF}.", val1);
        }
    }

    // Direct numeric floats
    if(!sscanf(params, "fff", val1, val2, val3))
    {
        if(val1 < 0.1 || val1 > 50.0)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Spacing must be between 0.1 and 50.0 meters.");
        if(val2 < -10.0 || val2 > 20.0)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Height offset must be between -10.0 and 20.0 meters.");
        if(val3 < -5.0 || val3 > 10.0)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Depth offset must be between -5.0 and 10.0 meters.");

        g_LoungeBoardSpacing = val1;
        g_LoungeBoardHeight = val2;
        g_LoungeBoardDepth = val3;
        SaveLoungeConfig();
        RespawnLoungeBoard();
        return va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board offsets updated: Spacing: {MAIN}%.2f m{FFFFFF} | Height: {MAIN}%.2f m{FFFFFF} | Depth: {MAIN}%.3f m{FFFFFF}.", g_LoungeBoardSpacing, g_LoungeBoardHeight, g_LoungeBoardDepth);
    }
    else if(!sscanf(params, "ff", val1, val2))
    {
        if(val1 < 0.1 || val1 > 50.0)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Spacing must be between 0.1 and 50.0 meters.");
        if(val2 < -10.0 || val2 > 20.0)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Height offset must be between -10.0 and 20.0 meters.");

        g_LoungeBoardSpacing = val1;
        g_LoungeBoardHeight = val2;
        SaveLoungeConfig();
        RespawnLoungeBoard();
        return va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Board offsets updated: Spacing: {MAIN}%.2f m{FFFFFF} | Height: {MAIN}%.2f m{FFFFFF}.", g_LoungeBoardSpacing, g_LoungeBoardHeight);
    }
    else if(!sscanf(params, "f", val1))
    {
        if(val1 < 0.1 || val1 > 50.0)
            return SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Spacing must be between 0.1 and 50.0 meters.");

        g_LoungeBoardSpacing = val1;
        SaveLoungeConfig();
        RespawnLoungeBoard();
        return va_SendClientMessage(playerid, -1, "{MAIN}[LOUNGE] {FFFFFF}Lateral board spacing updated to {MAIN}%.2f m{FFFFFF}.", val1);
    }

    return SendClientMessage(playerid, -1, "{MAIN}[HELP] {FFFFFF}Usage: /boardspacing [spacing] (height) (depth) OR /boardspacing [lateral/altura/profundidade] [value]");
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
    if(objectid == g_BoardBaseObject || objectid == g_Object || objectid == g_BoardKillsFront || objectid == g_BoardKillsPillar || objectid == g_BoardDamageFront || objectid == g_BoardDamagePillar)
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

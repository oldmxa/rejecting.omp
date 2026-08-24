#if defined _hooks_included
	#endinput
#endif
#define _hooks_included

#include <YSI_Coding/y_hooks>
#include <YSI_Coding/y_va>

// CoresDoBglh Global Processor

stock GetPlayerMainHex(playerid, dest[], len = sizeof(dest))
{
    if(playerid < 0 || playerid >= MAX_PLAYERS)
    {
        format(dest, len, "FFFFFF");
        return 1;
    }

    new mainVal = User[playerid][chatMainColor];
    if(1 <= mainVal <= sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[mainVal - 1][shadeHex]);
        return 1;
    }
    else if(mainVal & 0x10000000)
    {
        format(dest, len, "%06x", mainVal & 0xFFFFFF);
        return 1;
    }

    // Default / 0 / -1: Follows HUD primary shade or HUD preset color
    new shade = User[playerid][hudShadeOne];
    if(0 <= shade < sizeof(hudShades))
    {
        format(dest, len, "%06x", hudShades[shade][shadeHex]);
        return 1;
    }

    new idx = User[playerid][hudColor];
    if(idx < 0 || idx >= sizeof(CoresDoBglh)) idx = 0;

    format(dest, len, "%s", CoresDoBglh[idx]);
    return 1;
}

forward CB_ProcessMessage(playerid, buffer[], len);
public CB_ProcessMessage(playerid, buffer[], len)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    
    new pos;
    while((pos = strfind(buffer, "{MAIN}")) != -1)
    {
        new hex[7], tag[16];
        GetPlayerMainHex(playerid, hex);
        format(tag, sizeof(tag), "{%s}", hex);
        
        strdel(buffer, pos, pos + 6);
        strins(buffer, tag, pos, len);
    }
    return 1;
}

// Hook SendClientMessage
stock CB_SendClientMessage(playerid, color, const fmat[], va_args<>)
{
    if(!IsPlayerConnected(playerid)) return 0;
    
    new buffer[1024];
    va_format(buffer, sizeof(buffer), fmat, va_start<3>);
    CB_ProcessMessage(playerid, buffer, sizeof(buffer));
    
    #if defined _ALS_SendClientMessage
        return SendClientMessage(playerid, color, buffer);
    #else
        return SendClientMessage(playerid, color, buffer);
    #endif
}

#if defined _ALS_SendClientMessage
    #undef SendClientMessage
#else
    #define _ALS_SendClientMessage
#endif
#define SendClientMessage CB_SendClientMessage

// Hook SendClientMessageToAll
stock CB_SendClientMessageToAll(color, const fmat[], va_args<>)
{
    new buffer[1024];
    va_format(buffer, sizeof(buffer), fmat, va_start<2>);

    foreach(new i : Player)
    {
        new player_buf[1024];
        strcopy(player_buf, buffer);
        
        if (strfind(player_buf, "{MAIN}") != -1)
        {
            CB_ProcessMessage(i, player_buf, sizeof(player_buf));
        }
        
        SendClientMessage(i, color, "%s", player_buf);
    }
    return 1;
}

#if defined _ALS_SendClientMessageToAll
    #undef SendClientMessageToAll
#else
    #define _ALS_SendClientMessageToAll
#endif
#define SendClientMessageToAll CB_SendClientMessageToAll

// Hook ShowPlayerDialog
stock CB_ShowPlayerDialog(playerid, dialogid, style, const title[], const body[], const button1[], const button2[])
{
    if(!IsPlayerConnected(playerid)) return 0;

    new t[128], b[4096];
    strcopy(t, title);
    strcopy(b, body);
    
    CB_ProcessMessage(playerid, t, sizeof(t));
    CB_ProcessMessage(playerid, b, sizeof(b));
    
    #if defined _ALS_ShowPlayerDialog
        return ShowPlayerDialog(playerid, dialogid, style, t, b, button1, button2);
    #else
        return ShowPlayerDialog(playerid, dialogid, style, t, b, button1, button2);
    #endif
}

#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog CB_ShowPlayerDialog


new playerCmdCooldown[MAX_PLAYERS];

#if !defined MAX_CLIENT_MESSAGE
    #define MAX_CLIENT_MESSAGE (144)
#endif

static stock CarryColorCode(const message[], end, dest[], size = sizeof(dest))
{
    dest[0] = EOS;

    for(new i = end - 1; i >= 0; i--)
    {
        if(message[i] != '{') continue;

        if(i + 7 < end && message[i + 7] == '}')
        {
            strmid(dest, message, i, i + 8, size);
            return 1;
        }

        if(i + 6 <= end && !strcmp(message[i], "{MAIN}", false, 6))
        {
            strmid(dest, message, i, i + 6, size);
            return 1;
        }
    }
    return 0;
}

stock SendWrappedClientMessage(playerid, color, const message[])
{
    new len = strlen(message);

    new mainCount, mainPos = -1;
    while((mainPos = strfind(message, "{MAIN}", false, mainPos + 1)) != -1)
        mainCount++;

    new reserved = mainCount * 2;

    if(reserved > 64)
        reserved = 64;

    if(len + reserved <= MAX_CLIENT_MESSAGE)
        return SendClientMessage(playerid, color, "%s", message);

    new carry[9], line[MAX_CLIENT_MESSAGE + 1], piece[MAX_CLIENT_MESSAGE + 1];
    new start = 0, ret = 1;

    while(start < len)
    {
        new prefix = strlen(carry);
        new room = MAX_CLIENT_MESSAGE - prefix - reserved;

        if(room < 1)
            room = 1;

        new end = start + room;

        if(end > len)
        {
            end = len;
        }
        else
        {
            for(new i = end; i > start + (room / 3); i--)
            {
                if(message[i] == ' ')
                {
                    end = i;
                    break;
                }
            }
        }

        strmid(piece, message, start, end, sizeof(piece));

        line[0] = EOS;
        if(prefix)
            strcat(line, carry);
        strcat(line, piece);

        ret = SendClientMessage(playerid, color, "%s", line);

        CarryColorCode(message, end, carry, sizeof(carry));

        start = end;
        while(message[start] == ' ')
            start++;
    }
    return ret;
}

stock SendWrappedClientMessageToAll(color, const message[])
{
    foreach(new i : Player)
        SendWrappedClientMessage(i, color, message);

    return 1;
}

// Hook va_SendClientMessage (YSI)
stock CB_va_SendClientMessage(playerid, color, const fmat[], va_args<>)
{
    new buffer[1024];
    va_format(buffer, sizeof(buffer), fmat, va_start<3>);
    return SendWrappedClientMessage(playerid, color, buffer); 
}

#if defined _ALS_va_SendClientMessage
    #undef va_SendClientMessage
#else
    #define _ALS_va_SendClientMessage
#endif
#define va_SendClientMessage CB_va_SendClientMessage

// Hook va_SendClientMessageToAll (YSI)
stock CB_va_SendClientMessageToAll(color, const fmat[], va_args<>)
{
    new buffer[1024];
    va_format(buffer, sizeof(buffer), fmat, va_start<2>);
    return SendWrappedClientMessageToAll(color, buffer);
}

#if defined _ALS_va_SendClientMessageToAll
    #undef va_SendClientMessageToAll
#else
    #define _ALS_va_SendClientMessageToAll
#endif
#define va_SendClientMessageToAll CB_va_SendClientMessageToAll

// Hook va_ShowPlayerDialog (YSI)
stock CB_va_ShowPlayerDialog(playerid, dialogid, style, const title[], const body[], const button1[], const button2[], va_args<>)
{
    new b[4096];
    va_format(b, sizeof(b), body, va_start<7>);
    return ShowPlayerDialog(playerid, dialogid, style, title, b, button1, button2);
}

#if defined _ALS_va_ShowPlayerDialog
    #undef va_ShowPlayerDialog
#else
    #define _ALS_va_ShowPlayerDialog
#endif
#define va_ShowPlayerDialog CB_va_ShowPlayerDialog

hook OnGameModeInit()
{
    User[MAX_PLAYERS][user_sqlid] = 0;
    User[MAX_PLAYERS][adminLevel] = 0;
    User[MAX_PLAYERS][playerMoney] = 0;
    User[MAX_PLAYERS][playerCoins] = 0;
    User[MAX_PLAYERS][playerLevel] = 1;
    User[MAX_PLAYERS][playerKills] = 0;
    User[MAX_PLAYERS][playerDeaths] = 0;
    User[MAX_PLAYERS][playerExp] = 0;
    User[MAX_PLAYERS][playerSkin] = 0;
    User[MAX_PLAYERS][isLogged] = false;
    User[MAX_PLAYERS][isAdminLogged] = false;
    User[MAX_PLAYERS][isAfk] = false;
    User[MAX_PLAYERS][isSpectating] = false;
    User[MAX_PLAYERS][warnings] = 0;
    User[MAX_PLAYERS][killsSession] = 0;
    User[MAX_PLAYERS][deathsSession] = 0;
    User[MAX_PLAYERS][dmgSession] = 0.0;
    User[MAX_PLAYERS][maxStreak] = 0;
    User[MAX_PLAYERS][wonArenas] = 0;
    User[MAX_PLAYERS][duelWins] = 0;
    User[MAX_PLAYERS][duelLosses] = 0;
    User[MAX_PLAYERS][duel2v2Wins] = 0;
    User[MAX_PLAYERS][duel2v2Losses] = 0;
    User[MAX_PLAYERS][fightStyle] = FIGHT_STYLE_NORMAL;
    User[MAX_PLAYERS][weather] = 10;
    User[MAX_PLAYERS][time] = 10;
    User[MAX_PLAYERS][hudColor] = 0;
    User[MAX_PLAYERS][hudShadeOne] = -1;
    User[MAX_PLAYERS][hudShadeTwo] = -1;
    User[MAX_PLAYERS][chatColor] = 0;
    User[MAX_PLAYERS][chatIDColor] = 0;
    User[MAX_PLAYERS][chatMainColor] = 0;
    User[MAX_PLAYERS][clanID] = -1;
    User[MAX_PLAYERS][clanRole] = -1;
    User[MAX_PLAYERS][clanIDInvite] = -1;
    User[MAX_PLAYERS][locationId] = -1;
    User[MAX_PLAYERS][hitmarkTimer] = -1;
    User[MAX_PLAYERS][lastDamageTime] = -1;
    User[MAX_PLAYERS][lastDamagerId] = INVALID_PLAYER_ID;
    User[MAX_PLAYERS][hitmarkColor] = 0xffffff60;
    User[MAX_PLAYERS][loginTime] = gettime();
    User[MAX_PLAYERS][lastTimeUpdated] = gettime();
    User[MAX_PLAYERS][timerRespawn] = gettime();
    User[MAX_PLAYERS][showPlayerHud] = true;
    User[MAX_PLAYERS][showConnectMsg] = true;
    User[MAX_PLAYERS][hideGlobalMsg] = false;
    User[MAX_PLAYERS][hideWelcomeMsg] = false;
    User[MAX_PLAYERS][show3DDeathTag] = false;
    User[MAX_PLAYERS][showRandomDeathTags] = true;
    User[MAX_PLAYERS][hitSoundID] = 17802;
    User[MAX_PLAYERS][killSoundID] = 1057;
    User[MAX_PLAYERS][dmgInformerStyle] = 1;
    User[MAX_PLAYERS][loungeX] = 1209.1161;
    User[MAX_PLAYERS][loungeY] = -2036.4353;
    User[MAX_PLAYERS][loungeZ] = 69.0078;
    User[MAX_PLAYERS][loungeA] = 91.1024;
    User[MAX_PLAYERS][loungeVW] = 909;
    User[MAX_PLAYERS][loungeINT] = 0;
    format(User[MAX_PLAYERS][dc_username], 100, "-1");
    format(User[MAX_PLAYERS][dc_discriminator], 100, "-1");
    format(User[MAX_PLAYERS][dc_uid], 100, "-1");
    format(User[MAX_PLAYERS][dc_code], 100, "-1");
    // dc_codetimer is an int (timestamp), not one of the 100-cell dc_* arrays: a
    // format() call here writes 3 cells and corrupts acfg_pm/acfg_clanpm, which
    // come immediately after it in the enum and are copied to every player.
    User[MAX_PLAYERS][dc_codetimer] = -1;
    playerSpecID[MAX_PLAYERS] = INVALID_PLAYER_ID;
    return 1;
}

forward EnviarMsgCheat(playerid);
public EnviarMsgCheat(playerid)
{
    if(playerid >= 0 && playerid < MAX_PLAYERS)
    {
        MsgCheat[playerid] = false;
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    if (IsPlayerNPC(playerid)) return 1;

    User[playerid][adminLevel] = 0;
    User[playerid][hudColor] = 0;
    User[playerid][hudShadeOne] = -1;
    User[playerid][hudShadeTwo] = -1;
    User[playerid][chatColor] = 0;
    User[playerid][chatIDColor] = 0;
    User[playerid][chatMainColor] = 0;
    User[playerid][vipLevel] = 0;
    User[playerid][vipExpire] = 0;
    User[playerid][vipHeadTag] = 1;
    User[playerid][clanID] = -1;
    User[playerid][clanRole] = -1;
    User[playerid][hideGlobalMsg] = false;
    User[playerid][hideVipChat] = false;
    User[playerid][hideVipTag] = false;
    User[playerid][hideClanTag] = false;
    User[playerid][hideAdminTag] = true;
    User[playerid][spawnSafehouse] = false;
    User[playerid][hideCustomTag] = false;
    User[playerid][customTagPerm] = false;
    User[playerid][customTag][0] = '\0';
    ResetPlayerTagMenu(playerid);
    User[playerid][lastLocation] = -1;
    MsgCheat[playerid] = false;
    
    User[playerid][loginAttempts] = 0;
    User[playerid][loginTimer] = SetTimerEx("LoginTimeout", 120000, false, "i", playerid);

    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if (IsPlayerNPC(playerid)) return 1;

    MsgCheat[playerid] = false;
    if(ClanLabel[playerid] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(ClanLabel[playerid]);
        ClanLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    if(VipLabel[playerid] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(VipLabel[playerid]);
        VipLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    if(User[playerid][isAfk] && pVehicle[playerid] == vehicleid)
    {
        DestroyVehicle(vehicleid);
        pVehicle[playerid] = 999;
        ClearAnimations(playerid);
        SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Vehicle deleted.");
        return 1;
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if((newkeys & KEY_FIRE) && IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(GetVehicleComponentInSlot(vehicleid, CARMODTYPE_NITRO) != 0)
        {
            AddVehicleComponent(vehicleid, 1010);
            RepairVehicle(vehicleid);
            SetVehicleHealth(vehicleid, 1000.0);
        }
    }
    return 1;
}
#if defined _vip_included
    #endinput
#endif
#define _vip_included

enum E_VIP_PACKAGE {
    vip_days,
    vip_price_money,
    vip_price_coins
}

static const VipPackages[][E_VIP_PACKAGE] = {
    { 15, 1000000, 1500},
    { 30, 2000000, 2800},
    { 45, 3000000, 4000},
    { 60, 4000000, 5000},
    {120, 8000000, 9000}
};

// VipPackages e static; o bot do Discord le os pacotes por aqui.
stock GetVipPackageCount()      return sizeof(VipPackages);
stock GetVipPackageDays(idx)    return (0 <= idx < sizeof(VipPackages)) ? VipPackages[idx][vip_days]        : 0;
stock GetVipPackageMoney(idx)   return (0 <= idx < sizeof(VipPackages)) ? VipPackages[idx][vip_price_money] : 0;
stock GetVipPackageCoins(idx)   return (0 <= idx < sizeof(VipPackages)) ? VipPackages[idx][vip_price_coins] : 0;

#define VIP_PAY_MONEY (0)
#define VIP_PAY_COINS (1)

forward DIALOG_VIP_PAYMENT(playerid, dialogid, response, listitem, string:inputtext[]);


stock IsPlayerVip(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(!User[playerid][isLogged]) return 0;

    if(User[playerid][vipLevel] > 0)
    {
        if(User[playerid][vipExpire] > 0 && gettime() >= User[playerid][vipExpire])
        {
            User[playerid][vipLevel] = 0;
            User[playerid][vipExpire] = 0;
            User[playerid][vipHeadTag] = 0;
            SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP subscription has expired. Renew it with {MAIN}'/buyvip'{FFFFFF}.");
            
            if(VipLabel[playerid] != Text3D:INVALID_3DTEXT_ID)
            {
                DestroyDynamic3DTextLabel(VipLabel[playerid]);
                VipLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
            }
            
            new query[128];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_level = 0, vip_expire = 0, vip_head_tag = 0 WHERE id = %d", User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);
            return 0;
        }
        return 1;
    }
    return 0;
}

stock CheckPlayerVipOnLogin(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    if(!User[playerid][isLogged]) return 0;

    if(User[playerid][vipLevel] > 0 && User[playerid][vipExpire] > 0 && gettime() >= User[playerid][vipExpire])
    {
        User[playerid][vipLevel] = 0;
        User[playerid][vipExpire] = 0;
        User[playerid][vipHeadTag] = 0;

        if(VipLabel[playerid] != Text3D:INVALID_3DTEXT_ID)
        {
            DestroyDynamic3DTextLabel(VipLabel[playerid]);
            VipLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
        }

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_level = 0, vip_expire = 0, vip_head_tag = 0 WHERE id = %d", User[playerid][user_sqlid]);
        mysql_tquery(dbHandle, query);

        va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP subscription has expired. To activate another package use {MAIN}'/buyvip'{FFFFFF}.");
        return 0;
    }

    if(IsPlayerVip(playerid))
    {
        if(User[playerid][hideWelcomeMsg]) return 1;

        if(User[playerid][vipExpire] == 0)
        {
            va_SendClientMessage(playerid, -1, PLAYER_WELCOME_MSG_VIP_PERM);
        }
        else
        {
            new remaining = User[playerid][vipExpire] - gettime();
            new days = remaining / 86400;
            new hours = (remaining % 86400) / 3600;
            va_SendClientMessage(playerid, -1, PLAYER_WELCOME_MSG5, days, hours);
        }
        return 1;
    }

    return 0;
}

stock GetVipRankName(vip_level)
{
    new rank_name[16];
    if(vip_level > 0) format(rank_name, sizeof(rank_name), "VIP");
    else format(rank_name, sizeof(rank_name), "None");
    return rank_name;
}



stock GetVipTagColorHex(playerid, dest[], len = sizeof(dest))
{
    if(playerid < 0 || playerid >= MAX_PLAYERS)
    {
        format(dest, len, "FFD700");
        return 1;
    }
    new tagIdx = User[playerid][vipHeadTag];
    if(0 <= tagIdx < 10)
    {
        format(dest, len, "%s", vipColorListStr[tagIdx]);
    }
    else if(tagIdx >= 0x10000000)
    {
        format(dest, len, "%06x", tagIdx & 0xFFFFFF);
    }
    else
    {
        format(dest, len, "FFD700");
    }
    return 1;
}

stock GetVipTagStringEx(playerid, dest[], size = sizeof(dest), bool:ignoreHidden = false)
{
    dest[0] = '\0';
    if(IsPlayerVip(playerid))
    {
        new tagIdx = User[playerid][vipHeadTag];
        if(!ignoreHidden && (tagIdx == 10 || User[playerid][hideVipTag])) return 1;

        if(0 <= tagIdx < 10)
        {
            format(dest, size, "{%s}[VIP] ", vipColorListStr[tagIdx]);
        }
        else if(tagIdx >= 0x10000000)
        {
            format(dest, size, "{%06x}[VIP] ", tagIdx & 0xFFFFFF);
        }
        else
        {
            format(dest, size, "{FFD700}[VIP] ");
        }
    }
    return 1;
}

stock GetVipTagString(playerid, dest[], size = sizeof(dest))
{
    return GetVipTagStringEx(playerid, dest, size, false);
}

stock UpdateVipHeadTag(playerid)
{
    if(playerid < 0 || playerid >= MAX_PLAYERS) return 0;
    SetClanColor(playerid);
    return 1;
}

ShowVipMenu(playerid)
{
    if(!User[playerid][isLogged]) return 0;

    new strVip[2048];
    format(strVip, sizeof(strVip), "{MAIN}Perk / Feature\t{MAIN}Benefit Details\n");

    strcat(strVip, "{MAIN}> VIP Subscription Status\t \n");
    if(IsPlayerVip(playerid))
    {
        new statusStr[128];
        if(User[playerid][vipExpire] == 0)
        {
            format(statusStr, sizeof(statusStr), "{4d9253}Active VIP {B4B5B7}| {4d9253}Permanent Account");
        }
        else
        {
            new remaining = User[playerid][vipExpire] - gettime();
            if(remaining < 0) remaining = 0;

            format(statusStr, sizeof(statusStr), "{4d9253}Active VIP {B4B5B7}| {FFFFFF}%d days, %d hours left", remaining / 86400, (remaining % 86400) / 3600);
        }
        format(strVip, sizeof(strVip), "%s{BEBEBE}Current Status\t%s\n", strVip, statusStr);
    }
    else
    {
        strcat(strVip, "{BEBEBE}Current Status\t{FF0000}Not Active (Free Account)\n");
    }

    // Section 2: Perks
    strcat(strVip, "{MAIN}> VIP Perks & Benefits\t \n");
    strcat(strVip, "{BEBEBE}2x Money Multiplier\t{FFFFFF}Earn 2x Double Money ($) on all kills & outpost captures!\n");
    strcat(strVip, "{BEBEBE}Head Tag Customization\t{FFFFFF}3D {FFD700}[VIP]{FFFFFF} tag above character (Toggle with /vtag)\n");
    strcat(strVip, "{BEBEBE}Discord Role Sync\t{FFFFFF}Automatic VIP role & channels in official Discord server\n");
    strcat(strVip, "{BEBEBE}Private VIP Chat\t{FFFFFF}Exclusive chat channel with online VIPs & Staff (/v [msg])\n");

    // Section 3: Commands
    strcat(strVip, "{MAIN}> VIP Commands\t \n");
    strcat(strVip, "{BEBEBE}/vipcmds\t{FFFFFF}View full list of VIP commands\n");
    strcat(strVip, "{BEBEBE}/buyvip\t{FFFFFF}Activate or renew your VIP package\n");
    strcat(strVip, "{BEBEBE}/v [message]\t{FFFFFF}Send a message in the private VIP Chat channel\n");
    strcat(strVip, "{BEBEBE}/vips\t{FFFFFF}View the list of all online VIP members\n");
    strcat(strVip, "{BEBEBE}/vtag\t{FFFFFF}Toggle VIP 3D Head Tag (Hidden / Visible)\n");

    new title[64];
    format(title, sizeof(title), "{MAIN}VIP Perks & Advantages: {FFFFFF}%s", User[playerid][userName]);
    format_MainColor(playerid, title);
    format_MainColor(playerid, strVip);

    Dialog_ShowCallback(playerid, using public DIALOG_VIP_MENU<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, strVip, "Close", "");
    return 1;
}

forward DIALOG_VIP_MENU(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_VIP_MENU(playerid, dialogid, response, listitem, string:inputtext[])
{
    return 1;
}

forward DIALOG_BUY_VIP(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_BUY_VIP(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;
    if(!response) return 1;
    if(listitem < 0 || listitem >= sizeof(VipPackages)) return 1;

    SetPVarInt(playerid, "vip_package", listitem);

    new days  = VipPackages[listitem][vip_days];
    new money = VipPackages[listitem][vip_price_money];
    new coins = VipPackages[listitem][vip_price_coins];

    new body[512], title[64];
    format(body, sizeof(body), "{MAIN}Payment\t{MAIN}Cost\t{MAIN}Your balance\n");

    format(title, sizeof(title), "{FFFFFF}In-game money\t{00FF00}%s\t%s%s\n",
        FormatMoney(money), (User[playerid][playerMoney] >= money) ? ("{23C552}") : ("{F84F31}"), FormatMoney(User[playerid][playerMoney]));
    strcat(body, title);

    format(title, sizeof(title), "{FFFFFF}Coins\t{FFFF00}%s\t%s%s",
        FormatCoins(coins), (User[playerid][playerCoins] >= coins) ? ("{23C552}") : ("{F84F31}"), FormatCoins(User[playerid][playerCoins]));
    strcat(body, title);

    format(title, sizeof(title), "{MAIN}VIP %d Days - Payment", days);
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    Dialog_ShowCallback(playerid, using public DIALOG_VIP_PAYMENT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Pay", "Back");
    return 1;
}

public DIALOG_VIP_PAYMENT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;

    if(!response)
    {
        new cmd[16];
        strcopy(cmd, "/buyvip");
        Command_ReProcess(playerid, cmd, 0);
        return 1;
    }

    new idx = GetPVarInt(playerid, "vip_package");
    if(idx < 0 || idx >= sizeof(VipPackages)) return 1;

    new days  = VipPackages[idx][vip_days];
    new money = VipPackages[idx][vip_price_money];
    new coins = VipPackages[idx][vip_price_coins];

    new paidWith[32], balanceStr[64];

    if(listitem == VIP_PAY_COINS)
    {
        if(User[playerid][playerCoins] < coins)
            return va_SendClientMessage(playerid, -1, VIP_NO_COINS, FormatCoins(coins - User[playerid][playerCoins]));

        User[playerid][playerCoins] -= coins;
        format(paidWith, sizeof(paidWith), "{FFFF00}%s", FormatCoins(coins));
        format(balanceStr, sizeof(balanceStr), "{FFFF00}%s", FormatCoins(User[playerid][playerCoins]));
    }
    else
    {
        if(User[playerid][playerMoney] < money)
            return va_SendClientMessage(playerid, -1, VIP_NO_MONEY, FormatMoney(money - User[playerid][playerMoney]));

        User[playerid][playerMoney] -= money;
        GivePlayerMoney(playerid, -money);
        format(paidWith, sizeof(paidWith), "{23C552}%s", FormatMoney(money));
        format(balanceStr, sizeof(balanceStr), "{23C552}%s", FormatMoney(User[playerid][playerMoney]));
    }

    if(IsPlayerVip(playerid) && User[playerid][vipExpire] > 0)
    {
        User[playerid][vipExpire] += (days * 86400);
    }
    else
    {
        User[playerid][vipLevel] = 1;
        User[playerid][vipExpire] = gettime() + (days * 86400);
    }

    User[playerid][vipHeadTag] = 1;
    UpdateVipHeadTag(playerid);
    SyncPlayerDiscordRoles(playerid);

    new query[192];
    mysql_format(dbHandle, query, sizeof(query),
        "UPDATE accounts SET vip_level = 1, vip_expire = %d, vip_head_tag = 1, player_money = %d, coins = %d WHERE id = %d",
        User[playerid][vipExpire], User[playerid][playerMoney], User[playerid][playerCoins], User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, query);

    GameTextForPlayer(playerid, "~g~~h~VIP ACTIVATED", 2500, 1);
    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);

    va_SendClientMessage(playerid, -1, "{MAIN}~~~~~~~~~~~~~~~~~~~~~~~~[ {FFFFFF}VIP ACTIVATION {MAIN}]~~~~~~~~~~~~~~~~~~~~~~~~");
    va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Congratulations {MAIN}%s{FFFFFF}, your {MAIN}VIP %d Days{FFFFFF} package has been activated!", User[playerid][userName], days);
    va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Paid with: %s {FFFFFF}| Remaining: %s", paidWith, balanceStr);
    va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Use {MAIN}'/vipcmds'{FFFFFF} to check your perks and {MAIN}'/vtag'{FFFFFF} to customize your tag.");
    va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Discord Sync: Link your account to receive your VIP role on Discord!");
    va_SendClientMessage(playerid, -1, "{MAIN}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

    va_SendClientMessageToAll(-1, "{MAIN}[VIP] {FFFFFF}Player {MAIN}%s{FFFFFF} has just activated a {MAIN}VIP Package %d Days{FFFFFF}! Enjoy the perks!", User[playerid][userName], days);
    return 1;
}

@cmd(.alts = {"activatevip"}) buyvip(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    new string2[768], line[128];
    format(string2, sizeof(string2), "{MAIN}Package / Duration\t{MAIN}Price ($)\t{MAIN}Price (Coins)\n");

    for(new i = 0; i < sizeof(VipPackages); i++)
    {
        format(line, sizeof(line), "{FFFFFF}VIP %d Days\t{00FF00}%s\t{FFFF00}%s\n",
            VipPackages[i][vip_days], FormatMoney(VipPackages[i][vip_price_money]), FormatCoins(VipPackages[i][vip_price_coins]));
        strcat(string2, line);
    }

    new title[64];
    format(title, sizeof(title), "{MAIN}VIP Package Activation");
    format_MainColor(playerid, title);
    format_MainColor(playerid, string2);

    Dialog_ShowCallback(playerid, using public DIALOG_BUY_VIP<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, string2, "Activate", "Cancel");
    return true;
}

forward DIALOG_VIP_TAG(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_VIP_TAG(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;

    if(response)
    {
        if(!IsPlayerVip(playerid) && User[playerid][adminLevel] < 1)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Only VIP or Staff players have permission to change the VIP tag.");
            return 1;
        }

        if(listitem >= 0 && listitem < 10)
        {
            User[playerid][vipHeadTag] = listitem;
            User[playerid][hideVipTag] = false;
            UpdateVipHeadTag(playerid);
            
            new query[144];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_head_tag = %d, hide_vip_tag = 0 WHERE id = %d", User[playerid][vipHeadTag], User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, query);

            va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP 3D head tag color has been updated to {%s}[VIP]{FFFFFF}!", vipColorListStr[listitem]);
        }
        else if(listitem == 10)
        {
            ShowVipCustomHexDialog(playerid);
        }
        else if(listitem == 11)
        {
            if(User[playerid][vipHeadTag] == 10)
            {
                new lastTag = GetPVarInt(playerid, "LastVipHeadTag");
                if(lastTag == 10) lastTag = 0;

                User[playerid][vipHeadTag] = lastTag;
                User[playerid][hideVipTag] = false;
                UpdateVipHeadTag(playerid);

                new query[144];
                mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_head_tag = %d, hide_vip_tag = 0 WHERE id = %d", User[playerid][vipHeadTag], User[playerid][user_sqlid]);
                mysql_tquery(dbHandle, query);

                va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP 3D head tag is now {23C552}Visible{FFFFFF}!");
            }
            else
            {
                SetPVarInt(playerid, "LastVipHeadTag", User[playerid][vipHeadTag]);
                User[playerid][vipHeadTag] = 10;
                User[playerid][hideVipTag] = true;
                UpdateVipHeadTag(playerid);

                new query[144];
                mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_head_tag = 10, hide_vip_tag = 1 WHERE id = %d", User[playerid][user_sqlid]);
                mysql_tquery(dbHandle, query);

                va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP 3D head tag has been {FF0000}Hidden{FFFFFF}.");
            }
        }
    }
    return 1;
}

ShowVipCustomHexDialog(playerid)
{
    new curHex[7];
    GetVipTagColorHex(playerid, curHex);

    new body[512];
    format(body, sizeof(body),
"{FFFFFF}Current VIP Tag Color: {%s}#%s{FFFFFF}\n\n\
{BEBEBE}Enter your new 6-digit HEX color code for your 3D VIP Tag below:\n\
{BEBEBE}Examples: {MAIN}FF0000{BEBEBE} (Red), {MAIN}00FF88{BEBEBE} (Mint Green), {MAIN}FF00FF{BEBEBE} (Magenta).\n\
{BEBEBE}Valid characters: 0-9, A-F.",
        curHex, curHex);

    new title[64] = "{MAIN}Custom VIP Tag Color (HEX)";
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public DIALOG_VIP_CUSTOM_HEX<iiiis>, DIALOG_STYLE_INPUT, title, body, "Confirm", "Back");
}

forward DIALOG_VIP_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_VIP_CUSTOM_HEX(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!User[playerid][isLogged]) return 1;

    if(!response)
    {
        return ShowVipTagDialog(playerid);
    }

    if(!IsPlayerVip(playerid) && User[playerid][adminLevel] < 1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Only VIP or Staff players have permission to change the VIP tag.");
        return 1;
    }

    new hexInput[16];
    format(hexInput, sizeof(hexInput), "%s", inputtext);

    if(hexInput[0] == '#')
    {
        strdel(hexInput, 0, 1);
    }

    new len = strlen(hexInput);
    if(len != 6)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX format! Please enter 6 characters (e.g. FF0000).");
        return ShowVipCustomHexDialog(playerid);
    }

    new colorVal;
    if(sscanf(hexInput, "x", colorVal))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Invalid HEX color code.");
        return ShowVipCustomHexDialog(playerid);
    }

    new storedVal = 0x10000000 | (colorVal & 0xFFFFFF);
    User[playerid][vipHeadTag] = storedVal;
    UpdateVipHeadTag(playerid);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_head_tag = %d WHERE id = %d", User[playerid][vipHeadTag], User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, query);

    va_SendClientMessage(playerid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP 3D head tag color has been updated to {%06x}#%06x{FFFFFF}!", colorVal & 0xFFFFFF, colorVal & 0xFFFFFF);
    return 1;
}

ShowVipCommandsMenu(playerid)
{
    if(!User[playerid][isLogged]) return 0;

    new strCmds[1024];
    format(strCmds, sizeof(strCmds), "{MAIN}Command\t{MAIN}Description\n");
    strcat(strCmds, "{BEBEBE}/v [message]\t{FFFFFF}Send a message in the private VIP Chat channel\n");
    strcat(strCmds, "{BEBEBE}/von\t{FFFFFF}Enable receiving messages from VIP Chat\n");
    strcat(strCmds, "{BEBEBE}/voff\t{FFFFFF}Disable receiving messages from VIP Chat\n");
    strcat(strCmds, "{BEBEBE}/vips\t{FFFFFF}View the list of all online VIP members\n");
    strcat(strCmds, "{BEBEBE}/vtag\t{FFFFFF}Customize or toggle your 3D VIP Head Tag color\n");
    strcat(strCmds, "{BEBEBE}/buyvip\t{FFFFFF}Activate or renew your VIP package\n");
    strcat(strCmds, "{BEBEBE}/vip\t{FFFFFF}View all VIP Perks, status & subscription details\n");

    new title[64];
    format(title, sizeof(title), "{MAIN}VIP Commands");
    format_MainColor(playerid, title);
    format_MainColor(playerid, strCmds);

    Dialog_ShowCallback(playerid, using public DIALOG_VIP_CMDS<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, strCmds, "Close", "");
    return 1;
}

forward DIALOG_VIP_CMDS(playerid, dialogid, response, listitem, string:inputtext[]);
public DIALOG_VIP_CMDS(playerid, dialogid, response, listitem, string:inputtext[])
{
    return 1;
}

@cmd() vip(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;
    ShowVipMenu(playerid);
    return true;
}

@cmd(.alts = {"vipcommands"}) vipcmds(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;
    ShowVipCommandsMenu(playerid);
    return true;
}

@cmd() vips(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    new string[128], vipsonline = 0;
    new gs_Buffer[4096] = EOS;
    format(gs_Buffer, sizeof(gs_Buffer), "{MAIN}Player\t{MAIN}Online Time\t{MAIN}VIP Time\n");

    foreach(new i : Player)
    {
        if(User[i][isLogged] && IsPlayerVip(i))
        {
            new ontime = (User[i][totalOnTime] + (gettime() - User[i][loginTime]));
            if(ontime < 0) ontime = 0;

            new vipTimeStr[32];
            if(User[i][vipExpire] == 0)
            {
                format(vipTimeStr, sizeof(vipTimeStr), "Permanent");
            }
            else
            {
                new remaining = User[i][vipExpire] - gettime();
                if(remaining < 0) remaining = 0;

                format(vipTimeStr, sizeof(vipTimeStr), "%dd %dh", remaining / 86400, (remaining % 86400) / 3600);
            }

            format(string, sizeof(string), "{FFFFFF}%s[%d]\t{FFFFFF}%02d:%02d:%02d\t{00FF00}%s\n",
                User[i][userName], i, ontime / 3600, (ontime % 3600) / 60, ontime % 60, vipTimeStr);
            strcat(gs_Buffer, string);
            vipsonline++;
        }
    }

    if(vipsonline == 0)
    {
        new noVipTitle[64], noVipMsg[128];
        format(noVipTitle, sizeof(noVipTitle), "{MAIN}VIP Members");
        format(noVipMsg, sizeof(noVipMsg), "\n{MAIN}No VIP members currently online.");
        format_MainColor(playerid, noVipTitle);
        format_MainColor(playerid, noVipMsg);
        Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, noVipTitle, noVipMsg, "Close", "");
        return true;
    }

    new title[64];
    format(title, sizeof(title), "{MAIN}VIP Members [ {FFFFFF}%d {MAIN}]", vipsonline);
    format_MainColor(playerid, title);
    format_MainColor(playerid, gs_Buffer);

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, title, gs_Buffer, "Close", "");
    return true;
}

@cmd() v(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    if(!IsPlayerVip(playerid) && User[playerid][adminLevel] < 1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must be a VIP or Staff member to use VIP Chat.");
        return true;
    }

    if(User[playerid][hideVipChat])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Your VIP Chat is currently disabled! Type {MAIN}'/von'{FFFFFF} to enable it before sending messages.");
        return true;
    }

    if(User[playerid][muteTimestamp] > gettime())
    {
        va_SendClientMessage(playerid, -1, IS_MUTED);
        return true;
    }

    if(!AntiSpam(playerid, "flood_chat", "flood_chat_count"))
    {
        return true;
    }

    if(isnull(params))
    {
        SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/v [message]");
        return true;
    }

    new vipStr[256];
    format(vipStr, sizeof(vipStr), "[Chat-Vip] %s[%d] say: %s", 
        User[playerid][userName], playerid, params);

    foreach(new i : Player)
    {
        if(User[i][isLogged] && (IsPlayerVip(i) || User[i][adminLevel] > 0) && !User[i][hideVipChat])
        {
            SendClientMessage(i, 0x00B6F6AA, "%s", vipStr);
        }
    }

    LogChatMessage(User[playerid][user_sqlid], User[playerid][userName], "vip", params);
    return true;
}

@cmd() von(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    if(!IsPlayerVip(playerid) && User[playerid][adminLevel] < 1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must be a VIP or Staff member to use VIP Chat.");
        return true;
    }

    if(!User[playerid][hideVipChat])
    {
        SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Your VIP Chat is already enabled!");
        return true;
    }

    User[playerid][hideVipChat] = false;
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}VIP Chat enabled! Use {MAIN}'/voff'{FFFFFF} to disable receiving messages.");
    return true;
}

@cmd() voff(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    if(!IsPlayerVip(playerid) && User[playerid][adminLevel] < 1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You must be a VIP or Staff member to use VIP Chat.");
        return true;
    }

    if(User[playerid][hideVipChat])
    {
        SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}Your VIP Chat is already disabled!");
        return true;
    }

    User[playerid][hideVipChat] = true;
    SendClientMessage(playerid, -1, "{MAIN}[INFO] {FFFFFF}VIP Chat disabled! Use {MAIN}'/von'{FFFFFF} to enable receiving messages.");
    return true;
}

ShowVipTagDialog(playerid)
{
    if(!User[playerid][isLogged]) return 0;

    if(!IsPlayerVip(playerid) && User[playerid][adminLevel] < 1)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Only VIP or Staff players have permission to change the VIP tag.");
        return 0;
    }

    new tagList[1024];
    new curColorHex[7];
    GetVipTagColorHex(playerid, curColorHex);

    format(tagList, sizeof(tagList), "{MAIN}Color Option\t{MAIN}Preview\n");
    strcat(tagList, "{0022FF}Dark Blue\t{0022FF}[VIP]\n");
    strcat(tagList, "{008CFF}Light Blue\t{008CFF}[VIP]\n");
    strcat(tagList, "{09FF00}Green\t{09FF00}[VIP]\n");
    strcat(tagList, "{FF7700}Orange\t{FF7700}[VIP]\n");
    strcat(tagList, "{FF0000}Red\t{FF0000}[VIP]\n");
    strcat(tagList, "{FFFB00}Yellow\t{FFFB00}[VIP]\n");
    strcat(tagList, "{FF00FB}Pink / Purple\t{FF00FB}[VIP]\n");
    strcat(tagList, "{4D3F4C}Dark Purple\t{4D3F4C}[VIP]\n");
    strcat(tagList, "{00FFA2}Cyan / Aqua\t{00FFA2}[VIP]\n");
    strcat(tagList, "{7300FF}Violet\t{7300FF}[VIP]\n");
    strcat(tagList, fmat_return("{FFFFFF}Custom HEX Color\t{%s}#%s{FFFFFF}\n", curColorHex, curColorHex));
    if(User[playerid][vipHeadTag] == 10)
    {
        strcat(tagList, "{FFFFFF}Toggle Tag Visibility\t{FF0000}[Hidden]");
    }
    else
    {
        strcat(tagList, "{FFFFFF}Toggle Tag Visibility\t{23C552}[Active]");
    }

    new title[64] = "{MAIN}VIP Tag Color";
    format_MainColor(playerid, title);
    format_MainColor(playerid, tagList);

    return Dialog_ShowCallback(playerid, using public DIALOG_VIP_TAG<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, tagList, "Select", "Close");
}

@cmd() vtag(playerid, params[], help)
{
    ShowVipTagDialog(playerid);
    return true;
}

@cmd() givevip(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    if(User[playerid][adminLevel] < 4)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You do not have permission to use this command (Admin Level 4 Required).");
        return true;
    }

    new targetid, days;
    if(sscanf(params, "rd", targetid, days))
    {
        SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/givevip [playerid/name] [days 0=permanent]");
        return true;
    }

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Target player is not connected or logged in.");
        return true;
    }

    if(days < 0 || days > 3650)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}VIP duration must be between 0 permanent and 3650 days.");
        return true;
    }

    User[targetid][vipLevel] = 1;
    if(days > 0)
    {
        User[targetid][vipExpire] = gettime() + (days * 86400);
    }
    else
    {
        User[targetid][vipExpire] = 0;
    }

    User[targetid][vipHeadTag] = 1;
    UpdateVipHeadTag(targetid);
    SyncPlayerDiscordRoles(targetid);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_level = 1, vip_expire = %d, vip_head_tag = 1 WHERE id = %d", 
        User[targetid][vipExpire], User[targetid][user_sqlid]);
    mysql_tquery(dbHandle, query);

    va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}You granted VIP status {23C552}%s{FFFFFF} for %d days to %s[%d].", 
        (days > 0) ? ("Temporary") : ("Permanent"), days, User[targetid][userName], targetid);

    va_SendClientMessage(targetid, -1, "{MAIN}[VIP] {FFFFFF}Congratulations! Admin %s granted you {23C552}VIP{FFFFFF} status!", 
        User[playerid][userName]);

    return true;
}

@cmd() removevip(playerid, params[], help)
{
    if(!User[playerid][isLogged]) return true;

    if(User[playerid][adminLevel] < 4)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You do not have permission to use this command (Admin Level 4 Required).");
        return true;
    }

    new targetid;
    if(sscanf(params, "r", targetid))
    {
        SendClientMessage(playerid, -1, "{MAIN}[USAGE] {FFFFFF}/removevip [playerid/name]");
        return true;
    }

    if(!IsPlayerConnected(targetid) || !User[targetid][isLogged])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Target player is not connected or logged in.");
        return true;
    }

    User[targetid][vipLevel] = 0;
    User[targetid][vipExpire] = 0;
    User[targetid][vipHeadTag] = 0;

    if(VipLabel[targetid] != Text3D:INVALID_3DTEXT_ID)
    {
        DestroyDynamic3DTextLabel(VipLabel[targetid]);
        VipLabel[targetid] = Text3D:INVALID_3DTEXT_ID;
    }

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE accounts SET vip_level = 0, vip_expire = 0, vip_head_tag = 0 WHERE id = %d", User[targetid][user_sqlid]);
    mysql_tquery(dbHandle, query);

    va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Removed VIP status from %s[%d].", User[targetid][userName], targetid);
    va_SendClientMessage(targetid, -1, "{MAIN}[VIP] {FFFFFF}Your VIP status has been removed by Admin %s.", User[playerid][userName]);

    return true;
}

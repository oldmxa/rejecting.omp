#if defined _safehouse_dialogs_included
    #endinput
#endif
#define _safehouse_dialogs_included

forward DIALOG_SAFE_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_NAME(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_PASS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_VAULT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_DEPOSIT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_WITHDRAW(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_UPGRADE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_MEMBERS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_EXPIRY(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_RENEW(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_SELL(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_BUY(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_ENTERPASS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_SAFE_HISTORY(playerid, dialogid, response, listitem, string:inputtext[]);

enum
{
    SAFE_MENU_CLAN,
    SAFE_MENU_NAME,
    SAFE_MENU_PASS,
    SAFE_MENU_VAULT,
    SAFE_MENU_UPGRADE,
    SAFE_MENU_MEMBERS,
    SAFE_MENU_EXPIRY,
    SAFE_MENU_SELL
}

enum
{
    SAFE_VAULT_BALANCE,
    SAFE_VAULT_DEPOSIT,
    SAFE_VAULT_WITHDRAW,
    SAFE_VAULT_HISTORY
}

enum
{
    SAFE_EXPIRY_LEFT,
    SAFE_EXPIRY_NEXT,
    SAFE_EXPIRY_CHARGE,
    SAFE_EXPIRY_VAULT,
    SAFE_EXPIRY_RENEW,
    SAFE_EXPIRY_HISTORY
}

stock ShowSafehouseMenu(playerid)
{
    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index))
        return SendClientMessage(playerid, -1, "%s", SH_NO_SAFEHOUSE);

    new clanid = SafehouseData[index][sh_ClanID];

    new title[128];
    format(title, sizeof(title), "{MAIN}Safehouse: {FFFFFF}%s", SafehouseData[index][sh_Name]);

    format_MainColor(playerid, title);
    new buf[1024];
    format(buf, sizeof(buf), "{BEBEBE}Option\t{BEBEBE}Details\t{BEBEBE}Action\n");

    if (IsValidClanID(clanid))
        strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Clan Info\t{%06x}%s\t{BEBEBE}[View]\n", ClanInfo[clanid][clanColor] >>> 8, ClanInfo[clanid][clanName]));
    else
        strcat(buf, "{B4B5B7}\x95 {FFFFFF}Clan Info\t{BEBEBE}None\t{BEBEBE}[None]\n");

    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Rename Base\t{FFFFFF}%s\t{3CCFFA}[Edit]\n", SafehouseData[index][sh_Name]));

    if (SafehouseData[index][sh_Password][0] != '\0')
        strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Password\t{23C552}%s\t{3CCFFA}[Edit]\n", SafehouseData[index][sh_Password]));
    else
        strcat(buf, "{B4B5B7}\x95 {FFFFFF}Password\t{BEBEBE}None\t{3CCFFA}[Set]\n");

    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Clan Vault\t{23C552}%s\t{3CCFFA}[Manage]\n", FormatMoney(SafehouseData[index][sh_Vault])));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Upgrade Base\t{FFFFFF}Level %d\t{3CCFFA}[Upgrade]\n", SafehouseData[index][sh_Level]));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Clan Members\t{FFFFFF}%d/%d\t{3CCFFA}[View]\n", ClanMembersCount[clanid], GetClanMemberSlots(clanid)));

    new leftStr[24];
    FormatSafehouseTime(GetSafehouseTimeLeft(index), leftStr);
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Contract Expiry\t{FFFFFF}%s left\t{3CCFFA}[Extend]\n", leftStr));

    new refund = (SH_BUY_PRICE / 100) * SH_REFUND_PCT + ((SH_BUY_PRICE % 100) * SH_REFUND_PCT) / 100;
    strcat(buf, fmat_return("{F84F31}\x95 Sell Safehouse\t{23C552}%s\t{F84F31}[Sell]\n", FormatMoney(refund)));

    format_MainColor(playerid, buf);
    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_MAIN<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Select", "Close");
}

public DIALOG_SAFE_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    new title[64];

    switch (listitem)
    {
        case SAFE_MENU_CLAN: return ShowSafehouseMenu(playerid);
        case SAFE_MENU_NAME:
        {
            if (!IsSafehouseLeader(playerid, index))
                return ShowSafehouseDenied(playerid, "Only the clan leader can change the name.");

            title = "{MAIN}Safehouse - Rename Base";

            format_MainColor(playerid, title);
            new body[384];
            format(body, sizeof(body),
                "{FFFFFF}Enter a new name for your {MAIN}Safehouse{FFFFFF} below:\n\n"\
                "{MAIN}\x95 {B4B5B7}Length: {FFFFFF}3 to 31 characters\n"\
                "{MAIN}\x95 {B4B5B7}Rules: {FFFFFF}Letters, numbers and basic symbols only\n\n"\
                "{BEBEBE}The new name will be displayed at the entrance pickup and on menus.");


            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_NAME<iiiis>, DIALOG_STYLE_INPUT,
                title, body, "Save", "Back");
        }
        case SAFE_MENU_PASS:
        {
            if (!IsSafehouseLeader(playerid, index))
                return ShowSafehouseDenied(playerid, "Only the clan leader can change the password.");

            title = "{MAIN}Safehouse - Password";

            new body[384];
            format(body, sizeof(body),
                "{FFFFFF}Enter a new password for your {MAIN}Safehouse{FFFFFF} below:\n\n"\
                "{MAIN}\x95 {B4B5B7}Length: {FFFFFF}%d to %d characters\n"\
                "{MAIN}\x95 {B4B5B7}Access: {FFFFFF}Guests must enter this password to enter\n\n"\
                "{BEBEBE}Leave this field empty to make the safehouse public.",
                SAFEHOUSE_PASS_MIN, SAFEHOUSE_PASS_MAX);


            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_PASS<iiiis>, DIALOG_STYLE_INPUT,
                title, body, "Save", "Back");
        }
        case SAFE_MENU_VAULT:  return ShowSafehouseVault(playerid);
        case SAFE_MENU_UPGRADE: return ShowSafehouseUpgrade(playerid);
        case SAFE_MENU_MEMBERS: return ShowSafehouseMembers(playerid);
        case SAFE_MENU_EXPIRY:  return ShowSafehouseExpiry(playerid);
        case SAFE_MENU_SELL:
        {
            if (!IsSafehouseLeader(playerid, index))
                return ShowSafehouseDenied(playerid, "Only the clan leader can sell the safehouse.");

            new refund = (SH_BUY_PRICE / 100) * SH_REFUND_PCT + ((SH_BUY_PRICE % 100) * SH_REFUND_PCT) / 100;
            new refundStr[36], vaultStr[36], totalStr[36];
            format(refundStr, sizeof(refundStr), "%s", FormatMoney(refund));
            format(vaultStr, sizeof(vaultStr), "%s", FormatMoney(SafehouseData[index][sh_Vault]));
            format(totalStr, sizeof(totalStr), "%s", FormatMoney(refund + SafehouseData[index][sh_Vault]));

            new body[512];
            format(body, sizeof(body),
                "{FFFFFF}Sell the clan safehouse at {MAIN}%s{FFFFFF}?\n\n"\
                "{MAIN}\x95 {B4B5B7}Base Refund ({FFFFFF}%d%%{B4B5B7}): {23C552}%s\n"\
                "{MAIN}\x95 {B4B5B7}Vault Balance: {23C552}%s\n"\
                "{MAIN}\x95 {B4B5B7}Total Payout: {23C552}%s\n\n"\
                "{F3230E}WARNING: All upgrades, interiors and custom settings will be reset.",
                SafehouseData[index][sh_Name],
                SH_REFUND_PCT, refundStr,
                vaultStr,
                totalStr);

            title = "{MAIN}Sell Safehouse - Confirmation";

            format_MainColor(playerid, title);
            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_SELL<iiiis>,
                DIALOG_STYLE_MSGBOX, title, body, "Sell", "Back");
        }
    }
    return 1;
}

stock ShowSafehouseDenied(playerid, const reason[])
{
    va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"%s", reason);
    return ShowSafehouseMenu(playerid);
}

public DIALOG_SAFE_NAME(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseMenu(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsSafehouseLeader(playerid, index)) return ShowSafehouseMenu(playerid);

    new clean[SAFEHOUSE_NAME_LEN];
    if (!SanitizeSafehouseText(inputtext, clean, sizeof(clean), 3))
    {
        SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Invalid name. Use 3 to 31 normal characters.");
        return ShowSafehouseMenu(playerid);
    }

    format(SafehouseData[index][sh_Name], SAFEHOUSE_NAME_LEN, "%s", clean);
    SaveSafehouse(index);
    RefreshSafehousePickup(index);
    LogSafehouse(index, playerid, "rename", clean);

    SendSafehouseClanMessage(index, SAFEHOUSE_PREFIX"%s renamed the safehouse to {MAIN}%s{FFFFFF}.",
        User[playerid][userName], clean);
    return ShowSafehouseMenu(playerid);
}

public DIALOG_SAFE_PASS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseMenu(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsSafehouseLeader(playerid, index)) return ShowSafehouseMenu(playerid);

    if (isnull(inputtext))
    {
        SafehouseData[index][sh_Password][0] = '\0';
        SaveSafehouse(index);
        LogSafehouse(index, playerid, "password", "removed");
        SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Password removed.");
        return ShowSafehouseMenu(playerid);
    }

    new clean[SAFEHOUSE_PASS_MAX + 1];
    if (!SanitizeSafehouseText(inputtext, clean, sizeof(clean), SAFEHOUSE_PASS_MIN))
    {
        va_SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Invalid password. Use %d to %d normal characters.",
            SAFEHOUSE_PASS_MIN, SAFEHOUSE_PASS_MAX);
        return ShowSafehouseMenu(playerid);
    }

    format(SafehouseData[index][sh_Password], SAFEHOUSE_PASS_LEN, "%s", clean);
    SaveSafehouse(index);
    LogSafehouse(index, playerid, "password", "changed");

    SendClientMessage(playerid, -1, SAFEHOUSE_PREFIX"Password updated.");
    return ShowSafehouseMenu(playerid);
}

stock ShowSafehouseVault(playerid)
{
    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    new title[128];
    format(title, sizeof(title), "{MAIN}Clan Vault: {FFFFFF}%s", SafehouseData[index][sh_Name]);

    format_MainColor(playerid, title);
    new buf[512];
    format(buf, sizeof(buf), "{BEBEBE}Option\t{BEBEBE}Description\t{BEBEBE}Balance\n");
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Balance\t{BEBEBE}Total funds stored in vault\t{23C552}%s\n", FormatMoney(SafehouseData[index][sh_Vault])));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Deposit\t{BEBEBE}Add cash into vault\t{23C552}%s in hand\n", FormatMoney(User[playerid][playerMoney])));
    strcat(buf, "{B4B5B7}\x95 {FFFFFF}Withdraw\t{BEBEBE}Take cash from the vault\t{3CCFFA}[Withdraw]\n");
    strcat(buf, "{B4B5B7}\x95 {FFFFFF}History\t{BEBEBE}View last vault transactions\t{3CCFFA}[View]\n");


    format_MainColor(playerid, buf);
    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_VAULT<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Select", "Back");
}

public DIALOG_SAFE_VAULT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseMenu(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    if (listitem != SAFE_VAULT_BALANCE && !CanUseSafehouseVault(playerid, index))
        return ShowSafehouseDenied(playerid, "Your clan rank cannot move vault money.");

    new title[64];

    switch (listitem)
    {
        case SAFE_VAULT_DEPOSIT:
        {
            title = "{MAIN}Vault - Deposit Money";

            new body[384];
            format(body, sizeof(body),
                "{FFFFFF}Enter the amount you wish to {23C552}deposit{FFFFFF} into the Clan Vault:\n\n"\
                "{MAIN}\x95 {B4B5B7}Your Cash: {23C552}%s\n"\
                "{MAIN}\x95 {B4B5B7}Vault Balance: {23C552}%s",
                FormatMoney(User[playerid][playerMoney]),
                FormatMoney(SafehouseData[index][sh_Vault]));


            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_DEPOSIT<iiiis>, DIALOG_STYLE_INPUT,
                title, body, "Deposit", "Back");
        }
        case SAFE_VAULT_WITHDRAW:
        {
            title = "{MAIN}Vault - Withdraw Money";

            new body[384];
            format(body, sizeof(body),
                "{FFFFFF}Enter the amount you wish to {FF6347}withdraw{FFFFFF} from the Clan Vault:\n\n"\
                "{MAIN}\x95 {B4B5B7}Vault Balance: {23C552}%s\n"\
                "{MAIN}\x95 {B4B5B7}Your Cash: {23C552}%s",
                FormatMoney(SafehouseData[index][sh_Vault]),
                FormatMoney(User[playerid][playerMoney]));


            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_WITHDRAW<iiiis>, DIALOG_STYLE_INPUT,
                title, body, "Withdraw", "Back");
        }
        case SAFE_VAULT_HISTORY: return ShowSafehouseHistory(playerid, index, "vault");
    }
    return ShowSafehouseVault(playerid);
}

public DIALOG_SAFE_DEPOSIT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseVault(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!CanUseSafehouseVault(playerid, index)) return ShowSafehouseMenu(playerid);

    if (!IsNumeric(inputtext))
    {
        SendClientMessage(playerid, -1, "%s", SH_INVALID_AMOUNT);
        return ShowSafehouseVault(playerid);
    }

    SafehouseVaultDeposit(playerid, index, strval(inputtext));
    return ShowSafehouseVault(playerid);
}

public DIALOG_SAFE_WITHDRAW(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseVault(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!CanUseSafehouseVault(playerid, index)) return ShowSafehouseMenu(playerid);

    if (!IsNumeric(inputtext))
    {
        SendClientMessage(playerid, -1, "%s", SH_INVALID_AMOUNT);
        return ShowSafehouseVault(playerid);
    }

    SafehouseVaultWithdraw(playerid, index, strval(inputtext));
    return ShowSafehouseVault(playerid);
}

stock ShowSafehouseUpgrade(playerid)
{
    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    new level = SafehouseData[index][sh_Level];
    new next = level + 1;

    new currentName[24];
    GetSafehouseLevelName(level, currentName);

    new title[128];
    format(title, sizeof(title), "{MAIN}Safehouse Upgrade: {FFFFFF}%s", SafehouseData[index][sh_Name]);

    format_MainColor(playerid, title);
    new body[768];

    if (!IsValidSafehouseLevel(next))
    {
        format(body, sizeof(body),
            "{FFFFFF}Your safehouse tier status:\n\n"\
            "{MAIN}\x95 {B4B5B7}Current Tier: {MAIN}Level %d - %s\n"\
            "{MAIN}\x95 {B4B5B7}Member Slots: {23C552}%d Members\n\n"\
            "{23C552}This safehouse is already at maximum upgrade level.",
            level, currentName, GetClanMemberSlots(SafehouseData[index][sh_ClanID]));

        format_MainColor(playerid, body);
        return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_UPGRADE<iiiis>,
            DIALOG_STYLE_MSGBOX, title, body, "Back", "");
    }

    new nextName[24], price, coins;
    GetSafehouseLevelName(next, nextName);
    GetSafehouseUpgradePrice(next, price, coins);

    format(body, sizeof(body),
        "{FFFFFF}Upgrade your clan safehouse to unlock more capacity and a new interior:\n\n"\
        "{MAIN}\x95 {B4B5B7}Current Tier: {FFFFFF}Level %d - %s\n"\
        "{MAIN}\x95 {B4B5B7}Next Tier: {MAIN}Level %d - %s\n"\
        "{MAIN}\x95 {B4B5B7}Upgrade Cost: {23C552}%s {FFFFFF}+ {FFD700}%s Coins\n"\
        "{MAIN}\x95 {B4B5B7}Member Slots: {FFFFFF}%d {23C552}-> %d Members\n"\
        "{MAIN}\x95 {B4B5B7}New Interior ID: {FFFFFF}%d\n\n"\
        "{BEBEBE}Upgrading updates the safehouse interior immediately.",
        level, currentName, next, nextName,
        FormatMoney(price), FormatCoins(coins),
        CLAN_BASE_SLOTS + GetSafehouseLevelSlots(level),
        CLAN_BASE_SLOTS + GetSafehouseLevelSlots(next),
        g_SafehouseLevels[next - 1][lvl_Interior]);


    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_UPGRADE<iiiis>,
        DIALOG_STYLE_MSGBOX, title, body, "Upgrade", "Back");
}

public DIALOG_SAFE_UPGRADE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseMenu(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    UpgradeSafehouse(playerid, index);
    return ShowSafehouseMenu(playerid);
}

stock ShowSafehouseMembers(playerid)
{
    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    new clanid = SafehouseData[index][sh_ClanID];
    if (!IsValidClanID(clanid)) return 1;

    new title[128];
    format(title, sizeof(title), "{MAIN}Safehouse Members: {FFFFFF}%s", ClanInfo[clanid][clanName]);

    format_MainColor(playerid, title);
    new buf[128 + (128 * MAX_MEMBERS)];
    format(buf, sizeof(buf), "{MAIN}Member\t{MAIN}Role\t{MAIN}Status\n");

    for (new i = 0; i < MAX_MEMBERS; i++)
    {
        if (!ClanMembers[clanid][i][member_slotused]) continue;

        new role = ClanMembers[clanid][i][member_role];
        new line[128];
        format(line, sizeof(line), "{FFFFFF}%s\t{BEBEBE}%s\t%s\n",
            ClanMembers[clanid][i][member_name],
            IsValidClanRole(role) ? (ClanRoles[clanid][role]) : ("-"),
            IsAccountOnline(ClanMembers[clanid][i][member_id]) ? ("{23C552}Online") : ("{BEBEBE}Offline"));
        strcat(buf, line);
    }

    strcat(buf, fmat_return("{MAIN}Total Slots\t{BEBEBE}%d / %d Used\t \n",
        ClanMembersCount[clanid], GetClanMemberSlots(clanid)));


    format_MainColor(playerid, buf);
    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_MEMBERS<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Back", "Close");
}

public DIALOG_SAFE_MEMBERS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    return ShowSafehouseMenu(playerid);
}

stock ShowSafehouseExpiry(playerid)
{
    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    new title[128];
    format(title, sizeof(title), "{MAIN}Safehouse Contract: {FFFFFF}%s", SafehouseData[index][sh_Name]);

    format_MainColor(playerid, title);
    new leftStr[24], nextStr[24];
    FormatSafehouseTime(GetSafehouseTimeLeft(index), leftStr);
    FormatSafehouseTime(GetSafehouseNextChargeIn(index), nextStr);

    new chargeStr[36], vaultStr[36], coinStr[36];
    format(chargeStr, sizeof(chargeStr), "%s", FormatMoney(SH_CHARGE_VALUE));
    format(vaultStr, sizeof(vaultStr), "%s", FormatMoney(SafehouseData[index][sh_Vault]));
    format(coinStr, sizeof(coinStr), "%s", FormatCoins(SH_RENEW_COINS));

    new buf[512];
    format(buf, sizeof(buf), "{BEBEBE}Parameter\t{BEBEBE}Value\n");
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Time Left\t{FFFFFF}%s\n", leftStr));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Next Charge\t{FFFFFF}%s\n", nextStr));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Charge Value\t{23C552}%s\n", chargeStr));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Vault Balance\t{23C552}%s\n", vaultStr));
    strcat(buf, fmat_return("{B4B5B7}\x95 {FFFFFF}Extend Contract\t{FFD700}+%d Days {BEBEBE}(%s Coins)\n", SH_RENEW_DAYS, coinStr));
    strcat(buf, "{B4B5B7}\x95 {FFFFFF}History\t{3CCFFA}[Logs]\n");


    format_MainColor(playerid, buf);
    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_EXPIRY<iiiis>,
        DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Select", "Back");
}

public DIALOG_SAFE_EXPIRY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseMenu(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    if (listitem == SAFE_EXPIRY_HISTORY)
        return ShowSafehouseHistory(playerid, index, "maintenance");

    if (listitem != SAFE_EXPIRY_RENEW)
        return ShowSafehouseExpiry(playerid);

    new leftStr[24], coinStr[36];
    FormatSafehouseTime(GetSafehouseTimeLeft(index), leftStr);
    format(coinStr, sizeof(coinStr), "%s", FormatCoins(SH_RENEW_COINS));

    new title[64] = "{MAIN}Contract - Extend Safehouse";

    new body[512];
    format(body, sizeof(body),
        "{FFFFFF}Extend the rental contract of {MAIN}%s{FFFFFF}?\n\n"\
        "{MAIN}\x95 {B4B5B7}Current Time: {FFFFFF}%s\n"\
        "{MAIN}\x95 {B4B5B7}Extension: {23C552}+%d Day(s)\n"\
        "{MAIN}\x95 {B4B5B7}Cost: {FFD700}%s Coins\n"\
        "{MAIN}\x95 {B4B5B7}Your Coins: {FFD700}%s\n\n"\
        "{BEBEBE}The contract cannot exceed %d days in total.",
        SafehouseData[index][sh_Name],
        leftStr,
        SH_RENEW_DAYS,
        coinStr,
        FormatCoins(User[playerid][playerCoins]),
        SH_DURATION / 86400);


    format_MainColor(playerid, body);
    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_RENEW<iiiis>,
        DIALOG_STYLE_MSGBOX, title, body, "Extend", "Back");
}

public DIALOG_SAFE_RENEW(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseExpiry(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    RenewSafehouse(playerid, index);
    return ShowSafehouseExpiry(playerid);
}

public DIALOG_SAFE_SELL(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowSafehouseMenu(playerid);

    new index = GetPlayerSafehouse(playerid);
    if (!IsValidSafehouseIndex(index)) return 1;

    SellSafehouse(playerid, index);
    return 1;
}

stock ShowSafehouseBuyDialog(playerid, index)
{
    if (!IsValidSafehouseIndex(index)) return 1;

    new title[64] = "{MAIN}Buy Safehouse";

    format_MainColor(playerid, title);
    new body[640];
    format(body, sizeof(body),
        "{FFFFFF}Purchase {MAIN}%s{FFFFFF} for your clan?\n\n"\
        "{MAIN}\x95 {B4B5B7}Price: {23C552}%s {FFFFFF}+ {FFD700}%s Coins\n"\
        "{MAIN}\x95 {B4B5B7}Initial Contract: {FFFFFF}%d Days\n"\
        "{MAIN}\x95 {B4B5B7}Maintenance: {23C552}%s {B4B5B7}every {FFFFFF}%d Days\n"\
        "{MAIN}\x95 {B4B5B7}Starting Slots: {23C552}%d Members\n\n"\
        "{BEBEBE}Only the clan leader can buy, and each clan may own one safehouse.",
        SafehouseData[index][sh_Name],
        FormatMoney(SH_BUY_PRICE), FormatCoins(SH_BUY_COINS),
        SH_DURATION / 86400,
        FormatMoney(SH_CHARGE_VALUE), SH_CHARGE_EVERY / 86400,
        CLAN_BASE_SLOTS + GetSafehouseLevelSlots(1));


    format_MainColor(playerid, body);
    g_PlayerSafehousePrompt[playerid] = index;

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_BUY<iiiis>,
        DIALOG_STYLE_MSGBOX, title, body, "Buy", "Cancel");
}

public DIALOG_SAFE_BUY(playerid, dialogid, response, listitem, string:inputtext[])
{
    new index = g_PlayerSafehousePrompt[playerid];
    g_PlayerSafehousePrompt[playerid] = -1;

    if (!response) return 1;
    if (!IsValidSafehouseIndex(index)) return 1;

    BuySafehouse(playerid, index);
    return 1;
}

stock ShowSafehousePasswordPrompt(playerid, index)
{
    g_PlayerSafehousePrompt[playerid] = index;

    new title[64] = "{MAIN}Safehouse - Enter Password";

    format_MainColor(playerid, title);
    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_ENTERPASS<iiiis>,
        DIALOG_STYLE_PASSWORD, title, "{FFFFFF}This safehouse is locked. Enter the password below:", "Enter", "Cancel");
}

public DIALOG_SAFE_ENTERPASS(playerid, dialogid, response, listitem, string:inputtext[])
{
    new index = g_PlayerSafehousePrompt[playerid];
    g_PlayerSafehousePrompt[playerid] = -1;

    if (!response) return 1;
    if (!IsValidSafehouseIndex(index)) return 1;
    if (SafehouseData[index][sh_Status] != SAFEHOUSE_STATUS_OWNED) return 1;

    if (IsSafehouseMember(playerid, index))
        return EnterSafehouse(playerid, index);

    if (SafehouseData[index][sh_Password][0] == '\0')
        return SendClientMessage(playerid, -1, "%s", SH_LOCKED);

    if (strcmp(inputtext, SafehouseData[index][sh_Password], false) != 0)
    {
        LogSafehouse(index, playerid, "guest_denied", "wrong password");
        return SendClientMessage(playerid, -1, "%s", SH_WRONG_PASSWORD);
    }

    LogSafehouse(index, playerid, "guest_enter", "entered with the password");

    return EnterSafehouse(playerid, index);
}

stock ShowSafehouseHistory(playerid, index, const filter[])
{
    if (!IsValidSafehouseIndex(index)) return 1;
    if (dbHandle == MYSQL_INVALID_HANDLE) return 1;

    new title[64];
    format(title, sizeof(title), "{MAIN}Safehouse - Audit History");

    format_MainColor(playerid, title);
    inline OnSafehouseHistory()
    {
        new rows = cache_num_rows();
        new buf[128 + (160 * SAFEHOUSE_HISTORY_ROWS)];
        format(buf, sizeof(buf), "{MAIN}Date\t{MAIN}Player\t{MAIN}Action\t{MAIN}Detail\n");

        if (!rows)
        {
            strcat(buf, "{BEBEBE}No records yet\t \t \t \n");
        }
        else
        {
            for (new i = 0; i < rows; i++)
            {
                new pname[MAX_PLAYER_NAME + 1], action[32], detail[80], datestr[24];
                cache_get_value_name(i, "player_name", pname, sizeof(pname));
                cache_get_value_name(i, "action", action, sizeof(action));
                cache_get_value_name(i, "detail", detail, sizeof(detail));
                cache_get_value_name(i, "date_str", datestr, sizeof(datestr));

                new line[256];
                format(line, sizeof(line), "{BEBEBE}%s\t{FFFFFF}%s\t{MAIN}%s\t{BEBEBE}%s\n",
                    datestr, pname, action, detail);
                strcat(buf, line);
            }
        }

        format_MainColor(playerid, buf);
        Dialog_ShowCallback(playerid, using public DIALOG_SAFE_HISTORY<iiiis>,
            DIALOG_STYLE_TABLIST_HEADERS, title, buf, "Back", "Close");
    }

    MySQL_TQueryInline(dbHandle, using inline OnSafehouseHistory,
        "SELECT `player_name`, `action`, `detail`, \
DATE_FORMAT(FROM_UNIXTIME(`created_at`), '%%d/%%m %%H:%%i') AS `date_str` \
FROM `safehouse_logs` WHERE `safehouse_id` = %d AND `action` LIKE '%%%e%%' \
ORDER BY `created_at` DESC LIMIT %d",
        SafehouseData[index][sh_SQLID], filter, SAFEHOUSE_HISTORY_ROWS);
    return 1;
}

public DIALOG_SAFE_HISTORY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    return ShowSafehouseMenu(playerid);
}

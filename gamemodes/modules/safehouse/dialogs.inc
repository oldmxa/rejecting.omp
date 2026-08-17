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

    new levelName[24];
    GetSafehouseLevelName(SafehouseData[index][sh_Level], levelName);

    new title[64] = "{MAIN}Safehouse";
    format_MainColor(playerid, title);

    new buf[1024];
    format(buf, sizeof(buf), "{MAIN}Option\t{MAIN}Current\n");

    new line[160];

    if (IsValidClanID(clanid))
    {
        format(line, sizeof(line), "{FFFFFF}Clan\t{%06x}%s {BEBEBE}[%s]\n",
            ClanInfo[clanid][clanColor] >>> 8, ClanInfo[clanid][clanName], ClanInfo[clanid][clanTag]);
    }
    else
    {
        format(line, sizeof(line), "{FFFFFF}Clan\t{BEBEBE}-\n");
    }
    strcat(buf, line);

    format(line, sizeof(line), "{FFFFFF}Name\t{BEBEBE}%s\n", SafehouseData[index][sh_Name]);
    strcat(buf, line);

    new passStr[SAFEHOUSE_PASS_LEN + 12] = "{BEBEBE}-";
    if (SafehouseData[index][sh_Password][0] != '\0')
        format(passStr, sizeof(passStr), "{23C552}%s", SafehouseData[index][sh_Password]);

    format(line, sizeof(line), "{FFFFFF}Password\t%s\n", passStr);
    strcat(buf, line);
    format(line, sizeof(line), "{FFFFFF}Vault\t{23C552}%s\n", FormatMoney(SafehouseData[index][sh_Vault]));
    strcat(buf, line);
    format(line, sizeof(line), "{FFFFFF}Upgrade\t{BEBEBE}Level %d - %s\n", SafehouseData[index][sh_Level], levelName);
    strcat(buf, line);
    format(line, sizeof(line), "{FFFFFF}Members\t{BEBEBE}%d / %d\n",
        ClanMembersCount[clanid], GetClanMemberSlots(clanid));
    strcat(buf, line);

    new leftStr[24];
    FormatSafehouseTime(GetSafehouseTimeLeft(index), leftStr);
    format(line, sizeof(line), "{FFFFFF}Expiry\t{BEBEBE}%s left\n", leftStr);
    strcat(buf, line);
    strcat(buf, "{F3230E}Sell base\t{BEBEBE}Release the safehouse\n");

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

            title = "{MAIN}Safehouse Name";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_NAME<iiiis>, DIALOG_STYLE_INPUT,
                title, "{FFFFFF}Enter the new safehouse name (3 to 31 characters):", "Save", "Back");
        }
        case SAFE_MENU_PASS:
        {
            if (!IsSafehouseLeader(playerid, index))
                return ShowSafehouseDenied(playerid, "Only the clan leader can change the password.");

            title = "{MAIN}Safehouse Password";
            format_MainColor(playerid, title);

            new body[160];
            format(body, sizeof(body), "{FFFFFF}Enter a new password (%d to %d characters), or leave it empty to remove it:",
                SAFEHOUSE_PASS_MIN, SAFEHOUSE_PASS_MAX);

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

            // FormatMoney devolve um array, entao cada valor precisa do proprio buffer
            new refund = (SH_BUY_PRICE * SH_REFUND_PCT) / 100;
            new refundStr[36], vaultStr[36], totalStr[36];
            format(refundStr, sizeof(refundStr), "%s", FormatMoney(refund));
            format(vaultStr, sizeof(vaultStr), "%s", FormatMoney(SafehouseData[index][sh_Vault]));
            format(totalStr, sizeof(totalStr), "%s", FormatMoney(refund + SafehouseData[index][sh_Vault]));

            new body[512];
            format(body, sizeof(body),
                "{FFFFFF}Sell the safehouse at {MAIN}%s{FFFFFF}?\n\n\
{BEBEBE}Refund ({FFFFFF}%d%%{BEBEBE} of %s)\t{23C552}%s\n\
{BEBEBE}Vault balance\t{23C552}%s\n\
{FFFFFF}You receive\t{23C552}%s\n\n\
{F3230E}Upgrades, password and vault are lost and the base becomes available again.",
                SafehouseData[index][sh_Name],
                SH_REFUND_PCT, FormatMoney(SH_BUY_PRICE), refundStr,
                vaultStr,
                totalStr);

            title = "{MAIN}Sell Safehouse";
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

    new title[64] = "{MAIN}Safehouse Vault";
    format_MainColor(playerid, title);

    // a primeira linha e o cabecalho da tabela; sem ela os listitem saem deslocados
    new buf[512];
    format(buf, sizeof(buf),
        "{MAIN}Option\t{MAIN}Value\n\
{FFFFFF}Balance\t{23C552}%s\n\
{FFFFFF}Deposit\t{BEBEBE}Add money to the vault\n\
{FFFFFF}Withdraw\t{BEBEBE}Take money out\n\
{FFFFFF}History\t{BEBEBE}Last vault movements\n",
        FormatMoney(SafehouseData[index][sh_Vault]));

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
            title = "{MAIN}Deposit";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_DEPOSIT<iiiis>, DIALOG_STYLE_INPUT,
                title, "{FFFFFF}How much do you want to deposit?", "Deposit", "Back");
        }
        case SAFE_VAULT_WITHDRAW:
        {
            title = "{MAIN}Withdraw";
            format_MainColor(playerid, title);
            return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_WITHDRAW<iiiis>, DIALOG_STYLE_INPUT,
                title, "{FFFFFF}How much do you want to withdraw?", "Withdraw", "Back");
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

    new title[64] = "{MAIN}Safehouse Upgrade";
    format_MainColor(playerid, title);

    new body[512];

    if (!IsValidSafehouseLevel(next))
    {
        format(body, sizeof(body),
            "{FFFFFF}Current level: {MAIN}%d - %s\n{FFFFFF}Member slots: {MAIN}%d\n\n{23C552}This safehouse is fully upgraded.",
            level, currentName, GetClanMemberSlots(SafehouseData[index][sh_ClanID]));

        format_MainColor(playerid, body);
        return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_UPGRADE<iiiis>,
            DIALOG_STYLE_MSGBOX, title, body, "Back", "");
    }

    new nextName[24], price, coins;
    GetSafehouseLevelName(next, nextName);
    GetSafehouseUpgradePrice(next, price, coins);

    format(body, sizeof(body),
        "{FFFFFF}Current level: {MAIN}%d - %s\n\
{FFFFFF}Next level: {MAIN}%d - %s\n\n\
{FFFFFF}Price: {MAIN}%s {FFFFFF}+ {MAIN}%s coins\n\
{FFFFFF}Member slots: {MAIN}%d {FFFFFF}-> {23C552}%d\n\
{FFFFFF}Interior: {MAIN}%d\n\n\
{BEBEBE}Upgrading swaps the interior immediately.",
        level, currentName, next, nextName,
        FormatMoney(price), FormatCoins(coins),
        CLAN_BASE_SLOTS + GetSafehouseLevelSlots(level),
        CLAN_BASE_SLOTS + GetSafehouseLevelSlots(next),
        g_SafehouseLevels[next - 1][lvl_Interior]);

    format_MainColor(playerid, body);

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

    new title[64] = "{MAIN}Safehouse Members";
    format_MainColor(playerid, title);

    // MAX_MEMBERS rows of name + rank + status with colour codes
    new buf[128 + (128 * MAX_MEMBERS)];
    format(buf, sizeof(buf), "{MAIN}Member\t{MAIN}Rank\t{MAIN}Status\n");

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

    new footer[128];
    format(footer, sizeof(footer), "{BEBEBE}Slots used: {FFFFFF}%d {BEBEBE}of {FFFFFF}%d\t \t \n",
        ClanMembersCount[clanid], GetClanMemberSlots(clanid));
    strcat(buf, footer);

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

    new title[64] = "{MAIN}Safehouse Expiry";
    format_MainColor(playerid, title);

    new leftStr[24], nextStr[24];
    FormatSafehouseTime(GetSafehouseTimeLeft(index), leftStr);
    FormatSafehouseTime(GetSafehouseNextChargeIn(index), nextStr);

    new chargeStr[36], vaultStr[36], coinStr[36];
    format(chargeStr, sizeof(chargeStr), "%s", FormatMoney(SH_CHARGE_VALUE));
    format(vaultStr, sizeof(vaultStr), "%s", FormatMoney(SafehouseData[index][sh_Vault]));
    format(coinStr, sizeof(coinStr), "%s", FormatCoins(SH_RENEW_COINS));

    new buf[768];
    format(buf, sizeof(buf),
        "{MAIN}Item\t{MAIN}Value\n\
{FFFFFF}Time left\t{MAIN}%s\n\
{FFFFFF}Next charge in\t{MAIN}%s\n\
{FFFFFF}Charge amount\t{MAIN}%s {BEBEBE}every %d day(s)\n\
{FFFFFF}Vault balance\t{23C552}%s\n\
{MAIN}Extend contract\t{FFD700}+%d day(s) for %s coins\n\
{FFFFFF}History\t{BEBEBE}Last maintenance records\n",
        leftStr,
        nextStr,
        chargeStr, SH_CHARGE_EVERY / 86400,
        vaultStr,
        SH_RENEW_DAYS, coinStr);

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

    new title[64] = "{MAIN}Extend Contract";
    format_MainColor(playerid, title);

    new body[512];
    format(body, sizeof(body),
        "{FFFFFF}Extend the contract of {MAIN}%s{FFFFFF}?\n\n\
{BEBEBE}Time left\t{MAIN}%s\n\
{BEBEBE}Added\t{23C552}+%d day(s)\n\
{BEBEBE}Cost\t{FFD700}%s coins\n\
{BEBEBE}Your coins\t{FFD700}%s\n\n\
{BEBEBE}The contract cannot go past %d day(s) in total.",
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

    new body[512];
    format(body, sizeof(body),
        "{FFFFFF}Safehouse: {MAIN}%s\n\n\
{FFFFFF}Price: {MAIN}%s {FFFFFF}+ {MAIN}%s coins\n\
{FFFFFF}Contract: {MAIN}%d days\n\
{FFFFFF}Maintenance: {MAIN}%s {FFFFFF}every {MAIN}%d days\n\
{FFFFFF}Starting slots: {MAIN}%d members\n\n\
{BEBEBE}Only the clan leader can buy, and each clan may own one safehouse.",
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

    new title[64] = "{MAIN}Safehouse Password";
    format_MainColor(playerid, title);

    return Dialog_ShowCallback(playerid, using public DIALOG_SAFE_ENTERPASS<iiiis>,
        DIALOG_STYLE_PASSWORD, title, "{FFFFFF}This safehouse is locked. Enter the password:", "Enter", "Cancel");
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
    format(title, sizeof(title), "{MAIN}Safehouse History");
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

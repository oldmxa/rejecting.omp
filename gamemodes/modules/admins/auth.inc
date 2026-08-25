
forward GenerateAdminAuthCode(playerid);
forward OnAdminPrivateChannelCreated(playerid, const code[]);
forward StartAdminPasswordAuth(playerid);
forward ShowAdminPasswordDialog(playerid);
forward OnAdminLoginTimeout(playerid);
forward OnAdminPasswordVerify(playerid, bool:success);
forward ShowAdminAuthMethodDialog(playerid);
forward DIALOG_ADMIN_AUTH_SELECT(playerid, dialogid, response, listitem, string:inputtext[]);
forward ShowCreateAdminPasswordDialog(playerid);
forward DIALOG_CREATE_ADMIN_PASS(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_ADMIN_LOGIN_PASS(playerid, dialogid, response, listitem, string:inputtext[]);
forward OnCreateAdminPasswordHash(playerid);

stock bool:CheckPlayerAdminAuth(playerid)
{
    if(!User[playerid][isLogged] || User[playerid][adminLevel] < 1)
        return false;

    if(!User[playerid][isAdminLogged])
    {
        if(User[playerid][adminAuthType] == 1)
        {
            SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {4287f5}You must log in as an administrator first! Enter your password in the authentication dialog.");
            StartAdminPasswordAuth(playerid);
        }
        else
        {
            SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {4287f5}You must log in as an administrator first! Use {FFFFFF}/alogin {4287f5}to receive the code on your Discord.");
        }
        return false;
    }
    return true;
}

#define CheckAdminAuth(%0) \
    if(!CheckPlayerAdminAuth(%0)) return 1

stock bool:CanAdminTargetPlayer(playerid, targetid, bool:allowSelf = false)
{
    if(!allowSelf && targetid == playerid)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}You cannot use this command on yourself.");
        return false;
    }
    if(User[targetid][adminLevel] >= User[playerid][adminLevel])
    {
        SendClientMessage(playerid, -1, "%s", SUPERIOR_RIGHTS);
        return false;
    }
    return true;
}

stock GenerateAdminAuthCode(playerid)
{
    if (!IsAdmin(playerid, 1)) return 0;

    if(User[playerid][adminAuthTimer] > gettime())
    {
        new remaining = User[playerid][adminAuthTimer] - gettime();
        va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {4287f5}You already have a code active! Wait {FFFFFF}%d seconds{4287f5} to generate a new one.", remaining);
        return 1;
    }

    new code = randomEx(100000, 999999);
    User[playerid][adminAuthCode] = code;
    User[playerid][isAdminLogged] = false;
    User[playerid][adminAuthTimer] = gettime() + 120;

    printf("[ADMIN-AUTH] Sending code to: %s (ID %d). Discord UID: %s", User[playerid][userName], playerid, User[playerid][dc_uid]);

    if(isnull(User[playerid][dc_uid]) || strequal(User[playerid][dc_uid], "-1"))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Your Discord is not linked! Link it to receive the login code.");
        printf("[ADMIN-AUTH] Error: dc_uid null or -1 for %s.", User[playerid][userName]);
        return 1;
    }

    new DCC_User:user = DCC_FindUserById(User[playerid][dc_uid]);
    if(user == DCC_INVALID_USER)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Discord User not found in the server.");
        printf("[ADMIN-AUTH] Error: DCC_FindUserById failed for %s.", User[playerid][dc_uid]);
        return 1;
    }

    new szCode[10];
    format(szCode, sizeof(szCode), "%d", code);

    printf("[ADMIN-AUTH] Opening private channel...");
    DCC_CreatePrivateChannel(user, "OnAdminPrivateChannelCreated", "is", playerid, szCode);
    return 1;
}

public OnAdminPrivateChannelCreated(playerid, const code[])
{
    new DCC_Channel:channel = DCC_GetCreatedPrivateChannel();
    
    if(channel == DCC_INVALID_CHANNEL)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}ERROR: Could not send to your PRIVATE. Are your DMs open?");
        printf("[ADMIN-AUTH] Error: Invalid DM channel for %s.", User[playerid][userName]);
        return 0;
    }

    new msg[200];
    format(msg, sizeof(msg), "Hello **%s**, your administrator login code for **%s** is: `%s` (Valid for 2 minutes)", User[playerid][userName], ServerHostname, code);
    
    DCC_SendChannelMessage(channel, msg);

    SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}The code has been sent to your {4287f5}PRIVATE DM{FFFFFF} on Discord.");
    printf("[ADMIN-AUTH] Success: code sent to %s.", User[playerid][userName]);
    return 1;
}

stock ShowAdminAuthMethodDialog(playerid)
{
    new title[64] = "{MAIN}Admin Auth Method";

    format_MainColor(playerid, title);
    new body[320];
    strcat(body, "{MAIN}Method\t{MAIN}Description\n");
    strcat(body, "{FFFFFF}\x95 {B4B5B7}Code (Discord DM)\t{FFFFFF}Receive a 6-digit code via Discord DM (/alogin)\n");
    strcat(body, "{FFFFFF}\x95 {B4B5B7}Password (Login)\t{FFFFFF}Authenticate using an Admin Password on join");

    format_MainColor(playerid, body);
    Dialog_ShowCallback(playerid, using public DIALOG_ADMIN_AUTH_SELECT<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
    return 1;
}

public DIALOG_ADMIN_AUTH_SELECT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        return ShowSecurityMenu(playerid);
    }

    switch(listitem)
    {
        case 0:
        {
            User[playerid][adminAuthType] = 0;

            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `admin_auth_type` = 0 WHERE `id` = %d", User[playerid][user_sqlid]);
            mysql_tquery(dbHandle, queryGlobal);

            va_SendClientMessage(playerid, -1, "{MAIN}[SECURITY] {FFFFFF}Admin Auth Method set to {23C552}Code (Discord /alogin){FFFFFF}.");
            ShowSecurityMenu(playerid);
        }
        case 1:
        {
            ShowCreateAdminPasswordDialog(playerid);
        }
    }
    return 1;
}

stock ShowCreateAdminPasswordDialog(playerid)
{
    new title[64] = "{MAIN}Create Admin Password";

    format_MainColor(playerid, title);
    new body[280];
    format(body, sizeof(body), "{FFFFFF}Enter a secure password for your Administrator account below:\n\n{FFFFFF}\x95 {B4B5B7}Note: This password will be required every time you join the server.");

    Dialog_ShowCallback(playerid, using public DIALOG_CREATE_ADMIN_PASS<iiiis>, DIALOG_STYLE_PASSWORD, title, body, "Save", "Cancel");
    return 1;
}

public DIALOG_CREATE_ADMIN_PASS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        SendClientMessage(playerid, -1, "{MAIN}[SECURITY] {FFFFFF}Admin password creation cancelled.");
        ShowSecurityMenu(playerid);
        return 1;
    }

    if(strlen(inputtext) < 4 || strlen(inputtext) > 32)
    {
        SendClientMessage(playerid, -1, "{MAIN}[SECURITY] {FF0000}Admin password must be between 4 and 32 characters long!");
        ShowCreateAdminPasswordDialog(playerid);
        return 1;
    }

    bcrypt_hash(playerid, "OnCreateAdminPasswordHash", inputtext, BCRYPT_COST);
    return 1;
}

public OnCreateAdminPasswordHash(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;

    new password[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(password);

    format(User[playerid][adminPass], BCRYPT_HASH_LENGTH, "%s", password);
    User[playerid][adminAuthType] = 1;

    mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "UPDATE `accounts` SET `admin_auth_type` = 1, `admin_password` = '%e' WHERE `id` = %d", password, User[playerid][user_sqlid]);
    mysql_tquery(dbHandle, queryGlobal);

    SendClientMessage(playerid, -1, "{MAIN}[SECURITY] {FFFFFF}Admin Password created successfully! Auth method set to {23C552}Password (Login){FFFFFF}.");
    ShowSecurityMenu(playerid);
    return 1;
}

static stock Admin_KillLoginTimer(playerid)
{
    if(User[playerid][adminLoginTimer] != 0)
    {
        KillTimer(User[playerid][adminLoginTimer]);
        User[playerid][adminLoginTimer] = 0;
    }
    return 1;
}

stock StartAdminPasswordAuth(playerid)
{
    if (!IsAdmin(playerid, 1)) return 0;
    if(User[playerid][isAdminLogged]) return 0;

    User[playerid][adminAuthAttempts] = 0;
    Admin_KillLoginTimer(playerid);

    User[playerid][adminLoginTimer] = SetTimerEx("OnAdminLoginTimeout", 30000, false, "i", playerid);
    ShowAdminPasswordDialog(playerid);
    return 1;
}

stock ShowAdminPasswordDialog(playerid)
{
    new title[64] = "{MAIN}Admin Authentication";

    new body[280];
    format(body, sizeof(body), "{FFFFFF}Welcome back, Admin %s.\n\nPlease enter your Admin Password below to log in as Administrator:\n\n{FFFFFF}\x95 {FF0000}Note: You have 30 seconds to enter your password or you will be kicked.", User[playerid][userName]);

    Dialog_ShowCallback(playerid, using public DIALOG_ADMIN_LOGIN_PASS<iiiis>, DIALOG_STYLE_PASSWORD, title, body, "Login", "Disconnect");
    return 1;
}

public DIALOG_ADMIN_LOGIN_PASS(playerid, dialogid, response, listitem, string:inputtext[])
{
    if(!response)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Authentication cancelled. You have been disconnected.");
        KickEx(playerid, 500);
        return 1;
    }

    if(isnull(inputtext))
    {
        SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Password cannot be empty!");
        ShowAdminPasswordDialog(playerid);
        return 1;
    }

    if(isnull(User[playerid][adminPass]))
        bcrypt_verify(playerid, "OnAdminPasswordVerify", inputtext, User[playerid][user_pass]);
    else
        bcrypt_verify(playerid, "OnAdminPasswordVerify", inputtext, User[playerid][adminPass]);

    return 1;
}

public OnAdminPasswordVerify(playerid, bool:success)
{
    if(!IsPlayerConnected(playerid)) return 0;

    if(success)
    {
        Admin_KillLoginTimer(playerid);

        User[playerid][adminAuthAttempts] = 0;
        User[playerid][isAdminLogged] = true;
        SendServerInfoMessage(playerid, ADMIN_LOGIN_WELCOME, User[playerid][userName]);
        
        new log_details[128];
        format(log_details, sizeof(log_details), "Admin %s logged in successfully (Password)", User[playerid][userName]);
        LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "auth", "Admin Login", log_details);
    }
    else
    {
        User[playerid][adminAuthAttempts]++;
        if(User[playerid][adminAuthAttempts] >= 3)
        {
            Admin_KillLoginTimer(playerid);

            SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FF0000}You failed 3 password attempts and have been temp-banned for 2 minutes.");

            new timestamp = gettime();
            new unban_timestamp = timestamp + 120;
            
            mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "INSERT INTO `banimentos` (`username`, `ip`, `gpci`, `admin`, `reason`, `ban_timestamp`, `unban_timestamp`) VALUES ('%e', '%e', '%e', 'System', 'Excessive Admin Login Failures', %d, %d)",
                User[playerid][userName], User[playerid][user_lastip], User[playerid][user_gpci], timestamp, unban_timestamp);
            mysql_tquery(dbHandle, queryGlobal);

            new log_details[128];
            format(log_details, sizeof(log_details), "Admin %s temp-banned (2m) for 3 failed admin password attempts", User[playerid][userName]);
            LogServerAction(User[playerid][user_sqlid], User[playerid][userName], "auth", "Admin Login Ban", log_details);

            KickEx(playerid, 500);
            return 1;
        }

        va_SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FFFFFF}Incorrect password! Attempt {FF0000}%d{FFFFFF}/3.", User[playerid][adminAuthAttempts]);
        ShowAdminPasswordDialog(playerid);
    }
    return 1;
}

public OnAdminLoginTimeout(playerid)
{
    User[playerid][adminLoginTimer] = 0;

    if(IsPlayerConnected(playerid) && User[playerid][adminLevel] > 0 && !User[playerid][isAdminLogged])
    {
        SendClientMessage(playerid, -1, "{MAIN}[ADMIN] {FF0000}You failed to enter your admin password within 30 seconds and were kicked.");
        KickEx(playerid, 500);
    }
    return 1;
}

public OnPlayerConnect(playerid)
{
    User[playerid][isAdminLogged] = false;
    User[playerid][adminAuthCode] = 0;
    User[playerid][adminAuthTimer] = 0;
    User[playerid][adminAuthType] = 0;
    User[playerid][adminAuthAttempts] = 0;
    User[playerid][adminPass][0] = '\0';
    Admin_KillLoginTimer(playerid);

    #if defined AAUT_OnPlayerConnect
        return AAUT_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AAUT_OnPlayerConnect
#if defined AAUT_OnPlayerConnect
    forward AAUT_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    Admin_KillLoginTimer(playerid);

    #if defined AAUT_OnPlayerDisconnect
        return AAUT_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect AAUT_OnPlayerDisconnect
#if defined AAUT_OnPlayerDisconnect
    forward AAUT_OnPlayerDisconnect(playerid, reason);
#endif

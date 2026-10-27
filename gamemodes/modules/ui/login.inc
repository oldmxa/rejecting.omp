new PlayerText:LoginRC[MAX_PLAYERS][17];
new bool:IsPlayerInLogin[MAX_PLAYERS];
new CameraLoginTimer[MAX_PLAYERS];

#define LOGIN_PANEL_LEFT      (243.0)
#define LOGIN_PANEL_RIGHT     (397.0)
#define LOGIN_PANEL_TOP       (110.0)
#define LOGIN_PANEL_BOTTOM    (244.0)
#define LOGIN_PANEL_R          (12.5)

#define LOGIN_CORNER_FIT       (6.0)

#define LOGIN_HOVER_COLOR (0xFFD700FF)

#define LOGIN_PANEL_W (LOGIN_PANEL_RIGHT - LOGIN_PANEL_LEFT)
#define LOGIN_PANEL_H (LOGIN_PANEL_BOTTOM - LOGIN_PANEL_TOP)

#define LOGIN_CORNER_SIZE ((LOGIN_PANEL_R * 2.0) + (LOGIN_CORNER_FIT * 2.0))
#define LOGIN_CORNER_L    (LOGIN_PANEL_LEFT - LOGIN_CORNER_FIT)
#define LOGIN_CORNER_R    (LOGIN_PANEL_RIGHT - (LOGIN_PANEL_R * 2.0) - LOGIN_CORNER_FIT)
#define LOGIN_CORNER_T    (LOGIN_PANEL_TOP - LOGIN_CORNER_FIT)
#define LOGIN_CORNER_B    (LOGIN_PANEL_BOTTOM - (LOGIN_PANEL_R * 2.0) - LOGIN_CORNER_FIT)

public OnPlayerConnect(playerid)
{
    for(new i = 0; i < sizeof(LoginRC[]); i++)
        LoginRC[playerid][i] = PlayerText:INVALID_TEXT_DRAW;

    IsPlayerInLogin[playerid] = false;

    #if defined LGIN_OnPlayerConnect
        return LGIN_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect LGIN_OnPlayerConnect
#if defined LGIN_OnPlayerConnect
    forward LGIN_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    IsPlayerInLogin[playerid] = false;
    KillTimer(CameraLoginTimer[playerid]);

    for(new i=0; i < sizeof(LoginRC[]); i++)
    {
        if(LoginRC[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, LoginRC[playerid][i]);
            LoginRC[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }
    }

    #if defined LGIN_OnPlayerDisconnect
        return LGIN_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect LGIN_OnPlayerDisconnect
#if defined LGIN_OnPlayerDisconnect
    forward LGIN_OnPlayerDisconnect(playerid, reason);
#endif

forward LoginScreensMusic(playerid);
public LoginScreensMusic(playerid)
{
    switch(random(4))
	{
		case 0: PlayAudioStreamForPlayer(playerid, "https://dl.dropboxusercontent.com/scl/fi/3h3bfkzjn9lkkim8pivdh/1000-Bocados.mp3?rlkey=5ed2grriojtrk31ir7pzaxw5t&dl=1");
		case 1: PlayAudioStreamForPlayer(playerid, "https://dl.dropboxusercontent.com/scl/fi/e7qii20jxcewx8877gj9j/Dissabores.mp3?rlkey=34fepxcbjq90d1v9jawxfi0wr&dl=1");
		case 2: PlayAudioStreamForPlayer(playerid, "https://dl.dropboxusercontent.com/scl/fi/pfn3an0k5ytj1io6jb9xz/Francisca.mp3?rlkey=oyf4urtnerprofka6ih6j6yte&dl=1");
		case 3: PlayAudioStreamForPlayer(playerid, "https://dl.dropboxusercontent.com/scl/fi/4x05tafaeaupnx3tp20tw/Montanha.mp3?rlkey=uvo7o10xteo18xoivd8xl5dcl&dl=1");
	}

    ClearChat(playerid, 100);
    return 1;
}

forward LoginScreensAnimated(playerid);
public LoginScreensAnimated(playerid)
{
    if(!IsPlayerInLogin[playerid]) return 1;

    TogglePlayerSpectating(playerid, 1); 
    
    new time_cam = 15000;

    switch(random(13))
    {
        case 0:
        {
            SetPlayerPos(playerid, 1335.117309, -982.668579, 202.226531-100.0);
            InterpolateCameraPos(playerid, 1335.117309, -982.668579, 202.226531, 1359.346557, -994.981811, 43.454010, 9000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 1335.542968, -982.761779, 197.245559, 1355.163452, -992.879699, 41.698246, 9000, CAMERA_MOVE);
            time_cam = 9000;
        }
        case 1:
        {
            SetPlayerPos(playerid, -2800.575439, 1946.292846, 9.821128-100.0);
            InterpolateCameraPos(playerid, -2800.575439, 1946.292846, 9.821128, -2095.973876, 2414.078613, 36.446117, 15000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, -2795.944580, 1948.178100, 9.817220, -2100.835449, 2413.132080, 35.760780, 15000, CAMERA_MOVE);
            time_cam = 15000;
        }
        case 2:
        {
            SetPlayerPos(playerid, -2565.683593, -575.448120, 128.396759-100.0);
            InterpolateCameraPos(playerid, -2565.683593, -575.448120, 128.396759, -2731.914794, -186.188644, 16.551181, 15000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, -2567.003417, -570.895935, 126.804222, -2731.856445, -191.182617, 16.789693, 15000, CAMERA_MOVE);
            time_cam = 15000;
        }
        case 3:
        {
            SetPlayerPos(playerid, 827.323547, -1874.888305, 23.979452-100.0);
            InterpolateCameraPos(playerid, 827.323547, -1874.888305, 23.979452, 66.566551, -1988.415527, 43.567714, 45000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 822.343505, -1874.958496, 23.538618, 70.389862, -1985.299560, 42.747226, 45000, CAMERA_MOVE);
            time_cam = 45000;
        }
        case 4:
        {
            SetPlayerPos(playerid, -1156.062622, 561.796325, 17.635738-100.0);
            InterpolateCameraPos(playerid, -1156.062622, 561.796325, 17.635738, -1519.754760, 1208.645385, 82.424659, 20000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, -1156.982055, 566.701599, 17.331239, -1522.607910, 1204.658447, 81.442802, 20000, CAMERA_MOVE);
            time_cam = 20000;
        }
        case 5:
        {
            SetPlayerPos(playerid, -2832.894042, 1075.414916, 40.042976-100.0);
            InterpolateCameraPos(playerid, -2832.894042, 1075.414916, 40.042976, -2567.480957, 1694.606323, 147.083724, 15000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, -2831.105224, 1080.040283, 39.405303, -2569.622802, 1690.272949, 145.805419, 15000, CAMERA_MOVE);
            time_cam = 15000;
        }
        case 6:
        {
            SetPlayerPos(playerid, 2077.037597, 1910.735107, 29.922159-100.0);
            InterpolateCameraPos(playerid, 2077.037597, 1910.735107, 29.922159, 2177.085693, 1914.485717, 76.301391, 25000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 2072.054443, 1910.988037, 30.246150, 2172.086914, 1914.438598, 76.406852, 25000, CAMERA_MOVE);
            time_cam = 25000;
        }
        case 7:
        {
            SetPlayerPos(playerid, 1544.126220, -1353.114868, 329.288299-100.0);
            InterpolateCameraPos(playerid, 1544.126220, -1353.114868, 329.288299, 1626.332641, -1373.562133, 440.902526, 30000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 1543.690429, -1353.117797, 324.307312, 1624.042602, -1371.274536, 437.091674, 30000, CAMERA_MOVE);
            time_cam = 30000;
        }
        case 8:
        {
            SetPlayerPos(playerid, 724.966247, -1802.078857, 2.082828-100.0);
            InterpolateCameraPos(playerid, 724.966247, -1802.078857, 2.082828, 715.309692, -1489.849365, 23.671327, 30000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 724.936828, -1797.081787, 1.915112, 720.096557, -1488.412963, 23.822275, 30000, CAMERA_MOVE);
            time_cam = 30000;
        }
        case 9:
        {
            SetPlayerPos(playerid, 591.171752, -2012.596191, 1.286335-100.0);
            InterpolateCameraPos(playerid, 591.171752, -2012.596191, 1.286335, 646.246154, -1321.586425, 99.962242, 20000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 591.206115, -2017.527465, 2.112067, 651.110412, -1320.434082, 99.856185, 20000, CAMERA_MOVE);
            time_cam = 20000;
        }
        case 10:
        {
            SetPlayerPos(playerid, 2057.707763, 1182.382080, 19.827589-100.0);
            InterpolateCameraPos(playerid, 2057.707763, 1182.382080, 19.827589, 2352.199218, 1027.007446, 115.217376, 30000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 2062.035156, 1179.977661, 20.529798, 2348.497802, 1029.874023, 113.461807, 30000, CAMERA_MOVE);
            time_cam = 30000;
        }
        case 11:
        {
            SetPlayerPos(playerid, 1977.258300, 1915.498413, 135.402572-100.0);
            InterpolateCameraPos(playerid, 1977.258300, 1915.498413, 135.402572, 2134.105712, 1906.739868, 17.790742, 20000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 1980.819702, 1915.437622, 131.893600, 2129.328857, 1907.125000, 19.217063, 20000, CAMERA_MOVE);
            time_cam = 20000;
        }
        case 12:
        {
            SetPlayerPos(playerid, 2518.935302, 2130.874511, 11.243128-100.0);
            InterpolateCameraPos(playerid, 2518.935302, 2130.874511, 11.243128, 2104.693359, 2144.577148, 43.355522, 90000, CAMERA_MOVE);
            InterpolateCameraLookAt(playerid, 2513.973388, 2131.480712, 11.352965, 2109.667236, 2144.506103, 42.849361, 90000, CAMERA_MOVE);
            time_cam = 90000;
        }
    }
    
    if(IsPlayerInLogin[playerid])
    {
        CameraLoginTimer[playerid] = SetTimerEx("LoginScreensAnimated", time_cam, false, "d", playerid);
    }
    return 1;
}

ShowLoginScreen(playerid, type)
{
    if(IsPlayerInLogin[playerid])
    {
        SelectTextDraw(playerid, LOGIN_HOVER_COLOR);
        return 1;
    }

    IsPlayerInLogin[playerid] = true;

    LoginRC[playerid][0] = CreatePlayerTextDraw(playerid, LOGIN_PANEL_LEFT + LOGIN_PANEL_R, LOGIN_PANEL_TOP, "LD_SPAC:white");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][0], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][0], LOGIN_PANEL_W - (LOGIN_PANEL_R * 2.0), LOGIN_PANEL_H);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][0], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][0], 0x1C1C1CFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][0], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][0], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][0], 0);

    LoginRC[playerid][1] = CreatePlayerTextDraw(playerid, LOGIN_PANEL_LEFT, LOGIN_PANEL_TOP + LOGIN_PANEL_R, "LD_SPAC:white");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][1], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][1], LOGIN_PANEL_W, LOGIN_PANEL_H - (LOGIN_PANEL_R * 2.0));
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][1], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][1], 0x1C1C1CFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][1], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][1], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][1], 0);

    LoginRC[playerid][2] = CreatePlayerTextDraw(playerid, LOGIN_CORNER_L, LOGIN_CORNER_T, "LD_BEAT:chit");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][2], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][2], LOGIN_CORNER_SIZE, LOGIN_CORNER_SIZE);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][2], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][2], 0x1C1C1CFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][2], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][2], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][2], 0);

    LoginRC[playerid][3] = CreatePlayerTextDraw(playerid, LOGIN_CORNER_R, LOGIN_CORNER_T, "LD_BEAT:chit");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][3], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][3], LOGIN_CORNER_SIZE, LOGIN_CORNER_SIZE);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][3], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][3], 0x1C1C1CFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][3], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][3], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][3], 0);

    LoginRC[playerid][4] = CreatePlayerTextDraw(playerid, LOGIN_CORNER_L, LOGIN_CORNER_B, "LD_BEAT:chit");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][4], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][4], LOGIN_CORNER_SIZE, LOGIN_CORNER_SIZE);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][4], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][4], 0x1C1C1CFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][4], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][4], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][4], 0);

    LoginRC[playerid][5] = CreatePlayerTextDraw(playerid, LOGIN_CORNER_R, LOGIN_CORNER_B, "LD_BEAT:chit");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][5], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][5], LOGIN_CORNER_SIZE, LOGIN_CORNER_SIZE);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][5], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][5], 0x1C1C1CFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][5], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][5], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][5], 0);

    LoginRC[playerid][6] = CreatePlayerTextDraw(playerid, 258.609008, 150.333389, "box");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][6], 0.000000, 1.193246);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][6], 385.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][6], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][6], -1);
    PlayerTextDrawUseBox(playerid, LoginRC[playerid][6], 1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][6], 0x121212FF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][6], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][6], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][6], 1);

    LoginRC[playerid][7] = CreatePlayerTextDraw(playerid, 258.609008, 150.333389, "box");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][7], 0.000000, 1.193246);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][7], 269.339538, 0.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][7], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][7], -1);
    PlayerTextDrawUseBox(playerid, LoginRC[playerid][7], 1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][7], 0x2A2A2AFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][7], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][7], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][7], 1);

    LoginRC[playerid][8] = CreatePlayerTextDraw(playerid, 258.609008, 181.633407, "box");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][8], 0.000000, 1.193246);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][8], 385.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][8], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][8], -1);
    PlayerTextDrawUseBox(playerid, LoginRC[playerid][8], 1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][8], 0x121212FF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][8], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][8], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][8], 1);
    LoginRC[playerid][9] = CreatePlayerTextDraw(playerid, 258.609008, 181.833175, "box");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][9], 0.000000, 1.183246);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][9], 269.339538, 0.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][9], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][9], -1);
    PlayerTextDrawUseBox(playerid, LoginRC[playerid][9], 1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][9], 0x2A2A2AFF);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][9], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][9], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][9], 1);

    // SERVER HOSTNAME TEXT
    LoginRC[playerid][10] = CreatePlayerTextDraw(playerid, 301.707427, 120.949935, ServerHostname);
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][10], 0.231330, 1.104162);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][10], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][10], -1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][10], 1);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][10], 83);
    PlayerTextDrawFont(playerid, LoginRC[playerid][10], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][10], 1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][10], 1);

    // ICONS (User & Lock)
    LoginRC[playerid][11] = CreatePlayerTextDraw(playerid, 252.844314, 148.416687, "LD_OTB2:ric1");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][11], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][11], 20.000000, 16.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][11], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][11], -1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][11], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][11], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][11], 0);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][11], 0);

    LoginRC[playerid][12] = CreatePlayerTextDraw(playerid, 251.844314, 178.016479, "");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][12], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][12], 24.000000, 18.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][12], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][12], -1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][12], 0);
    PlayerTextDrawFont(playerid, LoginRC[playerid][12], TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][12], 0);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][12], 0);
    PlayerTextDrawSetPreviewModel(playerid, LoginRC[playerid][12], 19804);
    PlayerTextDrawSetPreviewRot(playerid, LoginRC[playerid][12], 0.000000, 0.000000, 0.000000, 1.000000);

    // NAME LABEL
    new nameStr[128];
    format(nameStr, sizeof(nameStr), "%s", User[playerid][userName]);
    strreplace(nameStr, "_", " ");
    LoginRC[playerid][13] = CreatePlayerTextDraw(playerid, 275.944274, 149.749969, nameStr);
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][13], 0.264126, 1.250000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][13], 410.000000, 12.0000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][13], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][13], -1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][13], 1);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][13], 100);
    PlayerTextDrawFont(playerid, LoginRC[playerid][13], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][13], 1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][13], 1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][13], 0x00000000);
    PlayerTextDrawUseBox(playerid, LoginRC[playerid][13], 0);
    PlayerTextDrawSetSelectable(playerid, LoginRC[playerid][13], true);

    // PASSWORD LABEL
    new btnText[64];
    if(type == 1) format(btnText, sizeof(btnText), "Password");
    else format(btnText, sizeof(btnText), "Create password");

    LoginRC[playerid][14] = CreatePlayerTextDraw(playerid, 275.944274, 181.249755, btnText);
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][14], 0.231800, 1.156666);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][14], 368.000000, 12.0000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][14], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][14], -1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][14], 0);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][14], 1);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][14], 64);
    PlayerTextDrawFont(playerid, LoginRC[playerid][14], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][14], 1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][14], 1);
    PlayerTextDrawBoxColor(playerid, LoginRC[playerid][14], 0x00000000);
    PlayerTextDrawUseBox(playerid, LoginRC[playerid][14], 0);
    PlayerTextDrawSetSelectable(playerid, LoginRC[playerid][14], true);
    LoginRC[playerid][15] = CreatePlayerTextDraw(playerid, 278.000000, 117.000000, "ld_shtr:ufo");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][15], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][15], 20.000000, 20.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][15], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][15], -1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][15], 255);
    PlayerTextDrawFont(playerid, LoginRC[playerid][15], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][15], 0);

    LoginRC[playerid][16] = CreatePlayerTextDraw(playerid, 312.578430, 203.399978, "");
    PlayerTextDrawLetterSize(playerid, LoginRC[playerid][16], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, LoginRC[playerid][16], 23.000000, 27.000000);
    PlayerTextDrawAlignment(playerid, LoginRC[playerid][16], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, LoginRC[playerid][16], -1);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][16], 0);
    PlayerTextDrawSetOutline(playerid, LoginRC[playerid][16], 0);
    PlayerTextDrawBackgroundColor(playerid, LoginRC[playerid][16], 0);
    PlayerTextDrawFont(playerid, LoginRC[playerid][16], TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawSetProportional(playerid, LoginRC[playerid][16], 0);
    PlayerTextDrawSetShadow(playerid, LoginRC[playerid][16], 0);
    PlayerTextDrawSetPreviewModel(playerid, LoginRC[playerid][16], 19134);
    PlayerTextDrawSetPreviewRot(playerid, LoginRC[playerid][16], 0.000000, -90.000000, 90.000000, 1.000000);
    PlayerTextDrawSetSelectable(playerid, LoginRC[playerid][16], true);

    for(new i=0; i < sizeof(LoginRC[]); i++){
        if(LoginRC[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
            PlayerTextDrawShow(playerid, LoginRC[playerid][i]);
    }

    LoginScreensAnimated(playerid);
    LoginScreensMusic(playerid);
    
    SelectTextDraw(playerid, LOGIN_HOVER_COLOR);
    return 1;
}

HideLoginScreen(playerid)
{
    TogglePlayerSpectating(playerid, 0);
    CancelSelectTextDraw(playerid);

    KillTimer(CameraLoginTimer[playerid]);

    for(new i=0; i < sizeof(LoginRC[]); i++){
        if(LoginRC[playerid][i] != PlayerText:INVALID_TEXT_DRAW) {
            PlayerTextDrawDestroy(playerid, LoginRC[playerid][i]);
            LoginRC[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }
    }
    IsPlayerInLogin[playerid] = false;
    StopAudioStreamForPlayer(playerid);
    return 1;
}

forward UpdateLoginNameTD(playerid);
public UpdateLoginNameTD(playerid)
{
    if(IsPlayerInLogin[playerid] && LoginRC[playerid][13] != PlayerText:INVALID_TEXT_DRAW)
    {
        new nameStr[128];
        format(nameStr, sizeof(nameStr), "%s", User[playerid][userName]);
        strreplace(nameStr, "_", " ");
        PlayerTextDrawSetString(playerid, LoginRC[playerid][13], nameStr);
    }
    return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(IsPlayerInLogin[playerid])
    {
        if(clickedid == INVALID_TEXT_DRAW)
        {
            SelectTextDraw(playerid, LOGIN_HOVER_COLOR);
            return 1;
        }
    }

    #if defined LGIN_OnPlayerClickTextDraw
        return LGIN_OnPlayerClickTextDraw(playerid, clickedid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif
#define OnPlayerClickTextDraw LGIN_OnPlayerClickTextDraw
#if defined LGIN_OnPlayerClickTextDraw
    forward LGIN_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(IsPlayerInLogin[playerid])
    {
        if(playertextid == LoginRC[playerid][13])
        {
            CallLocalFunction("ShowChangeNickDialog", "d", playerid);
        }
        else if(playertextid == LoginRC[playerid][14] || playertextid == LoginRC[playerid][16])
        {
            CallLocalFunction("OnLoginTypePassword", "d", playerid);
        }
    }

    #if defined LGIN_OnPlayerClickPlayerTD
        return LGIN_OnPlayerClickPlayerTD(playerid, playertextid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerClickPlayerTD
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnPlayerClickPlayerTD
#endif
#define OnPlayerClickPlayerTextDraw LGIN_OnPlayerClickPlayerTD
#if defined LGIN_OnPlayerClickPlayerTD
    forward LGIN_OnPlayerClickPlayerTD(playerid, PlayerText:playertextid);
#endif
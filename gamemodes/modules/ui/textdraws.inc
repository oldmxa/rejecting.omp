//Global

new PlayerText:DeathInfo[MAX_PLAYERS][2];
new PlayerText:killsandexp[MAX_PLAYERS];
new PlayerText:killsandexpii[MAX_PLAYERS];
new PlayerText:speedometer[MAX_PLAYERS];

new PlayerText:topLocationTable[MAX_PLAYERS][MAX_LOCATIONS * 20 + 26];


new PlayerText:specTextDraw[MAX_PLAYERS][7];
new PlayerText:OutpostBar[MAX_PLAYERS][5];
new Text:votekickTextdraws[3];
new PlayerText:ClanHUD[MAX_PLAYERS][4];


#define COLOR_DETAIL1	0x5f9d92FF
#define COLOR_DETAIL2	0x133d47FF
#define COLOR_BOX		0x0f151fFF
#define COLOR_DETAILSHADE	0x83aff04d

InitRoundStatsTD(playerid)
{
	// Static Textdraws (0-25 reserved)
	topLocationTable[playerid][0] = CreatePlayerTextDraw(playerid, 3.200005, 390.257629, ""); // Box
	PlayerTextDrawFont(playerid, topLocationTable[playerid][0], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][0], 0.180000, 0.799998);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][0], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][0], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][0], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][0], 1280.000000, 1280.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][0], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][0], 0x0000004B);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][0], 0);

	topLocationTable[playerid][1] = CreatePlayerTextDraw(playerid, 279.000000, 133.000000, "ROUND_RESULTS"); // Header
	PlayerTextDrawFont(playerid, topLocationTable[playerid][1], TEXT_DRAW_FONT_2);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][1], 0.283333, 1.200000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][1], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][1], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][1], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][1], 400.000000, 17.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][1], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][1], 0x0000004B);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][1], 0);

	topLocationTable[playerid][2] = CreatePlayerTextDraw(playerid, 369.000000, 154.000000, "Kills"); // Header Kills
	PlayerTextDrawFont(playerid, topLocationTable[playerid][2], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][2], 0.191666, 0.800000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][2], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][2], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][2], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][2], 400.000000, 17.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][2], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][2], 0x0000004B);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][2], 0);

	topLocationTable[playerid][3] = CreatePlayerTextDraw(playerid, 229.000000, 148.000000, "LD_POKE:cd9s"); // Logo 1
	PlayerTextDrawFont(playerid, topLocationTable[playerid][3], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][3], 0.600000, 2.000000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][3], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][3], 0x00000055);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][3], 1);
	PlayerTextDrawUseBox(playerid, topLocationTable[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, topLocationTable[playerid][3], 0x00000000);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][3], 197.500000, 63.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][3], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][3], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][3], 0x000000E2);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][3], 0);

	topLocationTable[playerid][4] = CreatePlayerTextDraw(playerid, 546.000000, 66.699997, "LD_SPAC:white"); // Logo 2
	PlayerTextDrawFont(playerid, topLocationTable[playerid][4], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][4], 0.480000, 1.120000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][4], TEXT_DRAW_ALIGN:0);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][4], 0x000000FF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][4], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][4], 61.700001, 8.400000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][4], 2);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][4], 0);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][4], 0x000000FF);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][4], 0);

	topLocationTable[playerid][5] = CreatePlayerTextDraw(playerid, 574.572083, 53.782265, "_"); // Logo 3
	PlayerTextDrawFont(playerid, topLocationTable[playerid][5], TEXT_DRAW_FONT_3);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][5], 0.255226, 1.190832);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][5], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][5], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][5], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][5], 1280.000000, 1280.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][5], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][5], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][5], 0x000000FF);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][5], 0);

	topLocationTable[playerid][7] = CreatePlayerTextDraw(playerid, 241.000000, 154.000000, "Name"); // Header Name
	PlayerTextDrawFont(playerid, topLocationTable[playerid][7], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][7], 0.191666, 0.800000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][7], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][7], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][7], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][7], 400.000000, 17.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][7], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][7], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][7], 0x0000004B);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][7], 0);

	topLocationTable[playerid][8] = CreatePlayerTextDraw(playerid, 331.000000, 154.000000, "Damage"); // Header Damage
	PlayerTextDrawFont(playerid, topLocationTable[playerid][8], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][8], 0.191666, 0.800000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][8], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][8], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][8], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][8], 400.000000, 17.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][8], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][8], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][8], 0x0000004B);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][8], 0);

	topLocationTable[playerid][9] = CreatePlayerTextDraw(playerid, 413.000000, 154.000000, "Ratio"); // Header Ratio
	PlayerTextDrawFont(playerid, topLocationTable[playerid][9], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][9], 0.191666, 0.800000);
	PlayerTextDrawAlignment(playerid, topLocationTable[playerid][9], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, topLocationTable[playerid][9], 0xFFFFFFFF);
	PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][9], 1);
	PlayerTextDrawTextSize(playerid, topLocationTable[playerid][9], 400.000000, 17.000000);
	PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][9], 0);
	PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][9], 1);
	PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][9], 0x0000004B);
	PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][9], 0);

    new defaultInfoStr[192];
    new colorStr[20];

    if(strfind(GetPlayerHudTag(playerid, false), "~w~") != -1)
    {
        format(colorStr, sizeof(colorStr), "%s", GetPlayerHudTag(playerid, true));
    }
    else
    {
        format(colorStr, sizeof(colorStr), "%s", GetPlayerHudTag(playerid, false));
    }

    format(defaultInfoStr, sizeof(defaultInfoStr), "Best clan on Arena #0 - %sNone~n~Damage: %s0~w~ / Kills: %s0~n~MVP: %sNone", colorStr, colorStr, colorStr, colorStr);

	// Per-location Textdraws
	for (new i = 26; i < (MAX_LOCATIONS * 20 + 26); i += 20)
	{
		topLocationTable[playerid][i] = CreatePlayerTextDraw(playerid, 329.000000, 218.000000, defaultInfoStr); // Clan Info
		PlayerTextDrawFont(playerid, topLocationTable[playerid][i], TEXT_DRAW_FONT_1);
		PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][i], 0.191666, 0.800000);
		PlayerTextDrawAlignment(playerid, topLocationTable[playerid][i], TEXT_DRAW_ALIGN_CENTRE);
		PlayerTextDrawColor(playerid, topLocationTable[playerid][i], 0xFFFFFFFF);
		PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][i], 1);
		PlayerTextDrawTextSize(playerid, topLocationTable[playerid][i], 1280.000000, 1280.000000);
		PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][i], 1);
		PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][i], 0x0000004B);
		PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][i], 0);

		topLocationTable[playerid][i + 1] = CreatePlayerTextDraw(playerid, 241.000000, 166.000000, "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~N/A~n~N/A~n~N/A~n~N/A~n~N/A~n~"); // Names List
		PlayerTextDrawFont(playerid, topLocationTable[playerid][i + 1], TEXT_DRAW_FONT_1);
		PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][i + 1], 0.191666, 0.800000);
		PlayerTextDrawAlignment(playerid, topLocationTable[playerid][i + 1], TEXT_DRAW_ALIGN_LEFT);
		PlayerTextDrawColor(playerid, topLocationTable[playerid][i + 1], 0xFFFFFFFF);
		PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][i + 1], 1);
		PlayerTextDrawTextSize(playerid, topLocationTable[playerid][i + 1], 400.000000, 17.000000);
		PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][i + 1], 0);
		PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][i + 1], 1);
		PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][i + 1], 0x0000004B);
		PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][i + 1], 0);

		topLocationTable[playerid][i + 2] = CreatePlayerTextDraw(playerid, 337.000000, 166.000000, "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~N/A~n~N/A~n~N/A~n~N/A~n~N/A~n~"); // Stats 1 List
		PlayerTextDrawFont(playerid, topLocationTable[playerid][i + 2], TEXT_DRAW_FONT_1);
		PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][i + 2], 0.191666, 0.800000);
		PlayerTextDrawAlignment(playerid, topLocationTable[playerid][i + 2], TEXT_DRAW_ALIGN_CENTRE);
		PlayerTextDrawColor(playerid, topLocationTable[playerid][i + 2], 0xFFFFFFFF);
		PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][i + 2], 1);
		PlayerTextDrawTextSize(playerid, topLocationTable[playerid][i + 2], 1280.000000, 1280.000000);
		PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][i + 2], 0);
		PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][i + 2], 1);
		PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][i + 2], 0x0000004B);
		PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][i + 2], 0);

		topLocationTable[playerid][i + 3] = CreatePlayerTextDraw(playerid, 375.000000, 166.000000, "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~N/A~n~N/A~n~N/A~n~N/A~n~N/A~n~"); // Stats 2 List
		PlayerTextDrawFont(playerid, topLocationTable[playerid][i + 3], TEXT_DRAW_FONT_1);
		PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][i + 3], 0.191666, 0.800000);
		PlayerTextDrawAlignment(playerid, topLocationTable[playerid][i + 3], TEXT_DRAW_ALIGN_CENTRE);
		PlayerTextDrawColor(playerid, topLocationTable[playerid][i + 3], 0xFFFFFFFF);
		PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][i + 3], 1);
		PlayerTextDrawTextSize(playerid, topLocationTable[playerid][i + 3], 400.000000, 17.000000);
		PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][i + 3], 0);
		PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][i + 3], 1);
		PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][i + 3], 0x0000004B);
		PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][i + 3], 0);

		topLocationTable[playerid][i + 4] = CreatePlayerTextDraw(playerid, 413.000000, 166.000000, "~w~~h~~w~~h~~w~~h~~w~~h~~w~~h~N/A~n~N/A~n~N/A~n~N/A~n~N/A~n~"); // Stats 3 List (Ratio)
		PlayerTextDrawFont(playerid, topLocationTable[playerid][i + 4], TEXT_DRAW_FONT_1);
		PlayerTextDrawLetterSize(playerid, topLocationTable[playerid][i + 4], 0.191666, 0.800000);
		PlayerTextDrawAlignment(playerid, topLocationTable[playerid][i + 4], TEXT_DRAW_ALIGN_CENTRE);
		PlayerTextDrawColor(playerid, topLocationTable[playerid][i + 4], 0xFFFFFFFF);
		PlayerTextDrawSetProportional(playerid, topLocationTable[playerid][i + 4], 1);
		PlayerTextDrawTextSize(playerid, topLocationTable[playerid][i + 4], 400.000000, 17.000000);
		PlayerTextDrawSetShadow(playerid, topLocationTable[playerid][i + 4], 0);
		PlayerTextDrawSetOutline(playerid, topLocationTable[playerid][i + 4], 1);
		PlayerTextDrawBackgroundColor(playerid, topLocationTable[playerid][i + 4], 0x0000004B);
		PlayerTextDrawSetSelectable(playerid, topLocationTable[playerid][i + 4], 0);
	}
    UpdateRoundStatsColors(playerid);
}

UpdateRoundStatsColors(playerid)
{
    new colorStr[20];

    if(strfind(GetPlayerHudTag(playerid, false), "~w~") != -1)
    {
        format(colorStr, sizeof(colorStr), "%s", GetPlayerHudTag(playerid, true));
    }
    else
    {
        format(colorStr, sizeof(colorStr), "%s", GetPlayerHudTag(playerid, false));
    }

    new headerStr[64];
    format(headerStr, sizeof(headerStr), "%sROUND_RESULTS", colorStr);
    PlayerTextDrawSetString(playerid, topLocationTable[playerid][1], headerStr);

    format(headerStr, sizeof(headerStr), "%sKills", colorStr);
    PlayerTextDrawSetString(playerid, topLocationTable[playerid][2], headerStr);

    format(headerStr, sizeof(headerStr), "%sName", colorStr);
    PlayerTextDrawSetString(playerid, topLocationTable[playerid][7], headerStr);

    format(headerStr, sizeof(headerStr), "%sDamage", colorStr);
    PlayerTextDrawSetString(playerid, topLocationTable[playerid][8], headerStr);

    format(headerStr, sizeof(headerStr), "%sRatio", colorStr);
    PlayerTextDrawSetString(playerid, topLocationTable[playerid][9], headerStr);
}

HideRoundStatsTD(playerid)
{
	for(new i = 0; i <= 5; i++)
		PlayerTextDrawHide(playerid, topLocationTable[playerid][i]);

	for(new i = 7; i <= 9; i++)
		PlayerTextDrawHide(playerid, topLocationTable[playerid][i]);

	for(new loc = 0; loc < MAX_LOCATIONS; loc++)
	{
		new base = (loc * 20) + 26;
		for(new i = 0; i <= 4; i++)
			PlayerTextDrawHide(playerid, topLocationTable[playerid][base + i]);
	}
	return 1;
}

gmtextdraws()
{
	votekickTextdraws[0] = TextDrawCreate(609.000000, 107.000000, "_");
	TextDrawFont(votekickTextdraws[0], TEXT_DRAW_FONT_1);
	TextDrawLetterSize(votekickTextdraws[0], 0.600000, 5.000000);
	TextDrawTextSize(votekickTextdraws[0], 525.000000, 40.0);
	TextDrawSetOutline(votekickTextdraws[0], 1);
	TextDrawSetShadow(votekickTextdraws[0], 0);
	TextDrawAlignment(votekickTextdraws[0], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(votekickTextdraws[0], -1);
	TextDrawBackgroundColor(votekickTextdraws[0], 255);
	TextDrawBoxColor(votekickTextdraws[0], 135);
	TextDrawUseBox(votekickTextdraws[0], true);
	TextDrawSetProportional(votekickTextdraws[0], true);
	TextDrawSetSelectable(votekickTextdraws[0], false);

	votekickTextdraws[1] = TextDrawCreate(535.000000, 110.0, "VOTEKICK");
	TextDrawFont(votekickTextdraws[1], TEXT_DRAW_FONT_1);
	TextDrawLetterSize(votekickTextdraws[1], 0.395832, 1.549998);
	TextDrawTextSize(votekickTextdraws[1], 400.0, 17.000000);
	TextDrawSetOutline(votekickTextdraws[1], 1);
	TextDrawSetShadow(votekickTextdraws[1], 0);
	TextDrawAlignment(votekickTextdraws[1], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(votekickTextdraws[1], -1);
	TextDrawBackgroundColor(votekickTextdraws[1], 255);
	TextDrawBoxColor(votekickTextdraws[1], 50);
	TextDrawUseBox(votekickTextdraws[1], false);
	TextDrawSetProportional(votekickTextdraws[1], true);
	TextDrawSetSelectable(votekickTextdraws[1], false);

	votekickTextdraws[2] = TextDrawCreate(567.000000, 132.0, "0 (Y) - 0 (N)");
	TextDrawFont(votekickTextdraws[2], TEXT_DRAW_FONT_1);
	TextDrawLetterSize(votekickTextdraws[2], 0.249999, 0.899999);
	TextDrawTextSize(votekickTextdraws[2], 399.000000, 75.500000);
	TextDrawSetOutline(votekickTextdraws[2], 1);
	TextDrawSetShadow(votekickTextdraws[2], 0);
	TextDrawAlignment(votekickTextdraws[2], TEXT_DRAW_ALIGN_CENTRE);
	TextDrawColor(votekickTextdraws[2], -1);
	TextDrawBackgroundColor(votekickTextdraws[2], 255);
	TextDrawBoxColor(votekickTextdraws[2], 50);
	TextDrawUseBox(votekickTextdraws[2], false);
	TextDrawSetProportional(votekickTextdraws[2], true);
	TextDrawSetSelectable(votekickTextdraws[2], false);
}

playertextdraws(playerid)
{
	RecreatePlayerFpsPingTd(playerid);

	DeathInfo[playerid][0] = CreatePlayerTextDraw(playerid, 326.400146, 390.0, "_"); // You killed someone
	PlayerTextDrawFont(playerid, DeathInfo[playerid][0], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, DeathInfo[playerid][0], 0.25, 0.91);
	PlayerTextDrawSetShadow(playerid, DeathInfo[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, DeathInfo[playerid][0], 1);
	PlayerTextDrawAlignment(playerid, DeathInfo[playerid][0], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, DeathInfo[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, DeathInfo[playerid][0], MAIN_BACKGROUND_COLOUR);

	DeathInfo[playerid][1] = CreatePlayerTextDraw(playerid, 326.000000, 382.0, "_"); // Killed by someone
	PlayerTextDrawFont(playerid, DeathInfo[playerid][1], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, DeathInfo[playerid][1], 0.25, 0.91);
	PlayerTextDrawSetOutline(playerid, DeathInfo[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, DeathInfo[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, DeathInfo[playerid][1], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, DeathInfo[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, DeathInfo[playerid][1], MAIN_BACKGROUND_COLOUR);

	killsandexp[playerid] = CreatePlayerTextDraw(playerid, 320.399749, 92.933311, "Double kill!");
	PlayerTextDrawLetterSize(playerid, killsandexp[playerid], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, killsandexp[playerid], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, killsandexp[playerid], -1);
	PlayerTextDrawSetShadow(playerid, killsandexp[playerid], 1);
	PlayerTextDrawSetOutline(playerid, killsandexp[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, killsandexp[playerid], 100);
	PlayerTextDrawFont(playerid, killsandexp[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, killsandexp[playerid], true);
	PlayerTextDrawSetShadow(playerid, killsandexp[playerid], 1);

	killsandexpii[playerid] = CreatePlayerTextDraw(playerid, 615.000000, 56.000000, "_");
	PlayerTextDrawFont(playerid, killsandexpii[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, killsandexpii[playerid], 0.179167, 0.750000);
	PlayerTextDrawSetOutline(playerid, killsandexpii[playerid], 1);
	PlayerTextDrawAlignment(playerid, killsandexpii[playerid], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, killsandexpii[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, killsandexpii[playerid], 80);
	PlayerTextDrawBoxColor(playerid, killsandexpii[playerid], 50);
	PlayerTextDrawSetProportional(playerid, killsandexpii[playerid], 1);

	speedometer[playerid] = CreatePlayerTextDraw(playerid,153.000000, 373.000000, "_");
	PlayerTextDrawBackgroundColor(playerid,speedometer[playerid], 60);
	PlayerTextDrawFont(playerid,speedometer[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid,speedometer[playerid], 0.200000, 0.899999);
	PlayerTextDrawColor(playerid,speedometer[playerid], -1);
	PlayerTextDrawSetOutline(playerid,speedometer[playerid], 1);
	PlayerTextDrawSetProportional(playerid,speedometer[playerid], true);

	specTextDraw[playerid][1] = CreatePlayerTextDraw(playerid, 61.000000, 187.000000, "__");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][1], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][1], 0.166667, 0.950000);
	PlayerTextDrawSetOutline(playerid, specTextDraw[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][1], 0);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][1], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][1], 40);

	specTextDraw[playerid][0] = CreatePlayerTextDraw(playerid, 7.000000, 185.000000, "LD_POKE:cd9s");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][0], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][0], 0.600000, 2.0);
	PlayerTextDrawTextSize(playerid, specTextDraw[playerid][0], 111.500000, 196.000000);
	PlayerTextDrawSetOutline(playerid, specTextDraw[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][0], 0);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][0], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][0], 85);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][0], 255);

	specTextDraw[playerid][2] = CreatePlayerTextDraw(playerid, 11.000000, 204.000000, "ID: 20~n~HP: (~r~~h~200~w~)~n~PING: 500~n~FPS: 101~n~Version: 0.3.7~n~Mobile: No~n~");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][2], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][2], 0.225000, 0.800000);
	PlayerTextDrawSetOutline(playerid, specTextDraw[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][2], 0);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][2], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][2], 40);

	specTextDraw[playerid][3] = CreatePlayerTextDraw(playerid, 58.000000, 262.0, "Weapons");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][3], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][3], 0.225000, 0.800000);
	PlayerTextDrawSetOutline(playerid, specTextDraw[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][3], 0);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][3], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][3], 40);

	specTextDraw[playerid][4] = CreatePlayerTextDraw(playerid, 11.000000, 274.000000, "Deagle (900)~n~Shotgun (200)~n~Sniper (100)~n~");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][4], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][4], 0.225000, 0.800000);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][4], 1);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][4], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][4], 40);

	specTextDraw[playerid][5] = CreatePlayerTextDraw(playerid, 58.000000, 315.000000, "Arena Stats");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][5], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][5], 0.225000, 0.800000);
	PlayerTextDrawSetOutline(playerid, specTextDraw[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][5], 0);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][5], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][5], 40);

	specTextDraw[playerid][6] = CreatePlayerTextDraw(playerid, 11.000000, 327.000000, "Kills: 0~n~Deaths: 0~n~Ratio: 0.00~n~Damage: 0~n~Accuracy: 0.0%");
	PlayerTextDrawFont(playerid, specTextDraw[playerid][6], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, specTextDraw[playerid][6], 0.225000, 0.800000);
	PlayerTextDrawSetOutline(playerid, specTextDraw[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, specTextDraw[playerid][6], 0);
	PlayerTextDrawAlignment(playerid, specTextDraw[playerid][6], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, specTextDraw[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, specTextDraw[playerid][6], 40);



	// CLAN HUD PLAYER

	ClanHUD[playerid][0] = CreatePlayerTextDraw(playerid, 554.000000, 99.000000, "~w~~h~Clan: ~g~WOOAH  ~w~~h~Kills: ~g~5000000");
	PlayerTextDrawFont(playerid, ClanHUD[playerid][0], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, ClanHUD[playerid][0], 0.266667, 0.900000);
	PlayerTextDrawTextSize(playerid, ClanHUD[playerid][0], 999.000000, 999.500000);
	PlayerTextDrawSetShadow(playerid, ClanHUD[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, ClanHUD[playerid][0], 1);
	PlayerTextDrawAlignment(playerid, ClanHUD[playerid][0], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, ClanHUD[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, ClanHUD[playerid][0], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawSetProportional(playerid, ClanHUD[playerid][0], true);
	PlayerTextDrawSetSelectable(playerid, ClanHUD[playerid][0], false);
	
	ClanHUD[playerid][1] = CreatePlayerTextDraw(playerid, 554.000000, 107.000000, "~w~~h~Zone Points: ~g~5000000");
	PlayerTextDrawFont(playerid, ClanHUD[playerid][1], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, ClanHUD[playerid][1], 0.266667, 0.900000);
	PlayerTextDrawTextSize(playerid, ClanHUD[playerid][1], 999.000000, 999.500000);
	PlayerTextDrawSetShadow(playerid, ClanHUD[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, ClanHUD[playerid][1], 1);
	PlayerTextDrawAlignment(playerid, ClanHUD[playerid][1], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, ClanHUD[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, ClanHUD[playerid][1], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawSetProportional(playerid, ClanHUD[playerid][1], true);
	PlayerTextDrawSetSelectable(playerid, ClanHUD[playerid][1], false);
	
	ClanHUD[playerid][2] = CreatePlayerTextDraw(playerid, 555.0, 115.000000, "~w~~h~Rank: ~g~Leader");	PlayerTextDrawFont(playerid, ClanHUD[playerid][2], TEXT_DRAW_FONT_1);
	PlayerTextDrawLetterSize(playerid, ClanHUD[playerid][2], 0.266667, 0.900000);
	PlayerTextDrawTextSize(playerid, ClanHUD[playerid][2], 999.000000, 999.500000);
	PlayerTextDrawSetShadow(playerid, ClanHUD[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, ClanHUD[playerid][2], 1);
	PlayerTextDrawAlignment(playerid, ClanHUD[playerid][2], TEXT_DRAW_ALIGN_CENTRE);
	PlayerTextDrawColor(playerid, ClanHUD[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, ClanHUD[playerid][2], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawSetProportional(playerid, ClanHUD[playerid][2], true);
	PlayerTextDrawSetSelectable(playerid, ClanHUD[playerid][2], false);

    ClanHUD[playerid][3] = CreatePlayerTextDraw(playerid, 3.126094, 210.166687, "_");
	PlayerTextDrawTextSize(playerid, ClanHUD[playerid][3], 999.000000, 999.500000);
    PlayerTextDrawLetterSize(playerid, ClanHUD[playerid][3], 0.197172, 0.776665);
    PlayerTextDrawAlignment(playerid, ClanHUD[playerid][3], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColor(playerid, ClanHUD[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, ClanHUD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, ClanHUD[playerid][3], 1);
    PlayerTextDrawBackgroundColor(playerid, ClanHUD[playerid][3], MAIN_BACKGROUND_COLOUR);
    PlayerTextDrawFont(playerid, ClanHUD[playerid][3], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, ClanHUD[playerid][3], 1);

    OutpostBar[playerid][0] = CreatePlayerTextDraw(playerid, 235.0, 340.0, "LD_POKE:cd9s"); // Outer Rounded Card Frame (Round Results sprite)
    PlayerTextDrawFont(playerid, OutpostBar[playerid][0], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawColor(playerid, OutpostBar[playerid][0], 0x00000066);
    PlayerTextDrawTextSize(playerid, OutpostBar[playerid][0], 170.0, 34.0);

    OutpostBar[playerid][1] = CreatePlayerTextDraw(playerid, 245.0, 362.0, "LD_POKE:cd9s"); // Rounded Track Background
    PlayerTextDrawFont(playerid, OutpostBar[playerid][1], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawColor(playerid, OutpostBar[playerid][1], 0x000000A0);
    PlayerTextDrawTextSize(playerid, OutpostBar[playerid][1], 150.0, 6.0);

    OutpostBar[playerid][2] = CreatePlayerTextDraw(playerid, 245.0, 362.0, "LD_POKE:cd9s"); // Rounded Fill Bar (Progress)
    PlayerTextDrawFont(playerid, OutpostBar[playerid][2], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawColor(playerid, OutpostBar[playerid][2], 0x23C552FF);
    PlayerTextDrawTextSize(playerid, OutpostBar[playerid][2], 2.0, 6.0);

    OutpostBar[playerid][3] = CreatePlayerTextDraw(playerid, 320.0, 345.0, "CAPTURING OUTPOST [ ~g~0%~w~ ]"); // Header (Font 2)
    PlayerTextDrawFont(playerid, OutpostBar[playerid][3], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, OutpostBar[playerid][3], 0.22, 1.00);
    PlayerTextDrawAlignment(playerid, OutpostBar[playerid][3], TEXT_DRAW_ALIGN_CENTRE);
    PlayerTextDrawColor(playerid, OutpostBar[playerid][3], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, OutpostBar[playerid][3], 1);
    PlayerTextDrawSetShadow(playerid, OutpostBar[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, OutpostBar[playerid][3], 0x0000004B);
    PlayerTextDrawSetProportional(playerid, OutpostBar[playerid][3], 1);

    OutpostBar[playerid][4] = CreatePlayerTextDraw(playerid, 320.0, 355.0, "HOLD POSITION WITHIN ~y~70M ~w~RANGE"); // Subtitle
    PlayerTextDrawFont(playerid, OutpostBar[playerid][4], TEXT_DRAW_FONT_1);
    PlayerTextDrawLetterSize(playerid, OutpostBar[playerid][4], 0.16, 0.75);
    PlayerTextDrawAlignment(playerid, OutpostBar[playerid][4], TEXT_DRAW_ALIGN_CENTRE);
    PlayerTextDrawColor(playerid, OutpostBar[playerid][4], 0xCCCCCCFF);
    PlayerTextDrawSetOutline(playerid, OutpostBar[playerid][4], 1);
    PlayerTextDrawSetShadow(playerid, OutpostBar[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, OutpostBar[playerid][4], 0x0000004B);
    PlayerTextDrawSetProportional(playerid, OutpostBar[playerid][4], 1);

    InitRoundStatsTD(playerid);
    }
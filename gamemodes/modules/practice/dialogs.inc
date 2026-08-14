#if defined _practice_dialogs_incl
    #endinput
#endif
#define _practice_dialogs_incl

enum
{
    PRAC_ROW_TYPE,
    PRAC_ROW_MODEL,
    PRAC_ROW_NPCFIGHT,
    PRAC_ROW_NPCWEAPON,
    PRAC_ROW_SPEED,
    PRAC_ROW_HEIGHT,
    PRAC_ROW_TOLERANCE,
    PRAC_ROW_LATENCY,
    PRAC_ROW_RECORD,
    PRAC_ROW_MOVING,
    PRAC_ROW_POS1,
    PRAC_ROW_POS2,
    PRAC_ROW_STATS,
    PRAC_ROW_RESTART,
    PRAC_ROW_EXIT,

    PRAC_ROW_COUNT
};

forward DIALOG_PRAC_MAIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_MODEL(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_SKIN(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_STYLE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_WEAPON(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_SPEED(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_HEIGHT(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_TOLERANCE(playerid, dialogid, response, listitem, string:inputtext[]);
forward DIALOG_PRAC_LATENCY(playerid, dialogid, response, listitem, string:inputtext[]);

stock ShowPracticeDialog(playerid)
{
    new body[1600], idx = g_Practice[playerid][pracModel];
    if (idx < 0 || idx >= sizeof(g_PracticeModels)) idx = 0;

    new widx = g_Practice[playerid][pracNpcWeapon];
    if (widx < 0 || widx >= sizeof(g_PracticeWeapons)) widx = 0;

    new sidx = g_Practice[playerid][pracMoveStyle];
    if (sidx < 0 || sidx >= sizeof(g_PracticeStyles)) sidx = 0;

    strcat(body, "{A29582}Setting\t{A29582}Value\n");

    new bool:isNpc = (g_Practice[playerid][pracTargetType] == PRACTICE_TYPE_NPC);
    new bool:locked = PracticeRecordLocks(playerid);

    strcat(body, fmat_return("{FFFFFF}\x95 Target Type\t%s\n",
        isNpc ? ("{23C552}NPC") : ("{BEBEBE}Ball")));

    if (isNpc) strcat(body, fmat_return("{FFFFFF}\x95 NPC Skin\t{BEBEBE}ID %d\n", g_Practice[playerid][pracSkin]));
    else strcat(body, fmat_return("{FFFFFF}\x95 Target Model\t{BEBEBE}%s\n", g_PracticeModels[idx][pmName]));

    strcat(body, fmat_return("{FFFFFF}\x95 NPC Combat\t%s\n",
        !isNpc ? ("{7F7F7F}-") : (g_Practice[playerid][pracNpcFight] ? ("{23C552}Shoots back") : ("{F84F31}Passive"))));

    new wepLabel[20] = "{7F7F7F}-";
    if (isNpc && g_Practice[playerid][pracNpcFight])
        format(wepLabel, sizeof(wepLabel), "{BEBEBE}%s", g_PracticeWeapons[widx][pwName]);

    strcat(body, fmat_return("{FFFFFF}\x95 NPC Weapon\t%s\n", wepLabel));


    if (!isNpc) strcat(body, fmat_return("{FFFFFF}\x95 Speed\t{BEBEBE}%.1f\n", g_Practice[playerid][pracSpeed]));
    else if (locked) strcat(body, "{FFFFFF}\x95 Move Style\t{7F7F7F}Locked by recording\n");
    else strcat(body, fmat_return("{FFFFFF}\x95 Move Style\t{BEBEBE}%s\n", g_PracticeStyles[sidx][psName]));
    strcat(body, fmat_return("{FFFFFF}\x95 Height\t{BEBEBE}%.1f\n", g_Practice[playerid][pracHeight]));
    strcat(body, fmat_return("{FFFFFF}\x95 Hit Tolerance\t{BEBEBE}%d ms\n", g_Practice[playerid][pracTolerance]));

    new lagLabel[40] = "{7F7F7F}Ball target only";
    if (!isNpc)
    {
        if (g_Practice[playerid][pracLatency] > 0) format(lagLabel, sizeof(lagLabel), "{FFA500}%d ms", g_Practice[playerid][pracLatency]);
        else format(lagLabel, sizeof(lagLabel), "{BEBEBE}Off");
    }

    strcat(body, fmat_return("{FFFFFF}\x95 Lag Simulation\t%s\n", lagLabel));

    new recLabel[44] = "{7F7F7F}NPC target only";
    if (isNpc)
    {
        if (g_Practice[playerid][pracRecording])       format(recLabel, sizeof(recLabel), "{FFA500}Recording... select to stop");
        else if (g_Practice[playerid][pracRecPending]) format(recLabel, sizeof(recLabel), "{FFA500}Processing...");
        else if (PracticeHasRecord(playerid))          format(recLabel, sizeof(recLabel), "{23C552}Playing - select to delete it");
        else                                           format(recLabel, sizeof(recLabel), "{BEBEBE}None - select to record");
    }

    strcat(body, fmat_return("{FFFFFF}\x95 NPC Record\t%s\n", recLabel));

    if (locked)
    {
        strcat(body, "{FFFFFF}\x95 Movement\t{7F7F7F}Locked by recording\n");
        strcat(body, "{FFFFFF}\x95 Position 1\t{7F7F7F}Locked by recording\n");
        strcat(body, "{FFFFFF}\x95 Position 2\t{7F7F7F}Locked by recording\n");
    }
    else
    {
        strcat(body, fmat_return("{FFFFFF}\x95 Movement\t%s\n",
            g_Practice[playerid][pracMoving] ? ("{23C552}Moving") : ("{F84F31}Static")));

        strcat(body, fmat_return("{FFFFFF}\x95 Position 1\t%s\n",
            g_Practice[playerid][pracHasPos1] ? ("{23C552}Defined {BEBEBE}- select to remove") : ("{F84F31}Not set {BEBEBE}- select to use your spot")));

        strcat(body, fmat_return("{FFFFFF}\x95 Position 2\t%s\n",
            g_Practice[playerid][pracHasPos2] ? ("{23C552}Defined {BEBEBE}- select to remove") : ("{F84F31}Not set {BEBEBE}- select to use your spot")));
    }

    new Float:acc = 0.0;
    if (g_Practice[playerid][pracShots] > 0)
        acc = (float(g_Practice[playerid][pracHits]) / float(g_Practice[playerid][pracShots])) * 100.0;

    strcat(body, fmat_return("{FFFFFF}\x95 Accuracy\t{BEBEBE}%d/%d {B4B5B7}(%.1f%%) - select to reset\n",
        g_Practice[playerid][pracHits], g_Practice[playerid][pracShots], acc));

    strcat(body, "{FFFFFF}\x95 Respawn Target\t{BEBEBE}Apply changes\n");
    strcat(body, "{F84F31}\x95 Exit Practice\t{BEBEBE}Leave the private world\n");

    new title[64] = "{MAIN}Practice - Settings";
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_MAIN<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Close");
}

stock ShowPracticeModelDialog(playerid)
{
    new body[512];
    strcat(body, "{A29582}Model\t{A29582}ID\n");

    for (new i; i < sizeof(g_PracticeModels); i++)
    {
        strcat(body, fmat_return("%s{FFFFFF}%s\t{BEBEBE}%d\n",
            STYLE_MARK(g_Practice[playerid][pracModel] == i),
            g_PracticeModels[i][pmName], g_PracticeModels[i][pmModel]));
    }

    new title[64] = "{MAIN}Practice - Target Model";
    format_MainColor(playerid, title);
    format_MainColor(playerid, body);

    return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_MODEL<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, title, body, "Select", "Back");
}

public DIALOG_PRAC_MAIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return 1;
    if (!PracticeValidateWorld(playerid)) return 1;

    switch (listitem)
    {
        case PRAC_ROW_TYPE:
        {
            g_Practice[playerid][pracTargetType] =
                (g_Practice[playerid][pracTargetType] == PRACTICE_TYPE_NPC) ? PRACTICE_TYPE_BALL : PRACTICE_TYPE_NPC;

            CreatePracticeTarget(playerid);
            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_MODEL:
        {
            if (g_Practice[playerid][pracTargetType] == PRACTICE_TYPE_NPC)
            {
                new body[224];
                format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Current skin: "COL_WHITE"%d\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%d to %d",
                    g_Practice[playerid][pracSkin], PRACTICE_SKIN_MIN, PRACTICE_SKIN_MAX);

                return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_SKIN<iiiis>, DIALOG_STYLE_INPUT, "Practice - NPC Skin", body, "Confirm", "Back");
            }
            return ShowPracticeModelDialog(playerid);
        }

        case PRAC_ROW_NPCFIGHT:
        {
            if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC)
                return ShowPracticeDialog(playerid);

            g_Practice[playerid][pracNpcFight] = !g_Practice[playerid][pracNpcFight];
            PracticeNPCApplyCombat(playerid);
            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_NPCWEAPON:
        {
            if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC)
                return ShowPracticeDialog(playerid);

            new body[320];
            strcat(body, "{A29582}Weapon\t{A29582}ID\n");

            for (new i; i < sizeof(g_PracticeWeapons); i++)
            {
                strcat(body, fmat_return("%s{FFFFFF}%s\t{BEBEBE}%d\n",
                    STYLE_MARK(g_Practice[playerid][pracNpcWeapon] == i),
                    g_PracticeWeapons[i][pwName], g_PracticeWeapons[i][pwId]));
            }

            format_MainColor(playerid, body);
            return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_WEAPON<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "Practice - NPC Weapon", body, "Select", "Back");
        }

        case PRAC_ROW_SPEED:
        {
            if (g_Practice[playerid][pracTargetType] == PRACTICE_TYPE_NPC)
            {
                if (PracticeRecordLocks(playerid))
                {
                    SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The recording drives the NPC. Clear it in {MAIN}NPC Record {FFFFFF}to change this.");
                    return ShowPracticeDialog(playerid);
                }

                new sbody[256];
                strcat(sbody, "{A29582}Move Style\t{A29582}Selected\n");

                for (new i; i < sizeof(g_PracticeStyles); i++)
                {
                    strcat(sbody, fmat_return("%s{FFFFFF}%s\t{BEBEBE}%s\n",
                        STYLE_MARK(g_Practice[playerid][pracMoveStyle] == i),
                        g_PracticeStyles[i][psName],
                        (g_Practice[playerid][pracMoveStyle] == i) ? ("Active") : ("-")));
                }

                format_MainColor(playerid, sbody);
                return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_STYLE<iiiis>, DIALOG_STYLE_TABLIST_HEADERS, "Practice - Move Style", sbody, "Select", "Back");
            }

            new body[192];
            format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Current: "COL_WHITE"%.1f\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%.1f to %.1f",
                g_Practice[playerid][pracSpeed], PRACTICE_SPEED_MIN, PRACTICE_SPEED_MAX);

            return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_SPEED<iiiis>, DIALOG_STYLE_INPUT, "Practice - Speed", body, "Confirm", "Back");
        }

        case PRAC_ROW_HEIGHT:
        {
            new body[192];
            format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Current: "COL_WHITE"%.1f\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%.1f to %.1f",
                g_Practice[playerid][pracHeight], PRACTICE_HEIGHT_MIN, PRACTICE_HEIGHT_MAX);

            return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_HEIGHT<iiiis>, DIALOG_STYLE_INPUT, "Practice - Height", body, "Confirm", "Back");
        }

        case PRAC_ROW_TOLERANCE:
        {
            new body[256];
            format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Current: "COL_WHITE"%d ms\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%d to %d\n\n{BEBEBE}Ping compensation when checking hits on the moving ball. Higher is more forgiving. Does not affect the NPC.",
                g_Practice[playerid][pracTolerance], PRACTICE_TOLERANCE_MIN, PRACTICE_TOLERANCE_MAX);

            return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_TOLERANCE<iiiis>, DIALOG_STYLE_INPUT, "Practice - Hit Tolerance", body, "Confirm", "Back");
        }

        case PRAC_ROW_LATENCY:
        {
            if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_BALL)
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Lag simulation only works with the ball target.");
                return ShowPracticeDialog(playerid);
            }

            new body[352];
            format(body, sizeof(body), "{FFFFFF}\x95 {B4B5B7}Current: "COL_WHITE"%d ms\n\n{FFFFFF}\x95 {B4B5B7}Allowed range: "COL_WHITE"%d to %d\n\n{BEBEBE}Simulates a lagging target. The valid position runs ahead of the ball you see, so hitting it on screen no longer counts - you have to lead the shot. Needs Movement on. 0 turns it off.",
                g_Practice[playerid][pracLatency], PRACTICE_LATENCY_MIN, PRACTICE_LATENCY_MAX);

            return Dialog_ShowCallback(playerid, using public DIALOG_PRAC_LATENCY<iiiis>, DIALOG_STYLE_INPUT, "Practice - Lag Simulation", body, "Confirm", "Back");
        }

        case PRAC_ROW_RECORD:
        {
            if (g_Practice[playerid][pracTargetType] != PRACTICE_TYPE_NPC)
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Recording only works with the NPC target.");
                return ShowPracticeDialog(playerid);
            }

            if (g_Practice[playerid][pracRecPending]) return ShowPracticeDialog(playerid);

            if (g_Practice[playerid][pracRecording]) PracticeStopRecording(playerid);
            else if (PracticeHasRecord(playerid)) PracticeClearRecord(playerid);
            else PracticeStartRecording(playerid);

            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_MOVING:
        {
            if (PracticeRecordLocks(playerid))
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The recording drives the NPC. Clear it in {MAIN}NPC Record {FFFFFF}to change this.");
                return ShowPracticeDialog(playerid);
            }

            g_Practice[playerid][pracMoving] = !g_Practice[playerid][pracMoving];

            if (g_Practice[playerid][pracMoving] && !PracticeCanMove(playerid))
                SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Select {MAIN}Position 1 {FFFFFF}and {MAIN}Position 2 {FFFFFF}where you want them to make it move.");

            CreatePracticeTarget(playerid);
            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_POS1, PRAC_ROW_POS2:
        {
            if (PracticeRecordLocks(playerid))
            {
                SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}The recording drives the NPC. Clear it in {MAIN}NPC Record {FFFFFF}to change this.");
                return ShowPracticeDialog(playerid);
            }

            new slot = (listitem == PRAC_ROW_POS1) ? 1 : 2;
            new bool:has = (slot == 1) ? g_Practice[playerid][pracHasPos1] : g_Practice[playerid][pracHasPos2];

            if (has) ClearPracticePoint(playerid, slot);
            else SetPracticePoint(playerid, slot);

            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_STATS:
        {
            g_Practice[playerid][pracHits]  = 0;
            g_Practice[playerid][pracShots] = 0;
            g_Practice[playerid][pracShotId]    = 0;
            g_Practice[playerid][pracHitShotId] = -1;

            SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Accuracy counters reset.");
            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_RESTART:
        {
            CreatePracticeTarget(playerid);
            SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Target respawned with the current settings.");
            return ShowPracticeDialog(playerid);
        }

        case PRAC_ROW_EXIT:
        {
            ExitPractice(playerid);
            SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}You left the practice world.");
            return 1;
        }
    }
    return 1;
}

public DIALOG_PRAC_MODEL(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    if (0 <= listitem < sizeof(g_PracticeModels))
    {
        g_Practice[playerid][pracModel] = listitem;
        CreatePracticeTarget(playerid);
    }
    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_SPEED(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    new Float:value = floatstr(inputtext);

    if (!(PRACTICE_SPEED_MIN <= value <= PRACTICE_SPEED_MAX))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Speed must be between {MAIN}%.1f {FFFFFF}and {MAIN}%.1f{FFFFFF}.",
            PRACTICE_SPEED_MIN, PRACTICE_SPEED_MAX);

        return ShowPracticeDialog(playerid);
    }

    g_Practice[playerid][pracSpeed] = value;
    CreatePracticeTarget(playerid);
    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_TOLERANCE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    if (!IsNumeric(inputtext) || strlen(inputtext) > 4)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Enter a valid number.");
        return ShowPracticeDialog(playerid);
    }

    new value = strval(inputtext);

    if (!(PRACTICE_TOLERANCE_MIN <= value <= PRACTICE_TOLERANCE_MAX))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Hit Tolerance must be between {MAIN}%d {FFFFFF}and {MAIN}%d{FFFFFF}.",
            PRACTICE_TOLERANCE_MIN, PRACTICE_TOLERANCE_MAX);

        return ShowPracticeDialog(playerid);
    }

    g_Practice[playerid][pracTolerance] = value;
    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_LATENCY(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    if (!IsNumeric(inputtext) || strlen(inputtext) > 4)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Enter a valid number.");
        return ShowPracticeDialog(playerid);
    }

    new value = strval(inputtext);

    if (!(PRACTICE_LATENCY_MIN <= value <= PRACTICE_LATENCY_MAX))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Lag simulation must be between {MAIN}%d {FFFFFF}and {MAIN}%d{FFFFFF}.",
            PRACTICE_LATENCY_MIN, PRACTICE_LATENCY_MAX);

        return ShowPracticeDialog(playerid);
    }

    g_Practice[playerid][pracLatency] = value;

    if (value > 0 && !PracticeCanMove(playerid))
        SendClientMessage(playerid, -1, "{MAIN}[PRACTICE] {FFFFFF}Turn Movement on and set both points, otherwise the lag has nothing to offset.");

    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_HEIGHT(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    new Float:value = floatstr(inputtext);

    if (!(PRACTICE_HEIGHT_MIN <= value <= PRACTICE_HEIGHT_MAX))
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Height must be between {MAIN}%.1f {FFFFFF}and {MAIN}%.1f{FFFFFF}.",
            PRACTICE_HEIGHT_MIN, PRACTICE_HEIGHT_MAX);

        return ShowPracticeDialog(playerid);
    }

    g_Practice[playerid][pracHeight] = value;
    CreatePracticeTarget(playerid);
    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_SKIN(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    if (!IsNumeric(inputtext) || strlen(inputtext) > 4)
    {
        SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Enter a valid number.");
        return ShowPracticeDialog(playerid);
    }

    new value = strval(inputtext);

    if (!(PRACTICE_SKIN_MIN <= value <= PRACTICE_SKIN_MAX) || value == BLOCKED_SKIN_ID)
    {
        va_SendClientMessage(playerid, -1, "{MAIN}[ERROR] {FFFFFF}Skin must be between {MAIN}%d {FFFFFF}and {MAIN}%d{FFFFFF}, and not %d.",
            PRACTICE_SKIN_MIN, PRACTICE_SKIN_MAX, BLOCKED_SKIN_ID);

        return ShowPracticeDialog(playerid);
    }

    g_Practice[playerid][pracSkin] = value;
    CreatePracticeTarget(playerid);
    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_STYLE(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    if (0 <= listitem < sizeof(g_PracticeStyles))
    {
        g_Practice[playerid][pracMoveStyle] = listitem;
        CreatePracticeTarget(playerid);
    }
    return ShowPracticeDialog(playerid);
}

public DIALOG_PRAC_WEAPON(playerid, dialogid, response, listitem, string:inputtext[])
{
    if (!response) return ShowPracticeDialog(playerid);
    if (!PracticeValidateWorld(playerid)) return 1;

    if (0 <= listitem < sizeof(g_PracticeWeapons))
    {
        g_Practice[playerid][pracNpcWeapon] = listitem;
        PracticeNPCApplyCombat(playerid);
    }
    return ShowPracticeDialog(playerid);
}

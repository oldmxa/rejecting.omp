#if defined _ac_config_included
    #endinput
#endif
#define _ac_config_included

#define AC_KICK_DELAY (1500)

#define AC_R2_GRACE_UPDATES (10)

#define AC_CHECK_R2_A     (5)
#define AC_CHECK_R2_A_ADR (0x400000)
#define AC_CHECK_R2_A_LEN (4)

#define AC_CHECK_R2_B     (69)
#define AC_CHECK_R2_B_ADR (0x0)
#define AC_CHECK_R2_B_LEN (4)

#define AC_CHECK_DESKTOP     (72)
#define AC_CHECK_DESKTOP_ADR (0x0)
#define AC_CHECK_DESKTOP_LEN (2)

#define AC_ACTION_IGNORE (0)
#define AC_ACTION_REPORT (1)
#define AC_ACTION_KICK   (2)
#define AC_ACTION_GTEMP  (3)
#define AC_ACTION_BAN    (4)

#define AC_MIN_ADMIN_LEVEL (5)

#define AC_BAN_HOURS (9999)

#define AC_DIALOG_KICKED     (14520)
#define AC_SCREEN_CAM_DIST   (2.6)
#define AC_SCREEN_CAM_HEIGHT (0.4)
#define AC_SCREEN_TIME       (7000)

enum
{
    AC_CAT_AIM,
    AC_CAT_MOVEMENT,
    AC_CAT_EXPLOIT,
    AC_CAT_CHEATS,
    AC_CAT_NOPS,
    AC_CAT_CRASHER,

    MAX_AC_CATEGORIES
}

new const AC_CATEGORY_NAME[MAX_AC_CATEGORIES][] =
{
    "Aim list",
    "Movement list",
    "Exploit list",
    "Cheats list",
    "NOPs list",
    "Crasher list"
};

new const AC_ACTION_NAME[][] =
{
    "Ignore",
    "Warn",
    "Kick",
    "Gtemp",
    "Ban"
};

new const AC_ACTION_HELP[][] =
{
    "Detect and record only, no alert",
    "Alert admins and log to Discord",
    "Alert and kick the player",
    "5 minutes to record the screen, then ban",
    "Alert and ban permanently"
};

new const AC_ACTION_COLOR[][] =
{
    "{BEBEBE}",
    "{DEAD10}",
    "{FFA500}",
    "{f1802d}",
    "{F84F31}"
};

#define AC_DAMAGER            (15)
#define AC_LOW_FOV_SILENT     (16)
#define AC_HIGH_FOV_SILENT    (17)
#define AC_AIMBOT             (18)
#define AC_AUTO_CBUG          (19)
#define AC_RAPID_FIRE         (20)
#define AC_EXTRA_WS           (21)
#define AC_AIRBREAK           (25)
#define AC_MAP_RUN            (26)
#define AC_CLICK_WARP         (27)
#define AC_NO_FALL            (24)
#define AC_INVALID_AIMZ       (31)
#define AC_PIZDA_WALK         (22)
#define AC_ANTI_STUN          (28)
#define AC_UPSIDE_DOWN        (32)
#define AC_INVISIBLE_UG       (34)
#define AC_ANTI_AIM           (29)
#define AC_INVISIBLE_SURF     (33)
#define AC_NOP_CONTROLLABLE   (35)
#define AC_DIALOG_CRASHER     (38)
#define AC_SEAT_CRASHER       (39)
#define AC_TUNING_CRASHER     (40)
#define AC_WEAPON_CRASHER     (41)
#define AC_CALLBACK_FLOOD     (42)
#define AC_NOP_INTERIOR       (43)
#define AC_NOP_WEAPON         (44)
#define AC_NOP_POS            (45)

#define E_REJECTING_AC_UNKNOWN         (-1)
#define E_REJECTING_AC_S0BEIT          (0)
#define E_REJECTING_AC_SAMPFUNCS       (1)
#define E_REJECTING_AC_CLEO            (2)
#define E_REJECTING_AC_IMPROVEDDEAGLE  (3)
#define E_REJECTING_AC_SPRINTHOOK      (4)
#define E_REJECTING_AC_1337            (5)
#define E_REJECTING_AC_ARCANE          (6)
#define E_REJECTING_AC_STEALTH         (7)
#define E_REJECTING_AC_EXTREMECHEATS   (8)
#define E_REJECTING_AC_OTHER_CHEATS    (9)
#define E_REJECTING_AC_BYPASS          (10)
#define E_REJECTING_AC_FAKE_FPS        (11)
#define E_REJECTING_AC_VENOM           (12)
#define E_REJECTING_AC_PARKOUR_MOD     (13)
#define E_REJECTING_AC_FAKE_VERSION    (14)

forward OnRejectingACDetection(playerid, code);

enum e_AC_DETECTION
{
    acdName[26],
    acdHelp[50],
    acdCategory,
    bool:acdActive
}

new const AC_DETECTION[][e_AC_DETECTION] =
{
    {"s0beit/ENB",      "s0beit cheat menu in memory",             AC_CAT_CHEATS,   true},
    {"SAMPFUNCS",       "SAMPFUNCS loader (tool, not proof)",      AC_CAT_CHEATS,   true},
    {"CLEO",            "CLEO library loaded (tool, not proof)",   AC_CAT_CHEATS,   true},
    {"ImprovedDeagle",  "Deagle fire rate modification",           AC_CAT_AIM,      true},
    {"SprintHook",      "Sprint key hook, unlimited sprint",       AC_CAT_MOVEMENT, true},
    {"1337",            "1337 cheat client",                       AC_CAT_CHEATS,   true},
    {"Arcane",          "Arcane cheat client",                     AC_CAT_CHEATS,   true},
    {"Stealth",         "Stealth cheat client",                    AC_CAT_CHEATS,   true},
    {"ExtremeCheats",   "ExtremeCheats client",                    AC_CAT_CHEATS,   true},
    {"Trash Cheats",    "Unrecognized injection at a known spot",  AC_CAT_CHEATS,   true},
    {"Bypass",          "Attempt to fool the memory probe",        AC_CAT_EXPLOIT,  true},
    {"Fake FPS",        "Forged FPS counter",                      AC_CAT_EXPLOIT,  true},
    {"VP Cheat",        "VP cheat client",                         AC_CAT_CHEATS,   true},
    {"Parkour Mod",     "Impossible body angle on foot",           AC_CAT_MOVEMENT, true},

    {"Fake version",    "Client reports a version it cannot back", AC_CAT_EXPLOIT,  true},

    {"Damager",         "Damage above what the weapon can deal",   AC_CAT_AIM,      true},
    {"Low FOV silent",  "Hits off camera by a narrow angle",       AC_CAT_AIM,      true},
    {"High FOV silent", "Hits off camera by a wide angle",         AC_CAT_AIM,      true},
    {"Aimbot",          "Hit claimed where the target is not",     AC_CAT_AIM,      true},
    {"Auto C-bug",      "Reload cancel done by script",            AC_CAT_AIM,      true},
    {"Rapid fire",      "Shots faster than the weapon allows",     AC_CAT_AIM,      true},
    {"Extra WS",        "More weapon shots than fired rounds",     AC_CAT_AIM,      true},

    {"Pizda walk",      "Walk animation swapped to hide the aim",  AC_CAT_MOVEMENT, false},
    {"",                "",                                        AC_CAT_MOVEMENT, false},
    {"No fall",         "Long fall with no damage taken",          AC_CAT_MOVEMENT, true},
    {"Airbreak",        "Free movement with no ground contact",    AC_CAT_MOVEMENT, true},
    {"Map run",         "Ground speed above the animation limit",  AC_CAT_MOVEMENT, true},
    {"Click warp",      "Position jump with no server teleport",   AC_CAT_MOVEMENT, true},

    {"Anti stun",       "Ignores the stagger from being hit",      AC_CAT_EXPLOIT,  true},
    {"Anti aim",        "Body angle forged against the aim",       AC_CAT_EXPLOIT,  true},
    {"Bullet proof",    "Client refuses the health the server set", AC_CAT_EXPLOIT,  false},
    {"Invalid aim Z",   "Forged or unnormalized aim vector",       AC_CAT_EXPLOIT,  true},
    {"Upside down",     "Inverted body orientation on foot",       AC_CAT_EXPLOIT,  true},
    {"Invisible surf",  "Surfing an object that is not rendered",  AC_CAT_EXPLOIT,  true},
    {"Invisible under", "Standing below the world geometry",       AC_CAT_EXPLOIT,  true},

    {"NOP Controllable","Keeps moving while the server froze him",  AC_CAT_NOPS,     true},
    {"NOP Animation",   "Blocks SetPlayerAnimation",               AC_CAT_NOPS,     false},
    {"NOP EngineState", "Blocks SetVehicleEngineState",            AC_CAT_NOPS,     false},

    {"Dialog crasher",  "Dialog answer with a forged field",       AC_CAT_CRASHER,  true},
    {"Seat crasher",    "Claims a seat the vehicle does not have", AC_CAT_CRASHER,  true},
    {"Tuning crasher",  "Invalid component, paintjob or colour",   AC_CAT_CRASHER,  true},
    {"Weapon crasher",  "Shot packet with an impossible field",    AC_CAT_CRASHER,  true},
    {"Callback flood",  "Client side callback called too often",   AC_CAT_CRASHER,  true},

    {"NOP Interior",    "Blocks SetPlayerInterior",                AC_CAT_NOPS,     true},
    {"NOP GiveWeapon",  "Blocks GivePlayerWeapon",                 AC_CAT_NOPS,     true},
    {"NOP SetPos",      "Blocks SetPlayerPos",                     AC_CAT_NOPS,     true}
};

#define MAX_AC_DETECTIONS (sizeof(AC_DETECTION))

new AC_DETECTION_ACTION[MAX_AC_DETECTIONS] = {AC_ACTION_REPORT, ...};

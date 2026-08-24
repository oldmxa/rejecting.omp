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

#define AC_TAG_NORM COL_ADM_CMD"[RAC] "
#define AC_TAG_PUN  "{3CCFFA}[RAC]"

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
    AC_CAT_MEMORY,
    AC_CAT_FILE,
    AC_CAT_MACRO,
    AC_CAT_NOPS,
    AC_CAT_CRASHER,

    MAX_AC_CATEGORIES
}

new const AC_CATEGORY_NAME[MAX_AC_CATEGORIES][] =
{
    "Aim list",
    "Movement list",
    "Exploit list",
    "Memory list",
    "File list",
    "Macro list",
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
    "{B4B5B7}",
    "{DEAD10}",
    "{FFA500}",
    "{f1802d}",
    "{FF0000}"
};

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
#define E_REJECTING_AC_MODLOADER       (15)
#define E_REJECTING_AC_RUN_EVERYWHERE  (16)
#define E_REJECTING_AC_CRASHERS_ASI    (17)
#define E_REJECTING_AC_SAMP_GRASS      (18)

#define AC_DAMAGER            (19)
#define AC_LOW_FOV_SILENT     (20)
#define AC_HIGH_FOV_SILENT    (21)
#define AC_AIMBOT             (22)
#define AC_AUTO_CBUG          (23)
#define AC_RAPID_FIRE         (24)
#define AC_EXTRA_WS           (25)
#define AC_PIZDA_WALK         (26)
#define AC_NULL_VELOCITY      (27)
#define AC_NO_FALL            (28)
#define AC_AIRBREAK           (29)
#define AC_MAP_RUN            (30)
#define AC_CLICK_WARP         (31)
#define AC_ANTI_STUN          (32)
#define AC_ANTI_AIM           (33)
#define AC_BULLET_PROOF       (34)
#define AC_INVALID_AIMZ       (35)
#define AC_UPSIDE_DOWN        (36)
#define AC_INVISIBLE_SURF     (37)
#define AC_INVISIBLE_UG       (38)
#define AC_NOP_CONTROLLABLE   (39)
#define AC_NOP_ANIMATION      (40)
#define AC_NOP_ENGINE_STATE   (41)
#define AC_DIALOG_CRASHER     (42)
#define AC_SEAT_CRASHER       (43)
#define AC_TUNING_CRASHER     (44)
#define AC_WEAPON_CRASHER     (45)
#define AC_CALLBACK_FLOOD     (46)
#define AC_NOP_INTERIOR       (47)
#define AC_NOP_WEAPON         (48)
#define AC_NOP_POS            (49)
#define AC_CAR_AIRBREAK       (50)
#define AC_CAR_SPEED          (51)
#define AC_DEATH_FLOOD        (52)
#define AC_ARMOUR_PROOF       (53)
#define AC_MEGA_JUMP          (54)
#define AC_WEAPON_HACK        (55)
#define AC_CAR_SLAPPER        (56)
#define AC_INFINITE_AMMO      (57)

forward OnRejectingACDetection(playerid, code);

enum e_AC_DETECTION
{
    acdName[32],
    acdHelp[64],
    acdCategory,
    bool:acdActive
}

new const AC_DETECTION[][e_AC_DETECTION] =
{
    {"d3d9.dll (s0beit/ENB Series)", "s0beit / ENB modification in memory",      AC_CAT_FILE,     true},
    {"sampfuncs.asi",                "SAMPFUNCS loader in memory",              AC_CAT_FILE,     true},
    {"cleo",                         "CLEO library in memory",                  AC_CAT_FILE,     true},
    {"crouchhook.asi",               "Crouch hook modification in memory",      AC_CAT_FILE,     true},
    {"Sprint macro",                 "Sprint key hook / unlimited sprint",      AC_CAT_MACRO,    true},
    {"1337",                         "1337 cheat client in memory",             AC_CAT_MEMORY,   true},
    {"Arcane",                       "Arcane cheat client in memory",           AC_CAT_MEMORY,   true},
    {"Stealth",                      "Stealth cheat client in memory",          AC_CAT_MEMORY,   true},
    {"ExtremeCheats",                "ExtremeCheats client in memory",          AC_CAT_MEMORY,   true},
    {"External Cheats",              "External cheat injection in memory",      AC_CAT_MEMORY,   true},
    {"Bypass",                       "Attempt to fool the memory probe",        AC_CAT_EXPLOIT,  true},
    {"Fake FPS",                     "Forged FPS counter",                      AC_CAT_EXPLOIT,  true},
    {"VP Cheat",                     "VP cheat client in memory",               AC_CAT_MEMORY,   true},
    {"Parkour Mod",                  "Impossible body angle on foot",           AC_CAT_MOVEMENT, true},
    {"Fake version",                 "Client reports a version it cannot back", AC_CAT_EXPLOIT,  true},
    {"modloader",                    "Modloader library in memory",             AC_CAT_FILE,     true},
    {"runeverywhere.asi",            "Run everywhere modification in memory",   AC_CAT_FILE,     true},
    {"crashers.asi",                 "Crasher modification in memory",          AC_CAT_FILE,     true},
    {"samp_grass.asi",               "Grass removal modification in memory",    AC_CAT_FILE,     true},

    {"Damager",                      "Damage above what the weapon can deal",   AC_CAT_AIM,      true},
    {"Low FOV silent",               "Hits off camera by a narrow angle",       AC_CAT_AIM,      true},
    {"High FOV silent",              "Hits off camera by a wide angle",         AC_CAT_AIM,      true},
    {"Aimbot",                       "Hit claimed where the target is not",     AC_CAT_AIM,      true},
    {"Switch +C",                    "Reload cancel done by script / macro",    AC_CAT_MACRO,    true},
    {"Rapid fire",                   "Shots faster than the weapon allows",     AC_CAT_MACRO,    true},
    {"Extra WS",                     "More weapon shots than fired rounds",     AC_CAT_AIM,      true},

    {"Pizda walk",                   "Walk animation swapped to hide the aim",  AC_CAT_MOVEMENT, true},
    {"Null velocity",                "Moves without reporting any speed",       AC_CAT_MOVEMENT, true},
    {"No fall",                      "Long fall with no damage taken",          AC_CAT_MOVEMENT, true},
    {"Airbreak",                     "Free movement with no ground contact",    AC_CAT_MOVEMENT, true},
    {"Map run",                      "Ground speed above the animation limit",  AC_CAT_MOVEMENT, true},
    {"Click warp",                   "Position jump with no server teleport",   AC_CAT_MOVEMENT, true},

    {"Anti stun",                    "Ignores the stagger from being hit",      AC_CAT_EXPLOIT,  true},
    {"Anti aim",                     "Body angle forged against the aim",       AC_CAT_EXPLOIT,  true},
    {"Bullet proof",                 "Client refuses the health the server set", AC_CAT_EXPLOIT, true},
    {"Invalid aim Z",                "Forged or unnormalized aim vector",       AC_CAT_EXPLOIT,  true},
    {"Upside down",                  "Inverted body orientation on foot",       AC_CAT_EXPLOIT,  true},
    {"Invisible surf",               "Surfing an object that is not rendered",  AC_CAT_EXPLOIT,  true},
    {"Invisible under",              "Standing below the world geometry",       AC_CAT_EXPLOIT,  true},

    {"NOP Controllable",             "Keeps moving while the server froze him",  AC_CAT_NOPS,     true},
    {"NOP Animation",                "Blocks SetPlayerAnimation",               AC_CAT_NOPS,     true},
    {"NOP EngineState",              "Blocks SetVehicleEngineState",            AC_CAT_NOPS,     true},

    {"Dialog crasher",               "Dialog answer with a forged field",       AC_CAT_CRASHER,  true},
    {"Seat crasher",                 "Claims a seat the vehicle does not have", AC_CAT_CRASHER,  true},
    {"Tuning crasher",               "Invalid component, paintjob or colour",   AC_CAT_CRASHER,  true},
    {"Weapon crasher",               "Shot packet with an impossible field",    AC_CAT_CRASHER,  true},
    {"Callback flood",               "Client side callback called too often",   AC_CAT_CRASHER,  true},

    {"NOP Interior",                 "Blocks SetPlayerInterior",                AC_CAT_NOPS,     true},
    {"NOP GiveWeapon",               "Blocks GivePlayerWeapon",                 AC_CAT_NOPS,     true},
    {"NOP SetPos",                   "Blocks SetPlayerPos",                     AC_CAT_NOPS,     true},
    {"Car Airbreak",                 "Airbreak or flying in a vehicle",         AC_CAT_MOVEMENT, true},
    {"Car Speed",                    "Vehicle speed above the model limit",      AC_CAT_MOVEMENT, true},
    {"Death Flood",                  "Sending deaths too fast (fake kill)",      AC_CAT_CRASHER,  true},
    {"Armour proof",                 "Client refuses the armour the server set", AC_CAT_EXPLOIT,  true},
    {"Mega jump",                    "Abnormal upward jump velocity",           AC_CAT_MOVEMENT, true},
    {"Weapon hack",                  "Spawning unauthorized weapons",           AC_CAT_MEMORY,   true},
    {"Car slapper",                  "Moving unoccupied vehicles remotely",     AC_CAT_CRASHER,  true},
    {"Infinite ammo",                "Ammo not decreasing after firing",        AC_CAT_AIM,      true}
};

#define MAX_AC_DETECTIONS (sizeof(AC_DETECTION))

new AC_DETECTION_ACTION[MAX_AC_DETECTIONS] = {AC_ACTION_REPORT, ...};

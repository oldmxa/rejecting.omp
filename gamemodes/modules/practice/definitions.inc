#if defined _practice_definitions_incl
    #endinput
#endif
#define _practice_definitions_incl

#define PRACTICE_MIN_LEVEL          (5)
#define PRACTICE_VW_BASE            (80000)

#define PRACTICE_TARGET_HP          (20000.0)
#define PRACTICE_HIT_SOUND          (17802)

#define PRACTICE_SPEED_MIN          (1.0)
#define PRACTICE_SPEED_MAX          (30.0)
#define PRACTICE_SPEED_DEFAULT      (8.5)

#define PRACTICE_HEIGHT_MIN         (-2.0)
#define PRACTICE_HEIGHT_MAX         (10.0)
#define PRACTICE_HEIGHT_DEFAULT     (0.0)

#define PRACTICE_TOLERANCE_MIN        (0)
#define PRACTICE_TOLERANCE_MAX        (500)
#define PRACTICE_TOLERANCE_DEFAULT    (300)

#define PRACTICE_TYPE_BALL          (0)
#define PRACTICE_TYPE_NPC           (1)

#define PRACTICE_SKIN_MIN           (0)
#define PRACTICE_SKIN_MAX           (311)
#define PRACTICE_SKIN_DEFAULT       (29)

#define PRACTICE_NPC_HP             (100.0)
#define PRACTICE_BALL_LIFT          (0.6)
#define PRACTICE_NPC_SHOOT_DELAY    (900)
#define PRACTICE_NPC_ACCURACY       (0.35)

#define PRACTICE_RESPAWN_DELAY      (400)
#define PRACTICE_RADIUS_SCALE       (1.05)
#define PRACTICE_RADIUS_MIN         (0.35)
#define PRACTICE_RADIUS_FALL        (0.60)

#define PRACTICE_DMG_DEFAULT        (10.0)
#define PRACTICE_DMG_DEAGLE         (35.0)
#define PRACTICE_DMG_SHOTGUN        (15.0)
#define PRACTICE_DMG_M4             (10.0)
#define PRACTICE_DMG_SNIPER         (48.0)

enum E_PRACTICE_MODEL
{
    pmModel,
    pmName[20]
};

new const g_PracticeModels[][E_PRACTICE_MODEL] =
{
    {19059, "Red Orb"},
    {19060, "Blue Orb"},
    {19061, "Green Orb"},
    {19062, "Yellow Orb"},
    {19063, "Purple Orb"},
    {1240,  "Health Icon"},
    {1242,  "Armour Icon"},
    {18693, "Beach Ball"}
};

enum E_PRACTICE_STYLE
{
    psType,
    psName[10]
};

new const g_PracticeStyles[][E_PRACTICE_STYLE] =
{
    {_:NPC_MOVE_TYPE_WALK,   "Walk"},
    {_:NPC_MOVE_TYPE_JOG,    "Jog"},
    {_:NPC_MOVE_TYPE_SPRINT, "Sprint"}
};

enum E_PRACTICE_WEAPON
{
    pwId,
    pwName[14]
};

new const g_PracticeWeapons[][E_PRACTICE_WEAPON] =
{
    {_:WEAPON_DEAGLE,  "Deagle"},
    {_:WEAPON_SHOTGUN, "Shotgun"},
    {_:WEAPON_M4,      "M4"},
    {_:WEAPON_AK47,    "AK-47"},
    {_:WEAPON_MP5,     "MP5"},
    {_:WEAPON_SNIPER,  "Sniper"}
};

enum E_PRACTICE
{
    bool:pracActive,
    pracTargetType,
    pracSkin,
    pracMoveStyle,
    bool:pracNpcFight,
    pracNpcWeapon,
    pracNpcId,
    bool:pracRecording,
    bool:pracRecPending,
    pracRecordId,
    pracModel,
    Float:pracSpeed,
    Float:pracHeight,
    pracTolerance,
    bool:pracMoving,

    bool:pracHasPos1,
    Float:pracPos1[3],
    bool:pracHasPos2,
    Float:pracPos2[3],

    pracObject,
    Float:pracHealth,
    pracWaypoint,
    pracMoveTimer,

    pracHits,
    pracShots
};

new g_Practice[MAX_PLAYERS][E_PRACTICE];

new g_PracticeNPCOwner[MAX_NPCS] = {INVALID_PLAYER_ID, ...};
new g_PracticeNameSeq[MAX_PLAYERS];

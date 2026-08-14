#if defined _training_skinhit_lv_incl
    #endinput
#endif
#define _training_skinhit_lv_incl

// Arena: Skinhit Aero LV

#define SKINHIT_LV_NAME           "Airport LV"
#define SKINHIT_LV_TYPE           "Double hit"

// Player Spawn Point
#define SKINHIT_LV_SPAWN_X        (1384.5424)
#define SKINHIT_LV_SPAWN_Y        (1654.1525)
#define SKINHIT_LV_SPAWN_Z        (10.8125)
#define SKINHIT_LV_SPAWN_A        (95.2027)

// Target Ball Details
#define SKINHIT_LV_TARGET_MODEL   (19059) // XmasOrb1
#define SKINHIT_LV_SPEED          (8.5)

// Waypoints exact trajectory matching ramp & container elevation
static const Float:g_SkinhitLVWaypoints[6][3] = {
    { 1348.5349, 1671.9773, 10.8203 },
    { 1349.0098, 1665.1542, 10.9240 },
    { 1349.0933, 1656.3779, 14.8727 },
    { 1349.2059, 1644.9747, 14.8729 },
    { 1349.2659, 1634.8771, 10.8203 },
    { 1349.2454, 1631.2040, 10.8203 } 
};

forward Init_SkinhitLV();
public Init_SkinhitLV()
{
    new weapons[2] = { 24, 25 }; // Desert Eagle & Shotgun only

    new arenaid = RegisterTrainingArena(
        SKINHIT_LV_NAME,
        SKINHIT_LV_TYPE,
        SKINHIT_LV_SPAWN_X, SKINHIT_LV_SPAWN_Y, SKINHIT_LV_SPAWN_Z, SKINHIT_LV_SPAWN_A,
        g_SkinhitLVWaypoints[0][0], g_SkinhitLVWaypoints[0][1], g_SkinhitLVWaypoints[0][2], 180.6906,
        g_SkinhitLVWaypoints[5][0], g_SkinhitLVWaypoints[5][1], g_SkinhitLVWaypoints[5][2], 359.5006,
        SKINHIT_LV_SPEED,
        SKINHIT_LV_TARGET_MODEL,
        weapons,
        2
    );

    SetTrainingArenaWaypoints(arenaid, g_SkinhitLVWaypoints, sizeof(g_SkinhitLVWaypoints));
    return 1;
}

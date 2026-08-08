#if defined _training_skinhit_ls_included
    #endinput
#endif
#define _training_skinhit_ls_included


// Arena: Skinhit Aero LS

#define SKINHIT_LS_NAME           "Airport LS"
#define SKINHIT_LS_TYPE           "Double hit"

// Player Spawn Point
#define SKINHIT_LS_SPAWN_X        (1995.6570)
#define SKINHIT_LS_SPAWN_Y        (-2214.3516)
#define SKINHIT_LS_SPAWN_Z        (13.5469)
#define SKINHIT_LS_SPAWN_A        (92.1554)

// Target Ball Details
#define SKINHIT_LS_TARGET_MODEL   (19059) // XmasOrb1
#define SKINHIT_LS_SPEED          (8.5)

// Waypoints exact trajectory matching ramp & container elevation
static const Float:g_SkinhitLSWaypoints[6][3] = {
    { 1966.4843, -2201.9241, 13.5469 }, // 0: start of the course on the ground
    { 1966.6227, -2207.0178, 13.5469 }, // 1: start of the ramp (preparing for the ascent)
    { 1966.6162, -2212.4304, 16.1250 }, // 2: top of the container
    { 1966.6173, -2220.1470, 16.1250 }, // 3: end of the container / start of the descent ramp
    { 1966.6732, -2225.1858, 13.5469 }, // 4: end of the ramp on the ground
    { 1966.4751, -2226.8909, 13.5469 }  // 5: end of the course on the ground (return)
};

forward Init_SkinhitLS();
public Init_SkinhitLS()
{
    new weapons[2] = { 24, 25 }; // Desert Eagle & Shotgun only

    new arenaid = RegisterTrainingArena(
        SKINHIT_LS_NAME,
        SKINHIT_LS_TYPE,
        SKINHIT_LS_SPAWN_X, SKINHIT_LS_SPAWN_Y, SKINHIT_LS_SPAWN_Z, SKINHIT_LS_SPAWN_A,
        g_SkinhitLSWaypoints[0][0], g_SkinhitLSWaypoints[0][1], g_SkinhitLSWaypoints[0][2], 179.8485,
        g_SkinhitLSWaypoints[5][0], g_SkinhitLSWaypoints[5][1], g_SkinhitLSWaypoints[5][2], 359.4529,
        SKINHIT_LS_SPEED,
        SKINHIT_LS_TARGET_MODEL,
        weapons,
        2
    );

    SetTrainingArenaWaypoints(arenaid, g_SkinhitLSWaypoints, sizeof(g_SkinhitLSWaypoints));
    return 1;
}

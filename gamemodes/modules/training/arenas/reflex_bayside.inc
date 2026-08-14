#if defined _training_reflex_bayside_incl
    #endinput
#endif
#define _training_reflex_bayside_incl

// Arena: Reflex Bayside (M4 Reflex Arena)

#define REFLEX_BAYSIDE_NAME           "Reflex Bayside"
#define REFLEX_BAYSIDE_TYPE           "Reflex M4"

// Player Spawn Point
#define REFLEX_BAYSIDE_SPAWN_X        (-2256.7156)
#define REFLEX_BAYSIDE_SPAWN_Y        (2394.7266)
#define REFLEX_BAYSIDE_SPAWN_Z        (5.1050)
#define REFLEX_BAYSIDE_SPAWN_A        (316.3068)

// Target Ball Details (Random Spawning)
#define REFLEX_BAYSIDE_TARGET_MODEL   (19059)
#define REFLEX_BAYSIDE_SPEED          (1.0)
#define REFLEX_BAYSIDE_TARGET_HP      (10.0)

// Target random spawn coordinates
static const Float:g_ReflexBaysideWaypoints[4][3] = {
    { -2233.6565, 2402.7510, 2.4899 },
    { -2251.4111, 2419.4490, 2.4939 },
    { -2230.1594, 2439.1384, 2.4930 },
    { -2213.9243, 2422.9924, 2.4964 } 
};

forward Init_ReflexBayside();
public Init_ReflexBayside()
{
    new weapons[1] = { 31 }; // M4 only

    new arenaid = RegisterTrainingArena(
        REFLEX_BAYSIDE_NAME,
        REFLEX_BAYSIDE_TYPE,
        REFLEX_BAYSIDE_SPAWN_X, REFLEX_BAYSIDE_SPAWN_Y, REFLEX_BAYSIDE_SPAWN_Z, REFLEX_BAYSIDE_SPAWN_A,
        g_ReflexBaysideWaypoints[0][0], g_ReflexBaysideWaypoints[0][1], g_ReflexBaysideWaypoints[0][2], 0.0,
        g_ReflexBaysideWaypoints[3][0], g_ReflexBaysideWaypoints[3][1], g_ReflexBaysideWaypoints[3][2], 0.0,
        REFLEX_BAYSIDE_SPEED,
        REFLEX_BAYSIDE_TARGET_MODEL,
        weapons,
        1,
        REFLEX_BAYSIDE_TARGET_HP
    );

    SetTrainingArenaWaypoints(arenaid, g_ReflexBaysideWaypoints, sizeof(g_ReflexBaysideWaypoints));
    SetTrainingArenaRandomSpawning(arenaid, true);
    return 1;
}

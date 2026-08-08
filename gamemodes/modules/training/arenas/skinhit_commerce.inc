#if defined _training_skinhit_cmmrc_incl
    #endinput
#endif
#define _training_skinhit_cmmrc_incl

// Arena: Skinhit Commerce LS

#define SKINHIT_COMMERCE_NAME           "Skinhit Commerce"
#define SKINHIT_COMMERCE_TYPE           "Skinhit"

// Player Spawn Point
#define SKINHIT_COMMERCE_SPAWN_X        (1350.2057)
#define SKINHIT_COMMERCE_SPAWN_Y        (-1652.7360)
#define SKINHIT_COMMERCE_SPAWN_Z        (17.8166)
#define SKINHIT_COMMERCE_SPAWN_A        (247.1147)

// Target Ball Details
#define SKINHIT_COMMERCE_TARGET_MODEL   (19060)
#define SKINHIT_COMMERCE_SPEED          (12.0)

// Waypoint Start
#define SKINHIT_COMMERCE_START_X        (1386.3862)
#define SKINHIT_COMMERCE_START_Y        (-1654.4351)
#define SKINHIT_COMMERCE_START_Z        (18.5469)
#define SKINHIT_COMMERCE_START_A        (179.5187)

// Waypoint End
#define SKINHIT_COMMERCE_END_X          (1386.1643)
#define SKINHIT_COMMERCE_END_Y          (-1695.0562)
#define SKINHIT_COMMERCE_END_Z          (18.5469)
#define SKINHIT_COMMERCE_END_A          (358.2377)

forward Init_SkinhitCommerce();
public Init_SkinhitCommerce()
{
    new weapons[2] = { 24, 25 }; // Desert Eagle & Shotgun only

    RegisterTrainingArena(
        SKINHIT_COMMERCE_NAME,
        SKINHIT_COMMERCE_TYPE,
        SKINHIT_COMMERCE_SPAWN_X, SKINHIT_COMMERCE_SPAWN_Y, SKINHIT_COMMERCE_SPAWN_Z, SKINHIT_COMMERCE_SPAWN_A,
        SKINHIT_COMMERCE_START_X, SKINHIT_COMMERCE_START_Y, SKINHIT_COMMERCE_START_Z, SKINHIT_COMMERCE_START_A,
        SKINHIT_COMMERCE_END_X, SKINHIT_COMMERCE_END_Y, SKINHIT_COMMERCE_END_Z, SKINHIT_COMMERCE_END_A,
        SKINHIT_COMMERCE_SPEED,
        SKINHIT_COMMERCE_TARGET_MODEL,
        weapons,
        2
    );
    return 1;
}

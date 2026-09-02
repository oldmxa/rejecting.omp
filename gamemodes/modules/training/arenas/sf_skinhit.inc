#if defined _training_skinhit_sf_included
    #endinput
#endif
#define _training_skinhit_sf_included

#define SKINHIT_SF_NAME           "Skinhit SF"
#define SKINHIT_SF_TYPE           "Skinhit"

// Player Spawn Point
#define SKINHIT_SF_SPAWN_X        (-1921.8234)
#define SKINHIT_SF_SPAWN_Y        (957.6293)
#define SKINHIT_SF_SPAWN_Z        (45.8125)
#define SKINHIT_SF_SPAWN_A        (266.2424)

// Target Ball Details
#define SKINHIT_SF_TARGET_MODEL   (19063)
#define SKINHIT_SF_SPEED          (12.0)

// Waypoint Start (Initial Position - Lowered Height)
#define SKINHIT_SF_START_X        (-1881.5663)
#define SKINHIT_SF_START_Y        (944.3248)
#define SKINHIT_SF_START_Z        (46.4500)
#define SKINHIT_SF_START_A        (357.4323)

// Waypoint End (Final Position - Lowered Height)
#define SKINHIT_SF_END_X          (-1881.7721)
#define SKINHIT_SF_END_Y          (983.3173)
#define SKINHIT_SF_END_Z          (46.4500)
#define SKINHIT_SF_END_A          (181.3354)

forward Init_SkinhitSF();
public Init_SkinhitSF()
{
    new weapons[2] = { 24, 25 }; // Desert Eagle & Shotgun only

    RegisterTrainingArena(
        SKINHIT_SF_NAME,
        SKINHIT_SF_TYPE,
        SKINHIT_SF_SPAWN_X, SKINHIT_SF_SPAWN_Y, SKINHIT_SF_SPAWN_Z, SKINHIT_SF_SPAWN_A,
        SKINHIT_SF_START_X, SKINHIT_SF_START_Y, SKINHIT_SF_START_Z, SKINHIT_SF_START_A,
        SKINHIT_SF_END_X, SKINHIT_SF_END_Y, SKINHIT_SF_END_Z, SKINHIT_SF_END_A,
        SKINHIT_SF_SPEED,
        SKINHIT_SF_TARGET_MODEL,
        weapons,
        2
    );
    return 1;
}

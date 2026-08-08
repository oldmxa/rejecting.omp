#if defined _training_skinhit_marina_incl
    #endinput
#endif
#define _training_skinhit_marina_incl

// Arena: Skinhit Marina LS

#define SKINHIT_MARINA_NAME           "Double hit marina"
#define SKINHIT_MARINA_TYPE           "Double hit"

// Player Spawn Point
#define SKINHIT_MARINA_SPAWN_X        (749.7359)
#define SKINHIT_MARINA_SPAWN_Y        (-1698.4259)
#define SKINHIT_MARINA_SPAWN_Z        (4.7299)
#define SKINHIT_MARINA_SPAWN_A        (91.8870)

// Target Ball Details
#define SKINHIT_MARINA_TARGET_MODEL   (19059) // XmasOrb1
#define SKINHIT_MARINA_SPEED          (12.0)

// Waypoint Start
#define SKINHIT_MARINA_START_X        (715.4827)
#define SKINHIT_MARINA_START_Y        (-1690.4674)
#define SKINHIT_MARINA_START_Z        (2.4297)
#define SKINHIT_MARINA_START_A        (177.4143)

// Waypoint End
#define SKINHIT_MARINA_END_X          (715.4580)
#define SKINHIT_MARINA_END_Y          (-1706.5165)
#define SKINHIT_MARINA_END_Z          (2.4297)
#define SKINHIT_MARINA_END_A          (357.6399)

forward Init_SkinhitMarina();
public Init_SkinhitMarina()
{
    new weapons[2] = { 24, 25 }; // Desert Eagle & Shotgun only

    RegisterTrainingArena(
        SKINHIT_MARINA_NAME,
        SKINHIT_MARINA_TYPE,
        SKINHIT_MARINA_SPAWN_X, SKINHIT_MARINA_SPAWN_Y, SKINHIT_MARINA_SPAWN_Z, SKINHIT_MARINA_SPAWN_A,
        SKINHIT_MARINA_START_X, SKINHIT_MARINA_START_Y, SKINHIT_MARINA_START_Z, SKINHIT_MARINA_START_A,
        SKINHIT_MARINA_END_X, SKINHIT_MARINA_END_Y, SKINHIT_MARINA_END_Z, SKINHIT_MARINA_END_A,
        SKINHIT_MARINA_SPEED,
        SKINHIT_MARINA_TARGET_MODEL,
        weapons,
        2
    );
    return 1;
}

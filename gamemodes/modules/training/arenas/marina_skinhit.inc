#if defined _training_skinhit_marina_incl
    #endinput
#endif
#define _training_skinhit_marina_incl

#define SKINHIT_MARINA_NAME           "Skinhit Marina"
#define SKINHIT_MARINA_TYPE           "Double hit"
#define SKINHIT_MARINA_SPAWN_X        (749.7359)
#define SKINHIT_MARINA_SPAWN_Y        (-1698.4259)
#define SKINHIT_MARINA_SPAWN_Z        (4.7299)
#define SKINHIT_MARINA_SPAWN_A        (91.8870)
#define SKINHIT_MARINA_TARGET_MODEL   (19059) // XmasOrb1
#define SKINHIT_MARINA_SPEED          (12.0)
#define SKINHIT_MARINA_START_X        (715.4827)
#define SKINHIT_MARINA_START_Y        (-1690.4674)
#define SKINHIT_MARINA_START_Z        (2.4297)
#define SKINHIT_MARINA_START_A        (177.4143)
#define SKINHIT_MARINA_END_X          (715.4580)
#define SKINHIT_MARINA_END_Y          (-1706.5165)
#define SKINHIT_MARINA_END_Z          (2.4297)
#define SKINHIT_MARINA_END_A          (357.6399)

forward Init_SkinhitMarina();
public Init_SkinhitMarina()
{
    new weapons[2] = { 24, 25 };

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

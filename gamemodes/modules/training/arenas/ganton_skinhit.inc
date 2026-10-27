#if defined _training_skinhit_ganton_incl
    #endinput
#endif
#define _training_skinhit_ganton_incl

#define SKINHIT_GANTON_NAME         "Skinhit Ganton"
#define SKINHIT_GANTON_TYPE         "Skinhit"
#define SKINHIT_GANTON_SPAWN_X      (2429.4163)
#define SKINHIT_GANTON_SPAWN_Y      (-1655.9668)
#define SKINHIT_GANTON_SPAWN_Z      (13.3828)
#define SKINHIT_GANTON_SPAWN_A      (183.2217)
#define SKINHIT_GANTON_TARGET_MODEL (19063)
#define SKINHIT_GANTON_SPEED        (8.5)
#define SKINHIT_GANTON_START_X      (2439.6333)
#define SKINHIT_GANTON_START_Y      (-1680.3707)
#define SKINHIT_GANTON_START_Z      (13.7759)
#define SKINHIT_GANTON_START_A      (88.0319)
#define SKINHIT_GANTON_END_X        (2422.3071)
#define SKINHIT_GANTON_END_Y        (-1680.2498)
#define SKINHIT_GANTON_END_Z        (13.7819)
#define SKINHIT_GANTON_END_A        (269.5808)

forward Init_SkinhitGanton();
public Init_SkinhitGanton()
{
    new weapons[2] = { 24, 25 };

    RegisterTrainingArena(
        SKINHIT_GANTON_NAME,
        SKINHIT_GANTON_TYPE,
        SKINHIT_GANTON_SPAWN_X, SKINHIT_GANTON_SPAWN_Y, SKINHIT_GANTON_SPAWN_Z, SKINHIT_GANTON_SPAWN_A,
        SKINHIT_GANTON_START_X, SKINHIT_GANTON_START_Y, SKINHIT_GANTON_START_Z, SKINHIT_GANTON_START_A,
        SKINHIT_GANTON_END_X, SKINHIT_GANTON_END_Y, SKINHIT_GANTON_END_Z, SKINHIT_GANTON_END_A,
        SKINHIT_GANTON_SPEED,
        SKINHIT_GANTON_TARGET_MODEL,
        weapons,
        2
    );
    return 1;
}

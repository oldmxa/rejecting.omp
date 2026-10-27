#if defined _training_variables_included
    #endinput
#endif
#define _training_variables_included

//   global variables 

// registered training arenas array
new g_TrainingArenas[TRAINING_MAX_ARENAS][E_TRAINING_ARENA];
new g_TotalTrainingArenas = 0;

// per-player training state
new g_PlayerTraining[MAX_PLAYERS][E_PLAYER_TRAINING];
new PlayerText:g_TrainingTimerTD[MAX_PLAYERS];
new PlayerText:g_TrainingCountdownTD[MAX_PLAYERS];

new g_ChallengeServerRecord[TRAINING_MAX_ARENAS];
new g_ChallengeServerHolder[TRAINING_MAX_ARENAS][MAX_PLAYER_NAME + 1];
new g_ChallengePlayerRecord[MAX_PLAYERS][TRAINING_MAX_ARENAS];

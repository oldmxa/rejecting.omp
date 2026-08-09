
#if defined _vehicle_picker_included
    #endinput
#endif
#define _vehicle_picker_included

#include <a_samp>

#if !defined DIALOG_VEHICLE_PREVIEW
    #define DIALOG_VEHICLE_PREVIEW 14503
#endif

#define VEHICLE_ID_MIN 400
#define VEHICLE_ID_MAX 611

stock ShowVehicleDialog(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    new body[4096];
    body[0] = '\0';

    for (new vehicle = VEHICLE_ID_MIN; vehicle <= VEHICLE_ID_MAX; vehicle++)
    {
        strcat(body, fmat_return("%s%d\t%s",
            (body[0] == EOS) ? ("") : ("\n"), vehicle, VehicleNames[vehicle - VEHICLE_ID_MIN]), sizeof(body));
    }

    new title[64] = "{MAIN}Lounge - Vehicle";
    format_MainColor(playerid, title);

    return ShowPlayerDialog(playerid, DIALOG_VEHICLE_PREVIEW, DIALOG_STYLE_PREVIEW_MODEL, title, body, "Spawn", "Cancel");
}

/*
 * Dispatched from modules/ui/preview_dialog.inc. inputtext carries the model id
 * of the clicked item.
 *
 * The lounge check runs again here: the grid stays open while the player moves,
 * so being in the lounge when it opened does not mean they still are.
 */
stock HandleVehiclePreviewResponse(playerid, response, const inputtext[])
{
    if (!response) return 1;

    if (!CanSpawnLoungeVehicle(playerid)) return 1;

    SpawnLoungeVehicle(playerid, strval(inputtext));
    return 1;
}

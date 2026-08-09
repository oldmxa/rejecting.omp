
#if defined _preview_dialog_included
    #endinput
#endif
#define _preview_dialog_included

forward OnPreviewModelResponse(playerid, dialogid, response, listitem, inputtext[]);
public OnPreviewModelResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
    {
        case DIALOG_SKIN_PREVIEW:    return HandleSkinPreviewResponse(playerid, response, inputtext);
        case DIALOG_VEHICLE_PREVIEW: return HandleVehiclePreviewResponse(playerid, response, inputtext);
    }

    return 0;
}

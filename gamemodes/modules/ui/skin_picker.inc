
#if defined _skin_picker_included
    #endinput
#endif
#define _skin_picker_included

#include <a_samp>

#if !defined DIALOG_SKIN_PREVIEW
    #define DIALOG_SKIN_PREVIEW 14502
#endif

#define SKIN_ID_MIN 2
#define SKIN_ID_MAX 311

// Ids in range that have no usable model. ChangeSkin() rejects them too.
static const InvalidSkins[] = {74, 86, 270, 271};

static bool:IsSelectableSkin(skinid)
{
    if (skinid < SKIN_ID_MIN || skinid > SKIN_ID_MAX) return false;

    for (new i = 0; i < sizeof(InvalidSkins); i++)
        if (InvalidSkins[i] == skinid) return false;

    return true;
}

stock ShowSkinDialog(playerid, source)
{
    if (!IsPlayerConnected(playerid)) return 0;

    SetAppearanceSource(playerid, source);

    new body[3072];
    body[0] = '\0';

    for (new skinid = SKIN_ID_MIN; skinid <= SKIN_ID_MAX; skinid++)
    {
        if (!IsSelectableSkin(skinid)) continue;

        strcat(body, fmat_return("%s%d\t%d", (body[0] == EOS) ? ("") : ("\n"), skinid, skinid), sizeof(body));
    }

    new title[64] = "{MAIN}Personalization - Character Skin";
    format_MainColor(playerid, title);

    return ShowPlayerDialog(playerid, DIALOG_SKIN_PREVIEW, DIALOG_STYLE_PREVIEW_MODEL, title, body, "Select", "Back");
}

/*
 * Fired by PreviewModelDialog.inc, not by SA-MP: a preview dialog is drawn with
 * textdraws and never reaches the native dialog callback. inputtext carries the
 * model id of the clicked item, which is the skin id here.
 */
forward OnPreviewModelResponse(playerid, dialogid, response, listitem, inputtext[]);
public OnPreviewModelResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid != DIALOG_SKIN_PREVIEW) return 0;

    if (response)
    {
        new skinid = strval(inputtext);

        if (IsSelectableSkin(skinid))
            ChangeSkin(playerid, skinid);
    }

    ReturnToAppearanceSource(playerid);
    return 1;
}

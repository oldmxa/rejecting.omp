
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

    return ShowPlayerDialog(playerid, DIALOG_SKIN_PREVIEW, DIALOG_STYLE_PREVIEW_MODEL,
        "Personalization - Character Skin", body, "Select", "Back");
}

stock HandleSkinPreviewResponse(playerid, response, const inputtext[])
{
    if (response)
    {
        new skinid = strval(inputtext);

        if (IsSelectableSkin(skinid))
            ChangeSkin(playerid, skinid);
    }

    ReturnToAppearanceSource(playerid);
    return 1;
}

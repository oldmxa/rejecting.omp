
#if defined _style_picker_included
    #endinput
#endif
#define _style_picker_included

#define STYLE_PICKER_MARKER "{MAIN}> "
#define STYLE_PICKER_OK     "Select"
#define STYLE_PICKER_CANCEL "Cancel"

stock AppendStyleRow(dest[], const label[], bool:selected, len = sizeof(dest))
{
    if (selected) strcat(dest, fmat_return(STYLE_PICKER_MARKER "{FFFFFF}%s\n", label), len);
    else          strcat(dest, fmat_return("{FFFFFF}%s\n", label), len);

    return 1;
}

stock ShowStylePicker(playerid, dialogid, const title[], body[], bodyLen = sizeof(body))
{
    new dialogTitle[64];
    format(dialogTitle, sizeof(dialogTitle), "%s", title);

    format_MainColor(playerid, dialogTitle);
    format_MainColor(playerid, body, bodyLen);

    return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, dialogTitle, body, STYLE_PICKER_OK, STYLE_PICKER_CANCEL);
}

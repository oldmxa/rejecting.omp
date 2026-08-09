
#if defined _style_picker_included
    #endinput
#endif
#define _style_picker_included

#define STYLE_PICKER_HEADER   "ID\tStyle\tStatus\n"
#define STYLE_PICKER_SELECTED "{00FF00}Selected"
#define STYLE_PICKER_SELECT   "{FFFFFF}Select"
#define STYLE_PICKER_OK       "Select"
#define STYLE_PICKER_CANCEL   "Cancel"

stock AppendStyleRow(dest[], index, const label[], bool:selected, len = sizeof(dest))
{
    strcat(dest, fmat_return("%d\t%s\t%s\n", index, label, selected ? (STYLE_PICKER_SELECTED) : (STYLE_PICKER_SELECT)), len);
    return 1;
}

stock ShowStylePicker(playerid, dialogid, const title[], const body[])
{
    return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, title, body, STYLE_PICKER_OK, STYLE_PICKER_CANCEL);
}

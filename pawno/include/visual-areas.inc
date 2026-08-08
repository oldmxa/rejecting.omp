#if defined _INC_visual_areas
    #endinput
#endif
#define _INC_visual_areas

#tryinclude <open.mp>

#if !defined _INC_open_mp
    #include <a_samp>
#endif

#include <colandreas>
#include <YSI_Data\y_iterate>

#if !defined MAX_VISUAL_AREAS
    #define MAX_VISUAL_AREAS            (VisualArea:MAX_PLAYERS)
#endif

#if !defined MAX_OBJECTS_PER_VISUAL_AREA
    #define MAX_OBJECTS_PER_VISUAL_AREA 100
#endif

#define INVALID_VISUAL_AREA_ID          (VisualArea:cellmin)

static
    STREAMER_TAG_OBJECT:s_VisualAreaObject[MAX_VISUAL_AREAS][MAX_OBJECTS_PER_VISUAL_AREA],

    Iterator:VisualArea<(_:MAX_VISUAL_AREAS)>,
    Iterator:VisualAreaObject[MAX_VISUAL_AREAS]<MAX_OBJECTS_PER_VISUAL_AREA>;

stock bool:IsValidVisualArea(VisualArea:id)
{
    return Iter_Contains(VisualArea, _:id);
}

stock VisualArea:CreateVisualArea(Float:minx, Float:miny, Float:maxx, Float:maxy, playerid = -1, Float:streamdistance = STREAMER_OBJECT_SD)
{
    new VisualArea:id = VisualArea:Iter_Alloc(VisualArea);
    
    if (id == INVALID_VISUAL_AREA_ID)
    {
        return id;
    }

    new Float:x, Float:y, Float:z, Float:lowestZ, Float:distance, objectid, modelid;

    CA_FindZ_For2DCoord(minx, miny, z);
    lowestZ = z;

    CA_FindZ_For2DCoord(maxx, miny, z);
    if (z < lowestZ)
    {
        lowestZ = z;
    }
    
    CA_FindZ_For2DCoord(minx, maxy, z);
    if (z < lowestZ)
    {
        lowestZ = z;
    }

    CA_FindZ_For2DCoord(maxx, maxy, z);
    if (z < lowestZ)
    {
        lowestZ = z;
    }

    x = minx;
    distance = floatabs(maxx - minx);

    while (distance > 0.0)
    {
        if (distance >= 50.0)
        {
            x += 25.0;
            distance -= 50.0;
            modelid = 11751;
        }
        else if (distance >= 10.0)
        {
            x += 5.0;
            distance -= 10.0;
            modelid = 11752;
        }
        else
        {
            x += 0.5;
            distance -= 1.0;
            modelid = 11753;
        }

        for (new i; i < 2; i ++)
        {
            objectid = Iter_Alloc(VisualAreaObject[id]);

            if (objectid == INVALID_ITERATOR_SLOT)
            {
                break;
            }

            s_VisualAreaObject[id][objectid] = CreateDynamicObject(modelid, x, !i ? miny : maxy, z, 0.0, 0.0, 0.0, .playerid = playerid, .streamdistance = streamdistance);
            SetDynamicObjectMaterial(s_VisualAreaObject[id][objectid], 0, modelid, "EnExMarkers", "enexmarker4-4");
        }

        if (objectid == INVALID_ITERATOR_SLOT)
        {
            break;
        }

        switch (modelid)
        {
            case 11751:
                x += 25.0;
            case 11752:
                x += 5.0;
            case 11753:
                x += 0.5;
        }
    }

    y = miny;
    distance = floatabs(maxy - miny);

    while (distance > 0.0)
    {
        if (distance >= 50.0)
        {
            y += 25.0;
            distance -= 50.0;
            modelid = 11751;
        }
        else if (distance >= 10.0)
        {
            y += 5.0;
            distance -= 10.0;
            modelid = 11752;
        }
        else
        {
            y += 0.5;
            distance -= 1.0;
            modelid = 11753;
        }

        for (new i; i < 2; i ++)
        {
            objectid = Iter_Alloc(VisualAreaObject[id]);

            if (objectid == INVALID_ITERATOR_SLOT)
            {
                break;
            }

            s_VisualAreaObject[id][objectid] = CreateDynamicObject(modelid, !i ? minx : maxx, y, z, 0.0, 0.0, 90.0, .playerid = playerid, .streamdistance = streamdistance);
            SetDynamicObjectMaterial(s_VisualAreaObject[id][objectid], 0, modelid, "EnExMarkers", "enexmarker4-4");
        }

        if (objectid == INVALID_ITERATOR_SLOT)
        {
            break;
        }

        switch (modelid)
        {
            case 11751:
                y += 25.0;
            case 11752:
                y += 5.0;
            case 11753:
                y += 0.5;
        }
    }

    return id;
}

stock bool:DestroyVisualArea(VisualArea:id)
{
    if (!IsValidVisualArea(id))
    {
        return false;
    }

    foreach (new i : VisualAreaObject[id])
    {
        DestroyDynamicObject(s_VisualAreaObject[id][i]);
        s_VisualAreaObject[id][i] = STREAMER_TAG_OBJECT:INVALID_STREAMER_ID;
    }

    Iter_Clear(VisualAreaObject[id]);
    Iter_Remove(VisualArea, _:id);

    return true;
}

stock Float:GetVisualAreaZ(VisualArea:id)
{
    if (!IsValidVisualArea(id))
    {
        return FLOAT_NAN;
    }

    new Float:z;
    GetDynamicObjectPos(s_VisualAreaObject[id][0], z, z, z);
    return z;
}

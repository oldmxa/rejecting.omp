
#if defined _arena_object_included
    #endinput
#endif
#define _arena_object_included

#include <a_samp>

stock STREAMER_TAG_OBJECT:CreateArenaObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, const worlds[] = { -1 }, const interiors[] = { -1 }, const players[] = { -1 }, const STREAMER_TAG_AREA:areas[] = { STREAMER_TAG_AREA:-1 }, priority = 0, maxworlds = sizeof worlds, maxinteriors = sizeof interiors, maxplayers = sizeof players, maxareas = sizeof areas)
{
    new STREAMER_TAG_OBJECT:id = CreateDynamicObjectEx(modelid, x, y, z, rx, ry, rz, streamdistance, drawdistance, worlds, interiors, players, areas, priority, maxworlds, maxinteriors, maxplayers, maxareas);

    CA_CreateObject(modelid, x, y, z, rx, ry, rz, true);

    return id;
}

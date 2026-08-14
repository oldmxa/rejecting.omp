#if defined _scoreboard_included
    #endinput
#endif
#define _scoreboard_included

const RPC_SCORES_PINGS = 155;

#define SB_RECORD_BITS (80)
#define SB_PING_BIT    (48)

ORPC:RPC_SCORES_PINGS(playerid, BitStream:bs)
{
    #pragma unused playerid

    new bits;
    BS_GetNumberOfBitsUsed(bs, bits);

    new records = bits / SB_RECORD_BITS;
    if (records < 1) return 1;

    for (new i; i < records; i++)
    {
        BS_SetWriteOffset(bs, i * SB_RECORD_BITS + SB_PING_BIT);
        BS_WriteUint32(bs, SCOREBOARD_PING);
    }
    BS_SetWriteOffset(bs, bits);
    return 1;
}

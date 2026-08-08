#if defined _INC_alpha_fix
	#endinput
#endif
#define _INC_alpha_fix

#tryinclude <open.mp>

#if !defined _INC_open_mp
	#include <a_samp>
#endif

#include <Pawn.RakNet>

const
	AF_RPC_TOGGLE_SELECT_TD = 83,
	AF_RPC_SHOW_TEXT_DRAW = 134;

static bool:s_NewClient[MAX_PLAYERS char];

static FixAlpha(colour)
{
	// Extract the alpha channel.
	new alpha = (colour >> 24) & 0xFF;
	if (alpha == 0 || alpha == 255)
	{
		return colour;
	}
	// Convert to a fraction.
	new Float:n = alpha / 255.0;
	// Darken the result.
	new Float:m = n * (2.0 - n);
	// Convert back to a value.
	new final_ = floatround(m * 255.0, floatround_ceil);
	// Clamp.
	if (final_ > 255)
	{
		final_ = 255;
	}
	// Put back in the colour.
	return (colour & 0xFFFFFF) | (final_ << 24);
}

ORPC:AF_RPC_TOGGLE_SELECT_TD(playerid, BitStream:bs)
{
	if (playerid == -1 || !s_NewClient{playerid})
	{
		return 1;
	}

	new colour;
	
	BS_ReadValue(bs,
		PR_IGNORE_BITS, 8,
		PR_UINT32, colour
	);

	BS_SetWriteOffset(bs, 8);
	BS_WriteUint32(bs, FixAlpha(colour));

	return 1;
}

ORPC:AF_RPC_SHOW_TEXT_DRAW(playerid, BitStream:bs)
{
	if (playerid == -1 || !s_NewClient{playerid})
	{
		return 1;
	}

	new letterColour, boxColour, backgroundColour, offset;

	BS_ReadValue(bs,
		PR_IGNORE_BITS, 16 + 8 + 32 + 32,
		PR_UINT32, letterColour,
		PR_IGNORE_BITS, 32 + 32,
		PR_UINT32, boxColour,
		PR_IGNORE_BITS, 8 + 8,
		PR_UINT32, backgroundColour
	);

	BS_GetWriteOffset(bs, offset);
	BS_SetWriteOffset(bs, 16 + 8 + 32 + 32);

	BS_WriteValue(bs,
		PR_UINT32, FixAlpha(letterColour),
		PR_IGNORE_BITS, 32 + 32,
		PR_UINT32, FixAlpha(boxColour),
		PR_IGNORE_BITS, 8 + 8,
		PR_UINT32, FixAlpha(backgroundColour)
	);

	BS_SetWriteOffset(bs, offset);

	return 1;
}

public OnPlayerConnect(playerid)
{
	new version[24];
	GetPlayerVersion(playerid, version);
	
	s_NewClient{playerid} = !strequal(version, "0.3.7") && !strequal(version, "0.3.7-R2");

	#if defined AF_OnPlayerConnect
		return AF_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AF_OnPlayerConnect
#if defined AF_OnPlayerConnect
	forward AF_OnPlayerConnect(playerid);
#endif

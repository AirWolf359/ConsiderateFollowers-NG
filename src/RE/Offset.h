#pragma once

// Address library IDs for SE and AE are verified by decoding the respective binary
// address library files and confirming each ID resolves to a valid function start.
// IDs that are the same for SE and AE reflect genuine stability in the address library
// numbering across versions — CreateDialogueItem is the exception, not the rule.

namespace RE
{
	namespace Offset
	{
		namespace AIProcess
		{
			// SE/AE: address library IDs verified in both address library binaries
			// VR: 0x64DB70 forwards into the high-process greeting implementation at 0x66D9A0.
			constexpr auto ProcessGreet = REL::VariantID(39162, 39162, 0x64DB70);
		}

		namespace Character
		{
			// SE/AE: same ID verified in both address library binaries
			constexpr auto IsTalking = REL::VariantID(37266, 37266, 0x6211C0);
		}

		namespace DialogueItem
		{
			// SE/AE: address library IDs verified in both address library binaries
			constexpr auto FirstResponse = REL::VariantID(35222, 35222, 0x573290);
			constexpr auto RunResult     = REL::VariantID(35225, 35225, 0x573300);
		}

		namespace PlayerCharacter
		{
			// SE/AE: same ID verified in both address library binaries
			constexpr auto GetSleepRestState = REL::VariantID(40413, 40413, 0x707380);
		}

		namespace TESTopic
		{
			// SE ID 25014: 0x38F9A0 — AE ID 25541: 0x3E82A0 — hook offset +0xE2 verified for both
			// VR: 0x39F220 + 0xE2 calls DialogueItem::Ctor at 0x572FD0.
			constexpr auto CreateDialogueItem = REL::VariantID(25014, 25541, 0x39F220);
		}
	}
}

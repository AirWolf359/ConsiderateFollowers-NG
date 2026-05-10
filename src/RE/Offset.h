#pragma once

namespace RE
{
	namespace Offset
	{
		namespace AIProcess
		{
			// VR: CSV maps to 0x69BA80 but Ghidra shows this is mid-function — needs runtime verification
			constexpr auto ProcessGreet = REL::VariantID(39162, 39162, 0x69BA80);
		}

		namespace Character
		{
			constexpr auto IsTalking = REL::VariantID(37266, 37266, 0x6211C0);
		}

		namespace DialogueItem
		{
			constexpr auto FirstResponse = REL::VariantID(35222, 35222, 0x595DB0);
			constexpr auto RunResult     = REL::VariantID(35225, 35225, 0x5A3CF0);
		}

		namespace PlayerCharacter
		{
			constexpr auto GetSleepRestState = REL::VariantID(40413, 40413, 0x707380);
		}

		namespace TESTopic
		{
			// SE ID 25014: 0x38F9A0 — AE ID 25541: 0x3E82A0 (1.6.1179) — hook at +0xE2
			// VR: 0x3B8720 confirmed but function is only 66 bytes; hook at +0xE2 cannot work
			constexpr auto CreateDialogueItem = REL::VariantID(25014, 25541, 0x3B8720);
		}
	}
}

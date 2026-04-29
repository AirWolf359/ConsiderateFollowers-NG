#pragma once

namespace RE
{
	namespace Offset
	{
		namespace AIProcess
		{
			constexpr auto ProcessGreet = REL::VariantID(39162, 39162, 0x69BB90);
		}

		namespace Character
		{
			constexpr auto IsTalking = REL::VariantID(37266, 37266, 0x621000);
		}

		namespace DialogueItem
		{
			constexpr auto FirstResponse = REL::VariantID(35222, 35222, 0x5A3C40);
			constexpr auto RunResult     = REL::VariantID(35225, 35225, 0x5A3CF0);
		}

		namespace PlayerCharacter
		{
			constexpr auto GetSleepRestState = REL::VariantID(40413, 40413, 0x707380);
		}

		namespace TESTopic
		{
			constexpr auto CreateDialogueItem = REL::VariantID(25541, 25541, 0x3B8770);
		}
	}
}

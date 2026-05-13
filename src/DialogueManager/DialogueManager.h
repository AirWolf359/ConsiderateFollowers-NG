#pragma once

#include <unordered_set>

#include "StoredDialogue.h"

namespace DialogueManager
{
	class Manager :
		public ISingleton<Manager>,
		public SKSE::detail::TaskDelegate
	{
	public:
		bool AllowDialogue(RE::Actor* a_speaker, RE::TESTopic* a_topic);
		void UpdateInternalConversation(RE::Actor* a_speaker);
		void RefreshCachedState();

		bool RequestINISettings();
		void RegisterWhitelistedNPCs(const std::vector<const RE::TESNPC*>& a_newNPCs);

		void Run() override;
		void Dispose() override;
		void QueueTask();

	private:
		bool IsClosestActorSpeaking() const;
		bool IsAnyFollowerSpeaking(RE::Actor* a_excluded) const;

		std::unordered_set<const RE::TESNPC*> whitelistedNPCs{};
		std::vector<StoredDialogue>           pendingDialogue;
		RE::FormID                            closestSpeakerID{ 0 };

		std::atomic<RE::FormID> m_speakingFollowerID{ 0 };
		std::atomic<bool>       m_closestSpeakerTalking{ false };

		bool  queued{ false };
		float maximumDistance{ 0.0f };
	};
}
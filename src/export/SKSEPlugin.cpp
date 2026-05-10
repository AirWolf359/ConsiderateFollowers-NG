#include "Hooks/Hooks.h"
#include "Settings/INISettings.h"
#include "Settings/JSONSettings.h"

namespace
{
	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path) {
			SKSE::stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format("{}.log"sv, Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%^%l%$] %v"s);
	}

	void MessageEventCallback(SKSE::MessagingInterface::Message* a_msg)
	{
		if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
			if (!Settings::JSON::Holder::GetSingleton()->Read()) {
				SKSE::stl::report_and_fail("Failed to read JSON settings."sv);
			}
			logger::info("Startup tasks finished, enjoy your game!");
		}
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();

	const auto plugin = SKSE::PluginDeclaration::GetSingleton();
	logger::info("{} v{} loading...", plugin->GetName(), plugin->GetVersion());

	SKSE::Init(a_skse);

	if (!Settings::INI::Holder::GetSingleton()->Read()) {
		SKSE::stl::report_and_fail("Failed to read INI settings."sv);
	}
	if (!Hooks::Install()) {
		SKSE::stl::report_and_fail("Failed to install necessary hooks."sv);
	}

	SKSE::GetMessagingInterface()->RegisterListener(&MessageEventCallback);

	logger::info("{} loaded.", plugin->GetName());
	return true;
}

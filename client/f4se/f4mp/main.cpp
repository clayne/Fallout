#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"

#include "F4MP.h"

#include <memory>
#include <shlobj.h>

std::unique_ptr<f4mp::F4MP> instance;

extern "C"
{
	bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4MP\\F4MP.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "F4MP";
		info->version = 1;

		if (f4se->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if (f4se->runtimeVersion != RUNTIME_VERSION_1_10_163)
		{
			_MESSAGE("unsupported runtime version %08X", f4se->runtimeVersion);

			return false;
		}

		return true;
	}

	bool F4SEPlugin_Load(const F4SEInterface* f4se)
	{
		FILE* tmp;
		AllocConsole();
		freopen_s(&tmp, "CONOUT$", "w", stdout);
		printf("opening console for debug.\n");

		instance = std::make_unique<f4mp::F4MP>(f4se);

		_MESSAGE("F4MP loaded");

		return true;
	}
};
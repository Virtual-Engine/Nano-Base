#pragma once

namespace Features {
	void OptionToggleUpdate();
	
}

namespace Protection {
	void OptionToggleUpdate();

	extern bool BlockScriptEventData;
	void blockscripteventdata(bool toggle);

	extern bool BlockCrashModelSkid;
	void blockmodelcrashskid(bool toggle); 
}


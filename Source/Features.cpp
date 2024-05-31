#include "stdafx.h"
#include "Functions.h"
#include "Features.h"

void Features::OptionToggleUpdate() {



}


void Protection::OptionToggleUpdate() {
	//Here you add the option boolean calling 
	BlockScriptEventData ? blockscripteventdata(true) : NULL;
	BlockCrashModelSkid ? blockmodelcrashskid(true) : NULL;
}


//////////////
//Protection//
//////////////

bool Protection::BlockScriptEventData = false;
void Protection::blockscripteventdata(bool toggle) {
	//No code Hosted here look in hooking.cpp :)
}

bool Protection::BlockCrashModelSkid = false;
void Protection::blockmodelcrashskid(bool toggle) {
	//No code Hosted here look in hooking.cpp :)
}
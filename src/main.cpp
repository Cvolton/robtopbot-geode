#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>

#include "utils.h"

using namespace geode::prelude;

Patch* extras = nullptr;

void loadExtras() {
	#ifdef GEODE_IS_WINDOWS
        log::info("Performing Windows specific loading steps");
        extras = Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0xAA8CE), { 0x1 }).unwrap();
    #endif

	#ifdef GEODE_IS_MACOS
        log::info("Performing Mac specific loading steps");
        extras = Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x2C7E82), { 0x89, 0xE1 }).unwrap(); //mov ecx, esp ; (very good, yes)
        
    #endif

    if(!extras) log::error("Enabling extras failed");

}

$execute {
	TempSettings::shouldLoadReplay = false;

	if(Mod::get()->getSettingValue<bool>("download-verification-replay")) {
		loadExtras();
    }

	listenForSettingChanges("download-verification-replay", +[](bool value) {
		if(!extras) loadExtras();
        value ? extras->apply() : extras->restore();
    });
}
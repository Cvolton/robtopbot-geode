/**
 * Utilizing snippets from:
 * https://github.com/qimiko/robtop-bot/blob/main/src/game_hooks.cpp
 * licensed under MIT
 */

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "../utils.h"

using namespace geode::prelude;

/**
 * @brief      Splits a string into an array of substrings
 *
 * @param      string     The string
 * @param[in]  separator  The separator
 *
 * @return     a vector of strings split by the separator
 */
std::vector<std::string> explode(gd::string &string, char separator) {
	std::stringstream segmentstream(string);
	std::string segmented;
	std::vector<std::string> splitlist;

	while (std::getline(segmentstream, segmented, separator)) {
		splitlist.push_back(segmented);
	}

	return splitlist;
}

class $modify(RBPlayLayer, PlayLayer) {
	void setupReplay(gd::string replay) {
		this->m_collisionDisabled = true;
	    // this has to be false. it should be false by default.
		this->unknown701 = false;
		//auto disable_replay_clicks = offset_from_base<bool>(self, 0x471);
		//*disable_replay_clicks = false;

		//auto replay_inputs = offset_from_base<cocos2d::CCArray *>(self, 0x448);

	    // we don't know if there is a replay loaded or not, assume not
		auto new_inputs = cocos2d::CCArray::create();

		auto parsed_replay_data = explode(replay, ';');

		for (const auto& parsed_data : parsed_replay_data)
		{
			auto string_action = cocos2d::CCString::create(parsed_data);
			new_inputs->addObject(string_action);
		}

		new_inputs->retain();

		m_replayInputs = new_inputs;
	}

	bool init(GJGameLevel* level) {
		auto res = PlayLayer::init(level);
		if(!res) return false;

		if (TempSettings::shouldLoadReplay && !std::string(level->m_recordString).empty()) {
			auto decompressed_string = cocos2d::ZipUtils::decompressString(level->m_recordString, false, 0xB);

			setupReplay(decompressed_string);

        // "safety"
			auto label = cocos2d::CCLabelBMFont::create("Playback", "bigFont.fnt");
			if(Mod::get()->getSettingValue<bool>("show-playing")) addChild(label, 50);

			label->setScale(0.25f);
			label->setOpacity(80);

			label->setPosition(22.0f, 8.0f);

			m_isTestMode = true;

			TempSettings::shouldLoadReplay = false;
		} else {
        	// recording is only usually enabled when levelType == 2, this keeps it always enabled
			//auto do_record_actions = offset_from_base<bool>(self, 0x429);
			//*do_record_actions = true;
			//
			log::info("no replay available");
		}

		return true;
	}

	void updateReplay(float dt) {
		m_antiCheatPassed = true;

		PlayLayer::updateReplay(dt);
	}

};

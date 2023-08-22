#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include "../utils.h"

using namespace geode::prelude;

class $modify(RBLevelInfoLayer, LevelInfoLayer) {
    /*
     * Callbacks
     */

    void onPlayReplay(CCObject* sender) {
        TempSettings::shouldLoadReplay = true;

        LevelInfoLayer::onPlay(sender);
    }

    /*
     * Hooks
     */

    bool init(GJGameLevel* level) {
        if(!LevelInfoLayer::init(level)) return false;

        if(!std::string(m_level->m_recordString).empty()) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            auto playReplay = CCSprite::createWithSpriteFrameName("GJ_playEditorBtn_001.png");
            playReplay->setScale(0.8f);
            auto playReplayBtn = CCMenuItemSpriteExtra::create(
                playReplay,
                this,
                menu_selector(RBLevelInfoLayer::onPlayReplay)
            );
            playReplayBtn->setPosition(138, -24);

            auto newMenu = CCMenu::create();
            newMenu->addChild(playReplayBtn);
            this->addChild(newMenu);
            newMenu->setID("rb-play-menu");
        } else {
            log::info("no replay available for this level");
        }

        return true;
    }
};
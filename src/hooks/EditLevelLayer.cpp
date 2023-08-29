#include <Geode/Bindings.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

#include "../utils.h"

using namespace geode::prelude;

class $modify(RBEditLevelLayer, EditLevelLayer) {
    /*
     * Callbacks
     */

    void onPlayReplay(CCObject* sender) {
        TempSettings::shouldLoadReplay = true;

        EditLevelLayer::onPlay(sender);
    }

    /*
     * Hooks
     */

    bool init(GJGameLevel* level) {
        if(!EditLevelLayer::init(level)) return false;

        if(!std::string(m_level->m_recordString).empty()) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            auto playReplay = CCSprite::createWithSpriteFrameName("GJ_playEditorBtn_001.png");
            playReplay->setScale(0.8f);
            auto playReplayBtn = CCMenuItemSpriteExtra::create(
                playReplay,
                this,
                menu_selector(RBEditLevelLayer::onPlayReplay)
            );
            playReplayBtn->setPosition(150, - winSize.height / 2 + 25);

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
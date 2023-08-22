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

    void createReplayButton() {
        if(this->getChildByID("rb-play-menu")) return;

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
    }

    /*
     * Hooks
     */

    bool init(GJGameLevel* level) {
        if(!LevelInfoLayer::init(level)) return false;

        this->createReplayButton();

        return true;
    }


    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);
        createReplayButton();
    };

    void levelUpdateFinished(GJGameLevel* level, UpdateResponse response) {
        LevelInfoLayer::levelUpdateFinished(level, response);
        createReplayButton();
    };
};
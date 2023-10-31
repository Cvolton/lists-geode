#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/utils/web.hpp>

#include "layers/ListsSearchLayer.h"
#include "layers/ListsViewLayer.h"
#include "objects/ListSearchObject.hpp"

using namespace geode::prelude;

class $modify(ListsCreatorLayer, CreatorLayer) {
	void onLists(cocos2d::CCObject* sender) {
        auto layer = ListsSearchLayer::scene();
        auto transitionFade = CCTransitionFade::create(0.5, layer);
        CCDirector::sharedDirector()->pushScene(transitionFade);
	}

	bool init() {
		if(!CreatorLayer::init()) return false;

		auto listsSprite = CCSprite::create("GJ_listsBtn_001.png"_spr);
		listsSprite->setScale(0.85f);

		auto listsBtn = CCMenuItemSpriteExtra::create(
	        listsSprite,
	        this,
	        menu_selector(ListsCreatorLayer::onLists)
	    );
	    listsBtn->setPosition({120,-57.5f});
	    listsBtn->setSizeMult(1.2f);
	    listsBtn->setID("lists"_spr);

		listsBtn->setContentSize({88.825f, 88.825f});
		listsSprite->setPosition(listsBtn->getContentSize() / 2);

		auto menu = this->getChildByID("creator-buttons-menu");
        menu->addChild(listsBtn);
        menu->updateLayout();

		return true;
	}
};

class $modify(ListsProfilePage, ProfilePage) {
	void onLists(CCObject*) {
		ListSearchObject obj;
		obj.m_type = 5;
		obj.m_str = std::to_string(m_accountID);

		auto browserLayer = ListsViewLayer::scene(obj);
		auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
		CCDirector::sharedDirector()->pushScene(transitionFade);
	}

	void loadPageFromUserInfo(GJUserScore* score) {
		ProfilePage::loadPageFromUserInfo(score);

		if(score->m_accountID == GJAccountManager::sharedState()->m_accountID) return;

		auto listsBtnSpr = AccountButtonSprite::createWithSprite("lists_icon.png"_spr, 1.f, AccountBaseColor::Purple);
		listsBtnSpr->setScale(.75f);

		auto listsBtn = CCMenuItemSpriteExtra::create(
            listsBtnSpr, this, menu_selector(ListsProfilePage::onLists)
        );
		listsBtn->setID("my-lists"_spr);
		m_buttonMenu->addChild(listsBtn);
		this->m_buttons->addObject(listsBtn);

		if(Mod::get()->getSettingValue<bool>("alt-profilepage")) listsBtn->setPosition(408, -224);
		else {
			listsBtn->setPosition(368, -258);

			//hide my levels text
			auto winSize = CCDirector::sharedDirector()->getWinSize();
			for(unsigned int i = 0; i < m_mainLayer->getChildrenCount(); i++){
				CCNode* node = dynamic_cast<CCNode*>(m_mainLayer->getChildren()->objectAtIndex(i));
				if(node != nullptr && node->getPositionX() == (winSize.width / 2) + 138 && node->getPositionY() == (winSize.height / 2) - 123) node->setVisible(false);
			}
		}
	}
};

$on_mod(Loaded) {
	web::AsyncWebRequest().fetch(fmt::format("https://geometrydash.eu/mods/lists/_api/ping/?platform={}&version={}", GEODE_PLATFORM_NAME, Mod::get()->getVersion().toString(false))).json().then([](const json::Value& info){
        auto notice = info.try_get("notice");
        if(notice == std::nullopt) return;
        
        if(info["notice"].is_string()) {
            auto alert = FLAlertLayer::create("Lists Mod", info["notice"].as_string(), "OK");
            alert->show();
        }
    }).expect([](const std::string& error){
        log::warn("Fetching important notices failed: {}", error);
    });
}
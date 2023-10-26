#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>

#include "layers/ListsSearchLayer.h"

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

		auto menu = this->getChildByID("creator-buttons-menu");
        menu->addChild(listsBtn);
        menu->updateLayout();

		return true;
	}
};

#include "ListsSearchLayer.h"
#include "ListsViewLayer.h"
#include "../objects/ListSearchObject.hpp"

ListsSearchLayer* ListsSearchLayer::create() {
    auto ret = new ListsSearchLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool ListsSearchLayer::init() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    auto size = backgroundSprite->getContentSize();
    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({0, 102, 255});
    backgroundSprite->setZOrder(-2);
    addChild(backgroundSprite);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(ListsSearchLayer::onBack)
    );

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);

    setKeypadEnabled(true);

    auto menu = CCMenu::create();
    menu->setID("search-menu"_spr);
    addChild(menu);

    auto searchBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png"),
        this,
        menu_selector(ListsSearchLayer::onSearch)
    );
    menu->addChild(searchBtn);
    searchBtn->setPosition({60,55});
    searchBtn->setSizeMult(1.2f);
    searchBtn->setID("bi-search-button");
    return true;
}

void ListsSearchLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void ListsSearchLayer::onBack(CCObject* object) {
    keyBackClicked();
}

void ListsSearchLayer::onSearch(CCObject* object) {    
    ListSearchObject obj;
    obj.m_type = 4;

    auto browserLayer = ListsViewLayer::scene(obj);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

    CCDirector::sharedDirector()->pushScene(transitionFade);
}

CCScene* ListsSearchLayer::scene() {
    auto layer = ListsSearchLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
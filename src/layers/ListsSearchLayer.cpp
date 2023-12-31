#include "ListsSearchLayer.h"
#include "ListsViewLayer.h"
#include "ListSearch/MoreListSearchOptions.h"
#include "../objects/ListSearchObject.hpp"
#include "../utils.hpp"

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

CCMenuItemSpriteExtra* ListsSearchLayer::createButton(const char* texture, const char* text, const char* icon, int searchType, float size) {
    auto searchTexture = SearchButton::create(texture,
        text,
        size,
        icon
    );
    auto searchBtn = CCMenuItemSpriteExtra::create(
        searchTexture,
        this,
        menu_selector(ListsSearchLayer::onSearch)
    );
    m_menu->addChild(searchBtn);
    searchBtn->setSizeMult(1.2f);
    searchBtn->setTag(searchType);
    return searchBtn;
}

bool ListsSearchLayer::init() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto diff = Mod::get()->getSavedValue<int>("difficulty");
    if(diff) m_diff = diff;
    
    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    auto size = backgroundSprite->getContentSize();
    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({0, 102, 255});
    backgroundSprite->setZOrder(-2);
    backgroundSprite->setID("background"_spr);
    addChild(backgroundSprite);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(ListsSearchLayer::onBack)
    );
    backBtn->setID("exit-button"_spr);

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});
    menuBack->setID("exit-menu"_spr);

    addChild(menuBack);

    setKeypadEnabled(true);

    m_menu = CCMenu::create();
    m_menu->setID("search-menu"_spr);
    m_menu->setPosition({winSize.width / 2, (winSize.height / 2)});
    addChild(m_menu);

    //quick search
    const float btnOffset = 0.f;

    auto quickBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    quickBg->setContentSize({365,115});
    quickBg->setColor({0,46,117});
    quickBg->setPosition({winSize.width / 2, (winSize.height / 2) + 28 + btnOffset});
    quickBg->setID("quick-search-bg"_spr);
    addChild(quickBg, -1);

    auto quickTitle = CCLabelBMFont::create("Quick Search", "bigFont.fnt");
    quickTitle->setPosition((winSize.width / 2), (winSize.height / 2) + 97);
    quickTitle->setScale(.5f);
    quickTitle->setID("quick-title"_spr);
    //quickTitle->setVisible(false);
    addChild(quickTitle);

    auto downloadBtn = createButton("GJ_longBtn03_001.png", "Most Downloaded", "GJ_sDownloadIcon_001.png", 1);
    downloadBtn->setPosition({-90.5f, 64.f + btnOffset});
    downloadBtn->setID("most-downloaded"_spr);

    auto likeBtn = createButton("GJ_longBtn03_001.png", "Most Liked", "GJ_sLikeIcon_001.png", 2, 0.6f);
    likeBtn->setPosition({90.5f, 64.f + btnOffset});
    likeBtn->setID("most-liked"_spr);

    auto featuredBtn = createButton("GJ_longBtn03_001.png", "Featured", "GJ_sBigStar_001.png"_spr, 6, 0.5f);
    featuredBtn->setPosition({-90.5f, 28.f + btnOffset});
    featuredBtn->setID("featured"_spr);

    auto followedBtn = createButton("GJ_longBtn03_001.png", "Followed", "GJ_sFollowedIcon_001.png", 12, 0.5f);
    followedBtn->setID("followed"_spr);
    followedBtn->setPosition({90.5f, 28.f + btnOffset});

    auto awardedBtn = createButton("GJ_longBtn04_001.png", "Awarded", "GJ_sStarsIcon_001.png", 11, 0.5f);
    awardedBtn->setPosition({-118.5f, -8.f + btnOffset});
    awardedBtn->setID("awarded"_spr);

    auto recentBtn = createButton("GJ_longBtn04_001.png", "Recent", "GJ_sRecentIcon_001.png", 4, 0.5f);
    recentBtn->setPosition({0.f, -8.f + btnOffset});
    recentBtn->setID("recent"_spr);

    auto friendsBtn = createButton("GJ_longBtn04_001.png", "Friends", "GJ_sFriendsIcon_001.png", 13, 0.5f);
    friendsBtn->setPosition({117.5f, -8.f + btnOffset});
    friendsBtn->setID("friends"_spr);

    //search
    auto searchBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    searchBg->setContentSize({365,40});
    searchBg->setColor({0,46,117});
    searchBg->setPosition({winSize.width / 2, (winSize.height / 2) + 130});
    searchBg->setID("search-bg"_spr);
    addChild(searchBg, -2);

    auto searchTextBg = cocos2d::extension::CCScale9Sprite::create("square02b_small.png", { 0.0f, 0.0f, 40.0f, 40.0f });
    searchTextBg->setContentSize({210,30});
    searchTextBg->setColor({0,39,98});
    searchTextBg->setPosition({winSize.width / 2 - 73, (winSize.height / 2) + 130});
    searchTextBg->setID("search-bg"_spr);
    addChild(searchTextBg, -1);

    auto searchButton = createButton("GJ_longBtn02_001.png", "Search", nullptr, 0, 0.6f);
    searchButton->setPosition({84.f, 130.f});
    searchButton->setID("search"_spr);

    auto searchBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_longBtn05_001.png"),
        this,
        menu_selector(ListsSearchLayer::onSearch)
    );
    searchBtn->setTag(5);
    searchBtn->setPosition(156.f, 130.f);
    searchBtn->setID("user-search"_spr);
    m_menu->addChild(searchBtn);

    m_textNode = CCTextInputNode::create(194, 50, "Enter a list, list ID or account ID", "bigFont.fnt");
    m_textNode->setLabelPlaceholderColor({108, 153, 216});
    m_textNode->setAllowedChars(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    m_textNode->setMaxLabelScale(0.7f);
    m_textNode->setMaxLabelWidth(50);
    m_textNode->m_textField->setAnchorPoint({ .0f, .5f });
    m_textNode->m_placeholderLabel->setAnchorPoint({ .0f, .5f });
    m_textNode->setPosition({winSize.width / 2 - 73 - 101, (winSize.height / 2) + 130});
    m_textNode->setID("search-bar"_spr);
    addChild(m_textNode);
    
    //filters
    auto filtersBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    filtersBg->setContentSize({365,97});
    filtersBg->setColor({0,36,91});
    filtersBg->setPosition({winSize.width / 2, (winSize.height / 2) - 24 - 73});
    filtersBg->setID("filters-bg"_spr);
    addChild(filtersBg, -1);

    auto filtersTitle = CCLabelBMFont::create("Filters", "bigFont.fnt");
    filtersTitle->setPosition((winSize.width / 2), (winSize.height / 2) - 37);
    filtersTitle->setScale(.5f);
    filtersTitle->setID("filters-title"_spr);
    //filtersTitle->setVisible(false);
    addChild(filtersTitle);

    renderFilters();
    
    //corners
    auto cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBL->setPosition({0,0});
    cornerBL->setAnchorPoint({0,0});
    cornerBL->setID("left-corner"_spr);
    addChild(cornerBL, -1);

    auto cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBR->setPosition({winSize.width,0});
    cornerBR->setAnchorPoint({1,0});
    cornerBR->setFlipX(true);
    cornerBR->setID("right-corner"_spr);
    addChild(cornerBR, -1);

    //more filters menu
    auto moreFilters = CCMenu::create();
    moreFilters->setPosition({winSize.width, winSize.height});
    moreFilters->setID("other-filter-menu"_spr);

    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    resetSpr->setScale(0.8f);
    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetSpr,
        this,
        menu_selector(ListsSearchLayer::onReset)
    );
    resetBtn->setPosition({-25, -25});
    resetBtn->setID("clear-filters-button"_spr);
    moreFilters->addChild(resetBtn);
    
    auto plusSpr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    plusSpr->setScale(0.8f);
    auto plusBtn = CCMenuItemSpriteExtra::create(
        plusSpr,
        this,
        menu_selector(ListsSearchLayer::onAdditional)
    );
    plusBtn->setPosition({-25, -75});
    plusBtn->setID("advanced-filters-button"_spr);
    moreFilters->addChild(plusBtn);

    addChild(moreFilters);

    //disclaimer
    if(Mod::get()->getSettingValue<bool>("disclaimer")) {
        auto disclaimer = FLAlertLayer::create(nullptr, "Disclaimer", "The lists displayed by this mod contain testing data created by RobTop. While these are real lists from 2.2, this doesn't mean that everyone who has made a list has access to this version. <cr>Some people who have created lists do NOT have 2.2</c>. Please do not go out of your way to harass any of these users about any 2.2-related inquiries. You can disable this disclaimer in the mod settings.", "OK", nullptr, 360);
        disclaimer->m_scene = this;
        disclaimer->show();
    }
    
    return true;
    
}

CCMenuItemSpriteExtra* ListsSearchLayer::createDiffButton(int i) {
    auto diffSprite = CCSprite::createWithSpriteFrameName(getDemonDifficultyIcon(i).c_str());
    diffSprite->setScale(.8f);
    auto diffBtn = CCMenuItemSpriteExtra::create(
        diffSprite,
        this,
        menu_selector(ListsSearchLayer::onDifficulty)
    );
    diffBtn->setTag(i);
    diffBtn->setContentSize({diffBtn->getContentSize().width, diffBtn->getContentSize().height > 40.f ? 40.0f : diffBtn->getContentSize().height});
    if(m_diff != i) diffBtn->setColor({125,125,125});
    m_filterMenu->addChild(diffBtn);
    return diffBtn;
}

void ListsSearchLayer::renderFilters() {
    if(m_filterMenu) m_filterMenu->removeFromParentAndCleanup(true);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    //difficulties
    m_filterMenu = CCMenu::create();
    for(int i = -1; i <= 10; i++) {
        if(i == 0 || i == 6) continue;
        auto btn = createDiffButton(i != 0 ? i : -3);

        if(i == 5) {
            createDiffButton(-2);
            createDiffButton(-3);
        }

        if(i == 8) createDiffButton(6);
    }

    m_filterMenu->setContentSize({342,89});
    m_filterMenu->setLayout(RowLayout::create()->setGrowCrossAxis(true)->setCrossAxisOverflow(true)->setGap(13.f)->setAutoScale(false));
    m_filterMenu->updateLayout();
    m_filterMenu->setPosition({winSize.width / 2, (winSize.height / 2) - 24 - 73});
    m_filterMenu->setID("filters-menu"_spr);
    addChild(m_filterMenu);
}

void ListsSearchLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void ListsSearchLayer::onBack(CCObject* object) {
    keyBackClicked();
}

void ListsSearchLayer::onReset(CCObject* object) {
    FLAlertLayer::create(this, "Clear Filters", "Do you want to <cr>reset</c> all search <cy>filters</c>?", "NO", "YES", 300)->show();
}

void ListsSearchLayer::onAdditional(CCObject* object) {
    MoreListSearchOptions::create()->show();
}

void ListsSearchLayer::onDifficulty(CCObject* object) {
    if(m_diff == object->getTag()) m_diff = -4;
    else m_diff = object->getTag();

    Mod::get()->setSavedValue("difficulty", m_diff);

    renderFilters();
}

void ListsSearchLayer::onSearch(CCObject* object) {    
    ListSearchObject obj;
    obj.m_type = object->getTag();
    if(obj.m_type == 0 || obj.m_type == 5) obj.m_str = m_textNode->getString();
    if(m_diff != -4) obj.m_diff = m_diff;
    obj.m_star = Mod::get()->getSavedValue<bool>("star");

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

void ListsSearchLayer::FLAlert_Clicked(FLAlertLayer*, bool btn) {
    if(btn) {
        Mod::get()->setSavedValue("star", false);
        Mod::get()->setSavedValue("difficulty", 0);
        m_diff = -4;
        renderFilters();
    }
}
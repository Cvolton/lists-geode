#include "ListsSearchLayer.h"
#include "ListsViewLayer.h"
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

    auto recentBtn = createButton("GJ_longBtn03_001.png", "Recent", "GJ_sRecentIcon_001.png", 4, 0.5f);
    recentBtn->setPosition({-90.5f, 28.f + btnOffset});
    recentBtn->setID("recent"_spr);

    auto followedBtn = createButton("GJ_longBtn03_001.png", "Followed", "GJ_sFollowedIcon_001.png", 12, 0.5f);
    followedBtn->setID("followed"_spr);
    followedBtn->setPosition({90.5f, 28.f + btnOffset});

    auto featuredBtn = createButton("GJ_longBtn03_001.png", "Featured", "GJ_sStarsIcon_001.png", 6, 0.5f);
    featuredBtn->setPosition({-90.5f, -8.f + btnOffset});
    featuredBtn->setID("recent"_spr);

    auto friendsBtn = createButton("GJ_longBtn03_001.png", "Friends", "GJ_sFriendsIcon_001.png", 13, 0.5f);
    friendsBtn->setPosition({90.5f, -8.f + btnOffset});
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
    m_menu->addChild(searchBtn);

    m_textNode = CCTextInputNode::create(194, 50, "Enter a list, list ID or account ID", "bigFont.fnt");
    m_textNode->setLabelPlaceholderColor({108, 153, 216});
    m_textNode->setAllowedChars(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    m_textNode->setMaxLabelScale(0.7f);
    m_textNode->setMaxLabelWidth(50);
    m_textNode->m_textField->setAnchorPoint({ .0f, .5f });
    m_textNode->m_placeholderLabel->setAnchorPoint({ .0f, .5f });
    m_textNode->setPosition({winSize.width / 2 - 73 - 101, (winSize.height / 2) + 130});
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
    addChild(cornerBL, -1);

    auto cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBR->setPosition({winSize.width,0});
    cornerBR->setAnchorPoint({0,0});
    cornerBR->setRotation(270);
    addChild(cornerBR, -1);
    
    return true;
    
}

void ListsSearchLayer::renderFilters() {
    if(m_filterMenu) m_filterMenu->removeFromParentAndCleanup(true);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    //difficulties
    m_filterMenu = CCMenu::create();
    for(int i = -1; i <= 10; i++) {
        auto diffSprite = CCSprite::createWithSpriteFrameName(getDifficultyIcon(i).c_str());
        diffSprite->setScale(.8f);
        auto diffBtn = CCMenuItemSpriteExtra::create(
            diffSprite,
            this,
            menu_selector(ListsSearchLayer::onDifficulty)
        );
        diffBtn->setTag(i);
        if(m_diff != i) diffBtn->setColor({125,125,125});
        m_filterMenu->addChild(diffBtn);
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

void ListsSearchLayer::onDifficulty(CCObject* object) {
    if(m_diff == object->getTag()) m_diff = -2;
    else m_diff = object->getTag();

    renderFilters();
}

void ListsSearchLayer::onSearch(CCObject* object) {    
    ListSearchObject obj;
    obj.m_type = object->getTag();
    if(obj.m_type == 0 || obj.m_type == 5) obj.m_str = m_textNode->getString();

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
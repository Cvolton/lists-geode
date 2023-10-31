#include "ListsViewLayer.h"
#include "../objects/list.hpp"

#include <Geode/utils/web.hpp>

std::unordered_map<int, std::string> ListsViewLayer::responseToMap(const std::string& response){
    std::unordered_map<int, std::string> map;

    std::stringstream responseStream(response);
    std::string currentKey;
    std::string keyID;

    unsigned int i = 0;
    while(getline(responseStream, currentKey, ':')){

        if(i % 2 == 0) keyID = currentKey;
        else map[std::stoi(keyID)] = currentKey;

        i++;
    }

    return map;
}

void ListsViewLayer::responseToVector(const std::string& response){
    std::stringstream responseStream(response);
    std::string currentKey;

    while(getline(responseStream, currentKey, '|')){
        auto list = List(responseToMap(currentKey));
        m_lists.push_back(list);
    }
}

void ListsViewLayer::storeUsernames(const std::string& response){
    std::stringstream responseStream(response);
    std::string currentKey;

    while(getline(responseStream, currentKey, '|')){
        std::stringstream keyStream(currentKey);
        std::string currentPart;
        try {
            getline(keyStream, currentPart, ':');
            int userID = std::stoi(currentPart);
            getline(keyStream, currentPart, ':');
            std::string userName = currentPart;
            getline(keyStream, currentPart, ':');
            int accountID = std::stoi(currentPart);

            GameLevelManager::sharedState()->storeUserName(userID, accountID, userName);
        } catch(std::exception e) {
            log::error("failed to parse username: {}", e.what());
        }
    }
}

void ListsViewLayer::parsePageData(const std::string& response) {
    std::stringstream responseStream(response);
    std::string currentKey;

    getline(responseStream, currentKey, ':');
    m_total = std::stoi(currentKey);
    getline(responseStream, currentKey, ':');
    m_offset = std::stoi(currentKey);
    getline(responseStream, currentKey, ':');
    m_thisPage = std::stoi(currentKey);
}

ListsViewLayer* ListsViewLayer::create(ListSearchObject obj) {
    auto ret = new ListsViewLayer();
    if (ret && ret->init(obj)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool ListsViewLayer::init(ListSearchObject obj) {
    m_obj = obj;

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
        menu_selector(ListsViewLayer::onBack)
    );

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);

    //counter
    m_counter = CCLabelBMFont::create("0 to 0 of 0", "goldFont.fnt");
    m_counter->setAnchorPoint({ 1.f, 1.f });
    m_counter->setPosition(winSize - CCPoint(7,3));
    m_counter->setScale(0.5f);
    addChild(m_counter);

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

    //page arrows
    auto menu = CCMenu::create();
    addChild(menu);

    auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    m_prevBtn = CCMenuItemSpriteExtra::create(
        prevSprite,
        this,
        menu_selector(ListsViewLayer::onPrev)
    );
    m_prevBtn->setPosition({- (winSize.width / 2) + 25, 0});
    menu->addChild(m_prevBtn);

    auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextSprite->setFlipX(true);
    m_nextBtn = CCMenuItemSpriteExtra::create(
        nextSprite,
        this,
        menu_selector(ListsViewLayer::onNext)
    );
    m_nextBtn->setPosition({+ (winSize.width / 2) - 25, 0});
    menu->addChild(m_nextBtn);

    loadLists();

    setKeypadEnabled(true);
    return true;
}

void ListsViewLayer::loadLists() {
    m_lists.clear();
    m_counter->setVisible(false);
    renderList();

    if(m_circle) m_circle->fadeAndRemove();
    m_circle = LoadingCircle::create();
    m_circle->setParentLayer(this);
    m_circle->show();

    web::AsyncWebRequest()
        .userAgent("")
        .postRequest()
        .postFields(m_obj.postRequest())
        .fetch("http://www.boomlings.com/database/getGJLevelLists.php")
        .text()
        .then([this](const std::string& response) {
            if(m_circle) {
                m_circle->fadeAndRemove();
                m_circle = nullptr;
            }
            if(response == "-1") return;

            std::stringstream responseStream(response);
            std::string part;
            
            getline(responseStream, part, '#');
            responseToVector(part);

            renderList();

            getline(responseStream, part, '#'); //user data
            storeUsernames(part);

            getline(responseStream, part, '#'); //page data
            parsePageData(part);

            m_counter->setCString(fmt::format("{} to {} of {}", m_offset + 1, m_offset + m_thisPage, m_total).c_str());
            m_counter->setVisible(true);
        });
}

void ListsViewLayer::renderList() {
    m_prevBtn->setVisible(m_obj.m_page > 0);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(m_listLayer != nullptr) m_listLayer->removeFromParentAndCleanup(true);

    m_nextBtn->setVisible(m_lists.size() > 0);

    CCArray* displayedLevels = CCArray::create();
    for(auto& list : m_lists) {
        auto node = CCNode::create();
        node->setUserData(&list);
        displayedLevels->addObject(node);
    }

    m_listView = ListsViewListView::create(displayedLevels, 356.f, 220.f);
    m_listLayer = GJListLayer::create(m_listView, "Lists", {191, 114, 62, 255}, 356.f, 220.f);
    m_listLayer->setPosition(winSize / 2 - m_listLayer->getScaledContentSize() / 2 - CCPoint(0,5));
    addChild(m_listLayer);
}

void ListsViewLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void ListsViewLayer::keyDown(enumKeyCodes key){
    switch(key){
        case KEY_Left:
        case CONTROLLER_Left:
            if(m_prevBtn->isVisible() == true) onPrev(nullptr);
            break;
        case KEY_Right:
        case CONTROLLER_Right:
            if(m_nextBtn->isVisible() == true) onNext(nullptr);
            break;
        default:
            CCLayer::keyDown(key);
    }
}

void ListsViewLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* ListsViewLayer::scene(ListSearchObject obj) {
    auto layer = ListsViewLayer::create(obj);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void ListsViewLayer::onPrev(CCObject* object) {
    if(m_obj.m_page <= 0) return;
    m_obj.m_page -= 1;
    loadLists();
}

void ListsViewLayer::onNext(CCObject* object) {
    m_obj.m_page += 1;
    loadLists();
}
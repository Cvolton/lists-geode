#include "ListCell.h"
#include "../ListInfoLayer.h"
#include "../../utils.hpp"

CCMenuItemSpriteExtra* ListCell::createButton(CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, float width, float height, float scale){
    auto buttonSprite = ButtonSprite::create(text, width, true, "bigFont.fnt", "GJ_button_01.png", height, scale);
    auto buttonButton = CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        handler
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({x,y});
    menu->addChild(buttonButton);

    return buttonButton;
}

void ListCell::loadFromWrapper(CCNode* wrapper) {
    this->m_list = *(static_cast<List*>(wrapper->getUserData()));

    auto levelVector = m_list.getLevelIDs();
    auto completedVector = m_list.getCompletedLevelIDs();

    auto GLM = GameLevelManager::sharedState();
    auto GSM = GameStatsManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto diffSprite = CCSprite::createWithSpriteFrameName(getDifficultyIcon(m_list.m_difficulty).c_str());
    diffSprite->setPosition({22.f, 28.f});
    diffSprite->setScale(0.8f);
    diffSprite->setZOrder(1);
    this->m_mainLayer->addChild(diffSprite);

    auto featureSprite = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
    featureSprite->setPosition({22.f, 28.f});
    featureSprite->setScale(0.8f);
    this->m_mainLayer->addChild(featureSprite);
    featureSprite->setVisible(m_list.m_featuredIdx > 0);

    auto title = CCLabelBMFont::create(m_list.m_name.c_str(), "bigFont.fnt");
    title->setAnchorPoint({ 0.0f, .5f });
    title->setPosition(43.0f, 42.f);
    title->limitLabelWidth(170, .625f, .3f);
    this->m_mainLayer->addChild(title);

    auto userName = CCLabelBMFont::create(fmt::format("By {}", m_list.m_userName).c_str(), "goldFont.fnt");
    userName->setAnchorPoint({ 0.0f, .5f });
    userName->setPosition(43.0f + ((title->getContentSize().width) * title->getScaleX()) + 7.f, 42.f);
    userName->limitLabelWidth(50, .5f, .35f);
    this->m_mainLayer->addChild(userName);

    //progress bar
    auto bar = CCSprite::create("GJ_progressBar_001.png");
    bar->setScale(0.6f);
    bar->setPosition({147.f, 23.5f});
    bar->setColor({0, 0, 0});
    bar->setOpacity(125);

    this->m_mainLayer->addChild(bar);

    float size = (float) completedVector.size() / (float) levelVector.size();

    auto status = CCLabelBMFont::create(fmt::format("{}/{}", completedVector.size(), levelVector.size()).c_str(), "bigFont.fnt");
    status->setPosition({147.f, 24.5f});
    status->limitLabelWidth(170, .7f, .3f);
    status->setScale(0.5f);
    this->m_mainLayer->addChild(status);

    auto progress = CCSprite::create("GJ_progressBar_001.png");
    //progress->setColor({0x47,0xaa,0xe8});
    progress->setColor({0,255,0});
    progress->setScaleX(0.985f);
    progress->setScaleY(0.830f);
    progress->setOpacity(255);
    bar->addChild(progress);
    progress->setAnchorPoint({0.f,0.5f});
    progress->setPosition({2.5f, 10.f});

    CCRect progressRect = progress->getTextureRect();
    progressRect.size.width *= size;
    progress->setTextureRect(progressRect);

    //row 2
    auto numberSprite = CCSprite::createWithSpriteFrameName("GJ_sDownloadIcon_001.png");
    numberSprite->setPosition({44, 9});
    numberSprite->setAnchorPoint({0, 0.5f});
    numberSprite->setScale(0.7f);
    this->m_mainLayer->addChild(numberSprite);

    auto number = CCLabelBMFont::create(std::to_string(m_list.m_downloads).c_str(), "bigFont.fnt");
    number->setAnchorPoint({ 0.0f, 0.5f });
    number->setPosition(55.5f, 10.0f);
    number->setScale(.325f);
    this->m_mainLayer->addChild(number);

    auto diamondSprite = CCSprite::createWithSpriteFrameName(m_list.m_likes < 0 ? "GJ_dislikesIcon_001.png" : "GJ_likesIcon_001.png");
    diamondSprite->setPosition({number->getPositionX() + ((number->getContentSize().width) * number->getScaleX()) + 5, 10});
    diamondSprite->setAnchorPoint({0, 0.5f});
    diamondSprite->setScale(0.45f);
    this->m_mainLayer->addChild(diamondSprite);

    auto diamond = CCLabelBMFont::create(std::to_string(m_list.m_likes).c_str(), "bigFont.fnt");
    diamond->setAnchorPoint({ 0.0f, 0.5f });
    diamond->setPosition(diamondSprite->getPositionX() + 13.f, 10.0f);
    diamond->setScale(.325f);
    this->m_mainLayer->addChild(diamond);
    auto menu = CCMenu::create();
    auto similarButton = createButton(menu, "View", menu_selector(ListCell::onView), 28, -132, 40.f, 25.f, 0.5f);
    m_mainLayer->addChild(menu);


    this->m_mainLayer->setVisible(true);
    this->m_backgroundLayer->setOpacity(255);
}

void ListCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

ListCell::ListCell(const char* name, CCSize size) :
    GenericListCell(name, size) {}

ListCell* ListCell::create(const char* key, CCSize size) {
    auto ret = new ListCell(key, size);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void ListCell::onView(CCObject* sender){
    //auto searchObject = GJSearchObject::create(SearchType::ListsOnClick, m_list.m_levels);
    //auto browserLayer = LevelBrowserLayer::scene(searchObject);
    auto browserLayer = ListInfoLayer::scene(m_list);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

    CCDirector::sharedDirector()->pushScene(transitionFade);
}
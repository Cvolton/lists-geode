#include "ListInfoLayer.h"
#include "ListInfo/ExtendedListInfo.h"
#include "../utils.hpp"

#include <Geode/utils/web.hpp>

ListInfoLayer* ListInfoLayer::create(List list) {
    auto ret = new ListInfoLayer();
    if (ret && ret->init(list)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool ListInfoLayer::init(List list) {
    m_list = list;

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
        menu_selector(ListInfoLayer::onBack)
    );

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);

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

    //buttons
    auto menu = CCMenu::create();
    addChild(menu);

    /*
        GJ_updateBtn_001.png
        GJ_infoBtn_001.png
        GJ_like2Btn_001.png
        GJ_duplicateBtn_001.png

    */

    auto sInfoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto sInfoBtn = CCMenuItemSpriteExtra::create(
        sInfoSprite,
        this,
        menu_selector(ListInfoLayer::onBack)
    );
    sInfoBtn->setPosition({- (winSize.width / 2) + 25, - (winSize.height / 2) + 25});
    menu->addChild(sInfoBtn);

    auto refreshSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    refreshSprite->setScale(0.9f);
    auto refreshBtn = CCMenuItemSpriteExtra::create(
        refreshSprite,
        this,
        menu_selector(ListInfoLayer::onRefresh)
    );
    refreshBtn->setPosition({+ (winSize.width / 2) - 26, + (winSize.height / 2) - 26});
    menu->addChild(refreshBtn);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSprite->setScale(0.9f);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSprite,
        this,
        menu_selector(ListInfoLayer::onInfo)
    );
    infoBtn->setPosition({+ (winSize.width / 2) - 26, + (winSize.height / 2) - 71});
    menu->addChild(infoBtn);

    auto likeSprite = CCSprite::createWithSpriteFrameName("GJ_like2Btn_001.png");
    likeSprite->setScale(0.9f);
    auto likeBtn = CCMenuItemSpriteExtra::create(
        likeSprite,
        this,
        menu_selector(ListInfoLayer::onLike)
    );
    likeBtn->setPosition({+ (winSize.width / 2) - 26, + (winSize.height / 2) - 116});
    menu->addChild(likeBtn);

    //top header
    auto diffSprite = CCSprite::createWithSpriteFrameName(getDifficultyIcon(m_list.m_difficulty).c_str());
    diffSprite->setPosition({(winSize.width / 2) - 160, (winSize.height / 2) + 140});
    diffSprite->setScale(0.8f);
    diffSprite->setZOrder(1);
    addChild(diffSprite);

    /*CCRect diffSpriteRect = diffSprite->getTextureRect();
    diffSpriteRect.size.height -= 12;
    diffSprite->setTextureRect(diffSpriteRect);*/

    auto title = CCLabelBMFont::create(m_list.m_name.c_str(), "bigFont.fnt");
    title->setAnchorPoint({ 0.0f, .5f });
    title->setPosition({(winSize.width / 2) - 140, (winSize.height / 2) + 140});
    title->limitLabelWidth(240, .8, .3f);
    addChild(title);

    auto userName = CCLabelBMFont::create(fmt::format("{}", m_list.m_userName).c_str(), "goldFont.fnt");
    userName->setAnchorPoint({ 0.0f, .5f });
    userName->setPosition((winSize.width / 2) - 140 + ((title->getContentSize().width) * title->getScaleX()) + 7.f, (winSize.height / 2) + 140);
    userName->limitLabelWidth(70, .8f, .35f);
    addChild(userName);

    //dl and likes
    auto numberSprite = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    numberSprite->setPosition({25, winSize.height - 70});
    numberSprite->setScale(.9f);
    addChild(numberSprite);

    auto number = CCLabelBMFont::create(std::to_string(m_list.m_downloads).c_str(), "bigFont.fnt");
    number->setPosition({25, winSize.height - 70 - 20});
    number->setScale(.4f);
    addChild(number);

    auto likesSprite = CCSprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
    likesSprite->setPosition({25, winSize.height - 70 - 50});
    likesSprite->setScale(.9f);
    addChild(likesSprite);

    auto likesText = CCLabelBMFont::create(std::to_string(m_list.m_likes).c_str(), "bigFont.fnt");
    likesText->setPosition({25, winSize.height - 70 - 70});
    likesText->setScale(.4f);
    addChild(likesText);

    renderList();
    setupProgressBars();
    loadList();

    setKeypadEnabled(true);
    return true;
}

void ListInfoLayer::renderList() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(m_listLayer != nullptr) m_listLayer->removeFromParentAndCleanup(true);
    if(m_levels == nullptr) {
        m_levels = CCArray::create();
        m_levels->retain();
    }

    if(m_circle) {
        m_circle->fadeAndRemove();
        m_circle = nullptr;
    }

    m_listView = CustomListView::create(m_levels, BoomListType::Level, 220.f, 356.f);
    m_listLayer = GJListLayer::create(m_listView, nullptr, {191, 114, 62, 255}, 356.f, 220.f);
    m_listLayer->setPosition(winSize / 2 - m_listLayer->getScaledContentSize() / 2 - CCPoint(0,5));
    addChild(m_listLayer);
}

void ListInfoLayer::loadList() {
    auto GLM = GameLevelManager::sharedState();
    GLM->m_onlineListDelegate = this;
    GLM->getOnlineLevels(GJSearchObject::create((SearchType) 26, m_list.m_levels));

    m_circle = LoadingCircle::create();
    m_circle->setParentLayer(this);
    m_circle->show();
}

void ListInfoLayer::setupProgressBars() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto levelVector = m_list.getLevelIDs();
    auto completedVector = m_list.getCompletedLevelIDs();

    //progress bar
    auto bar = CCSprite::create("GJ_progressBar_001.png");
    bar->setScale(0.7f);
    bar->setPosition({winSize.width / 2, 12});
    bar->setColor({0, 0, 0});
    bar->setOpacity(125);
    bar->setZOrder(1);

    addChild(bar);

    float size = (float) completedVector.size() / (float) levelVector.size();

    auto status = CCLabelBMFont::create(fmt::format("{}/{}", completedVector.size(), levelVector.size()).c_str(), "bigFont.fnt");
    status->setPosition({winSize.width / 2, 13});
    status->limitLabelWidth(170, .7f, .3f);
    status->setScale(0.5f);
    status->setZOrder(2);
    addChild(status);

    auto progress = CCSprite::create("GJ_progressBar_001.png");
    progress->setColor({0x47,0xaa,0xe8});
    //progress->setColor({0,255,0});
    progress->setScaleX(0.92f);
    progress->setScaleY(0.860f);
    progress->setOpacity(255);
    bar->addChild(progress);
    progress->setAnchorPoint({0.f,0.5f});
    progress->setPosition({2.5f, 10.f});

    CCRect progressRect = progress->getTextureRect();
    progressRect.size.width *= size;
    progress->setTextureRect(progressRect);
}

void ListInfoLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void ListInfoLayer::onBack(CCObject* listect) {
    keyBackClicked();
}

void ListInfoLayer::onRefresh(CCObject* listect) {
    loadList();
}

void ListInfoLayer::onInfo(CCObject* listect) {
    ExtendedListInfo::create(m_list)->show();
}

void ListInfoLayer::onLike(CCObject* listect) {
    LikeItemLayer::create((LikeItemType) 4, m_list.m_id, 0)->show();
}

CCScene* ListInfoLayer::scene(List list) {
    auto layer = ListInfoLayer::create(list);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void ListInfoLayer::loadListFinished(cocos2d::CCArray* levels, const char*) {
    if(m_levels) m_levels->release();
    m_levels = levels;
    m_levels->retain();

    renderList();
}

void ListInfoLayer::loadListFailed(const char*) {}
void ListInfoLayer::setupPageInfo(gd::string, const char*) {}
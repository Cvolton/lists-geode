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
    cornerBR->setAnchorPoint({1,0});
    cornerBR->setFlipX(true);
    addChild(cornerBR, -1);

    //buttons
    m_menu = CCMenu::create();
    addChild(m_menu);

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
    m_menu->addChild(sInfoBtn);

    auto refreshSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    refreshSprite->setScale(0.9f);
    auto refreshBtn = CCMenuItemSpriteExtra::create(
        refreshSprite,
        this,
        menu_selector(ListInfoLayer::onRefresh)
    );
    refreshBtn->setPosition({+ (winSize.width / 2) - 26, + (winSize.height / 2) - 26});
    m_menu->addChild(refreshBtn);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSprite->setScale(0.9f);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSprite,
        this,
        menu_selector(ListInfoLayer::onInfo)
    );
    infoBtn->setPosition({+ (winSize.width / 2) - 26, + (winSize.height / 2) - 71});
    m_menu->addChild(infoBtn);

    reloadMetadata();

    //top header
    auto node = CCMenu::create();
    addChild(node);

    /*CCRect diffSpriteRect = diffSprite->getTextureRect();
    diffSpriteRect.size.height -= 12;
    diffSprite->setTextureRect(diffSpriteRect);*/

    auto diffSprite = CCSprite::createWithSpriteFrameName(Mod::get()->expandSpriteName(getDifficultyIcon(m_list.m_difficulty).c_str()));
    //diffSprite->setPosition({(winSize.width / 2) - 160, (winSize.height / 2) + 140});
    diffSprite->setScale(0.8f);
    //diffSprite->setZOrder(1);
    node->addChild(diffSprite);

    if(m_list.m_featuredIdx) {
        auto featureSprite = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
        featureSprite->setScale(1.f);
        featureSprite->setZOrder(-1);
        featureSprite->setPosition({diffSprite->getContentSize().width / 2, 0.f});
        featureSprite->setAnchorPoint({0.5, 0.325});
        diffSprite->addChild(featureSprite);
    }

    auto title = CCLabelBMFont::create(m_list.m_name.c_str(), "bigFont.fnt");
    title->setAnchorPoint({ 0.0f, .5f });
    //title->setPosition({(winSize.width / 2) - 140, (winSize.height / 2) + 140});
    title->limitLabelWidth(240, .8, .3f);
    node->addChild(title);

    auto userName = CCLabelBMFont::create(fmt::format("{}", m_list.m_userName).c_str(), "goldFont.fnt");
    userName->setAnchorPoint({ 0.0f, .5f });
    //userName->setPosition((winSize.width / 2) - 140 + ((title->getContentSize().width) * title->getScaleX()) + 7.f, (winSize.height / 2) + 140);
    userName->limitLabelWidth(70, .8f, .35f);
    auto userNameBtn = CCMenuItemSpriteExtra::create(
        userName,
        this,
        menu_selector(ListInfoLayer::onMore)
    );
    node->addChild(userNameBtn);
    
    node->setContentSize({400,30});
    node->setAnchorPoint({0.5,0.5});
    node->setPosition(winSize.width / 2, (winSize.height) - 20);
    node->setLayout(RowLayout::create()->setAutoScale(false)->setGrowCrossAxis(false)->setCrossAxisOverflow(true)->setGap(10.f));
    node->updateLayout();

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
    progress->setScaleX(0.985f);
    progress->setScaleY(0.860f);
    progress->setOpacity(255);
    bar->addChild(progress);
    progress->setAnchorPoint({0.f,0.5f});
    progress->setPosition({2.5f, 10.f});

    CCRect progressRect = progress->getTextureRect();
    progressRect.size.width *= size;
    progress->setTextureRect(progressRect);
}

void ListInfoLayer::reloadMetadata() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto hasLiked = GameLevelManager::sharedState()->hasLikedItemFullCheck((LikeItemType) 4, m_list.m_id, 0);

    //like btn
    if(m_likeBtn) m_likeBtn->removeFromParentAndCleanup(true);

    auto likeSprite = CCSprite::createWithSpriteFrameName(hasLiked ? "GJ_like2Btn2_001.png" : "GJ_like2Btn_001.png");
    likeSprite->setScale(0.9f);
    m_likeBtn = CCMenuItemSpriteExtra::create(
        likeSprite,
        this,
        menu_selector(ListInfoLayer::onLike)
    );
    m_likeBtn->setPosition({+ (winSize.width / 2) - 26, + (winSize.height / 2) - 116});
    m_menu->addChild(m_likeBtn);

    if(hasLiked) m_likeBtn->setEnabled(false);

    //dl and likes
    if(m_downloadsIcon) m_downloadsIcon->removeFromParentAndCleanup(true);
    if(m_downloadsText) m_downloadsText->removeFromParentAndCleanup(true);
    if(m_likesIcon) m_likesIcon->removeFromParentAndCleanup(true);
    if(m_likesText) m_likesText->removeFromParentAndCleanup(true);

    m_downloadsIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    m_downloadsIcon->setPosition({25, winSize.height - 70});
    m_downloadsIcon->setScale(.9f);
    addChild(m_downloadsIcon);

    m_downloadsText = CCLabelBMFont::create(std::to_string(m_list.m_downloads).c_str(), "bigFont.fnt");
    m_downloadsText->setPosition({25, winSize.height - 70 - 20});
    m_downloadsText->setScale(.4f);
    addChild(m_downloadsText);

    m_likesIcon = CCSprite::createWithSpriteFrameName(m_list.m_likes < 0 ? "GJ_dislikesIcon_001.png" : "GJ_likesIcon_001.png");
    m_likesIcon->setPosition({25, winSize.height - 70 - 50 + (m_list.m_likes < 0 ? 4 : 0)});
    m_likesIcon->setScale(.9f);
    addChild(m_likesIcon);

    m_likesText = CCLabelBMFont::create(std::to_string(m_list.m_likes).c_str(), "bigFont.fnt");
    m_likesText->setPosition({25, winSize.height - 70 - 70});
    m_likesText->setScale(.4f);
    addChild(m_likesText);
}

void ListInfoLayer::keyBackClicked() {
    GameLevelManager::sharedState()->m_onlineListDelegate = nullptr;
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
    auto layer = LikeItemLayer::create((LikeItemType) 4, m_list.m_id, 0);
    layer->m_likeDelegate = this;
    layer->show();
}

void ListInfoLayer::onMore(CCObject* listect) {
    ProfilePage::create(m_list.m_accountID, false)->show();
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

void ListInfoLayer::likedItem(LikeItemType type, int id, bool liked) {
    liked ? m_list.m_likes += 1 : m_list.m_likes -= 1;
    reloadMetadata();
}
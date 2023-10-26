#include "ListCell.h"

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

std::string getDifficultyIcon(int diff){
    if(diff == -1) return "difficulty_00_btn_001.png";
    if(diff == 0) return "difficulty_auto_btn_001.png";
    if(diff <= 10) return fmt::format("difficulty_{:02}_btn_001.png", diff);
    return "difficulty_01_btn_001.png";
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
    //infoSprite->setScale(0.7f);

    /*auto starsLabel = CCLabelBMFont::create(std::to_string(level->m_stars).c_str(), "bigFont.fnt");
    starsLabel->setPosition({22, 8});
    starsLabel->setScale(.34f);
    starsLabel->setAnchorPoint({1, 0.5f});
    this->m_mainLayer->addChild(starsLabel);

    auto starsSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
    starsSprite->setPosition({23, 8});
    starsSprite->setAnchorPoint({0, 0.5f});
    starsSprite->setScale(0.8f);
    this->m_mainLayer->addChild(starsSprite);*/

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

    auto diamondSprite = CCSprite::createWithSpriteFrameName("GJ_sLikeIcon_001.png");
    diamondSprite->setPosition({number->getPositionX() + ((number->getContentSize().width) * number->getScaleX()) + 5, 9});
    diamondSprite->setAnchorPoint({0, 0.5f});
    diamondSprite->setScale(0.7f);
    this->m_mainLayer->addChild(diamondSprite);

    auto diamond = CCLabelBMFont::create(std::to_string(m_list.m_likes).c_str(), "bigFont.fnt");
    diamond->setAnchorPoint({ 0.0f, 0.5f });
    diamond->setPosition(diamondSprite->getPositionX() + 13.f, 10.0f);
    diamond->setScale(.325f);
    this->m_mainLayer->addChild(diamond);

    /*auto orbSprite = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
    orbSprite->setPosition({diamond->getPositionX() + ((diamond->getContentSize().width) * diamond->getScaleX()) + 5, 9});
    orbSprite->setAnchorPoint({0, 0.5f});
    orbSprite->setScale(0.5f);
    this->m_mainLayer->addChild(orbSprite);

    int orbsMax = (GSM->getAwardedCurrencyForLevel(level) * 125) / 100;
    int orbsCollectible = GSM->getBaseCurrencyForLevel(level);
    auto orb = CCLabelBMFont::create(CCString::createWithFormat("%i/%i", orbsCollectible, orbsMax)->getCString(), "bigFont.fnt");
    if(orbsCollectible == orbsMax){ 
        orb = CCLabelBMFont::create(CCString::createWithFormat("%i", orbsCollectible)->getCString(), "bigFont.fnt");
        orb->setColor({100, 255, 255});
    }
    orb->setAnchorPoint({ 0.0f, 0.5f });
    orb->setPosition(orbSprite->getPositionX() + 11.5f, 10.0f);
    orb->setScale(.325f);
    this->m_mainLayer->addChild(orb);

    //row 1
    auto percentSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    percentSprite->setFlipX(true);
    percentSprite->setPosition({44, 23});
    percentSprite->setAnchorPoint({0, 0.5f});
    percentSprite->setScale(0.35f);
    this->m_mainLayer->addChild(percentSprite);

    auto percent = CCLabelBMFont::create(CCString::createWithFormat("%i%%", level->m_normalPercent.value())->getCString(), "bigFont.fnt");
    percent->setAnchorPoint({ 0.0f, 0.5f });
    percent->setPosition(57.5f, 24.0f);
    percent->setScale(.325f);
    if(level->m_normalPercent == 100) percent->setColor({255,255,128});
    this->m_mainLayer->addChild(percent);

    auto practiceSprite = CCSprite::createWithSpriteFrameName("checkpoint_01_001.png");
    practiceSprite->setPosition({percent->getPositionX() + ((percent->getContentSize().width) * percent->getScaleX()) + 5, 23});
    practiceSprite->setAnchorPoint({0, 0.5f});
    practiceSprite->setScale(0.35f);
    this->m_mainLayer->addChild(practiceSprite);

    auto practice = CCLabelBMFont::create(CCString::createWithFormat("%i%%", level->m_practicePercent)->getCString(), "bigFont.fnt");
    practice->setAnchorPoint({ 0.0f, 0.5f });
    practice->setPosition(practiceSprite->getPositionX() + 8.f, 24.0f);
    practice->setScale(.325f);
    if(level->m_practicePercent == 100) practice->setColor({255,255,128});
    this->m_mainLayer->addChild(practice);

    auto coinDict = GameStatsManager::sharedState()->m_verifiedUserCoins;

    float coinX = practice->getPositionX() + ((practice->getContentSize().width) * practice->getScaleX());
    for(int i = 1; i <= biCache->getCoinCount(level->m_levelID); i++){
        bool isCollected = coinDict->objectForKey(level->getCoinKey(i)) == nullptr;
        auto coinSprite = CCSprite::createWithSpriteFrameName("usercoin_small01_001.png");
        if(isCollected) coinSprite->setColor({165, 165, 165});
        coinSprite->setPosition({coinX += 6, 23});
        coinSprite->setAnchorPoint({0, 0.5f});
        coinSprite->setScale(0.7f);
        //coinSprite->setScale(0.35f);
        this->m_mainLayer->addChild(coinSprite);
    }

    //fix level data (deleted from saved)
    level->m_levelName = biCache->getLevelName(level->m_levelID);
    level->m_coins = biCache->getCoinCount(level->m_levelID);
    level->m_coinsVerified = true;

    //buttons
    auto menu = CCMenu::create();

    auto viewSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    viewSprite->setScale(.5f);
    auto viewBtn = CCMenuItemSpriteExtra::create(
        viewSprite,
        this,
        menu_selector(ListCell::onView)
    );
    //viewBtn->setPosition({42, -132});
    viewBtn->setPosition(CCPoint(327, 28) - (winSize / 2));
    menu->addChild(viewBtn);

    auto leaderboardSprite = CCSprite::createWithSpriteFrameName("GJ_levelLeaderboardBtn_001.png");
    leaderboardSprite->setScale(.85f);
    auto leaderboardBtn = CCMenuItemSpriteExtra::create(
        leaderboardSprite,
        this,
        menu_selector(ListCell::onLeaderboards)
    );
    leaderboardBtn->setPosition(viewBtn->getPosition() - CCPoint(40, 0));
    menu->addChild(leaderboardBtn);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSprite->setScale(.85f);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSprite,
        this,
        menu_selector(ListCell::onInfo)
    );
    infoBtn->setPosition(leaderboardBtn->getPosition() - CCPoint(40, 0));
    menu->addChild(infoBtn);

    this->m_mainLayer->addChild(menu);*/
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
    auto searchObject = GJSearchObject::create(SearchType::ListsOnClick, m_list.m_levels);
    auto browserLayer = LevelBrowserLayer::scene(searchObject);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

    CCDirector::sharedDirector()->pushScene(transitionFade);
}
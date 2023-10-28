#include "ExtendedListInfo.h"
#include "../../utils.hpp"

#include <string>

ExtendedListInfo* ExtendedListInfo::create(List list){
    auto ret = new ExtendedListInfo();
    if (ret && ret->init(list)) {
        //robert 1 :D
        ret->autorelease();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void ExtendedListInfo::onClose(cocos2d::CCObject* sender)
{
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void ExtendedListInfo::onCopyName(cocos2d::CCObject* sender)
{
    utils::clipboard::write(m_list.m_name);
    Notification::create("Copied to Clipboard")->show();
}

void ExtendedListInfo::onCopyAuthor(cocos2d::CCObject* sender)
{
    utils::clipboard::write(m_list.m_userName);
    Notification::create("Copied to Clipboard")->show();
}

void ExtendedListInfo::onCopyDesc(cocos2d::CCObject* sender)
{
    utils::clipboard::write(m_list.getDescription());
    Notification::create("Copied to Clipboard")->show();
}

void ExtendedListInfo::onCopyID(cocos2d::CCObject* sender)
{
    utils::clipboard::write(std::to_string(m_list.m_id));
    Notification::create("Copied to Clipboard")->show();
}

void ExtendedListInfo::onCopyVersion(cocos2d::CCObject* sender)
{
    utils::clipboard::write(std::to_string(m_list.m_version));
    Notification::create("Copied to Clipboard")->show();
}

void ExtendedListInfo::loadPage(int page) {
    m_info->setString(m_primary);
}

std::string ExtendedListInfo::zeroIfNA(int value){
    if(value == 0) return "NA";
    return std::to_string(value);
}

inline std::string timeZeroIfNA(int value){
    if(value == 0) return "NA";
    return timeToString(value);
}

void ExtendedListInfo::refreshInfoTexts() {
    std::ostringstream infoText;
    infoText << "\n<cj>Uploaded</c>: " << timeZeroIfNA(m_list.m_uploaded)
        << "\n<cj>Updated</c>: " << timeZeroIfNA(m_list.m_updated)
        //<< "\n<cy>Stars Requested</c>: " << m_level->m_starsRequested
        << "\n<cg>Feature score</c>: " << zeroIfNA(m_list.m_featuredIdx);

    m_primary = infoText.str();
}

bool ExtendedListInfo::init(List list){
    bool init = createBasics({440.0f, 290.0f}, menu_selector(ExtendedListInfo::onClose));
    if(!init) return false;

    m_list = list;

    auto listName = CCLabelBMFont::create(m_list.m_name.c_str(), "bigFont.fnt");
    listName->limitLabelWidth(360, 1.f, 0.1f);
    auto listNameBtn = CCMenuItemSpriteExtra::create(
        listName,
        this,
        menu_selector(ExtendedListInfo::onCopyName)
    );
    m_buttonMenu->addChild(listNameBtn);
    listNameBtn->setPosition({0,125});

    std::ostringstream userNameText;
    userNameText << "By " << std::string(m_list.m_userName);
    auto userName = CCLabelBMFont::create(userNameText.str().c_str(), "goldFont.fnt");
    userName->setScale(0.8f);
    auto userNameBtn = CCMenuItemSpriteExtra::create(
        userName,
        this,
        menu_selector(ExtendedListInfo::onCopyAuthor)
    );
    userNameBtn->setPosition({0,99});
    m_buttonMenu->addChild(userNameBtn);

    cocos2d::extension::CCScale9Sprite* descBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    descBg->setContentSize({340,55});
    descBg->setColor({130,64,33});
    m_buttonMenu->addChild(descBg, -1);
    descBg->setPosition({0,52});

    auto descText = fixNullByteCrash(fixColorCrashes(m_list.getDescription()));
    size_t descLength = descText.length();
    float descDelimiter = 1;
    if(descLength > 140) descLength = 140;
    if(descLength > 70) descDelimiter = ((((140 - descLength) / 140) * 0.3f) + 0.7f);
    auto description = TextArea::create(descText, "chatFont.fnt", 1, 295 / descDelimiter, {0.5f,0.5f}, 20, false);
    description->setScale(descDelimiter);
    description->setAnchorPoint({1,1});
    description->setPosition( ( (description->getContentSize() / 2 ) * descDelimiter ) + (CCPoint(340,55) / 2) );
    auto descSprite = CCSprite::create();
    descSprite->addChild(description);
    descSprite->setContentSize({340,55});
    auto descBtn = CCMenuItemSpriteExtra::create(
        descSprite,
        this,
        menu_selector(ExtendedListInfo::onCopyDesc)
    );
    descBtn->setPosition({0,52});
    m_buttonMenu->addChild(descBtn);

    cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    infoBg->setContentSize({340,140});
    infoBg->setColor({123,60,31});
    m_buttonMenu->addChild(infoBg, -1);
    infoBg->setPosition({0,-55});

    refreshInfoTexts();

    m_info = TextArea::create(m_primary, "chatFont.fnt", 1, 340, {0,1}, 20, false);
    m_info->setPosition({-160.5,26});
    //m_info->setPosition({-160.5,10});
    m_info->setAnchorPoint({0,1});
    m_info->setScale(0.925f);
    m_buttonMenu->addChild(m_info);

    loadPage(0);

    auto listId = CCLabelBMFont::create(fmt::format("ID: {}", m_list.m_id).c_str(), "goldFont.fnt");
    listId->setScale(.6f);
    auto listIdBtn = CCMenuItemSpriteExtra::create(
        listId,
        this,
        menu_selector(ExtendedListInfo::onCopyID)
    );
    m_buttonMenu->addChild(listIdBtn);
    listIdBtn->setPosition({50,-131});

    auto listVersion = CCLabelBMFont::create(fmt::format("Version: {}", m_list.m_version).c_str(), "goldFont.fnt");
    listVersion->setScale(.6f);
    auto listVersionBtn = CCMenuItemSpriteExtra::create(
        listVersion,
        this,
        menu_selector(ExtendedListInfo::onCopyVersion)
    );
    m_buttonMenu->addChild(listVersionBtn);
    listVersionBtn->setPosition({-50,-131});

    return true;
}
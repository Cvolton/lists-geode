#include "MoreListSearchOptions.h"
#include "../../utils.hpp"

#include <string>

MoreListSearchOptions* MoreListSearchOptions::create(){
    auto ret = new MoreListSearchOptions();
    if (ret && ret->init()) {
        //robert 1 :D
        ret->autorelease();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void MoreListSearchOptions::onClose(cocos2d::CCObject* sender)
{
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void MoreListSearchOptions::onStarToggle(cocos2d::CCObject* sender)
{
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    Mod::get()->setSavedValue("star", !toggler->isToggled());
}

bool MoreListSearchOptions::init(){
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    bool init = createBasics({440.0f, 290.0f}, menu_selector(MoreListSearchOptions::onClose));
    if(!init) return false;

    auto bmFont = CCLabelBMFont::create("Advanced Search", "bigFont.fnt");
    bmFont->setPosition((winSize.width / 2), (winSize.height / 2) + 125);

    m_mainLayer->addChild(bmFont);

    auto star = Mod::get()->getSavedValue<bool>("star");

    auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(MoreListSearchOptions::onStarToggle), .8f);
    if(star) toggler->toggle(true);
    toggler->setPosition({-170, 75});
    m_buttonMenu->addChild(toggler);

    auto sprite = CCLabelBMFont::create("Star", "bigFont.fnt");
    sprite->setScale(.5f);
    sprite->setAnchorPoint({0, 0.5});
    sprite->setPosition({-150, 75});
    m_buttonMenu->addChild(sprite);

    return true;
}
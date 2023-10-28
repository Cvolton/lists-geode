#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ListsSearchLayer : public cocos2d::CCLayer {
    CCMenu* m_menu = nullptr;
    CCTextInputNode* m_textNode = nullptr;

protected:
    CCMenuItemSpriteExtra* createButton(const char* texture, const char* text, const char* icon, int searchType, float size = 0.4f);
    virtual bool init();
    virtual void keyBackClicked();
    void loadLists();
    void onBack(cocos2d::CCObject*);
    void onSearch(cocos2d::CCObject*);
public:
    static ListsSearchLayer* create();
    static cocos2d::CCScene* scene();
};
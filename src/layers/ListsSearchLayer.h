#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ListsSearchLayer : public cocos2d::CCLayer {
    CCMenu* m_menu = nullptr;
    CCMenu* m_filterMenu = nullptr;
    CCTextInputNode* m_textNode = nullptr;

    int m_diff = -4;

protected:
    CCMenuItemSpriteExtra* createButton(const char* texture, const char* text, const char* icon, int searchType, float size = 0.4f);
    virtual bool init();
    virtual void keyBackClicked();
    CCMenuItemSpriteExtra* createDiffButton(int i);
    void renderFilters();
    void onBack(cocos2d::CCObject*);
    void onSearch(cocos2d::CCObject*);
    void onDifficulty(cocos2d::CCObject*);
public:
    static ListsSearchLayer* create();
    static cocos2d::CCScene* scene();
};
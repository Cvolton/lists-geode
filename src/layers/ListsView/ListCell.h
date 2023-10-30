#pragma once

#include <Geode/Bindings.hpp>
#include "../../objects/list.hpp"

using namespace geode::prelude;

class ListCell : public GenericListCell {
        List m_list;

        CCMenuItemSpriteExtra* createButton(CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, float width, float height, float scale);
        
        void draw() override;
        void onView(cocos2d::CCObject* sender);
        void onMore(cocos2d::CCObject* sender);
    
    public:
        ListCell(const char* name, cocos2d::CCSize size);
        void loadFromWrapper(CCNode* wrapper);
        static ListCell* create(const char* key, cocos2d::CCSize size);
};
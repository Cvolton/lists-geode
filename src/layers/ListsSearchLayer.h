#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ListsSearchLayer : public cocos2d::CCLayer {
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void loadLists();
    void onBack(cocos2d::CCObject*);
    void onSearch(cocos2d::CCObject*);
public:
    static ListsSearchLayer* create();
    static cocos2d::CCScene* scene();
};
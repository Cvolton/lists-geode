#pragma once

#include <Geode/Bindings.hpp>
#include "ListsView/ListsViewListView.h"
#include "../objects/ListSearchObject.hpp"

using namespace geode::prelude;

class ListsViewLayer : public cocos2d::CCLayer {
protected:
    std::vector<List> m_lists;
    ListSearchObject m_obj;

    ListsViewListView* m_listView = nullptr;
    GJListLayer* m_listLayer = nullptr;
    CCLabelBMFont* m_counter = nullptr;
    LoadingCircle* m_circle = nullptr;

    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;

    int m_total = 0;
    int m_offset = 0;
    int m_thisPage = 0;

    std::unordered_map<int, std::string> responseToMap(const std::string& response);
    void responseToVector(const std::string& response);
    void parsePageData(const std::string& response);

    virtual bool init(ListSearchObject obj);
    virtual void keyBackClicked();
    virtual void keyDown(enumKeyCodes key);
    void loadLists();
    void renderList();
    void onBack(cocos2d::CCObject*);
    void onPrev(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
public:
    static ListsViewLayer* create(ListSearchObject obj);
    static cocos2d::CCScene* scene(ListSearchObject obj);
};
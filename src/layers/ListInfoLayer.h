#pragma once

#include <Geode/Bindings.hpp>
#include "../objects/list.hpp"

using namespace geode::prelude;

class ListInfoLayer : public cocos2d::CCLayer, public OnlineListDelegate {
protected:
    List m_list;
    CCArray* m_levels = nullptr;

    CustomListView* m_listView = nullptr;
    GJListLayer* m_listLayer = nullptr;

    virtual bool init(List list);
    void renderList();
    void loadList();
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    static ListInfoLayer* create(List list);
    static cocos2d::CCScene* scene(List list);

    void loadListFinished(cocos2d::CCArray*, const char*);
    void loadListFailed(const char*);
    void setupPageInfo(gd::string, const char*);
};
#pragma once
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

#include "../include/CvoltonAlertLayerStub.h"
#include "../../objects/list.hpp"

class MoreListSearchOptions : public CvoltonAlertLayerStub {
    std::string m_primary;
    List m_list;
    TextArea* m_info;
public:
    static MoreListSearchOptions* create();
    void onClose(cocos2d::CCObject* sender);
    void onStarToggle(cocos2d::CCObject* sender);
    bool init();
};
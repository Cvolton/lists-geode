#pragma once
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

#include "../include/CvoltonAlertLayerStub.h"
#include "../../objects/list.hpp"

class ExtendedListInfo : public CvoltonAlertLayerStub {
    std::string m_primary;
    List m_list;
    TextArea* m_info;
public:
    static ExtendedListInfo* create(List list);
    void onClose(cocos2d::CCObject* sender);
    void onCopyName(cocos2d::CCObject* sender);
    void onCopyAuthor(cocos2d::CCObject* sender);
    void onCopyDesc(cocos2d::CCObject* sender);
    void onCopyID(cocos2d::CCObject* sender);
    void onCopyVersion(cocos2d::CCObject* sender);
    void loadPage(int page);
    void refreshInfoTexts();
    static std::string zeroIfNA(int value);
    bool init(List list);
};
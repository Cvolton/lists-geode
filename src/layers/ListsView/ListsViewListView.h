#pragma once

#include "../include/CvoltonListView.h"
#include "../../objects/list.hpp"

class ListsViewListView : public CvoltonListView {
protected:
    void setupList() override;
    TableViewCell* getListCell(const char* key) override;
    void loadCell(TableViewCell* cell, int index) override;
    
public:
    static ListsViewListView* create(cocos2d::CCArray* levels, float width, float height);
};
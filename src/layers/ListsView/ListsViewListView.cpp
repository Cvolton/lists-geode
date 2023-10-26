#include "ListsViewListView.h"
#include "ListCell.h"

ListsViewListView* ListsViewListView::create(cocos2d::CCArray* levels, float width, float height) {
    auto ret = new ListsViewListView();
    if (ret && ret->init(levels, 0x420, width, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void ListsViewListView::setupList() {
    this->m_itemSeparation = 55.0f;

    this->m_tableView->reloadData();

    auto coverage = calculateNodeCoverage(m_tableView->m_contentLayer->getChildren());
    if (this->m_entries->count() > 4)
        m_tableView->m_contentLayer->setContentSize({-coverage.origin.x + coverage.size.width, -coverage.origin.y + coverage.size.height});

    this->m_tableView->moveToTop();

    if (this->m_entries->count() == 1)
        this->m_tableView->moveToTopWithOffset(this->m_itemSeparation);
}

void ListsViewListView::loadCell(TableViewCell* cell, int index) {
    static_cast<ListCell*>(cell)->loadFromWrapper(
        static_cast<CCNode*>(this->m_entries->objectAtIndex(index))
    );
    static_cast<ListCell*>(cell)->updateBGColor(index);
}

TableViewCell* ListsViewListView::getListCell(const char* key) {
    return new ListCell(key, { this->m_width, 55.0f });
}
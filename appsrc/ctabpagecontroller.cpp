#include "ctabpagecontroller.h"
#include "rdefine.h"

//tab page controller delegation:

define_reflectable_class_function(CTabPageControllerDelegation, setPagesChangeTarget, "setter;args:target")
void CTabPageControllerDelegation::setPagesChangeTarget(const MFunction<void ()>::ptr &target) {
    mPagesChangeTarget = target;
}

define_reflectable_class_function(CTabPageControllerDelegation, pagesChangeTarget, "getter")
MFunction<void ()>::ptr CTabPageControllerDelegation::pagesChangeTarget() {
    return mPagesChangeTarget;
}

define_reflectable_class_function(CTabPageControllerDelegation, pagesChange)
void CTabPageControllerDelegation::pagesChange() {
    if (mPagesChangeTarget) {
        mPagesChangeTarget->call();
    }
}

void CTabPageControllerDelegation::on_dispose() {
    base::dispose();

    mPagesChangeTarget = nullptr;
}

//tab page controller:

CTabPageController::CTabPageController() {
    mPages = MVector<CViewController::ptr>::create();
}

define_reflectable_class_function(CTabPageController, setDelegation, "setter;args:delegation")
void CTabPageController::setDelegation(const CTabPageControllerDelegation::ptr &delegation) {
    mDelegation = delegation;
}

define_reflectable_class_function(CTabPageController, delegation, "getter")
CTabPageControllerDelegation::ptr CTabPageController::delegation() {
    if (!mDelegation) {
        mDelegation = CTabPageControllerDelegation::create();
    }
    return mDelegation;
}

define_reflectable_class_function(CTabPageController, setPages, "args:pages")
void CTabPageController::setPages(const MVector<CViewController::ptr>::ptr &pages) {
    if (mPages->empty() && (!pages || pages->empty())) {
        return;
    }

    //remove old pages:
    if (mSelectedIndex != NoneIndex) {
        CViewController::ptr page = mPages->at(mSelectedIndex);
        mSelectedIndex = NoneIndex;
        hidePage(page);
    }

    for (const auto &it : *mPages) {
        it->setTabPageController(nullptr);
    }
    mPages->clear();

    //set new pages.
    if (pages && !pages->empty()) {
        mPages->insert(mPages->end(), pages->begin(), pages->end());

        for (const auto &it : *mPages) {
            it->setTabPageController(me());
        }

        //select first.
        mSelectedIndex = 0;
        showPage(mPages->at(0));
    }

    //emit event.
    delegation()->pagesChange();
}

define_reflectable_class_function(CTabPageController, indexOfPage, "args:page")
int CTabPageController::indexOfPage(const CViewController::ptr &page) {
    if (!page) {
        return NoneIndex;
    }

    auto it = std::find(mPages->begin(), mPages->end(), page);
    if (it == mPages->end()) {
        return NoneIndex;
    }

    return (int)(it - mPages->begin());
}

define_reflectable_class_function(CTabPageController, pageAtIndex, "args:index")
CViewController::ptr CTabPageController::pageAtIndex(int index) {
    if (0 <= index && index < mPages->size()) {
        return mPages->at(index);
    }
    return nullptr;
}

define_reflectable_class_function(CTabPageController, selectPage, "args:page")
void CTabPageController::selectPage(const CViewController::ptr &page) {
    int index = indexOfPage(page);
    selectIndex(index);
}

define_reflectable_class_function(CTabPageController, selectIndex, "args:index")
void CTabPageController::selectIndex(int index) {
    if (index == mSelectedIndex) {
        return;
    }

    //hide old page.
    if (mSelectedIndex != NoneIndex) {
        CViewController::ptr page = mPages->at(mSelectedIndex);
        mSelectedIndex = NoneIndex;
        hidePage(page);
    }

    //show new page.
    if (0 <= index && index < mPages->size()) {
        CViewController::ptr page = mPages->at(index);
        mSelectedIndex = index;
        showPage(page);
    }

    //emit event.
    delegation()->pagesChange();
}

define_reflectable_class_function(CTabPageController, selectedPage, "getter")
CViewController::ptr CTabPageController::selectedPage() {
    if (mSelectedIndex != NoneIndex) {
        return mPages->at(mSelectedIndex);
    }
    return nullptr;
}

define_reflectable_class_function(CTabPageController, selectedIndex, "getter")
int CTabPageController::selectedIndex() {
    return mSelectedIndex;
}

void CTabPageController::onViewLoad() {
    view()->setLayouter(MF(this, &CTabPageController::layoutPages));
}

void CTabPageController::layoutPages() {
    if (mSelectedIndex != NoneIndex) {
        CViewController::ptr page = mPages->at(mSelectedIndex);
        page->view()->setFrame(view()->bounds());
    }
}

void CTabPageController::showPage(const CViewController::ptr &page) {
    addChildController(page);
}

void CTabPageController::hidePage(const CViewController::ptr &page) {
    //NOTE: resign focus if needed.
    CResponder::ptr focus = CResponder::focusResponder();
    if (page->existResponder(focus)) {
        focus->resignFocusResponder();
    }

    page->removeFromParentController();
}

void CTabPageController::on_dispose() {
    base::dispose();

    if (mDelegation) {
        mDelegation->dispose();
    }
    for (const auto &it : *mPages) {
        it->dispose();
    }
}

#include "cnavigationcontroller.h"
#include "rdefine.h"

//navigation controller delegation:

define_reflectable_class_function(CNavigationControllerDelegation, setPagesChangeTarget, "setter;args:target")
void CNavigationControllerDelegation::setPagesChangeTarget(const MFunction<void ()>::ptr &target) {
    mPagesChangeTarget = target;
}

define_reflectable_class_function(CNavigationControllerDelegation, pagesChangeTarget, "getter")
MFunction<void ()>::ptr CNavigationControllerDelegation::pagesChangeTarget() {
    return mPagesChangeTarget;
}

define_reflectable_class_function(CNavigationControllerDelegation, pagesChange)
void CNavigationControllerDelegation::pagesChange() {
    if (mPagesChangeTarget) {
        mPagesChangeTarget->call();
    }
}

void CNavigationControllerDelegation::on_dispose() {
    base::dispose();

    mPagesChangeTarget = nullptr;
}

//navigation controller:

CNavigationController::CNavigationController() {
    mPages = MVector<CViewController::ptr>::create();
}

define_reflectable_class_function(CNavigationController, setDelegation, "setter;args:delegation")
void CNavigationController::setDelegation(const CNavigationControllerDelegation::ptr &delegation) {
    mDelegation = delegation;
}

define_reflectable_class_function(CNavigationController, delegation, "getter")
CNavigationControllerDelegation::ptr CNavigationController::delegation() {
    if (!mDelegation) {
        mDelegation = CNavigationControllerDelegation::create();
    }
    return mDelegation;
}

define_reflectable_class_function(CNavigationController, pushPage, "args:page")
void CNavigationController::pushPage(const CViewController::ptr &page) {
    if (!page) {
        return;
    }

    auto it = std::find(mPages->begin(), mPages->end(), page);
    if (it != mPages->end()) {
        return;
    }

    //hide old top.
    if (!mPages->empty()) {
        hidePage(mPages->back());
    }

    //show new top.
    mPages->push_back(page);
    page->setNavigationController(me());
    showPage(page);

    //emit event.
    delegation()->pagesChange();
}

define_reflectable_class_function(CNavigationController, popToPage, "args:page")
void CNavigationController::popToPage(const CViewController::ptr &page) {
    if (!page) {
        return;
    }

    auto handle = std::find(mPages->begin(), mPages->end(), page);
    if (handle == mPages->end() || *handle == mPages->back()) {
        return;
    }

    //remove unused pages and old top:
    for (auto it = handle + 1; it != mPages->end(); ++it) {
        (*it)->setNavigationController(nullptr);
    }

    CViewController::ptr top = mPages->back();
    mPages->erase(handle + 1, mPages->end());

    hidePage(top);

    //show new top.
    showPage(*handle);

    //emit event.
    delegation()->pagesChange();
}

define_reflectable_class_function(CNavigationController, popPage)
void CNavigationController::popPage() {
    //remove old top.
    if (!mPages->empty()) {
        CViewController::ptr top = mPages->back();
        mPages->pop_back();

        top->setNavigationController(nullptr);
        hidePage(top);
    }

    //show new top.
    if (!mPages->empty()) {
        CViewController::ptr top = mPages->back();
        showPage(top);
    }

    //emit event.
    delegation()->pagesChange();
}

define_reflectable_class_function(CNavigationController, topPage, "getter")
CViewController::ptr CNavigationController::topPage() {
    if (!mPages->empty()) {
        return mPages->back();
    }
    return nullptr;
}

void CNavigationController::onViewLoad() {
    view()->setLayouter(MF(this, &CNavigationController::layoutPages));
}

void CNavigationController::layoutPages() {
    if (!mPages->empty()) {
        CViewController::ptr top = mPages->back();
        top->view()->setFrame(view()->bounds());
    }
}

void CNavigationController::showPage(const CViewController::ptr &page) {
    addChildController(page);
}

void CNavigationController::hidePage(const CViewController::ptr &page) {
    //NOTE: resign focus if needed.
    CResponder::ptr focus = CResponder::focusResponder();
    if (page->existResponder(focus)) {
        focus->resignFocusResponder();
    }

    page->removeFromParentController();
}

void CNavigationController::on_dispose() {
    base::dispose();

    if (mDelegation) {
        mDelegation->dispose();
    }
    for (const auto &it : *mPages) {
        it->dispose();
    }
}

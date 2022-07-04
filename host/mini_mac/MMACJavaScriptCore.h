#import <JavaScriptCore/JavaScriptCore.h>
#import "mjsvm.h"

m_class(MMACJsVM, MJsVM) {
public:
    static void install();
    
public:
    MMACJsVM();

public:
    JSValue     *getJsValue (const reflect::any &cppValue);
    reflect::any getCppValue(JSValue *jsValue);

protected:
    void onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func) override;
    void onEvaluate(const std::string &name, const std::string &script) override;
    
    JSValue *onCallNativeFunction(const MBaseFunction::ptr &func, NSArray<JSValue *> *args);
    void onHandleException(JSValue *exception);
    
private:
    BOOL (^mIsFunction)(JSValue *);
    JSContext *mContext;
};

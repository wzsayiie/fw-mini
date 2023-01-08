#import <JavaScriptCore/JavaScriptCore.h>
#import "mjsvm.h"

//object pool:

@compatibility_alias MMACJsHash NSNumber;
@compatibility_alias MMACCppPtr NSNumber;

@interface MMACCppJsWrapper : NSObject
- (instancetype)initWithCppObject:(const reflect::object::ptr &)cppObject;
@property (nonatomic) reflect::object::ptr cppObject;
@end

@interface MMACJsWeak : NSObject
- (instancetype)initWithJsObject:(MMACCppJsWrapper *)jsObject;
@property (nonatomic, weak) MMACCppJsWrapper *jsObject;
@end

@interface MMACJsObjectPool : NSObject

+ (instancetype)instance;

//js hold cpp objects.
@property (nonatomic) NSMutableDictionary<MMACCppPtr *, MMACJsWeak *> *cppJsHolders;
//cpp hold js objects.
@property (nonatomic) NSMutableDictionary<MMACCppPtr *, JSValue    *> *jsObjects   ;
@property (nonatomic) NSMutableDictionary<MMACJsHash *, MMACCppPtr *> *jsCppHolders;

- (JSValue    *)jsValueFromCpp:(const reflect::any &)cppValue;
- (reflect::any)cppValueFromJs:(JSValue            *)jsValue ;

- (void)whenCollectCpp:(MMACCppPtr *)cppPtr;
- (void)whenCollectJs :(MMACCppPtr *)cppPtr;

@end

//js function:
class MMACJsVM;

m_class(MMACJsFunction, MGenericFunction) {
public:
    MMACJsFunction(JSValue *func);
    ~MMACJsFunction();

private:
    void on_call() const override;

private:
    JSValue *mFunc;
};

//virtual machine:

m_class(MMACJsVM, MJsVM) {
public:
    static void      install ();
    static MMACJsVM *instance();
    
public:
    MMACJsVM();

public:
    bool isFunction(JSValue *value);
    JSContext *context();

protected:
    void onRegisterFunction(const std::string &name, const MGenericFunction::ptr &func) override;
    void onEvaluate(const std::string &name, const std::string &script) override;
    
private:
    JSValue *onCallNativeFunction(const MGenericFunction::ptr &func, NSArray<JSValue *> *args);
    void onHandleException(JSValue *exception);
    
private:
    BOOL (^mIsFunction)(JSValue *);
    JSContext *mContext;
};

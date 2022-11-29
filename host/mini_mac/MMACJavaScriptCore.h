#import <JavaScriptCore/JavaScriptCore.h>
#import "mjsvm.h"

//object pool:

@compatibility_alias MMACCppObjectHolderHash NSNumber;
@compatibility_alias MMACCppObjectPointer    NSNumber;

@interface MMACCppObjectHolder : NSObject
- (instancetype)initWithCppValue:(const reflect::object::ptr &)cppObject;
@property (nonatomic) reflect::object::ptr cppObject;
@end

@interface MMACJsObjectWeakRef : NSObject
- (instancetype)initWithJsValue:(JSValue *)jsObject;
@property (nonatomic, weak) JSValue *jsObject;
@end

@interface MMACJsObjectPool : NSObject

+ (instancetype)instance;
+ (void)clearInstance;

//js hold cpp objects.
@property (nonatomic) NSMutableDictionary<MMACCppObjectHolderHash *, MMACCppObjectHolder *> *cppObjects;
@property (nonatomic) NSMutableDictionary<MMACCppObjectPointer    *, MMACJsObjectWeakRef *> *cppHolders;
//cpp hold js objects.
@property (nonatomic) NSMutableDictionary<MMACCppObjectPointer    *, JSValue              *> *jsObjects;
@property (nonatomic) NSMutableDictionary<MMACCppObjectHolderHash *, MMACCppObjectPointer *> *jsHolders;

- (JSValue    *)jsValueFromCpp:(const reflect::any &)cppValue;
- (reflect::any)cppValueFromJs:(JSValue            *)jsValue ;

- (void)collectCppObject:(MMACCppObjectPointer    *)cppPointer;
- (void)collectJsObject :(MMACCppObjectHolderHash *)holderHash;

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
    ~MMACJsVM();

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

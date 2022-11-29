#import "MMACJavaScriptCore.h"

//object pool:

@implementation MMACCppObjectHolder

- (instancetype)initWithCppValue:(const reflect::object::ptr &)cppObject {
    if (self = [super init]) {
        self.cppObject = cppObject;
    }
    return self;
}

- (void)dealloc {
    [MMACJsObjectPool.instance collectJsObject:@(self.hash)];
}

@end

@implementation MMACJsObjectWeakRef

- (instancetype)initWithJsValue:(JSValue *)jsObject {
    if (self = [super init]) {
        self.jsObject = jsObject;
    }
    return self;
}

@end

static MMACJsObjectPool *sSharedJsObjectPool = nil;

@implementation MMACJsObjectPool

+ (instancetype)instance {
    if (!sSharedJsObjectPool) {
        sSharedJsObjectPool = [[self alloc] init];
    }
    return sSharedJsObjectPool;
}

+ (void)clearInstance {
    sSharedJsObjectPool = nil;
}

- (instancetype)init {
    if (self = [super init]) {
        self.cppObjects = [NSMutableDictionary dictionary];
        self.cppHolders = [NSMutableDictionary dictionary];
        self.jsObjects  = [NSMutableDictionary dictionary];
        self.jsHolders  = [NSMutableDictionary dictionary];
    }
    return self;
}

- (JSValue *)jsValueFromCpp:(const reflect::any &)cppValue {
    JSContext *context = MMACJsVM::instance()->context();
    
    switch (cppValue.preferred_type()) {
        case reflect::data_type::is_object: {
            reflect::object::ptr  cppObject  = cppValue.as_object_shared();
            MMACCppObjectPointer *cppPointer = @((intptr_t)cppObject.get());
            
            //null.
            if (!cppObject) {
                return nil;
            }
            
            //query from cache:
            {
                MMACJsObjectWeakRef *holder = self.cppHolders[cppPointer];
                if (holder) {
                    return holder.jsObject;
                }
                
                JSValue *held = self.jsObjects[cppPointer];
                if (held) {
                    return held;
                }
            }
            
            //new object.
            MMACCppObjectHolder     *cppHolder  = [[MMACCppObjectHolder alloc] initWithCppValue:cppObject];
            MMACCppObjectHolderHash *holderHash = @(cppHolder.hash);
            JSValue                 *jsObject   = [JSValue valueWithObject:cppHolder inContext:context];
            MMACJsObjectWeakRef     *jsWeak     = [[MMACJsObjectWeakRef alloc] initWithJsValue:jsObject];
            
            self.cppObjects[holderHash] = cppHolder;
            self.cppHolders[cppPointer] = jsWeak   ;
            
            return jsObject;
        }
        
        case reflect::data_type::is_string: {
            return [JSValue valueWithObject:@(cppValue.as_chars()) inContext:context];
        }
        
        case reflect::data_type::is_byte  :
        case reflect::data_type::is_int   :
        case reflect::data_type::is_int64 :
        case reflect::data_type::is_float :
        case reflect::data_type::is_double: {
            return [JSValue valueWithDouble:cppValue.as_double() inContext:context];
        }
        
        case reflect::data_type::is_bool: {
            return [JSValue valueWithBool:cppValue.as_bool() inContext:context];
        }
        
        default: {
            return nil;
        }
    }
}

- (reflect::any)cppValueFromJs:(JSValue *)jsValue {
    if (jsValue.isObject) {
        MMACCppObjectHolderHash *holderHash = @([jsValue.toObject hash]);
        
        //query from cache:
        {
            MMACCppObjectPointer *cppPointer = self.jsHolders[holderHash];
            if (cppPointer) {
                return ((reflect::object *)cppPointer.longLongValue);
            }
            
            MMACCppObjectHolder *cppHolder = self.cppObjects[holderHash];
            if (cppHolder) {
                return cppHolder.cppObject;
            }
        }
        
        //new object if the value is a function.
        if (MMACJsVM::instance()->isFunction(jsValue)) {
            reflect::object::ptr  cppObject  = MMACJsFunction::create(jsValue);
            MMACCppObjectPointer *cppPointer = @((intptr_t)cppObject.get());
            
            self.jsObjects[cppPointer] = jsValue   ;
            self.jsHolders[holderHash] = cppPointer;
            
            return cppObject;
        }
        
        return nullptr;
        
    } else if (jsValue.isString) {
        return jsValue.toString.UTF8String;
        
    } else if (jsValue.isNumber) {
        return jsValue.toDouble;
        
    } else if (jsValue.isBoolean) {
        return (bool)jsValue.toBool;
        
    } else {
        return nullptr;
    }
}

- (void)collectCppObject:(MMACCppObjectPointer *)cppPointer {
    JSValue *jsValue = self.jsObjects[cppPointer];
    MMACCppObjectHolderHash *holderHash = @([jsValue.toObject hash]);
    
    [self.jsObjects removeObjectForKey:cppPointer];
    [self.jsHolders removeObjectForKey:holderHash];
}

- (void)collectJsObject:(MMACCppObjectHolderHash *)holderHash {
    MMACCppObjectHolder  *cppHolder  = self.cppObjects[holderHash];
    MMACCppObjectPointer *cppPointer = @((intptr_t)cppHolder.cppObject.get());
    
    [self.cppObjects removeObjectForKey:holderHash];
    [self.cppHolders removeObjectForKey:cppPointer];
}

@end

//js fucntion:

MMACJsFunction::MMACJsFunction(JSValue *func) {
    mFunc = func;
}

MMACJsFunction::~MMACJsFunction() {
    [MMACJsObjectPool.instance collectCppObject:@((intptr_t)this)];
}

void MMACJsFunction::on_call() const {
    //argument:
    NSMutableArray *jsArgs = [NSMutableArray array];
        
    int count = reflect::get_arg_count();
    for (int i = 0; i < count; ++i) {
        reflect::any cppArg = reflect::get_arg_value(i);
        JSValue *jsArg = [MMACJsObjectPool.instance jsValueFromCpp:cppArg];
            
        [jsArgs addObject:jsArg];
    }
        
    //call.
    JSValue *jsRet = [mFunc callWithArguments:jsArgs];
        
    //return.
    reflect::any cppRet = [MMACJsObjectPool.instance cppValueFromJs:jsRet];
    reflect::return_value(cppRet);
}


//virtual machine:

void MMACJsVM::install() {
    auto obj = MMACJsVM::create();
    setInstance(obj);
}

MMACJsVM *MMACJsVM::instance() {
    return (MMACJsVM *)MJsVM::instance();
}

MMACJsVM::MMACJsVM() {
    mContext = [[JSContext alloc] init];
    mContext.exceptionHandler = ^(JSContext *, JSValue *exception) {
        onHandleException(exception);
    };
    
    //a JSValue does not provide the method to determine whether it is a function.
    [mContext evaluateScript:@"function _is_function(a) { return typeof(a) == 'function' }"];
    
    JSValue *isFunc = [mContext.globalObject valueForProperty:@"_is_function"];
    mIsFunction = ^(JSValue *value) {
        return [isFunc callWithArguments:@[ value ]].toBool;
    };
}

MMACJsVM::~MMACJsVM() {
    [MMACJsObjectPool clearInstance];
}

bool MMACJsVM::isFunction(JSValue *value) {
    return mIsFunction(value);
}

JSContext *MMACJsVM::context() {
    return mContext;
}

void MMACJsVM::onRegisterFunction(const std::string &name, const MGenericFunction::ptr &func) {
    NSString *key = @(name.c_str());
    
    //NOTE: let the block hold the object, not the pointer.
    MGenericFunction::ptr strongFunc = func;
    
    //supports up to 4 parameters.
    mContext[key] = ^(JSValue *a, JSValue *b, JSValue *c, JSValue *d) {
        return onCallNativeFunction(strongFunc, @[a, b, c, d]);
    };
}

void MMACJsVM::onEvaluate(const std::string &name, const std::string &script) {
    NSString *scriptText = @(script.c_str());
    NSURL *sourceURL = [NSURL URLWithString:@(name.c_str())];
    
    [mContext evaluateScript:scriptText withSourceURL:sourceURL];
}

JSValue *MMACJsVM::onCallNativeFunction(const MGenericFunction::ptr &func, NSArray<JSValue *> *jsArgs) {
    std::vector<reflect::any> cppArgs;
    for (JSValue *jsArg in jsArgs) {
        reflect::any cppArg = [MMACJsObjectPool.instance cppValueFromJs:jsArg];
        cppArgs.push_back(cppArg);
    }
    
    reflect::any cppRet = func->call_with_args(cppArgs);
    return [MMACJsObjectPool.instance jsValueFromCpp:cppRet];
}

void MMACJsVM::onHandleException(JSValue *exception) {
    JSValue *source = [exception objectForKeyedSubscript:@"sourceURL"];
    JSValue *line   = [exception objectForKeyedSubscript:@"line"     ];
    JSValue *stack  = [exception objectForKeyedSubscript:@"stack"    ];
    
    NSMutableString *message = [NSMutableString string];
    [message appendFormat:@"JavaScript Exception\n"  ];
    [message appendFormat:@"Error :  %@\n", exception];
    [message appendFormat:@"Source:  %@\n", source   ];
    [message appendFormat:@"Line  :  %@\n", line     ];
    [message appendFormat:@"Stack :\n%@\n", stack    ];
    
    onException(message.UTF8String);
}

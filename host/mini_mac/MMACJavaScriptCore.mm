#import "MMACJavaScriptCore.h"

//object pool:

@implementation MMACCppJsWrapper

- (instancetype)initWithCppObject:(const reflect::object::ptr &)cppObject {
    if (self = [super init]) {
        self.cppObject = cppObject;
    }
    return self;
}

- (void)dealloc {
    MMACCppPtr *cppPtr = @((intptr_t)self.cppObject.get());
    [MMACJsObjectPool.instance whenCollectJs:cppPtr];
}

@end

@implementation MMACJsWeak

- (instancetype)initWithJsObject:(MMACCppJsWrapper *)jsObject {
    if (self = [super init]) {
        self.jsObject = jsObject;
    }
    return self;
}

@end

@implementation MMACJsObjectPool

+ (instancetype)instance {
    static MMACJsObjectPool *obj = [[self alloc] init];
    return obj;
}

- (instancetype)init {
    if (self = [super init]) {
        self.cppJsHolders = [NSMutableDictionary dictionary];
        self.jsObjects    = [NSMutableDictionary dictionary];
        self.jsCppHolders = [NSMutableDictionary dictionary];
    }
    return self;
}

- (JSValue *)jsValueFromCpp:(const reflect::any &)cppValue {
    JSContext *context = MMACJsVM::instance()->context();
    
    switch (cppValue.preferred_type()) {
        case reflect::data_type::is_object: {
            reflect::object::ptr cppObject = cppValue.as_object_shared();
            MMACCppPtr          *cppPtr    = @((intptr_t)cppObject.get());
            
            //null.
            if (!cppObject) {
                return nil;
            }
            
            //query from cache:
            {
                MMACJsWeak *holder = self.cppJsHolders[cppPtr];
                if (holder) {
                    return [JSValue valueWithObject:holder.jsObject inContext:context];
                }
                
                JSValue *held = self.jsObjects[cppPtr];
                if (held) {
                    return held;
                }
            }
            
            //new object.
            MMACCppJsWrapper *jsObject = [[MMACCppJsWrapper alloc] initWithCppObject:cppObject];
            MMACJsWeak       *jsWeak   = [[MMACJsWeak alloc] initWithJsObject:jsObject];
            
            //IMPORTANT: store "MMACCppJsWrapper", instead of "JSValue".
            //a "JSValue" may released, and pass the held object to other "JSValue".
            self.cppJsHolders[cppPtr] = jsWeak;
            
            return [JSValue valueWithObject:jsObject inContext:context];;
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
        MMACJsHash *jsHash = @(jsValue.hash);
        
        //query from cache:
        {
            MMACCppPtr *holder = self.jsCppHolders[jsHash];
            if (holder) {
                return ((reflect::object *)holder.longLongValue)->me();
            }
            
            MMACCppJsWrapper *held = jsValue.toObject;
            if ([held isMemberOfClass:MMACCppJsWrapper.class]) {
                return held.cppObject;
            }
        }
        
        //new object if the value is a function.
        if (MMACJsVM::instance()->isFunction(jsValue)) {
            reflect::object::ptr cppObject = MMACJsFunction::create(jsValue);
            MMACCppPtr          *cppPtr    = @((intptr_t)cppObject.get());
            
            self.jsObjects   [cppPtr] = jsValue;
            self.jsCppHolders[jsHash] = cppPtr ;
            
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

- (void)whenCollectCpp:(MMACCppPtr *)cppPtr {
    JSValue    *jsObject = self.jsObjects[cppPtr];
    MMACJsHash *jsHash   = @(jsObject.hash);
    
    [self.jsObjects    removeObjectForKey:cppPtr];
    [self.jsCppHolders removeObjectForKey:jsHash];
}

- (void)whenCollectJs:(MMACCppPtr *)cppPtr {
    [self.cppJsHolders removeObjectForKey:cppPtr];
}

@end

//js fucntion:

MMACJsFunction::MMACJsFunction(JSValue *func) {
    mFunc = func;
}

MMACJsFunction::~MMACJsFunction() {
    MMACCppPtr *cppPtr = @((intptr_t)this);
    [MMACJsObjectPool.instance whenCollectCpp:cppPtr];
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

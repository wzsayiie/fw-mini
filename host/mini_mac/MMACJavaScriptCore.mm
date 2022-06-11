#import "MMACJavaScriptCore.h"

//assist:

@interface MCPPValueWrapper : NSObject
@property (nonatomic) reflect::any CPPValue;
@end

@implementation MCPPValueWrapper
@end

minikit_class(MJsFunction, MBaseFunction) {
public:
    MJsFunction(MMACJsVM *jsVM, JSValue *func) {
        mJsVM = jsVM;
        mFunc = func;
    }

    void on_call() const override {
        //argument:
        NSMutableArray *jsArgs = [NSMutableArray array];
        
        int count = reflect::get_arg_count();
        for (int i = 0; i < count; ++i) {
            reflect::any cppArg = reflect::get_arg_value(i);
            JSValue *jsArg = mJsVM->getJsValue(cppArg);
            
            [jsArgs addObject:jsArg];
        }
        
        //call.
        JSValue *jsRet = [mFunc callWithArguments:jsArgs];
        
        //return.
        reflect::any cppRet = mJsVM->getCppValue(jsRet);
        reflect::return_value(cppRet);
    }

private:
    MMACJsVM *mJsVM;
    JSValue  *mFunc;
};

//js vm:

void MMACJsVM::install() {
    auto obj = MMACJsVM::create();
    setInstance(obj);
}

MMACJsVM::MMACJsVM() {
    mContext = [[JSContext alloc] init];
    mContext.exceptionHandler = ^(JSContext *, JSValue *exception) {
        onHandleException(exception);
    };
    
    //a JSValue does not provide the method to determine whether it is a function.
    [mContext evaluateScript:@"function _is_function(a) { return typeof a == 'function' }"];
    
    JSValue *isFunc = [mContext.globalObject valueForProperty:@"_is_function"];
    mIsFunction = ^(JSValue *value) {
        return [isFunc callWithArguments:@[ value ]].toBool;
    };
}

JSValue *MMACJsVM::getJsValue(const reflect::any &cppValue) {
    switch (cppValue.preferred_type()) {
        case reflect::data_type::is_object: {
            MCPPValueWrapper *wrapper = [[MCPPValueWrapper alloc] init];
            wrapper.CPPValue = cppValue;
            
            return [JSValue valueWithObject:wrapper inContext:mContext];
        }
        
        case reflect::data_type::is_string: {
            return [JSValue valueWithObject:@(cppValue.as_chars()) inContext:mContext];
        }
        
        case reflect::data_type::is_byte  :
        case reflect::data_type::is_int   :
        case reflect::data_type::is_int64 :
        case reflect::data_type::is_float :
        case reflect::data_type::is_double: {
            return [JSValue valueWithDouble:cppValue.as_double() inContext:mContext];
        }
        
        case reflect::data_type::is_bool: {
            return [JSValue valueWithBool:cppValue.as_bool() inContext:mContext];
        }
        
        default: {
            return nullptr;
        }
    }
}

reflect::any MMACJsVM::getCppValue(JSValue *jsValue) {
    if (jsValue.isObject) {
        if (mIsFunction(jsValue)) {
            return MJsFunction::create(this, jsValue);
            
        } else if ([jsValue.toObject isKindOfClass:MCPPValueWrapper.class]) {
            return ((MCPPValueWrapper *)jsValue.toObject).CPPValue;
            
        } else {
            return nullptr;
        }
        
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

void MMACJsVM::onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func) {
    NSString *key = @(name.c_str());
    
    //NOTE: let the block hold the object, not the pointer.
    MBaseFunction::ptr strongFunc = func;
    
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

JSValue *MMACJsVM::onCallNativeFunction(const MBaseFunction::ptr &func, NSArray<JSValue *> *jsArgs) {
    std::vector<reflect::any> cppArgs;
    for (JSValue *jsArg in jsArgs) {
        reflect::any cppArg = getCppValue(jsArg);
        cppArgs.push_back(cppArg);
    }
    
    reflect::any cppRet = func->call_with_args(cppArgs);
    return getJsValue(cppRet);
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

#import "MJavaScriptCore.h"
#import <JavaScriptCore/JavaScriptCore.h>
#import "mhostjs.h"

static JSContext *sContext = nil;

@interface NativeObjectWrapper : NSObject
@property (nonatomic) MObjectRef nativeObject;
@end

@implementation NativeObjectWrapper

- (instancetype)initWithNativeObject:(MObject *)object {
    if (self = [super init]) {
        self.nativeObject = m_cast_shared object;
    }
    return self;
}

@end

class JSFunctionWrapper : public MUnknown {

public:
    JSFunctionWrapper(JSValue *value) {
        mValue = value;
    }

    static void call(MObject *load) {
        auto wrapper = (JSFunctionWrapper *)load;
        [wrapper->mValue callWithArguments:nil];
    }

private:
    JSValue *mValue;
};

static MObject *CopyObjectFromJSValue(JSValue *value) {
    if (value.isBoolean) {
        return MBoolCreate(value.toBool);
        
    } else if (value.isNumber) {
        return MDoubleCreate(value.toDouble);
        
    } else if (value.isString) {
        return MStringCreateU8(value.toString.UTF8String);
        
    } else if (value.isObject) {
        //if value is function.
        JSValue *fnFunc = [sContext.globalObject valueForProperty:@"isFunc"];
        JSValue *isFunc = [fnFunc callWithArguments:@[ value ]];
        if (isFunc.toBool) {
            auto wrapper = new JSFunctionWrapper(value);
            return MLambdaCreate(JSFunctionWrapper::call, wrapper);
        }
        
        //if value is native object.
        if ([value.toObject isKindOfClass:NativeObjectWrapper.class]) {
            NativeObjectWrapper *wrapper = (NativeObjectWrapper *)value.toObject;
            return MRetain(wrapper.nativeObject.get());
        }
        
        return nullptr;
        
    } else {
        return nullptr;
    }
}

static JSValue *JSValueFromObject(MObject *object) {
    MTypeId type = MGetTypeId(object);
    
    if (object == nullptr) {
        return nil;
        
    } else if (type == MTypeIdOf<MBool *>::Value) {
        bool raw = MBoolValue(object);
        return [JSValue valueWithBool:raw inContext:sContext];
        
    } else if (MIsNumberObject(type)) {
        double raw = MDoubleValue(object);
        return [JSValue valueWithDouble:raw inContext:sContext];
        
    } else if (type == MTypeIdOf<MString *>::Value) {
        const char *raw = MStringU8Chars((MString *)object);
        return [JSValue valueWithObject:@(raw) inContext:sContext];
        
    } else {
        auto wrapper = [[NativeObjectWrapper alloc] initWithNativeObject:object];
        
        JSValue *value = [JSValue valueWithObject:wrapper inContext:sContext];
        [value setValue:@YES forProperty:@"isNativeObject"];
        return value;
    }
}

static NSArray *CallingJSParams(JSValue *a, JSValue *b, JSValue *c, JSValue *d) {
    if /**/ (!d.isUndefined) { return @[a, b, c, d]; }
    else if (!c.isUndefined) { return @[a, b, c]; }
    else if (!b.isUndefined) { return @[a, b]; }
    else if (!a.isUndefined) { return @[a]; }
    
    return nil;
}

static JSValue *OnCallNative(NSString *name, JSValue *a, JSValue *b, JSValue *c, JSValue *d) {
    NSArray *JSParams = CallingJSParams(a, b, c, d);
    
    MArrayRef params;
    if (JSParams.count > 0) {
        params = m_auto_release MArrayCreate();
        
        for (JSValue *value in JSParams) {
            MObjectRef object = m_auto_release CopyObjectFromJSValue(value);
            MArrayAppend(params.get(), object.get());
        }
    }
    
    MStringRef funcName = m_auto_release MStringCreateU8(name.UTF8String);
    MObjectRef returned = m_auto_release _MJsOnCallCopyRet(funcName.get(), params.get());
    return JSValueFromObject(returned.get());
}

static void RegisterFunc(MString *name) {
    NSString *funcName = @(MStringU8Chars(name));
    
    sContext[funcName] = ^(JSValue *a, JSValue *b, JSValue *c, JSValue *d) {
        return OnCallNative(funcName, a, b, c, d);
    };
}

static void RunScript(MString *name, MString *script) {
    NSString *scriptText = @(MStringU8Chars(script));
    NSString *scriptName = @(MStringU8Chars(name));
    NSURL    *scriptURL  = [NSURL URLWithString:scriptName];
    
    [sContext evaluateScript:scriptText withSourceURL:scriptURL];
}

static void OnException(JSValue *exception) {
    JSValue *source = [exception objectForKeyedSubscript:@"sourceURL"];
    JSValue *line   = [exception objectForKeyedSubscript:@"line"     ];
    JSValue *stack  = [exception objectForKeyedSubscript:@"stack"    ];
    
    NSMutableString *infoBuilder = [NSMutableString string];
    [infoBuilder appendFormat:@"JavaScript Exception\n"  ];
    [infoBuilder appendFormat:@"Error :  %@\n", exception];
    [infoBuilder appendFormat:@"Source:  %@\n", source   ];
    [infoBuilder appendFormat:@"Line  :  %@\n", line     ];
    [infoBuilder appendFormat:@"Stack :\n%@\n", stack    ];
    
    MStringRef inforString = m_auto_release MStringCreateU8(infoBuilder.UTF8String);
    _MJsOnHappenError(inforString.get());
}

void MInstallJSVirtualMachine() {
    sContext = [[JSContext alloc] init];
    sContext.exceptionHandler = ^(JSContext *context, JSValue *exception) {
        OnException(exception);
    };
    
    //a JSValue does not provide the method to determine whether it is a function.
    [sContext evaluateScript:@"function isFunc(a) { return typeof a == 'function' }"];
    
    _MJsSetRegisterFunc(RegisterFunc);
    _MJsSetRunScript(RunScript);
}

//minikit.js

//------------------------------------------------------------------------------
//string format & log:

function _MakeIndent(indent, out) {
    while (indent-- > 0) {
        out.push("  ")
    }
}

function _MakeStringWithShrink(shrink, indent, object, out) {
    if (!shrink) {
        _MakeIndent(indent, out)
    }

    if (object === null) {
        out.push("null")

    } else if (Array.isArray(object)) {
        out.push("[\n")

        for (let item of object) {
            _MakeStringWithShrink(false, indent + 1, item, out)
            out.push(",\n")
        }

        _MakeIndent(out, indent)
        out.push("]")

    } else if (typeof object == "object") {
        out.push("{\n")

        for (let key in object) {
            let val = object[key]

            _MakeStringWithShrink(false, indent + 1, key, out)
            out.push(": " )

            _MakeStringWithShrink(true , indent + 1, val, out)
            out.push(",\n")
        }

        _MakeIndent(indent, out)
        out.push("}")

    } else {
        out.push(String(object))
    }
}

function MString(object) {
    let array = []
    _MakeStringWithShrink(true, 0, object, array)
    return array.join("")
}

function I(object) {
    let message = MString(object)
    MPrintMessage(message)
}

//------------------------------------------------------------------------------
//MEnumId:

function MEnumId(string) {
    let id = 0
    for (let index in string) {
        id = (id << 8) | string.charCodeAt(index)
    }
    return id
}

//------------------------------------------------------------------------------
//MType:

const MType = Object.freeze({
    Object : MEnumId("Obj"),
    Bool   : MEnumId("Bol"),
    Int    : MEnumId("Int"),
    Float  : MEnumId("Flt"),
    Pointer: MEnumId("Ptr"),
    String : MEnumId("Str"),
    Lambda : MEnumId("Lmd"),
    Data   : MEnumId("Dat"),
    Array  : MEnumId("Arr"),
    Image  : MEnumId("Img"),
    Special: MEnumId("Spc"),
})

//------------------------------------------------------------------------------
//MWindowEvent:

const MWindowEvent = Object.freeze({
    Load      : MEnumId("L"),
    Show      : MEnumId("S"),
    Hide      : MEnumId("H"),
    Resize    : MEnumId("R"),
    Draw      : MEnumId("D"),
    TouchBegin: MEnumId("B"),
    TouchMove : MEnumId("M"),
    TouchEnd  : MEnumId("E"),
    TextBox   : MEnumId("T"),
    KeyDown   : MEnumId("K"),
})

//------------------------------------------------------------------------------
//MJsLambda:

let _jsLambdaPool = {}
let _jsLambdaIden = 0

function _JsLambdaInsert(func) {
    let iden = ++_jsLambdaIden
    _jsLambdaPool[iden] = func
    return iden
}

function _JsLambdaInvoke(iden) {
    let func = _jsLambdaPool[iden]
    if (func) {
        func()
    }
}

function _JsLambdaRemove(iden) {
    delete _jsLambdaPool[iden]
}

function MJsLambda(func) {
    if (typeof func == "function") {
        let iden = _JsLambdaInsert(func)
        return MJsLambdaCreate(iden)
    }
    return null
}

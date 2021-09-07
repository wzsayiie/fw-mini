//minikit.js

function _makeIndent(indent, out) {
    while (indent-- > 0) {
        out.push("  ")
    }
}

function _makeStringWithShrink(shrink, indent, object, out) {
    if (!shrink) {
        _makeIndent(indent, out)
    }

    if (object === null) {
        out.push("null")

    } else if (Array.isArray(object)) {
        out.push("[\n")

        for (let item of object) {
            _makeStringWithShrink(false, indent + 1, item, out)
            out.push(",\n")
        }

        _makeIndent(out, indent)
        out.push("]")

    } else if (typeof object == "object") {
        out.push("{\n")

        for (let key in object) {
            let val = object[key]

            _makeStringWithShrink(false, indent + 1, key, out)
            out.push(": " )

            _makeStringWithShrink(true , indent + 1, val, out)
            out.push(",\n")
        }

        _makeIndent(indent, out)
        out.push("}")

    } else {
        out.push(String(object))
    }
}

function MString(object) {
    let array = []
    _makeStringWithShrink(true, 0, object, array)
    return array.join("")
}

function MLog(object) {
    let message = MString(object)
    MPrintMessage(message)
}

function MEnumId(string) {
    let id = 0
    for (let index in string) {
        id = (id << 8) | string.charCodeAt(index)
    }
    return id
}

const MType = {
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
}
Object.freeze(MType)

let _jsLambdaPool = {}
let _jsLambdaIden = 0

function _insertJsLambda(func) {
    let iden = ++_jsLambdaIden
    _jsLambdaPool[iden] = func
    return iden
}

function _invokeJsLambda(iden) {
    let func = _jsLambdaPool[iden]
    if (func) {
        func()
    }
}

function _removeJsLambda(iden) {
    delete _jsLambdaPool[iden]
}

function MJsLambda(func) {
    if (typeof func == "function") {
        let iden = _insertJsLambda(func)
        return MJsLambdaCreate(iden)
    }
    return null
}

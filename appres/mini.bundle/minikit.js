//minikit.js

function _make_indent(indent, out) {
    while (indent-- > 0) {
        out.push("  ")
    }
}

function _make_string_with_shrink(shrink, indent, object, out) {
    if (!shrink) {
        _make_indent(indent, out)
    }

    if (object === null) {
        out.push("null")

    } else if (Array.isArray(object)) {
        out.push("[\n")

        for (let item of object) {
            _make_string_with_shrink(false, indent + 1, item, out)
            out.push(",\n")
        }

        _make_indent(out, indent)
        out.push("]")

    } else if (typeof object == "object") {
        out.push("{\n")

        for (let key in object) {
            let val = object[key]

            _make_string_with_shrink(false, indent + 1, key, out)
            out.push(": " )

            _make_string_with_shrink(true , indent + 1, val, out)
            out.push(",\n")
        }

        _make_indent(indent, out)
        out.push("}")

    } else {
        out.push(String(object))
    }
}

function MString(object) {
    let array = []
    _make_string_with_shrink(true, 0, object, array)
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

//shield this function, it's unsafe for the js environment.
function MRelease() {}

let _MJsLambdaObjects = {}
let _MJsLambdaIdCount = 0

function _MJsLambdaInsert(func) {
    let iden = _MJsLambdaIdCount
    _MJsLambdaIdCount += 1

    _MJsLambdaObjects[iden] = func
    return iden
}

function _MJsLambdaInvoke(iden) {
    let func = _MJsLambdaObjects[iden]
    if (func) {
        func()
    }
}

function _MJsLambdaRemove(iden) {
    delete _MJsLambdaObjects[iden]
}

function MJsLambda(func) {
    if (typeof func == "function") {
        let iden = _MJsLambdaInsert(func)
        return MJsLambdaCreate(iden)
    }
    return null
}

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

        _MakeIndent(indent, out)
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

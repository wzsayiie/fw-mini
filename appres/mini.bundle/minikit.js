//minikit.js

let inner = {}
let m = {}

//------------------------------------------------------------------------------
//print:

inner.makeIndent = function (outArray, indent) {
    while (indent-- > 0) {
        outArray.push("  ")
    }
}

inner.makeString = function (outArray, isHeadIndent, indent, object) {
    if (isHeadIndent) {
        inner.makeIndent(outArray, indent)
    }

    if (object == null) {
        outArray.push("null")

    } else if (typeof(object) == "object" && Array.isArray(object)) {
        outArray.push("[\n")

        for (let item of object) {
            inner.makeString(outArray, true, indent + 1, item)
            outArray.push(",\n")
        }

        inner.makeIndent(outArray, indent)
        outArray.push("]")

    } else if (typeof(object) == "object") {
        outArray.push("{\n")

        for (let key in object) {
            inner.makeString(outArray, true, indent + 1, key)
            outArray.push(": ")

            inner.makeString(outArray, false, indent + 1, object[key])
            outArray.push(",\n")
        }

        inner.makeIndent(outArray, indent)
        outArray.push("}")

    } else {
        outArray.push(String(object))
    }
}

m.string = function (object) {
    let array = []
    inner.makeString(array, false, 0, object)
    return array.join("")
}

m.print = function (object) {
    let message = m.string(object)
    MPrintMessage(message)
}

//------------------------------------------------------------------------------
//MObject:

inner.enumId = function (string) {
    let a = string.charCodeAt(0)
    let b = string.charCodeAt(1)
    let c = string.charCodeAt(2)

    return a | (b << 8) | (c << 16)
}

m.TypeObject  = inner.enumId("Obj")
m.TypeBool    = inner.enumId("Bol")
m.TypeInt     = inner.enumId("Int")
m.TypeFloat   = inner.enumId("Flt")
m.TypePointer = inner.enumId("Ptr")
m.TypeString  = inner.enumId("Str")
m.TypeLambda  = inner.enumId("Lmd")
m.TypeData    = inner.enumId("Dat")
m.TypeArray   = inner.enumId("Arr")
m.TypeImage   = inner.enumId("Img")
m.TypeSpecial = inner.enumId("Spc")

m.release = function (object) {
    MRelease(object)
    delete object.isNativeObject
}

m.getType = function (object) {
    return MGetType(object)
}

//------------------------------------------------------------------------------
//MLambda:

inner.lambdaObjects = {}
inner.lambdaIdCount = 0

inner.addLambda = function (func) {
    let id = ++inner.lambdaIdCount
    inner.lambdaObjects[id] = func
    return id
}

inner.callLambda = function (id) {
    let func = inner.lambdaObjects[id]
    if (func) {
        func()
    }
}

inner.removeLambda = function (id) {
    delete inner.lambdaObjects[id]
}

m.lambdaCreate = function (func) {
    if (typeof(func) == "function") {
        let id = inner.addLambda(func)
        return MLambdaCreate(id)
    }
    return null
}

m.lambdaCall = function (lambda) {
    MLambdaCall(lambda)
}

//------------------------------------------------------------------------------
//MArray:

m.arrayCreate = function () {
    return MArrayCreate()
}

m.arrayAppend = function (array, item) {
    MArrayAppend(array, item)
}

m.arrayLength = function (array) {
    return MArrayLength(array)
}

m.arrayItem = function (array, index) {
    return MArrayItem(array, index)
}

//------------------------------------------------------------------------------
//MJsLambda:

let _lambdaPool = {}
let _lambdaIden = 0

function MJsLambdaInsert(func) {
    let iden = ++_lambdaIden
    _lambdaPool[iden] = func
    return iden
}

function MJsLambdaInvoke(iden) {
    let func = _lambdaPool[iden]
    if (func) {
        func()
    }
}

function MJsLambdaRemove(iden) {
    delete _lambdaPool[iden]
}

function MJsLambda(func) {
    if (typeof func == 'function') {
        let iden = MJsLambdaInsert(func)
        return MJsLambdaWrap(iden)
    }
    return null
}

//------------------------------------------------------------------------------
//require.js module support:

function _absolutePath(base, relative) {
    let array = base ? base.split('/') : []
    let fresh = relative.split('/')

    fresh.forEach((item) => {
        if (item == '..') {
            if (array.length == 0 || array[array.length - 1] == '..') {
                array.push('..')
            } else {
                array.pop()
            }

        } else if (item == '.') {
            return

        } else {
            array.push(item)
        }
    })

    let last = array.pop()
    let dir  = array.join('/')
    let file = `${dir}/${last}`

    return {
        file: file,
        dir : dir ,
    }
}

//this variable is useless. only for writing 'module.exports = x',
//so that vs code can complete code.
let module = {}

let _locationStack = []
let _moduleStack   = []
let _loadedModules = {}

function define(func) {
    let module = func()
    _moduleStack.push(module)
}

function require(name) {
    if (!name.endsWith('.js')) {
        name = `${name}.js`
    }

    //get the absolute path of name.
    let base = _locationStack.length > 0 ? _locationStack[_locationStack.length - 1] : null
    let path = _absolutePath(base, name)

    //is the module loaded?
    let module = _loadedModules[path.file]
    if (module) {
        return module
    }

    //load the module:
    let count = _moduleStack.length

    _locationStack.push(path.dir)
    MJsRunFile(path.file)
    _locationStack.pop()

    if (count + 1 == _moduleStack.length) {
        let fresh = _moduleStack.pop()
        _loadedModules[path.file] = fresh
        return fresh
    } else {
        return null
    }
}

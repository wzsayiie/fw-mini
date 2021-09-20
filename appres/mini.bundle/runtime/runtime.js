//------------------------------------------------------------------------------
//MJsLambda:

/**
 * @callback runtime.LambdaFunction
 * @returns {void} 
 */

/** @type {Map<number, runtime.LambdaFunction>} */
let _lambdaPool = new Map()

let _lambdaIden = 0

/** @param {runtime.LambdaFunction} func */
function MJsLambdaInsert(func) {
    let iden = ++_lambdaIden
    _lambdaPool[iden] = func
    return iden
}

/** @param {number} iden */
function MJsLambdaInvoke(iden) {
    let func = _lambdaPool[iden]
    if (func) {
        func()
    }
}

/** @param {number} iden */
function MJsLambdaRemove(iden) {
    delete _lambdaPool[iden]
}

/**
 * @param {runtime.LambdaFunction} func
 *
 * @returns {Object}
 */
function MJsLambda(func) {
    if (typeof func == 'function') {
        let iden = MJsLambdaInsert(func)
        return MJsLambdaWrap(iden)
    }
    return null
}

//------------------------------------------------------------------------------
//require.js module support:

/**
 * @param {string} base
 * @param {string} relative
 */
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

/** @type {Set<string>} */
let _loadingNameSet = new Set()

/** @type {string[]} */
let _baseDirStack = []

/** @type {Object[]} */
let _loadingModuleStack = []

/** @type {Map<string, Object>} */
let _loadedModuleMap = new Map()

/**
 * @callback define.FactoryFunction
 * @return {Object} 
 */

/** @param {define.FactoryFunction} factory */
function define(factory) {
    let module = factory()
    _loadingModuleStack.push(module)
}

/** @param {string} name */
function require(name) {
    if (!name.endsWith('.js')) {
        name = `${name}.js`
    }

    let base = _baseDirStack.length > 0 ? _baseDirStack[_baseDirStack.length - 1] : null
    let path = _absolutePath(base, name)

    //is the module loaded?
    let module = _loadedModuleMap.get(path.file)
    if (module) {
        return module
    }

    //whether it's module circular dependency?
    if (_loadingNameSet.has(path)) {
        return null
    }

    //load the module:
    let count = _loadingModuleStack.length

    _loadingNameSet.add(path.file)
    _baseDirStack.push(path.dir)

    MJsRunFile(path.file)

    _loadingNameSet.delete(path.file)
    _baseDirStack.pop()

    let fresh = null
    if (count + 1 == _loadingModuleStack.length) {
        fresh = _loadingModuleStack.pop()
    }
    _loadedModuleMap.set(path.file, fresh)
    return fresh
}

//------------------------------------------------------------------------------
//javascript native lambda:

const _lambda = (function () {
    const lambda = {}

    /**
     * @callback LambdaFunction
     * @returns  {void} 
     */

    /** @type {Map<number, LambdaFunction>} */
    let lambdaMap = new Map()

    let lambdaIden = 0

    /** @param {LambdaFunction} func */
    lambda.MJsLambda = function (func) {
        if (typeof(func) != 'function') {
            return null
        }

        let iden = ++lambdaIden
        lambdaMap.set(iden, func)

        return MJsLambdaWrap(iden)
    }

    /** @param {number} iden */
    lambda.MJsLambdaInvoke = function (iden) {
        let func = lambdaMap.get(iden)
        if (func) {
            func()
        }
    }

    /** @param {number} iden */
    lambda.MJsLambdaRemove = function (iden) {
        lambdaMap.delete(iden)
    }

    return Object.freeze(lambda)
})()

const MJsLambda       = _lambda.MJsLambda
const MJsLambdaInvoke = _lambda.MJsLambdaInvoke
const MJsLambdaRemove = _lambda.MJsLambdaRemove

//------------------------------------------------------------------------------
//amd module support:

let _amd = (function () {
    const amd = {}

    /**
     * @param {string} base
     * @param {string} relative
     */
    function AbsolutePath(base, relative) {
        if (!relative.startsWith('./') && !relative.startsWith('../')) {
            return relative
        }

        let array = base ? base.split('/') : []
        let added = relative.split('/')

        for (let item of added) {
            if (item == '..') {
                if (array.length == 0 || array[array.length - 1] == '..') {
                    array.push('..')
                } else {
                    array.pop()
                }

            } else if (item == '.') {
                continue

            } else {
                array.push(item)
            }
        }

        return array.join('/')
    }

    /**
     * @param {string}   base
     * @param {string[]} relativePaths
     */
    function AbsolutePathArray(base, relativePaths) {
        let absolutePaths = []
        for (let relative of relativePaths) {
            let absolute = AbsolutePath(base, relative)
            absolutePaths.push(absolute)
        }
        return absolutePaths
    }

    /** @param {Object[]} array */
    function LastOf(array) {
        return array.length > 0 ? array[array.length - 1] : null
    }

    /**
     * @callback ModuleFactory
     * @param    {Object[]}
     * @returns  {Object}
     */

    /**
     * @typedef  ModuleItem
     * @type     {Object}
     * @property {string}        dir
     * @property {string[]}      needArray
     * @property {ModuleFactory} factory
     * @property {Object}        module
     */

    /** @type {Map<string, ModuleItem>} */
    let moduleMap = new Map()

    function requireSync(path) {
        let item = moduleMap.get(path)
        if (item.module) {
            return item.module
        }

        let moduleArray = []
        for (let need of item.needArray) {
            let module = requireSync(need)
            moduleArray.push(module)
        }
        item.module = item.factory.apply(undefined, moduleArray)

        return item.module
    }

    //builtin modules.
    moduleMap.set('require', {module: requireSync})
    moduleMap.set('module' , {module: {}})

    /** @type {string[]} */ let currentPathStack = []
    /** @type {string[]} */ let currentDirStack  = []

    /**
     * @param {string[]}      needs
     * @param {ModuleFactory} factory
     */
    amd.define = function (needs, factory) {
        let path = LastOf(currentPathStack)
        let dir  = LastOf(currentDirStack )

        let item = {
            dir      : dir,
            needArray: AbsolutePathArray(dir, needs),
            factory  : factory,
        }
        moduleMap.set(path, item)
    }

    /**
     * @callback AsyncCallback
     * @returns  {void}
     */

    /**
     * @param {string}        base
     * @param {string[]}      absolutePaths
     * @param {AsyncCallback} complete
     */
    function RequestDefinitions(base, absolutePaths, complete) {
    }

    /**
     * @callback ProcFunction
     * @param    {Object[]}
     * @returns  {void}
     */

    /**
     * @param {string[]}     needs
     * @param {ProcFunction} proc
     */
    amd.require = function (needs, proc) {
        let needArray = AbsolutePathArray(needs)

        RequestDefinitions(null, needArray, () => {
            let moduleArray = []

            for (let need of needArray) {
                let module = requireSync(need)
                moduleArray.push(module)
            }

            proc.apply(undefined, moduleArray)
        })
    }

    return Object.freeze(amd)
})()

const define  = _amd.define
const require = _amd.require

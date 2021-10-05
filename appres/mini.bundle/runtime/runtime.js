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
            func.call(undefined)
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

        let array = !!base ? base.split('/') : []
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
     * @param {string[]} relativeArray
     */
    function AbsolutePathArray(base, relativeArray) {
        let absoluteArray = []

        for (let relative of relativeArray) {
            let absolute = AbsolutePath(base, relative)
            absoluteArray.push(absolute)
        }

        return absoluteArray
    }

    /** @param {string} path */
    function DirOf(path) {
        let div = path.lastIndexOf('/')
        if (div >= 0) {
            return path.substring(0, div)
        } else {
            return ''
        }
    }

    /** @param {string} name */
    function JsFilename(name) {
        return !name.endsWith('.js') ? `${name}.js` : name
    }

    /** @param {string[]} srcArray */
    function JsFilenameArray(srcArray) {
        let dstArray = []

        for (let src of srcArray) {
            let dst = JsFilename(src)
            dstArray.push(dst)
        }

        return dstArray
    }

    /**
     * @callback Generator
     * @param    {any[]} module
     * @returns  {any}
     */

    /**
     * @typedef  GenerationData
     * @type     {Object}
     * @property {string}    localedDir
     * @property {string[]}  needArray
     * @property {Generator} generator
     * @property {boolean}   isGenerated
     * @property {any}       module
     */

    /** @type {Map<string, GenerationData>} */
    let generationMap = new Map()

    /** @type {string[]} */
    let generationDirStack = []

    /** @type {string} path */
    function requireInner(path) {
        path = JsFilename(path)

        let directory = generationDirStack[generationDirStack.length - 1]
        let absolute  = AbsolutePath(directory, path)

        let data = generationMap.get(absolute)
        return data ? data.module : undefined
    }

    //builtin modules.
    generationMap.set('require.js', { isGenerated: true, module: requireInner })
    generationMap.set('module.js' , { isGenerated: true, module: {} })

    /**
     * @param {string}    path
     * @param {string}    dir
     * @param {string[]}  needArray
     * @param {Generator} generator
     */
    function AppendGenerationMap(path, dir, needArray, generator) {
        if (generationMap.has(path)) {
            return
        }

        let data = {}
        data.localedDir = dir
        data.needArray  = needArray
        data.generator  = generator

        generationMap.set(path, data)
    }

    /** @param {string} path */
    function GenerateModule(path) {
        let data = generationMap.get(path)
        if (!data) {
            return undefined
        }

        if (data.isGenerated) {
            return data.module
        }

        //NOTE: first set this flag, to avoid circular dependences.
        data.isGenerated = true

        let dependences = []
        if (data.needArray) {
            for (let need of data.needArray) {
                let module = GenerateModule(need)
                dependences.push(module)
            }
        }

        generationDirStack.push(data.localedDir)
        data.module = data.generator.apply(undefined, dependences)
        generationDirStack.pop()

        return data.module
    }

    /** @type {Set<string>} */
    let requestSet = new Set()

    /** @type {string[]} */
    let requestPathStack = []

    //builtin modules don't need request.
    for (let [path, _] of generationMap) {
        requestSet.add(path)
    }

    function CurrentRequestPath() {
        if (requestPathStack.length > 0) {
            return requestPathStack[requestPathStack.length - 1]
        } else {
            return ''
        }
    }

    /** @param {string[]} pathArray */
    function RequestModules(pathArray) {
        for (let path of pathArray) {
            if (requestSet.has(path)) {
                continue
            }

            requestPathStack.push(path)
            MJsRunFile(path)
            requestPathStack.pop()

            requestSet.add(path)
        }
    }

    /**
     * @param {string[]}  needs
     * @param {Generator} generator
     */
    amd.define = function (needs, generator) {
        needs = JsFilenameArray(needs)

        let currentPath = CurrentRequestPath()
        let currentDir  = DirOf(currentPath)
        let dependences = AbsolutePathArray(currentDir, needs)

        AppendGenerationMap(currentPath, currentDir, dependences, generator)
        RequestModules(dependences)
    }

    /**
     * @param {string[]}  needs
     * @param {Generator} generator
     */
    amd.require = function (needs, generator) {
        needs = JsFilenameArray(needs)

        let currentPath = CurrentRequestPath()
        if (!currentPath) {
            //assume it is the launch entry.
            currentPath = 'app.js'
        }

        let currentDir  = DirOf(currentPath)
        let dependences = AbsolutePathArray(currentDir, needs)

        AppendGenerationMap(currentPath, currentDir, dependences, generator)
        RequestModules(dependences)

        GenerateModule(currentPath)
    }

    return Object.freeze(amd)
})()

const define  = _amd.define
const require = _amd.require

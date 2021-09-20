define(function () {
    const util = {}

    /**
     * @param {string} type
     * @param {any}    object
     */
    util.is = function (type, object) {
        if (type == 'Array') {
            return object && Array.isArray(object)
        }
        if (type == 'Map') {
            return object && Object.getPrototypeOf(object).constructor == Map
        }
        if (type == 'Set') {
            return object && Object.getPrototypeOf(object).constructor == Set
        }

        return typeof object == type
    }

    /**
     * @param {number}   indent
     * @param {string[]} out 
     */
    function MakeIndent(indent, out) {
        while (indent-- > 0) {
            out.push('  ')
        }
    }

    /**
     * @param {boolean}  shrink
     * @param {number}   indent
     * @param {any}      object
     * @param {string[]} out
     */
    function MakeStringWithShrink(shrink, indent, object, out) {
        if (!shrink) {
            MakeIndent(indent, out)
        }

        if (object === null) {
            out.push('null')

        } else if (util.is('Array', object) || util.is('Set', object)) {
            out.push('[\n')

            object.forEach((item) => {
                MakeStringWithShrink(false, indent + 1, item, out)
                out.push(',\n')
            })

            MakeIndent(indent, out)
            out.push(']')

        } else if (util.is('Map', object)) {
            out.push('{\n')

            object.forEach((val, key) => {
                MakeStringWithShrink(false, indent + 1, key, out)
                out.push(': ' )

                MakeStringWithShrink(true , indent + 1, val, out)
                out.push(',\n')
            })

            MakeIndent(indent, out)
            out.push('}')

        } else if (util.is('object', object)) {
            out.push('{\n')

            for (let key in object) {
                let val = object[key]

                MakeStringWithShrink(false, indent + 1, key, out)
                out.push(': ' )

                MakeStringWithShrink(true , indent + 1, val, out)
                out.push(',\n')
            }

            MakeIndent(indent, out)
            out.push('}')

        } else {
            out.push(String(object))
        }
    }

    /** @param {any} object */
    util.string = function (object) {
        let array = []
        MakeStringWithShrink(true, 0, object, array)
        return array.join('')
    }

    /** @param {any} object */
    util.log = function (object) {
        let message = util.string(object)
        MPrintMessage(message)
    }

    /**
     * @callback util.LambdaFunction
     * @returns {void}
     */

    /**
     * @param {util.LambdaFunction} func
     *
     * @returns {Object}
     */
    util.lambda = function (func) {
        return MJsLambda(func)
    }

    return module.exports = util
})

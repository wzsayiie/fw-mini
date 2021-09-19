define(function () {
    const util = {}

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

    function MakeIndent(indent, out) {
        while (indent-- > 0) {
            out.push('  ')
        }
    }

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

    util.string = function (object) {
        let array = []
        MakeStringWithShrink(true, 0, object, array)
        return array.join('')
    }

    util.log = function (object) {
        let message = util.string(object)
        MPrintMessage(message)
    }

    util.lambda = function (func) {
        return MJsLambda(func)
    }

    return module.exports = util
})

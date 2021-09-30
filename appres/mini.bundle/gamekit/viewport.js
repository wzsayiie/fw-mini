define([
    'require'        ,
    'module'         ,
    '../minikit/util',
],
function (require, module) {
    const util = require('../minikit/util')

    let centerX = 0
    let centerY = 0

    /** @type {number} */ let windowWidth  = MWindowWidth ()
    /** @type {number} */ let windowHeight = MWindowHeight()

    function UpdateWindowSize() {
        windowWidth  = MWindowWidth ()
        windowHeight = MWindowHeight()
    }

    MWindowAddListener(util.lambda(() => {
        let event = MWindowCurrentEvent()
        if (event == MWindowEvent_Load || event == MWindowEvent_Resize) {
            UpdateWindowSize()
        }
    }))

    class Viewport {
        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            centerX = x
            centerY = y
        }

        /** @param {number} value */ set x(value) { centerX = value }
        /** @param {number} value */ set y(value) { centerY = value }

        get x() { return centerX }
        get y() { return centerY }

        get width () { return windowWidth  }
        get height() { return windowHeight }
    }
    let viewport = new Viewport()

    return module.exports = viewport
})

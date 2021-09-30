define([
    'require'        ,
    'module'         ,
    './Renderer'     ,
    '../minikit/util',
    './viewport'     ,
],
function (require, module) {
    const Renderer = require('./Renderer')
    const util     = require('../minikit/util')
    const viewport = require('./viewport')

    /** @type {Set<Camera>} */
    let cameraSet = new Set()

    function Draw() {
        //the main camera is processed finally,
        //so that the image drawn by the main camera is on the top.

        for (let camera of cameraSet) {
            if (!camera.isMainCamera) {
                camera.draw()
            }
        }

        for (let camera of cameraSet) {
            if (camera.isMainCamera) {
                camera.draw()
                break
            }
        }
    }

    MWindowAddListener(util.lambda(() => {
        let event = MWindowCurrentEvent()
        if (event == MWindowEvent_Draw) {
            Draw()
        }
    }))

    /** @type {Camera} */
    let mainCamera = null

    class Camera {

        static get mainCamera() {
            if (!mainCamera) {
                mainCamera = new Camera(true)
            }
            return mainCamera
        }

        /** @param {(boolean | undefined)} isMainCamera */
        constructor(isMainCamera) {
            /** @private */
            this._isMainCamera = isMainCamera

            /** @private */ this._x = 0
            /** @private */ this._y = 0

            /** @private */
            this._active = true

            cameraSet.add(this)
        }

        get isMainCamera() {
            return this._isMainCamera
        }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            this._x = x
            this._y = y

            if (this._isMainCamera) {
                viewport.moveTo(x, y)
            }
        }

        /** @param {number} value */ set x(value) { this.moveTo(value  , this._y) }
        /** @param {number} value */ set y(value) { this.moveTo(this._x, value  ) }

        get x() { return this._x }
        get y() { return this._y }

        /** @param {boolean} value */
        set active(value) {
            if /**/ ( value && !this._active) { cameraSet.add   (this) }
            else if (!value &&  this._active) { cameraSet.delete(this) }

            this._active = value
        }

        get active() {
            return this._active
        }

        get viewWidth () { return viewport.width  }
        get viewHeight() { return viewport.height }

        /** @protected */
        draw() {
            Renderer.drawCenteredOn(this._x, this._y)
        }
    }

    return module.exports = Camera
})

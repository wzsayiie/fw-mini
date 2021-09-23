define(function () {
    const Renderer = require('./Renderer')
    const util     = require('../minikit/util')
    const viewport = require('./viewport')

    /** @type {Set<Camera>} */
    let cameraSet = new Set()

    /** @type {Camera} */
    let uiCamera = null

    function Draw() {
        //the ui camera is processed finally,
        //so that the image drawn by the ui camera is on the top.
        for (let camera of cameraSet) {
            if (camera != uiCamera) {
                camera.draw()
            }
        }

        if (uiCamera && uiCamera.active) {
            uiCamera.draw()
        }
    }

    MWindowAddListener(util.lambda(() => {
        let event = MWindowCurrentEvent()
        if (event == MWindowEvent_Draw) {
            Draw()
        }
    }))

    class Camera {

        static get mainCamera() {
            if (!uiCamera) {
                uiCamera = new Camera()
            }
            return uiCamera
        }

        constructor() {
            /** @private */ this._x = 0
            /** @private */ this._y = 0

            /** @private */
            this._active = true

            cameraSet.add(this)
        }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            this._x = x
            this._y = y

            if (this == uiCamera) {
                viewport.moveTo(x, y)
            }
        }

        /** @param {number} value */ set x(value) { this.moveTo(value  , this._y) }
        /** @param {number} value */ set y(value) { this.moveTo(this._x, value  ) }

        get x() { return this._x }
        get y() { return this._y }

        get viewWidth () { return viewport.width  }
        get viewHeight() { return viewport.height }

        /** @param {boolean} value */
        set active(value) {
            if /**/ ( value && !this._active) { cameraSet.add   (this) }
            else if (!value &&  this._active) { cameraSet.delete(this) }

            this._active = value
        }

        get active() {
            return this._active
        }

        /** @protected */
        draw() {
            Renderer.drawCenteredOn(this._x, this._y)
        }
    }

    return module.exports = Camera
})

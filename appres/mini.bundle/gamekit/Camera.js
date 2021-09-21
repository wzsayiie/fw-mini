define(function () {
    const Position = require('./Position')
    const Sprite   = require('./Sprite')
    const context  = require('./context')
    const util     = require('../minikit/util')

    /** @type {Camera} */
    let mainCamera = null

    class Camera {

        static get mainCamera() {
            if (!mainCamera) {
                mainCamera = new Camera()
            }
            return mainCamera
        }

        constructor() {
            /**
             * @private
             * @type {number}
             */
            this._viewWidth = MWindowWidth()

            /**
             * @private
             * @type {number}
             */
            this._viewHeight = MWindowHeight()

            /** @private */
            this._x = 0

            /** @private */
            this._y = 0

            MWindowAddListener(util.lambda(() => {
                this.onWindowEvent()
            }))
        }

        get viewWidth () { return this._viewWidth  }
        get viewHeight() { return this._viewHeight }

        get x() { return this._x }
        get y() { return this._y }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            this._x = x
            this._y = y
        }

        /** @private */
        onWindowEvent() {
            let event = MWindowCurrentEvent()

            switch (event) {
                case MWindowEvent_Load  : this.onLoad  (); break
                case MWindowEvent_Resize: this.onResize(); break
                case MWindowEvent_Draw  : this.onDraw  (); break
            }
        }

        /** @private */
        onLoad() {
            this._viewWidth  = MWindowWidth ()
            this._viewHeight = MWindowHeight()
        }

        /** @private */
        onResize() {
            this._viewWidth  = MWindowWidth ()
            this._viewHeight = MWindowHeight()
        }

        /** @private */
        onDraw() {
            let viewOffsetX = this._viewWidth  / 2 - this._x
            let viewOffsetY = this._viewHeight / 2 - this._y

            Position.topPositions.forEach((item) => {
                this.drawSprites(viewOffsetX, viewOffsetY, item)
            })
        }

        /**
         * @private
         * @param {number}   viewOffsetX
         * @param {number}   viewOffsetY
         * @param {Position} position
         */
        drawSprites(viewOffsetX, viewOffsetY, position) {
            let x = viewOffsetX + position.x
            let y = viewOffsetY + position.y

            let renderer = Sprite.getSprite(position).renderer
            let width    = renderer.width
            let height   = renderer.height

            if (this.isInView(x, y, width, height)) {
                let originX = x - width  / 2
                let originY = y - height / 2
                context.setOffset(originX, originY)

                renderer.onDraw()
            }

            position.children.forEach((item) => {
                this.drawSprites(x, y, item)
            })
        }

        /**
         * @private
         * @param {number} x
         * @param {number} y
         * @param {number} width
         * @param {number} height
         */
        isInView(x, y, width, height) {
            if (width  <= 0) { return false }
            if (height <= 0) { return false }

            if (x + width  / 2 < 0               ) { return false }
            if (x - width  / 2 > this._viewWidth ) { return false }
            if (y + height / 2 < 0               ) { return false }
            if (y - height / 2 > this._viewHeight) { return false }

            return true
        }
    }

    return module.exports = Camera
})

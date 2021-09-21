define(function () {
    const Position = require('./Position')
    const Sprite   = require('./Sprite')
    const context  = require('./context')
    const util     = require('../minikit/util')
    const viewport = require('./viewport')

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
            MWindowAddListener(util.lambda(() => {
                this.onDraw()
            }))
        }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            viewport.x = x
            viewport.y = y
        }

        /** @param {number} value */
        set x(value) { viewport.x = value }

        /** @param {number} value */
        set y(value) { viewport.y = value }

        get x() { return viewport.x }
        get y() { return viewport.y }

        get viewWidth () { return viewport.width  }
        get viewHeight() { return viewport.height }

        /** @private */
        onDraw() {
            let viewOffsetX = viewport.width  / 2 - viewport.x
            let viewOffsetY = viewport.height / 2 - viewport.y

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

            let sprite = Sprite.getSpriteOf(position)
            if (sprite.hasRenderer) {
                let renderer = sprite.renderer
                let width    = renderer.width
                let height   = renderer.height

                if (this.isInView(x, y, width, height)) {
                    let originX = x - width  / 2
                    let originY = y - height / 2
                    context.setOffset(originX, originY)

                    renderer.onDraw()
                }
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

            if (x + width  / 2 < 0              ) { return false }
            if (x - width  / 2 > viewport.width ) { return false }
            if (y + height / 2 < 0              ) { return false }
            if (y - height / 2 > viewport.height) { return false }

            return true
        }
    }

    return module.exports = Camera
})

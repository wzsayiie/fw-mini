define(function () {
    const Feature  = require('./Feature' )
    const context  = require('./context' )
    const viewport = require('./viewport')

    /** @type {Set<Renderer>} */
    let rendererSet = new Set()

    /**
     * @callback Renderer.Drawer
     * @param   {number} width
     * @param   {number} height
     * @returns {void}
     */

    class Renderer extends Feature {

        /**
         * @param {number} centerX
         * @param {number} centerY
         */
        static drawCenteredOn(centerX, centerY) {
            for (let renderer of rendererSet) {
                renderer.drawIfNeed(centerX, centerY)
            }
        }

        constructor(sprite) {
            super(sprite)

            /** @private */ this._width  = 0
            /** @private */ this._height = 0

            /**
             * @private
             * @type {Renderer.Drawer}
             */
            this._drawer = null

            /** @private */
            this._image = null

            /** @private */
            this._color = 0x4488CCFF
        }

        /**
         * @param {number} width
         * @param {number} height
         */
        setSize(width, height) {
            this._width  = width
            this._height = height
        }

        /** @param {number} value */ set width (value) { this._width  = value }
        /** @param {number} value */ set height(value) { this._height = value }

        get width () { return this._width  }
        get height() { return this._height }

        /** @param {Renderer.Drawer} value */
        set drawer(value) { this._drawer = value }

        /** @param {Object} value */
        set image(value) { this._image = value }

        /** @param {number} value */
        set color(value) { this._color = value }

        /** @protected */
        onCreate() {
            rendererSet.add(this)
        }

        /** @protected */
        onDestroy() {
            rendererSet.delete(this)
        }

        /**
         * @private
         * @param {number} centerX
         * @param {number} centerY
         */
        drawIfNeed(centerX, centerY) {
            //is render size empty?
            if (this._width  <= 0) { return }
            if (this._height <= 0) { return }

            //is in the viewport?
            let worldX = this.sprite.position.worldX
            let worldY = this.sprite.position.worldY
            let viewX  = worldX - centerX
            let viewY  = worldY - centerY

            let viewHalfW = viewport.width  / 2
            let viewHalfH = viewport.height / 2
            let selfHalfW = this._width     / 2
            let selfHalfH = this._height    / 2

            if (viewX + selfHalfW < -viewHalfW) { return }
            if (viewX - selfHalfW >  viewHalfW) { return }
            if (viewY + selfHalfH < -viewHalfH) { return }
            if (viewY - selfHalfH >  viewHalfH) { return }

            //the rendering origin is in the lower left corner.
            let originX = viewX - selfHalfW + viewHalfW
            let originY = viewY - selfHalfH + viewHalfH
            context.setOffset(originX, originY)

            this.onDraw()
        }

        /** @protected */
        onDraw() {
            if (this._drawer) {
                this._drawer.call(this.sprite, this._width, this._height)

            } else if (this._image) {
                context.selectImage(this._image)
                context.drawImage(0, 0, this._width, this._height)

            } else {
                context.selectColor(this._color)
                context.drawRect(0, 0, this._width, this._height)
            }
        }
    }

    return module.exports = Renderer
})

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

            /** @private */ this._renderingWidth  = 0
            /** @private */ this._renderingHeight = 0

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
        setRenderingSize(width, height) {
            this._renderingWidth  = width
            this._renderingHeight = height
        }

        /** @param {number} v */ set renderingWidth (v) { this._renderingWidth  = v }
        /** @param {number} v */ set renderingHeight(v) { this._renderingHeight = v }

        get renderingWidth () { return this._renderingWidth  }
        get renderingHeight() { return this._renderingHeight }

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
            //is rendering size empty?
            if (this._renderingWidth  <= 0) { return }
            if (this._renderingHeight <= 0) { return }

            //is in the viewport?
            let worldX = this.sprite.position.worldX
            let worldY = this.sprite.position.worldY
            let viewX  = worldX - centerX
            let viewY  = worldY - centerY

            let viewHalfW = viewport.width        / 2
            let viewHalfH = viewport.height       / 2
            let selfHalfW = this._renderingWidth  / 2
            let selfHalfH = this._renderingHeight / 2

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
            let width  = this._renderingWidth
            let height = this._renderingHeight

            if (this._drawer) {
                this._drawer.call(this.sprite, width, height)

            } else if (this._image) {
                context.selectImage(this._image)
                context.drawImage(0, 0, width, height)

            } else {
                context.selectColor(this._color)
                context.drawRect(0, 0, width, height)
            }
        }
    }

    return module.exports = Renderer
})

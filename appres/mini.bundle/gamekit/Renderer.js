define(function () {
    const Feature = require('./Feature')
    const context = require('./context')

    /**
     * @callback Renderer.Drawer
     * 
     * @param {number} width
     * @param {number} height
     * 
     * @returns {void}
     */

    class Renderer extends Feature {

        constructor(sprite) {
            super(sprite)

            /** @private */
            this._width = 0

            /** @private */
            this._height = 0

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

        /** @param {number} value */
        set width(value) { this._width = value }

        /** @param {number} value */
        set height(value) { this._height = value }

        get width () { return this._width  }
        get height() { return this._height }

        /** @param {Renderer.Drawer} value */
        set drawer(value) { this._drawer = value }

        /** @param {Object} value */
        set image(value) { this._image = value }

        /** @param {number} value */
        set color(value) { this._color = value }

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
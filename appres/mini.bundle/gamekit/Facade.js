define(function () {
    const Feature = require('./Feature')
    const context = require('./context')

    class Facade extends Feature {

        constructor(sprite) {
            super(sprite)

            this._width  = 0
            this._height = 0

            this._drawer = null
            this._image  = null
            this._color  = 0x4488CCFF
        }

        setSize(width, height) {
            this._width  = width
            this._height = height
        }

        get width () { return this._width  }
        get height() { return this._height }

        set drawer(value) { this._drawer = value }
        set image (value) { this._image  = value }
        set color (value) { this._color  = value }

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

    return module.exports = Facade
})

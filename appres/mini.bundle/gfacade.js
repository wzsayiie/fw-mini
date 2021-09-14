include("gcontext.js")
include("gfeature.js")

class GFacade extends GFeature {

    constructor(sprite) {
        super(sprite)

        this._width  = 0
        this._height = 0

        this._drawHandler = null

        this._color = 0
        this._image = null
    }

    setSize(width, height) {
        this._width  = width
        this._height = height
    }

    get width () { return this._width  }
    get height() { return this._height }

    set drawHandler(value) {
        this._drawHandler = value
    }

    set color(value) { this._color = value }
    set image(value) { this._image = value }

    draw() {
        if (this._drawHandler) {
            this._drawHandler(this._width, this._height)
            return
        }

        //default draw a ellipse.
        GContext.current.selectColor(this._color)
        GContext.current.drawEllipse(0, 0, this._width, this._height)
    }
}

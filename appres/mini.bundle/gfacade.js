include("gcontext.js")
include("gfeature.js")

const GFacadeGraph = Object.freeze({
    Ellipse: 1,
    Rect   : 2,
})

class GFacade extends GFeature {

    constructor(sprite) {
        super(sprite)

        this._width  = 0
        this._height = 0

        this._drawer = null
        this._image  = null
        this._graph  = GFacadeGraph.Rect
        this._color  = 0xFFffFFff
    }

    setSize(width, height) {
        this._width  = width
        this._height = height
    }

    get width () { return this._width  }
    get height() { return this._height }

    set drawer(value) { this._drawer = value }
    set image (value) { this._image  = value }
    set graph (value) { this._graph  = value }
    set color (value) { this._color  = value }

    onDraw() {
        if (this._drawer) {
            this._drawer.call(this.sprite, this._width, this._height)

        } else if (this._image) {
            GContext.current.selectImage(this._image)
            GContext.current.drawImage(0, 0, this._width, this._height)

        } else if (this._graph == GFacadeGraph.Ellipse) {
            GContext.current.selectColor(this._color)
            GContext.current.drawEllipse(0, 0, this._width, this._height)
            
        } else if (this._graph == GFacadeGraph.Rect) {
            GContext.current.selectColor(this._color)
            GContext.current.drawRect(0, 0, this._width, this._height)
        }
    }
}

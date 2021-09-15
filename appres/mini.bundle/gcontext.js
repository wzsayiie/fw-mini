include("minikit.js")

//"GContext" takes the lower left corner as the origin,
//the X axis is right and the Y axis is up.

class GContext {

    static get current() {
        if (!this._current) {
            this._current = new GContext()
        }
        return this._current
    }

    constructor() {
        this._windowHeight = MWindowHeight()

        MWindowAddListener(MJsLambda(() => {
            this._onWindowEvent()
        }))
    }

    _onWindowEvent() {
        let event = MWindowCurrentEvent()
        if (event == MWindowEvent.Load || event == MWindowEvent.Resize) {
            this._windowHeight = MWindowHeight()
        }
    }

    setOffset(x, y) { MContextSetOffset(x, this._windowHeight - y) }

    selectColor    (color ) { MContextSelectColor    (color ) }
    selectImage    (image ) { MContextSelectImage    (image ) }
    selectString   (string) { MContextSelectString   (string) }
    selectHAlign   (align ) { MContextSelectHAlign   (align ) }
    selectVAlign   (align ) { MContextSelectVAlign   (align ) }
    selectFontSize (size  ) { MContextSelectFontSize (size  ) }
    selectLineWidth(width ) { MContextSelectLineWidth(width ) }

    drawTriangle0(x0, y0) { MContextDrawTriangle0(x0, this._windowHeight - y0) }
    drawTriangle1(x1, y1) { MContextDrawTriangle1(x1, this._windowHeight - y1) }
    drawTriangle2(x2, y2) { MContextDrawTriangle2(x2, this._windowHeight - y2) }

    drawFlatLine  (x0, y0, x1, y1) { MContextDrawFlatLine  (x0, this._windowHeight - y0, x1, this._windowHeight - y1) }
    drawSquareLine(x0, y0, x1, y1) { MContextDrawSquareLine(x0, this._windowHeight - y0, x1, this._windowHeight - y1) }
    drawRoundLine (x0, y0, x1, y1) { MContextDrawRoundLine (x0, this._windowHeight - y0, x1, this._windowHeight - y1) }

    drawRect   (x, y, width, height) { MContextDrawRect   (x, y - height, width, height) }
    drawEllipse(x, y, width, height) { MContextDrawEllipse(x, y - height, width, height) }
    drawImage  (x, y, width, height) { MContextDrawImage  (x, y - height, width, height) }
    drawString (x, y, width, height) { MContextDrawString (x, y - height, width, height) }
}

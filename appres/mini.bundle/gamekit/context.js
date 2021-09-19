define(function () {
    const util = require('../minikit/util')

    let WH = MWindowHeight()
    function OnWindowEvent() {
        let event = MWindowCurrentEvent()
        if (event == MWindowEvent_Load || event == MWindowEvent_Resize) {
            WH = MWindowHeight()
        }
    }
    MWindowAddListener(util.lambda(() => {
        OnWindowEvent()
    }))

    //'context' takes the lower left corner as the origin,
    //the X axis is right and the Y axis is up.
    let context = {}

    context.setOffset = function (x, y) { MContextSetOffset(x, WH - y) }

    context.selectColor     = function (color ) { MContextSelectColor    (color ) }
    context.selectImage     = function (image ) { MContextSelectImage    (image ) }
    context.selectString    = function (string) { MContextSelectString   (string) }
    context.selectHAlign    = function (align ) { MContextSelectHAlign   (align ) }
    context.selectVAlign    = function (align ) { MContextSelectVAlign   (align ) }
    context.selectFontSize  = function (size  ) { MContextSelectFontSize (size  ) }
    context.selectLineWidth = function (width ) { MContextSelectLineWidth(width ) }

    context.drawTriangle0 = function (x0, y0) { MContextDrawTriangle0(x0, WH - y0) }
    context.drawTriangle1 = function (x1, y1) { MContextDrawTriangle1(x1, WH - y1) }
    context.drawTriangle2 = function (x2, y2) { MContextDrawTriangle2(x2, WH - y2) }

    context.drawFlatLine   = function (x0, y0, x1, y1) { MContextDrawFlatLine  (x0, WH - y0, x1, WH - y1) }
    context.drawSquareLine = function (x0, y0, x1, y1) { MContextDrawSquareLine(x0, WH - y0, x1, WH - y1) }
    context.drawRoundLine  = function (x0, y0, x1, y1) { MContextDrawRoundLine (x0, WH - y0, x1, WH - y1) }

    context.drawRect    = function (x, y, w, h) { MContextDrawRect   (x, y - h, w, h) }
    context.drawEllipse = function (x, y, w, h) { MContextDrawEllipse(x, y - h, w, h) }
    context.drawImage   = function (x, y, w, h) { MContextDrawImage  (x, y - h, w, h) }
    context.drawString  = function (x, y, w, h) { MContextDrawString (x, y - h, w, h) }

    return module.exports = context
})

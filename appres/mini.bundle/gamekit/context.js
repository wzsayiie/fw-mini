define(function () {
    const viewport = require('./viewport')

    //'context' takes the lower left corner as the origin,
    //the X axis is right and the Y axis is up.
    let context = {}

    /**
     * @param {number} x
     * @param {number} y
     */
    context.setOffset = function (x, y) {
        MContextSetOffset(x, viewport.height - y)
    }

    /** @param {number} color */
    context.selectColor = function (color) {
        MContextSelectColor(color)
    }

    /** @param {Object} image */
    context.selectImage = function (image) {
        MContextSelectImage(image)
    }

    /** @param {string} string */
    context.selectString = function (string) {
        MContextSelectString(string)
    }

    /** @param {number} align */
    context.selectHAlign = function (align) {
        MContextSelectHAlign(align)
    }

    /** @param {number} align */
    context.selectVAlign = function (align) {
        MContextSelectVAlign(align)
    }

    /** @param {number} size */
    context.selectFontSize = function (size) {
        MContextSelectFontSize(size)
    }

    /** @param {number} width */
    context.selectLineWidth = function (width) {
        MContextSelectLineWidth(width)
    }

    /**
     * @param {number} x0
     * @param {number} y0
     */
    context.drawTriangle0 = function (x0, y0) {
        MContextDrawTriangle0(x0, viewport.height - y0)
    }

    /**
     * @param {number} x1
     * @param {number} y1
     */
    context.drawTriangle1 = function (x1, y1) {
        MContextDrawTriangle1(x1, viewport.height - y1)
    }

    /**
     * @param {number} x2
     * @param {number} y2
     */
    context.drawTriangle2 = function (x2, y2) {
        MContextDrawTriangle2(x2, viewport.height - y2)
    }

    /**
     * @param {number} x0
     * @param {number} y0
     * @param {number} x1
     * @param {number} y1
     */
    context.drawFlatLine = function (x0, y0, x1, y1) {
        MContextDrawFlatLine(x0, viewport.height - y0, x1, viewport.height - y1)
    }

    /**
     * @param {number} x0
     * @param {number} y0
     * @param {number} x1
     * @param {number} y1
     */
    context.drawSquareLine = function (x0, y0, x1, y1) {
        MContextDrawSquareLine(x0, viewport.height - y0, x1, viewport.height - y1)
    }

    /**
     * @param {number} x0
     * @param {number} y0
     * @param {number} x1
     * @param {number} y1
     */
    context.drawRoundLine = function (x0, y0, x1, y1) {
        MContextDrawRoundLine(x0, viewport.height - y0, x1, viewport.height - y1)
    }

    /**
     * @param {number} x
     * @param {number} y
     * @param {number} width
     * @param {number} height
     */
    context.drawRect = function (x, y, width, height) {
        MContextDrawRect(x, y - height, width, height)
    }

    /**
     * @param {number} x
     * @param {number} y
     * @param {number} width
     * @param {number} height
     */
    context.drawEllipse = function (x, y, width, height) {
        MContextDrawEllipse(x, y - height, width, height)
    }

    /**
     * @param {number} x
     * @param {number} y
     * @param {number} width
     * @param {number} height
     */
    context.drawImage = function (x, y, width, height) {
        MContextDrawImage(x, y - height, width, height)
    }

    /**
     * @param {number} x
     * @param {number} y
     * @param {number} width
     * @param {number} height
     */
    context.drawString = function (x, y, width, height) {
        MContextDrawString(x, y - height, width, height)
    }

    return module.exports = context
})

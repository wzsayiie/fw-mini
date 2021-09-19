define(function () {
    const Site    = require('./Site')
    const context = require('./context')
    const util    = require('../minikit/util')

    class Camera {

        static get mainCamera() {
            if (!this._mainCamera) {
                this._mainCamera = new Camera()
            }
            return this._mainCamera
        }

        constructor() {
            this._viewWidth  = MWindowWidth ()
            this._viewHeight = MWindowHeight()

            this._x = 0
            this._y = 0

            MWindowAddListener(util.lambda(() => {
                this._onWindowEvent()
            }))
        }

        get viewWidth () { return this._viewWidth  }
        get viewHeight() { return this._viewHeight }

        get x() { return this._x }
        get y() { return this._y }

        moveTo(x, y) {
            this._x = x
            this._y = y
        }

        _onWindowEvent() {
            let event = MWindowCurrentEvent()

            switch (event) {
                case MWindowEvent_Load  : this._onLoad  (); break
                case MWindowEvent_Resize: this._onResize(); break
                case MWindowEvent_Draw  : this._onDraw  (); break
            }
        }

        _onLoad() {
            this._viewWidth  = MWindowWidth ()
            this._viewHeight = MWindowHeight()
        }

        _onResize() {
            this._viewWidth  = MWindowWidth ()
            this._viewHeight = MWindowHeight()
        }

        _onDraw() {
            let viewOffsetX = this._viewWidth  / 2 - this._x
            let viewOffsetY = this._viewHeight / 2 - this._y

            Site.topSites.forEach((site) => {
                this._drawSprites(viewOffsetX, viewOffsetY, site)
            })
        }

        _drawSprites(viewOffsetX, viewOffsetY, site) {
            let x = viewOffsetX + site.x
            let y = viewOffsetY + site.y

            let width  = site.sprite.facade.width
            let height = site.sprite.facade.height

            if (this._isInView(x, y, width, height)) {
                let originX = x - width  / 2
                let originY = y - height / 2
                context.setOffset(originX, originY)

                site.sprite.facade.onDraw()
            }

            site.children.forEach((subsite) => {
                this._drawSprites(x, y, subsite)
            })
        }

        _isInView(x, y, width, height) {
            if (width  <= 0) { return false }
            if (height <= 0) { return false }

            if (x + width  / 2 < 0               ) { return false }
            if (x - width  / 2 > this._viewWidth ) { return false }
            if (y + height / 2 < 0               ) { return false }
            if (y - height / 2 > this._viewHeight) { return false }

            return true
        }
    }

    return module.exports = Camera
})

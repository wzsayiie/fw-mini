define(function () {
    const Feature = require('./Feature')

    /** @type {Set<Site>} */
    let topSites = new Set()

    class Site extends Feature {

        static get topSites() {
            return topSites
        }

        constructor(sprite) {
            super(sprite)
            
            /**
             * @private
             * @type {Set<Site>}
             */
            this._children = new Set()

            /**
             * @private
             * @type {Site}
             */
            this._parent = null

            /** @private */
            this._x = 0

            /** @private */
            this._y = 0
        }

        onCreate() {
            //default as top-level object.
            topSites.add(this)
        }

        onDestroy() {
            if (this._parent) {
                this._parent.delete(this)
            } else {
                topSites.delete(this)
            }
        }

        /** @param {Site} fresh */
        set parent(fresh) {
            if (fresh == this._parent) {
                return
            }
            if (fresh == this) {
                return
            }

            //remove from old parent:
            let currentWorldX = this.worldX
            let currentWorldY = this.worldY

            if (this._parent) {
                this._parent._children.delete(this)
            } else {
                topSites.delete(this)
            }

            //add to new parent:
            if (fresh) {
                fresh._children.add(this)

                this._x = currentWorldX - fresh.worldX
                this._y = currentWorldY - fresh.worldY
            } else {
                topSites.add(this)

                this._x = currentWorldX
                this._y = currentWorldY
            }

            this._parent = fresh
        }

        get parent  () { return this._parent   }
        get children() { return this._children }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            this._x = x
            this._y = y
        }

        /** @param {number} value */
        set x(value) { this._x = value }

        /** @param {number} value */
        set y(value) { this._y = value }

        get x() { return this._x }
        get y() { return this._y }

        get worldX() {
            let value = this._x
            for (let site = this._parent; site; site = site._parent) {
                value += site._x
            }
            return value
        }

        get worldY() {
            let value = this._y
            for (let site = this._parent; site; site = site._parent) {
                value += site._y
            }
            return value
        }
    }

    return module.exports = Site
})

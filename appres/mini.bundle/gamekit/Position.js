define(function () {
    const Feature = require('./Feature')

    /** @type {Set<Position>} */
    let topPositions = new Set()

    class Position extends Feature {

        static get topPositions() {
            return topPositions
        }

        constructor(sprite) {
            super(sprite)
            
            /**
             * @private
             * @type {Set<Position>}
             */
            this._children = new Set()

            /**
             * @private
             * @type {Position}
             */
            this._parent = null

            /** @private */
            this._x = 0

            /** @private */
            this._y = 0
        }

        onCreate() {
            //default as top-level object.
            topPositions.add(this)
        }

        onDestroy() {
            if (this._parent) {
                this._parent.delete(this)
            } else {
                topPositions.delete(this)
            }
        }

        /** @param {Position} fresh */
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
                topPositions.delete(this)
            }

            //add to new parent:
            if (fresh) {
                fresh._children.add(this)

                this._x = currentWorldX - fresh.worldX
                this._y = currentWorldY - fresh.worldY
            } else {
                topPositions.add(this)

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

        get worldX() { return this._parent ? this._parent.worldX + this._x : this._x }
        get worldY() { return this._parent ? this._parent.worldY + this._y : this._y }
    }

    return module.exports = Position
})

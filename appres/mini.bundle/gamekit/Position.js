define(function () {
    const Feature = require('./Feature')

    /** @type {Set<Position>} */
    let topPositionSet = new Set()

    class Position extends Feature {

        constructor(sprite) {
            super(sprite)
            
            /**
             * @private
             * @type {Set<Position>}
             */
            this._childSet = new Set()

            /**
             * @private
             * @type {Position}
             */
            this._parent = null

            /** @private */ this._x = 0
            /** @private */ this._y = 0
        }

        /** @protected */
        onCreate() {
            //default as top-level object.
            topPositionSet.add(this)
        }

        /** @protected */
        onDestroy() {
            if (this._parent) {
                this._parent.delete(this)
            } else {
                topPositionSet.delete(this)
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
                this._parent._childSet.delete(this)
            } else {
                topPositionSet.delete(this)
            }

            //add to new parent:
            if (fresh) {
                fresh._childSet.add(this)

                this._x = currentWorldX - fresh.worldX
                this._y = currentWorldY - fresh.worldY
            } else {
                topPositionSet.add(this)

                this._x = currentWorldX
                this._y = currentWorldY
            }

            this._parent = fresh
        }

        get parent  () { return this._parent   }
        get childSet() { return this._childSet }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            this._x = x
            this._y = y
        }

        /** @param {number} value */ set x(value) { this._x = value }
        /** @param {number} value */ set y(value) { this._y = value }

        get x() { return this._x }
        get y() { return this._y }

        get worldX() {
            if (this._parent) {
                return this._parent.worldX + this._x
            } else {
                return this._x
            }
        }

        get worldY() {
            if (this._parent) {
                return this._parent.worldY + this._y
            } else {
                return this._y
            }
        }
    }

    return module.exports = Position
})

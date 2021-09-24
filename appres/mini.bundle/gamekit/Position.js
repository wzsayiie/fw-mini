define(function () {
    const Feature = require('./Feature')

    /** @type {Set<Position>} */
    let topPositionSet = new Set()

    class Position extends Feature {

        static get topPositionSet() {
            return topPositionSet
        }

        constructor(sprite) {
            super(sprite)
            
            /** @private */ this._worldX = 0
            /** @private */ this._worldY = 0

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
        }

        /** @protected */
        onCreate() {
            //default as top-level object.
            topPositionSet.add(this)
        }

        /** @protected */
        onDestroy() {
            if (this._parent) {
                this._parent._childSet.delete(this)
            } else {
                topPositionSet.delete(this)
            }
        }

        get worldX() { return this._worldX }
        get worldY() { return this._worldY }

        /** @param {Position} fresh */
        set parent(fresh) {
            if (fresh == this._parent) {
                return
            }
            if (fresh == this) {
                return
            }

            //remove from old parent.
            if (this._parent) {
                this._parent._childSet.delete(this)
            } else {
                topPositionSet.delete(this)
            }

            //add to new parent.
            if (fresh) {
                fresh._childSet.add(this)
            } else {
                topPositionSet.add(this)
            }
            this._parent = fresh
        }

        get parent  () { return this._parent   }
        get childSet() { return this._childSet }

        /** @private */
        get parentX() {
            return this._parent ? this._parent._worldX : 0
        }

        /** @private */
        get parentY() {
            return this._parent ? this._parent._worldY : 0
        }

        /**
         * @param {number} x
         * @param {number} y
         */
        moveTo(x, y) {
            this._worldX = this.parentX + x
            this._worldY = this.parentY + y
        }

        /** @param {number} value */
        set x(value) {
            this._worldX = this.parentX + value
        }

        /** @param {number} value */
        set y(value) {
            this._worldY = this.parentY + value
        }

        get x() { return this._worldX - this.parentX }
        get y() { return this._worldY - this.parentY }
    }

    return module.exports = Position
})

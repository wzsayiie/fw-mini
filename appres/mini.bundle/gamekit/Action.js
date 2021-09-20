define(function () {
    const Feature = require('./Feature')
    const util    = require('../minikit/util')

    const UpdateInterval = 0.1

    /** @type {Set<Action>} */
    let actionSet = new Set()

    function OnUpdate() {
        actionSet.forEach((action) => {
            action.onUpdate()
        })
    }
    MRunEverySeconds(UpdateInterval, util.lambda(() => {
        OnUpdate()
    }))

    class Action extends Feature {

        constructor(sprite) {
            super(sprite)

            /** @private */
            this._initial = true

            /**
             * @private
             * @type {Function}
             */
            this._awaker = null

            /**
             * @private
             * @type {Function}
             */
            this._updater = null
        }

        /** @param {Function} value */
        set updater(value) { this._updater = value }

        /** @param {Function} value */
        set awaker(value) { this._awaker = value }

        onCreate() {
            actionSet.add(this)
        }

        onDestroy() {
            actionSet.delete(this)
        }

        /** @private */
        onUpdate() {
            if (this._initial) {
                this._initial = false
                if (this._awaker) {
                    this._awaker.call(this.sprite)
                }
            }

            if (this._updater) {
                this._updater.call(this.sprite)
            }
        }
    }

    return module.exports = Action
})

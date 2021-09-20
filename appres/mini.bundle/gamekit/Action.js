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

    /**
     * @callback Action.Awaker
     * @returns {void}
     */

    /**
     * @callback Action.Updater
     * @returns {void}
     */

    class Action extends Feature {

        constructor(sprite) {
            super(sprite)

            /** @private */
            this._initial = true

            /**
             * @private
             * @type {Action.Awaker}
             */
            this._awaker = null

            /**
             * @private
             * @type {Action.Updater}
             */
            this._updater = null
        }

        /** @param {Action.Awaker} value */
        set awaker(value) { this._awaker = value }

        /** @param {Action.Updater} value */
        set updater(value) { this._updater = value }

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

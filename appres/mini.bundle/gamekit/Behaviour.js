define(function () {
    const Feature = require('./Feature')
    const util    = require('../minikit/util')

    const UpdateInterval = 0.1

    /** @type {Set<Behaviour>} */
    let behaviourSet = new Set()

    function OnUpdate() {
        behaviourSet.forEach((item) => {
            item.onUpdate()
        })
    }
    MRunEverySeconds(UpdateInterval, util.lambda(() => {
        OnUpdate()
    }))

    /**
     * @callback Behaviour.Awaker
     * @returns {void}
     */

    /**
     * @callback Behaviour.Updater
     * @returns {void}
     */

    class Behaviour extends Feature {

        constructor(sprite) {
            super(sprite)

            /** @private */
            this._initial = true

            /**
             * @private
             * @type {Behaviour.Awaker}
             */
            this._awaker = null

            /**
             * @private
             * @type {Behaviour.Updater}
             */
            this._updater = null
        }

        /** @param {Behaviour.Awaker} value */
        set awaker(value) { this._awaker = value }

        /** @param {Behaviour.Updater} value */
        set updater(value) { this._updater = value }

        onCreate() {
            behaviourSet.add(this)
        }

        onDestroy() {
            behaviourSet.delete(this)
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

    return module.exports = Behaviour
})

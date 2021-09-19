define(function () {
    const Feature = require('./Feature')
    const util = require('../minikit/util')

    const UpdateInterval = 0.1

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

            this._initial = true
            this._awaker  = null
            this._updater = null
        }

        set updater(value) { this._updater = value }
        set awaker (value) { this._awaker  = value }

        onCreate() {
            actionSet.add(this)
        }

        onDestory() {
            actionSet.delete(this)
        }

        onUpdate() {
            if (this._initial) {
                this._initial = false
                if (this._awaker) {
                    this._awaker.call(this.sprite)
                }

            } else {
                if (this._updater) {
                    this._updater.call(this.sprite)
                }
            }
        }
    }

    return module.exports = Action
})

include("gfeature.js")

const GActionUpdateInterval = 0.1

class GAction extends GFeature {

    static get actions() {
        if (!this._actions) {
            this._actions = new Set()
            MRunEverySeconds(GActionUpdateInterval, MJsLambda(() => {
                this.update()
            }))
        }
        return this._actions
    }

    static update() {
        this._actions.forEach((action) => {
            action.onUpdate()
        })
    }

    constructor(sprite) {
        super(sprite)

        this._initial = true
        this._awaker  = null
        this._updater = null
    }

    set updater(value) { this._updater = value }
    set awaker (value) { this._awaker  = value }

    onCreate() {
        GAction.actions.add(this)
    }

    onDestory() {
        GAction.actions.delete(this)
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

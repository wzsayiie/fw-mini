define(function () {
    const Action = require('./Action')
    const Facade = require('./Facade')
    const Site   = require('./Site'  )

    let classes = {
        'action': Action,
        'facade': Facade,
        'site'  : Site  ,
    }

    class Sprite {

        constructor() {
            this._destroyed = false
            this._features = {}

            //feature 'site' is default.
            this.getFeature('site')
        }

        getFeature(name) {
            if (this._destroyed) {
                return null
            }

            //is the feature exist?
            let feature = this._features[name]
            if (feature) {
                return feature
            }

            //try create the feature.
            let cls = classes[name]
            if (cls) {
                let fresh = new cls(this)
                fresh.onCreate()
                this._features[name] = fresh
                return fresh
            }

            return null
        }

        get action() { return this.getFeature('action') }
        get facade() { return this.getFeature('facade') }
        get site  () { return this.getFeature('site'  ) }

        destroy() {
            //destroy children.
            this.site.children.forEach((site) => {
                site.sprite.destroy()
            })

            //destroy self.
            this._features.forEach((feature) => {
                feature.onDestroy()
            })
            this._features = null

            this._destroyed = true
        }

        get destroyed() {
            return this._destroyed
        }
    }

    return module.exports = Sprite
})

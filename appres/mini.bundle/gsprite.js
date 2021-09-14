include("gaction.js")
include("gfacade.js")
include("gsite.js")

let _SpriteFeatureClasses = {
    "action": GAction,
    "facade": GFacade,
    "site"  : GSite  ,
}

class GSprite {

    getFeature(name) {
        if (this._destroyed) {
            return null
        }

        //is the feature exist?
        if (this._features.has(name)) {
            return this._features.get(name)
        }

        //try create the feature.
        let clazz = _SpriteFeatureClasses[name]
        if (clazz) {
            let feature = new clazz(this)
            feature.onCreate()

            this._features.set(name, feature)
            
            return feature
        }

        return null
    }

    constructor() {
        this._destroyed = false
        this._features = new Map()

        //the "site" is default.
        this.getFeature("site")
    }

    get action() { return this.getFeature("action") }
    get facade() { return this.getFeature("facade") }
    get site  () { return this.getFeature("site"  ) }

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

define(function () {
    const Action  = require('./Action' )
    const Facade  = require('./Facade' )
    const Feature = require('./Feature')
    const Site    = require('./Site'   )

    class Sprite {

        /**
         * @param {Feature} feature
         * 
         * @returns {Sprite}
         */
        static getSprite(feature) {
            return feature.sprite
        }

        constructor() {
            /** @private */
            this._destroyed = false

            /**
             * @private
             * @type {Map<Object, Feature>}
             */
            this._features = new Map()

            //feature 'site' is default.
            this.getFeature(Site)
        }

        /**
         * @param {Object} cls
         *
         * @returns {Feature}
         */
        getFeature(cls) {
            if (this._destroyed) {
                return null
            }

            //is the feature exist?
            let feature = this._features[cls]
            if (feature) {
                return feature
            }

            //create the feature.
            let fresh = new cls(this)
            fresh.onCreate()
            this._features[cls] = fresh
            return fresh
        }

        /** @returns {Action} */
        get action() { return this.getFeature(Action) }

        /** @returns {Facade} */
        get facade() { return this.getFeature(Facade) }

        /** @returns {Site} */
        get site() { return this.getFeature(Site) }

        destroy() {
            //destroy children.
            this.site.children.forEach((site) => {
                Sprite.getSprite(site).destroy()
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

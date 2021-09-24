define(function () {
    const Behaviour = require('./Behaviour')
    const Feature   = require('./Feature'  )
    const Position  = require('./Position' )
    const Renderer  = require('./Renderer' )

    class Sprite {

        /**
         * @param   {Feature} feature
         * @returns {Sprite}
         */
        static getSpriteOf(feature) {
            return feature.sprite
        }

        constructor() {
            /** @private */
            this._destroyed = false

            /**
             * @private
             * @type {Map<Object, Feature>}
             */
            this._featureMap = new Map()

            //feature 'position' is default.
            this.getFeature(Position)
        }

        /**
         * @param   {Object } cls
         * @returns {Feature}
         */
        getFeature(cls) {
            if (this._destroyed) {
                return null
            }

            //is the feature exist?
            let feature = this._featureMap.get(cls)
            if (feature) {
                return feature
            }

            //create the feature.
            let fresh = new cls(this)
            fresh.onCreate()
            this._featureMap.set(cls, fresh)
            return fresh
        }

        /** @returns {Behaviour} */ get behaviour() { return this.getFeature(Behaviour) }
        /** @returns {Position } */ get position () { return this.getFeature(Position ) }
        /** @returns {Renderer } */ get renderer () { return this.getFeature(Renderer ) }

        /**
         * @param   {Object } cls
         * @returns {boolean}
         */
        isThereFeature(cls) {
            return !this._destroyed && !!this._featureMap.get(cls)
        }

        get hasBehaviour() { return this.isThereFeature(Behaviour) }
        get hasPosition () { return this.isThereFeature(Position ) }
        get hasRenderer () { return this.isThereFeature(Renderer ) }

        destroy() {
            //destroy children.
            for (let child of this.position.childSet) {
                Sprite.getSpriteOf(child).destroy()
            }

            //destroy self.
            for (let [_, feature] of this._featureMap) {
                feature.onDestroy()
            }
            this._featureMap = null

            this._destroyed = true
        }

        get destroyed() {
            return this._destroyed
        }
    }

    return module.exports = Sprite
})

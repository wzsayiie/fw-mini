define([
    'require'    ,
    'module'     ,
    './Behaviour',
    './Feature'  ,
    './Position' ,
    './Renderer' ,
    './RigidBody',
],
function (require, module) {
    const Behaviour = require('./Behaviour')
    const Feature   = require('./Feature'  )
    const Position  = require('./Position' )
    const Renderer  = require('./Renderer' )
    const RigidBody = require('./RigidBody')

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
        /** @returns {RigidBody} */ get rigidBody() { return this.getFeature(RigidBody) }

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
        get hasRigidBody() { return this.isThereFeature(RigidBody) }

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

        /** @param {number} v */ set x     (v) { this.position.x       = v }
        /** @param {number} v */ set y     (v) { this.position.y       = v }
        /** @param {number} v */ set width (v) { this.rigidBody.width  = v }
        /** @param {number} v */ set height(v) { this.rigidBody.height = v }
        /** @param {number} v */ set mass  (v) { this.rigidBody.mass   = v }
        /** @param {number} v */ set vx    (v) { this.rigidBody.vx     = v }
        /** @param {number} v */ set vy    (v) { this.rigidBody.vy     = v }

        get x     () { return this.position.x       }
        get y     () { return this.position.y       }
        get width () { return this.rigidBody.width  }
        get height() { return this.rigidBody.height }
        get mass  () { return this.rigidBody.mass   }
        get vx    () { return this.rigidBody.vx     }
        get vy    () { return this.rigidBody.vy     }
    }

    return module.exports = Sprite
})

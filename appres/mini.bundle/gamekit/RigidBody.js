define([
    'require'  ,
    'module'   ,
    './Feature',
],
function (require, module) {
    const Feature = require('./Feature')

    class RigidBody extends Feature {

        constructor(sprite) {
            super(sprite)

            this.width  = 0
            this.height = 0
            this.mass   = 1
            this.vx     = 0
            this.vy     = 0
        }

        /** @protected */ onCreate () {}
        /** @protected */ onDestroy() {}
    }

    return module.exports = RigidBody
})

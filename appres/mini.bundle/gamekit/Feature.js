define([
    'module',
],
function (module) {
    class Feature {

        constructor(sprite) {
            /** @private */
            this._sprite = sprite
        }

        get sprite() {
            return this._sprite
        }

        onCreate () {}
        onDestroy() {}
    }

    return module.exports = Feature
})

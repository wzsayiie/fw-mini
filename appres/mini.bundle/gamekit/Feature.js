define(function () {
    class Feature {

        constructor(sprite) {
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

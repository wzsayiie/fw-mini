include("minikit.js")

class GFeature {

    constructor(sprite) {
        this._sprite = sprite
    }

    get sprite() {
        return this._sprite
    }

    onCreate () {}
    onDestroy() {}
}

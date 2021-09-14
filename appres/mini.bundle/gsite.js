include("gfeature.js")

class GSite extends GFeature {

    static get topSites() {
        if (!this._topSites) {
            this._topSites = new Set()
        }
        return this._topSites
    }

    constructor(sprite) {
        super(sprite)
        
        this._children = new Set()
        this._parent = null

        this._x = 0
        this._y = 0
    }

    onCreate() {
        //default as top-level object.
        GSite.topSites.add(this)
    }

    onDestroy() {
        if (this._parent) {
            this._parent.delete(this)
        } else {
            GSite.topSites.delete(this)
        }
    }

    set parent(fresh) {
        if (fresh == this._parent) {
            return
        }
        if (fresh == this) {
            return
        }

        //remove from old parent:
        let currentWorldX = this.worldX
        let currentWorldY = this.worldY

        if (this._parent) {
            this._parent._children.delete(this)
        } else {
            GSite.topSites.delete(this)
        }

        //add to new parent:
        if (fresh) {
            fresh._children.add(this)

            this._x = currentWorldX - fresh.worldX
            this._y = currentWorldY - fresh.worldY
        } else {
            GSite.topSites.add(this)

            this._x = currentWorldX
            this._y = currentWorldY
        }

        this._parent = fresh
    }

    get parent() {
        return this._parent
    }

    get children() {
        return this._children
    }

    moveTo(x, y) {
        this._x = x
        this._y = y
    }

    get x() { return this._x }
    get y() { return this._y }

    get worldX() {
        let value = this._x
        for (let site = this._parent; site; site = site._parent) {
            value += site._x
        }
        return value
    }

    get worldY() {
        let value = this._y
        for (let site = this._parent; site; site = site._parent) {
            value += site._y
        }
        return value
    }
}

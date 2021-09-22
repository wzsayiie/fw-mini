define(function () {
    const Feature  = require('./Feature')
    const util     = require('../minikit/util')
    const viewport = require('./viewport')

    /** @type {Set<Behaviour>} */
    let behaviourSet = new Set()

    /** @type {Behaviour} */
    let touchingBehaviour = null

    function OnUpdate() {
        //NOTE: create a copy of 'behaviourSet' to iterate.
        //because it's possible to modify 'behaviourSet' during the traversal.
        let set = new Set(behaviourSet)

        for (let behaviour of set) {
            behaviour.onUpdate()
        }
    }

    /**
     * @param {number} x
     * @param {number} y
     */
    function OnTouchBegin(x, y) {
        //convert the coordinate.
        let viewX =  x - viewport.width  / 2 + viewport.x
        let viewY = -y + viewport.height / 2 + viewport.y

        let target  = null
        let spriteX = 0
        let spriteY = 0

        let set = new Set(behaviourSet)
        for (let behaviour of set) {
            spriteX = viewX - behaviour.sprite.position.worldX
            spriteY = viewY - behaviour.sprite.position.worldY

            if (behaviour.touchableAt(spriteX, spriteY)) {
                target = behaviour
                break
            }
        }

        if (target) {
            touchingBehaviour = target
            touchingBehaviour.onTouchBegin(spriteX, spriteY)
        }
    }

    /**
     * @param {number} x
     * @param {number} y
     */
    function OnTouchMove(x, y) {
        if (!touchingBehaviour) {
            return
        }

        let viewX =  x - viewport.width  / 2 + viewport.x
        let viewY = -y + viewport.height / 2 + viewport.y

        let spriteX = viewX - touchingBehaviour.sprite.position.worldX
        let spriteY = viewY - touchingBehaviour.sprite.position.worldY

        touchingBehaviour.onTouchMove(spriteX, spriteY)
    }

    /**
     * @param {number} x
     * @param {number} y
     */
    function OnTouchEnd(x, y) {
        if (!touchingBehaviour) {
            return
        }

        let viewX =  x - viewport.width  / 2 + viewport.x
        let viewY = -y + viewport.height / 2 + viewport.y

        let spriteX = viewX - touchingBehaviour.sprite.position.worldX
        let spriteY = viewY - touchingBehaviour.sprite.position.worldY

        touchingBehaviour.onTouchEnd(spriteX, spriteY)
        touchingBehaviour = null
    }

    /**
     * @param {string}  string
     * @param {boolean} enter
     */
    function OnTextBox(string, enter) {
        let set = new Set(behaviourSet)
        for (let behaviour of set) {
            behaviour.onTextBox(string, enter)
        }
    }

    /** @param {number} key */
    function OnKeyDown(key) {
        let set = new Set(behaviourSet)
        for (let behaviour of set) {
            behaviour.onKeyDown(key)
        }
    }

    MAppAddUpdateListener(util.lambda(() => {
        OnUpdate()
    }))

    MWindowAddListener(util.lambda(() => {
        let event = MWindowCurrentEvent()

        if (event == MWindowEvent_TouchBegin) {
            let x = MWindowTouchX()
            let y = MWindowTouchY()
            OnTouchBegin(x, y)

        } else if (event == MWindowEvent_TouchMove) {
            let x = MWindowTouchX()
            let y = MWindowTouchY()
            OnTouchMove(x, y)

        } else if (event == MWindowEvent_TouchEnd) {
            let x = MWindowTouchX()
            let y = MWindowTouchY()
            OnTouchEnd(x, y)

        } else if (event == MWindowEvent_TextBox) {
            let string = MWindowTextBoxString()
            let enter  = MWindowTextBoxEnter ()
            OnTextBox(string, enter)

        } else if (event == MWindowEvent_KeyDown) {
            let key = MWindowActiveKey()
            OnKeyDown(key)
        }
    }))

    /**
     * @callback Behaviour.AwakeListener
     * @returns {void}
     */

    /**
     * @callback Behaviour.UpdateListener
     * @returns {void}
     */

    /**
     * @callback Behaviour.TouchBeginListener
     * @param   {number} x
     * @param   {number} y
     * @returns {void}
     */

    /**
     * @callback Behaviour.TouchMoveListener
     * @param   {number} x
     * @param   {number} y
     * @returns {void}
     */

    /**
     * @callback Behaviour.TouchEndListener
     * @param   {number} x
     * @param   {number} y
     * @returns {void}
     */

    /**
     * @callback Behaviour.TextBoxListener
     * @param   {string}  string
     * @param   {boolean} enter
     * @returns {void}
     */

    /**
     * @callback Behaviour.KeyDownListener
     * @param   {number} key
     * @returns {void}
     */

    class Behaviour extends Feature {

        constructor(sprite) {
            super(sprite)

            /** @private */ this._awaked = false

            /** @private */ this._touchableWidth  = 0
            /** @private */ this._touchableHeight = 0

            /** @private @type {Behaviour.AwakeListener     } */ this._awakeListener      = null
            /** @private @type {Behaviour.UpdateListener    } */ this._updateListener     = null
            /** @private @type {Behaviour.TouchBeginListener} */ this._touchBeginListener = null
            /** @private @type {Behaviour.TouchMoveListener } */ this._touchMoveListener  = null
            /** @private @type {Behaviour.TouchEndListener  } */ this._touchEndListener   = null
            /** @private @type {Behaviour.TextBoxListener   } */ this._textBoxListener    = null
            /** @private @type {Behaviour.KeyDownListener   } */ this._keyDownListener    = null
        }

        /**
         * @param {number} width
         * @param {number} height
         */
         setTouchableSize(width, height) {
            this._touchableWidth  = width
            this._touchableHeight = height
        }

        /** @param {number} value */ set touchableWidth (value) { this._touchableWidth  = value }
        /** @param {number} value */ set touchableHeight(value) { this._touchableHeight = value }

        /** @param {Behaviour.AwakeListener     } v */ set awakeListenrt     (v) { this._awakeListener      = v }
        /** @param {Behaviour.UpdateListener    } v */ set updateListener    (v) { this._updateListener     = v }
        /** @param {Behaviour.TouchBeginListener} v */ set touchBeginListener(v) { this._touchBeginListener = v }
        /** @param {Behaviour.TouchMoveListener } v */ set touchMoveListener (v) { this._touchMoveListener  = v }
        /** @param {Behaviour.TouchEndListener  } v */ set touchEndListener  (v) { this._touchEndListener   = v }
        /** @param {Behaviour.TextBoxListener   } v */ set textBoxListener   (v) { this._textBoxListener    = v }
        /** @param {Behaviour.KeyDownListener   } v */ set keydownListener   (v) { this._keyDownListener    = v }

        /** @protected */
        onCreate() {
            behaviourSet.add(this)
        }

        /** @protected */
        onDestroy() {
            behaviourSet.delete(this)
        }

        /** @private */
        awakeIfNeed() {
            if (this._awaked) {
                return
            }

            if (this._awakeListener) {
                this._awakeListener.call(this.sprite)
            }
            this._awaked = true
        }

        /** @protected */
        onUpdate() {
            this.awakeIfNeed()

            if (this._updateListener) {
                this._updateListener.call(this.sprite)
            }
        }

        /**
         * @protected
         * @param {number} x
         * @param {number} y
         */
        touchableAt(x, y) {
            if (this._touchableWidth  <= 0) { return false }
            if (this._touchableHeight <= 0) { return false }

            let halfW = this._touchableWidth  / 2
            let halfH = this._touchableHeight / 2

            if (x < -halfW || halfW < x) { return false }
            if (y < -halfH || halfH < y) { return false }

            return true
        }

        /**
         * @protected
         * @param {number} x
         * @param {number} y
         */
        onTouchBegin(x, y) {
            this.awakeIfNeed()

            if (this._touchBeginListener) {
                this._touchBeginListener.call(this.sprite, x, y)
            }
        }

        /**
         * @protected
         * @param {number} x
         * @param {number} y
         */
        onTouchMove(x, y) {
            if (this._touchMoveListener) {
                this._touchMoveListener.call(this.sprite, x, y)
            }
        }

        /**
         * @protected
         * @param {number} x
         * @param {number} y
         */
        onTouchEnd(x, y) {
            if (this._touchEndListener) {
                this._touchEndListener.call(this.sprite, x, y)
            }
        }

        /**
         * @protected
         * @param {string}  string
         * @param {boolean} enter
         */
        onTextBox(string, enter) {
            this.awakeIfNeed()

            if (this._textBoxListener) {
                this._textBoxListener.call(this.sprite, string, enter)
            }
        }

        /**
         * @protected
         * @param {number} key
         */
        onKeyDown(key) {
            this.awakeIfNeed()

            if (this._keyDownListener) {
                this._keyDownListener.call(this.sprite, key)
            }
        }
    }

    return module.exports = Behaviour
})

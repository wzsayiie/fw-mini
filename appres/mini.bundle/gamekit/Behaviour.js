define([
    'require'        ,
    'module'         ,
    './Feature'      ,
    '../minikit/util',
    './viewport'     ,
],
function (require, module) {
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
            if (behaviour.isListenUpdate) {
                behaviour.onUpdate()
            }
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
            //NOTE: even if the object does not listen touch events, perform a hit test.

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
            if (behaviour.isListenTextBox) {
                behaviour.onTextBox(string, enter)
            }
        }
    }

    /** @param {number} key */
    function OnKeyDown(key) {
        let set = new Set(behaviourSet)
        for (let behaviour of set) {
            if (behaviour.isListenKeyDown) {
                behaviour.onKeyDown(key)
            }
        }
    }

    /**
     * @param {number} x
     * @param {number} y
     */
    function OnMouseMove(x, y) {
        let viewX =  x - viewport.width  / 2 + viewport.x
        let viewY = -y + viewport.height / 2 + viewport.y

        let set = new Set(behaviourSet)
        for (let behaviour of set) {
            if (!behaviour.isListenMouseMove) {
                continue
            }

            let spriteX = viewX - behaviour.sprite.position.worldX
            let spriteY = viewY - behaviour.sprite.position.worldY

            behaviour.onMouseMove(spriteX, spriteY)
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

        } else if (event == MWindowEvent_MouseMove) {
            let x = MWindowMouseX()
            let y = MWindowMouseY()
            OnMouseMove(x, y)
        }
    }))

    /**
     * @callback AwakeListener
     * @returns  {void}
     */

    /**
     * @callback UpdateListener
     * @returns  {void}
     */

    /**
     * @callback PositionListener
     * @param    {number} x
     * @param    {number} y
     * @returns  {void}
     */

    /**
     * @callback TextBoxListener
     * @param    {string}  string
     * @param    {boolean} enter
     * @returns  {void}
     */

    /**
     * @callback KeyDownListener
     * @param    {number} key
     * @returns  {void}
     */

    class Behaviour extends Feature {

        constructor(sprite) {
            super(sprite)

            /** @private */ this._awaked = false

            /** @private */ this._touchableWidth  = 0
            /** @private */ this._touchableHeight = 0

            /** @private @type {AwakeListener   } */ this._awakeListener      = null
            /** @private @type {UpdateListener  } */ this._updateListener     = null
            /** @private @type {PositionListener} */ this._touchBeginListener = null
            /** @private @type {PositionListener} */ this._touchMoveListener  = null
            /** @private @type {PositionListener} */ this._touchEndListener   = null
            /** @private @type {TextBoxListener } */ this._textBoxListener    = null
            /** @private @type {KeyDownListener } */ this._keyDownListener    = null
            /** @private @type {PositionListener} */ this._mouseMoveListener  = null
        }

        /**
         * @param {number} width
         * @param {number} height
         */
         setTouchableSize(width, height) {
            this._touchableWidth  = width
            this._touchableHeight = height
        }

        /** @param {number} v */ set touchableWidth (v) { this._touchableWidth  = v }
        /** @param {number} v */ set touchableHeight(v) { this._touchableHeight = v }

        /** @param {AwakeListener   } v */ set awakeListenrt     (v) { this._awakeListener      = v }
        /** @param {UpdateListener  } v */ set updateListener    (v) { this._updateListener     = v }
        /** @param {PositionListener} v */ set touchBeginListener(v) { this._touchBeginListener = v }
        /** @param {PositionListener} v */ set touchMoveListener (v) { this._touchMoveListener  = v }
        /** @param {PositionListener} v */ set touchEndListener  (v) { this._touchEndListener   = v }
        /** @param {TextBoxListener } v */ set textBoxListener   (v) { this._textBoxListener    = v }
        /** @param {KeyDownListener } v */ set keydownListener   (v) { this._keyDownListener    = v }
        /** @param {PositionListener} v */ set mouseMoveListener (v) { this._mouseMoveListener  = v }

        get isListenUpdate   () { return !!this._updateListener    }
        get isListenTextBox  () { return !!this._textBoxListener   }
        get isListenKeyDown  () { return !!this._keydownListener   }
        get isListenMouseMove() { return !!this._mouseMoveListener }

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

        /**
         * @protected
         * @param {number} x
         * @param {number} y
         */
        onMouseMove(x, y) {
            this.awakeIfNeed()

            if (this._mouseMoveListener) {
                this._mouseMoveListener.call(this.sprite, x, y)
            }
        }
    }

    return module.exports = Behaviour
})

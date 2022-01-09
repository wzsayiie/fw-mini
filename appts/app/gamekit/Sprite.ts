import { Camera } from './Camera'

export class Sprite {

    private static s_initialized    = false
    private static s_sprites        = new Map<number, Sprite>()
    private static s_spriteCount    = 0
    private static s_touchingSprite = <Sprite> null

    public static Initialize(): void {
        if (this.s_initialized) {
            return
        }
        this.s_initialized = true

        MWindowAddListener(() => {
            let event = MWindowCurrentEvent()

            if /**/ (event == MWindowEvent_Draw      ) { this.OnDraw      () }
            else if (event == MWindowEvent_TouchBegin) { this.OnTouchBegin() }
            else if (event == MWindowEvent_TouchMove ) { this.OnTouchMove () }
            else if (event == MWindowEvent_TouchEnd  ) { this.OnTouchEnd  () }
        })
    }

    private static OnDraw(): void {
        let sprites = this.GetSortedSprites(-1)
        for (let sprite of sprites) {
            this.DrawSprite(sprite)
        }
    }

    private static DrawSprite(sprite: Sprite): void {
        let w = sprite._drawableW
        let h = sprite._drawableH

        if (w <= 0 || h <= 0) {
            return
        }

        let origin = this.GetViewPos(
            sprite._x - w / 2,
            sprite._y + h / 2
        )
        let minX = origin.x
        let minY = origin.y
        let maxX = origin.x + w
        let maxY = origin.y + h

        if (minX < 0 || MWindowWidth () < maxX ||
            minY < 0 || MWindowHeight() < maxY )
        {
            return
        }

        MContextSetOffset(minX, minY)
        MContextSelectColor(sprite._backgroundColor)
        MContextDrawRect(0, 0, w, h)
        sprite._drawHandler?.call(null, w, h)
        sprite.OnDraw(w, h)
    }

    private static OnTouchBegin(): void {
        let pos = this.GetWorldPos(
            MWindowTouchX(),
            MWindowTouchY()
        )

        let sprites = this.GetSortedSprites(1)
        for (let sprite of sprites) {
            let w = sprite._hitableW
            let h = sprite._hitableH

            if (w <= 0 || h <= 0) {
                continue
            }

            let minX = sprite._x - w / 2
            let maxX = sprite._x + w / 2
            let minY = sprite._y - h / 2
            let maxY = sprite._y + h / 2

            if (pos.x < minX || maxX < pos.x ||
                pos.y < minY || maxY < pos.y )
            {
                continue
            }

            this.s_touchingSprite = sprite

            let x = pos.x - minX
            let y = pos.y - minY
            sprite._hitBeginHandler?.call(null, x, y)
            sprite.OnHitBegin(x, y)

            break
        }
    }

    private static OnTouchMove(): void {
        if (!this.s_touchingSprite) {
            return
        }

        let pos = this.GetWorldPos(
            MWindowTouchX(),
            MWindowTouchY()
        )

        let s = this.s_touchingSprite
        let x = pos.x - s._x + s._hitableW / 2
        let y = pos.y - s._y + s._hitableH / 2
        s._hitMoveHandler?.call(null, x, y)
        s.OnHitMove(x, y)
    }

    private static OnTouchEnd(): void {
        if (!this.s_touchingSprite) {
            return
        }

        let pos = this.GetWorldPos(
            MWindowTouchX(),
            MWindowTouchY()
        )

        let s = this.s_touchingSprite
        let x = pos.x - s._x + s._hitableW / 2
        let y = pos.y - s._y + s._hitableH / 2
        s._hitEndHandler?.call(null, x, y)
        s.OnHitEnd(x, y)

        this.s_touchingSprite = null
    }

    private static GetSortedSprites(smaller: number): Sprite[] {
        let sprites = Array.from(this.s_sprites.values())

        return sprites.sort((a: Sprite, b: Sprite) => {
            if (a._z != b._z) {
                return a._z < b._z ? smaller : -smaller
            }
            return 0
        })
    }

    private static GetWorldPos(viewX: number, viewY: number): { x: number, y: number } {
        let viewW = MWindowWidth ()
        let viewH = MWindowHeight()

        let x = Camera.focusX - viewW / 2 + viewX
        let y = Camera.focusY + viewH / 2 - viewY

        return { x: x, y: y}
    }

    private static GetViewPos(worldX: number, worldY: number): { x: number, y: number } {
        let viewW = MWindowWidth ()
        let viewH = MWindowHeight()

        let x = viewW / 2 - Camera.focusX + worldX
        let y = viewH / 2 + Camera.focusY - worldY

        return { x: x, y: y }
    }

    private static AddSprite(sprite: Sprite): void {
        let id = ++this.s_spriteCount

        sprite._id = id
        //the last sprite is on the top.
        sprite._z = id

        this.s_sprites.set(id, sprite)
    }

    private static RemoveSprite(sprite: Sprite): void {
        this.s_sprites.delete(sprite._id)
    }

    private _drawHandler    : (w: number, h: number) => void
    private _hitBeginHandler: (x: number, y: number) => void
    private _hitMoveHandler : (x: number, y: number) => void
    private _hitEndHandler  : (x: number, y: number) => void

    private _backgroundColor = MColor_WhiteColor
    private _drawableW = 0
    private _drawableH = 0

    private _hitableW = 0
    private _hitableH = 0

    private _id = 0
    private _x  = 0
    private _y  = 0
    private _z  = 0

    public constructor() {
        Sprite.AddSprite(this)
    }

    public Destroy(): void {
        Sprite.RemoveSprite(this)
    }

    public set drawHandler    (v: (w: number, h: number) => void) { this._drawHandler     = v }
    public set hitBeginHandler(v: (x: number, y: number) => void) { this._hitBeginHandler = v }
    public set hitMoveHandler (v: (x: number, y: number) => void) { this._hitMoveHandler  = v }
    public set hitEndHandler  (v: (x: number, y: number) => void) { this._hitEndHandler   = v }

    protected OnDraw    (w: number, h: number): void {}
    protected OnHitBegin(x: number, y: number): void {}
    protected OnHitMove (x: number, y: number): void {}
    protected OnHitEnd  (x: number, y: number): void {}

    public set backgroundColor(value: number) {
        this._backgroundColor = value
    }

    public SetDrawableSize(w: number, h: number): void {
        this._drawableW = w
        this._drawableH = h
    }

    public SetHitableSize(w: number, h: number): void {
        this._hitableW = w
        this._hitableH = h
    }

    public MoveTo(x: number, y: number, z?: number): void {
        this._x = x
        this._y = y

        if (z !== undefined) {
            this._z = z
        }
    }

    public set x(value: number) { this._x = value }
    public set y(value: number) { this._y = value }
    public set z(value: number) { this._z = value }

    public get x(): number { return this._x }
    public get y(): number { return this._y }
    public get z(): number { return this._z }
}

Sprite.Initialize()

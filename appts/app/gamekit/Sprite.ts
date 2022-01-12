import { Camera } from './Camera'

class DataSprite {

    private _drawableWidth  = 0
    private _drawableHeight = 0

    public SetDrawableSize(width: number, height: number): void {
        this._drawableWidth  = width
        this._drawableHeight = height
    }

    public set drawableWidth (value: number) { this._drawableWidth  = value }
    public set drawableHeight(value: number) { this._drawableHeight = value }

    public get drawableWidth (): number { return this._drawableWidth  }
    public get drawableHeight(): number { return this._drawableHeight }

    private _hitableWidth  = 0
    private _hitableHeight = 0

    public SetHitableSize(width: number, height: number): void {
        this._hitableWidth  = width
        this._hitableHeight = height
    }

    public set hitableWidth (value: number) { this._hitableWidth  = value }
    public set hitableHeight(value: number) { this._hitableHeight = value }

    public get hitableWidth (): number { return this._hitableWidth  }
    public get hitableHeight(): number { return this._hitableHeight }

    private _x = 0
    private _y = 0
    private _z = 0

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

    public Draw    (w: number, h: number): void {}
    public HitBegin(x: number, y: number): void {}
    public HitMove (x: number, y: number): void {}
    public HitEnd  (x: number, y: number): void {}
}

class SpriteCollection {

    private static s_sprites      = new Set<DataSprite>()
    private static s_spriteCount  = 0
    private static s_hitingSprite = <DataSprite> null

    static {
        MWindowAddListener(() => {
            let event = MWindowCurrentEvent()

            if /**/ (event == MWindowEvent_Draw      ) { this.OnDraw      () }
            else if (event == MWindowEvent_TouchBegin) { this.OnTouchBegin() }
            else if (event == MWindowEvent_TouchMove ) { this.OnTouchMove () }
            else if (event == MWindowEvent_TouchEnd  ) { this.OnTouchEnd  () }
        })
    }

    public static AddSprite(sprite: DataSprite): void {
        //the last sprite is on the top.
        sprite.z = ++this.s_spriteCount
        this.s_sprites.add(sprite)
    }

    public static RemoveSprite(sprite: DataSprite): void {
        this.s_sprites.delete(sprite)
    }

    private static OnDraw(): void {
        let sprites = this.GetSortedSprites(-1)
        for (let sprite of sprites) {
            this.DrawSprite(sprite)
        }
    }

    private static DrawSprite(sprite: DataSprite,): void {
        let w = sprite.drawableWidth
        let h = sprite.drawableHeight

        if (w <= 0 || h <= 0) {
            return
        }

        let origin = this.GetViewPos(
            sprite.x - w / 2,
            sprite.y + h / 2
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
        sprite.Draw(w, h)
    }

    private static OnTouchBegin(): void {
        let pos = this.GetWorldPos(
            MWindowTouchX(),
            MWindowTouchY()
        )

        let sprites = this.GetSortedSprites(1)
        for (let sprite of sprites) {
            let w = sprite.hitableWidth
            let h = sprite.hitableHeight

            if (w <= 0 || h <= 0) {
                continue
            }

            let minX = sprite.x - w / 2
            let maxX = sprite.x + w / 2
            let minY = sprite.y - h / 2
            let maxY = sprite.y + h / 2

            if (pos.x < minX || maxX < pos.x ||
                pos.y < minY || maxY < pos.y )
            {
                continue
            }

            this.s_hitingSprite = sprite

            let x = pos.x - minX
            let y = pos.y - minY
            sprite.HitBegin(x, y)

            break
        }
    }

    private static OnTouchMove(): void {
        if (!this.s_hitingSprite) {
            return
        }

        let pos = this.GetWorldPos(
            MWindowTouchX(),
            MWindowTouchY()
        )

        let s = this.s_hitingSprite
        let x = pos.x - s.x + s.hitableWidth  / 2
        let y = pos.y - s.y + s.hitableHeight / 2

        s.HitMove(x, y)
    }

    private static OnTouchEnd(): void {
        if (!this.s_hitingSprite) {
            return
        }

        let pos = this.GetWorldPos(
            MWindowTouchX(),
            MWindowTouchY()
        )

        let s = this.s_hitingSprite
        let x = pos.x - s.x + s.hitableWidth  / 2
        let y = pos.y - s.y + s.hitableHeight / 2
        
        s.HitEnd(x, y)

        this.s_hitingSprite = null
    }

    private static GetSortedSprites(smaller: number): DataSprite[] {
        let sprites = Array.from(this.s_sprites)

        return sprites.sort((a: Sprite, b: Sprite) => {
            if (a.z != b.z) {
                return a.z < b.z ? smaller : -smaller
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
}

export class Sprite extends DataSprite {

    private _drawHandler    : (w: number, h: number) => void
    private _hitBeginHandler: (x: number, y: number) => void
    private _hitMoveHandler : (x: number, y: number) => void
    private _hitEndHandler  : (x: number, y: number) => void

    private _backgroundColor = MColor_WhiteColor

    public constructor(width?: number, height?: number) {
        super()

        if (width !== undefined && height !== undefined) {
            this.drawableWidth  = width
            this.drawableHeight = height
            this.hitableWidth   = width
            this.hitableHeight  = height
        }

        SpriteCollection.AddSprite(this)
    }

    public Destroy(): void {
        SpriteCollection.RemoveSprite(this)
    }

    public Draw(width: number, height: number): void {
        MContextSelectColor(this._backgroundColor)
        MContextDrawRect(0, 0, width, height)

        this._drawHandler?.call(null, width, height)
        this.OnDraw(width, height)
    }

    public HitBegin(x: number, y: number) { this._hitBeginHandler?.call(null, x, y); this.OnHitBegin(x, y) }
    public HitMove (x: number, y: number) { this._hitMoveHandler ?.call(null, x, y); this.OnHitMove (x, y) }
    public HitEnd  (x: number, y: number) { this._hitEndHandler  ?.call(null, x, y); this.OnHitEnd  (x, y) }

    public set drawHandler    (v: (w: number, h: number) => void) { this._drawHandler     = v }
    public set hitBeginHandler(v: (x: number, y: number) => void) { this._hitBeginHandler = v }
    public set hitMoveHandler (v: (x: number, y: number) => void) { this._hitMoveHandler  = v }
    public set hitEndHandler  (v: (x: number, y: number) => void) { this._hitEndHandler   = v }

    public set backgroundColor(value: number) {
        this._backgroundColor = value
    }

    protected OnDraw    (w: number, h: number): void {}
    protected OnHitBegin(x: number, y: number): void {}
    protected OnHitMove (x: number, y: number): void {}
    protected OnHitEnd  (x: number, y: number): void {}
}

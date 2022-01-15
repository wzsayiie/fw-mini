import { Camera } from './Camera'

class DataSprite {

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

    private _width  = 0
    private _height = 0

    public SetSize(width: number, height: number): void {
        this._width  = width
        this._height = height
    }

    public set width (value: number) { this._width  = value }
    public set height(value: number) { this._height = value }

    public get width (): number { return this._width  }
    public get height(): number { return this._height }

    public get minX(): number { return this._x - this._width  / 2 }
    public get maxX(): number { return this._x + this._width  / 2 }
    public get minY(): number { return this._y - this._height / 2 }
    public get maxY(): number { return this._y + this._height / 2 }

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
        if (sprite.width <= 0 || sprite.height <= 0) {
            return
        }

        //NOTE: the origin of the window coordinate is in the upper left corner.
        let windowPos = this.GetWindowPos(sprite.minX, sprite.maxY)

        if (windowPos.x + sprite.width  < 0 || MWindowWidth () < windowPos.x ||
            windowPos.y + sprite.height < 0 || MWindowHeight() < windowPos.y )
        {
            return
        }

        MContextSetOffset(windowPos.x, windowPos.y)
        sprite.Draw(sprite.width, sprite.height)
    }

    private static OnTouchBegin(): void {
        let worldPos = this.GetWorldPos(MWindowTouchX(), MWindowTouchY())

        let sprites = this.GetSortedSprites(1)
        for (let sprite of sprites) {
            if (sprite.width <= 0 || sprite.height <= 0) {
                continue
            }

            if (worldPos.x < sprite.minX || sprite.maxX < worldPos.x ||
                worldPos.y < sprite.minY || sprite.maxY < worldPos.y )
            {
                continue
            }

            this.s_hitingSprite = sprite
            sprite.HitBegin(
                - sprite.minX + worldPos.x,
                  sprite.maxY - worldPos.y,
            )

            break
        }
    }

    private static OnTouchMove(): void {
        if (!this.s_hitingSprite) {
            return
        }

        let worldPos = this.GetWorldPos(MWindowTouchX(), MWindowTouchY())
        this.s_hitingSprite.HitMove(
            - this.s_hitingSprite.minX + worldPos.x,
              this.s_hitingSprite.maxY - worldPos.y,
        )
    }

    private static OnTouchEnd(): void {
        if (!this.s_hitingSprite) {
            return
        }

        let worldPos = this.GetWorldPos(MWindowTouchX(), MWindowTouchY())
        this.s_hitingSprite.HitEnd(
            - this.s_hitingSprite.minX + worldPos.x,
              this.s_hitingSprite.maxY - worldPos.y,
        )

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

    private static GetWindowPos(worldX: number, worldY: number): { x: number, y: number } {
        let x = MWindowWidth () / 2 - Camera.focusX + worldX
        let y = MWindowHeight() / 2 + Camera.focusY - worldY

        return { x: x, y: y }
    }

    private static GetWorldPos(windowX: number, windowY: number): { x: number, y: number } {
        let x = Camera.focusX - MWindowWidth () / 2 + windowX
        let y = Camera.focusY + MWindowHeight() / 2 - windowY

        return { x: x, y: y}
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
            this.width  = width
            this.height = height
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

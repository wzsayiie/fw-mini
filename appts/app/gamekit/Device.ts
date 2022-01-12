export class Device {

    private static s_mouseListener: (x: number, y: number) => void
    private static s_keyListener  : (key: number) => void

    static {
        MWindowAddListener(() => {
            let event = MWindowCurrentEvent()

            if /**/ (event == MWindowEvent_MouseMove) { this.OnMouseMove() }
            else if (event == MWindowEvent_KeyDown  ) { this.OnKeyDown  () }
        })
    }

    private static OnMouseMove(): void {
        let x = MWindowMouseX()
        let y = MWindowMouseY()

        this.s_mouseListener?.call(null, x, y)
    }

    private static OnKeyDown(): void {
        let key = MWindowActiveKey()

        this.s_keyListener?.call(null, key)
    }

    public static get mouseX(): number { return MWindowMouseX() }
    public static get mouseY(): number { return MWindowMouseY() }

    public static set mouseListener(value: (x: number, y: number) => void) {
        this.s_mouseListener = value
    }

    public static set keyListener(value: (key: number) => void) {
        this.s_keyListener = value
    }
}

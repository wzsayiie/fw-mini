import { Camera } from './Camera'

export class Device {

    private static s_updateListener: () => void
    private static s_mouseListener: (x: number, y: number) => void
    private static s_keyListener: (key: number) => void

    static {
        MWindowAddListener(() => {
            let event = MWindowCurrentEvent()

            if /**/ (event == MWindowEvent_MouseMove) { this.OnMouseMove() }
            else if (event == MWindowEvent_KeyDown  ) { this.OnKeyDown  () }
        })

        MAppAddUpdateListener(() => {
            this.OnUpdate()
        })
    }

    private static OnMouseMove(): void {
        if (this.s_mouseListener) {
            this.s_mouseListener(this.mouseX, this.mouseY)
        }
    }

    private static OnKeyDown(): void {
        if (this.s_keyListener) {
            this.s_keyListener(MWindowActiveKey())
        }
    }

    private static OnUpdate(): void {
        this.s_updateListener?.call(null)
    }

    public static get mouseX(): number {
        return Camera.focusX - MWindowWidth () / 2 + MWindowMouseX()
    }

    public static get mouseY(): number {
        return Camera.focusY + MWindowHeight() / 2 - MWindowMouseY()
    }

    public static set mouseListener(value: (x: number, y: number) => void) {
        this.s_mouseListener = value
    }

    public static set keyListener(value: (key: number) => void) {
        this.s_keyListener = value
    }

    public static set updateListener(value: () => void) {
        this.s_updateListener = value
    }
}

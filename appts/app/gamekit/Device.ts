import { Camera } from './Camera'

export class Device {

    private static s_mouseListener: () => void

    static {
        MWindowAddListener(() => {
            let event = MWindowCurrentEvent()
            if (event == MWindowEvent_MouseMove) {
                this.OnMouseMove()
            }
        })
    }

    private static OnMouseMove(): void {
        if (this.s_mouseListener) {
            this.s_mouseListener()
        }
    }

    public static get mouseX(): number {
        return Camera.focusX - MWindowWidth () / 2 + MWindowMouseX()
    }

    public static get mouseY(): number {
        return Camera.focusY + MWindowHeight() / 2 - MWindowMouseY()
    }

    public static set mouseListener(value: () => void) {
        this.s_mouseListener = value
    }
}

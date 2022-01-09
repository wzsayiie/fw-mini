export class Camera {

    private static s_focusX = 0
    private static s_focusY = 0

    public static MoveTo(x: number, y: number): void {
        this.s_focusX = x
        this.s_focusY = y
    }

    public static set focusX(value: number) { this.s_focusX = value }
    public static set focusY(value: number) { this.s_focusY = value }

    public static get focusX(): number { return this.s_focusX }
    public static get focusY(): number { return this.s_focusY }
}

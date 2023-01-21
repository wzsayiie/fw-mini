import { NLinearBox } from './NLinearBox'

export class NHorizonBox extends NLinearBox {
    layout(x: number, y: number, w: number, h: number): void {
        let ranges = this.calcLayout(w, 'width')

        for (let i = 0; i < ranges.length; ++i) {
            let child = this.children[i]
            let range = ranges[i]

            let top   : number
            let height: number
            if (child.height && child.height < h) {
                top    = (h - child.height) / 2
                height = child.height
            } else {
                top    = 0
                height = h
            }

            child.layout(x + range.loc, y + top, range.len, height)
        }
    }
}

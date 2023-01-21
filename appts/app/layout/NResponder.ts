import { EntityNode } from './Nodes'

import {
    MColor,
    MFillMode,
    MHAlign,
    MVAlign,
} from '../host/native'

function C(ch: string): number {
    return ch.charCodeAt(0)
}

export class NResponder extends EntityNode {
    protected getColor(key: string, callback: (v: MColor) => void): void {
        let str: string = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'black'    : callback(MColor.blackColor    ); return
            case 'darkGray' : callback(MColor.darkGrayColor ); return
            case 'gray'     : callback(MColor.grayColor     ); return
            case 'lightGray': callback(MColor.lightGrayColor); return
            case 'white'    : callback(MColor.whiteColor    ); return
            case 'red'      : callback(MColor.redColor      ); return
            case 'green'    : callback(MColor.greenColor    ); return
            case 'blue'     : callback(MColor.blueColor     ); return
            case 'cyan'     : callback(MColor.cyanColor     ); return
            case 'yellow'   : callback(MColor.yellowColor   ); return
            case 'magenta'  : callback(MColor.magentaColor  ); return
            case 'orange'   : callback(MColor.orangeColor   ); return
            case 'purple'   : callback(MColor.purpleColor   ); return
            case 'brown'    : callback(MColor.brownColor    ); return
            case 'clear'    : callback(MColor.clearColor    ); return
        }

        if (str.startsWith('#') && (str.length == 7 || str.length == 9)) {
            let okay = true
            let rgba = 0
            for (let i = 1; i < str.length; ++i) {
                let n = str.charCodeAt(i)

                if /**/ (C('0') <= n && n <= C('9')) { rgba = (rgba << 4) + n - C('0')      }
                else if (C('A') <= n && n <= C('Z')) { rgba = (rgba << 4) + n - C('A') + 10 }
                else if (C('a') <= n && n <= C('z')) { rgba = (rgba << 4) + n - C('a') + 10 }

                else {
                    //unexpected characters.
                    okay = false
                    break
                }
            }

            //add the alpha component.
            if (okay && str.length == 7) {
                rgba = (rgba << 8) + 0xFF
            }

            if (okay) {
                callback(MColor.fromRGBA(rgba))
            }
        }
    }

    protected getHAlign(key: string, callback: (v: MHAlign) => void): void {
        let str: string = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'left'  : callback(MHAlign.Left  ); return
            case 'center': callback(MHAlign.Center); return
            case 'right' : callback(MHAlign.Right ); return
        }
    }

    protected getVAlign(key: string, callback: (v: MVAlign) => void): void {
        let str: string = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'top'   : callback(MVAlign.Top   ); return
            case 'middle': callback(MVAlign.Middle); return
            case 'bottom': callback(MVAlign.Bottom); return
        }
    }

    protected getFillMode(key: string, callback: (v: MFillMode) => void): void {
        let str: string = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'coverByAspectRatio': callback(MFillMode.CoverByAspectRatio); return
            case 'coverByStretch'    : callback(MFillMode.CoverByStretch    ); return
            case 'keepOriginalSize'  : callback(MFillMode.KeepOriginalSize  ); return
            case 'matchSpaceWidth'   : callback(MFillMode.MatchSpaceWidth   ); return
            case 'matchSpaceHeight'  : callback(MFillMode.MatchSpaceHeight  ); return
        }
    }
}

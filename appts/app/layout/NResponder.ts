import { EntityNode } from './Nodes'

import {
    MColor,
    MFillMode,
    MHAlign,
    MVAlign,
} from '../host/native'

export class NResponder extends EntityNode {
    protected getColor(key: string, callback: (v: MColor) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'black'    : callback(MColor.blackColor    ); break
            case 'darkGray' : callback(MColor.darkGrayColor ); break
            case 'gray'     : callback(MColor.grayColor     ); break
            case 'lightGray': callback(MColor.lightGrayColor); break
            case 'white'    : callback(MColor.whiteColor    ); break
            case 'red'      : callback(MColor.redColor      ); break
            case 'green'    : callback(MColor.greenColor    ); break
            case 'blue'     : callback(MColor.blueColor     ); break
            case 'cyan'     : callback(MColor.cyanColor     ); break
            case 'yellow'   : callback(MColor.yellowColor   ); break
            case 'magenta'  : callback(MColor.magentaColor  ); break
            case 'orange'   : callback(MColor.orangeColor   ); break
            case 'purple'   : callback(MColor.purpleColor   ); break
            case 'brown'    : callback(MColor.brownColor    ); break
            case 'clear'    : callback(MColor.clearColor    ); break
        }
    }

    protected getHAlign(key: string, callback: (v: MHAlign) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'left'  : callback(MHAlign.Left  ); break
            case 'center': callback(MHAlign.Center); break
            case 'right' : callback(MHAlign.Right ); break
        }
    }

    protected getVAlign(key: string, callback: (v: MVAlign) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'top'   : callback(MVAlign.Top   ); break
            case 'middle': callback(MVAlign.Middle); break
            case 'bottom': callback(MVAlign.Bottom); break
        }
    }

    protected getFillMode(key: string, callback: (v: MFillMode) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }

        switch (str) {
            case 'coverByAspectRatio': callback(MFillMode.CoverByAspectRatio); break
            case 'coverByStretch'    : callback(MFillMode.CoverByStretch    ); break
            case 'keepOriginalSize'  : callback(MFillMode.KeepOriginalSize  ); break
            case 'matchSpaceWidth'   : callback(MFillMode.MatchSpaceWidth   ); break
            case 'matchSpaceHeight'  : callback(MFillMode.MatchSpaceHeight  ); break
        }
    }
}

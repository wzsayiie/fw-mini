//this file is just for tag hints for jsx.

import * as React from 'react'

import {
    MColor,
    MFillMode,
    MHAlign,
    MVAlign,
} from '../host/native'

declare global {
    namespace JSX {
        interface IntrinsicElements {
            horizon : Base
            vertical: Base
            stack   : Base
            stretch : Base

            cViewController      : ViewController
            cNavigationController: ViewController
            cTabPageController   : ViewController

            cView      : View
            cImageView : ImageView
            cLabel     : Label
            cScrollView: ScrollView
            cControl   : Control
            cButton    : Button
            cTextField : TextField
        }
    }
}

interface Base           extends React.HTMLAttributes<Base>          , _Base           {}
interface ViewController extends React.HTMLAttributes<ViewController>, _ViewController {}
interface View           extends React.HTMLAttributes<View>          , _View           {}
interface ImageView      extends React.HTMLAttributes<ImageView>     , _ImageView      {}
interface Label          extends React.HTMLAttributes<Label>         , _Label          {}
interface ScrollView     extends React.HTMLAttributes<ScrollView>    , _ScrollView     {}
interface Control        extends React.HTMLAttributes<Control>       , _Control        {}
interface Button         extends React.HTMLAttributes<Button>        , _Button         {}
interface TextField      extends React.HTMLAttributes<TextField>     , _TextField      {}

interface _Base {
    ratio? : number
    width? : number
    height?: number
}

interface _Entity extends _Base {
    customObject?: object
}

interface _ViewController extends _Entity {
    name?           : string
    backgroundColor?: MColor
}

interface _View extends _Entity {
    name?           : string
    backgroundColor?: MColor
}

interface _ImageView extends _View {
    fillMode?: MFillMode
    hAlign?  : MHAlign
    vAlign?  : MVAlign
    image?   : string
}

interface _Label extends _View {
    text?     : string
    textColor?: MColor
    fontSize? : number
    hAlign?   : MHAlign
    vAlign?   : MVAlign
}

interface _ScrollView extends _View {
    contentWidth? : boolean
    contentHeight?: boolean
}

interface _Control extends _View {
    controlId?      : string
    previousControl?: string
    nextControl?    : string
}

interface _Button extends _Control {
    title?      : string
    text?       : string
    textColor?  : MColor
    fontSize?   : number
    hAlign?     : MHAlign
    vAlign?     : MVAlign
    clickTarget?: string
}

interface _TextField extends _Control {
    prompt?   : string
    textColor?: MColor
    fontSize? : number
    hAlign?   : MHAlign
    vAlign?   : MVAlign
}

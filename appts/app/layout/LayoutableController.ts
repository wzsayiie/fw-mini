import { JSXElem           } from './JSXObjects'
import { NBase             } from './Nodes'
import { layoutableClasses } from './layoutableClasses'

import {
    CView,
    CViewController,
} from '../host/native'

export class LayoutableController extends CViewController {

    private _rootNode: NBase

    loadView(): CView {
        let view = super.loadView()
        view.layouter = () => this.layoutNodes()
        return view
    }

    protected setLayout(layout: JSXElem): void {
        this._rootNode = this.makeTree(layout)
        this.layoutNodes()
    }

    private makeTree(elem: JSXElem): NBase {
        //node:
        let nodeClass = layoutableClasses.nodeClassOf(elem.type)
        if (!nodeClass) {
            return null
        }

        let node = new nodeClass()
        node.attach(elem, this)

        //children.
        let subElems = elem.props.children
        if (Array.isArray(subElems)) {
            for (let subElem of subElems) {
                let child = this.makeTree(subElem)
                if (child) {
                    node.addChild(child)
                }
            }

        } else if (subElems) {
            let child = this.makeTree(subElems)
            if (child) {
                node.addChild(child)
            }
        }

        return node
    }

    private layoutNodes(): void {
        if (!this._rootNode) {
            return
        }

        let frame = this.view.frame
        this._rootNode.layout(
            frame.origin.x  , frame.origin.y,
            frame.size.width, frame.size.height
        )
    }
}

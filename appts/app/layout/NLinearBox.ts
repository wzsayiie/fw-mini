import { ContainerNode } from './Nodes'

export class NLinearRange {
    loc: number
    len: number
}

export class NLinearBox extends ContainerNode {
    protected calcLayout(total: number, aspect: string): Array<NLinearRange> {
        if (this.children.length == 0) {
            return []
        }

        //collect data:
        let reserved = 0
        let ratioNum = 0

        for (let child of this.children) {
            if (child[aspect]) {
                reserved += child[aspect]
            } else {
                ratioNum += child.ratio
            }
        }

        //calculate:
        let blank   = total - reserved
        let aScale  = 0
        let rWeight = 0
        let begin   = 0
        let space   = 0

        if (reserved && ratioNum) {
            if (blank > 0) {
                aScale  = 1
                rWeight = blank / ratioNum
            } else {
                aScale  = total / reserved
                rWeight = 0
            }

        } else if (reserved) {
            if (blank > 0) {
                aScale = 1
                space  = blank / (this.children.length + 1)
                begin  = space
            } else {
                aScale = total / reserved
            }

        } else if (ratioNum) {
            rWeight = blank / ratioNum
        }

        //make ranges:
        let ranges = new Array<NLinearRange>()

        for (let child of this.children) {
            let len = 0
            if (child[aspect]) {
                len = child[aspect] * aScale
            } else {
                len = child.ratio * rWeight
            }

            ranges.push({
                loc: begin,
                len: len,
            })

            begin += len + space
        }

        return ranges
    }
}

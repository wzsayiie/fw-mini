declare function MPrintMessage(text: string): void

function MEnumId(n: string): number {
    let c0 = n.charCodeAt(0)
    let c1 = n.charCodeAt(1)
    let c2 = n.charCodeAt(2)

    return c0 | (c1 << 8) | (c2 << 16)
}

const MTypeId = {
    Void   : MEnumId('vid'),
    Bool   : MEnumId('bol'),
    Int    : MEnumId('int'),
    Float  : MEnumId('flt'),
    Pointer: MEnumId('ptr'),
    S8Ptr  : MEnumId('s08'),
    S16Ptr : MEnumId('s16'),
    MObject: MEnumId('Obj'),
    MBool  : MEnumId('Bol'),
    MInt   : MEnumId('Int'),
    MFloat : MEnumId('Flt'),
    MPtr   : MEnumId('Ptr'),
    MString: MEnumId('Str'),
    MLambda: MEnumId('Lmd'),
    MData  : MEnumId('Dat'),
    MArray : MEnumId('Arr'),
    MImage : MEnumId('Img'),
}

declare function MConstSelectFirst   (): void
declare function MConstSelectedValid (): boolean
declare function MConstSelectNext    (): void
declare function MConstSelectedName  (): string
declare function MConstSelectedNote  (): string
declare function MConstSelectedTypeId(): number
declare function MConstSelectedString(): string
declare function MConstSelectedInt   (): number
declare function MConstSelectedFloat (): number

function ScanConsts(buffer: string[]): void {
    for (MConstSelectFirst(); MConstSelectedValid(); MConstSelectNext()) {
        CollectConst(buffer)
    }
}

function CollectConst(buffer: string[]): void {
    let type = MConstSelectedTypeId()
    let name = MConstSelectedName()

    switch (type) {
        case MTypeId.MString: buffer.push(`declare const ${name} = '${MConstSelectedString()}'`); break
        case MTypeId.Int    : buffer.push(`declare const ${name} =  ${MConstSelectedInt   ()} `); break
        case MTypeId.Float  : buffer.push(`declare const ${name} =  ${MConstSelectedFloat ()} `); break
    }
}

const MTypeMap: Map<number, string> = new Map([
    [ MTypeId.Void   , 'void'       ],
    [ MTypeId.Bool   , 'boolean'    ],
    [ MTypeId.Int    , 'number'     ],
    [ MTypeId.Float  , 'number'     ],
    [ MTypeId.Pointer, 'MPointer'   ],
    [ MTypeId.S8Ptr  , 'string'     ],
    [ MTypeId.S16Ptr , 'string'     ],
    [ MTypeId.MObject, 'MObject'    ],
    [ MTypeId.MBool  , 'boolean'    ],
    [ MTypeId.MInt   , 'number'     ],
    [ MTypeId.MFloat , 'number'     ],
    [ MTypeId.MPtr   , 'MPointer'   ],
    [ MTypeId.MString, 'string'     ],
    [ MTypeId.MLambda, '() => void' ],
    [ MTypeId.MData  , 'MData'      ],
    [ MTypeId.MArray , 'MArray'     ],
    [ MTypeId.MImage , 'MImage'     ],
])

function ScanTypes(buffer: string[]): void {
    let set = new Set()
    for (let [_, name] of MTypeMap) {

        //only native class type need to declare.
        let c0 = name.charAt(0)
        let c1 = name.charAt(1)
        if (c0 != 'M' || c1 < 'A' || 'Z' < c1) {
            continue
        }

        if (set.has(name)) {
            continue
        }

        buffer.push(`declare class ${name} {}`)
        set.add(name)
    }
}

declare function MFuncSelectFirst      (): void
declare function MFuncSelectedValid    (): boolean
declare function MFuncSelectNext       (): void
declare function MFuncSelectedName     (): string
declare function MFuncSelectedNote     (): string
declare function MFuncSelectedRetTypeId(): number
declare function MFuncSelectedArgCount (): number
declare function MFuncSelectedArgTypeId(index: number): number

function ScanFuncs(buffer: string[]): void {
    for (MFuncSelectFirst(); MFuncSelectedValid(); MFuncSelectNext()) {
        CollectFunc(buffer)
    }
}

function CollectFunc(buffer: string[]): void {
    //collect:
    let funcName = MFuncSelectedName()

    let funcNote = MFuncSelectedNote()
    let argNames = new Array<string>()
    if (funcNote && funcNote.startsWith('args:')) {
        let list = funcNote.substring('args:'.length)
        argNames = list.split(',')
    }

    let argCount = MFuncSelectedArgCount()
    let argTypes = []
    for (let n = 0; n < argCount; n++) {
        let iden = MFuncSelectedArgTypeId(n)
        let type = MTypeMap.get(iden)
        argTypes.push(type)
    }

    let retIden = MFuncSelectedRetTypeId()
    let retType = MTypeMap.get(retIden)

    //join:
    let line = []

    line.push(`declare function ${funcName}(`)
    for (let n = 0; n < argTypes.length; n++) {
        let notLast = (n < argTypes.length - 1)
        let argName = argNames[n]
        let argType = argTypes[n]

        if (notLast) { line.push(`${argName}: ${argType}, `) }
        else  /****/ { line.push(`${argName}: ${argType}`  ) }
    }
    line.push(`): ${retType}`)

    buffer.push(line.join(''))
}

function Main(): void {
    let buffer = new Array<string>()
    ScanConsts(buffer)
    ScanTypes (buffer)
    ScanFuncs (buffer)

    //copy the print into "app/@types/index.d.ts".
    //(under mac os, the application has not write access to paths outside the sandbox.)
    MPrintMessage(buffer.join('\n'))
}

Main()

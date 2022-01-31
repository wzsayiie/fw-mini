declare function MPrintMessage(text: string): void

function C(text: string, index: number = 0): number {
    return text.charCodeAt(index)
}

function MHashFactor(text: string, index: number): number {
    let n = C(text, index)

    if (C('A') <= n && n <= C('Z')) { return n - C('A') + 1 }
    if (C('a') <= n && n <= C('z')) { return n - C('a') + 1 }

    return n ? 27 : 0
}

function MHashId(name: string): number {
    let iden = 0

    let size = Math.min(name.length, 6)
    let card = 1
    for (let i = 0; i < size; ++i) {
        iden += MHashFactor(name, i) * card
        card *= 32
    }

    return iden
}

const MTypeId = {
    Void   : MHashId('void'   ),
    Bool   : MHashId('bool'   ),
    Int    : MHashId('int'    ),
    Float  : MHashId('float'  ),
    Pointer: MHashId('intptr' ),
    U8Ptr  : MHashId('u8ptr'  ),
    U16Ptr : MHashId('u16ptr' ),
    MObject: MHashId('MObject'),
    MBool  : MHashId('MBool'  ),
    MInt   : MHashId('MInt'   ),
    MFloat : MHashId('MFloat' ),
    MPtr   : MHashId('MPtr'   ),
    MString: MHashId('MString'),
    MLambda: MHashId('MLambda'),
    MData  : MHashId('MData'  ),
    MArray : MHashId('MArray' ),
    MImage : MHashId('MImage' ),
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
    [ MTypeId.Pointer, 'MPtr'       ],
    [ MTypeId.U8Ptr  , 'string'     ],
    [ MTypeId.U16Ptr , 'string'     ],
    [ MTypeId.MObject, 'MObject'    ],
    [ MTypeId.MBool  , 'boolean'    ],
    [ MTypeId.MInt   , 'number'     ],
    [ MTypeId.MFloat , 'number'     ],
    [ MTypeId.MPtr   , 'MPtr'       ],
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

declare function MPrintMessage(message: string): void

function MEnumId(n: string): number {
    let c0 = n.charCodeAt(0)
    let c1 = n.charCodeAt(1)
    let c2 = n.charCodeAt(2)

    return c0 | (c1 << 8) | (c2 << 16)
}

function MEnumName(id: number): string {
    let c0 = String.fromCharCode((id      ) & 0xff)
    let c1 = String.fromCharCode((id >>  8) & 0xff)
    let c2 = String.fromCharCode((id >> 16) & 0xff)

    return `${c0}${c1}${c2}`
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
declare function MConstSelectedTypeId(): number
declare function MConstSelectedString(): string
declare function MConstSelectedInt   (): number
declare function MConstSelectedFloat (): number

function CollectConsts(outArray: string[]): void {
    for (MConstSelectFirst(); MConstSelectedValid(); MConstSelectNext()) {
        let line = CollectSelectedConst()
        if (line) {
            outArray.push(line)
        }
    }

    outArray.push('\n')
}

function CollectSelectedConst(): string {
    let name = MConstSelectedName  ()
    let type = MConstSelectedTypeId()

    switch (type) {
        case MTypeId.MString: return `declare const ${name} = '${MConstSelectedString()}'\n`
        case MTypeId.Int    : return `declare const ${name} =  ${MConstSelectedInt   ()} \n`
        case MTypeId.Float  : return `declare const ${name} =  ${MConstSelectedFloat ()} \n`

        default: {
            MPrintMessage(`ERROR: unkonwn constant type '${MEnumName(type)}'`)
            return null
        }
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
    [ MTypeId.MData  , 'ArrayBuffer'],
    [ MTypeId.MArray , 'MArray'     ],
    [ MTypeId.MImage , 'MImage'     ],
])

function CollectTypes(outArray: string[]): void {
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

        outArray.push(`declare class ${name} {}\n`)
        set.add(name)
    }

    outArray.push('\n')
}

declare function MFuncSelectFirst      (): void
declare function MFuncSelectedValid    (): boolean
declare function MFuncSelectNext       (): void
declare function MFuncSelectedName     (): string
declare function MFuncSelectedRetTypeId(): number
declare function MFuncSelectedArgCount (): number
declare function MFuncSelectedArgTypeId(index: number): number

function CollectFuncs(outArray: string[]): void {
    for (MFuncSelectFirst(); MFuncSelectedValid(); MFuncSelectNext()) {
        let line = CollectSelectedFunc()
        if (line) {
            outArray.push(line)
        }
    }

    outArray.push('\n')
}

function CollectSelectedFunc(): string {
    //check the argument types and return type.
    let argCount = MFuncSelectedArgCount()
    let argTypes = []
    for (let n = 0; n < argCount; n++) {
        let type = MFuncSelectedArgTypeId(n)
        if (!MTypeMap.has(type)) {
            MPrintMessage(`ERROR: unkonwn argument type '${MEnumName(type)}'`)
            return null
        }
        argTypes.push(type)
    }

    let retType = MFuncSelectedRetTypeId()
    if (!MTypeMap.has(retType)) {
        MPrintMessage(`ERROR: unkonwn return type '${MEnumName(retType)}'`)
        return null
    }

    //join the declaration.
    let name = MFuncSelectedName()
    let line = []

    line.push(`declare function ${name}(`)
    for (let n = 0; n < argTypes.length; ++n) {
        let argName = String.fromCharCode('a'.charCodeAt(0) + n)
        let argType = argTypes[n]

        line.push(`${argName}: ${MTypeMap.get(argType)}`)
        if (n < argTypes.length - 1) {
            line.push(`, `)
        }
    }
    line.push(`): ${MTypeMap.get(retType)}\n`)

    return line.join('')
}

declare function MMakeDirectory(path: string): boolean
declare function MWriteU8StringToFile(path: string, text: string): boolean

function Main(): void {
    let script = new Array<string>()
    CollectConsts(script)
    CollectTypes (script)
    CollectFuncs (script)

    //copy the print into "app/@types/index.d.ts".
    //(under mac os, the application has not write access to paths outside the sandbox.)
    MPrintMessage(script.join(''))
}

Main()

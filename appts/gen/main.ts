//meta description:

declare function native_metaJsonDescription(): string

class fcndesc_node {
    type: string
    note: string

    //additional custom field.
    options: { [key: string]: string[] }
}

class typedesc_node {
    type: string
    qual: string
}

class class_node {
    base_type       : string
    abstracted      : boolean
    static_strings  : { [key: string]: string }
    static_numbers  : { [key: string]: number }
    static_functions: { [key: string]: fcndesc_node }
    inst_functions  : { [key: string]: fcndesc_node }
}

class function_node {
    args: typedesc_node[]
    ret : typedesc_node
}

class enum_node {
    values: { [key: string]: number }
}

class map_node {
    key: typedesc_node
    val: typedesc_node
}

class vector_node {
    val: typedesc_node
}

class set_node {
    val: typedesc_node
}

class root_node {
    class_infos   : { [key: string]: class_node    }
    function_infos: { [key: string]: function_node }
    enum_infos    : { [key: string]: enum_node     }
    map_infos     : { [key: string]: map_node      }
    vector_infos  : { [key: string]: vector_node   }
    set_infos     : { [key: string]: set_node      }

    strings  : { [key: string]: string       }
    numbers  : { [key: string]: number       }
    functions: { [key: string]: fcndesc_node }
    classes  : string[]
    enums    : string[]
}

//meta initialization:

function parseFunctionNote(note: string): { [key: string]: string[] } {
    let options = {}

    if (!note) {
        return options
    }
    for (let segment of note.split(";")) {
        let sep = segment.indexOf(":")
        if (sep != -1) {
            let name = segment.substring(0, sep)
            let args = segment.substring(sep + 1)
            options[name] = args.split(",")

        } else {
            //only option name, no arguments.
            options[segment] = []
        }
    }

    return options
}

function fillFunctionOptions(meta: root_node): root_node {
    //class functions.
    for (let name of meta.classes) {
        let cls = meta.class_infos[name]

        for (let desc of Object.values(cls.inst_functions)) {
            desc.options = parseFunctionNote(desc.note)

            //the first argument is implied "this".
            let args = desc.options["args"]
            if (args) {
                desc.options["args"] = [ "_this", ...args]
            } else {
                desc.options["args"] = [ "_this" ]
            }
        }

        for (let desc of Object.values(cls.static_functions)) {
            desc.options = parseFunctionNote(desc.note)

            //make sure that "args" exsists.
            let args = desc.options["args"]
            if (!args) {
                desc.options["args"] = []
            }
        }
    }

    //global functions.
    for (let desc of Object.values(meta.functions)) {
        desc.options = parseFunctionNote(desc.note)

        let args = desc.options["args"]
        if (!args) {
            desc.options["args"] = []
        }
    }

    return meta
}

const _meta = (function (): root_node {
    let json = native_metaJsonDescription()
    let meta = JSON.parse(json)

    fillFunctionOptions(meta)

    return meta
})()

//generator:

const _buffer = new Array<string>()

function generateLine(parts: string[], args: { [key: string]: any }): void {
    for (let i = 0; i < parts.length; ) {
        if (parts[i] == "[[") {
            let intent = parts[i + 1]
            let holder = args[intent]

            if (typeof(holder) == "number") {
                _buffer.push(String(holder))

            } else if (typeof(holder) == "string") {
                _buffer.push(holder)

            } else if (typeof(holder) == "function") {
                holder()
            }

            i += 3

        } else {
            _buffer.push(parts[i])
            i += 1
        }
    }
}

function generate(lines: string[], args: { [key: string]: any }): void {
    for (let line of lines) {
        let parts = line.split(/(\[\[|\]\])/)
        generateLine(parts, args)

        _buffer.push("\n")
    }
}

function template(lines: string[]): { generate: (args: { [key: string]: any }) => void } {
    return {
        generate: (args: { [key: string]: any }) => {
            generate(lines, args)
        }
    }
}

function descriptType(desc: typedesc_node): string {
    let cls = _meta.class_infos[desc.type]
    if (cls) {
        switch (desc.type) {
            //special classes.
            case "generic_function": return "Function"
            case "generic_map"     : return "any"
            case "generic_set"     : return "any"
            case "generic_vector"  : return "any"

            default: return desc.type
        }
    }

    let fcn = _meta.function_infos[desc.type]
    if (fcn) {
        let buf = new Array<string>()

        //arguments.
        buf.push("(")
        for (let i = 0; i < fcn.args.length; ++i) {
            if (i > 0) {
                buf.push(", ")
            }
            buf.push(descriptType(fcn.args[i]))
        }
        //return value.
        buf.push(`) => ${descriptType(fcn.ret)}`)

        return buf.join("")
    }

    let enu = _meta.enum_infos[desc.type]
    if (enu) {
        return desc.type
    }

    let map = _meta.map_infos[desc.type]
    if (map) {
        return `MMap<${descriptType(map.key)}, ${descriptType(map.val)}>`
    }

    let vector = _meta.vector_infos[desc.type]
    if (vector) {
        return `MSet<${descriptType(vector.val)}>`
    }

    let set = _meta.set_infos[desc.type]
    if (set) {
        return `MSet<${descriptType(set.val)}>`
    }

    if (desc.type == 'char' && desc.qual == 'const_ptr') {
        return "string"
    }

    switch (desc.type) {
        case "any"   : return "any"
        case "void"  : return "void"
        case "bool"  : return "boolean"
        case "char"  : return "number"
        case "byte"  : return "number"
        case "int"   : return "number"
        case "int64" : return "number"
        case "float" : return "number"
        case "double": return "number"
        case "string": return "string"

        default: return "ERROR"
    }
}

function isNeedCast(desc: typedesc_node): boolean {
    return _meta.class_infos[desc.type] && desc.type != "generic_function"
}

function SetterName(raw: string): string {
    if (raw.length <= 3 || !raw.startsWith("set")) {
        return raw
    }

    let head = raw.substring(3, 4)
    let body = raw.substring(4)
    return head.toLowerCase() + body
}

function appendNamedArgs(
    argNames: string[], argTypes: typedesc_node[], cast: (type: typedesc_node) => string): void
{
    for (let i = 0; i < argNames.length; ++i) {
        if (i > 0) {
            _buffer.push(", ")
        }

        _buffer.push(`${argNames[i]}: ${cast(argTypes[i])}`)
    }
}

function appendCalledArgs(
    argNames: string[], argTypes: typedesc_node[], cast: (src: string) => string): void
{
    for (let i = 0; i < argNames.length; ++i) {
        if (i > 0) {
            _buffer.push(", ")
        }

        if (isNeedCast(argTypes[i])) {
            _buffer.push(cast(argNames[i]))
        } else {
            _buffer.push(argNames[i])
        }
    }
}

function declareNatives(): void {
    for (let clsName of _meta.classes) {
        if (clsName == "MObject") { continue } //the base class is provided by runtime.
        
        let cls = _meta.class_infos[clsName]

        //class static functions.
        for (let [fcnName, desc] of Object.entries(cls.static_functions)) {
            if (desc.options["ignore"]) { continue }

            template([
                "declare function native_[[cls]]_[[name]]([[args]]): any"
            ])
            .generate({
                "cls" : clsName,
                "name": fcnName,
                "args": () => {
                    appendNamedArgs(desc.options["args"], [], _ => "any")
                },
            })
        }

        //class instance functions.
        for (let [fcnName, desc] of Object.entries(cls.inst_functions)) {
            if (desc.options["ignore"]) { continue }

            template([
                "declare function native_[[cls]]_[[name]]([[args]]): any"
            ])
            .generate({
                "cls" : clsName,
                "name": fcnName,
                "args": () => {
                    appendNamedArgs(desc.options["args"], [], _ => "any")
                },
            })
        }
    }

    //global functions.
    for (let [fcnName, desc] of Object.entries(_meta.functions)) {
        if (desc.options["ignore"]) { continue }

        template([
            "declare function native_[[name]]([[args]]): any"
        ])
        .generate({
            "name": fcnName,
            "args": () => {
                appendNamedArgs(desc.options["args"], [], _ => "any")
            },
        })
    }
}

function defineConstants(): void {
    //constant strings.
    for (let [name, value] of Object.entries(_meta.strings)) {
        template([
            "export const [[name]] = '[[value]]'"
        ])
        .generate({
            "name" : name ,
            "value": value,
        })
    }

    //constant numbers.
    for (let [name, value] of Object.entries(_meta.numbers)) {
        template([
            "export const [[name]] = [[value]]"
        ])
        .generate({
            "name" : name ,
            "value": value,
        })
    }
}

function defineEnumMember(enu: enum_node): void {
    for (let [member, value] of Object.entries(enu.values)) {
        template([
            "    [[member]] = [[value]],",
        ])
        .generate({
            "member": member,
            "value" : value ,
        })
    }
}

function defineEnums(): void {
    for (let enumName of _meta.enums) {
        let enu = _meta.enum_infos[enumName]

        template([
            "export enum [[enumName]] {",
            "[[members]]",
            "}",
            "",
        ])
        .generate({
            "enumName": enumName,
            "members" : () => defineEnumMember(enu),
        })
    }
}

function defineClassConstants(cls: class_node): void {
    //constant strings.
    for (let [field, value] of Object.entries(cls.static_strings)) {
        template([
            "    static readonly [[field]] = '[[value]]'"
        ])
        .generate({
            "field": field,
            "value": value,
        })
    }

    //constant numbers.
    for (let [field, value] of Object.entries(cls.static_numbers)) {
        template([
            "    static readonly [[field]] = [[value]]"
        ])
        .generate({
            "field": field,
            "value": value,
        })
    }
}

function defineStaticFunctions(cls: class_node, clsName: string): void {
    for (let [fcnName, desc] of Object.entries(cls.static_functions)) {
        if (desc.options["ignore"]) { continue }
        if (fcnName == "create"   ) { continue } //ignore builtin "create".

        let fcn = _meta.function_infos[desc.type]
        let argNames = desc.options["args"]

        if (desc.options["setter"]) {
            template([
                "    static set [[setName]](value: [[argType]]) {",
                "        native_[[clsName]]_[[fcnName]](",
                "            [[argCast]] runtime.getNative(",
                "                value",
                "            [[argCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName": clsName,
                "setName": SetterName(fcnName),
                "fcnName": fcnName,
                "argType": descriptType(fcn.args[0]),
                "argCast": isNeedCast(fcn.args[0]) ? "/**/" : "//",
            })

        } else if (desc.options["getter"]) {
            template([
                "    static get [[fcnName]](): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            native_[[clsName]]_[[fcnName]]()",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName": clsName,
                "fcnName": fcnName,
                "retType": descriptType(fcn.ret),
                "retCast": isNeedCast(fcn.ret) ? "/**/" : "//",
            })

        } else {
            template([
                "    static [[fcnName]]([[argNames]]): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            native_[[clsName]]_[[fcnName]]([[argCalls]])",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName" : clsName,
                "fcnName" : fcnName,
                "retType" : descriptType(fcn.ret),
                "retCast" : isNeedCast(fcn.ret) ? "/**/" : "//",

                "argNames": () => {
                    appendNamedArgs(argNames, fcn.args, descriptType)
                },

                "argCalls": () => {
                    appendCalledArgs(argNames, fcn.args, v => `runtime.getNative(${v})`)
                },
            })
        }
    }
}

function defineNativeInject(fcnName: string, desc: fcndesc_node): void {
    let fcn = _meta.function_infos[desc.type]
    let argNames = desc.options["args"]

    if (desc.options["setter"]) {
        template([
            "        this.injectFunction(native, '[[fcnName]]', (value: [[argType]]) => {",
            "            this.[[fcnName]] = value",
            "        })",
            "",
        ])
        .generate({
            "fcnName": fcnName,
            "argType": descriptType(fcn.args[1]),
        })

    } else if (desc.options["getter"]) {
        template([
            "        this.injectFunction(native, '[[fcnName]]', () => {",
            "            return (",
            "            [[retCast]] runtime.getNative(",
            "                this.[[fcnName]]",
            "            [[retCast]] )",
            "            )",
            "        })",
            "",
        ])
        .generate({
            "fcnName": fcnName,
            "retCast": isNeedCast(fcn.ret) ? "/**/" : "//",
        })

    } else {
        template([
            "        this.injectFunction(native, '[[fcnName]]', ([[argNames]]) => {",
            "            return (",
            "            [[retCast]] runtime.getNative(",
            "                this.[[fcnName]]([[argCalls]])",
            "            [[retCast]] )",
            "            )",
            "        })",
            "",
        ])
        .generate({
            "fcnName" : fcnName,
            "retCast" : isNeedCast(fcn.ret) ? "/**/" : "//",

            "argNames": () => {
                appendNamedArgs(argNames, fcn.args, _ => "any")
            },

            "argCalls": () => {
                appendCalledArgs(
                    argNames.slice(1), fcn.args.slice(1), v => `runtime.getObject(${v})`
                )
            },
        })
    }
}

function defineNativeInjects(clsName: string): void {
    let definedFcns = new Set<string>()

    //IMPORTANT: need traversing super class virtual functions.
    while (clsName != "MObject") {
        let cls = _meta.class_infos[clsName]

        for (let [fcnName, desc] of Object.entries(cls.inst_functions)) {
            if ( desc.options["ignore" ] ) { continue }
            if (!desc.options["virtual"] ) { continue }
            if ( definedFcns.has(fcnName)) { continue }

            definedFcns.add(fcnName)

            defineNativeInject(fcnName, desc)
        }

        clsName = cls.base_type
    }
}

function defineNativeCreator(cls: class_node, clsName: string): void {
    if (cls.abstracted) {
        return
    }

    template([
        "    protected createNative(): object {",
        "        let native = native_[[clsName]]_create()",
        "",
        "[[injecting]]",
        "",
        "        return native",
        "    }",
    ])
    .generate({
        "clsName"  : clsName,
        "injecting": () => defineNativeInjects(clsName),
    })
}

function defineInstFunctions(cls: class_node, clsName: string): void {
    for (let [fcnName, desc] of Object.entries(cls.inst_functions)) {
        if (desc.options["ignore"]) { continue }

        let fcn = _meta.function_infos[desc.type]
        let argNames = desc.options["args"]

        if (desc.options["setter"]) {
            template([
                "    set [[setName]](value: [[argType]]) {",
                "        native_[[clsName]]_[[fcnName]](",
                "            this.native,",
                "            [[argCast]] runtime.getNative(",
                "                value",
                "            [[argCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName": clsName,
                "setName": SetterName(fcnName),
                "fcnName": fcnName,
                "argType": descriptType(fcn.args[1]),
                "argCast": isNeedCast(fcn.args[1]) ? "/**/" : "//",
            })

        } else if (desc.options["getter"]) {
            template([
                "    get [[fcnName]](): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            native_[[clsName]]_[[fcnName]](this.native)",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName": clsName,
                "fcnName": fcnName,
                "retType": descriptType(fcn.ret),
                "retCast": isNeedCast(fcn.ret) ? "/**/" : "//",
            })

        } else {
            template([
                "    [[fcnName]]([[argNames]]): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            native_[[clsName]]_[[fcnName]](this.native [[comma]] [[argCalls]])",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName" : clsName,
                "fcnName" : fcnName,
                "retType" : descriptType(fcn.ret),
                "retCast" : isNeedCast(fcn.ret) ? "/**/" : "//",
                "comma"   : fcn.args.length > 1 ? "," : "",

                "argNames": () => {
                    appendNamedArgs(
                        argNames.slice(1), fcn.args.slice(1), descriptType
                    )
                },

                "argCalls": () => {
                    appendCalledArgs(
                        argNames.slice(1), fcn.args.slice(1), v => `runtime.getNative(${v})`
                    )
                },
            })
        }
    }
}

function defineClass(defined: Set<string>, clsName: string): void {
    if (defined.has(clsName)) {
        return
    }
    defined.add(clsName)

    //the base class is provided by runtime.
    if (clsName == "MObject") { return }

    let cls = _meta.class_infos[clsName]

    //IMPORTANT: define base class firstly.
    defineClass(defined, cls.base_type)

    template([
        "export class [[clsName]] extends [[baseName]] {",
        "",
        "[[classConstants]]",
        "",
        "[[staticFunctions]]",
        "",
        "    static {",
        "        runtime.register(this)",
        "    }",
        "",
        "[[nativeCreator]]",
        "",
        "[[instFunctions]]",
        "}",
        "",
    ])
    .generate({
        "clsName" : clsName,
        "baseName": cls.base_type,
        "newable" : !cls.abstracted ? "/**/" : "//",

        "classConstants" : () => defineClassConstants (cls),
        "staticFunctions": () => defineStaticFunctions(cls, clsName),
        "nativeCreator"  : () => defineNativeCreator  (cls, clsName),
        "instFunctions"  : () => defineInstFunctions  (cls, clsName),
    })
}

function defineClasses(): void {
    let defined = new Set<string>()
    for (let name of _meta.classes) {
        defineClass(defined, name)
    }
}

function defineFunctions(): void {
    for (let [fcnName, desc] of Object.entries(_meta.functions)) {
        if (desc.options["ignore"]) { continue }

        let fcn = _meta.function_infos[desc.type]
        let argNames = desc.options["args"]

        template([
            "export function [[fcnName]]([[argNames]]): [[retType]] {",
            "    return (",
            "    [[retCast]] runtime.getObject(",
            "        native_[[fcnName]]([[argCalls]])",
            "    [[retCast]] )",
            "    )",
            "}",
            "",
        ])
        .generate({
            "fcnName" : fcnName,
            "retType" : descriptType(fcn.ret),
            "retCast" :  isNeedCast(fcn.ret) ? "/**/" : "//",

            "argNames": () => {
                appendNamedArgs(argNames, fcn.args, descriptType)
            },

            "argCalls": () => {
                appendCalledArgs(argNames, fcn.args, v => `runtime.getNative(${v})`)
            },
        })
    }
}

function defineScripts(): void {
    template([
        "export class MObject    extends runtime.Injectable {}",
        "export class MVector<T> extends runtime.MVector<T> {}",
        "export class MMap<K, T> extends runtime.MMap<K, T> {}",
        "export class MSet<T>    extends runtime.MSet<T>    {}",
        "",
        "[[constants]]",
        "",
        "[[enums]]",
        "",
        "[[classes]]",
        "",
        "[[functions]]",
        "",
    ])
    .generate({
        "constants": () => defineConstants(),
        "enums"    : () => defineEnums    (),
        "classes"  : () => defineClasses  (),
        "functions": () => defineFunctions(),
    })
}

function makeDTS(): void {
    template([
        "//the file was generated by automation tool.",
        "",
        "import { runtime } from './runtime'",
        "",
        "[[natives]]",
        "",
        "[[scripts]]",
        "",
    ])
    .generate({
        "natives": () => declareNatives(),
        "scripts": () => defineScripts (),
    })
}

//main:

declare function native_temporaryDirectory(): string
declare function native_writeTextFile(path: string, content: string): void

(function () {
    makeDTS()

    let file = ""
    // file = ".../fw-mini/appts/app/host/native.ts"
    // file = `${native_temporaryDirectory()}/native.ts`
    if (file) {
        native_writeTextFile(file, _buffer.join(""))
        console.log(`output: ${file}`)
    }
})()

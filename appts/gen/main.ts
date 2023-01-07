//meta description:

declare function metaJsonDescription(): string

class fdesc_node {
    type: string
    note: string

    //additional custom field.
    options: { [key: string]: string[] }
}

class class_node {
    base_type       : string
    abstracted      : boolean
    static_strings  : { [key: string]: string }
    static_numbers  : { [key: string]: number }
    static_functions: { [key: string]: fdesc_node }
    inst_functions  : { [key: string]: fdesc_node }
}

class function_node {
    arg_qualifiers: string[]
    arg_types     : string[]
    ret_qualifier : string
    ret_type      : string
}

class enum_node {
    values: { [key: string]: number }
}

class map_node {
    key_type: string
    val_type: string
}

class vector_node {
    val_type: string
}

class set_node {
    val_type: string
}

class root_node {
    class_infos   : { [key: string]: class_node    }
    function_infos: { [key: string]: function_node }
    enum_infos    : { [key: string]: enum_node     }
    map_infos     : { [key: string]: map_node      }
    vector_infos  : { [key: string]: vector_node   }
    set_infos     : { [key: string]: set_node      }

    strings  : { [key: string]: string     }
    numbers  : { [key: string]: number     }
    functions: { [key: string]: fdesc_node }
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
    let json = metaJsonDescription()
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

function descriptType(type: string): string {
    let cls = _meta.class_infos[type]
    if (cls) {
        switch (type) {
            //special classes.
            case "generic_function": return "Function"
            case "generic_map"     : return "any"
            case "generic_set"     : return "any"
            case "generic_vector"  : return "any"

            default: return type
        }
    }

    let fcn = _meta.function_infos[type]
    if (fcn) {
        let buf = new Array<string>()

        //arguments.
        buf.push("(")
        for (let i = 0; i < fcn.arg_types.length; ++i) {
            if (i > 0) {
                buf.push(", ")
            }
            buf.push(descriptType(fcn.arg_types[i]))
        }
        //return value.
        buf.push(`) => ${descriptType(fcn.ret_type)}`)

        return buf.join("")
    }

    let enu = _meta.enum_infos[type]
    if (enu) {
        return type
    }

    let map = _meta.map_infos[type]
    if (map) {
        return `MSet<${descriptType(map.key_type)}, ${descriptType(map.val_type)}>`
    }

    let vector = _meta.vector_infos[type]
    if (vector) {
        return `MSet<${descriptType(vector.val_type)}>`
    }

    let set = _meta.set_infos[type]
    if (set) {
        return `MSet<${descriptType(set.val_type)}>`
    }

    switch (type) {
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

function isNeedCast(type: string): boolean {
    return _meta.class_infos[type] && type != "generic_function"
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
    argNames: string[], argTypes: string[], cast: (type: string) => string): void
{
    for (let i = 0; i < argNames.length; ++i) {
        if (i > 0) {
            _buffer.push(", ")
        }

        _buffer.push(`${argNames[i]}: ${cast(argTypes[i])}`)
    }
}

function appendCalledArgs(
    argNames: string[], argTypes: string[], cast: (src: string) => string): void
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

function declareGlobals(): void {
    for (let clsName of _meta.classes) {
        if (clsName == "MObject") { continue } //the base class is provided by runtime.
        
        let cls = _meta.class_infos[clsName]

        //class static functions.
        for (let [fcnName, desc] of Object.entries(cls.static_functions)) {
            if (desc.options["ignore"]) { continue }

            template([
                "declare function [[cls]]_[[name]]([[args]]): any"
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
                "declare function [[cls]]_[[name]]([[args]]): any"
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
            "declare function [[name]]([[args]]): any"
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
            "    const [[name]] = '[[value]]'"
        ])
        .generate({
            "name" : name ,
            "value": value,
        })
    }

    //constant numbers.
    for (let [name, value] of Object.entries(_meta.numbers)) {
        template([
            "    export const [[name]] = [[value]]"
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
            "        [[member]] = [[value]],",
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
            "    export enum [[enumName]] {",
            "[[members]]",
            "    }",
            "",
        ])
        .generate({
            "enumName": enumName,
            "members" : () => defineEnumMember(enu),
        })
    }
}

function defineClassStaticFunctions(cls: class_node, clsName: string): void {
    for (let [fcnName, desc] of Object.entries(cls.static_functions)) {
        if (desc.options["ignore"]) { continue }
        if (fcnName == "create"   ) { continue } //ignore builtin "create".

        let fcn = _meta.function_infos[desc.type]

        let argNames = desc.options["args"]
        let argTypes = fcn.arg_types

        if (desc.options["setter"]) {
            template([
                "    static set [[setName]](value: [[argType]]) {",
                "        global.[[clsName]]_[[fcnName]](",
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
                "argType": descriptType(fcn.arg_types[0]),
                "argCast": isNeedCast(fcn.arg_types[0]) ? "/**/" : "//",
            })

        } else if (desc.options["getter"]) {
            template([
                "    static get [[fcnName]](): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            global.[[clsName]]_[[fcnName]]()",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName": clsName,
                "fcnName": fcnName,
                "retType": descriptType(fcn.ret_type),
                "retCast": isNeedCast(fcn.ret_type) ? "/**/" : "//",
            })

        } else {
            template([
                "    static [[fcnName]]([[argNames]]): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            global.[[clsName]]_[[fcnName]]([[argCalls]])",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName" : clsName,
                "fcnName" : fcnName,
                "retType" : descriptType(fcn.ret_type),
                "retCast" : isNeedCast(fcn.ret_type) ? "/**/" : "//",

                "argNames": () => {
                    appendNamedArgs(argNames, argTypes, descriptType)
                },

                "argCalls": () => {
                    appendCalledArgs(argNames, argTypes, v => `runtime.getNative(${v})`)
                },
            })
        }
    }
}

function defineClassInstFunctions(cls: class_node, clsName: string): void {
    for (let [fcnName, desc] of Object.entries(cls.inst_functions)) {
        if (desc.options["ignore"]) { continue }

        let fcn      = _meta.function_infos[desc.type]
        let argNames = desc.options["args"]
        let argTypes = fcn.arg_types

        if (desc.options["setter"]) {
            template([
                "    set [[setName]](value: [[argType]]) {",
                "        global.[[clsName]]_[[fcnName]](",
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
                "argType": descriptType(fcn.arg_types[1]),
                "argCast": isNeedCast(fcn.arg_types[1]) ? "/**/" : "//",
            })

        } else if (desc.options["getter"]) {
            template([
                "    get [[fcnName]](): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            global.[[clsName]]_[[fcnName]](this.native)",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName": clsName,
                "fcnName": fcnName,
                "retType": descriptType(fcn.ret_type),
                "retCast": isNeedCast(fcn.ret_type) ? "/**/" : "//",
            })

        } else {
            template([
                "    [[fcnName]]([[argNames]]): [[retType]] {",
                "        return (",
                "        [[retCast]] runtime.getObject(",
                "            global.[[clsName]]_[[fcnName]](this.native [[comma]] [[argCalls]])",
                "        [[retCast]] )",
                "        )",
                "    }",
                "",
            ])
            .generate({
                "clsName" : clsName,
                "fcnName" : fcnName,
                "retType" : descriptType(fcn.ret_type),
                "retCast" : isNeedCast(fcn.ret_type) ? "/**/" : "//",
                "comma"   : argTypes.length > 1 ? "," : "",

                "argNames": () => {
                    appendNamedArgs(
                        argNames.slice(1), argTypes.slice(1), descriptType
                    )
                },

                "argCalls": () => {
                    appendCalledArgs(
                        argNames.slice(1), argTypes.slice(1), v => `runtime.getNative(${v})`
                    )
                },
            })
        }
    }
}

function defineClassConstantFields(cls: class_node): void {
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

function defineClassBlockVirtuals(cls: class_node, clsName: string): void {
    for (let [fcnName, desc] of Object.entries(cls.inst_functions)) {
        if ( desc.options["ignore" ]) { continue }
        if (!desc.options["virtual"]) { continue }

        template([
            "        runtime.inject(this.classSymbol, '[[fcnName]]', this.js_[[clsName]]_[[fcnName]])"
        ])
        .generate({
            "clsName": clsName,
            "fcnName": fcnName,
        })
    }
}

function defineClassBlock(cls: class_node, clsName: string): void {
    template([
        "    static {",
        "        runtime.register(this)",
        "",
        "[[virtuals]]",
        "    }",
        "",
    ])
    .generate({
        "virtuals": () => defineClassBlockVirtuals(cls, clsName)
    })
}

function defineClassStaticVirtuals(cls: class_node, clsName: string): void {
    for (let [fcnName, desc] of Object.entries(cls.inst_functions)) {
        if ( desc.options["ignore" ]) { continue }
        if (!desc.options["virtual"]) { continue }

        let fcn      = _meta.function_infos[desc.type]
        let argNames = desc.options["args"]
        let argTypes = fcn.arg_types

        template([
            "    private static js_[[clsName]]_[[fcnName]]([[argNames]]): any {",
            "        return (",
            "        [[retCast]] runtime.getNative(",
            "            runtime.getObject(_this).[[fcnName]]([[argCalls]])",
            "        [[retCast]] )",
            "        )",
            "    }",
            "",
        ])
        .generate({
            "clsName" : clsName,
            "fcnName" : fcnName,
            "retCast" : isNeedCast(fcn.ret_type) ? "/**/" : "//",

            "argNames": () => {
                appendNamedArgs(argNames, argTypes, _ => "any")
            },

            "argCalls": () => {
                appendCalledArgs(
                    argNames.slice(1), argTypes.slice(1), v => `runtime.getObject(${v})`
                )
            },
        })
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
        "    protected static classSymbol = 'js_[[clsName]]'",
        "",
        "[[constants_fields]]",
        "",
        "[[static_virtuals]]",
        "",
        "[[static_block]]",
        "",
        "[[static_functions]]",
        "",
        "    [[newable]] protected createInjectableNative(): object {",
        "    [[newable]]     return [[clsName]]_create()",
        "    [[newable]] }",
        "",
        "[[inst_functions]]",
        "}",
    ])
    .generate({
        "clsName" : clsName,
        "baseName": cls.base_type,
        "newable" : !cls.abstracted ? "/**/" : "//",

        "constants_fields": () => defineClassConstantFields (cls),
        "static_virtuals" : () => defineClassStaticVirtuals (cls, clsName),
        "static_block"    : () => defineClassBlock          (cls, clsName),
        "static_functions": () => defineClassStaticFunctions(cls, clsName),
        "inst_functions"  : () => defineClassInstFunctions  (cls, clsName),
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

        let fcn      = _meta.function_infos[desc.type]
        let argNames = desc.options["args"]
        let argTypes = fcn.arg_types

        template([
            "export function [[fcnName]]([[argNames]]): [[retType]] {",
            "    return (",
            "    [[retCast]] runtime.getObject(",
            "        global.[[fcnName]]([[argCalls]])",
            "    [[retCast]] )",
            "    )",
            "}",
            "",
        ])
        .generate({
            "fcnName" : fcnName,
            "retType" : descriptType(fcn.ret_type),
            "retCast" :  isNeedCast(fcn.ret_type) ? "/**/" : "//",

            "argNames": () => {
                appendNamedArgs(argNames, argTypes, descriptType)
            },

            "argCalls": () => {
                appendCalledArgs(argNames, argTypes, v => `runtime.getNative(${v})`)
            },
        })
    }
}

function defineNamespace(): void {
    template([
        "export namespace cc {",
        "",
        "    export class MObject    extends runtime.Injectable {}",
        "    export class MVector<T> extends runtime.MVector<T> {}",
        "    export class MMap<K, T> extends runtime.MMap<K, T> {}",
        "    export class MSet<T>    extends runtime.MSet<T>    {}",
        "",
        "[[constants]]",
        "",
        "[[enums]]",
        "",
        "[[classes]]",
        "",
        "[[functions]]",
        "}",
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
        "[[globals]]",
        "",
        "[[namespace]]",
        "",
    ])
    .generate({
        "globals"  : () => declareGlobals (),
        "namespace": () => defineNamespace(),
    })
}

//main:

declare function temporaryDirectory(): string
declare function writeTextFile(path: string, content: string): void

(function () {
    makeDTS()

    let file = ''
    // file = ".../fw-mini/appts/app/host/native.ts"
    // file = `${temporaryDirectory()}/native.ts`
    if (file) {
        writeTextFile(file, _buffer.join(""))
        console.log(`output: ${file}`)
    }
})()

//native meta:

declare function MMetaJsonDescription(): string

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
    basic_infos   : { [key: string]: string        }

    strings  : { [key: string]: string     }
    numbers  : { [key: string]: number     }
    functions: { [key: string]: fdesc_node }
    classes  : string[]
    enums    : string[]
}

//generate:

let _json: string
let _meta: root_node
let _out : string[]

function AppendType(type: string, explicit: boolean): void {
    let cls = _meta.class_infos[type]
    if (cls) {
        //special classes.
        if /**/ (type == 'generic_function') { _out.push(explicit ? 'Function'    : 'Function') }
        else if (type == 'generic_map'     ) { _out.push(explicit ? 'MMap<any>'   : 'any'     ) }
        else if (type == 'generic_set'     ) { _out.push(explicit ? 'MSet<any>'   : 'any'     ) }
        else if (type == 'generic_vector'  ) { _out.push(explicit ? 'Mvector<any>': 'any'     ) }
        
        else {
            _out.push(explicit ? type : 'object')
        }

        return
    }

    let func = _meta.function_infos[type]
    if (func) {
        //arguments.
        _out.push(`(`)
        for (let i = 0; i < func.arg_types.length; ++i) {
            if (i > 0) {
                _out.push(`, `)
            }
            AppendType(func.arg_types[i], explicit)
        }

        //return value.
        _out.push(`) => `)
        AppendType(func.ret_type, explicit)

        return
    }

    let enu = _meta.enum_infos[type]
    if (enu) {
        if (explicit) {
            _out.push(type)
        } else {
            _out.push(`number`)
        }

        return
    }

    let map = _meta.map_infos[type]
    if (map) {
        if (explicit) {
            _out.push(`MMap<`)
            AppendType(map.key_type, explicit)
            _out.push(`,`)
            AppendType(map.val_type, explicit)
            _out.push(`>`)
        } else {
            _out.push(`any`)
        }

        return
    }

    let vector = _meta.vector_infos[type]
    if (vector) {
        if (explicit) {
            _out.push(`MVector<`)
            AppendType(vector.val_type, explicit)
            _out.push(`>`)
        } else {
            _out.push(`any`)
        }

        return
    }

    let set = _meta.set_infos[type]
    if (set) {
        if (explicit) {
            _out.push(`MSet<`)
            AppendType(set.val_type, explicit)
            _out.push(`>`)
        } else {
            _out.push(`any`)
        }

        return
    }

    let basic = _meta.basic_infos[type]
    if (basic) {
        switch (type) {
            case 'any'   : _out.push(`any`    ); break
            case 'void'  : _out.push(`void`   ); break
            case 'bool'  : _out.push(`boolean`); break
            case 'char'  : _out.push(`number` ); break
            case 'byte'  : _out.push(`number` ); break
            case 'int'   : _out.push(`number` ); break
            case 'int64' : _out.push(`number` ); break
            case 'float' : _out.push(`number` ); break
            case 'double': _out.push(`number` ); break
            case 'string': _out.push(`string` ); break
        }
    }
}

function AppendFuncArguments(desc: fdesc_node, argBegin: number, explicit: boolean): void {
    _out.push(`(`)

    let func = _meta.function_infos[desc.type]
    let args = desc.options['args']

    for (let i = argBegin; i < func.arg_types.length; ++i) {
        if (i > argBegin) {
            _out.push(', ')
        }

        _out.push(`${args[i]}: `)
        AppendType(func.arg_types[i], explicit)
    }

    _out.push(`)`)
}

function AppendFuncBody(name: string, desc: fdesc_node, argBegin: number): void {
    let func = _meta.function_infos[desc.type]
    let args = desc.options['args']

    //name.
    if (_meta.class_infos[func.ret_type] && func.ret_type != 'generic_function') {
        _out.push(`return runtime.getObject(${name}(`)

    } else if (func.ret_type != 'void') {
        _out.push(`return ${name}(`)

    } else {
        _out.push(`${name}(`)
    }

    //arguments.
    for (let i = argBegin; i < func.arg_types.length; ++i) {
        if (i > argBegin) {
            _out.push(', ')
        }

        let type = func.arg_types[i]
        let name = args[i]

        if (name == '_this') {
            _out.push(`this.native`)
        
        } else if (_meta.class_infos[type] && type != 'generic_function') {
            _out.push(`${name}.native`)

        } else {
            _out.push(name)
        }
    }

    //end.
    if (_meta.class_infos[func.ret_type] && func.ret_type != 'generic_function') {
        _out.push(`))`)
    } else {
        _out.push(`)`)
    }
}

function DeclareImport(): void {
    _out.push(`import { runtime } from '../runtime/runtime'\n\n`)
}

function DeclareClassFunctions(): void {
    for (let name of _meta.classes) {
        let cls = _meta.class_infos[name]

        for (let [field, desc] of Object.entries(cls.static_functions)) {
            if (desc.options['ignore']) {
                continue
            }

            //name.
            _out.push(`declare function ${name}_${field}`)

            //arguments.
            AppendFuncArguments(desc, 0, false)

            //return value.
            _out.push(`: `)
            AppendType(_meta.function_infos[desc.type].ret_type, false)
            _out.push(`\n`)
        }
        _out.push('\n')

        for (let [field, desc] of Object.entries(cls.inst_functions)) {
            if (desc.options['ignore']) {
                continue
            }
            
            //name.
            _out.push(`declare function ${name}_${field}`)

            //arguments.
            AppendFuncArguments(desc, 0, false)
            
            //return value.
            _out.push(`: `)
            AppendType(_meta.function_infos[desc.type].ret_type, false)
            _out.push(`\n`)
        }
        _out.push('\n')
    }
}

function DeclareGlobalFunctions(): void {
    for (let [name, desc] of Object.entries(_meta.functions)) {
        if (desc.options['ignore']) {
            continue
        }

        //name.
        _out.push(`declare function ${name}`)

        //arguments.
        AppendFuncArguments(desc, 0, false)
        
        //return value.
        _out.push(`: `)
        AppendType(_meta.function_infos[desc.type].ret_type, false)
        _out.push(`\n`)
    }
    _out.push(`\n`)
}

function BeginNamespace(): void {
    _out.push(`export namespace cc {\n`)
    _out.push(`\n`)
    _out.push(`export class MObject    extends runtime.Injectable {}\n`)
    _out.push(`export class MVector<T> extends runtime.MVector<T> {}\n`)
    _out.push(`export class MMap<K, T> extends runtime.MMap<K, T> {}\n`)
    _out.push(`export class MSet<T>    extends runtime.MSet<T>    {}\n`)
    _out.push(`\n`)
}

function EndNamespace(): void {
    _out.push(`} // end namspace cc.\n`)
}

function DefineConstants(): void {
    //strings.
    for (let [name, value] of Object.entries(_meta.strings)) {
        _out.push(`export const ${name} = '${value}'\n`)
    }
    _out.push(`\n`)

    //numbers.
    for (let [name, value] of Object.entries(_meta.strings)) {
        _out.push(`export const ${name} = ${value}\n`)
    }
    _out.push(`\n`)
}

function DefineEnums(): void {
    for (let name of _meta.enums) {
        _out.push(`export enum ${name} {\n`)

        let enu = _meta.enum_infos[name]
        for (let [member, value] of Object.entries(enu.values)) {
            _out.push(`    ${member} = ${value},\n`)
        }

        _out.push(`}\n\n`)
    }
}

function SetterName(raw: string): string {
    if (raw.length <= 3 || !raw.startsWith('set')) {
        return raw
    }

    let head = raw.substring(3, 4)
    let body = raw.substring(4)
    return head.toLowerCase() + body
}

function DefineClass(defined: Set<string>, name: string): void {
    if (defined.has(name)) {
        return
    }
    defined.add(name)

    //the base class is provided by the runtime.
    if (name == 'MObject') {
        return
    }

    let cls = _meta.class_infos[name]

    //base class.
    DefineClass(defined, cls.base_type)

    //head:
    _out.push(`export class ${name} extends ${cls.base_type} {\n`)
    _out.push(`    protected static classSymbol = '${name}_js'\n`)
    _out.push(`    \n`)
    _out.push(`    static {\n`)
    _out.push(`        runtime.register(this)\n`)
    _out.push(`    }\n\n`)

    if (!cls.abstracted) {
        _out.push(`    protected createInjectableNative(): object {\n`)
        _out.push(`        return ${name}_create()\n`)
        _out.push(`    }\n\n`)
    }

    //static constants:
    for (let [field, value] of Object.entries(cls.static_strings)) {
        _out.push(`    static readonly ${field} = '${value}'\n`)
    }
    _out.push(`\n`)

    for (let [field, value] of Object.entries(cls.static_numbers)) {
        _out.push(`    static readonly ${field} = ${value}\n`)
    }
    _out.push(`\n`)

    //static functions.
    for (let [field, desc] of Object.entries(cls.static_functions)) {
        if (desc.options['ignore']) {
            continue
        }

        //ignore builtin "create" function.
        if (field == 'create') {
            continue
        }

        //name.
        if (desc.options['setter']) {
            _out.push(`    static set ${SetterName(field)}`)
        } else if (desc.options['getter']) {
            _out.push(`    static get ${field}`)
        } else {
            _out.push(`    static ${field}`)
        }
        
        //arguments.
        AppendFuncArguments(desc, 0, true)

        //return value.
        if (!desc.options['setter']) {
            _out.push(`: `)
            AppendType(_meta.function_infos[desc.type].ret_type, true)
        }

        //body.
        _out.push(` {\n        `)
        AppendFuncBody(`${name}_${field}`, desc, 0)
        _out.push(`\n    }\n\n`)
    }

    //instance functions.
    for (let [field, desc] of Object.entries(cls.inst_functions)) {
        if (desc.options['ignore']) {
            continue
        }

        //name.
        if (desc.options['setter']) {
            _out.push(`    set ${SetterName(field)}`)
        } else if (desc.options['getter']) {
            _out.push(`    get ${field}`)
        } else {
            _out.push(`    ${field}`)
        }
        
        //arguments.
        AppendFuncArguments(desc, 1, true)

        //return value.
        if (!desc.options['setter']) {
            _out.push(`: `)
            let ret = _meta.function_infos[desc.type].ret_type
            AppendType(ret, true)
        }

        //body.
        _out.push(` {\n        `)
        AppendFuncBody(`${name}_${field}`, desc, 0)
        _out.push(`\n    }\n\n`)
    }

    //end.
    _out.push(`}\n\n`)
}

function DefineClasses(): void {
    let defined = new Set<string>()

    for (let name of _meta.classes) {
        DefineClass(defined, name)
    }
}

function DefineFunctions(): void {
    for (let [name, desc] of Object.entries(_meta.functions)) {
        if (desc.options['ignore']) {
            continue
        }

        let func = _meta.function_infos[desc.type]

        //name.
        _out.push(`export function ${name}`)

        //arguments.
        AppendFuncArguments(desc, 0, true)
        
        //return value.
        _out.push(`: `)
        AppendType(func.ret_type, true)

        //body.
        _out.push(` {\n    `)
        AppendFuncBody(`global.${name}`, desc, 0)
        _out.push(`\n}\n\n`)
    }
}

function Generate(): void {
    DeclareImport()

    DeclareClassFunctions ()
    DeclareGlobalFunctions()

    BeginNamespace ()
    DefineConstants()
    DefineEnums    ()
    DefineClasses  ()
    DefineFunctions()
    EndNamespace   ()
}

//main:

declare function MWriteTextFile(path: string, content: string): void

function ParseFuncNote(note: string): { [key: string]: string[] } {
    let options = {}

    if (!note) {
        return options
    }
    for (let segment of note.split(';')) {
        let sep = segment.indexOf(':')
        if (sep != -1) {
            let name = segment.substring(0, sep)
            let args = segment.substring(sep + 1)
            options[name] = args.split(',')

        } else {
            //only option name, no arguments.
            options[segment] = []
        }
    }

    return options
}

function FillFuncOptions(meta: root_node): void {
    //class functions.
    for (let name of meta.classes) {
        let cls = meta.class_infos[name]

        for (let desc of Object.values(cls.inst_functions)) {
            desc.options = ParseFuncNote(desc.note)

            //the first argument is implied "this".
            let args = desc.options['args']
            if (args) {
                desc.options['args'] = [ '_this', ...args]
            } else {
                desc.options['args'] = [ '_this' ]
            }
        }

        for (let desc of Object.values(cls.static_functions)) {
            desc.options = ParseFuncNote(desc.note)
        }
    }

    //global functions.
    for (let desc of Object.values(meta.functions)) {
        desc.options = ParseFuncNote(desc.note)
    }
}

function Main(): void {
    _json = MMetaJsonDescription()
    _meta = JSON.parse(_json)
    FillFuncOptions(_meta)

    _out = new Array<string>()
    Generate()

    //MWriteTextFile('file/path/to/write', _out.join(''))
}

Main()

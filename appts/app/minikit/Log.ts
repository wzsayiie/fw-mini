export class Log {

    public static I(...values: any[]): void {
        for (let value of values) {
            let buffer = new Array<string>()
            this.Stringify(false, 0, value, buffer)
            MPrintMessage(buffer.join(''))
        }
    }

    private static Stringify(headIndent: boolean, level: number, value: any, buffer: string[]): void {
        if (headIndent) {
            this.Indent(level, buffer)
        }

        if (value === undefined) {
            buffer.push('undefined')

        } else if (value === null) {
            buffer.push('null')

        } else if (this.IsType('Array', value) || this.IsType('Set', value)) {
            buffer.push('[\n')

            for (let item of value) {
                this.Stringify(true, level + 1, item, buffer)
                buffer.push(',\n')
            }

            this.Indent(level, buffer)
            buffer.push(']')

        } else if (this.IsType('Map', value)) {
            buffer.push('{\n')

            for (let [key, obj] of value) {
                this.Stringify(true , level + 1, key, buffer); buffer.push(': ' )
                this.Stringify(false, level + 1, obj, buffer); buffer.push(',\n')
            }

            this.Indent(level, buffer)
            buffer.push('}')

        } else if (this.IsType('string', value)) {
            buffer.push(level > 0 ? `'${value}'` : value)

        } else if (this.IsType('function', value)) {
            buffer.push(`<function ${value.name}>`)

        } else if (this.IsType('object', value)) {
            //do not iterate an object, it is maybe huge.
            buffer.push(JSON.stringify(value))

        } else {
            buffer.push(String(value))
        }
    }

    private static Indent(level: number, buffer: string[]): void {
        while (level-- > 0) {
            buffer.push('    ')
        }
    }

    private static IsType(type: string, thing: any): boolean {
        if (type == 'Map') {
            return Object.getPrototypeOf(thing).constructor == Map
        }
        if (type == 'Set') {
            return Object.getPrototypeOf(thing).constructor == Set
        }
        if (type == 'Array') {
            return Array.isArray(thing)
        }
        return typeof(thing) == type
    }
}

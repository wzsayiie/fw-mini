import {
    runtime,
} from "../host/runtime"

export namespace classes {
    let _classes = new Map<string, { new(): object }>()

    export function register<T extends { new(): object }>(cls: T): void {
        _classes.set(cls.name, cls)
    }

    export function newObject(clsName: string): any {
        //check registered classes.
        let cls = _classes.get(clsName)
        if (cls) {
            return new cls()
        }

        //check native classes.
        runtime.newObject(clsName)
    }
}

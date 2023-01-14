//BaseObject & Injectable:

declare function native_MGetObjectClass(native: object): string
declare function native_MInjectFunction(native: object, name: string, func: Function): void
declare function native_MDisposeObject (native: object): void

export namespace runtime {
    let _mappedClasses = new Map<string, { new(): BaseObject }>()
    let _mappedObjects = new WeakMap<object, any>()
    let _bindingNative = null

    export function register<T extends { new(): BaseObject }>(cls: T): void {
        _mappedClasses.set(cls.name, cls)
    }

    export function newObject(clsName: string): any {
        let cls = _mappedClasses.get(clsName)
        if (cls) {
            return new cls()
        } else {
            return null
        }
    }

    export function getObject(native: object): any {
        //null.
        if (!native) {
            return null
        }

        //query native from cache.
        let obj = _mappedObjects.get(native)
        if (obj) {
            return obj
        }

        //new object.
        let sym = native_MGetObjectClass(native)
        let cls = _mappedClasses.get(sym)
        if (cls) {
            _bindingNative = native
            obj = new cls()
            return obj
        }
        return null
    }

    export function getNative(obj: BaseObject): object {
        return obj?.native
    }

    export class BaseObject {
        private _native: object

        public constructor() {
            if (_bindingNative) {
                //use the object passed from native side.
                this._native = _bindingNative
                _bindingNative = null
            } else {
                //new a native object.
                this._native = this.createNative()
            }

            _mappedObjects.set(this._native, this)
        }

        public get native(): object {
            return this._native
        }

        //subclasses need to override this method.
        protected createNative(): object {
            return null
        }
    }

    export class Injectable extends BaseObject {

        protected injectFunction(native: object, name: string, func: Function): void {
            native_MInjectFunction(native, name, func)
        }

        public dispose(): void {
            native_MDisposeObject(this.native)
        }
    }
}

//MVector:

declare function native_MVector_create   ()                                         : object
declare function native_MVector_insert   (native: object, index: number, value: any): void
declare function native_MVector_erase    (native: object, index: number)            : void
declare function native_MVector_push_back(native: object, value: any   )            : void
declare function native_MVector_pop_back (native: object)                           : void
declare function native_MVector_clear    (native: object)                           : void
declare function native_MVector_size     (native: object)                           : number
declare function native_MVector_at       (native: object, index: number)            : any
declare function native_MVector_back     (native: object)                           : any

export namespace runtime {
    export class MVector<Value> extends BaseObject {
        protected createNative(): object {
            return native_MVector_create()
        }

        public *[Symbol.iterator]() {
            for (let i = 0; i < this.size; ++i) {
                yield native_MVector_at(this.native, i)
            }
        }
    
        public insert(index: number, value: Value): void {
            native_MVector_insert(this.native, index, value)
        }

        public erase(index: number): void {
            native_MVector_erase(this.native, index)
        }
        
        public push_back(value: Value): void {
            native_MVector_push_back(this.native, value)
        }
        
        public pop_back(): void {
            native_MVector_pop_back(this.native)
        }
        
        public clear(): void {
            native_MVector_clear(this.native)
        }
        
        public get size(): number {
            return native_MVector_size(this.native)
        }
        
        public at(index: number): Value {
            return native_MVector_at(this.native, index)
        }

        public get back(): Value {
            return native_MVector_back(this.native)
        }
    }
}

//MMap:

declare function native_MMap_create()                                    : object
declare function native_MMap_insert(native: object, key: any, value: any): void
declare function native_MMap_erase (native: object, key: any)            : void
declare function native_MMap_clear (native: object)                      : void
declare function native_MMap_has   (native: object, key: any)            : boolean
declare function native_MMap_get   (native: object, key: any)            : any
declare function native_MMap_size  (native: object)                      : number
declare function native_MMap_begin (native: object)                      : void
declare function native_MMap_on    (native: object)                      : boolean
declare function native_MMap_next  (native: object)                      : void
declare function native_MMap_key   (native: object)                      : any
declare function native_MMap_value (native: object)                      : any

export namespace runtime {
    export class MMap<Key, Value> extends BaseObject {
        protected createNative(): object {
            return native_MMap_create()
        }

        public *[Symbol.iterator]() {
            let n = this.native
            for (native_MMap_begin(n); native_MMap_on(n); native_MMap_next(n)) {
                let k = native_MMap_key  (n)
                let v = native_MMap_value(n)
    
                yield [k, v]
            }
        }
    
        public insert(key: Key, value: Value): void {
            native_MMap_insert(this.native, key, value)
        }

        public erase(key: Key): void {
            native_MMap_erase(this.native, key)
        }

        public clear(): void {
            native_MMap_clear(this.native)
        }

        public has(key: Key): boolean {
            return native_MMap_has(this.native, key)
        }

        public get(key: Key): Value {
            return native_MMap_get(this.native, key)
        }

        public get size(): number {
            return native_MMap_size(this.native)
        }
    }
}

//MSet:

declare function native_MSet_create()                          : object
declare function native_MSet_insert(native: object, value: any): void
declare function native_MSet_erase (native: object, value: any): void
declare function native_MSet_clear (native: object)            : void
declare function native_MSet_has   (native: object, value: any): boolean
declare function native_MSet_size  (native: object)            : number
declare function native_MSet_begin (native: object)            : void
declare function native_MSet_on    (native: object)            : boolean
declare function native_MSet_next  (native: object)            : void
declare function native_MSet_value (native: object)            : any

export namespace runtime {
    export class MSet<Value> extends BaseObject {
        protected createNative(): object {
            return native_MSet_create()
        }

        public *[Symbol.iterator]() {
            let n = this.native
            for (native_MSet_begin(n); native_MSet_on(n); native_MSet_next(n)) {
                yield native_MSet_value(n)
            }
        }
    
        public insert(value: Value): void {
            native_MSet_insert(this.native, value)
        }

        public erase(value: Value): void {
            native_MSet_erase(this.native, value)
        }

        public clear(): void {
            native_MSet_clear(this.native)
        }

        public has(value: Value): boolean {
            return native_MSet_has(this.native, value)
        }

        public get size(): number {
            return native_MSet_size(this.native)
        }
    }
}

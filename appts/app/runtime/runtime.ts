//BaseObject & Injectable:

declare function MGetObjectClassSymbol(native: object): string
declare function MInjectObjectSymbol  (native: object, sym: string): void
declare function MInjectClassFunction (clsSym: string, fcnName: string, fcn: Function): void

export namespace runtime {
    let _mappedClasses = new Map<string, { new(): BaseObject }>()
    let _mappedObjects = new WeakMap<object, any>()
    let _bindingNative = null

    export function register<T extends { new(): BaseObject }>(cls: T): void {
        _mappedClasses.set(cls.name, cls)
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
        let sym = MGetObjectClassSymbol(native)
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
                this._native = _bindingNative
                _bindingNative = null
            } else {
                this._native = this.createNative()
            }

            if (!this._native) {
                throw `${this.constructor.name} hold no native object.`
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

    export function inject(clsSym: string, fcnName: string, fcn: Function): void {
        MInjectClassFunction(clsSym, fcnName, fcn)
    }

    export class Injectable extends BaseObject {
        protected createNative(): object {
            let ntv = this.createInjectableNative()
            let sym = (<any> this.constructor).classSymbol
            if (ntv && sym) {
                MInjectObjectSymbol(ntv, sym)
            }
            return ntv
        }

        //subclasses need to override this field.
        protected static classSymbol: string

        //subclasses need to override this method.
        protected createInjectableNative(): object {
            return null
        }
    }
}

//MVector:

declare function MVector_create   ()                                         : object
declare function MVector_insert   (native: object, index: number, value: any): void
declare function MVector_erase    (native: object, index: number)            : void
declare function MVector_push_back(native: object, value: any   )            : void
declare function MVector_pop_back (native: object)                           : void
declare function MVector_clear    (native: object)                           : void
declare function MVector_size     (native: object)                           : number
declare function MVector_at       (native: object, index: number)            : any
declare function MVector_back     (native: object)                           : any

export namespace runtime {
    export class MVector<Value> extends BaseObject {
        protected createNative(): object {
            return MVector_create()
        }
    
        public insert(index: number, value: Value): void {
            MVector_insert(this.native, index, value)
        }

        public erase(index: number): void {
            MVector_erase(this.native, index)
        }
        
        public push_back(value: Value): void {
            MVector_push_back(this.native, value)
        }
        
        public pop_back(): void {
            MVector_pop_back(this.native)
        }
        
        public clear(): void {
            MVector_clear(this.native)
        }
        
        public get size(): number {
            return MVector_size(this.native)
        }
        
        public at(index: number): Value {
            return MVector_at(this.native, index)
        }

        public get back(): Value {
            return MVector_back(this.native)
        }
    
        public *[Symbol.iterator]() {
            for (let i = 0; i < this.size; ++i) {
                yield MVector_at(this.native, i)
            }
        }
    }
}

//MMap:

declare function MMap_create()                                    : object
declare function MMap_insert(native: object, key: any, value: any): void
declare function MMap_erase (native: object, key: any)            : void
declare function MMap_clear (native: object)                      : void
declare function MMap_has   (native: object, key: any)            : boolean
declare function MMap_get   (native: object, key: any)            : any
declare function MMap_size  (native: object)                      : number
declare function MMap_begin (native: object)                      : void
declare function MMap_on    (native: object)                      : boolean
declare function MMap_next  (native: object)                      : void
declare function MMap_key   (native: object)                      : any
declare function MMap_value (native: object)                      : any

export namespace runtime {
    export class MMap<Key, Value> extends BaseObject {
        protected createNative(): object {
            return MMap_create()
        }
    
        public insert(key: Key, value: Value): void {
            MMap_insert(this.native, key, value)
        }

        public erase(key: Key): void {
            MMap_erase(this.native, key)
        }

        public clear(): void {
            MMap_clear(this.native)
        }

        public has(key: Key): boolean {
            return MMap_has(this.native, key)
        }

        public get(key: Key): Value {
            return MMap_get(this.native, key)
        }

        public get size(): number {
            return MMap_size(this.native)
        }
    
        public *[Symbol.iterator]() {
            let n = this.native
            for (MMap_begin(n); MMap_on(n); MMap_next(n)) {
                let k = MMap_key  (n)
                let v = MMap_value(n)
    
                yield [k, v]
            }
        }
    }
}

//MSet:

declare function MSet_create()                          : object
declare function MSet_insert(native: object, value: any): void
declare function MSet_erase (native: object, value: any): void
declare function MSet_clear (native: object)            : void
declare function MSet_has   (native: object, value: any): boolean
declare function MSet_size  (native: object)            : number
declare function MSet_begin (native: object)            : void
declare function MSet_on    (native: object)            : boolean
declare function MSet_next  (native: object)            : void
declare function MSet_value (native: object)            : any

export namespace runtime {
    export class MSet<Value> extends BaseObject {
        protected createNative(): object {
            return MSet_create()
        }
    
        public insert(value: Value): void {
            MSet_insert(this.native, value)
        }

        public erase(value: Value): void {
            MSet_erase(this.native, value)
        }

        public clear(): void {
            MSet_clear(this.native)
        }

        public has(value: Value): boolean {
            return MSet_has(this.native, value)
        }

        public get size(): number {
            return MSet_size(this.native)
        }
    
        public *[Symbol.iterator]() {
            let n = this.native
            for (MSet_begin(n); MSet_on(n); MSet_next(n)) {
                yield MSet_value(n)
            }
        }
    }
}

//BaseObject & Injectable:

declare function MSetObjectClassSymbol(native: object, sym: string): void
declare function MGetObjectClassSymbol(native: object): string
declare function MInjectClassFunction (clsSym: string, funcName: string, func: Function): void

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

        public constructor(args?: any) {
            if (_bindingNative) {
                this._native = _bindingNative
                _bindingNative = null
            } else {
                this._native = this.createNative(args)
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
        protected createNative(args: any): object {
            return null
        }
    }

    export function inject(clsSym: string, funcName: string, func: Function): void {
        MInjectClassFunction(clsSym, funcName, func)
    }

    export class Injectable extends BaseObject {
        protected createNative(args: any): object {
            let ntv = this.createInjectableNative(args)
            let sym = (<any> this.constructor).classSymbol
            if (ntv && sym) {
                MSetObjectClassSymbol(ntv, sym)
            }
            return ntv
        }

        //subclasses need to override this field.
        protected static classSymbol: string

        //subclasses need to override this method.
        protected createInjectableNative(args: any): object {
            return null
        }
    }
}

//MVector:

declare function MVector_create   (vType : string): object
declare function MVector_insert   (native: object, index: number, value: any): void
declare function MVector_erase    (native: object, index: number): void
declare function MVector_push_back(native: object, value: any   ): void
declare function MVector_pop_back (native: object): void
declare function MVector_clear    (native: object): void
declare function MVector_size     (native: object): number
declare function MVector_at       (native: object, index: number): any
declare function MVector_back     (native: object): any

export namespace runtime {
    export class MVector<T> extends BaseObject {
        public constructor(vType: string) {
            super({ vType: vType })
        }
    
        protected createNative(args: { vType: string }): object {
            return MVector_create(args.vType)
        }
    
        public insert   (i: number, v: T): void   { MVector_insert     (this.native, i, v) }
        public erase    (i: number)      : void   { MVector_erase      (this.native, i)    }
        public push_back(v: T)           : void   { MVector_push_back  (this.native, v)    }
        public pop_back ()               : void   { MVector_pop_back   (this.native)       }
        public clear    ()               : void   { MVector_clear      (this.native)       }
        public get size ()               : number { return MVector_size(this.native)       }
        public at       (i: number)      : T      { return MVector_at  (this.native, i)    }
        public get back ()               : T      { return MVector_back(this.native)       }
    
        public *[Symbol.iterator]() {
            for (let i = 0; i < this.size; ++i) {
                yield MVector_at(this.native, i)
            }
        }
    }
}

//MMap:

declare function MMap_create(kType : string, vType: string)       : object
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
    export class MMap<K, V> extends BaseObject {
        public constructor(kType: string, vType: string) {
            super({ kType: kType, vType: vType })
        }
    
        protected createNative(args: { kType: string, vType: string }): object {
            return MMap_create(args.kType, args.vType)
        }
    
        public insert  (k: K, v: K): void    { MMap_insert     (this.native, k, v) }
        public erase   (k: K)      : void    { MMap_erase      (this.native, k)    }
        public clear   ()          : void    { MMap_clear      (this.native)       }
        public has     (k: K)      : boolean { return MMap_has (this.native, k)    }
        public get     (k: K)      : V       { return MMap_get (this.native, k)    }
        public get size()          : number  { return MMap_size(this.native)       }
    
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

declare function MSet_create(vType : string)            : object
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
    export class MSet<T> extends BaseObject {
        public constructor(vType: string) {
            super({ vType: vType })
        }
    
        protected createNative(args: { vType: string }): object {
            return MSet_create(args.vType)
        }
    
        public insert  (v: T): void    { MSet_insert     (this.native, v) }
        public erase   (v: T): void    { MSet_erase      (this.native, v) }
        public clear   ()    : void    { MSet_clear      (this.native)    }
        public has     (v: T): boolean { return MSet_has (this.native, v) }
        public get size()    : number  { return MSet_size(this.native)    }
    
        public *[Symbol.iterator]() {
            let n = this.native
            for (MSet_begin(n); MSet_on(n); MSet_next(n)) {
                yield MSet_value(n)
            }
        }
    }
}

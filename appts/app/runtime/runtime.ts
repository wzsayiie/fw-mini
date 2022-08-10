//base object:

export namespace runtime {
    let _object_map     = new WeakMap<object, any>()
    let _binding_native = null

    export function get_object<T extends { new(): any }>(cls: T, native: object)
        : InstanceType<T>
    {
        //null.
        if (!native) {
            return null
        }

        //query native from cache.
        let obj = _object_map.get(native)
        if (obj) {
            return obj
        }   

        //new object.
        _binding_native = native
        obj = new cls()
        return obj
    }

    export function get_native(obj: base_object): object {
        return obj?.native
    }

    export class base_object {
        private _native: object

        public constructor(args?: any) {
            if (_binding_native) {
                this._native = _binding_native
                _binding_native = null
            } else {
                this._native = this.create_native(args)
            }

            if (!this._native) {
                throw `${this.constructor.name} hold no native object.`
            }
            _object_map.set(this._native, this)
        }

        public get native(): object {
            return this._native
        }

        protected create_native(args: any): object {
            return null
        }
    }
}

//injectable:

declare function MSetObjectClassSymbol(native: object, sym: string): void
declare function MInjectClassFunction (clsName: string, funcName: string, func: Function): void

export namespace runtime {
    export function inject(clsName: string, funcName: string, func: Function): void {
        MInjectClassFunction(clsName, funcName, func)
    }

    export class injectable extends base_object {
        protected create_native(args: any): object {
            let ntv = this.create_injectable_native(args)
            if (ntv) {
                MSetObjectClassSymbol(ntv, this.injected_class_symbol)
            }
            return ntv
        }

        protected create_injectable_native(args: any): object {
            return null
        }

        protected get injected_class_symbol(): string {
            return ''
        }
    }
}

//vector:

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
    export class vector<T> extends base_object {
        public constructor(vType: string) {
            super({ vType: vType })
        }
    
        protected create_native(args: { vType: string }): object {
            return MVector_create(args.vType)
        }
    
        public insert    (i: number, v: T): void   { MVector_insert     (this.native, i, v) }
        public delete    (i: number)      : void   { MVector_erase      (this.native, i)    }
        public push      (v: T)           : void   { MVector_push_back  (this.native, v)    }
        public pop       ()               : void   { MVector_pop_back   (this.native)       }
        public clear     ()               : void   { MVector_clear      (this.native)       }
        public get length()               : number { return MVector_size(this.native)       }
        public at        (i: number)      : T      { return MVector_at  (this.native, i)    }
        public get last  ()               : T      { return MVector_back(this.native)       }
    
        public *[Symbol.iterator]() {
            for (let i = 0; i < this.length; ++i) {
                yield MVector_at(this.native, i)
            }
        }
    }
}

//map:

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
    export class map<K, V> extends base_object {
        public constructor(kType: string, vType: string) {
            super({ kType: kType, vType: vType })
        }
    
        protected create_native(args: { kType: string, vType: string }): object {
            return MMap_create(args.kType, args.vType)
        }
    
        public set       (k: K, v: K): void    { MMap_insert     (this.native, k, v) }
        public delete    (k: K)      : void    { MMap_erase      (this.native, k)    }
        public clear     ()          : void    { MMap_clear      (this.native)       }
        public has       (k: K)      : boolean { return MMap_has (this.native, k)    }
        public get       (k: K)      : V       { return MMap_get (this.native, k)    }
        public get length()          : number  { return MMap_size(this.native)       }
    
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

//set:

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
    export class set<T> extends base_object {
        public constructor(vType: string) {
            super({ vType: vType })
        }
    
        protected create_native(args: { vType: string }): object {
            return MSet_create(args.vType)
        }
    
        public add       (v: T): void    { MSet_insert     (this.native, v) }
        public delete    (v: T): void    { MSet_erase      (this.native, v) }
        public clear     ()    : void    { MSet_clear      (this.native)    }
        public has       (v: T): boolean { return MSet_has (this.native, v) }
        public get length()    : number  { return MSet_size(this.native)    }
    
        public *[Symbol.iterator]() {
            let n = this.native
            for (MSet_begin(n); MSet_on(n); MSet_next(n)) {
                yield MSet_value(n)
            }
        }
    }
}

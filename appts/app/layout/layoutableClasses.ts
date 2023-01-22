import { NButton         } from './NButton'
import { NControl        } from './NControl'
import { NHorizonBox     } from './NHorizonBox'
import { NImageView      } from './NImageView'
import { NLabel          } from './NLabel'
import { NScrollView     } from './NScrollView'
import { NStackBox       } from './NStackBox'
import { NTextField      } from './NTextField'
import { NVerticalBox    } from './NVerticalBox'
import { NView           } from './NView'
import { NViewController } from './NViewController'

import {
    CButton,
    CControl,
    CImageView,
    CLabel,
    CNavigationController,
    CScrollView,
    CTabPageController,
    CTextField,
    CView,
    CViewController,
} from '../host/native'

import {
    NBase,
    NEntity,
} from './Nodes'

function isDerived(derived: { new(): object }, base: { new(): object }): boolean {
    return derived == base || derived.prototype instanceof base;
}

export class ClassInfo {
    nodeClass  : { new(): NBase  }
    entityClass: { new(): object }
}

let _classes = new Map<string, ClassInfo>()

export const layoutableClasses = {
    register: function(
        type: string, entity: { new(): object }, node?: { new(): NBase }): void
    {
        if (!entity && !node) {
            return
        }

        //speculate the node type.
        if (!node) {
            if /**/ (isDerived(entity, CNavigationController)) { node = NViewController }
            else if (isDerived(entity, CTabPageController   )) { node = NViewController }
            else if (isDerived(entity, CViewController      )) { node = NViewController }

            else if (isDerived(entity, CButton    )) { node = NButton     }
            else if (isDerived(entity, CTextField )) { node = NTextField  }
            else if (isDerived(entity, CControl   )) { node = NControl    }
            else if (isDerived(entity, CImageView )) { node = NImageView  }
            else if (isDerived(entity, CLabel     )) { node = NLabel      }
            else if (isDerived(entity, CScrollView)) { node = NScrollView }
            else if (isDerived(entity, CView      )) { node = NView       }
        }

        if (node) {
            let info: ClassInfo = {
                entityClass: entity,
                nodeClass  : node  ,
            }
            _classes.set(type, info)
        }
    },

    find: function (type: string): ClassInfo {
        return _classes.get(type)
    },
}

layoutableClasses.register('horizon' , null, NHorizonBox )
layoutableClasses.register('vertical', null, NVerticalBox)
layoutableClasses.register('stack'   , null, NStackBox   )
layoutableClasses.register('stretch' , null, NEntity     )

layoutableClasses.register('cViewController'      , CViewController      )
layoutableClasses.register('cNavigationController', CNavigationController)
layoutableClasses.register('cTabPageController'   , CTabPageController   )

layoutableClasses.register('cView'      , CView      )
layoutableClasses.register('cImageView' , CImageView )
layoutableClasses.register('cLabel'     , CLabel     )
layoutableClasses.register('cScrollView', CScrollView)
layoutableClasses.register('cControl'   , CControl   )
layoutableClasses.register('cButton'    , CButton    )
layoutableClasses.register('cTextField' , CTextField )

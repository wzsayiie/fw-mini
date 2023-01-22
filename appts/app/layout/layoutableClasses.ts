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

let _nodeClasses   = new Map<string, { new(): NBase  }>()
let _entityClasses = new Map<string, { new(): object }>()

export const layoutableClasses = {
    register: function(
        type: string, nodeClass: { new(): NBase }, entityClass: { new(): object }): void
    {
        _nodeClasses.set(type, nodeClass)

        if (entityClass) {
            _entityClasses.set(type, entityClass)
        }
    },

    nodeClassOf: function (type: string): { new(): NBase } {
        return _nodeClasses.get(type)
    },

    entityClassOf: function (type: string): { new(): object } {
        return _entityClasses.get(type)
    },
}

layoutableClasses.register('horizon' , NHorizonBox , null)
layoutableClasses.register('vertical', NVerticalBox, null)
layoutableClasses.register('stack'   , NStackBox   , null)
layoutableClasses.register('stretch' , NEntity     , null)

layoutableClasses.register('cViewController'      , NViewController, CViewController      )
layoutableClasses.register('cNavigationController', NViewController, CNavigationController)
layoutableClasses.register('cTabPageController'   , NViewController, CTabPageController   )

layoutableClasses.register('cView'      , NView      , CView      )
layoutableClasses.register('cImageView' , NImageView , CImageView )
layoutableClasses.register('cLabel'     , NLabel     , CLabel     )
layoutableClasses.register('cScrollView', NScrollView, CScrollView)
layoutableClasses.register('cControl'   , NControl   , CControl   )
layoutableClasses.register('cButton'    , NButton    , CButton    )
layoutableClasses.register('cTextField' , NTextField , CTextField )

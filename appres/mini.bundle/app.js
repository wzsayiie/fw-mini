define(function () {
    const Camera  = require('./gamekit/Camera' )
    const Sprite  = require('./gamekit/Sprite' )
    const context = require('./gamekit/context')
    const util    = require('./minikit/util'   )

    function Main() {
        Camera.mainCamera.moveTo(0, 0)
    }

    Main()
})

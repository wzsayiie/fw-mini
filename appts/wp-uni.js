const path = require('path')

module.exports = {
    mode: 'development',

    output: {
        filename: 'bundle.js',
        path: path.resolve(__dirname, '../appres/mini.bundle'),
    },

    module: {
        rules: [
            {
                loader : 'ts-loader',
                test   : /\.tsx?$/,
                options: {
                    configFile: '',
                },
            },
        ],
    },
    resolve: {
        extensions: ['.ts', '.tsx' ],
    },

    externals: {
        //node.js builtin modules:
        fs   : 'commonjs fs'   ,
        http : 'commonjs http' ,
        https: 'commonjs https',
        net  : 'commonjs net'  ,
        os   : 'commonjs os'   ,
        path : 'commonjs path' ,
        url  : 'commonjs url'  ,
        util : 'commonjs util' ,
    },
}

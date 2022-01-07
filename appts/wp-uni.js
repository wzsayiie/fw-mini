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
                test  : /\.ts$/,
                loader: 'ts-loader',
            },
        ],
    },
    resolve: {
        extensions: ['.ts', '.js'],
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

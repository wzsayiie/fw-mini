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
}

const config = require('./wp-uni')

config.entry = './app/main.ts'
config.module.rules[0].options.configFile = 'tsconfig.json'

module.exports = config

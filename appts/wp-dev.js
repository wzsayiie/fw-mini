const config = require('./wp-uni')

config.entry = './dev/main.ts'
config.module.rules[0].options.configFile = 'tsdev.json'

module.exports = config

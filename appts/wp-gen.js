const config = require('./wp-uni')

config.entry = './gen/main.ts'
config.module.rules[0].options.configFile = 'tsgen.json'

module.exports = config

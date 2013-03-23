var sysinfo     = require('bindings')('sysinfo');
var provider    = require('./providers')(sysinfo);

module.exports = provider;
module.exports.sysinfo = sysinfo;

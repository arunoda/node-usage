var providers = {
  'linux': ['linux', 'ps'],
  'sunos': ['sunos', 'ps'],
  'darwin': ['ps'],
  'freebsd': ['ps']
  'win': ['other']
  'other': ['other']
};

module.exports = function(sysinfo) {
  var platform = (process.platform == "solaris")? "sunos": process.platform;

}

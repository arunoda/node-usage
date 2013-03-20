module.exports = function(sysinfo) {

    var providers = {
        'linux': require('./linux')(sysinfo),
        'mac': require('./mac')(sysinfo),
        'win': require('./win')(sysinfo),
        'other': require('./other')(sysinfo)
    };

    var provider = providers[sysinfo.OS];
    if(provider) {
        return provider;
    } else {
        return providers['other'];
    }
}
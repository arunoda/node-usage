var fs = require('fs');

module.exports = function parseProcStat(procVersion) {

    return selectProcStatProvider(procVersion);
};

function selectProcStatProvider(procVersion) {
    
    var providers = [
        require('./linux')
    ];

    for(var lc=0; lc<providers.length; lc++) {
        var result = providers[lc](procVersion);
        if(result) {
            return result;
        }
    }
}
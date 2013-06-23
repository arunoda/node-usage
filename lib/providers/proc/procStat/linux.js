var fs = require('fs');

var STAT_INDEXES = {
    STIME: 11,
    UTIME: 12,
    START_TIME: 19,
    RSS: 21
};

function getLinuxProcStat(pid, callback) {
    
    var fileName = '/proc/' + pid + '/stat';
    fs.readFile(fileName, 'utf8', function(err, data) {

        if(err) {
            callback(err);
        } else {
            data = data.substr(data.lastIndexOf(')') + 2);
            var parts = data.split(' ');
            var statObject = {
                stime: parseFloat(parts[STAT_INDEXES.STIME]),
                utime: parseFloat(parts[STAT_INDEXES.UTIME]),
                startTime: parseFloat(parts[STAT_INDEXES.START_TIME]),
                rss: parseFloat(parts[STAT_INDEXES.RSS])
            };
            callback(null, statObject);
        }
    });
}

module.exports = function (procVersion) {

    if(procVersion.match(/linux/i)) {
        return getLinuxProcStat;
    } else {
        null;
    }
};

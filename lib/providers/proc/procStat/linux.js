var fs = require('fs');

var STAT_INDEXES = {
    STIME: 13,
    UTIME: 14,
    START_TIME: 21,
    RSS: 23
};

function getLinuxProcStat(pid, callback) {
    
    var fileName = '/proc/' + pid + '/stat';
    fs.readFile(fileName, 'utf8', function(err, data) {

        if(err) {
            callback(err);
        } else {
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
var fs = require('fs');

module.exports = function linuxProvider(sysinfo) {

    return new LinuxProvider(sysinfo);
};

function LinuxProvider(sysinfo) {

    var procVersion = fs.readFileSync('/proc/version', 'utf8');
    var historyCpuUsage = {};

    var getProcStat = require('./procStat')(procVersion);

    this.clearHistory = function clearHistory (pid) {
        
        if(pid) {
            if(historyCpuUsage[pid]) {
                historyCpuUsage[pid] = null;
            }
        } else {
            historyCpuUsage = {};
        }
    };

    this.lookup = function linuxLookup(pid, options, callback) {

        if(typeof options == 'function') {
            callback = options;
            options = {};
        }
        options = options || {};

        if(!getProcStat) {
            return callback(new Error('Not Supported platform. Please submit an issue: https://github.com/arunoda/node-usage/issues'));
        }

        var uptime;
        getUptime(function(err, value) {

            if(err) {
                callback(err);
            } else {
                uptime = value;
                getProcStat(pid, calculateUsage);
            }
        });

        function calculateUsage(err, stat) {

            if(err) {
                callback(err);
            } else {
                var usageData = {
                    memory: calculateMemoryUsage(sysinfo, stat)
                };

                if(historyCpuUsage[pid] && options.keepHistory) {
                    usageData['cpu'] = calculateCpuUsageFromHistory(sysinfo, uptime, stat, historyCpuUsage[pid]);
                } else {
                    usageData['cpu'] = calculateCpuUsage(sysinfo, uptime, stat);
                }

                if(options.keepHistory) {
                    //save totalTime in history
                    historyCpuUsage[pid] = {
                        timestamp: Date.now(),
                        stat: stat,
                        uptime: uptime
                    };
                }

                callback(null, usageData);
            }
        }
    };
}

function calculateCpuUsage(sysinfo, uptime, stat) {

    var totalTime = (stat.stime + stat.utime) / sysinfo.HERTZ;
    var processUptime = uptime - stat.startTime / sysinfo.HERTZ;
    var pcpu = (totalTime / processUptime) * 100;
    return pcpu;
}

function calculateCpuUsageFromHistory(sysinfo, uptime, stat, lastUsage) {

    var totalTime = (stat.stime + stat.utime) / sysinfo.HERTZ;
    var lastTotalTime = (lastUsage.stat.stime + lastUsage.stat.utime) / sysinfo.HERTZ;

    var usedTimeSinceLast = totalTime - lastTotalTime;
    var timeSpent = uptime - lastUsage.uptime;

    var pcpu = (usedTimeSinceLast / timeSpent) * 100;
    return pcpu;
}

function calculateMemoryUsage(sysinfo, stat) {

    return stat.rss * sysinfo.PAGE_SIZE;
}

function getUptime(callback) {

    fs.readFile('/proc/uptime', 'utf8', function(err, data) {

        if(err) {
            callback(err);
        } else {
            var matched = data.match(/^(.*) /);
            if(matched) {
                var uptime = parseFloat(matched[1]);
                callback(null, uptime);
            } else {
                callback(new Error('Invalid formatted uptime file'));
            }
        }
    });
}
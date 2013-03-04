var sysinfo = require('bindings')('sysinfo');
var fs		= require('fs');

var UPTIME_FILE = '/proc/uptime';
var STAT_FILE 	= '/proc/$pid/stat';
var STAT_INDEXES = {
	STIME: 13,
	UTIME: 14,
	START_TIME: 21,
	RSS: 23
};

var historyCpuUsage = {};

function usageLookup(pid, options, callback) {

	if(typeof options == 'function') {
		callback = options;
		options = {};
	}
	options = options || {};

	if(!sysinfo.IS_SUPPORTED) {
		return callback(new Error('NOT_SUPPORTED_ON_THIS_PLATFORM'));
	}

	var uptime;
	getUptime(UPTIME_FILE, function(err, value) {

		if(err) {
			callback(err);
		} else {
			uptime = value;
			getStat(STAT_FILE, pid, calculateUsage);
		}
	});

	function calculateUsage(err, stat) {

		if(err) {
			callback(err);
		} else {
			var usageData = {
				memory: calculateMemoryUsage(stat)
			};

			if(historyCpuUsage[pid] && options.keepHistory) {
				usageData['cpu'] = calculateCpuUsageFromHistory(uptime, stat, historyCpuUsage[pid]);
			} else {
				usageData['cpu'] = calculateCpuUsage(uptime, stat);
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
}

function calculateCpuUsage(uptime, stat) {

	var totalTime = (stat.stime + stat.utime) / sysinfo.HERTZ;
	var processUptime = uptime - stat.startTime / sysinfo.HERTZ;
	var pcpu = (totalTime / processUptime) * 100;
	return pcpu;
}

function calculateCpuUsageFromHistory(uptime, stat, lastUsage) {

	var totalTime = (stat.stime + stat.utime) / sysinfo.HERTZ;
	var lastTotalTime = (lastUsage.stat.stime + lastUsage.stat.utime) / sysinfo.HERTZ;

	var usedTimeSinceLast = totalTime - lastTotalTime;
	var timeSpent = uptime - lastUsage.uptime;

	var pcpu = (usedTimeSinceLast / timeSpent) * 100;
	return pcpu;
}

function calculateMemoryUsage(stat) {

	return stat.rss * sysinfo.PAGE_SIZE;
}

function getUptime(file, callback) {

	fs.readFile(file, 'utf8', function(err, data) {

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

function getStat(file, pid, callback) {

	file = file.replace('$pid', pid);
	fs.readFile(file, 'utf8', function(err, data) {

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

function setKeepHistory (value) {
	
	keepHistory = value;
}

function clearHistory (pid) {
	
	if(pid) {
		historyCpuUsage[pid] = null;
	} else {
		historyCpuUsage = {};
	}
}

exports.lookup = usageLookup;
exports.setKeepHistory = setKeepHistory;
exports.clearHistory = clearHistory;
exports._getUptime= getUptime;
exports._getStat = getStat;
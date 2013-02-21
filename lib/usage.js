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

function usageLookup(pid, callback) {

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
				cpu: calculateCpuUsage(uptime, stat),
				memory: calculateMemoryUsage(stat)
			};

			callback(null, usageData);
		}
	}
}

function calculateCpuUsage(uptime, stat) {

  // unsigned long long total_time;   /* jiffies used by this process */
  // unsigned pcpu = 0;               /* scaled %cpu, 999 means 99.9% */
  // unsigned long long seconds;      /* seconds of process life */
  // unsigned int seconds_since_boot = uptime(0, 0);

  // total_time = pp->utime + pp->stime;
  // seconds = seconds_since_boot - pp->start_time / Hertz;
  // if(seconds) pcpu = (total_time * 1000ULL / Hertz) / seconds;
  // return (float) pcpu/10U;

	var totalTime = stat.stime + stat.utime;
	var seconds = uptime - stat.startTime / sysinfo.HERTZ;
	var pcpu = (totalTime * 1000 / sysinfo.HERTZ ) / seconds;
	return pcpu / 10;
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

exports.lookup = usageLookup;
exports._getUptime= getUptime;
exports._getStat = getStat;
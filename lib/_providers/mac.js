var exec = require('child_process').exec;

module.exports = function(sysinfo) {

    return new MacProvider(sysinfo);
};

function MacProvider(sysinfo) {

    this.lookup = function(pid, options, callback) {

        if(typeof options == 'function') {
            callback = options;
            options = {};
        }
        options = options || {};

        exec('ps -vp ' + pid, function(err, stdout, stderr) {
          if (err || stderr) return callback(err || stderr);

          var usage = parsePS(pid, stdout);
          if (!usage)
            return callback('INVALID_PID');

          callback(null, usage);
        });
    };
}

function parsePS(pid, output) {
  var lines = output.trim().split('\n');
  if (lines.length !== 2)
    return null;

  var labelsMap = {};
  var labels = lines[0].trim().split(/[ \t]+/g);
  for (var i = 0; i < labels.length; i++)
    labelsMap[labels[i]] = i;

  var values = lines[1].trim().split(/[ \t]+/g);

  var foundPID = parseInt(values[labelsMap['PID']], 10);
  var rss = 1024 * parseInt(values[labelsMap['RSS']], 10);
  var cpu = parseFloat(values[labelsMap['%CPU']]);

  if (pid != foundPID || isNaN(rss) || isNaN(cpu))
    return null;

  return { memory: rss, cpu: cpu };
}

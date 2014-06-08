module.exports = function(sysinfo) {

    return new OtherProvider(sysinfo);
};

function OtherProvider(sysinfo) {

    this.lookup = function(pid, options, callback) {

        if(typeof options == 'function') {
            callback = options;
            options = {};
        }
        options = options || {};

        callback(new Error('unidentified OS. please submit an issue with your OS and version: https://github.com/arunoda/node-usage/issues'));
    };
}
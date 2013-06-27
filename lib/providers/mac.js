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

        sysinfo.getUsage(pid, callback);
    };
}

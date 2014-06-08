module.exports = function(sysinfo) {

    return new WindowsProvider(sysinfo);
};

function WindowsProvider(sysinfo) {

    this.lookup = function(pid, options, callback) {

        if(typeof options == 'function') {
            callback = options;
            options = {};
        }
        options = options || {};

        callback(new Error('node-usage windows provider does not implemented yet! patches are welcome.'));
    };
}
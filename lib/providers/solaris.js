module.exports = function(sysinfo) {

    return new SolarisProvider(sysinfo);
};

function SolarisProvider(sysinfo) {

    this.lookup = function(pid, options, callback) {

        if(typeof options == 'function') {
            callback = options;
            options = {};
        }
        options = options || {};

        callback(new Error('node-usage solaris provider does not implemented yet! patches are welcome.'));
    };
}
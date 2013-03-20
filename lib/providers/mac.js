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

        callback(new Error('node-usage mac provider does not implemented yet! patches are welcome.'));
    };
}
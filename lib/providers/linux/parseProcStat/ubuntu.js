var STAT_INDEXES = {
    STIME: 13,
    UTIME: 14,
    START_TIME: 21,
    RSS: 23
};

module.exports = function (procVersion) {

    return (procVersion.match(/ubuntu/i))? STAT_INDEXES: false;
};
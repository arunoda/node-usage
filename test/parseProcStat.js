var assert              = require('assert');
var parseProcStatModule = require('../lib/providers/linux/parseProcStat');

suite('parseProcStat', function() {

    test('ubuntu', function(done) {

        var procVersion = 'Linux version 3.0.0-12-generic (buildd@crested) (gcc version 4.6.1 (Ubuntu/Linaro 4.6.1-9ubuntu3) ) #20-Ubuntu SMP Fri Oct 7 14:56:25 UTC 2011';
        var procStatContent = '23418 (bash) S 23417 23418 23418 34818 26786 4202496 14043 1170269 5 307 23 22 1903 621 20 0 1 0 41294608 35561472 2449 18446744073709551615 4194304 5105884 140735939350560 140735939349136 140029720592670 0 65536 3686404 1266761467 18446744071579247796 0 0 17 0 0 0 60 0 0';
        var parseProcStat = parseProcStatModule(procVersion);

        parseProcStat(procStatContent, function(err, stat) {

            assert.ifError(err);
            assert.deepEqual(stat, {
                stime: 23,
                utime: 22,
                startTime: 41294608,
                rss: 2449 
            });
            done();
        });
    });

    test('heroku', function(done) {

        var procVersion = 'Linux version 2.6.32-350-ec2 (buildd@batsu) (gcc version 4.4.3 (Ubuntu 4.4.3-4ubuntu5.1) ) #57-Ubuntu SMP Thu Nov 15 15:59:03 UTC 2012';
        var procStatContent = '23418 (bash) S 23417 23418 23418 34818 26786 4202496 14043 1170269 5 307 23 22 1903 621 20 0 1 0 41294608 35561472 2449 18446744073709551615 4194304 5105884 140735939350560 140735939349136 140029720592670 0 65536 3686404 1266761467 18446744071579247796 0 0 17 0 0 0 60 0 0';
        var parseProcStat = parseProcStatModule(procVersion);

        parseProcStat(procStatContent, function(err, stat) {

            assert.ifError(err);
            assert.deepEqual(stat, {
                stime: 23,
                utime: 22,
                startTime: 41294608,
                rss: 2449 
            });
            done();
        });
    });
});
var usage 	= require('../');
var assert 	= require('assert');

suite('Usage', function() {

	test('invalid pid', function(done) {
		
		usage.lookup(1232323, _usageCallback(done, function(err) {

			assert.ok(err.code, 'INVALID_PID');
			done();
		}));
	});

	test('valid pid', function(done) {

		usage.lookup(process.pid, _usageCallback(done, function(err, result) {

			assert.equal(err, null);
			assert.ok(result.cpu >= 0);
			assert.ok(result.memory > 0);
			done();
		}));
	});
});


function _usageCallback(done, calback) {

	return function(err, result) {

		if(err && err.code == 'PLATFORM_NOT_SUPPORTED') {
			console.log(err.code);
			done();
		} else {
			calback(err, result);
		}
	};
}
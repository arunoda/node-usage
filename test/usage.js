var usage 	= require('../');
var assert 	= require('assert');

suite('Usage', function() {

	test('invalid pid', function(done) {
		
		usage.lookup(1232323, function(err) {

			assert.ok(err);
			done();
		});
	});

	test('valid pid', function(done) {

		usage.lookup(process.pid, function(err, result) {

			assert.equal(err, null);
			assert.ok(result.cpu >= 0);
			assert.ok(result.memory > 0);
			done();
		});
	});
});
var binding = require('bindings')('node_usage');

exports.lookup = function(pid, callback) {

	binding.lookup(parseInt(pid), function(err, cpu, memory) {

		if(err) {
			callback(err);
		} else {
			callback(null, {
				cpu: cpu, 
				memory: memory
			});
		}
	});
};

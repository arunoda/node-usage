var usage = require('../');
usage.setKeepHistory(true);

setInterval(function() {

	usage.lookup(parseInt(process.argv[2]), function(err, stat) {

		console.log(err, stat);
	});
}, 2000);
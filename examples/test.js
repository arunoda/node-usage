var usage = require('../');

setInterval(function() {

	var options =  { keepHistory: true };
	usage.lookup(parseInt(process.argv[2]), options, function(err, stat) {

		console.log(err, stat);
	});
}, 2000);
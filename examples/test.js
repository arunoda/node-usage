var usage = require('./');

checkUsage();
setInterval(checkUsage, 1000);

function checkUsage() {

	usage.lookup(process.pid, prt);
}

function prt () {
	console.log(arguments);
}
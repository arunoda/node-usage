# node-usage 

### linux process usage lookup with nodejs

* With `node-usage` we can lookup cpu and memory usage of any accessible process on the system.
* This is based on a cut down version of [procps](http://procps.sourceforge.net/) which compiles fast.

## Example

### Code
~~~js
var usage = require('usage');

var pid = process.pid // you can use any valid PID instead
usage.lookup(pid, function(err, result) {
	
});
~~~

### Result Object
~~~js
{ 
	cpu: 10.6, //in percentage
	memory: 100065280 //in no of bytes
}
~~~

## Motivation

* Some NodeJS PAAS providers do not expose us an interface to monitor CPU and Memory usage of our apps.
* They asks us to go for somewhere else for that which I don't want to go (or simply I cannot go)
* So `node-usage` was born to tackle on this.

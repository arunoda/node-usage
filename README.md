[![Build Status](https://travis-ci.org/arunoda/node-usage.png?branch=master)](https://travis-ci.org/arunoda/node-usage)
# node-usage 

### linux process usage lookup with nodejs

* Simple interface to lookup cpu and memory usage of any accessible process on the system.
* Written in 98% JavaScript and logic is based on [procps](http://procps.sourceforge.net/)

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

## Average CPU usage vs Current CPU usage

By default CPU Percentage provided is an average from the starting time of the process. It does not correctly reflect the current CPU usage. (this is also a problem with linux `ps` utility)

But If you call `usage.lookup()` continuously for a given pid, you can turn on **keepHistory** flag and you'll get the CPU usage since last time you track the usage. This reflects the current CPU usage. 

see following example to enable keepHistory flag

~~~js
var pid = process.pid;
var options = { keepHistory: true }
usage.lookup(pid, options, function(err, result) {
	
});
~~~

you can clear history cache too
~~~js
usage.clearHistory(pid); //clear history for the given pid
usage.clearHistory(); //clean history for all pids
~~~

## Motivation

* Some of the NodeJS PAAS providers do not expose an interface to monitor CPU and Memory usage.
* They asks us to go for somewhere else for that which I don't want to go (or simply I cannot go)
* So `node-usage` was born to tackle on this.

#include <node.h>
#ifndef USAGE_NOT_SUPPORTED
#include <usage.h>
#endif

using namespace v8;

struct usage_result {
	int pid;
	Persistent<Function> callback;
	
	float cpu;
	float memory;
	bool invalid_pid;
};
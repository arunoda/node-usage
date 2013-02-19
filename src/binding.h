#include <node.h>
#include <usage.h>

using namespace v8;

struct usage_result {
	int pid;
	Persistent<Function> callback;
	
	float cpu;
	float memory;
	bool invalid_pid;
};
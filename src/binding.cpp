#include "binding.h"

using namespace v8;

void process_usage_async(uv_work_t* req) {
	usage_result* handler = static_cast<usage_result*>(req->data);

	usage_t* usage_info = usage_init(handler->pid);

	if(usage_info) {
		handler->cpu = usage_cpu(usage_info);
		handler->memory = usage_totalmem(usage_info);
	} else {
		handler->invalid_pid = true;
	}
}

void process_usage_after(uv_work_t* req) {
	HandleScope scope;

	usage_result* handler = static_cast<usage_result*>(req->data);

	if(handler->invalid_pid) {
		const unsigned argc = 1;
		Local<Value> argv[argc] = { 
			Exception::Error(String::New("Invalid PID"))
		};

		handler->callback->Call(Context::GetCurrent()->Global(), argc, argv);
	} else {
		const unsigned argc = 3;
		Local<Value> argv[argc] = { 
			Local<Value>::New(Null()), 
			Local<Value>::New(Number::New(handler->cpu)),
			Local<Value>::New(Number::New(handler->memory)) 
		};

		handler->callback->Call(Context::GetCurrent()->Global(), argc, argv);
	}

	handler->callback.Dispose();
	delete handler;
	delete req;
}

Handle<Value> node_process_usage(const Arguments& args) {
	HandleScope scope;

	if(!args[0]->IsNumber()) {
		return ThrowException(Exception::TypeError(
			String::New("First argument must be a valid pid")));
	}

	if(!args[1]->IsFunction()) {
		return ThrowException(Exception::TypeError(
			String::New("Second argument must be a callback function")));
	}

	Local<Function> callback = Local<Function>::Cast(args[1]);

	usage_result* handler = new usage_result();
	handler->pid = args[0]->ToInteger()->Value();
	handler->callback = Persistent<Function>::New(callback);

	uv_work_t* req = new uv_work_t();
	req->data = handler;

	uv_queue_work(uv_default_loop(), req, 
		process_usage_async, (uv_after_work_cb)process_usage_after);

	return scope.Close(Undefined());	
}

void RegisterModule(Handle<Object> target) {

	target->Set(String::NewSymbol("process_usage"),
		FunctionTemplate::New(node_process_usage)->GetFunction());
}

NODE_MODULE(node_usage, RegisterModule);


/*
const unsigned argc = 3;
		Local<Value> argv[argc] = { 
			Local<Value>::New(Null()), 
			Local<Value>::New(Number::New(cpu)),
			Local<Value>::New(Number::New(mem)) 
		};

		callback->Call(Context::GetCurrent()->Global(), argc, argv);
*/
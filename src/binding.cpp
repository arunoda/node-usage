#include "binding.h"

using namespace v8;

void RegisterModule(Handle<Object> target) {

#ifdef PROC_NOT_SUPPORTED
	target->Set(String::NewSymbol("IS_SUPPORTED"), Boolean::New(false));
#else 
	target->Set(String::NewSymbol("IS_SUPPORTED"), Boolean::New(true));
	target->Set(String::NewSymbol("HERTZ"), Number::New(Hertz));
	target->Set(String::NewSymbol("PAGE_SIZE"), Number::New(sysconf(_SC_PAGESIZE)));
#endif
}

NODE_MODULE(sysinfo, RegisterModule);
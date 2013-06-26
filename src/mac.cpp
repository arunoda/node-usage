#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/task.h>

#include "mac.h"

#define USAGE_SUCCESS 0
#define USAGE_MEM_FAILURE -1
#define USAGE_CPU_FAILURE -2

int get_usage(int pid, ps_usage_t* ps_usage) {

    // Use task_info() to fetch memory usage info
    task_t task = MACH_PORT_NULL;
    struct task_basic_info t_info;
    mach_msg_type_number_t count = TASK_BASIC_INFO_64_COUNT;

    if (KERN_SUCCESS != task_for_pid(mach_task_self(), (pid_t)pid, &task)) {
        return USAGE_MEM_FAILURE;
    }

    if (KERN_SUCCESS != task_info(task, TASK_BASIC_INFO_64, (task_info_t)&t_info, &count)) {
        mach_port_deallocate(mach_task_self(), task);
        return USAGE_MEM_FAILURE;
    }

    // Fetch process thread information with task_threads()
    thread_array_t th_array;
    mach_msg_type_number_t th_count;
    struct thread_basic_info th_info;
    mach_msg_type_number_t th_info_count;
    double cpu_usage_pct = 0.0;

    if (KERN_SUCCESS != task_threads(task, &th_array, &th_count)) {
        mach_port_deallocate(mach_task_self(), task);
        return USAGE_CPU_FAILURE;
    }

    // Loop over each thread info and add the incremental CPU% usage
    for (uint i = 0; i < th_count; i++) {
        th_info_count = THREAD_BASIC_INFO_COUNT;
        if ((KERN_SUCCESS != thread_info(th_array[i], THREAD_BASIC_INFO,
            (thread_info_t)&th_info, &th_info_count)))
        {
            continue;
        }

        cpu_usage_pct += (double)th_info.cpu_usage / TH_USAGE_SCALE;
        mach_port_deallocate(mach_task_self(), th_array[i]);
    }

    mach_port_deallocate(mach_task_self(), task);
    vm_deallocate(mach_task_self(), (vm_address_t)th_array, sizeof(thread_t) * th_count);

    ps_usage->cpu = cpu_usage_pct * 100.0;
    ps_usage->memory = t_info.resident_size;
    return USAGE_SUCCESS;
}

/*
    V8 Integration 
*/

void AsyncGetUsage(uv_work_t* req) {
    UsageData* usageData = static_cast<UsageData*>(req->data);
    usageData->res = get_usage(usageData->pid, &usageData->ps_usage);
}

void AsyncAfterGetUsage(uv_work_t* req) {

    UsageData* usageData = static_cast<UsageData*>(req->data);

    if (usageData->res == USAGE_MEM_FAILURE) {
        const unsigned argc = 1;
        Local<Value> argv[argc] = {
            Exception::Error(String::New("ACCESS_DENIED"))
        };
        usageData->callback->Call(Context::GetCurrent()->Global(), argc, argv);
    } else if (usageData->res == USAGE_CPU_FAILURE) {
        const unsigned argc = 1;
        Local<Value> argv[argc] = {
            Exception::Error(String::New("INVALID_PID"))
        };
        usageData->callback->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        Local<Object> usage = Object::New();
        usage->Set(String::NewSymbol("cpu"), Number::New(usageData->ps_usage.cpu));
        usage->Set(String::NewSymbol("memory"), Number::New(usageData->ps_usage.memory));

        const unsigned argc = 2;
        Local<Value> argv[argc] = { Local<Value>::New(Null()), usage };
        usageData->callback->Call(Context::GetCurrent()->Global(), argc, argv);
    }

    delete usageData;
    delete req;
}

Handle<Value> GetUsage(const Arguments& args) {
    
    HandleScope scope;

    int pid = (int)(Local<Number>::Cast(args[0])->Value());
    Local<Function> callback = Local<Function>::Cast(args[1]);

    UsageData* usageData = new UsageData();
    usageData->pid = pid;
    usageData->callback = Persistent<Function>::New(callback);

    uv_work_t* req = new uv_work_t();
    req->data = usageData;

    uv_queue_work(uv_default_loop(), req, AsyncGetUsage, (uv_after_work_cb)AsyncAfterGetUsage);
    
    return scope.Close(Undefined());
}

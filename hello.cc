/*
 * Copyright 2015, Tim Rochford
 */
#include <nan.h>
#include <v8.h>

using namespace v8;

class ProcessWorker : public NanAsyncWorker
{
  public:
    ProcessWorker(NanCallback *callback,
                  Handle<Object>& obj)
        : NanAsyncWorker(callback)
    {
        NanAssignPersistent(_respObj, obj);
    }

    ~ProcessWorker()
    {
        NanDisposePersistent(_respObj);
    }

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute()
    {
        // Do real work here...
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback()
    {
        NanScope();

        Handle<Object> jsonObj = NanNew<Object>();
        jsonObj->Set(NanNew<String>("widgets"),
                     NanNew<String>("button"));
        Handle<Value> argv[] = { NanNew(_respObj), jsonObj };
        callback->Call(2, argv);
    }

private:
    Persistent<Object> _respObj;
};

NAN_METHOD(Process)
{
   NanScope();
   NanCallback *callback = new NanCallback(args[0].As<Function>());
   Handle<Object> resp = Local<Object>::Cast(args[1]);
   NanAsyncQueueWorker(new ProcessWorker(callback, resp));
   NanReturnUndefined();
}

void InitAll(Handle<Object> exports)
{
    exports->Set(NanNew<String>("process"),
                 NanNew<FunctionTemplate>(Process)->GetFunction());
}

NODE_MODULE(hello, InitAll);

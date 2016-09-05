/*
 * Copyright 2015-2016, Tim Rochford
 */
#include <nan.h>

using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using v8::Value;
using v8::Number;

using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::GetFunction;
using Nan::New;
using Nan::Null;
using Nan::Set;

using Nan::Persistent;

class ProcessWorker : public AsyncWorker
{
  public:
    ProcessWorker(Callback *callback,
                  Handle<Object>& obj)
        : AsyncWorker(callback)
    {
        _optionsObject.Reset(obj);
    }

    ~ProcessWorker()
    {
        _optionsObject.Reset();
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
        using namespace v8;
        Nan::HandleScope scope;

        Local<Object> returnedObject = Nan::New<Object>();

        returnedObject->Set(Nan::New("age").ToLocalChecked(), New<Number>(42));
        returnedObject->Set(Nan::New("name").ToLocalChecked(), New<String>("Bob").ToLocalChecked());

        v8::Local<v8::Object> object = Nan::New(_optionsObject);
        Local<Value> argv[] = { object, returnedObject};
        callback->Call(2, argv);
    }
private:
    Persistent<Object> _optionsObject;
};

NAN_METHOD(Process)
{
   Callback *callback = new Callback(info[0].As<Function>());
   Handle<Object> resp = Local<Object>::Cast(info[1]);
   AsyncQueueWorker(new ProcessWorker(callback, resp));
}

NAN_MODULE_INIT(InitAll)
{
    Set(target, New<String>("process").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Process)).ToLocalChecked());
}

NODE_MODULE(hello, InitAll);

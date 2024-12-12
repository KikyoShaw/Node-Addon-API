//Napi::ThreadSafeFunction 是 Node-API（N-API）中的一部分，用于在 Node.js 的 C++ 插件中实现线程安全的回调功能。它允许你从非主线程安全地调用 JavaScript 函数

#include <napi.h>
#include <thread>

void CallJsFunction(Napi::Env env, Napi::Function jsCallback, int *data){
    jsCallback.Call({Napi::Number::New(env, *data)});
}

void WorkerThread(Napi::ThreadSafeFunction tsfn){
    for (int i = 0; i < 5; i++)
    {
        tsfn.BlockingCall(new int(i), CallJsFunction);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    
    //释放
    tsfn.Release();
}

Napi::Value StartThread(const Napi::CallbackInfo &info){
    Napi::Env env = info.Env();

    if(!info[0].IsFunction()){
        Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Function jsCallback = info[0].As<Napi::Function>();

    auto tf = Napi::ThreadSafeFunction::New(
        env,
        jsCallback, //要调用的 JavaScript 函数
        "ThreadSafeFunction", //线程安全函数的名称
        0, //队列大小不受限制
        1 //只有一个线程可以同时调用
        // [](Napi::Env env, void* finalizeData, void* data){
        //     delete reinterpret_cast<int*>(data);
        // } //可选的析构函数
    );

    std::thread t(WorkerThread, tf);
    t.detach();

    return Napi::Value();
}

Napi::Object Init(Napi::Env env, Napi::Object exports){
    exports.Set(Napi::String::New(env, "startThread"), Napi::Function::New(env, StartThread));
    return exports;
}

NODE_API_MODULE(threadsafefun, Init)
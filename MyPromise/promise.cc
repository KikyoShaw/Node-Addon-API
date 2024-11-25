#include <napi.h>
#include <chrono>
#include <thread>

class AsyncWorker : public Napi::AsyncWorker {
public:
    AsyncWorker(Napi::Promise::Deferred deferred)
    : Napi::AsyncWorker(Napi::Function::New(deferred.Env(), [](const Napi::CallbackInfo&) {})),
      deferred(deferred) {}

    void Execute() override {
        // Simulate a long-running task
        std::this_thread::sleep_for(std::chrono::seconds(2));
        result_ = 42; // Example result
    }

    void OnOK() override {
        deferred.Resolve(Napi::Number::New(Env(), result_));
    }

    void OnError(const Napi::Error& e) override {
        deferred.Reject(e.Value());
    }

private:
    Napi::Promise::Deferred deferred;
    int result_;
};

Napi::Value AsyncFunction(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
    AsyncWorker* worker = new AsyncWorker(deferred);
    worker->Queue();
    return deferred.Promise();
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "asyncFunction"), Napi::Function::New(env, AsyncFunction));
    return exports;
}

NODE_API_MODULE(promise, InitAll)
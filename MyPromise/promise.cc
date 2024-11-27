#include <napi.h>
#include <chrono>
#include <thread>

// 自定义的异步任务类，继承自Napi::AsyncWorker
class AsyncWorker : public Napi::AsyncWorker {
public:
    // 构造函数接收一个Promise的Deferred对象用于后续的resolve/reject操作
    AsyncWorker(Napi::Promise::Deferred deferred)
    : Napi::AsyncWorker(Napi::Function::New(deferred.Env(), [](const Napi::CallbackInfo&) {})),
      deferred(deferred) {}

    // 此函数在后台线程中执行长时间的操作
    void Execute() override {
        // 模拟一个耗时操作，例如网络请求或I/O
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // 假设操作得到的结果为42
        result_ = 42;
    }

    // 当Execute执行完成成功时，调用此函数
    void OnOK() override {
        // 使用deferred对象将结果传递回JavaScript，标记为resolved状态
        deferred.Resolve(Napi::Number::New(Env(), result_));
    }

    // 如果Execute过程中产生错误，调用此函数
    void OnError(const Napi::Error& e) override {
        // 使用deferred对象将错误传递回JavaScript，标记为rejected状态
        deferred.Reject(e.Value());
    }

private:
    Napi::Promise::Deferred deferred; // 用于操作JavaScript中的Promise对象
    int result_; // 任务的计算结果
};

// 在JavaScript中调用的函数，启动异步任务并返回Promise
Napi::Value AsyncFunction(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // 创建一个Promise的Deferred对象，供异步操作使用
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    // 创建并启动异步任务
    AsyncWorker* worker = new AsyncWorker(deferred);
    worker->Queue();

    // 返回Promise给JavaScript调用方
    return deferred.Promise();
}

// 初始化模块，将asyncFunction导出为Node.js中的可用函数
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "asyncFunction"), Napi::Function::New(env, AsyncFunction));
    return exports;
}

// 定义模块，在Node.js加载模块时调用InitAll函数
NODE_API_MODULE(promise, InitAll)
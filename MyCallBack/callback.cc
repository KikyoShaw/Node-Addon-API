#include <napi.h>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <iostream>

typedef void (*callback_success)(void* data, size_t data_length, void* user_data);
typedef void (*callback_error)(const char* error, void* user_data);

// 模拟异步任务（可能成功或失败）
void async_task(void* data, size_t data_length, callback_success cb_success, callback_error cb_error, void* user_data) {
    std::thread([data, data_length, cb_success, cb_error, user_data]() {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟耗时任务

        if (data == nullptr
            || data_length == 0){
            cb_error("Task failed due to some error, data is empty.", user_data); // 失败，返回错误信息
            return;
        }

        // 反转字节数组
        auto charData = static_cast<unsigned char*>(data);
        std::reverse(charData, charData + data_length);
        cb_success((void*)charData, data_length, user_data); // 成功，返回反转后的字节数组
    }).detach();
}

class AsyncTaskWrapper : public Napi::ObjectWrap<AsyncTaskWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "AsyncTaskWrapper", {
            InstanceMethod("startTask", &AsyncTaskWrapper::StartTask)
        });

        exports.Set("AsyncTaskWrapper", func);
        return exports;
    }

    AsyncTaskWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<AsyncTaskWrapper>(info) {}

private:
    Napi::Value StartTask(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        //std::cout << "Number of arguments: " << info.Length() << std::endl;
        //std::cout << "Type of argument 0: " << info[0].Type() << std::endl;


        // 返回一个 Promise 对象
        auto deferred = Napi::Promise::Deferred::New(env);

        // 创建 ThreadSafeFunction，用于通知 Promise 完成
        auto tsfn = Napi::ThreadSafeFunction::New(
            env,
            Napi::Function::New(env, [](const Napi::CallbackInfo& info) {}),
            "PromiseCallback",
            0,
            1
        );

        // 获取函数入参
        auto input = info[0].As<Napi::ArrayBuffer>();
        //auto inputData = input.Data();
        auto inputDataLength = input.ByteLength();
        auto inputData = new unsigned char[inputDataLength];
        std::memcpy(inputData, input.Data(), inputDataLength);

        // 调用 C 库的异步任务
        async_task(
            inputData,
            inputDataLength,
            [](void* resultData, size_t resultDataLength, void* data) {
                auto [deferred, tsfn] = *static_cast<std::pair<Napi::Promise::Deferred, Napi::ThreadSafeFunction>*>(data);

                // 如果任务成功，resolve Promise
                tsfn.BlockingCall([resultData, resultDataLength, deferred](Napi::Env env, Napi::Function) {
                    deferred.Resolve(Napi::ArrayBuffer::New(env, resultData, resultDataLength, 
                        [](Napi::Env env, void* finalizeData) {
                            // 释放自定义内存
                            delete[] static_cast<unsigned char*>(finalizeData);
                        }
                    ));
                });

                tsfn.Release(); // 释放 ThreadSafeFunction
                delete static_cast<std::pair<Napi::Promise::Deferred, Napi::ThreadSafeFunction>*>(data);
            },
            [](const char* error, void* data) {
                auto [deferred, tsfn] = *static_cast<std::pair<Napi::Promise::Deferred, Napi::ThreadSafeFunction>*>(data);

                // 如果任务失败，reject Promise
                tsfn.BlockingCall([error, deferred](Napi::Env env, Napi::Function) {
                    deferred.Reject(Napi::Error::New(env, error).Value());
                });

                tsfn.Release(); // 释放 ThreadSafeFunction
                delete static_cast<std::pair<Napi::Promise::Deferred, Napi::ThreadSafeFunction>*>(data);
            },
            new std::pair(deferred, tsfn)
        );

        return deferred.Promise();
    }
};

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return AsyncTaskWrapper::Init(env, exports);
}

NODE_API_MODULE(callback, InitAll)


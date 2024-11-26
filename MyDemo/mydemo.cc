#include <napi.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <iostream>

class MyAsyncWorker : public Napi::AsyncWorker {
public:
    MyAsyncWorker(Napi::Promise::Deferred& deferred, const std::vector<uint8_t>& data)
    : Napi::AsyncWorker(Napi::Function::New(deferred.Env(), [](const Napi::CallbackInfo&) {})), deferred_(deferred), data_(data) {}

    void Execute() override {
       std::reverse(data_.begin(), data_.end());
       std::cout << "1111" << std::endl;
       std::this_thread::sleep_for(std::chrono::seconds(2)); //延迟2s
    }

    void OnOK() override {
        Napi::Env env = Env();
        Napi::HandleScope scope(env);
        // 使用 vector 数据创建 Napi::Buffer 并解析 Promise
        std::cout << "2222" << std::endl;
         if (data_.empty()) {
            deferred_.Reject(Napi::Error::New(env, "Binary data is empty").Value());
            return;
        }
        deferred_.Resolve(Napi::Buffer<uint8_t>::Copy(env, data_.data(), data_.size()));
        std::cout << "3333" << std::endl;
    }

    void OnError(const Napi::Error& e) override {
        deferred_.Reject(e.Value());
    }

private:
   Napi::Promise::Deferred deferred_;
   std::vector<uint8_t> data_;
};

Napi::Value CreateReversedBinaryStreamAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::cout << info[0].Type() << std::endl;
    
    // 将 Buffer 或 ArrayBuffer 数据转换为 vector
    std::vector<uint8_t> binaryData;
    if (info[0].IsBuffer()) {
        Napi::Buffer<uint8_t> inputBuffer = info[0].As<Napi::Buffer<uint8_t>>();
        binaryData.assign(inputBuffer.Data(), inputBuffer.Data() + inputBuffer.ByteLength());
        std::cout <<  "IsBuffer" << std::endl;
    }
    else if (info[0].IsArrayBuffer()) {
        Napi::ArrayBuffer inputArrayBuffer = info[0].As<Napi::ArrayBuffer>();
        //uint8_t* data = static_cast<uint8_t*>(inputArrayBuffer.Data());
        //size_t length = inputArrayBuffer.ByteLength();
        //std::cout << "ArrayBuffer data: ";
       // for (size_t i = 0; i < length; ++i) {
            //std::cout << static_cast<int>(data[i]) << " ";
        //}
        //std::cout << std::endl;
        //uint8_t* data = static_cast<uint8_t*>(inputArrayBuffer.Data());
        //size_t length = inputArrayBuffer.ByteLength();
        //binaryData = std::vector<uint8_t>(data, data + length);
        binaryData.assign(static_cast<uint8_t*>(inputArrayBuffer.Data()), static_cast<uint8_t*>(inputArrayBuffer.Data()) + inputArrayBuffer.ByteLength());
        std::cout <<  "IsArrayBuffer" << std::endl;
    }

    // 打印初始化后的 binaryData 内容
    //std::cout << "Received binaryData: ";
   // for (size_t i = 0; i < binaryData.size(); ++i) {
       // std::cout << std::hex << static_cast<int>(binaryData[i]) << " ";
    //}
    //std::cout << std::endl;

    std::cout <<  "0000" << std::endl;

    // 创建并返回一个 Promise
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
    MyAsyncWorker* worker = new MyAsyncWorker(deferred, binaryData);
    worker->Queue();

    return deferred.Promise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "createReversedBinaryStreamAsync"), Napi::Function::New(env, CreateReversedBinaryStreamAsync));
    return exports;
}

// 模块注册
NODE_API_MODULE(mydemo, Init)
#include <napi.h>
#include <vector>
#include <iostream>

// 获取传入的ArrayBuffer或Buffer对象并将其反转
Napi::Value GetByteArray(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // 定义一个vector来存储二进制数据
    std::vector<uint8_t> binaryData;
    
    // 检查传入参数是否为Buffer
    if (info[0].IsBuffer()) {
        Napi::Buffer<uint8_t> inputBuffer = info[0].As<Napi::Buffer<uint8_t>>();
        // 将Buffer数据复制到vector
        binaryData.assign(inputBuffer.Data(), inputBuffer.Data() + inputBuffer.ByteLength());
        std::cout << "IsBuffer" << std::endl;
    }
    // 检查传入参数是否为ArrayBuffer
    else if (info[0].IsArrayBuffer()) {
        Napi::ArrayBuffer inputArrayBuffer = info[0].As<Napi::ArrayBuffer>();
        // 将ArrayBuffer数据复制到vector
        binaryData.assign(static_cast<uint8_t*>(inputArrayBuffer.Data()), static_cast<uint8_t*>(inputArrayBuffer.Data()) + inputArrayBuffer.ByteLength());
        std::cout << "IsArrayBuffer" << std::endl;
    }

    // 数据反转操作
    std::reverse(binaryData.begin(), binaryData.end());

    // 返回反转后的数据作为新的Buffer对象给JavaScript调用
    return Napi::Buffer<uint8_t>::Copy(env, binaryData.data(), binaryData.size());
}

// 初始化模块，将GetByteArray函数导出为Node.js可用函数
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "getByteArray"), Napi::Function::New(env, GetByteArray));
    return exports;
}

// 定义模块，这样Node.js可以在加载时调用InitAll函数
NODE_API_MODULE(bytearrary, InitAll)
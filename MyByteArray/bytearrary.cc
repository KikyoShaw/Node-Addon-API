#include <napi.h>
#include <vector>

Napi::Value CreateByteArray(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // 创建一个 10 字节的缓冲区，并将其填充为 0 到 9
    std::vector<uint8_t> vec(10);
    for (uint8_t i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }

    // 用 vector 数据创建 Napi::Buffer
    return Napi::Buffer<uint8_t>::Copy(env, vec.data(), vec.size());
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "createByteArray"), Napi::Function::New(env, CreateByteArray));
    return exports;
}

// 模模块注册
NODE_API_MODULE(bytearrary, InitAll)
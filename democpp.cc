#include <napi.h>

// Napi::Value: 表示泛型 JavaScript 值的基类。
// Napi::Object: 表示 JavaScript 对象。
// Napi::Array: 表示 JavaScript 数组。
// Napi::String: 表示 JavaScript 字符串。
// Napi::Number: 表示 JavaScript 数字。
// Napi::Boolean: 表示 JavaScript 布尔值。
// Napi::Function: 表示 JavaScript 函数。

// 这个函数将在C++代码中被当作回调函数调用
void CallJsCallback(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // 获取传入的第一个参数，即JavaScript中的回调函数
    if (!info[0].IsFunction()) {
        Napi::TypeError::New(env, "Expecting a function as first argument").ThrowAsJavaScriptException();
        return;
    }
    Napi::Function callback = info[0].As<Napi::Function>();

    // 执行回调并传递参数
    // 这里我们传回数字42给JavaScript
    callback.Call(env.Global(), { Napi::Number::New(env, 42) });
}

// 初始化模块，将CallJsCallback函数导出为Node.js中的可用函数
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "callJsCallback"), Napi::Function::New(env, CallJsCallback));
    return exports;
}

// 定义模块，在Node.js加载模块时调用InitAll函数
NODE_API_MODULE(callbacke, InitAll)
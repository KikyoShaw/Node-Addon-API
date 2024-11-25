#include <napi.h>

Napi::Value Add(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  int arg0 = info[0].As<Napi::Number>().Int32Value();
  int arg1 = info[1].As<Napi::Number>().Int32Value();

  int arg2 = arg0 + arg1;
  
  Napi::Number num = Napi::Number::New(env, arg2);

  return num;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("add", Napi::Function::New(env, Add));
  return exports;
}

NODE_API_MODULE(cal, Init)
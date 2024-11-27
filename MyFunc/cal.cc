#include <napi.h>

Napi::Value Add(const Napi::CallbackInfo& info) //定义一个C++函数Add，返回类型是Napi::Number，参数是Napi::CallbackInfo&，它提供了对JavaScript调用此函数时传递的参数的访问
{
  Napi::Env env = info.Env();

  // 检查参数数量和类型
  if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return Napi::Number::New(env, 0);
  }

  //参数提取和运算
  double arg0 = info[0].As<Napi::Number>().DoubleValue();
  double arg1 = info[1].As<Napi::Number>().DoubleValue();
  double sum = arg0 + arg1;
  return Napi::Number::New(env, sum); //将C++的double变成JavaScript的数字返回。
}

Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
  //使用 exports.Set 方法将 C++ 函数 Add 导出为JavaScript可调用的函数。
  exports.Set(Napi::String::New(env, "add"), Napi::Function::New(env, Add)); //"add"是JavaScript中调用这个方法的名称。
  return exports;
}

//使用 NODE_API_MODULE 宏来指定模块的名称 (cal) 和初始化函数 (Init)。
//它告诉Node.js在加载模块时应该调用哪个初始化函数。
NODE_API_MODULE(cal, Init)
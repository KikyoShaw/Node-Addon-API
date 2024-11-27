#include <napi.h>

// 定义一个简单的C++类MyClass，用于存储一个整数值
class MyClass {
public:
    MyClass(int value) : value_(value) {}  // 构造函数，用给定值初始化
    int getValue() { return value_; }      // 返回当前存储的值
    void setValue(int value) { value_ = value; }  // 设置新的值
private:
    int value_;  // 存储的整数值
};

// 定义一个包装类Wrapper，用于将MyClass与JavaScript对象关联
class Wrapper : public Napi::ObjectWrap<Wrapper> {
public:
    // 初始化JavaScript包装类
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    // 构造函数，通过JavaScript调用
    Wrapper(const Napi::CallbackInfo &info);

private:
    static Napi::FunctionReference constructor;  // 用于JavaScript构造此对象
    Napi::Value GetValue(const Napi::CallbackInfo &info);  // JavaScript访问函数
    void SetValue(const Napi::CallbackInfo &info);  // JavaScript修改函数

    MyClass* myClass_;  // 指向被包装的MyClass实例
};

Napi::FunctionReference Wrapper::constructor;

// 初始化Wrapper类，定义类函数并设置到exports对象
Napi::Object Wrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "MyClass", {
        InstanceMethod("getValue", &Wrapper::GetValue),  // 绑定到JS对象的成员函数
        InstanceMethod("setValue", &Wrapper::SetValue)   // 绑定到JS对象的成员函数
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();  // 防止垃圾回收

    exports.Set("MyClass", func);  // 将构造函数导出为模块的一部分
    return exports;
}

// 构造函数实现，用于初始化Wrapper对象，此时创建MyClass实例
Wrapper::Wrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Wrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int value = info[0].As<Napi::Number>().Int32Value();  // 从JavaScript参数中获取整数
    myClass_ = new MyClass(value);  // 创建MyClass实例
}

// 实现getValue，将C++中存储的值返回给JavaScript环境
Napi::Value Wrapper::GetValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int value = myClass_->getValue();
    return Napi::Number::New(env, value);  // 返回值作为JavaScript数字
}

// 实现setValue，从JavaScript获取新值并设置到C++类中
void Wrapper::SetValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int val = info[0].As<Napi::Number>().Int32Value();
    myClass_->setValue(val);
}

// 初始化模块，将Wrapper类注册到Node.js环境中
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return Wrapper::Init(env, exports);
}

// 定义模块入口，让Node.js在加载时调用InitAll函数
NODE_API_MODULE(mytestclass, InitAll)
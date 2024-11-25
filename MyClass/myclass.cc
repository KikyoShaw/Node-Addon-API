#include <napi.h>

class MyClass {
public:
    MyClass(int value) : value_(value) {}
    int getValue() { return value_; }
    void setValue(int value) { value_ = value; }
private:
    int value_;
};

class Wrapper : public Napi::ObjectWrap<Wrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Wrapper(const Napi::CallbackInfo &info);

private:
    static Napi::FunctionReference constructor;
    Napi::Value GetValue(const Napi::CallbackInfo &info);
    void SetValue(const Napi::CallbackInfo &info);

    MyClass* myClass_;
};

Napi::FunctionReference Wrapper::constructor;

Napi::Object Wrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "MyClass", {
        InstanceMethod("getValue", &Wrapper::GetValue),
        InstanceMethod("setValue", &Wrapper::SetValue)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("MyClass", func);
    return exports;
}

Wrapper::Wrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Wrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int value = info[0].As<Napi::Number>().Int32Value();
    myClass_ = new MyClass(value);
}

Napi::Value Wrapper::GetValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int value = myClass_->getValue();
    return Napi::Number::New(env, value);
}

void Wrapper::SetValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int val = info[0].As<Napi::Number>().Int32Value();
    myClass_->setValue(val);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return Wrapper::Init(env, exports);
}

NODE_API_MODULE(mytestclass, InitAll)
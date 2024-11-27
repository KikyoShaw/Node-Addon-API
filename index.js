// 导入编译好的本地模块
const addon = require('./build/Release/callbacke');

// 定义一个回调函数，将在C++代码中被调用
function myCallback(number) {
    console.log('Callback called with value:', number);
}

// 调用本地模块的函数，并传递JavaScript的回调函数
addon.callJsCallback(myCallback);
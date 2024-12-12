import addon from './require-demo.js';
import {greet} from './demo.js'

// 定义一个回调函数，将在C++代码中被调用
function myCallback(number) {
    console.log('Callback called with value:', number);
    console.log(greet("hello world"))
}

// 调用本地模块的函数，并传递JavaScript的回调函数
addon.callJsCallback(myCallback);
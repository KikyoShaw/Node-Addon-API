const addon = require('./build/Release/threadsafefun');

// 定义一个 JavaScript 回调函数
function callback(value) {
    console.log('Received from thread:', value);
  }
  
  // 调用 StartThread 函数并传递回调
  try {
    addon.startThread(callback);
  } catch (error) {
    // 处理错误
    console.error('Error:', error);
  }
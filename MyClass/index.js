// 导入用C++编写的本地模块，生成的addon在build/Release目录中
const myclass = require('./build/Release/mytestclass');

// 创建MyClass的一个实例，并传递初始值100
const myClassInstance = new myclass.MyClass(100);

// 调用getValue方法，输出当前值（100）
console.log(myClassInstance.getValue());

// 调用setValue方法，将实例的值设置为200
myClassInstance.setValue(200);

// 再次调用getValue方法，输出更新后的值（200）
console.log(myClassInstance.getValue());
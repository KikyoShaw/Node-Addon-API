const myclass = require('./build/Release/mytestclass');
const myClassInstance = new myclass.MyClass(100);
console.log(myClassInstance.getValue());
myClassInstance.setValue(200);
console.log(myClassInstance.getValue());
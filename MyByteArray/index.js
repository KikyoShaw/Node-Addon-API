const bytearrary = require('./build/Release/bytearrary');

const byteArray = bytearrary.createByteArray();
console.log(byteArray); // 输出: <Buffer 00 01 02 03 04 05 06 07 08 09>
console.log(byteArray.length); // 输出: 10
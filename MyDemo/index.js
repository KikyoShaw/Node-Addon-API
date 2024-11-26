const mydemo = require('./build/Release/mydemo');
const { Buffer } = require('node:buffer');

const binaryData = Buffer.from([0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A]);

// 获取底层 ArrayBuffer
const arrayBuffer = binaryData.buffer.slice(binaryData.byteOffset, binaryData.byteOffset + binaryData.byteLength);
//不能直接拿binaryData.buffer，会拿到整个buffer分配ArrayBuffer的内存池大小，通常为8192
//const arrayBuffer = binaryData.buffer;

console.log(arrayBuffer);
console.log(arrayBuffer.byteLength); // 输出 ArrayBuffer 的字节长度

mydemo.createReversedBinaryStreamAsync(arrayBuffer)
    .then((reversedBinaryStream) => {
        console.log(reversedBinaryStream); 
        console.log(reversedBinaryStream.length);
    })
    .catch((err) => {
        console.error(err);
    });
const { AsyncTaskWrapper } = require('./build/Release/callback');

const initialArray = new Uint8Array([1, 2, 3, 4, 5]); // 创建并初始化一个 Uint8Array
const buffer = initialArray.buffer;

console.log(buffer);
console.log(buffer.byteLength); // 输出 ArrayBuffer 的字节长度

const wrapper = new AsyncTaskWrapper();

wrapper.startTask(buffer)
    .then((result) => {
        console.log("Task succeeded:", new Uint8Array(result));
    })
    .catch((err) => {
        console.error("Task failed:", err);
    });
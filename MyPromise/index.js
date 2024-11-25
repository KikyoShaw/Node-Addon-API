const addon = require('./build/Release/promise');

async function test() {
    try {
        const result = await addon.asyncFunction();
        console.log(result); // 输出: 42
    } catch (err) {
        console.error(err);
    }
}

test();
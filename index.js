let a = require("./build/Release/hello.node");

console.log(a.hello());

a.asyncCall((ret)=>{
    console.log(ret);
})

a.asyncCall().then(ret => {
    console.log(ret);
}, fial => {
    console.log(fial);
})



console.log(a.sum(55,60));
console.log("end");

//js向napi传参
let obj = {name:"zhangjunpu",age:18};
let propValue = a.readJSInputArg(123,obj);
console.log("age: " + propValue);


let nativeObj = a.returnNativeObject();
console.log(JSON.stringify(nativeObj));

let network = {
    "longOperatorName":"zhangjunpu",
    "shortOperatorName":"zjp",
    "plmnNumeric":"bbb",
    "isRoaming": false,
    "regStatus":"zhangjunpu",
    "nsaState":"zhangjunpu",
    "isCaActive":true,
    "isEmergency": true,
};

let ret = a.setNetStats(network)
console.log("ret: " + ret);
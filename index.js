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
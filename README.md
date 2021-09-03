# N-APIExample
N-API-简单使用

* 说明

  > 这个demo在ubuntu上面运行通过环境如下

  ```c
  //node 版本
  v8.10.0
  
  //node-gyp 版本
  v3.6.2
  
  //ubuntu版本
  18.04
  
  //npm版本
  ```

* 环境安装

  ```c
  //node 直接网站搜索通过wget命令去下载8.0+版本,然后解压配置环境变量.
  //不要通过sudo apt install node去下载,版本会比较低
  
  //安装npm
  sudo apt install npm
  
  //安装gyp
  sudo npm install -g node-gyp
  ```



**遇到的问题**

* 环境问题

  ```c
  //问题1
  bravewinters@bravewinters:~$ node-gyp 
  internal/util.js:209
      throw new errors.TypeError('ERR_INVALID_ARG_TYPE', 'original', 'function');
      ^
  
  TypeError [ERR_INVALID_ARG_TYPE]: The "original" argument must be of type function
      at Object.promisify (internal/util.js:209:11)
      at Object.<anonymous> (/usr/local/lib/node_modules/node-gyp/lib/install.js:15:29)
      at Module._compile (module.js:652:30)
      at Object.Module._extensions..js (module.js:663:10)
      at Module.load (module.js:565:32)
      at tryModuleLoad (module.js:505:12)
      at Function.Module._load (module.js:497:3)
      at Module.require (module.js:596:17)
      at require (internal/module.js:11:18)
      at /usr/local/lib/node_modules/node-gyp/lib/node-gyp.js:190:37
         
  ```

  只要运行node-gyp 任何命令都会报这个错,一开始在16.04上没遇到类似问题.

  通过vim看了`/usr/local/lib/node_modules/node-gyp/lib/install.js`这个文件的15行,调用了utils.js

  这个类,但是路径跟上面的`internal/util.js`对不上,并且文件内容也对不上,就怀疑是找了其他文件, 随后打算重新卸载node-gyp里面的内容.把`/usr/local/lib/node_modules`里面的内容全部移除了,再编译居然没问题了. 对比下来报错的node-gyp版本是8.2.0

* 代码问题

  > 在调用asyncCall方法就是一直不成功反复看了很久

```c
//错误写法
....
napi_value argv[1] = {objResult};
napi_call_function(env, recv, callbackFunc, sizeof(argv), argv, &callResult);

//正确写法
...
napi_call_function(env, recv, callbackFunc, sizeof(argv) / sizeof(objResult), argv, &callResult);

// 区别在于方法的第四个参数 sizeof(argv) 和 sizeof(argv) /sizeof(objResult)
// 错误写法(sizeof(argv))获取的是argv所在内存的字节数, 但是这个地方需要传方法参数个数
// 所以要么写 1 要么写(sizeof(argv) / sizeof(objResult))

// 因为参数个数对不上所以一直调用不成功
```





**用到的编译命令**

1. `node-gyp rebuild` 会默认找当前路径的binding.gyp文件进行构建
2. `node --napi-modules index.js` 执行js文件


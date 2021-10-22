#include <node_api.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "include/napi_util.h"
#include "napi_net_helper.cpp"
typedef struct Context
{
    napi_deferred deferred;
    napi_async_work work;
    napi_ref ref;
    uint32_t refCount;
    int32_t value;
};

napi_value NapiHello(napi_env env, napi_callback_info info)
{
    napi_value ret = NULL;
    char content[] = "hello wintersweett";
    napi_create_string_utf8(env, content, strlen(content), &ret);
    return ret;
}

void work(napi_env env, void *arg)
{
    Context *context = (Context *)arg;
    context->value = 800;
};

void done(napi_env env, napi_status stats, void *arg)
{
    Context *context = (Context *)arg;
    napi_value value = NULL;
    if (true)
    {
        napi_create_int32(env, context->value, &value);
        napi_resolve_deferred(env, context->deferred, value);

        //创建AsyncCallback对象
        napi_value objResult = NULL;
        napi_create_object(env, &objResult);
        //创建返回对象的属性值
        napi_value propValueRet = NULL;
        napi_create_string_utf8(env, "bravewinters", NAPI_AUTO_LENGTH, &propValueRet);
        //设置对象的属性
        napi_value setPropNameRet = NULL;
        napi_set_named_property(env, objResult, "name", propValueRet);

        //获取会调方法的引用
        napi_value callbackFunc = NULL;
        napi_get_reference_value(env, context->ref, &callbackFunc);

        //调用方法
        napi_value callResult = NULL;
        napi_value recv = NULL;
        napi_get_undefined(env, &recv);
        napi_value argv[1] = {objResult};
        napi_call_function(env, recv, callbackFunc, sizeof(argv) / sizeof(objResult), argv, &callResult);
        napi_delete_reference(env, context->ref);
    }
    else
    {
        napi_create_int32(env, -1, &value);
        napi_reject_deferred(env, context->deferred, value);
    }
    napi_delete_async_work(env, context->work);
    context->deferred = NULL;
    free(context);
}

napi_value NapiSum(napi_env env, napi_callback_info info)
{
    napi_status status;

    size_t argc = 2;
    napi_value argv[2];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    assert(status == napi_ok);

    if (argc < 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments.");
        return NULL;
    }

    napi_valuetype valueType1, valueType2;

    status = napi_typeof(env, argv[0], &valueType1);
    assert(status == napi_ok);

    status = napi_typeof(env, argv[1], &valueType2);
    assert(status == napi_ok);

    if (valueType1 != napi_number || valueType2 != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }

    double value1, value2;

    status = napi_get_value_double(env, argv[0], &value1);
    assert(status == napi_ok);

    status = napi_get_value_double(env, argv[1], &value2);
    assert(status == napi_ok);

    napi_value sum;
    status = napi_create_double(env, value1 + value2, &sum);
    assert(status == napi_ok);

    return sum;
}

napi_value NapiAsyncCall(napi_env env, napi_callback_info info)
{
    Context *context = (Context *)malloc(sizeof(Context));
    napi_value promise_value = NULL;
    napi_status promise_ret = napi_create_promise(env, &context->deferred, &promise_value);

    size_t argc = 1;
    napi_value argv[1] = {0};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);

    napi_create_reference(env, argv[0], 1, &context->ref);

    napi_value resource_value = NULL;
    char taskName[] = "asyncMethod";
    napi_create_string_utf8(env, taskName, strlen(taskName), &resource_value);
    napi_value work_result = NULL;
    napi_create_async_work(env, NULL, resource_value, work, done, context, &context->work);
    napi_queue_async_work(env, context->work);
    return promise_value;
}

napi_value ReadJSInputArg(napi_env env, napi_callback_info info) {
    napi_value ret = NULL;
    napi_status stats = napi_ok;

    size_t argc = 2;
    napi_value argv[2] = {0};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    stats = napi_get_cb_info(env,info,&argc, argv,&thisVar,&data);
    assert(stats == napi_ok);

    int32_t arg1 = 0;
    stats = napi_get_value_int32(env, argv[0], &arg1);
    assert(stats == napi_ok);
    
    char buf[30] = {};
    GetStringProperty(buf,env,argv[1],"name");

    int32_t age = GetIntProperty(env,argv[1],"age");

    stats = napi_create_string_utf8(env,buf,sizeof(buf),&ret);
    assert(stats == napi_ok);

    stats =  napi_create_int32(env,age,&ret);
    assert(stats == napi_ok);
    return ret;
}

napi_value ReturnNativeObject(napi_env env, napi_callback_info info) {
    napi_value ret = NULL;

    napi_value obj;
    napi_create_object(env, &obj);

    napi_value propValue;
    char buf[] = "wintersweett";
    napi_create_string_utf8(env,buf,sizeof(buf) - 1,&propValue);

    napi_value propName;
    char keyBuf[] = "braveheart";
    napi_create_string_utf8(env,keyBuf,sizeof(keyBuf) - 1,&propName);
    napi_set_property(env,obj,propName,propValue);

    return obj;
}

napi_value SetNetStats(napi_env env,napi_callback_info info) {
    return ChangeNetStats(env,info);
}

#define MAP_JS_NAPI_METHOD(jsMethId, napiMethId)             \
    {                                                  \
        jsMethId, 0, napiMethId, 0, 0, 0, napi_default, 0 \
    }

extern "C"
{
    napi_value Register(napi_env env, napi_value exports)
    {
        napi_property_descriptor desc[] = {
            MAP_JS_NAPI_METHOD("hello", NapiHello),
            MAP_JS_NAPI_METHOD("sum", NapiSum),
            MAP_JS_NAPI_METHOD("asyncCall", NapiAsyncCall),
            MAP_JS_NAPI_METHOD("readJSInputArg",ReadJSInputArg),
            MAP_JS_NAPI_METHOD("returnNativeObject", ReturnNativeObject),
            MAP_JS_NAPI_METHOD("setNetStats", SetNetStats),
        };
        napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
        return exports;
    }
}

NAPI_MODULE(hello, Register);
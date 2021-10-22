#ifndef NAPI_NET_HELPER_H
#define NAPI_NET_HELPER_H

#include "include/napi_util.h"
#include <iostream>

using namespace std;

struct NetworkStats {
    char longOperatorName[MAX_PROPERTY_LENGTH];
    char shortOperatorName[MAX_PROPERTY_LENGTH];
    char plmnNumeric[MAX_PROPERTY_LENGTH];
    bool isRoaming;
    char regStatus[MAX_PROPERTY_LENGTH];
    char nsaState[MAX_PROPERTY_LENGTH];
    bool isCaActive;
    bool isEmergency;
};

napi_value ChangeNetStats(napi_env env,napi_callback_info info) {
     size_t argc  = 1;
    napi_value argv[1] = {0};
    napi_value thiz = NULL;
    void * data = NULL;
    napi_get_cb_info(env, info, &argc, argv,  &thiz, &data);
    struct NetworkStats *networkStats = (struct NetworkStats *)malloc(sizeof(struct NetworkStats));
    GetStringProperty(networkStats->longOperatorName, env, argv[0], "longOperatorName");
    GetStringProperty(networkStats->shortOperatorName, env, argv[0], "shortOperatorName");
    GetStringProperty(networkStats->plmnNumeric, env, argv[0], "plmnNumeric");
    networkStats->isRoaming = GetBoolProperty( env, argv[0], "isRoaming");
    GetStringProperty(networkStats->regStatus, env, argv[0],"regStatus");
    GetStringProperty(networkStats->nsaState, env, argv[0],"nsaState");
    networkStats->isCaActive = GetBoolProperty( env, argv[0], "isCaActive");
    networkStats->isEmergency = GetBoolProperty( env, argv[0], "isEmergency");

    cout << "longOptName: " << networkStats->longOperatorName 
    << "  shortOptName: "<< networkStats->shortOperatorName 
    << " plmnNumeric:" << networkStats->plmnNumeric 
    << " isRoaming:" << networkStats->isRoaming 
    << " regStatus:" << networkStats->regStatus 
    << " nsaState:" << networkStats->nsaState 
    << " isCaActive:" << networkStats->isCaActive 
    << " isEmergency:" << networkStats->isEmergency << endl;
    napi_value val = NULL;
    napi_create_int32(env, 1, &val);

    return  val;
}

#endif //NAPI_NET_HELPER_H
#include <d3d12.h>
#include <dxgi.h>

#include <stdio.h>

#include "NVAPI/nvapi.h"
#include "NVAPI/nvapi_lite_d3dext.h"
#include "NVAPI/nvShaderExtnEnums.h"

#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"
#include "Unity/IUnityGraphicsD3D12.h"
#include "Unity/IUnityLog.h"

IUnityInterfaces* s_UnityInterfaces = nullptr;
IUnityGraphics* s_UnityGraphics = nullptr;
IUnityGraphicsD3D12v2* s_D3D12Device = nullptr;

void Log(const char* message) 
{
    IUnityLog* debug = s_UnityInterfaces->Get<IUnityLog>();
    debug->Log(kUnityLogTypeLog, message, "", 0);
}

void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType) 
{
    switch (eventType) 
    {
    case kUnityGfxDeviceEventInitialize:
    {
        s_D3D12Device = s_UnityInterfaces->Get<IUnityGraphicsD3D12v2>();

        NvAPI_Status status = NvAPI_Initialize();

        char statusMsg[256] = { 0 };
        sprintf_s(statusMsg, sizeof(statusMsg), "NVAPI Init Status: %d", status);
        Log(statusMsg);

        break;
    }
       
    case kUnityGfxDeviceEventShutdown:

        NvAPI_Unload();

        break;
    };
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces) 
{
    s_UnityInterfaces = unityInterfaces;

    s_UnityGraphics = unityInterfaces->Get<IUnityGraphics>();
    s_UnityGraphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload() 
{
    s_UnityGraphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

extern "C" bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API NvAPI_IsShaderExecutionReorderingAPISupported()
{
    bool SERsupported = false;
    NvAPI_Status status = NvAPI_D3D12_IsNvShaderExtnOpCodeSupported(
        s_D3D12Device->GetDevice(), NV_EXTN_OP_HIT_OBJECT_REORDER_THREAD, &SERsupported);
    return (status == NVAPI_OK && SERsupported);
}

extern "C" bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API NvAPI_IsShaderExecutionReorderingSupportedByGPU()
{
    NVAPI_D3D12_RAYTRACING_THREAD_REORDERING_CAPS rtThreadReorderingCaps;
    NvAPI_Status status = NvAPI_D3D12_GetRaytracingCaps(s_D3D12Device->GetDevice(), NVAPI_D3D12_RAYTRACING_CAPS_TYPE_THREAD_REORDERING, &rtThreadReorderingCaps, sizeof(NVAPI_D3D12_RAYTRACING_THREAD_REORDERING_CAPS));
    return (status == NVAPI_OK && rtThreadReorderingCaps == NVAPI_D3D12_RAYTRACING_THREAD_REORDERING_CAP_STANDARD);
}

extern "C" bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API NvAPI_SetNvShaderExtnSlot(unsigned int uavSlot)
{
    const unsigned int uavSpace = 0;
    NvAPI_Status status = NvAPI_D3D12_SetNvShaderExtnSlotSpace(s_D3D12Device->GetDevice(), uavSlot, uavSpace);
    return status == NVAPI_OK;
}
This Unity plugin exports NVAPI functionality to enable Shader Execution Reordering in ray tracing shaders.

To build the plugin (NVAPIPlugin.dll), you need to download NVAPI from https://developer.nvidia.com/gameworksdownload#?search=nvapi and copy NVAPI contents into the plugin Visual Studio folder:

1. Download and extract the NVAPI contents.
2. Copy amd64 folder and all the header files (nvapi.h, nvapi_lite_common.h, etc) to NVAPI folder, next to Unity folder in the Visual Studio project.
3. Open the .sln project file and build Release.
4. Copy the result NVAPIPlugin.dll from its output location to a Unity project in a Assets/Plugins folder.

Test it with your ray tracing shaders. I made a test in https://github.com/INedelcu/PathTracingDemo in nvidia-ser branch. Some hlsl header files from NVAPI need to be present in Assets/Shaders/NVAPI folder: nvHLSLExtns.h, nvHLSLExtnsInternal.h and nvShaderExtnEnums.h.

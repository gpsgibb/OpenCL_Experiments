# OpenCL Platform and Device Info

This code displays some basic info on the available platforms and devices on your hardware. This can be used to determine the platform and device number for the hardware you wish your OpenCL application to use.

## Compilation instructions
To compile the code, type `make linux` for a linux host and `make apple` for a MacOS host. 

## Running the code
Once compiled, run the code as normal (e.g. `$ ./oclinfo`). You should get output similar to:
```
Number of platforms = 1

Platform 0:
  CL_PLATFORM_NAME: Apple
  CL_PLATFORM_PROFILE: FULL_PROFILE
  CL_PLATFORM_VERSION: OpenCL 1.2 (May  3 2020 20:15:19)
  CL_PLATFORM_VENDOR: Apple
  CL_PLATFORM_EXTENSIONS: cl_APPLE_SetMemObjectDestructor cl_APPLE_ContextLoggingFunctions cl_APPLE_clut cl_APPLE_query_kernel_names cl_APPLE_gl_sharing cl_khr_gl_event

  There are 3 devices
  Device 0:
    CL_DEVICE_NAME: Intel(R) Core(TM) i7-4870HQ CPU @ 2.50GHz
    CL_DEVICE_TYPE: CL_DEVICE_TYPE_CPU
    CL_DEVICE_MAX_COMPUTE_UNITS: 8
    CL_DEVICE_MAX_CLOCK_FREQUENCY: 2500MHz
    CL_DEVICE_GLOBAL_MEM_SIZE: 17179869184 bytes
    Supports double precision?: Yes
  Device 1:
    CL_DEVICE_NAME: Iris Pro
    CL_DEVICE_TYPE: CL_DEVICE_TYPE_GPU
    CL_DEVICE_MAX_COMPUTE_UNITS: 40
    CL_DEVICE_MAX_CLOCK_FREQUENCY: 1200MHz
    CL_DEVICE_GLOBAL_MEM_SIZE: 1610612736 bytes
    Supports double precision?: No
  Device 2:
    CL_DEVICE_NAME: AMD Radeon R9 M370X Compute Engine
    CL_DEVICE_TYPE: CL_DEVICE_TYPE_GPU
    CL_DEVICE_MAX_COMPUTE_UNITS: 10
    CL_DEVICE_MAX_CLOCK_FREQUENCY: 800MHz
    CL_DEVICE_GLOBAL_MEM_SIZE: 2147483648 bytes
    Supports double precision?: Yes

```
Where the specific platform and device info printed will depend on your hardware.

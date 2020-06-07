// Displays info on the OpenCL platforms and their associated devices on the 
// system
//
// Useful for getting the platform and device number of the device you wish to
// use in other programs 

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

//maximum length of a string
#define MAXL 1000

//gets the output string for a platform's infotype
void GetPlatformInfo(cl_platform_id platform, cl_platform_info infotype,char* outstring){
    cl_uint ierr;

    ierr = clGetPlatformInfo(platform,
                            infotype,
                            MAXL,
                            outstring,
                            NULL
                            );
    if (ierr != CL_SUCCESS){
        printf("Something went wrong in clGetPlatformInfo!\n");
    }
}

//gets the output from a device's infotype
void GetDeviceInfo(cl_device_id device, cl_device_info infotype, void* output){
    cl_uint ierr;

    ierr = clGetDeviceInfo(device,
                           infotype,
                           MAXL,
                           output,
                           NULL);
    if (ierr != CL_SUCCESS){
        printf("Something went wrong in clGetPlatformInfo!\n");
    }
}

//main program. Loops over available platforms and devices and displays info about them
int main(int argc, char **argv){

    cl_int ierr;

    //get the platforms

    cl_uint n_platforms;
    
    //query how many platforms there are
    ierr = clGetPlatformIDs(0, NULL, &n_platforms); //passing 0 and NULL means this just returns the number of platforms

    if (ierr != CL_SUCCESS){
        printf("Something went wrong querying platform number\n");
        return 1;
    }

    printf("Number of platforms = %d\n",n_platforms);

    //allocate memory for the platforms
    cl_platform_id *Platform_IDs = malloc(n_platforms * sizeof(cl_platform_id));

    //call this again, this time with Platform_IDs to populate this list
    ierr = clGetPlatformIDs(n_platforms,Platform_IDs,NULL);

    if (ierr != CL_SUCCESS){
        printf("Something went wrong getting platforms array\n");
        return 1;
    }

    
    size_t length;
    

    //string to contain output from the Get****Info calls
    char *outstring = malloc(MAXL * sizeof(char));
    // ints and longs for output from the Get***Info calls
    cl_uint cluint_var;
    cl_ulong clulong_var;

    //list of devices
    cl_uint ndevices;
    cl_device_id *devices;

    cl_device_type dev_type;
    
    //loop over list of available platforms and print info
    for (int i=0;i<n_platforms;i++){
        printf("\nPlatform %d:\n",i);
        
        //get specific information from the platform and print it
        GetPlatformInfo(Platform_IDs[i],CL_PLATFORM_NAME,outstring);
        printf("  CL_PLATFORM_NAME: %s\n",outstring); 
        GetPlatformInfo(Platform_IDs[i],CL_PLATFORM_PROFILE,outstring);
        printf("  CL_PLATFORM_PROFILE: %s\n",outstring);
        GetPlatformInfo(Platform_IDs[i],CL_PLATFORM_VERSION,outstring);
        printf("  CL_PLATFORM_VERSION: %s\n",outstring);
        GetPlatformInfo(Platform_IDs[i],CL_PLATFORM_VENDOR,outstring); 
        printf("  CL_PLATFORM_VENDOR: %s\n",outstring); 
        GetPlatformInfo(Platform_IDs[i],CL_PLATFORM_EXTENSIONS,outstring);
        printf("  CL_PLATFORM_EXTENSIONS: %s\n",outstring); 
        printf("\n");
        
        // get the list of devices
        //first call this with NULL to get the size so we can malloc the memoery we need
        ierr = clGetDeviceIDs(Platform_IDs[i],
                              CL_DEVICE_TYPE_ALL,
                              0,
                              NULL,
                              &ndevices);
        

        if (ierr != CL_SUCCESS){
            printf("Something went wrong\n");
            return 1;
        }

        printf("  There are %d devices\n",ndevices);
        
        //allocate memory for the list and call again to populate the list
        devices = malloc(ndevices*sizeof(cl_device_id));
        ierr = clGetDeviceIDs(Platform_IDs[i],
                              CL_DEVICE_TYPE_ALL,
                              ndevices,
                              devices,
                              NULL);
        
        //loop over all the devices and print some info out
        for (int j=0;j<ndevices;j++){
            printf("  Device %d:\n",j);

            GetDeviceInfo(devices[j],CL_DEVICE_NAME,outstring);
            printf("    CL_DEVICE_NAME: %s\n",outstring);

            GetDeviceInfo(devices[j],CL_DEVICE_TYPE,&dev_type);
            if (dev_type == CL_DEVICE_TYPE_GPU){
                printf("    CL_DEVICE_TYPE: CL_DEVICE_TYPE_GPU\n");
            } else if (dev_type == CL_DEVICE_TYPE_CPU){
                printf("    CL_DEVICE_TYPE: CL_DEVICE_TYPE_CPU\n");
            } else {
                printf("    CL_DEVICE_TYPE: other\n");
            }

            GetDeviceInfo(devices[j],CL_DEVICE_MAX_COMPUTE_UNITS,&cluint_var);
            printf("    CL_DEVICE_MAX_COMPUTE_UNITS: %d\n",cluint_var);

            GetDeviceInfo(devices[j],CL_DEVICE_MAX_CLOCK_FREQUENCY,&cluint_var);
            printf("    CL_DEVICE_MAX_CLOCK_FREQUENCY: %dMHz\n",cluint_var);

            GetDeviceInfo(devices[j],CL_DEVICE_GLOBAL_MEM_SIZE,&clulong_var);
            printf("    CL_DEVICE_GLOBAL_MEM_SIZE: %lu bytes\n",clulong_var);

            GetDeviceInfo(devices[j],CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,&cluint_var);
            if (cluint_var != 0){
                printf("    Supports double precision?: Yes\n");
            } else {
                printf("    Supports double precision?: No\n");
            }

        }

    }

}
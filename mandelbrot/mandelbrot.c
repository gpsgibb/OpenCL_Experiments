// Calculates a Mandelbrot set with a GPU using OpenCL

// The mandelbrot set is defined as the set of all complex numbers Z
// for which the iterative function
//               z_(n+1) = z_n^2 + C
// does not diverge.
//
// The program genrates an image of NX*NY pixels corresponding to points on the 
// complex plane. This is written to the file "out.dat"
// 
// XMIN, XMAX, YMIN and YMAX correspond the maximum and minimum values for the
// real and imaginary parts of the complex numbers z= x + iy in the image
//
// The parameters PLATFORMNUM and GPUNUM represent the OpenCL platform and GPU
// device that we wish to use.


#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

//The platform and GPU device IDs we wish to use
#define PLATFORMNUM 0
#define DEVICENUM 2

//length of the string for returning device/platform info
#define STRINGLENGTH 1000

//number of pixels in output image
#define NX 1000
#define NY 1000

// x and y range for the image. y corresponds to Re(z), y corresponds to Im(z)
#define XMIN -1.1785
#define XMAX -1.1755
#define YMIN -0.3000
#define YMAX -0.2970

// a callback function to report on any errors that occur within the context
void errorCallback(const char * errorString, const void *privateInfo, size_t cb, void *userData){
    printf("Error message:\n%s\n",errorString);
    return;
}


int main(int argc, char **argv){
    //arrays of platform and device IDs
    cl_platform_id *platforms;
    cl_device_id *devices;
    
    //the chosen platform and device IDs
    cl_platform_id platform;
    cl_device_id device;
    
    cl_uint nplatforms, ndevices;
    cl_uint ierr;


    //get the platform

    // call once to get the number of platforms
    ierr = clGetPlatformIDs(0,NULL,&nplatforms);
    //allocate memory for the platforms array and fill it
    platforms = malloc(nplatforms * sizeof(cl_platform_id));
    ierr = clGetPlatformIDs(nplatforms,platforms,NULL);
    //select the platform we want

    if (PLATFORMNUM >= nplatforms){
        printf("Error: PLATFORMNUM (%d) is greater than the number of available platforms (%d)!\n",PLATFORMNUM,nplatforms);
        return 1;
    }
    platform = platforms[PLATFORMNUM];

    free(platforms);


    //get the device

    //again we call this once to get the number. THen allocate memory
    ierr = clGetDeviceIDs(platform,CL_DEVICE_TYPE_ALL,0,NULL,&ndevices);

    devices = malloc(ndevices*sizeof(cl_device_id));
    ierr = clGetDeviceIDs(platform,CL_DEVICE_TYPE_ALL,ndevices,devices,NULL);

    if (DEVICENUM >= ndevices){
        printf("Error: DEVICENUM (%d) is greater than the number of available devices (%d)!\n",DEVICENUM,ndevices);
        return 1;
    }
    
    device = devices[DEVICENUM];

    free(devices);

    
    // print out some info on the platform and device
    char *string = malloc(sizeof(char)*STRINGLENGTH);

    ierr = clGetPlatformInfo(platform,CL_PLATFORM_NAME,STRINGLENGTH,string,NULL);
    printf("OpenCL platform: %s\n",string);
    ierr = clGetDeviceInfo(device,CL_DEVICE_NAME,STRINGLENGTH,string,NULL);
    printf("OpenCL device: %s\n",string);

    free(string);


    //create the context with which we communicate to the device
    
    //this is basically an integer array of settings of the form setting name, value, name, value ... 0
    cl_context_properties properties[] = {   
                                            CL_CONTEXT_PLATFORM, 
                                            (cl_context_properties) platform,
                                            0
                                         };
    //create the context, setting the errorCallback function to display any errors
    cl_context context = clCreateContext(properties,1,&device, &errorCallback,NULL,&ierr);

    if (ierr != CL_SUCCESS){
        printf("An error occurred creating the context!\n");
        return 1;
    }

    //create the command queue. Here we set the command_queue_properties to zero - e.g. use defaults
    cl_command_queue queue = clCreateCommandQueue(context,device,0,&ierr);
    if (ierr != CL_SUCCESS){
        printf("An error occurred creating the command queue!\n");
        return 1;
    }


    //create the program
    cl_program program;
    
    // load the program from file and load it in
    {
        size_t proglen;
        char *progstring;
        FILE *f = fopen("mandelbrot.cl","r");
        fseek(f,0,SEEK_END);
        proglen = ftell(f);
        fseek(f,0,SEEK_SET);

        progstring = malloc(sizeof(char)*proglen);

        fread(progstring,1,proglen,f);
        fclose(f);

        program = clCreateProgramWithSource(context,1,(const char**) &progstring,&proglen,&ierr);
        if (ierr != CL_SUCCESS){
            printf("An error occurred with program creation!\n");
            return 0;
        }
        free(progstring);
    }
    
    // compile the program
    ierr = clBuildProgram(program,1,&device,"\0",NULL,NULL);
    if (ierr != CL_SUCCESS){
        printf("An error occurred building the program!\n");
        return 1;
    }
    
    //select the kernel
    cl_kernel kernel = clCreateKernel(program,"mandelbrot",&ierr);
    if (ierr != CL_SUCCESS){
        printf("An error occurred creating the kernel!\n");
        return 1;
    }


    int nx = NX;
    int ny = NY;

    float xmin=XMIN;
    float xmax=XMAX;
    float ymin=YMIN;
    float ymax=YMAX;


    //set up memory
    int *output = malloc(sizeof(int)*nx*ny);
   
    // // tell OpenCL to use the above array as output from the GPU.
    cl_mem outputBuffer = clCreateBuffer(context,CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,sizeof(int)*NX*NY,(void*) output,&ierr);
    if (ierr != CL_SUCCESS){
        printf("An error occurred creating the output buffer!\n");
        return 1;
    }

   

    // //set the kernel arguments 
    ierr = clSetKernelArg(kernel,0,sizeof(cl_mem),(void *) &outputBuffer);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg0 for the kernel!\n");
        return 1;
    }

    ierr = clSetKernelArg(kernel,1,sizeof(float),&xmin);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg1 for the kernel!\n");
        return 1;
    }

    ierr = clSetKernelArg(kernel,2,sizeof(float),&xmax);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg2 for the kernel!\n");
        return 1;
    }

    ierr = clSetKernelArg(kernel,3,sizeof(float),&ymin);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg3 for the kernel!\n");
        return 1;
    }

    ierr = clSetKernelArg(kernel,4,sizeof(float),&ymax);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg4 for the kernel!\n");
        return 1;
    }

    ierr = clSetKernelArg(kernel,5,sizeof(int),&nx);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg5 for the kernel!\n");
        return 1;
    }

    ierr = clSetKernelArg(kernel,6,sizeof(int),&ny);
    if (ierr != CL_SUCCESS){
        printf("An error occurred setting arg6 for the kernel!\n");
        return 1;
    }




    //run kernel

    printf("Executing the kernel... ");

    //event to be associated with the kernel
    cl_event event;
    
    // the size of the work (nx*ny piexes of work)
	size_t global_work_size[] = { NY, NX};
	ierr = clEnqueueNDRangeKernel(queue, 
                                  kernel, 
                                  2, //number of dimensions
                                  NULL, // work offsets (None)
                                  global_work_size, // size of each dim of work
                                  NULL, //local workgroup size
                                  0, //number of tasks this needs to wait on
                                  NULL, //array of these tasks
                                  &event); //event object
    if (ierr != CL_SUCCESS){
        printf("An error occurred enqueueing the task!\n");
        return 1;
    }


    //get results back
    ierr = clEnqueueReadBuffer(queue,outputBuffer,CL_TRUE,0,sizeof(int)*NX*NY,(void *) output,1,&event,NULL);
    if (ierr != CL_SUCCESS){
        printf("An error occurred getting the output buffer!\n");
        return 1;
    }

    printf("Done!\n");
    

    //generate x and y arrays to convert the int image coordinates [i,j] into float x and y values
    float *x = malloc(sizeof(float)*NX);
    float *y = malloc(sizeof(float)*NY);

    for (int i=0;i<NX;i++){
        x[i] = xmin + (xmax-xmin)/NX*i;
    }
    for (int i=0;i<NY;i++){
        y[i] = ymin + (ymax-ymin)/NY*i;
    }


    //write to file
    FILE *f = fopen("out.dat","wb");
    fwrite(&nx,sizeof(int),1,f);
    fwrite(&ny,sizeof(int),1,f);
    fwrite(x,sizeof(int),NX,f);
    fwrite(y,sizeof(int),NY,f);
    fwrite(output,sizeof(int),NX*NY,f);
    fclose(f);

    
    clReleaseKernel(kernel); //Release kernel.
	clReleaseProgram(program); //Release the program object.
	clReleaseMemObject(outputBuffer); //Release the output buffer
    free(output); 
	clReleaseCommandQueue(queue); //Release  Command queue.
	clReleaseContext(context); //Release context.
    
}
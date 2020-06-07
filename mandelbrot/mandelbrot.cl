//Calculates the Mandelbrot set 

//output: out (the image array)
//inputs: xmin, xmax, ymin, ymax - x and y limits of the image
//inputs:  nx, ny number of points in x and y

__kernel void mandelbrot(__global int *out, __private float xmin, __private float xmax, __private float ymin, __private float ymax, __private int nx, __private int ny){
    //coords of thhis kernel instance
    int idx = get_global_id(1);
    int idy = get_global_id(0);
    
    //get the x0 and y0 values
    float x0 = xmin + (xmax-xmin)/nx * idx;
    float y0 = ymin + (ymax-ymin)/ny * idy;

    float x = 0.;
    float y = 0.;

    int n=0;

    // z_(n+1) = z_(n)^2 + (x0 + iy0)

    float z2 = x*x + y*y;

    while(z2 < 100 && n<256){
        //use this temporarily to hold the original x value
        z2 = x;
        
        //update x and y
        // (x+iy)^2 + x0 + iy0 = (x^2 - y^2 + x0) + (2*y*x + y0)i
        x = x*x - y*y + x0;
        y = 2*z2*y + y0;

        z2 = x*x + y*y;
        n+=1;
    }

    out[idx + nx*idy] = n;

}
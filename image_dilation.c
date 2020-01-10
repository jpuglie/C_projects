/*
* Jonathan Pugliese
* Spring 2018
* 
*
* Purpose: Program takes a .ppm image file, reads it in and changes each pixel to either black or white
* then preforms dilation on each pixel of the image.
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename)
{
         //char buff[16];
         PPMImage *img;
         FILE *fp;
	int rgb_comp_color;

         //open PPM file for reading
         fp = fopen(filename, "rb");

         //read image format
         //fgets(buff, sizeof(buff), fp);

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));

    //check for comments
   // c = getc(fp);
    //while (c == '#') {
    //while (getc(fp) != '\n') ;
      //   c = getc(fp);
    //}

    //ungetc(c, fp);

    fscanf(fp, "P6\n");

    //read image size information
    fscanf(fp, "%d %d", &img->x, &img->y);

    //read rgb component
    fscanf(fp, "%d", &rgb_comp_color);

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));


    //read pixel data from file
   fread(img->data, 3*img->x, img->y, fp);
    

    fclose(fp);
    return img;
}
void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}


void dilation(PPMImage *img)
{
    int i,j;
    if(img){

         for(i=0;i<img->x*img->y;i++){
            if(img->data[i].red > 185 || img->data[i].green >185 || img->data[i].blue > 185){
	      img->data[i].red = 255;
              img->data[i].green = 255;
              img->data[i].blue = 255;
	    }
	    else{
      	      img->data[i].red = 0;
              img->data[i].green = 0;
              img->data[i].blue = 0;
	    }
         }
    }
    i = 0;	
    PPMPixel *changed;
    changed = (PPMPixel *)malloc(img->x*img->y *sizeof(PPMPixel));
    if(img){

         for(i=0;i<img->x;i++){
	    for(j=0;j<img->y;j++){

            	if(img->data[i*(img->x) + j].red == 255){
			
			changed[i*(img->x) + j].red = 255;
			changed[i*(img->x) + j].green = 255;
			changed[i*(img->x) + j].blue = 255;

			if(j != 0){	//can change to left
				changed[(i)*(img->x) + (j-1)].red = 255;
				changed[(i)*(img->x) + (j-1)].green = 255;
				changed[(i)*(img->x) + (j-1)].blue = 255;
			}

			if(j != (img->y) - 1){		//can change to the right
				changed[(i)*(img->x) + (j+1)].red = 255;
				changed[(i)*(img->x) + (j+1)].green = 255;
				changed[(i)*(img->x) + (j+1)].blue = 255;					
			}

			if(j > 1){	//can change to left by 2
				changed[(i)*(img->x) + (j-2)].red = 255;
				changed[(i)*(img->x) + (j-2)].green = 255;
				changed[(i)*(img->x) + (j-2)].blue = 255;
			}

			if(j < (img->y) - 2){		//can change to the right by 2
				changed[(i)*(img->x) + (j+2)].red = 255;
				changed[(i)*(img->x) + (j+2)].green = 255;
				changed[(i)*(img->x) + (j+2)].blue = 255;					
			}
			
			if(i != 0){	//can change above
				changed[(i-1)*(img->x) + j].red = 255;
				changed[(i-1)*(img->x) + j].green = 255;
				changed[(i-1)*(img->x) + j].blue = 255;
			
				if(j != 0){	//can change to left
					changed[(i-1)*(img->x) + (j-1)].red = 255;
					changed[(i-1)*(img->x) + (j-1)].green = 255;
					changed[(i-1)*(img->x) + (j-1)].blue = 255;
				}

				if(j != (img->y) - 1){		//can change to the right
					changed[(i-1)*(img->x) + (j+1)].red = 255;
					changed[(i-1)*(img->x) + (j+1)].green = 255;
					changed[(i-1)*(img->x) + (j+1)].blue = 255;					
				}

				
				if(j > 1){	//can change to left by 2
					changed[(i-1)*(img->x) + (j-2)].red = 255;
					changed[(i-1)*(img->x) + (j-2)].green = 255;
					changed[(i-1)*(img->x) + (j-2)].blue = 255;
				}

				if(j < (img->y) - 2){		//can change to the right by 2
					changed[(i-1)*(img->x) + (j+2)].red = 255;
					changed[(i-1)*(img->x) + (j+2)].green = 255;
					changed[(i-1)*(img->x) + (j+2)].blue = 255;					
				}

			}



			if(i > 1){	//can change above up 2
				changed[(i-2)*(img->x) + j].red = 255;
				changed[(i-2)*(img->x) + j].green = 255;
				changed[(i-2)*(img->x) + j].blue = 255;
			
				if(j != 0){	//can change to left up 2
					changed[(i-2)*(img->x) + (j-1)].red = 255;
					changed[(i-2)*(img->x) + (j-1)].green = 255;
					changed[(i-2)*(img->x) + (j-1)].blue = 255;
				}

				if(j != (img->y) - 1){		//can change to the right up 2
					changed[(i-2)*(img->x) + (j+1)].red = 255;
					changed[(i-2)*(img->x) + (j+1)].green = 255;
					changed[(i-2)*(img->x) + (j+1)].blue = 255;					
				}


				if(j > 1){	//can change to left
					changed[(i)*(img->x) + (j-2)].red = 255;
					changed[(i)*(img->x) + (j-2)].green = 255;
					changed[(i)*(img->x) + (j-2)].blue = 255;
				}

				if(j < (img->y) - 2){		//can change to the right
					changed[(i)*(img->x) + (j+2)].red = 255;
					changed[(i)*(img->x) + (j+2)].green = 255;
					changed[(i)*(img->x) + (j+2)].blue = 255;					
				}

			}



			if(i != (img->x)-1){	//can change below
				changed[(i+1)*(img->x) + j].red = 255;
				changed[(i+1)*(img->x) + j].green = 255;
				changed[(i+1)*(img->x) + j].blue = 255;

				if(j != 0){	//can change to left
					changed[(i+1)*(img->x) + (j-1)].red = 255;
					changed[(i+1)*(img->x) + (j-1)].green = 255;
					changed[(i+1)*(img->x) + (j-1)].blue = 255;
				}

				if(j != (img->y) - 1){		//can change to the right
					changed[(i+1)*(img->x) + (j+1)].red = 255;
					changed[(i+1)*(img->x) + (j+1)].green = 255;
					changed[(i+1)*(img->x) + (j+1)].blue = 255;					
				}


				if(j > 1){	//can change to left by 2
					changed[(i+1)*(img->x) + (j-2)].red = 255;
					changed[(i+1)*(img->x) + (j-2)].green = 255;
					changed[(i+1)*(img->x) + (j-2)].blue = 255;
				}

				if(j < (img->y) - 2){		//can change to the right by 2
					changed[(i+1)*(img->x) + (j+2)].red = 255;
					changed[(i+1)*(img->x) + (j+2)].green = 255;
					changed[(i+1)*(img->x) + (j+2)].blue = 255;					
				}
			}


			if(i > (img->x)-2){	//can change below 2
				changed[(i+2)*(img->x) + j].red = 255;
				changed[(i+2)*(img->x) + j].green = 255;
				changed[(i+2)*(img->x) + j].blue = 255;

				if(j != 0){	//can change to left
					changed[(i+2)*(img->x) + (j-1)].red = 255;
					changed[(i+2)*(img->x) + (j-1)].green = 255;
					changed[(i+2)*(img->x) + (j-1)].blue = 255;
				}

				if(j != (img->y) - 1){		//can change to the right
					changed[(i+2)*(img->x) + (j+1)].red = 255;
					changed[(i+2)*(img->x) + (j+1)].green = 255;
					changed[(i+2)*(img->x) + (j+1)].blue = 255;					
				}

				if(j > 1){	//can change to left by 2
					changed[(i+2)*(img->x) + (j-2)].red = 255;
					changed[(i+2)*(img->x) + (j-2)].green = 255;
					changed[(i+2)*(img->x) + (j-2)].blue = 255;
				}

				if(j < (img->y) - 2){		//can change to the right by 2
					changed[(i+2)*(img->x) + (j+2)].red = 255;
					changed[(i+2)*(img->x) + (j+2)].green = 255;
					changed[(i+2)*(img->x) + (j+2)].blue = 255;					
				}
			}






	    	}

	    }

         }
	free(img->data);	
	img->data = changed;
	changed = NULL;
    }
}
int main(int argc, char *argv[]){
    PPMImage *image;
    image = readPPM(argv[1]);
    clock_t start = clock();	
    dilation(image);
    clock_t stop = clock();
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("Time elapsed in ms: %f", elapsed);
    writePPM(argv[1],image);
    return 0;    
}

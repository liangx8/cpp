// LibPNG example
// A.Greensted
// http://www.labbookpages.co.uk

// Version 2.0
// With some minor corrections to Mandlebrot code (thanks to Jan-Oliver)

// Version 1.0 - Initial release

#include <stdio.h>
#include <stdint.h>

#include <locale.h>
#include <math.h>
#include <malloc.h>
#include <png.h>
const float PI = 3.1415926;
// Creates a test image for saving. Creates a Mandelbrot Set fractal of size width x height
void createMandelbrotImage(int width, int height, float xS, float yS, float rad, int maxIteration);
// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int writeImage(char* filename, int width, int height, char* title);
float *buffer;
int g_width;
int g_height;
float g_pixle;
int (*callback_color)(int x,int y);
int sineColor(int x,int y){
	if(x == g_width/2 || y == g_height/2) return 0;
	//if(buffer[x]>=3.0 || buffer[x]<=-3.0)return 0xeaeaea;
	int iy=buffer[x]/g_pixle;
	if(y-g_height/2==iy) return 0x008fff;
	return 0xeaeaea;
}

void createSineImage(int width,int height){
	buffer=(float*)malloc(width*sizeof(float));
	if(buffer==NULL){
		return;
	}
	int half_width=width/2;
	int x;
	g_pixle=2*PI/half_width;
	for(x=0;x<width;x++){
		float fx=2*PI*(x-half_width)/half_width;
		buffer[x]=-9.8*fx*fx;
	}
	g_width=width;
	g_height=height;
	callback_color=sineColor;
}
int xbinColor(int x,int y){
	
	if (buffer[x]>.5)
		return 0x008fff;
	else
		return -1;
}
void xbinImage(char *filename,int width,int height){
	FILE *fp;
	uint8_t buf[512];
	int i,j;
	g_width=width;
	g_height=height;
	buffer=(float*)malloc(width*sizeof(float));
	fp = fopen(filename, "rb");
	if(fp == NULL ){
		fwprintf(stderr, L"Could not open file %s for reading\n", filename);
	}
	fseek(fp,0xc00,SEEK_SET);
	fread(buf,1,512,fp);
	if(fp != NULL) fclose(fp);
	for(i=0;i<512;i++){
		uint8_t v=buf[i];
		for(j=0;j<8;j++){
			if(v & 0x80){
				buffer[i*8+j]=1.0;
			} else {
				buffer[i*8+j]=0.0;
			}
			v = v << 1;
		}
	}
	callback_color=xbinColor;
}
int main(int argc, char *argv[])
{
	setlocale(LC_ALL,"");
	// Make sure that the output filename argument has been provided
	if (argc < 2) {
		fwprintf(stderr, L"Please specify output file\n");
		return 1;
	}

	// Specify an output image size
	const int width = 512*8;
	const int height = 100;

	// Create a test image - in this case a Mandelbrot Set fractal
	// The output is a 1D array of floats, length: width * height
	wprintf(L"创建图像数据\n");
//	createMandelbrotImage(width, height, -0.802, -0.177, 0.011, 110);
//	createSineImage(width,height);
	xbinImage(argv[2],512*8,100);
	if (buffer == NULL) {
		return 1;
	}
	// Save the image to a PNG file
	// The 'title' string is stored as part of the PNG file
	wprintf(L"保存PNG\n");
	int result = writeImage(argv[1], width, height,  "This is my test image");

	// Free up the memorty used to store the image
	free(buffer);

	return result;
}

int get_point(int x,int y){
	int color=buffer[y*g_width+x]*767;
	int offset=color % 256;
	if(color<256){
		return offset;
	}
	if(color<512){
		return offset * 256 + (255-offset);
	}
	return offset * 256 *256 + (255-offset) * 256;
}

int writeImage(char* filename, int width, int height, char* title)
{
	int code = 0;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fwprintf(stderr, L"Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fwprintf(stderr, L"Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fwprintf(stderr, L"Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fwprintf(stderr, L"Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
//			setRGB(&(row[x*3]), buffer[y*width + x]);
			int color=callback_color(x,y);
			row[x*3]=color & 0xff;
			row[x*3+1]=(color >> 8) & 0xff;
			row[x*3+2]=(color >> 16)& 0xff;
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}

void createMandelbrotImage(int width, int height, float xS, float yS, float rad, int maxIteration)
{
	g_width=width;
	buffer = (float *) malloc(width * height * sizeof(float));
	if (buffer == NULL) {
		fwprintf(stderr, L"Could not create image buffer\n");
		return ;
	}

	// Create Mandelbrot set image

	int xPos, yPos;
	float minMu = maxIteration;
	float maxMu = 0;

	for (yPos=0 ; yPos<height ; yPos++)
	{
		float yP = (yS-rad) + (2.0f*rad/height)*yPos;

		for (xPos=0 ; xPos<width ; xPos++)
		{
			float xP = (xS-rad) + (2.0f*rad/width)*xPos;

			int iteration = 0;
			float x = 0;
			float y = 0;

			while (x*x + y*y <= 4 && iteration < maxIteration)
			{
				float tmp = x*x - y*y + xP;
				y = 2*x*y + yP;
				x = tmp;
				iteration++;
			}

			if (iteration < maxIteration) {
				float modZ = sqrt(x*x + y*y);
				float mu = iteration - (log(log(modZ))) / log(2);
				if (mu > maxMu) maxMu = mu;
				if (mu < minMu) minMu = mu;
				buffer[yPos * width + xPos] = mu;
			}
			else {
				buffer[yPos * width + xPos] = 0;
			}
		}
	}

	// Scale buffer values between 0 and 1
	int count = width * height;
	while (count) {
		count --;
		buffer[count] = (buffer[count] - minMu) / (maxMu - minMu);
	}
	callback_color=get_point;
	return ;
}


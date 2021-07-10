
#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 524
#define Y 524
#define BPP 32
#define RGB1  -1
#define RGB2   0
BMP *bmp;

typedef struct _WINDOW{
	int width, height;
	unsigned long start;
}WINDOW;

void drawline(int x1, int y1, int x2, int y2, int rgb, WINDOW *w);
void drawrect(int x, int y, int width, int height, int rgb, WINDOW *w);

void button(WINDOW *w){

	int r = 0xFF;
	int g = 0xFF;
	int b = 0xFF;
	drawrect(100, 100, 200, 40, 0xB0B0B0, w);
	int i = 1;
	for( ; i < 20; i++)
		drawline(100+1, 100+i, 200-2, 1, r << 16 | g << 8 | b, w);

	r -= 0x8 &0xFF;
	g -= 0x8 &0xFF;
	b -= 0x8 &0xFF;
	for( ; i < 40-1; i++)
                drawline(100+1, 100+i, 200-2, 1, r << 16 | g << 8 | b, w);

	drawline(100 + 200, 100+1, 1, 40, 0x101010, w);

	drawline(100 + 1, 100+40, 200, 1, 0x101010, w);
}

int main(int argc, char **argv) {

	WINDOW w;
	w.width = X;
	w.height = Y;

	w.start = (unsigned long) malloc(X*Y*4);

	printf("Programa BMP\n");
	bmp = (BMP*) malloc(sizeof(BMP));
	memset(bmp, 0, sizeof(BMP));

	bmp->bformat.type = 0x4D42; //sig "BM"
	bmp->bformat.size = (X * Y * (BPP/8)) + 54;
	bmp->bformat.offset_bits = 54; // para truecolor

	bmp->binfo.height = Y;
	bmp->binfo.width  = X;

	bmp->binfo.count = BPP;
	bmp->binfo.size = 40;
	bmp->binfo.planes = 1;

	bmp->binfo.size_imag = bmp->bformat.size\
	- bmp->bformat.offset_bits;

	unsigned int *vmem = (unsigned int*) w.start;
	for(int i=0; i < (X*Y); i ++)
		vmem[i] =  0xFFFFFF;
	int rgb;



	button(&w);

	FILE *f = fopen("test.bmp","w+b");
	if(!f) {
		printf("fopen(...) error\n");

	} else {

		fwrite(bmp, 1, 54, f);

		for(int y=bmp->binfo.height; y > 0; y--) {
		for(int x= 0; x < bmp->binfo.width; x++) {

			rgb = vmem[x + (y*bmp->binfo.width)];
			fwrite(&rgb, 1,
			bmp->binfo.count/8, f);

		}}

		fclose(f);
	}


	free(bmp);
	return 0;
}

void put_pixel(long x, long y, unsigned int scanline,unsigned int color, unsigned long addr)
{

        unsigned int *buf = (unsigned int*) addr;

        *(unsigned int*)((unsigned int*)buf + (scanline * y) + x) = color;

}

void drawline(int x1, int y1, int x2, int y2, int rgb, WINDOW *w)
{

        for(int y=0; y < y2; y++ ) {
                for(int x=0; x < x2; x++) {
                        put_pixel(x1+x, y1+y, w->width, rgb, w->start);
                }
        }
}


void drawrect(int x, int y, int width, int height, int rgb, WINDOW *w)
{

        if(width <= 0 || height <=  0 ) return;

        for(int i=0; i < width; i++) put_pixel(x+i, y, w->width, rgb, w->start);

        for(int i=0; i < width; i++) put_pixel(x+i, y + height-1, w->width, rgb, w->start);


        for(int i=0; i < height; i++) put_pixel(x, y+i, w->width, rgb, w->start);

        for(int i=0; i < height; i++) put_pixel(x + width -1, y+i, w->width, rgb, w->start);


}

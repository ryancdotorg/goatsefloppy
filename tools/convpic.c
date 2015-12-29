// convpic - converts and scales jpeg images to goatsefloppy raw 320x200x8 format
// probably doesn't work if the source image is smaller than the destination

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jpeglib.h>

#define ADD_DIFF(dst,err) \
	tmp=(int)(dst)+(int)(err); \
	(dst)=tmp; 
	

int WritePalette(char *fn,int palette[][3])
{
	FILE *f=fopen(fn,"w");
	
	if (f==NULL) return 0;
	
	int i;
	
	fprintf(f,";\n; Palette by convpic\n;\n\n");
	
	for (i=0;i<256;i++)
	{
		fprintf(f,"\tdb 0x%02x,0x%02x,0x%02x\n",palette[i][0]>>2,palette[i][1]>>2,palette[i][2]>>2);	
	}
	
	fclose(f);
	
	return 1;
}

int Diff(int r1,int g1,int b1,int r2,int g2,int b2)
{
	return abs(r1-r2)+abs(g1-g2)+abs(b1-b2);
}

unsigned char GetColorIndex(int palette[][3],int r,int g, int b)
{
	int best=-1,c=0;
	int i;
	
	for (i=0;i<256;i++)
	{
		if (Diff(r,g,b,palette[i][0],palette[i][1],palette[i][2])<c || best==-1) 
		{	
			best=i;
			c=Diff(r,g,b,palette[best][0],palette[best][1],palette[best][2]);
		}
	}
	
	
	
	return best;
}

int ConvertJPEG(char *srcfn,char *dstfn,char *palfn)
{
	FILE *srcdata=fopen(srcfn,"rb");
	
	if (srcdata==NULL)
	{
		printf("Can't open '%s'.\n",srcfn);
		return 0;	
	}
	
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	memset(&cinfo,0,sizeof(struct jpeg_decompress_struct));
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,srcdata);
	jpeg_read_header(&cinfo,TRUE);
	
	
	cinfo.quantize_colors = TRUE; /* Tell the library to quantize */
  	cinfo.dither_mode = JDITHER_FS; /* or _ORDERED, _NONE */
  	cinfo.enable_external_quant = TRUE; /* Supplying color map */
  	cinfo.actual_number_of_colors = 256;
  	cinfo.out_color_components = 3;
  	cinfo.out_color_space = JCS_RGB; 
	
  	jpeg_start_decompress(&cinfo);
	
	
	
	int row_stride = cinfo.output_width * cinfo.output_components;
  	/* Make a one-row-high sample array that will go away when done with image */
  	JSAMPARRAY ImageRow = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	
	int x,y;
	for (y=0;y<cinfo.output_height;y++)
	{	
		if (jpeg_read_scanlines(&cinfo,ImageRow,1)!=1) 
		{
			printf("Premature end of JPEG data.\n");
			return 0;
		}
	}
	
	int palette[256][3];
	for (x=0;x<256;x++)
	{
		palette[x][0]=cinfo.colormap[0][x];
		palette[x][1]=cinfo.colormap[1][x];
		palette[x][2]=cinfo.colormap[2][x];
	}
	
	jpeg_finish_decompress(&(cinfo));
	jpeg_destroy_decompress(&(cinfo));	
	fclose(srcdata);
	srcdata=fopen(srcfn,"rb");
	
	memset(&cinfo,0,sizeof(struct jpeg_decompress_struct));
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,srcdata);
	jpeg_read_header(&cinfo,TRUE);
		 
	
	
	jpeg_start_decompress(&cinfo);
		
	unsigned char *dest=(char*)malloc(sizeof(unsigned char)*320*cinfo.output_height*3);
	
	if (dest==NULL)
	{
		printf("Can't alloc buffer.\n");	
		
		jpeg_finish_decompress(&(cinfo));	
		jpeg_destroy_decompress(&(cinfo));
		
		fclose(srcdata);
		
		return 0;
	}
	
	row_stride = cinfo.output_width * cinfo.output_components;
	
	ImageRow = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	
	unsigned char *dptr;

	for (y=0;y<cinfo.output_height;y++)
	{	
		jpeg_read_scanlines(&cinfo,ImageRow,1);
		int dx=0,ldx=0,c=0;
		int r=0,g=0,b=0;
		
		dptr=dest+y*320*3;
		
		for (x=0;x<cinfo.output_width;x++)
		{
			r+=ImageRow[0][x*3];
			g+=ImageRow[0][x*3+1];
			b+=ImageRow[0][x*3+2];
			
			
			dx=(int)ceil(((float)x/(float)cinfo.output_width)*320.0);
			
			c++;
			
			if (dx!=ldx)
			{
				*(dptr++)=(unsigned char)(r/c);
				*(dptr++)=(unsigned char)(g/c);
				*(dptr++)=(unsigned char)(b/c);
				
				r=0;
				g=0;
				b=0;
				
				c=0;
			}
			
			ldx=dx;			
		}	
		
		if (c>0)
		{
			*(dptr++)=(unsigned char)(r/c);
			*(dptr++)=(unsigned char)(g/c);
			*(dptr++)=(unsigned char)(b/c);
		}
	}
	int h=cinfo.output_height;
	
	jpeg_finish_decompress(&(cinfo));	
	jpeg_destroy_decompress(&(cinfo));
	
	fclose(srcdata);
	
	FILE *dstdata=fopen(dstfn,"wb");
	
	if (dstdata==NULL)
	{
		printf("Can't open '%s' for writing.\n",dstfn);
		free(dest);
		return 0;
	}
	
	for (x=0;x<320;x++)
	{
		int dy=0,ldy=0,c=0;
		int r=0,g=0,b=0;
		dptr=dest+x*3;
		for (y=0;y<h;y++)
		{
			r+=dest[(x+y*320)*3];
			g+=dest[(x+y*320)*3+1];
			b+=dest[(x+y*320)*3+2];
			
			dy=(int)ceil(((float)y/(float)h)*200.0);
			
			c++;
			
			if (dy!=ldy)
			{
				*(dptr)=(unsigned char)(r/c);
				*(dptr+1)=(unsigned char)(g/c);
				*(dptr+2)=(unsigned char)(b/c);
				dptr+=320*3;
				
				r=0;
				g=0;
				b=0;
				
				c=0;
			}
			
			ldy=dy;			
		}
		
		if (c>0)
		{
			*(dptr++)=(unsigned char)(r/c);
			*(dptr++)=(unsigned char)(g/c);
			*(dptr++)=(unsigned char)(b/c);
		}
	}
	
	dptr=dest;
	
	int *errorbuffer=(int*)malloc(sizeof(int)*320*2*3);
	memset(errorbuffer,0,sizeof(int)*320*2*3);
	
	for (y=0;y<200;y++)
	{
		memcpy(errorbuffer,errorbuffer+320*3,320*3*sizeof(int));
		memset(errorbuffer+320*3,0,320*3*sizeof(int));
		for (x=0;x<320;x++)
		{
			int r=*(dptr),g=*(dptr+1),b=*(dptr+2);
			unsigned char c=GetColorIndex(palette,r+errorbuffer[x*3+0],g+errorbuffer[x*3+1],b+errorbuffer[x*3+2]);
			
			
			// floyd-steinberg diffusion
			
			int err_r=(r-palette[c][0]);
			int err_g=(g-palette[c][1]);
			int err_b=(b-palette[c][2]);
			int tmp;
			
			
			if (x<319)
			{
				ADD_DIFF(errorbuffer[x*3],err_r*7/16);
				ADD_DIFF(errorbuffer[x*3+1],err_g*7/16);
				ADD_DIFF(errorbuffer[x*3+2],err_b*7/16);
			
				ADD_DIFF(errorbuffer[x*3+320*3+3],err_r*1/16);
				ADD_DIFF(errorbuffer[x*3+320*3+4],err_g*1/16);
				ADD_DIFF(errorbuffer[x*3+320*3+5],err_b*1/16);
				
			}
			
			ADD_DIFF(errorbuffer[x*3+320*3+0],err_r*5/16);
			ADD_DIFF(errorbuffer[x*3+320*3+1],err_g*5/16);
			ADD_DIFF(errorbuffer[x*3+320*3+2],err_b*5/16);
				
			if (x>0)
			{	
				ADD_DIFF(errorbuffer[x*3+320*3-3+0],err_r*3/16);
				ADD_DIFF(errorbuffer[x*3+320*3-3+1],err_g*3/16);
				ADD_DIFF(errorbuffer[x*3+320*3-3+2],err_b*3/16);
			}
			
			
			
			if (fwrite(&c,1,1,dstdata)!=1) 
			{
				printf("Error writing file.\n");
				return 0;
			}
			
			dptr+=3;
		}	
		
	}
	
	fclose(dstdata);
	//free(dest);
	if (!WritePalette(palfn,palette)) 
	{
		printf("Error saving palette.\n");
		return 0;
	}
	
	return 1;
}

int main(int argc,char *argv[])
{
	if (argc<4) 
	{
		printf("Too few args.\nUsage: convpic <inputjpeg> <outputraw> <outputpal>\n");
		return 1;	
	}
	
	if (!ConvertJPEG(argv[1],argv[2],argv[3]))
	{
		return 1;
	}	
	
	
	
	return 0;
}

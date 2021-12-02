
#include <stdio.h>
#include <stdlib.h>
#include <c6x.h>


void interpImg(unsigned char *input, int width, int height, unsigned char *Output);

int main()
{

	int width = 256;
	int height = 256;
	int exp_width = 2*width;

	volatile unsigned long long start, end, cycles_example;

	FILE *fIn;				                                    //Input File name
	FILE *fOut;                              		            //Output File name

	int in_size = width*height;
	int op_size = exp_width*height;

	unsigned char *Outbuff = malloc(op_size);
	unsigned char *buffer = malloc(in_size);

	const char *fone = "C:\\Users\\HP\\Downloads\\Image1_256x256.raw";
	const char *ftwo = "C:\\Users\\HP\\Downloads\\op_withintrinsics.raw";

	fIn = fopen(fone,"rb");
	fOut = fopen(ftwo,"wb");

	printf("\nWill debug fread next\n");
	fread(buffer,sizeof(unsigned char),in_size,fIn);		   //read image data into buffer
	printf("Done with fread\n");

	TSCL = 0;   /*Enable timer by writing any value on the TSC register*/
	start = _itoll(TSCH, TSCL);   /*read 64-bit TSC register with Upper & lower words*/

	interpImg(buffer, width, height, Outbuff);

	end = _itoll(TSCH, TSCL);    /*read 64-bit TSC register with Upper & lower words*/
	cycles_example = end - start;

	printf("Value is %d\n",cycles_example);

	fwrite(Outbuff,sizeof(unsigned char),op_size,fOut);	       //write interpolated data into fOut

	fclose(fIn);        //Close the input image
	fclose(fOut);       //Close the output image
	free(buffer);
	free(Outbuff);

	return 0;
}

void interpImg(unsigned char *input, int width, int height, unsigned char *Output)
{

	int p = 0;
	int k = 0;
	int i,j;

	int width_out = 2*width;

	unsigned char temp1[256][513];

	//Temporary1 buffer

	printf("Debugging for loop\n");
	for(i = 0; i < height; i=i+1)
	{
		temp1[i][width_out]=0;
		#pragma UNROLL(4);
		for (j = 0; j < width_out; j=j+8)
		{
			temp1[i][j] = input[p];
			temp1[i][j+2] = input[p+1];
			temp1[i][j+4] = input[p+2];
			temp1[i][j+6] = input[p+3];
			//_mem2(&temp1[i][j])=_mem2(&input[p]);
			p = p+4;
		}
		#pragma UNROLL(4);
		for (j = 0; j < width_out-1; j=j+8)
		{

			temp1[i][j+1]=(temp1[i][j]+temp1[i][j+2]+1)/2;
			temp1[i][j+3]=(temp1[i][j+2]+temp1[i][j+4]+1)/2;
			temp1[i][j+5]=(temp1[i][j+4]+temp1[i][j+6]+1)/2;
			temp1[i][j+7]=(temp1[i][j+6]+temp1[i][j+8]+1)/2;
			/*
			unsigned char z[4];
			unsigned int x=_packl4 (_mem4(&temp1[i][j]), _mem4(&temp1[i][j+4]));
			unsigned int y=_packl4 (_mem4(&temp1[i][j+2]), _mem4(&temp1[i][j+6]));
			_mem4(&z)=_add4(x,y);
			temp1[i][j+1]=(z[0]+1)/2;
			temp1[i][j+3]=(z[1]+1)/2;
			temp1[i][j+5]=(z[2]+1)/2;
			temp1[i][j+7]=(z[3]+1)/2;*/
		}

		for (j = 0; j < width_out; j+=1)
		{
			Output[k] = temp1[i][j];
			k+=1;
		}
	}

}



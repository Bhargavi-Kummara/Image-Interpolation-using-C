#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void interpImg(unsigned char *input, int width, int height, unsigned char *Output);

int main()
{

	int width = 256;
	int height = 256;	
	int exp_width = 2*width;							

	FILE *fIn = fopen("Image1_256x256.raw","r");				//Input File name
	FILE *fOut = fopen("output_img1.raw","w");		            //Output File name
	
	int in_size = width*height;								
	int op_size = exp_width*height;

	unsigned char *Outbuff = malloc(op_size);
	unsigned char *buffer = malloc(in_size);							

	fread(buffer,sizeof(unsigned char),in_size,fIn);		   //read image data into buffer

	interpImg(buffer, width, height, Outbuff);

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
	int width_out = 2*width;

	unsigned char temp1[height][width_out];
	unsigned char temp2[height][width_out];

	//Temporary1 buffer
	for(int i = 0; i < height; i++)
	{
		for (int j = 0; j < width_out; j=j+2)
		{
			
			temp1[i][j] = input[p];
				if((j+1) == (width_out-1))
					temp1[i][j+1] = (input[p] + 0 + 1)/2;
				else
					temp1[i][j+1] = (input[p] + input[p+1] + 1)/2;
			Output[k] = temp1[i][j];
			Output[k+1] = temp1[i][j+1];
			k = k+2;
			p = p+1;
		}
	}

/*
	//Temporary2 buffer
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width_out; j++)
		{
			if(j == (width_out-1))
				temp2[i][j] = (temp1[i][j-1] + 0 + 1)/2;
			else if(j%2 == 0)
				temp2[i][j] = temp1[i][j];
			else
				temp2[i][j] = (temp1[i][j-1] + temp1[i][j+1] + 1)/2;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width_out; j++)
		{
			Output[k] = temp1[i][j];
			k++;
		}
	}
*/

}
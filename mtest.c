#include <stdio.h>


int main(){
	char *a[2] = {"/home/jiangs/virusb/digitbox/mp3/azxyq.mp3" , "/home/jiangs/virusb/digitbox/mp3/bdth.mp3"};
	int n = 0;
	char fname[255];
	for(n = 0 ; n < 2 ; n ++)
	{
		sprintf(fname, "madplay %s", a[n]);
		system( fname );	
	}
	printf("over");
	return 0;

}

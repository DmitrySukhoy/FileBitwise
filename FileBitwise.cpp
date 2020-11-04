#include <stdio.h>
using namespace std;
	
	char prgnotice[] =  "FILEBITWISE v1.0\n(c) DmitrySukhoy, 2020\nDistribution and modification of this software is FREE on the condition that the name of the original author is preserved.";
	char helptext[] = "Usage:\tfilebitwise.exe <mode> <input 1> [input 2] <output>\n\nModes:\n! NOT\t# OR\n@ AND\t~ XOR\n\nThe operations are performed on input 1 file using input 2 as the second operand, so it MUST be bigger or same in size.\nFiles larger than 18 exabytes are highly likely to be processed perpetually.";    
	
	char fileopenerror[] = "Error opening one of the specified files.";
	char filesizeerror[] = "Files are invalid size.";
	
	char input1size[] = "Input 1 file size:\t%llu bytes. ";
	char input2size[] = "\nInput 2 file size:\t%llu bytes. ";
	char filenotice[] = "\nPlease note that files larger than 18 exabytes are highly likely to be processed perpetually.";
	
	char opercat = -1;
	
int errorexit(int errcode){
	switch (errcode){
		case -1:
			printf("%s\n%s",prgnotice,helptext);
    		break;
    	case -2:
       		printf("%s\n%s",prgnotice,fileopenerror);
       		break;
       	case -3:
      		printf("%s\n%s",prgnotice,filesizeerror);
       		break;	
    }
    return errcode;
}

int main(int argc, char *argv[]){
	
	if (argv[1] == NULL){return errorexit(-1);} 
	
	switch (argv[1][0]){
		case '!':
			if (argc < 4){return errorexit(-1);}
			opercat = 0;
			break;
		case '#':
		case '@':
		case '~':
			if (argc < 5){return errorexit(-1);}
			opercat = 1;
			break;
		default:
			return errorexit(-1);
	}
	
    FILE * in1f = fopen(argv[2], "rb");
    FILE * in2f;
    
    if (opercat == 1){in2f = fopen(argv[3], "rb");}
    
    FILE * outf = fopen(argv[3+opercat], "wb");
    
    if ((in1f == NULL) || ((in2f == NULL) && (opercat==1)) || (outf == NULL)){
       return errorexit(-2);
    }
	
    fseek(in1f, 0L, SEEK_END);
    unsigned long long in1f_s = ftell(in1f);
    rewind(in1f);

    unsigned long long in2f_s;
    
	if (opercat == 1){
	
    fseek(in2f, 0L, SEEK_END);
    in2f_s = ftell(in2f);
    rewind(in2f);
    
    	if (in2f_s < in1f_s){
       	return errorexit(-3);
    	} else {
		}
	}
				
    unsigned long long t1 = 0;

    printf(input1size, in1f_s);

	if (opercat==0){
		printf(filenotice);
    	while (t1 < in1f_s)
    	{
    		fputc(~(fgetc(in1f)), outf);
    		t1++;
    	}
	} else if (opercat==1){
    	printf(input2size, in2f_s);
		printf(filenotice);
		switch (argv[1][0]){
			case '#':
				while (t1 < in1f_s){
    	  			fputc((fgetc(in1f)|fgetc(in2f)), outf);
    	 		 t1++;
    			}
    		
			case '@':
				while (t1 < in1f_s){
    	  			fputc((fgetc(in1f)&fgetc(in2f)), outf);
    	 		 t1++;
    			}
    		
			case '~':
				while (t1 < in1f_s){
    	  			fputc((fgetc(in1f)^fgetc(in2f)), outf);
    	 		 t1++;
    			}
		}
	}

	printf("Successfully processed %llu bytes.", t1);
	return 1;
}

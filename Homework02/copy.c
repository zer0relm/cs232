/* Author: AJ Vrieland (ajv234)
 * Date: 2/22/2021
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]){
    
    char src[] = "";
    char dest[] = "";
    FILE *iFile;
    FILE *oFile;
    char tempChar = 'i';

    if( argc == 3 ){

        iFile = fopen(argv[1], "r");

        if ( access(argv[2], F_OK) == -1 ){
            oFile = fopen(argv[2], "w");
            while((tempChar = fgetc(iFile)) != EOF){
            fputc(tempChar, oFile);
            }
        } else {
            printf("Error: File already exists \n");
            exit(-1);
        }

        fclose(iFile);
        fclose(oFile);

    } else if ( argc >= 3 ) {
        printf("Error: To many arguments\n");
        exit(-1);
    } else if (argc <= 3 ){
        printf("Error: To few arguments\n");
        exit(-1);
    }

    return 0;
}
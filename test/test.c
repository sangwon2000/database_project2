#include <stdio.h>


int main() {

    FILE *test1 = fopen("test1.txt", "w");
    
    for(int i=1; i<10000; i++) {
        fprintf(test1,"i %d %d\n", i, i);
    }

    fclose(test1);

}
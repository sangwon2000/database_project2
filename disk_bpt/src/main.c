#include "bpt.h"

int tree_height;
int split_count;
int merge_count;

int main(){
    int64_t input;
    char instruction;
    char buf[120];
    char *result;
    open_table("test.db");
    while(scanf("%c", &instruction) != EOF){
        switch(instruction){
            case 'i':
                scanf("%ld %s", &input, buf);
                db_insert(input, buf);
                break;
            case 'f':
                scanf("%ld", &input);
                result = db_find(input);
                if (result) {
                    printf("Key: %ld, Value: %s\n", input, result);
                }
                else
                    printf("Not Exists\n");

                fflush(stdout);
                break;
            case 'd':
                scanf("%ld", &input);
                db_delete(input);
                break;
            case 'q':
                while (getchar() != (int)'\n');
                return EXIT_SUCCESS;
                break;
            case 'c':
                printf("tree_height: %d\n",tree_height);
                printf("split_count: %d\n",split_count);
                printf("merger_count: %d\n",merge_count);
                break;   

        }
        while (getchar() != (int)'\n');
    }
    printf("\n");
    return 0;
}




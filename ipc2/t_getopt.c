#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[]){
    int c;
    

    while( (c = getopt(argc, argv, "a::b:c:d:")) != EOF){
        printf("optchar is %c, optarg is %s, optind is %d\n", c, optarg, optind);
    }
    printf("%d\n", optind);
    

    char *ptr;

    for(int i = 0; (ptr = argv[i]) != NULL; ++i){
        printf("%s ", ptr);
    }
    printf("\n");
}
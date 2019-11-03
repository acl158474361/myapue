#include <grp.h>
 
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	struct group * grp;
	char *name;
	char **ptr;

	if (argc != 2)
		exit(1);
	errno = 0;
	name = argv[1];
	grp = getgrnam(name);
	if (grp != NULL)
		for (ptr = grp->gr_mem; *ptr != NULL; ptr++)
			printf("%s\n", *ptr);
	else if (errno == 0)
		printf("group %s not found\n", name);
	else 
		perror(name);
	return 0;
		



}

#include<stdio.h>
#include<string.h>

int main()
{
	char *src = "gfg";
	char *dst = strdup(src);

	printf("%s",dst);
}

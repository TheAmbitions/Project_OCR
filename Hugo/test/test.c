#include <stdio.h>

int main()
{
	FILE* fichier = NULL;
	int current = 0;

	fichier = fopen("test.txt", "r");

	if (fichier != NULL)
	{
		printf("OK\n");
		do
		{
			current = fgetc(fichier);
			
			
			printf("%d --> %c\n", current, current);
			/*if (current == '.')
				printf("0");
			else
				printf("%c", current);*/

		} while (current != EOF);
		
		fclose(fichier);
	}

	return 0;
}

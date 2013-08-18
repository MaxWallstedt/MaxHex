#include "maxhex.h"

void print_help(char *arg0)
{
	printf("Usage:\n\t$ %s file [/<start address>]\n\n", arg0);
	printf("Valid arguments:\n");
	printf("\t\"-h\" / \"--help\"\t\tDisplay this help message.\n");
	printf("\t\"-v\" / \"--version\"\tDisplay version and copyright information.\n");
	printf("\t\"/<start address>\"\tSpecify the address (in hex, no prefix/suffix)\n\t\t\t\twhere you want to start edit the file.\n");
}

void print_version(void)
{
	printf("%s version %s\n\n", PROGNAME, PROGVER);
	printf("Copyright and GPL and shit.\n");
}

int is_hex(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++) {
		if ((str[i] >= '0' && str[i] <= '9')
		 || (str[i] >= 'a' && str[i] <= 'f')
		 || (str[i] >= 'A' && str[i] <= 'F'))
			;
		else
			return 0;
	}

	return 1;
}

int str_to_hex(char *str)
{
	int i, hex = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] >= '0' && str[i] <= '9')
			hex = hex * 0x10 + str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			hex = hex * 0x10 + str[i] - 'a' + 0x0a;
		else if (str[i] >= 'A' && str[i] <= 'F')
			hex = hex * 0x10 + str[i] - 'A' + 0x0A;
	}

	return hex;
}

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
        char *file = NULL;
	int i, startaddr = 0;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h")
		 || !strcmp(argv[i], "--help")) {
			print_help(argv[0]);

			if (file)
				free(file);

			return 0;
		} else if (!strcmp(argv[i], "-v")
		        || !strcmp(argv[i], "--version")) {
			print_version();

			if (file)
				free(file);

			return 0;
		} else if (*argv[i] == '/') {
			if (!is_hex(argv[i] + 1)) {
				printf("ERROR: \"%s\" is not a valid address\n\n", argv[i] + 1);
				print_help(argv[0]);

				if (file)
					free(file);

				return -1;
			}

			startaddr = str_to_hex(argv[i] + 1);
		} else if (!file) {
			file = malloc(strlen(argv[i]) + 1);
			strcpy(file, argv[i]);
		} else {
			printf("ERROR: %s can only edit one file at the time\n\n", PROGNAME);
			print_help(argv[0]);
			free(file);
			return -1;
		}
	}

	if (!file) {
		printf("ERROR: Must specify file to edit\n\n");
		print_help(argv[0]);
		return -1;
	}

	if (!(fp = fopen(file, "rb"))) {
		printf("ERROR: Could not open file \"%s\"\n\n", file);
		print_help(argv[0]);
		free(file);
		return -1;
	}

	fclose(fp);
	start_maxhex(file, startaddr);
	free(file);

	return 0;
}

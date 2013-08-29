#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "maxhex.h"
#include "intstr.h"

void print_help(char *arg0)
{
	printf("Usage:\n\t$ %s <file> [options]\n\n", arg0);
	printf("Valid options:\n");
	printf("\t-h / --help\t\t\tDisplay this help message.\n\n");
	printf("\t-v / --version\t\t\tDisplay version and copyright\n");
	printf("\t\t\t\t\tinformation.\n\n");
	printf(
	  "\t-a / --address <address>\tSpecify the address (in hex, dec,\n"
		);
	printf("\t\t\t\t\toct or bin) where you want to start\n");
	printf("\t\t\t\t\tview the file.\n\n");
	printf("\t-ro / --readonly\t\tOpen file as read-only\n");
}

void print_version(void)
{
	printf("%s version %s\n\n", PROGNAME, PROGVER);
	printf("Copyright and GPL and shit.\n");
}

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
        char *filename = NULL;
	int i;
	char ro = 0;
	unsigned long long startaddress = 0;

	setlocale(LC_ALL, "");

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h")
		 || !strcmp(argv[i], "--help")) {
			print_help(argv[0]);

			if (filename)
				free(filename);

			return 0;
		} else if (!strcmp(argv[i], "-v")
		        || !strcmp(argv[i], "--version")) {
			print_version();

			if (filename)
				free(filename);

			return 0;
		} else if (!strcmp(argv[i], "-a")
		        || !strcmp(argv[i], "--address")) {
			i++;

			if (i == argc) {
				printf(
			    "ERROR: You must specify address after %s\n\n",
			    argv[i - 1]
					);
				print_help(argv[0]);

				if (filename)
					free(filename);

				return -1;
			}

			if (is_hex(argv[i])) {
				startaddress = str_to_hex(argv[i]);
			} else if (is_oct(argv[i])) {
				startaddress = str_to_oct(argv[i]);
			} else if (is_dec(argv[i])) {
				startaddress = str_to_dec(argv[i]);
			} else if (is_bin(argv[i])) {
				startaddress = str_to_bin(argv[i]);
			} else {
				printf(
				    "ERROR: %s is not a valid address\n\n",
				    argv[i]
					);
				print_help(argv[0]);

				if (filename)
					free(filename);

				return -1;
			}
		} else if (!strcmp(argv[i], "-ro")
		        || !strcmp(argv[i], "readonly")) {
			ro = 1;
		} else if (!filename) {
			filename = malloc(strlen(argv[i]) + 1);
			strcpy(filename, argv[i]);
		} else {
			printf(
		    "ERROR: %s can only open one file at the time\n\n",
		    PROGNAME
				);
			print_help(argv[0]);
			free(filename);

			return -1;
		}
	}

	if (!filename) {
		printf("ERROR: Must specify file to edit\n\n");
		print_help(argv[0]);

		return -1;
	}

	if (!(fp = fopen(filename, "rb"))) {
		printf("ERROR: Could not open file \"%s\"\n\n", filename);
		print_help(argv[0]);
		free(filename);

		return -1;
	}

	fclose(fp);
	start_maxhex(filename, startaddress, ro);
	free(filename);

	return 0;
}

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "maxhex.h"

char *make_swp(char *file)
{
	char *swp = malloc(strlen(file) + 6);
	int i;

	/* Find last '/' in file path */
	i = strlen(file) - 1;

	while (i > 0 && file[i - 1] != '/')
		i--;

	strncpy(swp, file, i);
	swp[i] = '.';
	strcpy(&swp[i + 1], &file[i]);
	strcpy(&swp[strlen(file) + 1], ".swp");

	return swp;
}

void start_maxhex(char *filename, unsigned long long cursoraddr, char ro)
{
	FILE *file = NULL;
	FILE *tmpfile = NULL;
	unsigned char filebuf[352] = {0};
	unsigned char dumpstr[17] = {0};
	char *tmpfilename = NULL;
	char *cmdstr = NULL;
	int i, j, y, c = 0, cmdc = 0;
	unsigned long long addrcount;
	unsigned long long startaddr = cursoraddr;
	unsigned long long endaddr;
	char eol = 0;	/* 0 == LF, 1 == CRLF */
	int maxy, maxx;

	file = fopen(filename, "rb");

	for (addrcount = 0; c != EOF; addrcount++) {
		c = fgetc(file);

		if (c != EOF)
			endaddr = addrcount;
	}

	fclose(file);

	if (cursoraddr > endaddr) {
		printf(
		"ERROR: File \"%s\" does not contain the address 0x%llX\n",
		filename,
		cursoraddr
			);

		return;
	}

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	meta(stdscr, TRUE);
	curs_set(0);

	getmaxyx(stdscr, maxy, maxx);

	if (!ro) {
		tmpfilename = make_swp(filename);
		tmpfile = fopen(tmpfilename, "wb");

		if (!tmpfile) {
			mvprintw(
			    maxy - 3,
			    0,
			    "File could not be opened as read-write."
				);
			mvprintw(
			    maxy - 2,
			    0,
			    "Do you want to open it as read-only? [y/n]"
				);

			do {
				c = getch();

				if (c == 'Y' || c == 'y') {
					ro = 1;
				} else if (c == 'N' || c == 'n') {
					free(tmpfilename);
					endwin();

					return;
				}
			} while (c != 'Y' && c != 'y');
		} else {
			fclose(tmpfile);
		}
	}

	startaddr -= cursoraddr % 0x10;

	if (maxy % 2 == 0) {
		while (startaddr > 0
		       && (cursoraddr - (cursoraddr % 0x10) - startaddr)
		          / 0x10 < (maxy - 2) / 2 - 1)
			startaddr -= 0x10;
	} else {
		while (startaddr > 0
		       && (cursoraddr - (cursoraddr % 0x10) - startaddr)
		          / 0x10 < (maxy - 2) / 2)
			startaddr -= 0x10;
	}

	mvprintw(0, 0, "     ");

	for (i = 0; i < 16; i++)
		printw("%2X ", i);

	printw("Dump:");

	do {
		y = 1;
		file = fopen(filename, "rb");

		for (i = 0; c != EOF; i++) {
			c = fgetc(file);

			if (c != EOF)
				endaddr = i;
		}

		fclose(file);
		file = fopen(filename, "rb");

		for (i = 0; i < startaddr; i++)
			fgetc(file);

		for (i = 0; i < 352; i++)
			filebuf[i] = fgetc(file);

		fclose(file);

		for (i = 0; i < 352; i++) {
			if (i % 16 == 0) {
				j = 0;
				mvprintw(y, 0, "%03Xx ", (startaddr + i) / 0x10);
			}

			if (i + startaddr == cursoraddr) {
				attron(A_STANDOUT);
				printw("%02X", filebuf[i]);
				attroff(A_STANDOUT);
				printw(" ");
			} else if (startaddr + i > endaddr) {
				printw("   ");
			} else {
				printw("%02X ", filebuf[i]);
			}

			if (startaddr + i > endaddr)
				dumpstr[j] = ' ';
			else if (filebuf[i] >= 0x21 && filebuf[i] <= 0x7E)
				dumpstr[j] = filebuf[i];
			else
				dumpstr[j] = '.';

			j++;

			if ((i + 1) % 16 == 0) {
				printw("%s", dumpstr);

				if (cursoraddr / 0x10 == (startaddr + i) / 0x10) {
					attron(A_STANDOUT);
					mvaddch(y, (cursoraddr % 0x10) + 53, dumpstr[cursoraddr % 0x10]);
					attroff(A_STANDOUT);
				}

				y++;
			}
		}

		mvprintw(23, 32, "Current position: %04X\tFile ends at: %04X", cursoraddr, endaddr);

		refresh();
		c = getch();

		if (c == KEY_UP && cursoraddr > 0x0F)
			cursoraddr -= 0x10;
		else if (c == KEY_DOWN && cursoraddr + 0x10 <= endaddr)
			cursoraddr += 0x10;
		else if (c == KEY_RIGHT && cursoraddr < endaddr)
			cursoraddr++;
		else if (c == KEY_LEFT && cursoraddr > 0)
			cursoraddr--;
		else if (c == ':') {
			curs_set(1);
			mvaddch(23, 0, ':');
			printw("\t\t\t\t");
			move(23, 1);

			do {
				refresh();
				c = getch();

				if (c >= 0x20 && c <= 0x7E) {
					addch(c);
					cmdstr[cmdc] = c;
					cmdc++;
				}
			} while (c != '\n' && c != '\r');

			if (!strcmp(cmdstr, "q")) {
				c = 'q';
			} else {
				for (cmdc = 0; cmdc < 80; cmdc++)
					mvaddch(23, cmdc, ' ');

				mvprintw(23, 0, "ERROR: Unknown command \"%s\"", cmdstr);
			}

			for (cmdc = 0; cmdc < 10; cmdc++)
				cmdstr[cmdc] = '\0';

			cmdc = 0;
			curs_set(0);
		}

		if ((cursoraddr - (cursoraddr % 16) == startaddr) && startaddr)
			startaddr -= 0x10;
		else if ((cursoraddr - (cursoraddr % 16)) == startaddr + 16 * 0x15 && cursoraddr - (cursoraddr % 16) < endaddr - (endaddr % 16))
			startaddr += 0x10;
	} while (c != 'q');

	endwin();

	printf("\n%s\n\n", tmpfilename);
	free(tmpfilename);
}

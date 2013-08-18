#include "maxhex.h"

void start_maxhex(char *filename, int startaddr)
{
	FILE *fp = NULL;
	unsigned char filebuf[352] = {0};
	unsigned char dumpstr[17] = {0};
	int i, j, y, c = 0;
	int cursoraddr = startaddr;
	int endaddr;

	startaddr -= startaddr % 16;
	i = 0;

	while (startaddr > 0 && i < 10) {
		startaddr -= 16;
		i++;
	}

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	meta(stdscr, TRUE);
	curs_set(0);

	mvprintw(0, 0, "     ");

	for (i = 0; i < 16; i++)
		printw("%2X ", i);

	printw("Dump:");

	do {
		y = 1;
		fp = fopen(filename, "rb");

		for (i = 0; c != EOF; i++) {
			c = fgetc(fp);

			if (c != EOF)
				endaddr = i;
		}

		fclose(fp);
		fp = fopen(filename, "rb");

		for (i = 0; i < startaddr; i++)
			fgetc(fp);

		for (i = 0; i < 352; i++)
			filebuf[i] = fgetc(fp);

		fclose(fp);

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

			if (filebuf[i] >= 0x21 && filebuf[i] <= 0x7E)
				dumpstr[j] = filebuf[i];
			else
				dumpstr[j] = '.';

			j++;

			if ((i + 1) % 16 == 0) {
				printw("%s", dumpstr);
				addch('\n');
				y++;
			}
		}

		mvprintw(23, 0, "Press 'q' to quit %s\tCurrent position: %04X\tFile ends at: %04X", PROGNAME, cursoraddr, endaddr);

		refresh();
		c = getchar();

		if (c == 65 && cursoraddr > 0x0F)		  /* Up? */
			cursoraddr -= 0x10;
		else if (c == 66 && cursoraddr + 0x10 <= endaddr) /* Down? */
			cursoraddr += 0x10;
		else if (c == 67 && cursoraddr < endaddr)	  /* Right? */
			cursoraddr++;
		else if (c == 68 && cursoraddr > 0)		  /* Left? */
			cursoraddr--;

		if ((cursoraddr - (cursoraddr % 16) == startaddr) && startaddr)
			startaddr -= 0x10;
		else if ((cursoraddr - (cursoraddr % 16)) == startaddr + 16 * 0x15 && cursoraddr - (cursoraddr % 16) < endaddr - (endaddr % 16))
			startaddr += 0x10;
	} while (c != 'q');

	endwin();
}

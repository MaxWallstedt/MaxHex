#include "maxhex.h"

void start_maxhex(char *filename, int startaddr)
{
	FILE *fp = NULL;
	unsigned char filebuf[352] = {0};
	unsigned char dumpstr[17] = {0};
	char cmdstr[10] = {0};
	int i, j, y, c = 0, cmdc = 0;
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
		c = getchar();

		if (c == 65 && cursoraddr > 0x0F)		  /* Up? */
			cursoraddr -= 0x10;
		else if (c == 66 && cursoraddr + 0x10 <= endaddr) /* Down? */
			cursoraddr += 0x10;
		else if (c == 67 && cursoraddr < endaddr)	  /* Right? */
			cursoraddr++;
		else if (c == 68 && cursoraddr > 0)		  /* Left? */
			cursoraddr--;
		else if (c == ':') {
			curs_set(1);
			mvaddch(23, 0, ':');
			printw("\t\t\t\t");
			move(23, 1);

			do {
				refresh();
				c = getchar();

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

				mvprintw(23, 0, "Unknown command \":%s\"", cmdstr);
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
}

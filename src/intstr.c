#include <string.h>

char is_hex(char *str)
{
	int i = 0;
	int len = strlen(str);

	if (!strncmp(str, "0x", 2) || !strncmp(str, "0X", 2))
		i += 2;
	else if (*(str + len - 1) == 'h' || *(str + len - 1) == 'H')
		len--;
	else
		return 0;

	for ( ; i < len; i++) {
		if (!((str[i] >= '0' && str[i] <= '9')
		 || (str[i] >= 'a' && str[i] <= 'f')
		 || (str[i] >= 'A' && str[i] <= 'F')))
			return 0;
	}

	return 1;
}

char is_dec(char *str)
{
	int i = 0;
	int len = strlen(str);

	if (*(str + len - 1) == 'd' || *(str + len - 1) == 'D')
		len--;

	for ( ; i < len; i++) {
		if (str[i] < '0' || str[i] > '9')
			return 0;
	}

	return 1;
}

char is_oct(char *str)
{
	int i = 0;
	int len = strlen(str);

	if (str[0] == '0')
		i++;
	else if (*(str + len - 1) == 'o' || *(str + len - 1) == 'O')
		len--;
	else
		return 0;

	for ( ; i < len; i++) {
		if (str[i] < '0' || str[i] > '7')
			return 0;
	}

	return 1;
}

char is_bin(char *str)
{
	int i = 0;
	int len = strlen(str);

	if (*(str + len - 1) == 'b' || *(str + len - 1) == 'B')
		len--;

	for ( ; i < len; i++) {
		if (str[i] != '0' && str[i] != '1')
			return 0;
	}

	return 1;
}

unsigned long long str_to_hex(char *str)
{
	int i;
	unsigned long long hex = 0;

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

unsigned long long str_to_dec(char *str)
{
	int i;
	unsigned long long dec = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] >= '0' && str[i] <= '9')
			dec = dec * 10 + str[i] - '0';
	}

	return dec;
}

unsigned long long str_to_oct(char *str)
{
	int i;
	unsigned long long oct = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] >= '0' && str[i] <= '7')
			oct = oct * 010 + str[i] - '0';
	}

	return oct;
}

unsigned long long str_to_bin(char *str)
{
	int i;
	unsigned long long bin = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '0' || str[i] == '1')
			bin = bin * 2 + str[i] - '0';
	}

	return bin;
}

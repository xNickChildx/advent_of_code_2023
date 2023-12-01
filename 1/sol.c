#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct string_to_num {
	char *str;
	char num;
};
static struct string_to_num string_dict [] = {
	{.str = "one", .num = '1'},
	{.str = "two", .num = '2'},
	{.str = "three", .num = '3'},
	{.str = "four", .num = '4'},
	{.str = "five", .num = '5'},
	{.str = "six", .num = '6'},
	{.str = "seven", .num = '7'},
	{.str = "eight", .num = '8'},
	{.str = "nine", .num = '9'},
};
static int try_num(const char *str, int max_len) {	
	// part 1
	if (str[0] >= '0' && str[0] <= '9')
		return str[0];
	// part 2
	for (int i = 0; i < sizeof(string_dict) / sizeof(*string_dict); i++) {
		if (max_len < strlen(string_dict[i].str))
			continue;
		if (!memcmp(string_dict[i].str, str,
			    strlen(string_dict[i].str)))
			return string_dict[i].num;
	}

	return 'X';

}


static size_t calc_calib_value(const char *line, size_t line_len) {
	char str_num[] = "XX"; // X works as "not set flag"
	// single pass through
	for (int i = 0; i < line_len; i++) {
		int i_end = line_len - (i+1);
		if (str_num[0] != 'X' && str_num[1] != 'X')
		       goto exit_early;	
		// parse from start
		if (str_num[0] == 'X')
			str_num[0] = try_num(line + i, line_len - i);

		// parse from end
		if (str_num[1] == 'X') 
		       	str_num[1] = try_num(line + i_end, line_len - i_end);
	}
	// assume always 1 digit
	if (str_num[0] == 'X')
		str_num[0] = str_num[1];
	if (str_num[1] == 'X')
                str_num[1] = str_num[0];
//	printf("str %s, is %s or %d\n", line, str_num, atoi(str_num));
	if (str_num[1] == 'X' && str_num[0] == 'X')
		return 0;
exit_early:
	return atoi(str_num);
}

int main(int argc, char ** argv) {
	FILE *file;
	char *line = NULL;
	size_t line_alloc_len = 0, sum = 0, line_sum;
	int line_len;

	if (argc < 2) {
		printf ("USAGE: %s [filename]\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");
	if (!file) {
		printf("Cannot read %s\n", argv[1]);
		return -1;
	}

	for (; (line_len = getline(&line, &line_alloc_len, file)) > 0; sum += line_sum)
		line_sum = calc_calib_value(line, line_len - 1);	

	printf("CALIBRATION VALUE %lu\n", sum);
	if (line)
		free(line);
	fclose(file);
	return 0;
}
	

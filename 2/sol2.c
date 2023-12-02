#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

enum color_idx {
	red = 0,
	green,
	blue
};

/* return the next number in a game
 * return number of bytes read
 */
static size_t get_next_num(const char* str, int *num_out) {
	size_t bytes_read = 0;
	
	while (str[bytes_read] < '0' || str[bytes_read] > '9')
		bytes_read++;
	
	*num_out = atoi(str + bytes_read);
	while (str[bytes_read] >= '0' && str[bytes_read] <= '9')
		bytes_read++;
	
	return bytes_read;
}

static size_t get_color_idx(const char *ptr, enum color_idx *idx) {
	if (!memcmp(" red", ptr, strlen(" red"))) {
			*idx = red;
			return strlen(" red");
	}
	if (!memcmp(" blue", ptr, strlen(" blue"))) {
                        *idx = blue;
                        return strlen(" blue");
        }
        *idx = green;
        return strlen(" green");
}

	

static size_t get_handful(const char *ptr, int *colors) {
	size_t bytes_read = 0;
	int num;
	enum color_idx color_idx;
	while (ptr[bytes_read] != '\n' && ptr[bytes_read] != ';') {
		bytes_read += get_next_num(ptr + bytes_read, &num);
		bytes_read += get_color_idx(ptr + bytes_read, &color_idx);
		if (num > colors[color_idx])
			colors[color_idx] = num;
	}
	return bytes_read + 1;
}

static int get_min_sum(const char *str, int line_len) {
	// {#red, #green, #blue}
	int game_id, len_left = line_len, color_count[3] = {0};
	const char *str_ptr = str;
	size_t bytes_read;

	// first get game id, update str_ptr
	str_ptr += get_next_num(str_ptr, &game_id);

	while (str_ptr < str + line_len) {
		str_ptr += get_handful(str_ptr, color_count);		
	}

	
	return color_count[red] * color_count[blue] * color_count[green];
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
		line_sum = get_min_sum(line, line_len - 1);	

	printf("GAME ID SUM %lu\n", sum);
	if (line)
		free(line);
	fclose(file);
	return 0;
}
	

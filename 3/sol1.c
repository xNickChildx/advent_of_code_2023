#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* return the next number in a game
 * return number of bytes read
 */
static size_t get_next_num(const char* str, int *num_out, int *int_len) {
	size_t bytes_read = 0;
	
	*int_len = 0;	
	while (str[bytes_read] < '0' || str[bytes_read] > '9') {
		if (str[bytes_read] == '\n') {
			*num_out = 0;
			return bytes_read;
		}
		bytes_read++;
	}
	*num_out = atoi(str + bytes_read);
	while (str[bytes_read] >= '0' && str[bytes_read] <= '9') {
		*int_len = *int_len + 1;
		bytes_read++;
	}
	
	return bytes_read;
}

static int is_sym(char c) {
	return c != '\n' && c != '.' &&  (c < '0' || c > '9');
}

static int parse_line(const char *curr, const char *prev, const char *next,
	       	      int line_len) {
	const char *str_ptr = curr;
	int sum = 0, num, int_len, sym_search_start, sym_search_end;
	if (!curr)
		return 0;

	while (str_ptr < curr + line_len) {
		str_ptr += get_next_num(str_ptr, &num, &int_len);
		if (!num)
			continue;
		sym_search_start = str_ptr - int_len - curr;
		if (sym_search_start > 0)
			sym_search_start--;

		sym_search_end = str_ptr - curr;
		for (int i = sym_search_start; i <= sym_search_end; i++)
			if (   (prev && is_sym(prev[i]))
		            || is_sym(curr[i]) 
			    || (next && is_sym(next[i]))) {
				sum += num;
				break;
			}
				
	}
	return sum;
}

int main(int argc, char ** argv) {
	FILE *file;
	char *line = NULL, *prev_line = NULL, *next_line = NULL;
	size_t line_alloc_len = 0, sum = 0, line_sum;
	int line_len, line_len_last;

	if (argc < 2) {
		printf ("USAGE: %s [filename]\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");
	if (!file) {
		printf("Cannot read %s\n", argv[1]);
		return -1;
	}

	for (; (line_len = getline(&next_line, &line_alloc_len, file)) > 0; sum += line_sum) {
		line_sum = parse_line(line, prev_line, next_line, line_len - 1);
		free(prev_line);
		prev_line = line;
		line = next_line;
		next_line = NULL;
		line_len_last = line_len;
	}
	sum  += parse_line(line, prev_line, NULL, line_len_last - 1);

	printf("ENGINE SUM %lu\n", sum);

	free(line);
	free(prev_line);
	free(next_line);
	fclose(file);
	return 0;
}
	

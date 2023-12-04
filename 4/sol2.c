#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct winning_nums {
	int num;
	struct winning_nums *next;
};

typedef struct winning_nums game_counts;
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

static void add_win_num(struct winning_nums **head, int num) {
	struct winning_nums *tmp = *head, *new = NULL;

	new = malloc(sizeof(*new));
	new->num = num;
	new->next = NULL;
	if (!*head) {
		*head = new;
		return;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;

	tmp->next = new;
}

static void add_copy (game_counts **tail) {
	add_win_num(tail, 1);
}

static void add_copies(game_counts **head, int count) {
	game_counts *tmp, *tmp_prev = NULL;

	if (!count)
		return;
	if (!*head) {
		add_copy(head);
		(*head)->num = 0;
	}
	tmp = *head;
	for (int i = 0; i < count; i++) {
		if (!tmp) {
			add_copy(&tmp_prev);
			if (!tmp_prev->next)
				tmp = tmp_prev;
			else
				tmp = tmp_prev->next;
		}
		else {
			tmp->num++;
		}
		tmp_prev = tmp;
		tmp = tmp->next;
	}
}

static int is_winner(struct winning_nums *head, int num) {
	struct winning_nums *tmp = head;
	int sum = 0;
	for (; tmp ; tmp = tmp->next)
		if (tmp->num == num)
			sum++;

	return sum;
}

static void remove_head(game_counts **head) {
	game_counts *tmp = *head;
	if (!*head)
		return;
	*head = (*head)->next;
	free(tmp);
}

static void free_win_nums(struct winning_nums **head) {
	if (*head && (*head)->next)
		free_win_nums(&(*head)->next);
	free(*head);
}

static int winning_numbers(const char *str, int line_len) {
	const char *str_ptr = str;
	size_t bytes_read;
	int num, in_winning = 1, winners = 0, sum = 0;
	struct winning_nums *head = NULL;
	// iterate to start of winning numbers
	while (str_ptr[0] != ':')
		str_ptr++;

	while (str_ptr < str + line_len) {
		if (str_ptr[1] == '|')
			in_winning = 0;
		str_ptr += get_next_num(str_ptr, &num);
		if (in_winning)
			add_win_num(&head, num);
		else
			sum += is_winner(head, num);
	}

	free_win_nums(&head);

	
	return sum;
}

int main(int argc, char ** argv) {
	FILE *file;
	char *line = NULL;
	size_t line_alloc_len = 0, sum = 0, line_sum;
	int line_len, game_id = 0, copies, this_copies;
	game_counts *head = NULL;

	if (argc < 2) {
		printf ("USAGE: %s [filename]\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");
	if (!file) {
		printf("Cannot read %s\n", argv[1]);
		return -1;
	}

	for (; (line_len = getline(&line, &line_alloc_len, file)) > 0; sum += line_sum, game_id++) {
		copies = winning_numbers(line, line_len - 1);
		if (head)
			this_copies = head->num;
		else
			this_copies = 0;
		remove_head(&head);
		line_sum = copies * (this_copies + 1) + 1;
		for (int i = 0; i <= this_copies; i++)
			add_copies(&head, copies);

	}

	printf("COPIES # SUM %lu\n", sum);

	free_win_nums(&head);
	if (line)
		free(line);
	fclose(file);
	return 0;
}
	

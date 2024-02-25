#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct list_s {
    char *str;
    unsigned int len;
    struct list_s *next;
} list_t;
////////////
size_t print_list(const list_t *h)
{
	size_t node = 0;

	while (h)
	{
		if (h->str == NULL)
			printf("[0] (nil)\n");
		else
			printf("[%d] %s\n", h->len, h->str);

		node++;
		h = h->next;
	}
	return (node);
}
///////////



list_t *add_node_end(list_t **head, const char *str){
    if (str == NULL) {
        return NULL;
    }
    
    list_t *new = malloc(sizeof(list_t));
    if (new == NULL) {
        return NULL;
    }
    
    new->str = strdup(str);
    if (new->str == NULL){
        free(new);
        return NULL;
    }
    new->len = strlen(str);
    new->next = NULL;
    
    if (*head == NULL) {
        *head = new;
    } else {
        list_t *last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new;
    }
    
    return new;
}


void free_list(list_t *head) {
    while (head != NULL) {
        list_t *temp = head;
        head = head->next;
        free(temp->str);
        free(temp);
    }
}


void print_message(void)__attribute__((constructor));

void print_message(void) {
    printf("You're beat! and yet, you must allow,\n"
        "I bore my house upon my back!\n");
}

int main(void)
{
    printf("(A tortoise, having pretty good sense of a hare's nature, challenges one to a race.)\n");
    return (0);
}


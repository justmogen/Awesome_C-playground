#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Size of the hash table */
#define CAPACITY 50000

unsigned long hash_function(char *str)
{
    unsigned long i = 0;

    for (int j=0; str[j]; j++)
        i += str[j];

    return i % CAPACITY;
}

// Define the hashTable item
typedef struct Htb_item
{
    char *key;
    char *value;
} Htb_item;

// Define the LinkedList
typedef struct LinkedList
{
    Htb_item *item;
    struct LinkedList *next;
} LinkedList;

// Define the hashTable
typedef struct HashTable
{
    // Array of pointers to the LinkedList
    Htb_item **items;
    LinkedList **overflow_buckets;
    int size;
    int count;
} HashTable;

LinkedList *allocate_list()
{
    // Allocate memory for a LinkedList pointer
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (list != NULL)
        list->next = NULL;
    return list;
}

LinkedList *linkedlist_insert(LinkedList *list, Htb_item *item)
{
    // Inserts the item onto the LinkedList
    if (!list)
    {
        LinkedList *head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }
    else if (list->next == NULL)
    {
        LinkedList *node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }
    else
    {
        return NULL;
    }

    LinkedList *temp = list;

    while (temp->next->next)
    {
        temp = temp->next;
    }

    LinkedList *node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;
}

Htb_item *linkedlist_remove(LinkedList *list)
{
    // Removes the head of the LinkedList
    // and returns the item removed

    if (list == NULL || list->item == NULL)
        return NULL;

    LinkedList *node = list->next;
    LinkedList *temp = list;
    temp->next = NULL;
    list = node;
    Htb_item *myItem = list->item;
    memcpy(temp->item, myItem, sizeof(Htb_item));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item);
    free(temp);
    return myItem;
}

void free_linkedlist(LinkedList *list)
{
    // Free the LinkedList
    if (!list)
        return;

    LinkedList *temp;
    while (list)
    {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

LinkedList **create_overflow_buckets(HashTable *table)
{
    // Create the overflow buckets - an array of linked lists
    LinkedList **buckets = (LinkedList **)calloc(table->size, sizeof(LinkedList *));

    for (int i = 0; i < table->size; i++)
    {
        buckets[i] = NULL;
    }
    return buckets;
}

void free_overflow_buckets(HashTable *table)
{
    // Free all the overflow bucket lists
    LinkedList **buckets = table->overflow_buckets;

    for (int i = 0; i < table->size; i++)
    {
        free_linkedlist(buckets[i]);
    }
    free (buckets);
}
Htb_item *create_item(char *key, char *value)
{
    // Creates a pointer to a new HashTable item
    Htb_item *item = (Htb_item *)malloc(sizeof(Htb_item));
    item->key = (char *)malloc(strlen(key) + 1);
    item->value = (char *)malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

HashTable *create_table(int size)
{
    // Create a new HashTable with the size given
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Htb_item **)calloc(table->size, sizeof(Htb_item *));

    for (int i = 0; i < table->size; i++)
    {
        table->items[i] = NULL;
    }
    table->overflow_buckets = create_overflow_buckets(table);
    return table;
}

void free_item (Htb_item *item)
{
    // Free the item
    free(item->key);
    free(item->value);
    free(item);
}

void free_table(HashTable *table)
{
    // Free the table
    for (int i = 0; i < table->size; i++)
    {
        Htb_item *item = table->items[i];
        if (item != NULL)
            free_item(item);
    }
    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable *table, unsigned long index, Htb_item *item)
{
    // Handles collisions by adding the item to the overflow bucket
    LinkedList *head = table->overflow_buckets[index];

    if (head == NULL)
    {
        // Create te list
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else
    {
        // Insert to the list
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return; 
    }
}

void ht_insert(HashTable *table, char *key, char *value)
{
    // Creates the item
    Htb_item *item = create_item(key, value);

    // Get the index
    int index = hash_function(key);

    Htb_item *current_item = table->items[index];

    if (current_item == NULL)
    {
        // Key does not exist
        if (table->count == table->size)
        {
            // Hash Table is full
            printf("Insert Error: Hash Table is full\n");
            free_item(item);
            return;
        }

        // Insert directly
        table->items[index] = item;
        table->count++;
    }
    else
    {
        // Scenario 1: We only need to update value
        if (strcmp(current_item->key, key) == 0 )
        {
            strcpy(table->items[index]->value, value);
            return;
        }
        else
        {
            // Scenario 2: Collision
            handle_collision(table, index, item);
            return;
        }
    }
}

char *ht_search(HashTable *table, char *key)
{
    // Searches the key in the table
    // Returns the value (if found) or NULL (if not found)

    if (table == NULL || table->overflow_buckets == NULL)
        return NULL;
    
    int index = hash_function(key);
    Htb_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    // Give only non-NULL item
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->value;

        if (head == NULL)
            return NULL;

        item = head->item;
        head = head->next;
    }

    return NULL;
}

void ht_delete(HashTable *table, char *key)
{
    // Deletes the key from the table
    int index = hash_function(key);
    Htb_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    if (item == NULL)
    {
        // Does not exist
        return;
    }
    else
    {
        if (head == NULL && strcmp(item->key, key) == 0)
        {
            // Collision chain is empty
            // Remove the item & set table index to NULL
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            // Collision chain is not empty 
            // Search for the item in the chain
            if (strcmp(item->key, key) == 0)
            {
                // Remove the item & set head of the list as the new item
                free_item(item);
                LinkedList *node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }
            LinkedList *curr = head;
            LinkedList *prev = NULL;

            while (curr)
            {
                if (strcmp(curr->item->key,  key) == 0 )
                {
                    if (prev == NULL)
                    {
                        // First element of the chain
                        // Remove the chain
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else
                    {
                        // This is somewhere in the chain
                        // Remove the item from the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }

                curr = curr->next;
                prev = curr;
            }

        }
    }
}
void print_search(HashTable *table, char *key)
{
    char *val;

    if ((val = ht_search(table, key)) == NULL)
    {
        printf("Key: %s does not exist\n", key);
        return;
    }
    else
    {
        printf("Key: %s, Value: %s\n", key, val);
    }
}

void print_table(HashTable *table)
{
    printf("\nHash Table\n-------------------\n");
    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i])
        {
            printf("Index: %d, Key: %s, Value: %s\n", i, table->items[i]->key, table->items[i]->value);
        }
    }

    printf("--------------------\n\n");
}

int main()
{
    HashTable *ht = create_table(CAPACITY);
    ht_insert(ht, (char *)"1", (char *)"First address");
    ht_insert(ht, (char *)"2", (char *)"Second address");
    ht_insert(ht, (char *)"3", (char *)"Third address");
    ht_insert(ht, (char *)"4", (char *)"Fourth address");
    ht_insert(ht, (char *)"5", (char *)"Fifth address");
    ht_insert(ht, (char *)"6", (char *)"Sixth address");
    print_search(ht, (char *)"1");
    print_search(ht, (char *)"2");
    print_search(ht, (char *)"3");
    print_search(ht, (char *)"4");
    print_search(ht, (char *)"Hel");
    print_search(ht, (char *)"Cau"); // This is a Collision!
    print_table(ht);
    ht_delete(ht, (char *)"2");
    ht_delete(ht, (char *)"3");
    ht_delete(ht, (char *)"Hel");
    print_table(ht);
    free_table(ht);
    return 0;
}
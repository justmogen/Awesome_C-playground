#include <stdio.h>
#include "hash_tables.h"


/**
 * shash_table_create - creates a sorted hash table
 * @size: size of the array
 * Return: pointer to the newly created sorted hash table
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *sht;
	unsigned long int i;

	sht = malloc(sizeof(shash_table_t));
	if (sht == NULL)
		return (NULL);
	sht->size = size;
	sht->array = malloc(sizeof(shash_node_t *) * size);
	if (sht->array == NULL)
	{
		free(sht);
		return (NULL);
	}
	for (i = 0; i < size; i++)
		sht->array[i] = NULL;
	sht->shead = NULL;
	sht->stail = NULL;
	return (sht);
}

/**
 * shash_table_set - adds an element to the sorted hash table
 * @ht: sorted hash table to add or update the key/value to
 * @key: key to add to the sorted hash table
 * @value: value to add to the sorted hash table
 * Return: 1 on success, 0 on failure
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{
	unsigned long int index;
	shash_node_t *new, *tmp;

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);
	index = key_index((const unsigned char *)key, ht->size);
	tmp = ht->shead;
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = strdup(value);
			if (tmp->value == NULL)
				return (0);
			return (1);
		}
		tmp = tmp->snext;
	}
	new = malloc(sizeof(shash_node_t));
	if (new == NULL)
		return (0);
	new->key = strdup(key);
	if (new->key == NULL)
	{
		free(new);
		return (0);
	}
	new->value = strdup(value);
	if (new->value == NULL)
	{
		free(new->key);
		free(new);
		return (0);
	}
	new->next = ht->array[index];
	ht->array[index] = new;
	if (ht->shead == NULL)
	{
		new->sprev = NULL;
		new->snext = NULL;
		ht->shead = new;
		ht->stail = new;
	}
	else if (strcmp(new->key, ht->shead->key) < 0)
	{
		new->sprev = NULL;
		new->snext = ht->shead;
		ht->shead->sprev = new;
		ht->shead = new;
	}
	else
	{
		tmp = ht->shead;
		while (tmp->snext && strcmp(new->key, tmp->snext->key) > 0)
			tmp = tmp->snext;
		new->sprev = tmp;
		new->snext = tmp->snext;
		if (tmp->snext == NULL)
			ht->stail = new;
		else
			tmp->snext->sprev = new;
		tmp->snext = new;
	}
	return (1);
}

/**
 * shash_table_get - retrieves a value associated with a key
 * @ht: sorted hash table to look into
 * @key: key to look for
 * Return: value associated with the element, or NULL if key couldn’t
 * be found
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	unsigned long int index;
	shash_node_t *tmp;

	if (ht == NULL || key == NULL || *key == '\0')
		return (NULL);
	index = key_index((const unsigned char *)key, ht->size);
	tmp = ht->array[index];
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

/**
 * shash_table_print - prints a sorted hash table
 * @ht: sorted hash table to print
 */
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *tmp;

	if (ht == NULL)
		return;
	tmp = ht->shead;
	printf("{");
	while (tmp)
	{
		printf("'%s': '%s'", tmp->key, tmp->value);
		tmp = tmp->snext;
		if (tmp)
			printf(", ");
	}
	printf("}\n");
}

/**
 * shash_table_print_rev - prints a sorted hash table in reverse
 * @ht: sorted hash table to print
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *tmp;

	if (ht == NULL)
		return;
	tmp = ht->stail;
	printf("{");
	while (tmp)
	{
		printf("'%s': '%s'", tmp->key, tmp->value);
		tmp = tmp->sprev;
		if (tmp)
			printf(", ");
	}
	printf("}\n");
}

/**
 * shash_table_delete - deletes a sorted hash table
 * @ht: sorted hash table to delete
 */
void shash_table_delete(shash_table_t *ht)
{
	shash_node_t *tmp, *tmp2;

	if (ht == NULL)
		return;
	tmp = ht->shead;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->snext;
		free(tmp2->key);
		free(tmp2->value);
		free(tmp2);
	}
	free(ht->array);
	free(ht);
}

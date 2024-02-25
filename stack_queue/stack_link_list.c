#include <stdio.h>
#include <stdlib.h>

// Node structure for the linked list
struct Node
{
    int data;
    struct Node *next;
};

// Function prototypes
void push(struct Node **top);
void pop(struct Node **top);
void display(struct Node *top);

int main()
{
    printf("Welcome to DataFlair tutorials!\n\n");

    int choice;
    struct Node *top = NULL; // Initialize top to NULL

    printf("LINKED LIST IMPLEMENTATION USING STACKS\n\n");
    do
    {
        printf("1. Insert\n2. Delete\n3. Display\n4. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            push(&top);
            break;
        case 2:
            pop(&top);
            break;
        case 3:
            display(top);
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("Sorry, invalid choice!\n");
            break;
        }
    } while (choice != 4);

    return 0;
}

void push(struct Node **top)
{
    int data;
    printf("Enter the element to be inserted: ");
    scanf("%d", &data);

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed. Stack overflow.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

void pop(struct Node **top)
{
    if (*top == NULL)
    {
        printf("Stack Underflow. The stack is empty.\n");
        return;
    }

    int item = (*top)->data;
    struct Node *temp = *top;
    *top = (*top)->next;
    free(temp);

    printf("The deleted item is %d\n", item);
}

void display(struct Node *top)
{
    if (top == NULL)
    {
        printf("Stack underflow. The stack is empty.\n");
        return;
    }

    printf("The elements of the stack are:\n");
    while (top != NULL)
    {
        printf("%d\n", top->data);
        top = top->next;
    }
}


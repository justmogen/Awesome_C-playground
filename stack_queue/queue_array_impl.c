#include <stdio.h>
#include <stdlib.h>

#define LIMIT 100 // Specifying the maximum limit of the queue

int queue[LIMIT]; // Array implementation of queue
int front = -1, rear = -1; // To insert and delete the data elements in the queue
int choice; // To choose either of the 3 queue operations

// Function prototypes
void insert();
void delete();
void display();

int main()
{
    printf("Welcome to DataFlair tutorials!\n\n");
    printf("ARRAY IMPLEMENTATION OF QUEUES\n\n");

    do
    {
        printf("1. Insert\n2. Delete\n3. Display\n4. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            insert();
            break;
        case 2:
            delete();
            break;
        case 3:
            display();
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

void insert()
{
    int element;
    if (rear == LIMIT - 1)
        printf("Queue Overflow\n");
    else
    {
        if (front == -1)
            front = 0;

        printf("Enter the element to be inserted in the queue: ");
        scanf("%d", &element);
        rear++;
        queue[rear] = element;
    }
}

void delete()
{
    if (front == -1 || front > rear)
    {
        printf("Queue Underflow \n");
    }
    else
    {
        printf("The deleted element in the queue is: %d\n", queue[front]);
        front++;
    }
}

void display()
{
    int i;
    if (front == -1)
    {
        printf("Queue underflow\n");
    }
    else
    {
        printf("The elements of the queue are:\n");
        for (i = front; i <= rear; i++)
            printf("%d\n", queue[i]);
    }
}


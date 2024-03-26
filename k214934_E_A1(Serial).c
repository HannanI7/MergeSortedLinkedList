//21K-4934 Hannan Irfan CS-E 
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Node
{
    int value;
    struct Node* next;
} Node;

Node* createNode(int value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, int value)
{
    Node* newNode = createNode(value);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

Node* mergeLists(Node* left, Node* right)
{
    if (left == NULL)
    {
        return right;
    }
    if (right == NULL)
    {
        return left;
    }

    Node* result = NULL;
    if (left->value <= right->value)
    {
        result = left;
        result->next = mergeLists(left->next, right);
    }
    else
    {
        result = right;
        result->next = mergeLists(left, right->next);
    }
    return result;
}

void mergeSort(Node** head)
{
    Node* current = *head;
    Node* left;
    Node* right;

    if (current == NULL || current->next == NULL)
    {
        return;
    }

    left = current;
    right = current->next;
    while (right != NULL && right->next != NULL)
    {
        left = left->next;
        right = right->next->next;
    }
    right = left->next;
    left->next = NULL;
    left = *head;

    mergeSort(&left);
    mergeSort(&right);

    *head = mergeLists(left, right);
}

void printLinkedList(Node* head)
{
    Node* current = head;
    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->next;
    }
}

int main()
{
    FILE *fptr;
    int num;
    Node* head = NULL;

    fptr = fopen("numberfile.txt", "r");
    
    clock_t start_time = clock();

    while (fscanf(fptr, "%d", &num) != EOF) 
    {
        insertNode(&head, num);
    }

    fclose(fptr);

    printf("Lisked List values(Before Sorting):\n");
    printLinkedList(head);
    printf("\n");
    
    mergeSort(&head);
    
    clock_t end_time = clock();
    
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Lisked List values(After Sorting):\n");
    printLinkedList(head);
    printf("\n");

    printf("Total runtime (Serial Execution): %.6f seconds\n", total_time);

    return 0;
}


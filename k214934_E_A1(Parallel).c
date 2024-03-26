//21K-4934 Hannan Irfan CS-E
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sched.h>
#include<time.h>

#define NUM_THREADS 4
#define MAX_NUMBERS 1000

typedef struct Node
{
    int value;
    struct Node* next;
} Node;

typedef struct ThreadArg
{
    Node** head;
    int* num;
    int start;
    int end;
} ThreadArg;

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

void* mergeSort(void* arg)
{
    ThreadArg* threadArg = (ThreadArg*)arg;
    Node** head = threadArg->head;
    int* num = threadArg->num;
    int start = threadArg->start;
    int end = threadArg->end;

    if (start >= end)
    {
        *head = createNode(num[start]);
        return NULL;
    }

    int mid = start + (end - start) / 2;
    Node* left = NULL;
    Node* right = NULL;

    ThreadArg leftArg = {&left, num, start, mid};
    ThreadArg rightArg = {&right, num, mid + 1, end};
    pthread_t leftThread, rightThread;

    pthread_create(&leftThread, NULL, mergeSort, (void*)&leftArg);
    pthread_create(&rightThread, NULL, mergeSort, (void*)&rightArg);

    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);

    *head = mergeLists(left, right);

    return NULL;
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
    int numbers[MAX_NUMBERS];
    int numbersRead = 0;

    fptr = fopen("numbersfile.txt", "r");
    
    if (fptr == NULL)
    {
    	fprintf(stderr, "Error opening file.\n");
    	return 1;
    }
    
    clock_t start_time = clock();

    while (fscanf(fptr, "%d", &numbers[numbersRead]) != EOF && numbersRead < MAX_NUMBERS)
    {
        numbersRead++;
    }

    fclose(fptr);

    Node* head = NULL;

    pthread_t threads[NUM_THREADS];
    ThreadArg threadArgs[NUM_THREADS];
    int numbersPerThread = numbersRead / NUM_THREADS;
    int remainder = numbersRead % NUM_THREADS;
    int start = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadArgs[i].head = &head;
        threadArgs[i].num = numbers;
        threadArgs[i].start = start;
        threadArgs[i].end = start + numbersPerThread - 1 + (i < remainder ? 1 : 0);
        start += numbersPerThread + (i < remainder ? 1 : 0);

        pthread_create(&threads[i], NULL, mergeSort, (void*)&threadArgs[i]);

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    clock_t end_time = clock();
    
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Linked List after sorting:\n");
    printLinkedList(head);
    
    printf("Total runtime (Parallel Execution): %.6f seconds\n", total_time);
    
    Node* temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

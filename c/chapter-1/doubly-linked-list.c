#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    struct Node* next;
    struct Node* prev;
    int value;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} LinkedList;

LinkedList* createList() {
    LinkedList* newList = malloc(sizeof(LinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

void insertFront(LinkedList* list, int value) {
    if (list == NULL) return;

    Node* newNode = malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = list->head;
    newNode->prev = NULL;
    
    if (list->head == NULL) {
        list->tail = newNode;
    }
    else {
        list->head->prev = newNode;
    }

    list->head = newNode;
}

void insertBack(LinkedList* list, int value) {
    if (list == NULL) return;

    Node* newNode = malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = list->tail;
    
    if (list->head == NULL) {
        list->head = newNode;
    }
    else {
        list->tail->next = newNode;
    }
    
    list->tail = newNode;
}

void insertAtIndex(LinkedList* list, int value, int index) {
    if (list == NULL) return;

    if (index == 0) {
        insertFront(list, value);
        return;
    }

    int counter = 0;
    Node* current = list->head;

    while (current != NULL) {
        if (counter != index) {
            current = current->next;
            counter++;
            continue;
        }

        if (current->next == NULL) {
            insertBack(list, value);
            break;
        }

        Node* newNode = malloc(sizeof(Node));
        Node* tempPrev = current->prev;

        newNode->value = value;
        newNode->next = current;
        newNode->prev = tempPrev;
        tempPrev->next = newNode;
        current->prev = newNode;
        break;
    }
}

void printList(LinkedList* list, int direction) {
    if (list == NULL) return;

    Node* current = direction ? list->head : list->tail;

    while(current != NULL) {
        printf("%d\n", current->value);
        current = direction ? current->next : current->prev;
    }
}

Node* findByValue(LinkedList* list, int value) {
    if (list == NULL) return NULL;

    Node* current = list->head;

    while(current != NULL) {
        if (current->value != value) {
            current = current->next;
            continue;
        }

        return current;
    }

    return NULL;
}

void deleteByValue(LinkedList* list, int value) {
    if (list == NULL) return;

    Node* current = NULL;

    if ((current = findByValue(list, value)) != NULL) {

        Node* tempNext = current->next;
        Node* tempPrev = current->prev;

        if (tempNext)
            tempNext->prev = tempPrev;
        else
            list->tail = tempPrev;
        
        if (tempPrev)
            tempPrev->next = tempNext;
        else
            list->head = tempNext;

        free(current);
    }
}

void destroyList(LinkedList* list) {
    if (list == NULL) return;
    
    Node* current = list->head;
    Node* temp = NULL;

    while(current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

    free(list);
}

int main() {
    LinkedList* list = createList();
    printf("List created\n");
    
    for (int i = 0; i < 10; i++) {
        insertBack(list, i);
    }

    insertAtIndex(list, 20, 1);
    insertAtIndex(list, 26, 5);
    insertAtIndex(list, 200, 50);
    
    deleteByValue(list, 6);
    deleteByValue(list, 9);
    deleteByValue(list, 0);
    
    insertAtIndex(list, 30, 4);

    Node* foundNode = findByValue(list, 30);
    if (foundNode)
        printf("Node found: %d\n", foundNode->value);

    foundNode = findByValue(list, 50);
    if (foundNode)
        printf("Node found: %d\n", foundNode->value);
    printList(list, 0);
    destroyList(list);
    printf("List deleted\n");

    return 0;
}

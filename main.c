#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Task {
    char name[50];
    int priority;
    time_t deadline;
    struct Task* next;
} Task;

Task* head = NULL;


void addTask(char name[], int priority, char deadline_str[]) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    strcpy(newTask->name, name);
    newTask->priority = priority;

    struct tm tm;
    memset(&tm, 0, sizeof(struct tm)); // Initialize tm struct to zero
    strptime(deadline_str, "%Y-%m-%d", &tm);
    newTask->deadline = mktime(&tm);

    newTask->next = NULL;

    if (head == NULL) {
        head = newTask;
        return;
    }

    Task* current = head;
    Task* prev = NULL;
    while (current != NULL && current->priority >= priority) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newTask->next = head;
        head = newTask;
    } else {
        prev->next = newTask;
        newTask->next = current;
    }
}


void removeTask(int priority) {
    Task* current = head;
    Task* prev = NULL;

    while (current != NULL && current->priority != priority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Task with priority %d not found.\n", priority);
        return;
    }

    if (prev == NULL) {
        head = head->next;
    } else {
        prev->next = current->next;
    }

    free(current);
}

void listTasks() {
    Task* current = head;

    printf("Priority\tTask Name\tDeadline\n");
    while (current != NULL) {
        char deadline_str[50];
        strftime(deadline_str, sizeof(deadline_str), "%Y-%m-%d", localtime(&current->deadline));
        printf("%d\t\t%s\t\t%s\n", current->priority, current->name, deadline_str);
        current = current->next;
    }
}

void executeTask() {
    if (head == NULL) {
        printf("No tasks to execute.\n");
        return;
    }

    Task* temp = head;
    head = head->next;
    printf("Executing task: %s\n", temp->name);
    free(temp);
}

int main() {
    int choice, priority;
    char name[50];
    char deadline_str[20]; // For date format, e.g., "YYYY-MM-DD"

    while (1) {
        printf("\nReal-Time Task Scheduler\n");
        printf("1. Add Task\n");
        printf("2. Remove Task\n");
        printf("3. List Tasks\n");
        printf("4. Execute Task\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter task name: ");
                scanf("%s", name);
                printf("Enter task priority: ");
                scanf("%d", &priority);
                printf("Enter task deadline (YYYY-MM-DD): ");
                scanf("%s", deadline_str);
                addTask(name, priority, deadline_str);
                break;
            case 2:
                printf("Enter priority of task to remove: ");
                scanf("%d", &priority);
                removeTask(priority);
                break;
            case 3:
                listTasks();
                break;
            case 4:
                executeTask();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

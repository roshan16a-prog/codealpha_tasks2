#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 10
#define MAX_UNDO 100

// Patient structure
typedef struct Patient {
    int id;
    char name[50];
    int age;
    char gender[10];
    char appointmentDate[20];
    struct Patient* next;
} Patient;

// Global variables for queue, stack, patient records, and room allocation
Patient* front = NULL;
Patient* rear = NULL;
Patient* head = NULL;
Patient* undoStack[MAX_UNDO];
int top = -1;
int rooms[MAX_ROOMS] = {0};  // 0 means room is available, patient ID means it's occupied

// Create a new patient node
Patient* createPatient(int id, char name[], int age, char gender[], char appointmentDate[]) {
    Patient* newP = (Patient*)malloc(sizeof(Patient));
    newP->id = id;
    strcpy(newP->name, name);
    newP->age = age;
    strcpy(newP->gender, gender);
    strcpy(newP->appointmentDate, appointmentDate);
    newP->next = NULL;
    return newP;
}

// Enqueue patient into the queue
void enqueue(Patient* p) {
    if (rear == NULL) {
        front = rear = p;
    } else {
        rear->next = p;
        rear = p;
    }
}

// Dequeue patient from the queue
Patient* dequeue() {
    if (front == NULL) return NULL;
    Patient* temp = front;
    front = front->next;
    if (front == NULL) rear = NULL;
    return temp;
}

// Push to the undo stack
void push(Patient* p) {
    if (top < MAX_UNDO - 1) {
        undoStack[++top] = p;
    }
}

// Pop from the undo stack
Patient* pop() {
    if (top == -1) return NULL;
    return undoStack[top--];
}

// Add patient to the records
void addToRecords(Patient* p) {
    p->next = head;
    head = p;
}

// Remove patient from the records
void removeFromRecords(int id) {
    Patient *temp = head, *prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) head = temp->next;
    else prev->next = temp->next;
    free(temp);
}

// Allocate a room for the patient
int allocateRoom(int id) {
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i] == 0) {
            rooms[i] = id;
            return i;
        }
    }
    return -1;  // No rooms available
}

// Free the room
void freeRoom(int id) {
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i] == id) {
            rooms[i] = 0;
            return;
        }
    }
}

// View all patients in the records
void viewPatients() {
    Patient* temp = head;
    if (temp == NULL) {
        printf("No patients in records.\n");
        return;
    }
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Age: %d, Gender: %s, Appointment Date: %s\n", temp->id, temp->name, temp->age, temp->gender, temp->appointmentDate);
        temp = temp->next;
    }
}

// Show the room allocation status
void showRooms() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i]) {
            printf("Room %d: Occupied by Patient ID %d\n", i + 1, rooms[i]);
        } else {
            printf("Room %d: Available\n", i + 1);
        }
    }
}

// Main function to drive the system
int main() {
    int choice, id, age, room;
    char name[50], gender[10], appointmentDate[20];
    int idCounter = 1;

    while (1) {
        printf("\n--- Hospital System ---\n");
        printf("1. Add New Patient\n2. Serve Next Patient\n3. View Patients\n4. Undo Last Add\n5. Room Status\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Name: ");
                scanf(" %[^\n]", name);
                printf("Enter Age: ");
                scanf("%d", &age);
                printf("Enter Gender: ");
                scanf(" %[^\n]", gender);
                printf("Enter Appointment Date (DD/MM/YYYY): ");
                scanf(" %[^\n]", appointmentDate);
                id = idCounter++;

                Patient* newP = createPatient(id, name, age, gender, appointmentDate);
                enqueue(newP);
                addToRecords(newP);
                push(newP);

                room = allocateRoom(id);
                if (room != -1)
                    printf("Patient added with ID %d. Room %d assigned.\n", id, room + 1);
                else
                    printf("Patient added with ID %d. No rooms available!\n");
                break;

            case 2: {
                Patient* p = dequeue();
                if (p == NULL) {
                    printf("No patients in queue.\n");
                } else {
                    printf("Serving Patient ID %d (%s)\n", p->id, p->name);
                    freeRoom(p->id);
                    removeFromRecords(p->id);
                    free(p);
                }
                break;
            }

            case 3:
                viewPatients();
                break;

            case 4: {
                Patient* last = pop();
                if (last == NULL) {
                    printf("No actions to undo.\n");
                } else {
                    removeFromRecords(last->id);
                    freeRoom(last->id);
                    printf("Undo: Removed patient ID %d (%s)\n", last->id, last->name);
                    free(last);
                }
                break;
            }

            case 5:
                showRooms();
                break;

            case 6:

                printf("Exiting system...\n");
                exit(0);

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

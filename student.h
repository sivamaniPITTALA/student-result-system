#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define MAX_ID_LEN 20
#define MAX_NAME_LEN 50
#define NUM_SUBJECTS 5
#define INTERNAL_MAX 40
#define EXTERNAL_MAX 60

typedef struct {
    int sno;
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int subjects[NUM_SUBJECTS][2];
    int subject_totals[NUM_SUBJECTS];
    char subject_grades[NUM_SUBJECTS][3];
    float cgpa;
} Student;

// Function prototypes
int readStudentsFromFile(Student students[], const char *filename);
int validateStudent(Student *student, Student students[], int count);
void computeResults(Student students[], int count);
void displayReport(Student students[], int count);
void writeToFile(Student students[], int count, const char *filename);
void computeClassStats(Student students[], int count);

#endif

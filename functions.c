#include "student.h"

void trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

void assignGrade(int total, char *grade) {
    if (total >= 90) strcpy(grade, "O");
    else if (total >= 85) strcpy(grade, "A+");
    else if (total >= 75) strcpy(grade, "A");
    else if (total >= 65) strcpy(grade, "B+");
    else if (total >= 60) strcpy(grade, "B");
    else if (total >= 55) strcpy(grade, "C");
    else if (total >= 50) strcpy(grade, "D");
    else strcpy(grade, "F");
}

int readStudentsFromFile(Student students[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return 0;
    }
    int count = 0;
    char line[256];
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file) && count < MAX_STUDENTS) {
        Student s = {0};
        char *token;
        int field = 0;
        token = strtok(line, ",");
        while (token && field < 18) {
            switch (field) {
                case 0: s.sno = atoi(token); break;
                case 1: strcpy(s.name, token); break;
                case 2: strcpy(s.id, token); break;
                case 3: s.subjects[0][0] = atoi(token); break;
                case 4: s.subjects[0][1] = atoi(token); break;
                case 5: s.subject_totals[0] = atoi(token); break;
                case 6: s.subjects[1][0] = atoi(token); break;
                case 7: s.subjects[1][1] = atoi(token); break;
                case 8: s.subject_totals[1] = atoi(token); break;
                case 9: s.subjects[2][0] = atoi(token); break;
                case 10: s.subjects[2][1] = atoi(token); break;
                case 11: s.subject_totals[2] = atoi(token); break;
                case 12: s.subjects[3][0] = atoi(token); break;
                case 13: s.subjects[3][1] = atoi(token); break;
                case 14: s.subject_totals[3] = atoi(token); break;
                case 15: s.subjects[4][0] = atoi(token); break;
                case 16: s.subjects[4][1] = atoi(token); break;
                case 17: s.subject_totals[4] = atoi(token); break;
            }
            token = strtok(NULL, ",");
            field++;
        }
        if (field == 18 && validateStudent(&s, students, count)) {
            students[count++] = s;
        } else {
            printf("Invalid data for student %s, skipping.\n", s.id);
        }
    }
    fclose(file);
    return count;
}

int validateStudent(Student *student, Student students[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].id, student->id) == 0) return 0;
    }
    for (int i = 0; student->id[i]; i++) {
        if (!isalnum(student->id[i])) return 0;
    }
    for (int i = 0; student->name[i]; i++) {
        if (!isalpha(student->name[i]) && !isspace(student->name[i])) return 0;
    }
    for (int j = 0; j < NUM_SUBJECTS; j++) {
        if (student->subjects[j][0] < 0 || student->subjects[j][0] > INTERNAL_MAX) return 0;
        if (student->subjects[j][1] < 0 || student->subjects[j][1] > EXTERNAL_MAX) return 0;
        if (student->subject_totals[j] != student->subjects[j][0] + student->subjects[j][1]) return 0;
    }
    return 1;
}

void computeResults(Student students[], int count) {
    for (int i = 0; i < count; i++) {
        float total_cgpa = 0;
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            assignGrade(students[i].subject_totals[j], students[i].subject_grades[j]);
            total_cgpa += students[i].subject_totals[j];
        }
        students[i].cgpa = (total_cgpa / NUM_SUBJECTS) / 10.0;
    }
}

void displayReport(Student students[], int count) {
    printf("%-5s %-20s %-30s ", "S.No", "REG. NO.", "NAME");
    for (int j = 1; j <= NUM_SUBJECTS; j++) {
        printf("Sub%d-Total Grade%d ", j, j);
    }
    printf("CGPA\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-30s ", students[i].sno, students[i].id, students[i].name);
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            printf("%-10d %-5s ", students[i].subject_totals[j], students[i].subject_grades[j]);
        }
        printf("%.2f\n", students[i].cgpa);
    }
}

void writeToFile(Student students[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file %s for writing\n", filename);
        return;
    }
    fprintf(file, "S.No,REG. NO.,NAME,");
    for (int j = 1; j <= NUM_SUBJECTS; j++) {
        fprintf(file, "Sub%d-Total,Grade%d,", j, j);
    }
    fprintf(file, "CGPA\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,", i+1, students[i].id, students[i].name);
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            fprintf(file, "%d,%s,", students[i].subject_totals[j], students[i].subject_grades[j]);
        }
        fprintf(file, "%.2f\n", students[i].cgpa);
    }
    fclose(file);
    printf("Results written to %s\n", filename);
}

void computeClassStats(Student students[], int count) {
    float totalCgpa = 0, highestCgpa = 0, lowestCgpa = 100;
    int gradeCount[8] = {0};
    for (int i = 0; i < count; i++) {
        totalCgpa += students[i].cgpa;
        if (students[i].cgpa > highestCgpa) highestCgpa = students[i].cgpa;
        if (students[i].cgpa < lowestCgpa) lowestCgpa = students[i].cgpa;
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            if (strcmp(students[i].subject_grades[j], "O") == 0) gradeCount[0]++;
            else if (strcmp(students[i].subject_grades[j], "A+") == 0) gradeCount[1]++;
            else if (strcmp(students[i].subject_grades[j], "A") == 0) gradeCount[2]++;
            else if (strcmp(students[i].subject_grades[j], "B+") == 0) gradeCount[3]++;
            else if (strcmp(students[i].subject_grades[j], "B") == 0) gradeCount[4]++;
            else if (strcmp(students[i].subject_grades[j], "C") == 0) gradeCount[5]++;
            else if (strcmp(students[i].subject_grades[j], "D") == 0) gradeCount[6]++;
            else gradeCount[7]++;
        }
    }
    printf("\nClass Average CGPA: %.2f\n", totalCgpa / count);
    printf("Highest CGPA: %.2f\n", highestCgpa);
    printf("Lowest CGPA: %.2f\n", lowestCgpa);
    printf("Grade Distribution:\n");
    printf("O: %d\n", gradeCount[0]);
    printf("A+: %d\n", gradeCount[1]);
    printf("A: %d\n", gradeCount[2]);
    printf("B+: %d\n", gradeCount[3]);
    printf("B: %d\n", gradeCount[4]);
    printf("C: %d\n", gradeCount[5]);
    printf("D: %d\n", gradeCount[6]);
    printf("F: %d\n", gradeCount[7]);
}

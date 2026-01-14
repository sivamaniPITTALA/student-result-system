#include "student.h"
#include <time.h>

int main() {
    srand(time(NULL)); // Seed for random number generation if needed
    Student students[MAX_STUDENTS];
    const char *input_filename = "students.txt";
    const char *output_filename = "results.csv";

    // Read student data from file
    int count = readStudentsFromFile(students, input_filename);
    if (count == 0) {
        printf("No valid student data found in %s\n", input_filename);
        return 1;
    }

    // Compute results (grades, CGPA)
    computeResults(students, count);

    // Display the report on console
    printf("Student Result Processing System\n");
    printf("=================================\n");
    displayReport(students, count);

    // Compute and display class statistics
    computeClassStats(students, count);

    // Write results to output file
    writeToFile(students, count, output_filename);

    return 0;
}

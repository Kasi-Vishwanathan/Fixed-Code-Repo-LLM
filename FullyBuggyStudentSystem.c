/* File: StudentSystem.c */
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    int age;
    float gpa;
} Student;

void addStudent(Student *st, const char *name, int age, float gpa) {
    // Use strncpy with explicit null-termination to prevent buffer overflow
    strncpy(st->name, name, MAX_NAME_LENGTH - 1);
    st->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    
    st->age = age;
    st->gpa = gpa;
}

int main() {
    Student student;
    addStudent(&student, "Alice Johnson", 20, 3.5f);
    
    printf("Student Name: %s\n", student.name);
    printf("Age: %d\n", student.age);
    printf("GPA: %.1f\n", student.gpa);
    
    return 0;
}
/* minigzip.c -- mini implementation of the gzip utility
 * Copyright (C) 1995-2006, 2010, 2011, 2016 Jean-loup Gailly
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "zlib.h"

static void error(const char *msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(EXIT_FAILURE);
}

static void gz_compress(FILE *fp) {
    char buf[1024 * 1024];  /* Reduced buffer size for stack safety */
    int len;
    int errnum;
    gzFile out;
    const char *outfile;

    FILE *in = fp;
    if (in == NULL) {
        perror("file open error");
        exit(EXIT_FAILURE);
    }

    outfile = (const char *)malloc(strlen((char *)in) + 4);
    if (!outfile) {
        error("out of memory");
    }
    strcpy((char *)outfile, (char *)in);
    strcat((char *)outfile, ".gz");

    out = gzopen(outfile, "wb");
    if (out == NULL) {
        if (errno != 0) {
            fprintf(stderr, "gzopen error: unable to open %s: %s\n", outfile, strerror(errno));
        } else {
            fprintf(stderr, "gzopen error: unknown zlib error for %s\n", outfile);
        }
        free((void *)outfile);
        exit(EXIT_FAILURE);
    }

    while ((len = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (gzwrite(out, buf, len) != len) {
            error(gzerror(out, &errnum));
        }
    }

    if (ferror(in)) {
        fprintf(stderr, "error reading input file: %s\n", strerror(errno));
        gzclose(out);
        free((void *)outfile);
        exit(EXIT_FAILURE);
    }

    if (gzclose(out) != Z_OK) {
        error("failed to close gz file");
    }
    free((void *)outfile);
}

int main(int argc, char **argv) {
    FILE *fp;
    char *outfile;
    int i;

    if (argc == 1) {
        fprintf(stderr, "use: minigzip file1 [file2 ...]\n");
        return EXIT_SUCCESS;
    }

    for (i = 1; i < argc; i++) {
        fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            fprintf(stderr, "can't open %s: %s\n", argv[i], strerror(errno));
            continue;
        }
        gz_compress(fp);
        fclose(fp);
        unlink(argv[i]);
    }

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>

struct student {
    char name[50];
    int age;
    float gpa;
};

void error(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

void process_student_data(FILE *fp) {
    int i = 0;
    struct student s;

    while (fscanf(fp, "%49s %d %f", s.name, &s.age, &s.gpa) == 3) {
        if (s.age < 0) {
            error("Invalid age");
            continue;
        }
        if (s.gpa < 0.0 || s.gpa > 4.0) {
            error("Invalid GPA");
            continue;
        }

        add_to_database(s); // Assuming this function is defined elsewhere

        i++;
        if (i % 100 == 0) {
            printf("Processing record %d\n", i);
        }
    }
}

void list_students(void) {
    // Implementation to list students (if required)
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned char Byte;

long check(Byte *v, int n) {
    long sum = 1;
    for (int j = 0; j < n; j++) {
        sum = (sum * 0xfff) & 0xFFFFFFFF;
        sum |= v[j];
    }
    return sum;
}

int main(void) {
    Byte buf[BUFSIZ];
    ssize_t len;
    long sum = 0;

    while ((len = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        sum += check(buf, len);
    }

    if (len < 0) {
        perror("read error");
        return 1;
    }

    fprintf(stderr, "sum is %ld\n", sum);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date {
    int day;
    int month;
    int year;
};

struct Student {
    int id;
    char name[50];
    char course[50];
    struct Date birthdate;
};

struct StudentNode {
    struct Student data;
    struct StudentNode *next;
};

typedef struct StudentNode *LinkedList;

LinkedList addStudent(LinkedList list) {
    struct Student newStudent;
    char buffer[100];
    int parsed;

    printf("Enter student ID: ");
    fgets(buffer, sizeof(buffer), stdin);
    parsed = sscanf(buffer, "%d", &newStudent.id);
    if (parsed != 1) {
        printf("Invalid ID input.\n");
        return list;
    }

    printf("Enter student name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    printf("Enter student course: ");
    fgets(newStudent.course, sizeof(newStudent.course), stdin);
    newStudent.course[strcspn(newStudent.course, "\n")] = '\0';

    printf("Enter student birthdate (dd/mm/yyyy): ");
    fgets(buffer, sizeof(buffer), stdin);
    parsed = sscanf(buffer, "%d/%d/%d", &newStudent.birthdate.day, &newStudent.birthdate.month, &newStudent.birthdate.year);
    if (parsed != 3) {
        printf("Invalid date format.\n");
        return list;
    }

    struct StudentNode *newNode = (struct StudentNode *)malloc(sizeof(struct StudentNode));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return list;
    }
    newNode->data = newStudent;
    newNode->next = list;
    return newNode;
}

LinkedList insertStudent(LinkedList students) {
    struct Student newStudent;
    char buffer[100];
    int parsed;

    printf("Enter student ID: ");
    fgets(buffer, sizeof(buffer), stdin);
    parsed = sscanf(buffer, "%d", &newStudent.id);
    if (parsed != 1) {
        printf("Invalid ID input.\n");
        return students;
    }

    printf("Enter student name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    printf("Enter student course: ");
    fgets(newStudent.course, sizeof(newStudent.course), stdin);
    newStudent.course[strcspn(newStudent.course, "\n")] = '\0';

    printf("Enter student birthdate (dd/mm/yyyy): ");
    fgets(buffer, sizeof(buffer), stdin);
    parsed = sscanf(buffer, "%d/%d/%d", &newStudent.birthdate.day, &newStudent.birthdate.month, &newStudent.birthdate.year);
    if (parsed != 3) {
        printf("Invalid date format.\n");
        return students;
    }

    struct StudentNode *newNode = (struct StudentNode *)malloc(sizeof(struct StudentNode));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return students;
    }
    newNode->data = newStudent;

    struct StudentNode *current = students;
    struct StudentNode *prev = NULL;

    while (current && current->data.id < newStudent.id) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newNode->next = students;
        students = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }

    return students;
}

void searchStudent(LinkedList students, int studentID) {
    struct StudentNode *current = students;
    while (current) {
        if (current->data.id == studentID) {
            printf("Student found:\n");
            printf("ID: %d\nName: %s\nCourse: %s\nBirthdate: %d/%d/%d\n",
                   current->data.id, current->data.name, current->data.course,
                   current->data.birthdate.day, current->data.birthdate.month,
                   current->data.birthdate.year);
            return;
        }
        current = current->next;
    }
    printf("Student not found.\n");
}

void updateStudent(LinkedList students, int studentID) {
    struct StudentNode *current = students;
    while (current) {
        if (current->data.id == studentID) {
            char buffer[100];

            printf("Enter new name: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(current->data.name, buffer);

            printf("Enter new course: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(current->data.course, buffer);

            printf("Enter new birthdate (dd/mm/yyyy): ");
            fgets(buffer, sizeof(buffer), stdin);
            if (sscanf(buffer, "%d/%d/%d", &current->data.birthdate.day,
                       &current->data.birthdate.month,
                       &current->data.birthdate.year) != 3) {
                printf("Invalid date format. Birthdate not updated.\n");
            }

            printf("Student updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Student not found.\n");
}

LinkedList deleteStudent(LinkedList students, int studentID) {
    struct StudentNode *current = students;
    struct StudentNode *prev = NULL;

    while (current && current->data.id != studentID) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        printf("Student not found.\n");
        return students;
    }

    if (prev) {
        prev->next = current->next;
    } else {
        students = current->next;
    }

    free(current);
    printf("Student deleted successfully.\n");
    return students;
}

void listStudentsByYear(LinkedList students, int year) {
    struct StudentNode *current = students;
    int found = 0;

    printf("Students enrolled in %d:\n", year);
    while (current) {
        if (current->data.birthdate.year == year) {
            printf("ID: %d, Name: %s, Course: %s\n",
                   current->data.id, current->data.name, current->data.course);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No students found for the year %d.\n", year);
    }
}

int main() {
    LinkedList students = NULL;
    int option;
    char buffer[100];

    do {
        printf("\nUniversity Student System\n");
        printf("1. Add Student\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. List Students by Year\n");
        printf("6. Exit\n");
        printf("Enter option: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &option) != 1) {
            printf("Invalid option. Please enter a number.\n");
            continue;
        }

        switch (option) {
            case 1:
                students = addStudent(students);
                break;
            case 2: {
                int studentID;
                printf("Enter student ID to search: ");
                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &studentID) == 1) {
                    searchStudent(students, studentID);
                } else {
                    printf("Invalid ID input.\n");
                }
                break;
            }
            case 3: {
                int studentID;
                printf("Enter student ID to update: ");
                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &studentID) == 1) {
                    updateStudent(students, studentID);
                } else {
                    printf("Invalid ID input.\n");
                }
                break;
            }
            case 4: {
                int studentID;
                printf("Enter student ID to delete: ");
                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &studentID) == 1) {
                    students = deleteStudent(students, studentID);
                } else {
                    printf("Invalid ID input.\n");
                }
                break;
            }
            case 5: {
                int year;
                printf("Enter year to list students: ");
                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &year) == 1) {
                    listStudentsByYear(students, year);
                } else {
                    printf("Invalid year input.\n");
                }
                break;
            }
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while (option != 6);

    // Free all allocated memory before exiting
    struct StudentNode *current = students;
    while (current) {
        struct StudentNode *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
/* gzlib.c -- zlib functions common to reading and writing gzip files
 * Copyright (C) 2004-2017 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

/* Clear the error and end-of-file flags. */
void gzclearerr(gzFile file) {
    gz_statep state;

    if (file == NULL)
        return;
    state = (gz_statep)file;

    state->err = 0;
    state->eof = 0;  /* Ensure eof is reset for both read and write modes */
    if (state->mode == GZ_READ) {
        state->past = 0;
        if (state->how == LOOK)
            state->seek = 1;
        /* Reinitialize zran state if needed */
        if (state->in == NULL) {
            state->have = 0;
            state->next = state->out;
        }
    }
    /* Reset any write-related buffers or states if necessary */
}
/* File: FullyBuggyStudentSystem.c_chunk7 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void process_data(char *str, int mode) {
    if (!str) {
        fprintf(stderr, "Invalid input string!\n");
        return;
    }

    size_t length = strlen(str);
    char *buffer = malloc(length + 1);  /* +1 for null terminator */
    if (!buffer) {
        perror("malloc failed");
        return;
    }

    for (int i = 0; i < length; i++) {
        buffer[i] = (char)(str[i] + mode);
    }
    buffer[length] = '\0';  /* Ensure proper null termination */

    handle_output(stdout, buffer);
    free(buffer);  /* Single ownership: freed by allocator */
}

void handle_output(FILE *fp, char *data) {
    if (!fp || !data) {
        fprintf(stderr, "Invalid arguments to handle_output!\n");
        return;
    }
    fprintf(fp, "%s\n", data);  /* Fixed format string */
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <string> <mode>\n", argv[0]);
        return 1;
    }

    char *input_str = argv[1];
    int operation_mode = atoi(argv[2]);

    process_data(input_str, operation_mode);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

struct Student {
    char name[50];
    int age;
    float gpa;
};

int main(void) {
    struct Student *student1 = malloc(sizeof(struct Student));
    if (student1 == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("Enter student name: ");
    scanf("%49s", student1->name);

    printf("Enter student age: ");
    scanf("%d", &student1->age);

    printf("Enter student GPA: ");
    scanf("%f", &student1->gpa);

    printf("\nStudent Details:\n");
    printf("Name: %s\n", student1->name);
    printf("Age: %d\n", student1->age);
    printf("GPA: %.2f\n", student1->gpa);

    free(student1);
    return 0;
}
    dropCourse_76(0, "Course76");
    dropCourse_77(0, "Course77");
    dropCourse_78(0, "Course78");
    dropCourse_79(0, "Course79");
    dropCourse_80(0, "Course80");
    dropCourse_81(0, "Course81");
    dropCourse_82(0, "Course82");
    dropCourse_83(0, "Course83");
    dropCourse_84(0, "Course84");
    dropCourse_85(0, "Course85");
    dropCourse_86(0, "Course86");
    dropCourse_87(0, "Course87");
    dropCourse_88(0, "Course88");
    dropCourse_89(0, "Course89");
    dropCourse_90(0, "Course90");
    dropCourse_91(0, "Course91");
    dropCourse_92(0, "Course92");
    dropCourse_93(0, "Course93");
    dropCourse_94(0, "Course94");
    dropCourse_95(0, "Course95");
    dropCourse_96(0, "Course96");
    dropCourse_97(0, "Course97");
    dropCourse_98(0, "Course98");
    dropCourse_99(0, "Course99");
    printStudent_0();
    printStudent_1();
    printStudent_2();
    printStudent_3();
    printStudent_4();
    printStudent_5();
    printStudent_6();
    printStudent_7();
    printStudent_8();
    printStudent_9();
    printStudent_10();
    printStudent_11();
    printStudent_12();
    printStudent_13();
    printStudent_14();
    printStudent_15();
    printStudent_16();
    printStudent_17();
    printStudent_18();
    printStudent_19();
    printStudent_20();
    printStudent_21();
    printStudent_22();
    printStudent_23();
    printStudent_24();
    printStudent_25();
    printStudent_26();
    printStudent_27();
    printStudent_28();
    printStudent_29();
    printStudent_30();
    printStudent_31();
    printStudent_32();
    printStudent_33();
    printStudent_34();
    printStudent_35();
    printStudent_36();
    printStudent_37();
    printStudent_38();
    printStudent_39();
    printStudent_40();
    printStudent_41();
    printStudent_42();
    printStudent_43();
    printStudent_44();
    printStudent_45();
    printStudent_46();
    printStudent_47();
    printStudent_48();
    printStudent_49();
    printStudent_50();
    printStudent_51();
    printStudent_52();
    printStudent_53();
    printStudent_54();
    printStudent_55();
    printStudent_56();
    printStudent_57();
    printStudent_58();
    printStudent_59();
    printStudent_60();
    printStudent_61();
    printStudent_62();
    printStudent_63();
    printStudent_64();
    printStudent_65();
    printStudent_66();
    printStudent_67();
    printStudent_68();
    printStudent_69();
    printStudent_70();
    printStudent_71();
    printStudent_72();
    printStudent_73();
    printStudent_74();
    printStudent_75();
    printStudent_76();
    printStudent_77();
    printStudent_78();
    printStudent_79();
    printStudent_80();
    printStudent_81();
    printStudent_82();
    printStudent_83();
    printStudent_84();
    printStudent_85();
    printStudent_86();
    printStudent_87();
    printStudent_88();
    printStudent_89();
    printStudent_90();
    printStudent_91();
    printStudent_92();
    printStudent_93();
    printStudent_94();
    printStudent_95();
    printStudent_96();
    printStudent_97();
    printStudent_98();
    printStudent_99();
    return 0;
}
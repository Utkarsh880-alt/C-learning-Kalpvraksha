#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FN "users.txt"
#define TN "temp_users.txt"
#define NL 50 // Name Length Max

// User record structure (i: id, n: name, a: age)
struct U { 
    int i;
    char n[NL];
    int a;
};

// Clears the input buffer
void clr_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Checks if a given user ID already exists in the data file.
 * @param id The ID to check.
 * @return 1 if the ID is unique (or file doesn't exist), 0 otherwise.
 */
int is_id_unique(int id) {
    FILE *f = fopen(FN, "r");
    if (!f) return 1; // File does not exist, so ID is unique.
    
    struct U temp_u;
    int is_unique = 1;

    // Scan the file for a matching ID
    while (fscanf(f, "%d,%49[^,],%d\n", &temp_u.i, temp_u.n, &temp_u.a) == 3) {
        if (temp_u.i == id) {
            is_unique = 0; // ID found
            break;
        }
    }
    
    fclose(f);
    return is_unique;
}


int main() {
    int id, fnd, scan_res;
    char cmd;
    FILE *f, *t;
    struct U u; 

    printf("--- Simple C File CRUD ---\n");

    while (1) {
        printf("\nC/R/U/D: ");
        scan_res = scanf(" %c", &cmd);
        if (scan_res != 1) { printf("\nExiting.\n"); break; }
        cmd = toupper(cmd);
        clr_buf();

        switch (cmd) {
            // --- C: CREATE (ADD UNIQUE RECORD) ---
            case 'C':
                f = fopen(FN, "a");
                if (!f) { perror("File Error"); break; }
                
                printf("ID Name Age: ");
                // Basic input validation: check scan count, positive ID, non-negative age
                if (scanf("%d %s %d", &u.i, u.n, &u.a) == 3 && u.i > 0 && u.a >= 0) {
                    
                    // NEW: Check for uniqueness before writing
                    if (is_id_unique(u.i)) {
                        fprintf(f, "%d,%s,%d\n", u.i, u.n, u.a);
                        printf("ID %d added.\n", u.i);
                    } else {
                        printf("ERROR: ID %d already exists. Must be unique.\n", u.i);
                    }
                } else {
                    printf("ERROR: Invalid input or data.\n");
                }
                clr_buf();
                fclose(f);
                break;

            // --- R: READ ---
            case 'R':
                f = fopen(FN, "r");
                if (!f) { printf("No records found.\n"); break; }
                
                printf("\nID\tNAME\tAGE\n---\t----\t---\n");
                fnd = 0;
                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    printf("%d\t%s\t%d\n", u.i, u.n, u.a);
                    fnd = 1;
                }
                if (!fnd) { printf("(File is empty.)\n"); }
                
                fclose(f);
                break;
                
            // --- U: UPDATE ---
            case 'U':
                f = fopen(FN, "r");
                if (!f) { printf("No records found.\n"); break; }
                t = fopen(TN, "w");
                if (!t) { perror("Temp Error"); fclose(f); break; }

                printf("Target ID to UPDATE: "); 
                if (scanf("%d", &id) != 1) { 
                    printf("Invalid ID.\n"); 
                    clr_buf(); fclose(f); fclose(t); remove(TN); break;
                }
                clr_buf();
                fnd = 0;

                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    if (u.i == id) {
                        fnd = 1;
                        printf("Found ID %d. New Name/Age: ", id);
                        char new_n[NL]; int new_a;
                        if (scanf("%s %d", new_n, &new_a) == 2 && new_a >= 0) {
                            fprintf(t, "%d,%s,%d\n", u.i, new_n, new_a);
                            printf("ID %d UPDATED.\n", id);
                        } else {
                            // If update fails, copy the original record to prevent data loss
                            fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a);
                            printf("WARNING: Update failed, original kept.\n");
                        }
                        clr_buf(); 
                    } else {
                        fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a); // Copy record
                    }
                }
                
                fclose(f); fclose(t);
                if (fnd) { 
                    remove(FN); 
                    rename(TN, FN); 
                } else { 
                    remove(TN); 
                    printf("ERROR: ID %d not found.\n", id); 
                }
                break;

            // --- D: DELETE ---
            case 'D':
                f = fopen(FN, "r");
                if (!f) { printf("No records found.\n"); break; }
                t = fopen(TN, "w");
                if (!t) { perror("Temp Error"); fclose(f); break; }

                printf("Target ID to DELETE: "); 
                if (scanf("%d", &id) != 1) { 
                    printf("Invalid ID.\n"); 
                    clr_buf(); fclose(f); fclose(t); remove(TN); break;
                }
                clr_buf();
                fnd = 0;

                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    if (u.i == id) {
                        fnd = 1;
                        printf("ID %d DELETED.\n", id); // Skip writing the record
                    } else {
                        fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a); // Copy non-target record
                    }
                }
                
                fclose(f); fclose(t);
                if (fnd) { 
                    remove(FN); 
                    rename(TN, FN); 
                } else { 
                    remove(TN); 
                    printf("ERROR: ID %d not found.\n", id); 
                }
                break;
                
            default:
                printf("Invalid command.\n");
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define FN "users.txt"// Defines a macro for the primary data file name 

#define TN "temp.txt"//for temperoary files

struct U { int i; char n[50]; int a; };

int main() {
    int id, fnd;
    char cmd;
    FILE *f, *t;
    struct U u;

    while (printf("\nC/R/U/D: ") && scanf(" %c", &cmd) == 1) {

        switch (cmd) {
            case 'C':
                f = fopen(FN, "a");
                if (!f) { perror("File error"); 
                break; }
                printf("ID Name Age: ");
                if (scanf("%d %s %d", &u.i, u.n, &u.a) == 3)
                    fprintf(f, "%d,%s,%d\n", u.i, u.n, u.a);
                fclose(f);
                break;

            case 'R':
                f = fopen(FN, "r");
                if (!f) { printf("No records.\n"); break; }
                printf("ID\tNAME\tAGE\n");
                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3)
                    printf("%d\t%s\t%d\n", u.i, u.n, u.a);
                fclose(f);
                break;
                
            case 'U':
            case 'D':
                f = fopen(FN, "r");
                if (!f) { printf("No records.\n"); 
                break; }
                t = fopen(TN, "w");
                if (!t) { perror("Temp error"); 
                fclose(f); 
                break; }

                printf("Target ID: "); 
                scanf("%d", &id); 
                fnd = 0;
                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    if (u.i == id) {
                        fnd = 1;
                        if (cmd == 'U') {
                            printf("Write New Name and Age: ");
                            if (scanf("%s %d", u.n, &u.a) == 2)
                            fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a);
                            else
                            fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a);
                        }
                    } else {
                        fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a);
                    }
                }
                
                fclose(f); fclose(t);
                if (fnd) { 
                    remove(FN); rename(TN, FN); 
                    printf("Operation complete.\n"); 
                } else { 
                    remove(TN); 
                    printf("ID not found.\n"); 
                }
                break;
            
            default:
                printf("Invalid command.\n");
        }
    }
    return 0;
}
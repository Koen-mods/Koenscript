#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_VARS 100
#define MAX_NAME 32

typedef struct {
    char name[MAX_NAME];
    int value;
} Var;

Var vars[MAX_VARS];
int var_count = 0;

// Find a variable, return NULL if not found
Var* find_var(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) return &vars[i];
    }
    return NULL;
}

// Create a new variable
Var* create_var(const char* name, int value) {
    if (var_count >= MAX_VARS) return NULL;
    strcpy(vars[var_count].name, name);
    vars[var_count].value = value;
    return &vars[var_count++];
}

// Trim newline and spaces
void trim(char* str) {
    int len = strlen(str);
    while(len > 0 && (str[len-1]=='\n' || str[len-1]==' ')) str[--len] = 0;
}

int has_content(const char *str) {
    while(*str) {
        if(!isspace((unsigned char)*str)) return 1;
        str++;
    }
    return 0;
}

// Execute one line
void execute_line(char* line) {
    trim(line);

    if (strncmp(line, "print ", 6) == 0) {
        char *varname = line + 6;
        if (varname[0] == '"' || varname[0] == '\'') {
            size_t len = strlen(varname);
            if (len >= 2) {
                varname[len-1] = '\0';
                printf("%s\n", varname + 1);
            } else {
                printf("\n");
            }
        } else {
            Var* v = find_var(varname);
            if (v) printf("%d\n", v->value);
            else printf("Error: Undefined variable '%s'\n", varname);
        }
        
    } else {
        // Assignment: var = value
        char varname[MAX_NAME];
        int value;
        if (sscanf(line, "%31s = %d", varname, &value) == 2) {
            Var* v = find_var(varname);
            if (v) {
                v->value = value;
            } else {
                create_var(varname, value);
            }
        } else {
            if (strncmp(line, "input(", 6) == 0) {
                char args[256];
                const char *start = strchr(line, '(');
                const char *end   = strrchr(line, ')');

                if (!start || !end || end <= start) {
                    printf("Syntax error in input()\n");
                    return;
                }

                strncpy(args, start + 1, end - start - 1);
                args[end - start - 1] = '\0';
                char varname[64];
                char prompt[128];

                if (sscanf(args, " %[^,], \"%[^\"]\"", varname, prompt) == 2) {
                    printf("%s", prompt);

                    char buffer[128];
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = 0;

                    int num = atoi(buffer); // convert to int

                    Var* v = find_var(varname);
                    if (v) {
                        v->value = num;
                    } else {
                        create_var(varname, num);
                    }
                } else {
                    printf("Syntax error in input() arguments\n");
                }
            } else {
                printf("Syntax error\n");
            }
        }
    }
}

// Execute a file line by line
void execute_file(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (has_content(line) == 0 || line[0] == '#') continue;
        execute_line(line);
    }
    fclose(f);
}

int main(int argc, char* argv[]) {
    char line[1024];

    if (argc > 1) {
        // Run file mode
        execute_file(argv[1]);
    } else {
        // REPL mode
        printf("TinyPy REPL (type Ctrl+C to exit)\n");
        while (1) {
            printf(">> ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (has_content(line) == 0 || line[0] == '#') continue;
            execute_line(line);
        }
    }

    return 0;
}


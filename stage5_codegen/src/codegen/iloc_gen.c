#include <iloc_gen.h>
#include <stdio.h>
#include <stdlib.h>

static int label_counter = 0;
static int temp_counter = 0;

char* temp_new(void)
{
    int length = snprintf(NULL, 0, "r%d", temp_counter);
    char* name = malloc(length + 1);
    if (!name)
        return NULL;
    sprintf(name, "r%d", temp_counter);
    return name;
}

char* label_new(void)
{
    int length = snprintf(NULL, 0, "l%d", label_counter);
    char* name = malloc(length + 1);
    if (!name)
        return NULL;
    sprintf(name, "l%d", label_counter);
    return name;
}

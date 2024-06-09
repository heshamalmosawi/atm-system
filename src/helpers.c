#include "header.h"

void lowerize(char *str){ // turns entire to lower case
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}
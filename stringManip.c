//contains any functions related to string manipulation
//such as 
//str_trim: removes the whitespace from each end of  a string sent to it, returning point to new string
//str_cut: takes three args, the string pointer, an int representing the start, and an int representing the end. Returns a point to the substring that begins at the start and ends at the end

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "stringManip.h"

char *str_trim(char *str){
    char *end;
    //trim leading
    while(isspace(*str)) str++;
     //trim trailing 
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    //null terminate
    *(end + 1) = 0;
    return str;
} 

char *str_cut(char *str, int start, int end) {
    if (start >= end || start < 0) return NULL;

    char *string = malloc(end - start + 1); //allocate memory for the substring
    if (string) {
        strncpy(string, str + start, end - start); 
        string[end - start] = '\0'; //null terminate the substring
    }
    return string;
}

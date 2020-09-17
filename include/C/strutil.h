#pragma once

typedef struct {
    size_t length;
    char *data;
} str_t;

typedef struct {
    size_t length;
    const char *data;
} strView_t;

/*!
 * Compares the strView a to the strView b.
 * @param a pointer to strView to be compared
 * @param b pointer to strView to be compared
 * @return an integral value indicating the relationship between the strings
 */
int strViewCmp(const void *a, const void *b);

/*!
 * Compares the strView a to the strView b in reverse order. Strings must contain '\0' before it.
 * @param a pointer to strView to be compared
 * @param b pointer to strView to be compared
 * @return an integral value indicating the relationship between the strings
 */
int strViewCmpReversed(const void *a, const void *b);

/*!
 * Calculate number of lines in string
 * @param str input string
 * @return number of lines in string
 */
size_t calcLines(const char *str);

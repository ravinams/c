// DO NOT MODIFY THIS HEADER FILE.

#ifndef _LIST1_H_
#define _LIST1_H_

struct Node {
    struct Node *next;
    short len;
    char data[6];
};

/*
 * Create a node that stores the given string s if strlen(s) <= 5,
 * or a substring consisting of the first 5 characters of s otherwise.
 *
 * The string stored in the node is always null-terminated.
 * In addition, the length of the stored string (not including null)
 * is stored in the 'len' field of the Node structure.
 *
 * The node is added to the front of the given list, and the resulting
 * list is returned.  NULL is returned if malloc for the node fails.
 *
 * If s is NULL, it is treated as if it were "".
 */
struct Node *prepend(struct Node *list, char *s);

/*
 * Does the same thing as strcpy(t,s), except that if strlen(s) > n,
 * only the first n chars of s are copied.  The string t gets always 
 * null-terminated, which means that t must point to a buffer of at least 
 * min( strlen(s)+1, n+1 ) bytes.
 *
 * Both t and s are assumed to be not NULL.
 */
void strcpy_upto(char *t, const char *s, int n);

/*
 * Print the given list in the following format:
 *
 *     ->[len:data]->[len->data]->...->[len:data]
 *
 * For instance, a two-node list containing "one" and "world" will print:
 *
 *     ->[3:one]->[5:world]
 *
 * A newline is printed at the end.
 */
void print(struct Node *list);

/*
 * Deallocates all nodes (i.e., free() each node.)
 */
void deallocate_list(struct Node *list);

#endif /* #ifndef _LIST1_H_ */

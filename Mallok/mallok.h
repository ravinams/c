#ifndef MALLOK_H
#define MALLOK_H

#ifdef __cplusplus
extern "C" {
#endif

void create_pool(int size);
void *my_malloc(int size);
void my_free(void *block);
void printAll();
void free_pool();


#ifdef __cplusplus
}
#endif

#endif /* MALLOK_H */


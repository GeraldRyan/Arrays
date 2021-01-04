#ifndef arrays_h
#define arrays_h

typedef struct Array
{
  int capacity;
  int count;
  char **elements;
} Array;

Array *create_array(int capacity);
void destroy_array(Array *arr);
char *arr_read(Array *arr, int index);
void arr_append(Array *arr, char *element);
void arr_insert(Array *arr, char *element, int index);
void arr_remove(Array *arr, char *element);

// STRETCH GOALS // WITHOUT COMMENTS == WORKING (i.e. Finished) 
int arr_clear(Array *arr); //
char* arr_pop(Array *arr); // FINISHED BUT WANT TO MAKE THIS VARIADIC WITH INDEX FEATURE
int arr_pop_no_rtn(Array *arr); // FINISHED BUT WANT TO MAKE THIS VARIADIC WITH INDEX FEATURE 
int arr_copy(Array *dst, Array* src); 
int arr_index(Array *arr, char *element); //
int arr_reverse(Array *arr); // 
int arr_sort(Array *arr);  // 
int arr_pop_by_index(Array *arr, int index); //
int arr_extend(Array* arr, Array* arr2); //

#endif
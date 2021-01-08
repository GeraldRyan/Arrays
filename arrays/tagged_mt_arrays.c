#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h> // for variadic functions
#include "var_arg_util.h"

typedef union element
{
  float f; // Is this the biggest size? Whatever is biggest will set space for all elements.
  int i;
  long l;
  char c;
  char *s; // This is the biggest size item? Or is float?
} element;

typedef struct Array
{
  int capacity;
  int original_capacity;
  int count;
  // char **elements;       // This was a pointer to an array of strings
  // element *elements; // can't be the element itself or we can't have array. Has to be pointer for array ADT.
  struct element_wrapper *elements;
} Array;

typedef struct element_wrapper
{
  char *type;
  union element *element;
} element_wrapper;

/*****
 * Allocate memory for a new array
 *****/
Array *create_array(int capacity)
{
  // Allocate memory for the Array struct
  struct Array *a = malloc(sizeof(struct Array));
  // Set initial values for capacity and count
  a->capacity = capacity;
  a->original_capacity = capacity;
  a->count = 0;
  // Allocate memory for elements
  // a->elements = malloc(sizeof(char **) * capacity);
  a->elements = malloc(sizeof(struct element_wrapper) * capacity); // Have to malloc the size of the array declared above. Could be 1. Could be 100.
  return a;
}

/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr)
{
  // Free all elements
  if (arr->elements != NULL)
    free(arr->elements);
  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/

// int shrink_array(Array *arr)
// {
//   if (arr->count == 0)
//   {
//     printf("Array count is zero, being restored to it's original capacity of %d\n", arr->capacity);
//     arr->capacity = arr->original_capacity;
//     arr->elements = realloc(arr->elements, arr->capacity * sizeof(char *));
//     return 1;
//   }
//   else if (arr->count < (arr->capacity / 4))
//   {
//     printf("Array being shrunk count:%d capacity:%d (less than 1/4 full, being shrunk by half)\n", arr->count, arr->capacity);
//     arr->capacity = arr->count * 2;
//     arr->elements = realloc(arr->elements, arr->capacity * sizeof(char *));
//     printf("New count:%d and new capacity:%d\n", arr->count, arr->capacity * sizeof(char *));
//     return 1;
//   }
//   else
//   {
//     return 0;
//   }
// }

void resize_array(Array *arr)
{
  struct element_wrapper *tmp;
  // Create a new element storage with double capacity
  arr->capacity *= 2;
  printf("Reallocing with new capacity %d\n", arr->capacity);
  tmp = realloc(arr->elements, arr->capacity * sizeof(struct element_wrapper));
  if (tmp == NULL)
  {
    printf("Realloc failed\n");
  }
  else
  {
    arr->elements = tmp;
  }
}

/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
struct element_wrapper *arr_read(Array *arr, int index)
{
  // Throw an error if the index is greater or equal to than the current count
  if (index > arr->count || index < 0)
  {
    fprintf(stderr, "Index out of range in array_read function");
    return NULL;
  }
  // We have to get nth index of array and return a pointer to an individual element
  return (arr->elements + index); // why isn't this doing pointer arithmetic?
}

struct element_wrapper *new_element_i(int i)
{
  union element *e = malloc(sizeof(union element));
  e->i = i;
  struct element_wrapper *ewr = malloc(sizeof(struct element_wrapper));
  ewr->element = e;
  ewr->type = "int";
  return ewr;
}
struct element_wrapper *new_element_s(char *s)
{
  union element *e = malloc(sizeof(union element));
  e->s = s;
  struct element_wrapper *ewr = malloc(sizeof(struct element_wrapper));
  ewr->element = e;
  ewr->type = "string";
  return ewr;
}
struct element_wrapper *new_element_c(char c)
{
  union element *e = malloc(sizeof(union element));
  e->c = c;
  struct element_wrapper *ewr = malloc(sizeof(struct element_wrapper));
  ewr->element = e;
  ewr->type = "char";
  return ewr;
}
struct element_wrapper *new_element_f(float f)
{
  union element *e = malloc(sizeof(union element));
  e->f = f;
  struct element_wrapper *ewr = malloc(sizeof(struct element_wrapper));
  ewr->element = e;
  ewr->type = "float";
  return ewr;
}
/*****
 * Insert an element to the array at the given index
 *
 * Store the VALUE of the given string, not the REFERENCE
 *****/
void arr_insert(Array *arr, struct element_wrapper *e, int index)
{
  // Throw an error if the index is greater than the current count
  if (index > arr->count)
  {
    fprintf(stderr, "Index out of range in arr_insert fprintf");
    printf("Index out of range in arr_insert\n");
    return;
  }
  // Resize the array if the number of elements is over capacity
  if (arr->count == arr->capacity)
    resize_array(arr);
  // Move every element after the insert index to the right one position
  memmove(arr->elements + ((index + 1)), arr->elements + (index), (arr->count - index) * sizeof(struct element_wrapper )); // NB might be memory leak or pass by value, but proved the code works. 
  // Copy the element (hint: use `strdup()`) and add it to the array
  *(arr->elements + (index)) = *e; // copy pointers

  // Increment count by 1
  arr->count++;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, struct element_wrapper *e)
{
  printf("Called here\n");
  if (arr->count == arr->capacity)
  {
    resize_array(arr);
  }

  *(arr->elements + arr->count) = *e;

  // Increment count by 1
  arr->count++;
}

// int mystrcmp(char *s1, char *s2)
// {
//   int len1 = strlen(s1);
//   int len2 = strlen(s2);
//   if (len1 != len2)
//     return 1;
//   for (int i = 0; i < len2; i++)
//   {
//     if (*(s1 + i) != *(s2 + i))
//     {
//       return 1;
//     }
//   }
//   return 0;
// }

// /*****
//  * Remove the first occurence of the given element from the array,
//  * then shift every element after that occurence to the left one slot.
//  *
//  * Throw an error if the value is not found.
//  *****/
// int arr_remove(Array *arr, char *element)
// {
//   for (int i = 0; i < arr->count; i++)
//   {
//     // if (mystrcmp(element, *(arr->elements + i)) == 0) // Built my own strcmp function, it works, but let's use the built-in.
//     if (strcmp(element, *(arr->elements + i)) == 0)
//     {
//       memmove((arr->elements + i), arr->elements + i + 1, sizeof(char *) * (arr->count - (i + 1)));
//       arr->elements[arr->count - 1] = NULL;
//       arr->count--;
//       shrink_array(arr);
//       return 1; // removed something
//     }
//   }
//   // fprintf(stderr, "Fprintf stderr: Array element not found\n");
//   return 0; // Nothing removed
//   // Search for the first occurence of the element and remove it.
//   // Don't forget to free its memory!
//   // Shift over every element after the removed element to the left one position
// }

// int arr_remove_all(Array *arr, char *element) // NB THIS IS NOT IN THE TEST FILE BEING TESTED. THIS IS CUSTOM FUNCTION OF MY OWN.
// {
//   int i = 0;
//   while (arr_remove(arr, element))
//   {
//     i++;
//   };
//   printf("element \"%s\" removed %d times\n", element, i);
//   shrink_array(arr);
//   return i;
// }

// /*****
//  * Utility function to print an array.
//  *****/
void arr_print(Array *arr)
{
  printf("Array count %d and capacity %d\n", arr->count, arr->capacity);
  printf("[");
  for (int i = 0; i < arr->count; i++) // i gets too high, so count must get too high.
  {
    // if (i == 3)
    //   continue;
    // printf("Seg fault before\n");
    char *type = arr->elements[i].type;
    // printf("array->elements[%d]->type %s\n", i, type);

    // switch (type)
    // {
    if (type == "string")
    {
      printf("%s", arr->elements[i].element->s); 
    }
    else if (type == "int")
    {
      printf("%d", arr->elements[i].element->i);
    }
    else if (type == "float")
    {
      printf("%f", arr->elements[i].element->f); 
    }
    else if (type == "char")
    {
      printf("%c", arr->elements[i].element->c); 
    }
    else
    {
      printf("Not sure. Type is %s \n", type); // seg faults
    }
    // case "char":
    //   printf("%c", arr->elements[i].element->c); // seg faults
    //   break;
    // case "float":
    //   printf("%d", arr->elements[i].element->f); // seg faults
    //   break;
    // default:
    //   break;
    // }

    // printf("Seg fault after\n");

    if (i != arr->count - 1)
    {
      printf(",");
    }
    //  printf("array[%d]: %s  ", i, arr->elements[i]);
  }
  printf("]\n");
}

// /// STRETCH GOALS ///////
// /// STRETCH GOALS ///////
// /// STRETCH GOALS ///////
// /// STRETCH GOALS ///////
// /// STRETCH GOALS ///////

// int arr_clear(Array *arr)
// {
//   if (arr->count == 0)
//     return 0;
//   while (arr->count > 0)
//   {
//     arr->elements[arr->count - 1] = NULL;
//     arr->count--;
//   }
//   shrink_array(arr);
//   return 1;
// }

// int arr_pop_no_rtn(Array *arr)
// {
//   if (arr->count > 0)
//   {
//     arr->elements[arr->count - 1] = NULL;
//     arr->count--;
//     return 1;
//   }
//   else
//   {
//     return 0;
//   }
// }

// char *arr_pop(Array *arr, char *return_val_ptr) // TODO MAKE THIS A VARIADIC FUNCTION TO POP A GIVEN INDEX
// {
//   if (arr->count > 0)
//   {
//     int last_index = arr->count - 1;
//     int last_element_length = strlen(arr->elements[last_index]);
//     char *return_val_ptr = malloc(last_element_length); // DON'T WANT. Then you have to free it from the heap.
//     strcpy(return_val_ptr, arr->elements[last_index]);
//     arr->elements[last_index] = NULL;
//     arr->count--;
//     shrink_array(arr);
//     return return_val_ptr;
//   }
//   else
//   {
//     return NULL;
//   }
// }

// char *arr_pop_by_index(Array *arr, char *return_val_ptr, int index)
// {
//   if (arr->count > 0)
//   {
//     int element_length = strlen(arr->elements[index]);
//     char *return_val_ptr = malloc(element_length); // DON'T WANT. Then you have to free it from the heap.
//     strcpy(return_val_ptr, arr->elements[index]);
//     // arr->elements[index] = NULL;
//     memmove((arr->elements + index), arr->elements + index + 1, sizeof(char *) * (arr->count - (index + 1)));
//     arr->elements[arr->count - 1] = NULL;
//     arr->count--;
//     shrink_array(arr);
//     return return_val_ptr;
//   }
//   else
//   {
//     return NULL;
//   }
// }

// // char *base_arr_pop_can_index(int count, Array *arr, char *return_val_ptr, ...)
// // {
// //   printf("Count is:%d \n", count);
// //   if (count == 3)
// //   {
// //     return arr_pop(arr, return_val_ptr);
// //   }

// //   if (count > 3)
// //   {
// //     int index;
// //     printf("He wants to index!\n");
// //     va_list va;
// //     va_start(return_val_ptr, va);
// //     va_arg(va, int);
// //     index = va;

// //     printf("VA Count %d", count);
// //     printf("He wants to index at %d!\n", index);
// //     va_end(va);
// //   }
// //   else
// //   {
// //     printf("Count is: %d. He maybe doesn't want to index\n");
// //   }
// // }

// int arr_index(Array *arr, char *element)
// { // gets the index matching
//   int index = -1;
//   if (arr->count > 0)
//   {
//     for (int i = 0; i < arr->count; i++)
//     {
//       if (strcmp(arr->elements[i], element) == 0)
//       {
//         return i;
//       }
//     }
//   }
//   else
//   {
//     return index;
//   }
// }

// int get_va_cnt(int count, ...)
// {
//   va_list va;
//   va_start(va, count);
//   for (int i = 0; i < count; i++)
//   {
//     int n = va_arg(va, int);
//     printf("VarArg %d = %d\n", i, n);
//   }
// }

// void va_test(int count, Array *arr, ...)
// {
//   va_list va;
//   // int va_count = PP_NARG()
//   va_start(va, arr); // seg faults
//   printf("VarArg i:arg");
//   for (int i = 0; i < count; i++)
//   {
//     if (i == 1)
//       continue;
//     int n = va_arg(va, int);
//     printf(" {%d:%d} ", i, n);
//   }
//   printf("\n");
//   // int index = va_arg(va, int);
// }

// int arr_sort_A_2_Z(const void *stringA, const void *stringB)
// {

//   const char **intA = (const char **)stringA;
//   const char **intB = (const char **)stringB;

//   return strcmp(*intA, *intB);
// }

// int arr_sort_Z_2_A(const void *stringA, const void *stringB)
// {
//   const char **intA = (const char **)stringA;
//   const char **intB = (const char **)stringB;
//   return strcmp(*intB, *intA);
// }

// int arr_sort(Array *arr, int (*op)())
// {
//   qsort(arr->elements, arr->count, sizeof(char *), op);
//   return 1;
// }

// void swap(char **a, char **b)
// {
//   // GETTING THESE EXACT POINTER COMBINATIONS IS CRITICAL FOR SUCCESS AND NOT CRASHING FOR THAT MATTER
//   char *tmp = *a;
//   *a = *b;  // now both equal value of b
//   *b = tmp; // now they're swapped
// }

// int arr_reverse(Array *arr)
// {
//   int start = 0;
//   int end = arr->count - 1;

//   while (start < end)
//   {
//     swap(arr->elements[start], arr->elements[end]);
//     start++;
//     end--;
//   }
//   return 1;
// }

// int arr_extend(Array *arr1, Array *arr2)
// {
//   arr1->elements = realloc(arr1->elements, (arr1->count + arr2->count + 5) * sizeof(char *));
//   memcpy(arr1->elements + (arr1->count), arr2->elements, (arr2->count) * sizeof(char *));
//   arr1->capacity += (arr2->count + 5);
//   arr1->count += arr2->count;
//   // don't free anything. Keep the original.

//   return 1;
// }

// Array *arr_copy(Array *src)
// {

//   Array *dst = create_array(1);

//   dst->count = src->count;
//   dst->capacity = src->capacity;
//   dst->original_capacity = src->original_capacity;
//   dst->elements = realloc(dst->elements, dst->capacity * sizeof(char *));
//   printf("Resized dst successfully. Capacity:%d count:%d\n", dst->capacity, dst->count);
//   memcpy(dst->elements, src->elements, src->count * sizeof(char *));
//   printf("PRINTING DESTINATION IN FUNCTION SCOPE\n");
//   arr_print(dst);
//   return dst;
// }

// void init_dummy_array(Array *arr)
// {
//   arr_append(arr, "D");
//   arr_append(arr, "C");
//   arr_append(arr, "G");
//   arr_append(arr, "A");
//   arr_append(arr, "Babba");
//   arr_append(arr, "Bubba");
//   arr_append(arr, "Fudge");
//   arr_append(arr, "Zed");
//   arr_append(arr, "T");
// }

#ifndef TESTING
int main(void)
{
  Array *arr = create_array(7);
  arr_print(arr);
  arr_append(arr, new_element_s("String 1"));
  arr_print(arr);

  arr_append(arr, new_element_s("String2"));
  // arr_print(arr);

  arr_insert(arr, new_element_s("add"), 0);
  // arr_print(arr);

  arr_insert(arr, new_element_c('a'), 1);
  // arr_print(arr);

  arr_insert(arr, new_element_i(1), 1);
  arr_append(arr, new_element_f(4.5));
  arr_print(arr); // This breaks

  // arr_insert(arr, "STRING1", 0);
  // arr_print(arr);

  // arr_append(arr, "STRING4");
  // arr_print(arr);

  // arr_insert(arr, "STRING2", 0);
  // arr_print(arr);
  // arr_insert(arr, "STRING3", 1);
  // arr_print(arr);
  // arr_remove(arr, "STRING3");

  // arr_print(arr);
  // arr_insert(arr, "STRING3", 1);
  // arr_print(arr);
  // arr_insert(arr, "STRING3", 1);
  // arr_insert(arr, "STRING3", 1);
  // arr_insert(arr, "STRING3", 1);
  // arr_insert(arr, "STRING3", 1);
  // arr_insert(arr, "STRING3", 1);
  // arr_print(arr);

  // arr_remove_all(arr, "STRING3");
  // arr_insert(arr, "Ssdf", 1);
  // arr_insert(arr, "BDSDF", 1);
  // arr_insert(arr, "DDD", 2);
  // arr_insert(arr, "garlic", 1);
  // arr_insert(arr, "onions", 1);
  // arr_append(arr, "liverwurst");
  // arr_print(arr);
  // char *poppa = arr_pop(arr, poppa);
  // printf("This is the value of the popped string: %s\n", poppa);
  // arr_print(arr);
  // char *popbyindex = arr_pop_by_index(arr, popbyindex, arr_index(arr, "DDD"));
  // printf("This is the value of the popped by index string: %s\n", popbyindex);
  // arr_print(arr);
  // arr_clear(arr);
  // arr_print(arr);
  // printf("Array Cleared\n\n\n");
  // // arr_insert(arr, "STRING3", 1);

  // printf("\nCHECK SORTING:\n");
  // init_dummy_array(arr);
  // printf("UNSORTED ARRAY\n");
  // arr_print(arr);
  // printf(". Sorting alphabetically A to Z\n");
  // arr_sort(arr, arr_sort_A_2_Z);
  // printf("Array Sorted. Did it work?\n");
  // arr_print(arr);
  // printf("Now sorting Z-A, reverse alphabetically\n");
  // arr_sort(arr, arr_sort_Z_2_A);
  // arr_print(arr);
  // arr_clear(arr);

  // printf("\nTEST ARRAY REVERSE\n");
  // init_dummy_array(arr);
  // arr_print(arr);
  // arr_reverse(arr); // went bad
  // arr_print(arr);
  // printf("\nTEST ARRAY EXTEND\n");
  // arr_clear(arr);
  // init_dummy_array(arr);
  // arr_print(arr);
  // Array *arr2 = create_array(5);
  // arr_append(arr2, "Jesus");
  // arr_append(arr2, "SX");
  // arr_append(arr2, "GXX");
  // arr_insert(arr2, "Andy", 2);
  // arr_insert(arr2, "Carlos", 1);
  // arr_insert(arr2, "Quigon", 1);
  // arr_print(arr2);
  // printf("EXTENDING\n");
  // arr_extend(arr, arr2);
  // printf("CHECKING extended array\n");
  // arr_print(arr);

  // printf("\nTESTING ARRAY COPY\n");
  // Array *arr_cpy;
  // printf("Printing array to copy\n");
  // arr_print(arr);
  // printf("Printing blank array\n");
  // // arr_print(arr_cpy);
  // arr_cpy = arr_copy(arr);
  // printf("Printing finished copy. Did it work?\n");
  // arr_print(arr_cpy);

  // TURN ON TO SEE VARIABLE ARGUMENT TESTING
  // my_va_test(arr, 7, 1, 2, 4, 5);
  // my_va_test(arr);

  // char *wont_index = arr_pop_can_index(arr, wont_index);
  // printf("This is the value of the popped string: %s\n", wont_index);
  // char *can_index = arr_pop_can_index(arr, can_index, 5);
  // printf("This is the value of the popped string: %s\n", can_index);

  // my_get_va_cnt(1, 2, 3);
  // my_get_va_cnt(5, 5, 5, 5, 5);
  // my_get_va_cnt(9);
  // my_get_va_cnt(6);
  destroy_array(arr);

  return 0;
}
#endif

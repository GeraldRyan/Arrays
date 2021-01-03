#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array
{
  // can we add a dimensionality field?? GR
  int capacity;    // How many elements can this array hold?
  int count;       // How many states does the array currently hold?
  char **elements; // The string elements contained in the array
} Array;

/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array(int capacity)
{
  // Allocate memory for the Array struct
  struct Array *a = malloc(sizeof(struct Array));

  // Set initial values for capacity and count
  a->capacity = capacity;
  a->count = 0;

  // Allocate memory for elements
  a->elements = malloc(sizeof(char *) * capacity);

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
void resize_array(Array *arr)
{

  // Create a new element storage with double capacity
  arr->capacity *= 2;
  arr->elements = realloc(arr->elements, arr->capacity);

  // Copy elements into the new storage
  // memcpy(new_array->elements, arr->elements, arr->count);

  // Free the old elements array (but NOT the strings they point to)
  // WHY DO THIS WHEN I USED REALLOC?
  // Update the elements and capacity to new values
  // USING REALLOC
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
char *arr_read(Array *arr, int index)
{
  // Throw an error if the index is greater or equal to than the current count
  if (index > arr->count)
  {
    fprintf(stderr, "Index out of range in array_read function");
    return NULL;
  }

  // Otherwise, return the element at the given index
  return *(arr->elements + index);
}

/*****
 * Insert an element to the array at the given index
 *
 * Store the VALUE of the given string, not the REFERENCE
 *****/
void arr_insert(Array *arr, char *element, int index)
{
  // Throw an error if the index is greater than the current count
  if (index > arr->count)
  {
    fprintf(stderr, "Index out of range");
    printf("Index out of range in arr_insert\n");
    return;
  }
  // Resize the array if the number of elements is over capacity
  if (arr->count == arr->capacity)
    resize_array(arr);
  // Move every element after the insert index to the right one position
  memmove(arr->elements + ((index + 1)), arr->elements + (index), (arr->count - index) * sizeof(char *));
  // Copy the element (hint: use `strdup()`) and add it to the array
  *(arr->elements + (index)) = strdup(element);
  // Increment count by 1
  arr->count++;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element)
{

  // Resize the array if the number of elements is over capacity
  // or throw an error if resize isn't implemented yet.
  if (arr->count == arr->capacity)
  {
    printf("Array size and capacity are %d and %d, resp. Resizing array\n", arr->count, arr->capacity);
    resize_array(arr);
  }

  // Copy the element and add it to the end of the array
  *(arr->elements + arr->count) = strdup(element);

  // Increment count by 1
  arr->count++;
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element)
{
  printf("Searching for element %s\n", element);
  printf("Boolean condition %d\n", (element == *(arr->elements + 1)));
  for (int i = 0; i < arr->count; i++)
  {
    printf("i is %d and element is %s. Arr->elements is %s \n", i, element, *(arr->elements + i));
    if (strcmp(element, *(arr->elements + i)))
    {
      printf("Found the element %s!!\n", element);
      memmove(*(arr->elements + i), arr->elements + i + 1, sizeof(char *) * (arr->count - (i + 1)));
      return;
    }
  }
  fprintf(stderr, "Fprintf stderr: Array element not found\n");

  // Search for the first occurence of the element and remove it.
  // Don't forget to free its memory!

  // Shift over every element after the removed element to the left one position

  // Decrement count by 1
}

void arr_remove_all(Array *arr, char *element) // NB THIS IS NOT IN THE TEST FILE-- YET. THIS IS CUSTOM FUNCTION OF MY OWN.
{

  // Search for the occurence of the element and remove all such instances.
  // Don't forget to free its memory!

  // Shift over every element after the removed element to the left one position

  // Decrement count by number of removed elements
}

/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr)
{
  printf("Array count %d\n", arr->count);
  printf("[");
  for (int i = 0; i < arr->count; i++) // i gets too high, so count must get too high.
  {
    // if (i == 3)
    //   continue;

    printf("%s", arr->elements[i]);

    if (i != arr->count - 1)
    {
      printf(",");
    }
    //  printf("array[%d]: %s  ", i, arr->elements[i]);
  }
  printf("]\n");
}

#ifndef TESTING
int main(void)
{
  Array *arr = create_array(1);
  arr_print(arr);

  arr_insert(arr, "STRING1", 0);
  arr_print(arr);

  arr_append(arr, "STRING4");
  arr_print(arr);

  arr_insert(arr, "STRING2", 0);
  arr_print(arr);
  arr_insert(arr, "STRING3", 1); // breaks when inserting into pos 1;
  // printf("NOT BROKEN YET\n");
  // printf("CURRENT ARRAY COUNT:%d\n", arr->count);
  // printf("NOT BROKEN YET\n");

  arr_print(arr);

  arr_remove(arr, "STRING3");

  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif

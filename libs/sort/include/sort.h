#pragma once
#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * @param arr array
 * @param len length of array
 * @param szel size of element
 * @param ltfn less than function (arr[j] < arr[j + 1])
 * @param tmpel temp buffer for element
 */
extern void insertion_sort(void* arr, size_t len, size_t szel, bool (*ltfn)(void* a, void* b), void* tmpel);

#endif
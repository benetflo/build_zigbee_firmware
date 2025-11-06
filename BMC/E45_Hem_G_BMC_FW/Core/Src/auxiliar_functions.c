/**
  ******************************************************************************
  * @file           auxiliar_functions.c
  * @brief          Implementation file for auxiliary functions and definitions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 SEACOMP. All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component. If no LICENSE file is provided,
  * the software is provided AS-IS.
  *
  ******************************************************************************
  */

#include <stdio.h>


/**
 * @brief Calculates the length of a null-terminated string.
 *
 * This function counts the number of characters in the input string
 * until the null terminator ('\0') is reached. It does not include
 * the null terminator in the count.
 *
 * @param str Pointer to the null-terminated string whose length is to be calculated.
 *            It should not be NULL. If a NULL pointer is passed, the behavior is
 *            undefined.
 *
 * @return The length of the string (excluding the null terminator) as a size_t value.
 *
 * @note This function does not use the standard <string.h> library.
 */

size_t strlen(const char *str) {
  size_t length = 0;
  while (*str != '\0') {
    length++;
    str++;
  }
  return length;
}

/*
** malign.c for Malloc in /home/la-val_c/devel/PSU_2014_malloc
**
** Made by Cyril La Valle
** Login <la-val_c@epitech.eu>
**
** Started on  Tue Feb 10 16:48:08 2015 Cyril La Valle
** Last update Sun Feb 15 23:26:36 2015 Zadkiel Aslafy-Aharonian
*/

#include "malign.h"

size_t	malign(size_t n)
{
  if (n < 8)
    return (8);
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n |= n >> 32;
  n++;
  return (n);
}

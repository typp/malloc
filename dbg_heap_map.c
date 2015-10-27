/*
** dbg_heap_map.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 22:20:06 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 22:20:07 2015 Zadkiel Aslafy-Aharonian
*/

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "malloc.h"
#include "wide.h"
#include "fast.h"
#include "bins.h"
#include "debug.h"

extern t_memheap g_heapctl;

void	heap_fast_print(int fderr, t_memfast *fastctl, unsigned arena)
{
  while (fastctl)
    {
      dprintf(fderr,
	      "%012lx-%012lx fast %06zu arena %02u\n",
	      (uintptr_t) fastctl,
	      (uintptr_t) fastctl + MFASTSIZE,
	      (size_t) 0,
	      arena);
      fastctl = fastctl->next;
    }
}

void	heap_bins_print(int fderr, t_membins *binsctl, unsigned arena)
{
  while (binsctl)
    {
      dprintf(fderr,
	      "%012lx-%012lx bins %06zu arena %02u\n",
	      (uintptr_t) binsctl,
	      (uintptr_t) binsctl + binsctl->reserved_area,
	      (size_t) binsctl->used_area,
	      arena);
      binsctl = binsctl->next;
    }
}

void	heap_wide_print(int fderr, t_memwide *widectl, unsigned arena)
{
  while (widectl)
    {
      dprintf(fderr,
	      "%012lx-%012lx wide %06zu arena %02u\n",
	      (uintptr_t) widectl,
	      (uintptr_t) widectl + widectl->size,
	      (size_t) widectl->size,
	      arena);
      widectl = widectl->next;
    }
}

void		heap_print(int fderr)
{
  unsigned	idx;

  idx = 0;
  while (idx < g_heapctl.arena_count)
    {
      heap_fast_print(fderr, g_heapctl.arena_table[idx]->fastctl, idx);
      heap_bins_print(fderr, g_heapctl.arena_table[idx]->binsctl, idx);
      heap_wide_print(fderr, g_heapctl.arena_table[idx]->widectl, idx);
      ++idx;
    }
}

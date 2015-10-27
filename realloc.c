/*
** realloc.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 22:27:39 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:37:14 2015 Zadkiel Aslafy-Aharonian
*/

#include <string.h>
#include "malloc.h"
#include "arena.h"
#include "bins.h"
#include "wide.h"
#include "fast.h"

extern t_memheap g_heapctl;

size_t		realloc_get_size_mfast(t_memarena *arenactl, void *ptr)
{
  t_memfast	*fastctl;
  uintptr_t	fast_addr;

  fastctl = arenactl->fastctl;
  while (fastctl)
    {
      if (fastctl->magic != MMAGIC)
	malloc_fatal("realloc(): memory corruption (fast) at %p",
		     fastctl, true);
      fast_addr = (uintptr_t) fastctl + sizeof(*fastctl);
      if (fast_addr <= (uintptr_t) ptr &&
	  fast_addr + MFASTSEGLEN > (uintptr_t) ptr)
	{
	  if ((uintptr_t) ptr % MFAST_BLKSIZE == 0)
	    return (8);
	  else
	    malloc_fatal("realloc(): invalid pointer %p (fast)",
			 ptr, true);
	}
      fastctl = fastctl->next;
    }
  return (0);
}

size_t		realloc_get_size_mbins(t_memarena *arenactl, void *ptr)
{
  t_membins	*binsctl;
  t_mchunk	*chk;

  binsctl = arenactl->binsctl;
  chk = (t_mchunk *) ptr - 1;
  while (binsctl)
    {
      if ((uintptr_t) chk >= (uintptr_t) binsctl + sizeof(*binsctl) &&
	  (uintptr_t) chk < (uintptr_t) binsctl + binsctl->used_area)
	{
	  if ((uintptr_t) chk % MALLOC_ALIGNMENT == 0)
	    return (chk->size - sizeof(*chk));
	  else
	    malloc_fatal("realloc(): invalid pointer %p (bins)", chk, true);
	}
      binsctl = binsctl->next;
    }
  return (0);
}

size_t		realloc_get_size_mwide(t_memarena *arenactl, void *ptr)
{
  t_memwide	*widectl;
  uintptr_t	wide_addr;

  widectl = arenactl->widectl;
  while (widectl)
    {
      if (widectl->cipher != MCHUNK_CIPHER(*widectl))
	malloc_fatal("realloc(): memory corruption (wide) at %p",
		     widectl, true);
      wide_addr = (uintptr_t) widectl + sizeof(*widectl);
      if (wide_addr == (uintptr_t) ptr)
	{
	  if (wide_addr % MALLOC_ALIGNMENT != 0)
	    malloc_fatal("realloc(): invalid pointer %p (wide)", ptr, true);
	  return (widectl->size);
	}
      widectl = widectl->next;
    }
  return (0);
}

size_t		realloc_get_size(void *ptr, t_memarena **arena)
{
  unsigned	arena_idx;
  size_t	size;

  size = 0;
  arena_idx = 0;
  while (arena_idx < g_heapctl.arena_count)
    {
      size = realloc_get_size_mfast(g_heapctl.arena_table[arena_idx], ptr);
      if (size == 0)
	{
	  size = realloc_get_size_mbins(g_heapctl.arena_table[arena_idx], ptr);
	  if (size == 0)
	    size = realloc_get_size_mwide(g_heapctl.arena_table[arena_idx],
					  ptr);
	}
      if (size != 0)
	{
	  *arena = g_heapctl.arena_table[arena_idx];
	  return (size);
	}
      ++arena_idx;
    }
  return (size);
}

void		*realloc(void *ptr, size_t size)
{
  void		*memory;
  size_t	previous_size;
  t_memarena	*arena;

  memory = NULL;
  if (ptr == NULL)
    memory = malloc(size);
  else if (size == 0)
    free(ptr);
  else
    {
      previous_size = realloc_get_size(ptr, &arena);
      if (!previous_size)
	malloc_fatal("realloc(): invalid pointer %p", ptr, true);
      if (MIN(previous_size, size) > MBINS_MAX_CHUNKSIZE)
	memory = mwide_realloc(arena, ptr, size);
      else
	{
	  memory = malloc(size);
	  memcpy(memory, ptr, MIN(previous_size, size));
	  free(ptr);
	}
    }
  return (memory);
}

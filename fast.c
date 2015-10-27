/*
** fast.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:18:55 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:32:01 2015 Zadkiel Aslafy-Aharonian
*/

#define _GNU_SOURCE

#include <string.h>
#include <strings.h>
#include "malloc.h"
#include "fast.h"

extern t_memheap g_heapctl;

void	mfast_init(t_memfast *fastctl)
{
  fastctl->statemap = 0;
  fastctl->magic = MMAGIC;
  fastctl->next = NULL;
}

void		*mfast_malloc(t_memfast *fastctl)
{
  void		*memory;
  int64_t	first;

  memory = NULL;
  while (fastctl != NULL)
    {
      if (fastctl->magic != MMAGIC)
	{
	  malloc_fatal("malloc(): memory corruption (fast) at %p",
		       fastctl, true);
	}
      first = ffsll(~fastctl->statemap);
      if (first != 0)
	{
	  memory = (void *)((size_t) (fastctl + 1) + (size_t) (first - 1) * 8);
	  fastctl->statemap |= 1L << (first - 1L);
	  fastctl = NULL;
	}
      else
	fastctl = fastctl->next;
    }
  return (memory);
}

bool		mfast_free_do(t_memarena *arenactl,
			      t_memfast *fastctl,
			      void *addr)
{
  uintptr_t	fast_addr;

  if (fastctl->magic != MMAGIC)
    malloc_fatal("free(): memory corruption (fast) at %p", fastctl, true);
  fast_addr = (uintptr_t) fastctl + sizeof(*fastctl);
  if (fast_addr <= (uintptr_t) addr &&
      fast_addr + MFASTSEGLEN > (uintptr_t) addr)
    {
      if ((uintptr_t) addr % MFAST_BLKSIZE == 0)
	{
	  pthread_mutex_lock(&arenactl->mutex);
	  fastctl->statemap &=
	    ~(1 << (((uintptr_t) addr - fast_addr) / MFAST_BLKSIZE));
	  pthread_mutex_unlock(&arenactl->mutex);
	}
	else
	malloc_fatal("free(): invalid pointer %p (fast)", addr, true);
      return (true);
    }
  return (false);
}

bool		mfast_free(void *addr)
{
  t_memfast	*fastctl;
  unsigned	idx;

  idx = 0;
  while (idx < g_heapctl.arena_count)
    {
      fastctl = g_heapctl.arena_table[idx]->fastctl;
      while (fastctl)
	{
	  if (mfast_free_do(g_heapctl.arena_table[idx],
				   fastctl, addr))
	    return (true);
	  fastctl = fastctl->next;
	}
      ++idx;
    }
  return (false);
}

void		*mfast_mbrk(t_memarena *arenactl)
{
  t_memfast	*fastctl;

  fastctl = arenactl->fastctl;
  while (fastctl->next != NULL)
    fastctl = fastctl->next;
  fastctl->next = marenabrk(arenactl, MFASTSIZE);
  if (fastctl->next == NULL)
    return (NULL);
  fastctl = fastctl->next;
  mfast_init(fastctl);
  return (mfast_malloc(fastctl));
}

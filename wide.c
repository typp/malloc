/*
** wide.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:22:03 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:39:39 2015 Zadkiel Aslafy-Aharonian
*/

#include <string.h>
#include "malloc.h"
#include "bins.h"
#include "wide.h"

extern t_memheap g_heapctl;

void		mwide_init(t_memarena *arenactl, size_t size)
{
  arenactl->widectl = mmbrk(MWIDE_GETPAGES(size));
  memset(arenactl->widectl, 0, sizeof(* arenactl->widectl));
  arenactl->widectl->size = MWIDE_GETPAGED(size);
  arenactl->widectl->cipher = MCHUNK_CIPHER(*(arenactl->widectl));
}

void		*mwide_extends(t_memwide *widectl, size_t blksize)
{
  t_memwide	*new;

  while (widectl->next)
    widectl = widectl->next;
  if ((new = mmbrk(MWIDE_GETPAGES(blksize))) == NULL)
    return (NULL);
  new->size = MWIDE_GETPAGED(blksize) - sizeof(*new);
  new->next = NULL;
  new->prev = widectl;
  new->cipher = MCHUNK_CIPHER(*new);
  widectl->next = new;
  widectl->cipher = MCHUNK_CIPHER(*widectl);
  return (new);
}

void		*mwide_malloc(t_memarena *arenactl, size_t size)
{
  t_memwide	*memory;
  size_t	blksize;

  memory = NULL;
  blksize = MALIGN(size + 2 * sizeof(*memory));
  if (arenactl->widectl == NULL)
    mwide_init(arenactl, blksize);
  if (arenactl->widectl == NULL)
    return (NULL);
  memory = mwide_next_fit(arenactl, blksize);
  return (memory);
}

void		*mwide_realloc(t_memarena *arenactl,
			       t_memwide *blk,
			       size_t size)
{
  void		*new;

  blk = blk - 1;
  if (blk->cipher != MCHUNK_CIPHER(*blk))
    malloc_fatal("realloc(): memory corruption (wide) at %p", blk, true);
  if (size > blk->size && blk->next && blk->next->used == false)
    arenactl->next_fit = mwide_join(blk);
  if (blk->size >= size)
    return (blk + 1);
  if ((new = malloc(size)) != NULL)
    {
      memcpy(new, blk + 1, MIN(blk->size, size));
      blk->used = false;
      blk->cipher = MCHUNK_CIPHER(*blk);
      arenactl->next_fit = mwide_join(blk);
      return (new);
    }
  return (NULL);
}

bool		mwide_free(void *addr)
{
  t_memwide	*widectl;
  unsigned	idx;
  bool		ret;

  idx = 0;
  while (idx < g_heapctl.arena_count)
    {
      widectl = g_heapctl.arena_table[idx]->widectl;
      while (widectl)
	{
	  if ((ret = mwide_free_do(g_heapctl.arena_table[idx], widectl, addr)))
	    return (true);
	  widectl = widectl->next;
	}
      ++idx;
    }
  return (false);
}

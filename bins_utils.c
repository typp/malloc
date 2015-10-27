/*
** bins_utils.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 23:34:35 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:35:41 2015 Zadkiel Aslafy-Aharonian
*/

#include <strings.h>
#include <string.h>
#include "malloc.h"
#include "arena.h"
#include "bins.h"

extern t_memheap g_heapctl;

void		mchunk_partition_init(t_mchunk *chk, size_t size)
{
  t_mchunk	*first;
  uint8_t	ind;

  chk->ind = 0;
  chk->size = size;
  chk->statemap = 0;
  chk->next = 0;
  chk->cipher = MCHUNK_CIPHER(*chk);
  first = chk;
  ind = 1;
  chk = MCHUNKNEXT(chk);
  while (ind < (MPARTITIONSIZE / size))
    {
      chk->ind = ind;
      chk->size = size;
      chk->statemap = 0;
      chk->first = first;
      chk->cipher = MCHUNK_CIPHER(*chk);
      chk = MCHUNKNEXT(chk);
      ++ind;
    }
}

void	mbins_init(t_membins *binsctl, size_t reserved_area)
{
  memset(binsctl, 0, sizeof(*binsctl));
  binsctl->reserved_area = reserved_area;
  binsctl->future_chunk = (t_mchunk *) (binsctl + 1);
  binsctl->used_area = sizeof(*binsctl);
  binsctl->cipher = MCHUNK_CIPHER(*binsctl);
}

t_mchunk	*mbins_mpartition(t_membins *binsctl, size_t size)
{
  t_mchunk	*chk;

  if (binsctl->used_area + MPARTITIONSIZE <= binsctl->reserved_area)
    {
      chk = (t_mchunk *) ((uintptr_t) binsctl
			  + (uintptr_t) binsctl->used_area);
      mchunk_partition_init(chk, size);
      binsctl->used_area += MPARTITIONSIZE;
      binsctl->cipher = MCHUNK_CIPHER(*binsctl);
      return (chk);
    }
  return (NULL);
}

bool		mbins_free(void *ptr)
{
  t_membins	*binsctl;
  t_mchunk	*chunk;
  unsigned	idx;

  chunk = (t_mchunk *) ptr - 1;
  idx = 0;
  while (idx < g_heapctl.arena_count)
    {
      binsctl = g_heapctl.arena_table[idx]->binsctl;
      while (binsctl)
	{
	  if (binsctl->cipher != MCHUNK_CIPHER(*binsctl))
	    malloc_fatal("free(): memory corruption (bins) at %p",
			 binsctl, true);
	  if ((uintptr_t) chunk >= (uintptr_t) binsctl + sizeof(*binsctl) &&
	      (uintptr_t) chunk < (uintptr_t) binsctl + binsctl->used_area)
	    return (mbins_mchunk_free(g_heapctl.arena_table[idx],
				      binsctl, chunk));
	  binsctl = binsctl->next;
	}
      ++idx;
    }
  return (false);
}

void		*mbins_mbrk(t_memarena *arenactl, size_t blksize)
{
  t_membins	*binsctl;

  binsctl = arenactl->binsctl;
  while (binsctl->next != NULL)
    binsctl = binsctl->next;
  binsctl->next = marenabrk(arenactl, binsctl->reserved_area);
  binsctl->cipher = MCHUNK_CIPHER(*binsctl);
  if (binsctl->next == NULL)
    return (NULL);
  mbins_init(binsctl->next, binsctl->reserved_area);
  return (mbins_malloc(binsctl->next, blksize));
}

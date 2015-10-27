/*
** bins.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:20:51 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:36:56 2015 Zadkiel Aslafy-Aharonian
*/

#include <strings.h>
#include <string.h>
#include "malloc.h"
#include "arena.h"
#include "bins.h"

extern t_memheap g_heapctl;

t_mchunk	*mbins_malloc_on_chunk(t_membins *binsctl,
				       unsigned chunk_idx,
				       size_t blksize)
{
  t_mchunk	*memory;
  unsigned	free_chunk_idx;

  memory = binsctl->free_chunk[chunk_idx];
  if (MCHUNK_CIPHER(*memory) != memory->cipher)
    malloc_fatal("malloc(): memory corruption (bins) at %p", memory, true);
  MCHUNK_SETUSED(*memory);
  MCHUNK_GET_FIRST(*memory)->cipher =
    MCHUNK_CIPHER(*MCHUNK_GET_FIRST(*memory));
  free_chunk_idx = ffs(~MCHUNK_GET_FIRST(*memory)->statemap);
  if (free_chunk_idx == 0 || (free_chunk_idx - 1) * memory->size >= 256)
    {
      binsctl->free_chunk[chunk_idx] = mbins_mpartition(binsctl, blksize);
      MCHUNK_GET_FIRST(*memory)->next = binsctl->free_chunk[chunk_idx];
      MCHUNK_GET_FIRST(*memory)->cipher =
	MCHUNK_CIPHER(*MCHUNK_GET_FIRST(*memory));
    }
  else
    {
      binsctl->free_chunk[chunk_idx] = (t_mchunk *)
	((uintptr_t) MCHUNK_GET_FIRST(*memory) +
	 (uintptr_t) ((free_chunk_idx - 1) * (memory->size)));
    }
  binsctl->cipher = MCHUNK_CIPHER(*binsctl);
  return (memory);
}

t_mchunk	*mbins_malloc_new_chunk(t_membins *binsctl,
					unsigned chunk_idx,
					size_t blksize,
					t_mchunk *memory)
{
  t_mchunk	*it;

  if (!binsctl->chunk[chunk_idx])
    binsctl->chunk[chunk_idx] = memory;
  else
    {
      it = binsctl->chunk[chunk_idx];
      while (it->next)
	it = it->next;
      it->next = memory;
      it->cipher = MCHUNK_CIPHER(*it);
    }
  binsctl->free_chunk[chunk_idx] = (blksize < 256) ?
    MCHUNKNEXT(memory) : NULL;
  binsctl->cipher = MCHUNK_CIPHER(*binsctl);
  MCHUNK_SETUSED(*memory);
  MCHUNK_GET_FIRST(*memory)->cipher =
    MCHUNK_CIPHER(*MCHUNK_GET_FIRST(*memory));
  return (memory);
}

void		*mbins_malloc(t_membins *binsctl, size_t blksize)
{
  t_mchunk	*memory;
  unsigned	idx;

  memory = NULL;
  idx = (unsigned) ffs(blksize) - 5;
  while (binsctl)
    {
      if (binsctl->cipher != MCHUNK_CIPHER(*binsctl))
	malloc_fatal("malloc(): memory corruption (bins) at %p",
		     binsctl, true);
      if (binsctl->free_chunk[idx] != 0)
	{
	  memory = mbins_malloc_on_chunk(binsctl, idx, blksize) + 1;
	  binsctl = NULL;
	}
      else if ((memory = mbins_mpartition(binsctl, blksize)) != NULL)
	{
	  memory = mbins_malloc_new_chunk(binsctl, idx, blksize, memory) + 1;
	  binsctl = NULL;
	}
      else
	binsctl = binsctl->next;
    }
  return (memory);
}

bool	mbins_mchunk_free(t_memarena *arenactl,
			  t_membins *binsctl,
			  t_mchunk *chunk)
{
  if ((uintptr_t) chunk % MALLOC_ALIGNMENT == 0)
    {
      if (chunk->cipher != MCHUNK_CIPHER(*chunk))
	malloc_fatal("free(): memory corruption (bins) at %p", chunk, true);
      pthread_mutex_lock(&arenactl->mutex);
      MCHUNK_SETFREE(*chunk);
      binsctl->free_chunk[ffs(chunk->size) - 5] = chunk;
      binsctl->cipher = MCHUNK_CIPHER(*binsctl);
      chunk = MCHUNK_GET_FIRST(*chunk);
      chunk->cipher = MCHUNK_CIPHER(*chunk);
      pthread_mutex_unlock(&arenactl->mutex);
    }
  else
    malloc_fatal("free(): invalid pointer %p (bins)", chunk, true);
  return (true);
}

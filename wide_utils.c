/*
** wide_utils.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 22:22:47 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:39:21 2015 Zadkiel Aslafy-Aharonian
*/

#include <string.h>
#include "malloc.h"
#include "bins.h"
#include "wide.h"

void		mwide_split_set(t_memwide *blk, size_t size,
				bool used, t_memwide *prev,
				t_memwide *next)
{
  blk->size = size;
  blk->used = used;
  blk->prev = prev;
  blk->next = next;
  blk->cipher = MCHUNK_CIPHER(*blk);
}

void		*mwide_split(t_memwide *blk, size_t size)
{
  t_memwide	*next;

  if (!blk)
    return (NULL);
  if (size == blk->size)
    {
      blk->used = true;
      blk->cipher = MCHUNK_CIPHER(*blk);
      return (blk);
    }
  next = (t_memwide *) ((uintptr_t) blk + sizeof(*blk) + size);
  mwide_split_set(next, blk->size - size - sizeof(*blk),
		  false, blk, blk->next);
  if (next->next != NULL)
    {
      next->next->prev = next;
      next->next->cipher = MCHUNK_CIPHER(*next->next);
    }
  mwide_split_set(blk, size, true, blk->prev, next);
  if (blk->prev != NULL)
    {
      blk->prev->next = blk;
      blk->prev->cipher = MCHUNK_CIPHER(*blk->prev);
    }
  return (blk);
}

void		mwide_join_link(t_memwide *blk)
{
  if (blk->prev != NULL)
    {
      blk->prev->next = blk;
      blk->prev->cipher = MCHUNK_CIPHER(*blk->prev);
    }
  if (blk->next != NULL)
    {
      blk->next->prev = blk;
      blk->next->cipher = MCHUNK_CIPHER(*blk->next);
    }
  blk->cipher = MCHUNK_CIPHER(*blk);
}

void		*mwide_join(t_memwide *blk)
{
  if (blk == NULL)
    return (NULL);
  if (blk->used == false)
    while (blk->prev && blk->prev->used == false &&
  	   (uintptr_t) blk->prev == (uintptr_t) (blk - 1) - blk->size)
      {
  	blk->prev->size += blk->size + sizeof(*blk);
  	blk->prev->next = blk->next;
  	blk = blk->prev;
      }
  while (blk->next && blk->next->used == false &&
  	 (uintptr_t) blk->next == (uintptr_t) (blk + 1) + blk->size)
    {
      blk->size += blk->next->size + sizeof(*blk);
      blk->next = blk->next->next;
    }
  mwide_join_link(blk);
  return (blk);
}

bool		mwide_free_do(t_memarena *arenactl,
			      t_memwide *widectl,
			      void *addr)
{
  uintptr_t	wide_addr;

  if (widectl->cipher != MCHUNK_CIPHER(*widectl))
    malloc_fatal("free(): memory corruption (wide) at %p",
		 widectl, true);
  wide_addr = (uintptr_t) widectl + sizeof(*widectl);
  if (wide_addr == (uintptr_t) addr)
    {
      if (wide_addr % MALLOC_ALIGNMENT != 0)
	malloc_fatal("free(): invalid pointer %p (wide)", addr, true);
      pthread_mutex_lock(&arenactl->mutex);
      widectl->used = false;
      widectl->cipher = MCHUNK_CIPHER(*widectl);
      arenactl->next_fit = mwide_join(widectl);
      pthread_mutex_unlock(&arenactl->mutex);
      return (true);
    }
  return (false);
}

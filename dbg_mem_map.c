/*
** dbg_mem_map.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 22:20:01 2015 Zadkiel Aslafy-Aharonian
** Last update Fri Feb 20 16:02:33 2015 Zadkiel Aslafy-Aharonian
*/

#define	_GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "malloc.h"
#include "wide.h"
#include "fast.h"
#include "bins.h"
#include "debug.h"

extern t_memheap g_heapctl;

void	mwide_show(int fderr, t_memwide *widectl)
{
  while (widectl)
    {
      dprintf(fderr,
	      "%p - %p : %zu bytes\n", widectl + 1,
	      (void *)((size_t)(widectl + 1) + widectl->size),
	      widectl->size);
      widectl = widectl->next;
    }
}

void		mbins_show_by_type(int fderr, t_membins *binsctl,
				   unsigned chunk_type_idx)
{
  t_mchunk	*chunk;
  uint16_t	bstate;

  chunk = binsctl->chunk[chunk_type_idx];
  while (chunk != NULL)
    {
      bstate = 1;
      while (bstate < (uint16_t) sizeof(bstate) * 8)
	{
	  if (chunk->statemap & bstate)
	    dprintf(fderr,
		    "%p - %p : %zu bytes\n",
		    (void *) ((uintptr_t) (chunk + 1)
			      + (ffs(bstate) - 1) * chunk->size),
		    (void *) ((uintptr_t) (chunk)
			      + (ffs(bstate)) * (chunk->size)),
		    chunk->size - sizeof(*chunk));
	  bstate <<= 1;
	}
      chunk = chunk->next;
    }
}

void		mbins_show(int fderr, t_membins *binsctl)
{
  unsigned	chunk_type_idx;

  while (binsctl)
    {
      chunk_type_idx = 0;
      while (chunk_type_idx < 5)
	{
	  mbins_show_by_type(fderr, binsctl, chunk_type_idx);
	  ++chunk_type_idx;
	}
      binsctl = binsctl->next;
    }
}

void		mfast_show(int fderr, t_memfast *fastctl)
{
  unsigned	state_idx;
  int64_t	state;

  while (fastctl)
    {
      state = 1L;
      state_idx = 0;
      while (state_idx < 64)
	{
	  if (fastctl->statemap & state)
	    dprintf(fderr,
		    "%p - %p : 8 bytes\n",
		    (void *) ((uintptr_t) (fastctl + 1)
			      + (ffsll(state) - 1) * 8),
		    (void *) ((uintptr_t) (fastctl + 1)
			      + (ffsll(state) - 1) * 8 + 8));
	  state <<= 1L;
	  ++state_idx;
	}
      fastctl = fastctl->next;
    }
}

void		show_alloc_mem()
{
  unsigned	idx;
  int		fderr;

  fderr = 2;
  idx = 0;
  dprintf(fderr, "break : %p\n", (void *) g_heapctl.ds_start);
  while (idx < g_heapctl.arena_count)
    {
      mfast_show(fderr, g_heapctl.arena_table[idx]->fastctl);
      mbins_show(fderr, g_heapctl.arena_table[idx]->binsctl);
      mwide_show(fderr, g_heapctl.arena_table[idx]->widectl);
      ++idx;
    }
}

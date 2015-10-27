/*
** malloc.c for Malloc in /home/la-val_c/rendu/PSU_2013_malloc
**
** Made by Cyril La Valle
** Login <la-val_c@epitech.eu>
**
** Started on  Wed Feb  4 19:15:07 2015 Cyril La Valle
** Last update Sun Feb 15 23:29:52 2015 Zadkiel Aslafy-Aharonian
*/

#define _GNU_SOURCE

#include <string.h>
#include <libgen.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"
#include "arena.h"
#include "bins.h"
#include "wide.h"
#include "fast.h"

static pthread_once_t malloc_once_control = PTHREAD_ONCE_INIT;
t_memheap g_heapctl;

static void	malloc_init(void)
{
  g_heapctl.ds_start = (size_t) sbrk(0);
  g_heapctl.ds_end = g_heapctl.ds_start;
  pthread_key_create(&g_heapctl.tkey_arenactl, NULL);
  g_heapctl.pagesize = sysconf(_SC_PAGESIZE);
  g_heapctl.arena_count = 0;
  g_heapctl.corrupted = 0;
}

void		*malloc(size_t size)
{
  t_memarena	*arena;
  void		*memory;

  memory = NULL;
  if (!g_heapctl.corrupted)
    {
      pthread_once(&malloc_once_control, malloc_init);
      arena = malloc_get_arena(&g_heapctl);
      if (arena != NULL)
  	{
  	  memory = marena_malloc(arena, size);
  	  pthread_mutex_unlock(&arena->mutex);
  	}
    }
  return (memory);
}

void	*mmbrk(size_t pages)
{
  void	*ptr;

  g_heapctl.ds_end += g_heapctl.pagesize * pages;
  ptr = sbrk(g_heapctl.pagesize * pages);
  if (ptr == (void *) -1)
    return (NULL);
  return (ptr);
}

void	free(void *ptr)
{
  if (ptr == NULL)
    return;
  if (!mfast_free(ptr) && !mbins_free(ptr) && !mwide_free(ptr))
    malloc_fatal("free(): invalid pointer %p (not in mapped region)",
		 ptr, true);
}

void	*calloc(size_t nmemb, size_t size)
{
  void	*ptr;

  if (nmemb == 0 || size == 0)
    return (NULL);
  ptr = malloc(nmemb * size);
  if (ptr)
    memset(ptr, 0, nmemb * size);
  return (ptr);
}

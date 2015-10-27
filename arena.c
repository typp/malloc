/*
** arena.c for Malloc in /home/la-val_c/rendu/PSU_2013_malloc
**
** Made by Cyril La Valle
** Login <la-val_c@epitech.eu>
**
** Started on  Fri Feb  6 19:04:57 2015 Cyril La Valle
** Last update Sun Feb 15 22:37:53 2015 Zadkiel Aslafy-Aharonian
*/

#include "malloc.h"
#include "arena.h"
#include "fast.h"
#include "bins.h"
#include "wide.h"

extern t_memheap g_heapctl;

/***  ARENAS  ***********************************************************
** Default arena:
** 0                  struct s_memarena
** +sizeof(memarena)  struct s_memfast
** +sizeof(memfast)   [fast]
** +(8 * 64)          struct s_membins
** +sizeof(membins)   [bins]
** ...
** end-of-page
**
** (allocations for memwide allocated on other pages, on-demand)
*/

/*
** create a new arena for the requesting thread
** (allocating a new page in memory)
*/
static pthread_mutex_t create_arena_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*mbrk(void)
{
  return (mmbrk(1));
}

t_memarena	*malloc_create_arena(t_memheap *heapctl)
{
  t_memarena	*arena;

  arena = mbrk();
  if (arena != NULL)
    {
      pthread_mutex_init(&arena->mutex, NULL);
      arena->fastctl = (t_memfast *) (arena + 1);
      arena->binsctl = (t_membins *) ((uintptr_t) (arena + 1)
				      + (uintptr_t) MFASTSIZE);
      arena->widectl = NULL;
      arena->free_addr = NULL;
      arena->next_fit = NULL;
      mfast_init(arena->fastctl);
      mbins_init(arena->binsctl,
		 (uintptr_t) arena + g_heapctl.pagesize -
		 (uintptr_t) arena->binsctl);
      pthread_mutex_lock(&create_arena_mutex);
      heapctl->arena_table[heapctl->arena_count] = arena;
      ++heapctl->arena_count;
      pthread_mutex_unlock(&create_arena_mutex);
    }
  return (arena);
}

/*
** get or create an arena linked to the requesting thread
** and lock it with a mutex
*/
t_memarena	*malloc_get_arena(t_memheap *heapctl)
{
  t_memarena	*arenactl;

  arenactl = pthread_getspecific(heapctl->tkey_arenactl);
  if (arenactl == NULL)
    {
      arenactl = malloc_create_arena(heapctl);
      if (arenactl != NULL)
	{
	  pthread_setspecific(heapctl->tkey_arenactl, arenactl);
	  pthread_mutex_lock(&arenactl->mutex);
	}
    }
  else
    pthread_mutex_lock(&arenactl->mutex);
  return (arenactl);
}

void		*marena_malloc(t_memarena *arenactl, size_t size)
{
  void		*memory;
  size_t	blksize;

  blksize = MALIGN(size);
  if (blksize == 8)
    {
      memory = mfast_malloc(arenactl->fastctl);
      if (memory == NULL)
	memory = mfast_mbrk(arenactl);
    }
  else
    {
      blksize = MALIGN(size + sizeof(t_mchunk));
      if (blksize <= 256)
	{
	  memory = mbins_malloc(arenactl->binsctl, blksize);
	  if (memory == NULL)
	    memory = mbins_mbrk(arenactl, blksize);
	}
      else
	{
	  memory = mwide_malloc(arenactl, size);
	}
    }
  return (memory);
}

void	*marenabrk(t_memarena *arenactl, size_t size)
{
  void	*memory;

  if (arenactl->free_addr != NULL &&
      (uintptr_t) arenactl->end_addr - (uintptr_t) arenactl->free_addr >= size)
    {
      memory = (void *) arenactl->free_addr;
      arenactl->free_addr = (void *) ((uintptr_t) arenactl->free_addr + size);
    }
  else
    {
      memory = mbrk();
      if (memory == (void *) -1)
	memory = NULL;
      else
	{
	  arenactl->end_addr = (void *) g_heapctl.ds_end;
	  arenactl->free_addr = (void *) ((uintptr_t) memory + size);
	}
    }
  return (memory);
}

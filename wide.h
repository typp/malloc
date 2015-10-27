/*
** wide.h for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:16:35 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 22:33:00 2015 Zadkiel Aslafy-Aharonian
*/

#ifndef __WIDE_H__
# define __WIDE_H__

# include "malloc.h"
# include "arena.h"

# define MWIDE_GETPAGES(s)	(((s) / g_heapctl.pagesize + 1))
# define MWIDE_GETPAGED(s)	((MWIDE_GETPAGES(s)) * g_heapctl.pagesize)

/*
** wide:
** arbitrary re-arranged length (fit MALLOC_LARGE_ALIGNMENT align)
** linked-list of user-allocated area, for sizes superior to 256 bytes
*/
struct		s_memwide
{
  uint8_t	cipher;
  bool		used;
  size_t	size;
  t_memwide	*next;
  t_memwide	*prev;
};

typedef struct s_memwide t_memwide;

void	*mwide_malloc(t_memarena *arenactl, size_t size);
void	*mwide_mbrk(size_t blksize, size_t *sizeptr);
void	*mwide_split(t_memwide *blk, size_t size);
void	*mwide_join(t_memwide *blk);
void	*mwide_extends(t_memwide *blk, size_t size);
bool	mwide_free(void *addr);
bool	mwide_free_do(t_memarena *arenactl, t_memwide *widectl, void *addr);
void	*mwide_realloc(t_memarena *arena, t_memwide *blk, size_t size);
void	*mwide_first_fit(t_memwide *widectl, size_t size);
void	*mwide_next_fit(t_memarena *arenactl, size_t size);

#endif

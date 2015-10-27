/*
** arena.h for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:11:22 2015 Zadkiel Aslafy-Aharonian
** Last update Fri Feb 13 13:43:20 2015 Zadkiel Aslafy-Aharonian
*/

#ifndef __ARENA_H__
# define __ARENA_H__

typedef struct s_memfast t_memfast;
typedef struct s_membins t_membins;
typedef struct s_memwide t_memwide;

struct			s_memarena
{
  pthread_mutex_t	mutex;
  t_memfast		*fastctl;
  t_membins		*binsctl;
  t_memwide		*widectl;
  t_memwide		*next_fit;
  void			*free_addr;
  void			*end_addr;
};

typedef struct s_memarena t_memarena;

t_memarena	*malloc_create_arena();
t_memarena	*malloc_get_arena(t_memheap *heapctl);
void		*marena_malloc(t_memarena *arenactl, size_t size);
void		*marenabrk(t_memarena *arenactl, size_t size);

#endif

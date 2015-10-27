/*
** malloc.h for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:09:34 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 22:58:45 2015 Zadkiel Aslafy-Aharonian
*/

#ifndef		__MALLOC_H__
# define	__MALLOC_H__

# include <stddef.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include "malign.h"
# include "debug.h"

# ifndef MALLOC_ALIGNMENT
#  define MALLOC_ALIGNMENT 8
# endif

# ifndef MALLOC_OFF_ALIGNMENT
#  define MALLOC_OFF_ALIGNMENT 8
# endif

# ifndef MALLOC_ARENA_MAX
#  define MALLOC_ARENA_MAX 64
# endif

# define MIN(a, b) ((a) < (b) ? (a) : (b))
# define MAX(a, b) ((a) > (b) ? (a) : (b))

# define ABS_DIFFERENCE(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

/*
** ALIGNMENT MACROS
*/

# define PACKED __attribute__((packed))

# define MOFFSET(offset) \
  ((offset) - ((offset) & (MALLOC_OFF_ALIGNMENT - 1)))

# define MALIGN(size) malign(size)

typedef struct s_memarena t_memarena;

struct		s_memheap
{
  size_t	ds_start;
  size_t	ds_end;
  size_t	pagesize;
  pthread_key_t	tkey_arenactl;
  size_t	arena_count;
  t_memarena	*arena_table[MALLOC_ARENA_MAX];
  bool		corrupted;
};

typedef struct s_memheap t_memheap;

void	*malloc(size_t size);
void	*calloc(size_t nbmem, size_t size);
void	malloc_fatal(const char *message, void *addr, bool mmap);
void	malloc_print_mmap(int);
void	malloc_print_bt(int);
void	*mbrk(void);
void	*mmbrk(size_t);
void	free(void *);

int	backtrace_no_malloc(void **array, int size);

#endif

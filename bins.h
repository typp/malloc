/*
** bins.h for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:12:57 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:36:37 2015 Zadkiel Aslafy-Aharonian
*/

#ifndef		__BINS_H__
# define	__BINS_H__

# include "malloc.h"
# include "arena.h"
# include "pearson.h"

/*
** MCHUNK MACROS
*/

# define MCHUNKNEXT(chunk) \
  ((t_mchunk *) ((uintptr_t) (chunk) + (uintptr_t) (chunk)->size))

# define MCHUNK_ISFREE_BT(m) \
  (((m).statemap & (1 << (m).ind)) == 0)

# define MCHUNK_ISFREE(m) \
  MCHUNK_BRANCH_TOP(m, MCHUNK_ISFREE_BT)

# define MCHUNK_SETUSED(m) \
  (MCHUNK_GET_FIRST(m)->statemap |= 1 << (m).ind)

# define MCHUNK_SETFREE(m) \
  (MCHUNK_GET_FIRST(m)->statemap &= ~(1 << (m).ind))

# define MCHUNK_GET_FIRST(m) \
  ((m).ind == 0 ? &m : ((m).first))

# define MCHUNK_BRANCH_TOP(m, func) \
  ((m).ind == 0 ? func(m) : func(*((m).first)))

# define MCHUNK_ISLAST(mchunk) \
  ((mchunk).size == (mchunk).ind + 1)

# define MCHUNK_CIPHER(m) \
  pearson((&((m).cipher)) + 1, sizeof(m) - sizeof((m).cipher))

# define MPARTITIONSIZE ((size_t) 256)

# define MBINS_MAX_CHUNKSIZE ((size_t) (MPARTITIONSIZE - sizeof(t_mchunk)))

/*
** bins:
** pointers to start of each binchunk linked-list,
** then pointers to first free element of each binchunk linked-list,
*/
# define		MBINCHUNK16	0
# define		MBINCHUNK32	1
# define		MBINCHUNK64	2
# define		MBINCHUNK128	3
# define		MBINCHUNK256	4

typedef struct s_mchunk t_mchunk;
typedef struct s_membins t_membins;

struct		s_membins
{
  uint8_t	cipher;
  t_mchunk	*chunk[5];
  t_mchunk	*free_chunk[5];
  t_mchunk	*future_chunk;
  size_t	used_area;
  size_t	reserved_area;
  t_membins	*next;
};

/*
** mchunk (10~14B):
** 256 byte user-space area for small allocs (add heads size to this)
*/
struct		s_mchunk
{
  // byte to check integrity of chunk
  uint8_t	cipher;

  // position in partition
  uint8_t	ind;

  // size of chunk (16, 32, 64, 128, 256)
  // maybe is better to use flags for that, but more easy for go back/forth
  uint16_t	size;

  // where to store free/alloc state for whole partition
  // set only for first chunk in partition (ind == 0)
  uint16_t	statemap;

  union
    {
      // pointer-to-first chunk in partition
      t_mchunk	*first;

      // pointer to next partition (only in first)
      t_mchunk	*next;
    };
};

void		mbins_init(t_membins *binsctl, size_t reserved_area);
void		*mbins_malloc(t_membins *binsctl, size_t blksize);
void		mchunk_partition_init(t_mchunk *chk, size_t size);
t_mchunk	*mbins_mpartition(t_membins *binsctl, size_t size);
bool		mbins_free(void *ptr);
void		*mbins_mbrk(t_memarena *arenactl, size_t blksize);
bool		mbins_mchunk_free(t_memarena *arenactl,
				  t_membins *binsctl,
				  t_mchunk *chunk);

#endif

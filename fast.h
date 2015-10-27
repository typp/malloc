/*
** fast.h for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc/cyril
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Mon Feb  9 13:14:40 2015 Zadkiel Aslafy-Aharonian
** Last update Thu Feb 12 16:17:09 2015 Zadkiel Aslafy-Aharonian
*/

#ifndef		__FAST_H__
# define	__FAST_H__

# include "malloc.h"
# include "arena.h"

/*
** for fast only
*/
# define MMAGIC ((t_malloc) 0xBEAD)
typedef uint16_t t_malloc;

/*
** fast:
** a 64 byte bit state array (allocated/free) correspunding of
** header-following aligned blocks of 8 bytes (no head/foot per block) ;
** the fast contains also a pointer to the next allocated fast ;
**
*/
struct		s_memfast
{
  int64_t	statemap;
  t_malloc	magic;
  t_memfast	*next;
};

typedef struct s_memfast t_memfast;

# define	MFAST_BLKCOUNT	(64)
# define	MFAST_BLKSIZE	(8)
# define	MFASTSEGLEN	(MFAST_BLKCOUNT * MFAST_BLKSIZE)
# define	MFASTSIZE	(MFASTSEGLEN + sizeof(t_memfast))

void	mfast_init(t_memfast *fastctl);
void	*mfast_malloc(t_memfast *fastctl);
bool	mfast_free(void *ptr);
void	*mfast_mbrk(t_memarena *arenactl);

#endif

/*
** backtrace.h for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Thu Feb 12 17:07:47 2015 Zadkiel Aslafy-Aharonian
** Last update Fri Feb 13 16:10:12 2015 Zadkiel Aslafy-Aharonian
*/

#ifndef __GLIBC_BACKTRACE_NO_MALLOC_H__
# define __GLIBC_BACKTRACE_NO_MALLOC_H__

# ifndef CURRENT_STACK_FRAME
#  define CURRENT_STACK_FRAME ({ char __csf; &__csf; })
# endif

# ifndef INNER_THAN
#  define INNER_THAN <
# endif

# ifndef ADVANCE_STACK_FRAME
#  define ADVANCE_STACK_FRAME(next) ((struct layout *) (next))
# endif

# ifndef FIRST_FRAME_POINTER
#  define FIRST_FRAME_POINTER __builtin_frame_address (0)
# endif

struct layout
{
 void *next;
 void *return_address;
};

#endif

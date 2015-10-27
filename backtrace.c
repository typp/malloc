/*
** backtrace.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Thu Feb 12 17:04:10 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:32:32 2015 Zadkiel Aslafy-Aharonian
*/

#include <execinfo.h>
#include <signal.h>
#include "backtrace.h"

extern void *__libc_stack_end;

int		backtrace_no_malloc(void **array, int size)
{
  struct layout	*current;
  void		*top_frame;
  void		*top_stack;
  int		cnt;

  cnt = 0;
  top_frame = FIRST_FRAME_POINTER;
  top_stack = CURRENT_STACK_FRAME;
  current = ((struct layout *) top_frame);
  while (cnt < size)
    {
      if ((void *) current INNER_THAN top_stack
	  || !((void *) current INNER_THAN __libc_stack_end))
	break;
      array[cnt++] = current->return_address;
      current = ADVANCE_STACK_FRAME(current->next);
    }
  return (cnt);
}

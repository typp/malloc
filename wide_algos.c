/*
** wide_algos.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 22:25:49 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 22:26:01 2015 Zadkiel Aslafy-Aharonian
*/

#include <string.h>
#include "malloc.h"
#include "bins.h"
#include "wide.h"

void		*mwide_first_fit(t_memwide *widectl, size_t size)
{
  t_memwide	*it;
  t_memwide	*memory;

  memory = NULL;
  it = widectl;
  while (it != NULL && memory == NULL)
    {
      if (it->size >= size + sizeof(*it) && it->used == false)
	memory = mwide_split(it, size);
      it = it->next;
    }
  if (memory == NULL)
    {
      memory = mwide_extends(widectl, size);
      memory = mwide_split(memory, size);
    }
  return ((!memory) ? memory : memory + 1);
}

void		*mwide_next_fit(t_memarena *arenactl, size_t size)
{
  t_memwide	*it;
  t_memwide	*memory;
  bool		restarted;

  restarted = false;
  memory = NULL;
  it = (!arenactl->next_fit) ? arenactl->widectl : arenactl->next_fit;
  while (it != NULL && memory == NULL)
    {
      if (it->size >= size + sizeof(*it) && it->used == false)
	memory = mwide_split(it, size);
      if (!it->next && !restarted && (restarted = true))
	it = arenactl->widectl;
      else
	it = it->next;
    }
  if (memory == NULL)
    {
      memory = mwide_extends(arenactl->widectl, size);
      memory = mwide_split(memory, size);
    }
  if (memory)
    arenactl->next_fit = memory;
  return ((!memory) ? memory : memory + 1);
}

/*
** fatal.c for Malloc in /home/aslafy_z/rendu/PSU_2014_malloc
**
** Made by Zadkiel Aslafy-Aharonian
** Login   <aslafy_z@epitech.net>
**
** Started on  Sun Feb 15 22:29:02 2015 Zadkiel Aslafy-Aharonian
** Last update Sun Feb 15 23:38:28 2015 Zadkiel Aslafy-Aharonian
*/

#include <string.h>
#include <libgen.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"
#include "backtrace.h"

extern t_memheap g_heapctl;

void		malloc_print_bt(int fderr)
{
  void		*bt[1024];
  ssize_t	bt_size;

  dprintf(fderr, "======= Backtrace: =========\n");
  bt_size = backtrace_no_malloc(bt, 1024);
  if (bt_size > 0)
    backtrace_symbols_fd(bt, bt_size, 2);
}

void		malloc_print_mmap(int fderr)
{
  int		fd;
  char		buffer[256];
  ssize_t	bytes;

  dprintf(fderr, "======= Memory map: ========\n");
  fd = open("/proc/self/maps", O_RDONLY);
  if (fd != 0)
    {
      while ((bytes = read(fd, &buffer, 256)) > 0)
	write(fderr, buffer, (size_t) bytes);
      close(fd);
    }
}

void		malloc_fatal(const char *message, void *addr, bool mmap)
{
  char		exe[1024];
  int		fderr;

  g_heapctl.corrupted = 1;
  fderr = open("/dev/tty", O_WRONLY | O_NOCTTY | O_NDELAY);
  if (fderr < 0)
    fderr = STDERR_FILENO;
  if (mmap)
    {
      dprintf(fderr, "*** Error ");
      memset(exe, 0, sizeof(exe));
      if (readlink("/proc/self/exe", exe, sizeof(exe)) != -1)
	dprintf(fderr, "in `./%s': ", basename(exe));
      dprintf(fderr, message, (uintptr_t) addr);
      dprintf(fderr, " ***\n");
      malloc_print_bt(fderr);
      malloc_print_mmap(fderr);
    }
  else
    {
      dprintf(fderr, message, addr);
    }
  abort();
}

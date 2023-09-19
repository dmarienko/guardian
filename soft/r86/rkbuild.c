/*
 * This file is part of Radio-86RK on JavaScript project.
 *
 * The project builder.
 *
 * Copyright (C) 2009 Alexander Demin <alexander@demin.ws>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump(const char* name) {
  FILE* f = fopen(name, "r");

  if (!f) {
    fprintf(stderr, "unable open [%s]\n", name);
    exit(1);
  }

  while (!feof(f)) {
    char line[1024];
    *line = 0;
    fgets(line, sizeof(line), f);
    if (!strlen(line)) break;
    printf("%s", line);
  }

  fclose(f);
}

int main(int argc, char* argv[]) {

  while (!feof(stdin)) {
    char line[1024];

    fgets(line, sizeof(line), stdin);
    if (!strlen(line)) break;

    if (line[0] == '@') {
      char* p = line;
      while (*p && *p != '\r' && *p != '\n') p++;
      *p = 0;
      dump(line + 1);
    } else
      printf("%s", line);
  }

  return 0;
}

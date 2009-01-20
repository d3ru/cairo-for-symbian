/* guniprop.c - Unicode character properties.
 *
 * Copyright (C) 1999 Tom Tromey
 * Copyright (C) 2000 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <glib.h>
#include "gscripttable.h"

#define G_N_ELEMENTS(arr)		(sizeof (arr) / sizeof ((arr)[0]))
#define G_SCRIPT_TABLE_MIDPOINT (G_N_ELEMENTS (g_script_table) / 2)

static inline GUnicodeScript
g_unichar_get_script_bsearch (gunichar ch)
{
  int lower = 0;
  int upper = G_N_ELEMENTS (g_script_table) - 1;
  static int saved_mid = G_SCRIPT_TABLE_MIDPOINT;
  int mid = saved_mid;


  do
    {
      if (ch < g_script_table[mid].start)
        upper = mid - 1;
      else if (ch >= g_script_table[mid].start + g_script_table[mid].chars)
        lower = mid + 1;
      else
        return g_script_table[saved_mid = mid].script;

      mid = (lower + upper) / 2;
    }
  while (lower <= upper);

  return G_UNICODE_SCRIPT_UNKNOWN;
}

GUnicodeScript
g_unichar_get_script (gunichar ch)
{
  if (ch < G_EASY_SCRIPTS_RANGE)
    return g_script_easy_table[ch];
  else
    return g_unichar_get_script_bsearch (ch);
}

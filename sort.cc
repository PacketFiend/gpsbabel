/*
    Arbitrary Sorting Filter(s)

    Copyright (C) 2004 Robert Lipe, robertlipe+source@gpsbabel.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111 USA

 */
#include "defs.h"
#include "filterdefs.h"
#include <cstdlib>

#if FILTERS_ENABLED

typedef enum {
  sm_unknown = 0,
  sm_gcid,
  sm_shortname,
  sm_description,
  sm_time
} sort_mode_;

static sort_mode_ sort_mode = sm_shortname;	/* How are we sorting these? */

static char* opt_sm_gcid, *opt_sm_shortname, *opt_sm_description, *opt_sm_time;

static
arglist_t sort_args[] = {
  {
    "gcid", &opt_sm_gcid, "Sort by numeric geocache ID",
    nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
  },
  {
    "shortname", &opt_sm_shortname, "Sort by waypoint short name",
    nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
  },
  {
    "description", &opt_sm_description, "Sort by waypoint description",
    nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
  },
  {
    "time", &opt_sm_time, "Sort by time",
    nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
  },
  ARG_TERMINATOR
};

static int
sort_comp(const queue* a, const queue* b)
{
  const Waypoint* x1 = (Waypoint*)a;
  const Waypoint* x2 = (Waypoint*)b;

  switch (sort_mode)  {
  case sm_gcid:
    return x1->gc_data->id - x2->gc_data->id;
  case sm_shortname:
    return x1->shortname.compare(x2->shortname);
  case sm_description:
    return x1->description.compare(x2->description);
  case sm_time:
    return x1->GetCreationTime().toTime_t() - x2->GetCreationTime().toTime_t();
  default:
    abort();
    return 0; /* Internal caller error. */
  }
}

static void
sort_process()
{
  sortqueue(&waypt_head, sort_comp);
}

static void
sort_init(const char*)
{
  if (opt_sm_gcid) {
    sort_mode = sm_gcid;
  }
  if (opt_sm_shortname) {
    sort_mode = sm_shortname;
  }
  if (opt_sm_description) {
    sort_mode = sm_description;
  }
  if (opt_sm_time) {
    sort_mode = sm_time;
  }
}

filter_vecs_t sort_vecs = {
  sort_init,
  sort_process,
  nullptr,
  nullptr,
  sort_args
};
#endif // FILTERS_ENABLED

/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

void poll_scoreboard(__global atomic_int *scoreboard, int threshold,
                     int2 mbid) {
  int entry = 0;
  int safety = 0;
  int MAX_POLLS = 15000;

  // Only one subgroup per threadgroup need poll for the threshold check, and
  // others can simply sync on the barrier. So the number of clients checking
  // the scoreaboard is equal to the number of threadgroups (which is equal to
  // the number of sub-slices).
  if (get_sub_group_local_id() == 0) {
    while (entry < threshold && safety < MAX_POLLS) {
      entry = atomic_load_explicit(scoreboard, memory_order_acquire,
                                   memory_scope_device);
      safety++;
    }
  }

  if (safety == MAX_POLLS && get_sub_group_local_id() == 0) {
    printf("FAIL (%d %d) %d %d\n", mbid.x, mbid.y, entry, threshold);
  }
}

inline void signal_scoreboard(__global atomic_int *scoreboard) {
  // All subgroups from all threadgroups need to signal completion for wavefront
  // progress.
  if (get_sub_group_local_id() == 0) {
    atomic_fetch_add_explicit(scoreboard, 1, memory_order_acq_rel,
                              memory_scope_device);
  }
}

inline bool check_ref_coord_valid(bool ref_coord[5], int loc) {
  bool valid = true;
  for (int i = loc; i > 0; i--) {
    if (ref_coord[i] == ref_coord[i - 1]) {
      valid = false;
    }
  }
  return valid;
}

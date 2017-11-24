/*
 * Copyright(c) 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

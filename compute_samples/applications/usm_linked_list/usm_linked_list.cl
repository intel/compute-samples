/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

struct Node {
  global struct Node *next;
  uint value;
};

kernel void usm_linked_list_kernel(global struct Node *head) {
  while (head) {
    head->value = head->value * 2 + 1;
    head = head->next;
  }
}

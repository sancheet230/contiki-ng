/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**
 * \file
 * Linked list library implementation.
 *
 * \author Adam Dunkels <adam@sics.se>
 *
 */

/**
 * \addtogroup list
 * @{
 */
#include "contiki.h"
#include "lib/list.h"

#include <string.h>

/*---------------------------------------------------------------------------*/
/* Define the struct for storing integer data in the list */
struct int_node {
  int value;
  struct int_node *next;
};
/*---------------------------------------------------------------------------*/

void *
list_tail(const_list_t list)
{
  struct int_node *l;

  if(*list == NULL) {
    return NULL;
  }

  for(l = (struct int_node *)*list; l->next != NULL; l = l->next);

  return l;
}
/*---------------------------------------------------------------------------*/

void
list_add(list_t list, void *item)
{
  struct int_node *l;

  /* Make sure not to add the same element twice */
  list_remove(list, item);

  ((struct int_node *)item)->next = NULL;

  l = (struct int_node *)list_tail(list);

  if(l == NULL) {
    *list = item;
  } else {
    l->next = (struct int_node *)item;
  }
}
/*---------------------------------------------------------------------------*/

void
list_push(list_t list, void *item)
{
  /* Make sure not to add the same element twice */
  list_remove(list, item);

  ((struct int_node *)item)->next = (struct int_node *)*list;
  *list = item;
}
/*---------------------------------------------------------------------------*/

void *
list_chop(list_t list)
{
  struct int_node *l, *r;

  if(*list == NULL) {
    return NULL;
  }
  if(((struct int_node *)*list)->next == NULL) {
    l = (struct int_node *)*list;
    *list = NULL;
    return l;
  }

  for(l = (struct int_node *)*list; l->next->next != NULL; l = l->next);

  r = l->next;
  l->next = NULL;

  return r;
}
/*---------------------------------------------------------------------------*/

void *
list_pop(list_t list)
{
  struct int_node *l;
  l = (struct int_node *)*list;
  if(*list != NULL) {
    *list = ((struct int_node *)*list)->next;
  }

  return l;
}
/*---------------------------------------------------------------------------*/

void
list_remove(list_t list, const void *item)
{
  struct int_node *l, *r;

  if(*list == NULL) {
    return;
  }

  r = NULL;
  for(l = (struct int_node *)*list; l != NULL; l = l->next) {
    if(l == item) {
      if(r == NULL) {
        /* First on list */
        *list = l->next;
      } else {
        /* Not first on list */
        r->next = l->next;
      }
      l->next = NULL;
      return;
    }
    r = l;
  }
}
/*---------------------------------------------------------------------------*/

int
list_length(const_list_t list)
{
  struct int_node *l;
  int n = 0;

  for(l = (struct int_node *)*list; l != NULL; l = l->next) {
    ++n;
  }

  return n;
}
/*---------------------------------------------------------------------------*/

void
list_insert(list_t list, void *previtem, void *newitem)
{
  if(previtem == NULL) {
    list_push(list, newitem);
  } else {
    list_remove(list, newitem);
    ((struct int_node *)newitem)->next = ((struct int_node *)previtem)->next;
    ((struct int_node *)previtem)->next = newitem;
  }
}
/*---------------------------------------------------------------------------*/

bool
list_contains(const_list_t list, const void *item)
{
  struct int_node *l;
  for(l = (struct int_node *)*list; l != NULL; l = l->next) {
    if(item == l) {
      return true;
    }
  }
  return false;
}
/*---------------------------------------------------------------------------*/
/** @} */

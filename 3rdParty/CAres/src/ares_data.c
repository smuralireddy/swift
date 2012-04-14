/* $Id: ares_data.c,v 1.2 2009-11-20 09:06:33 yangtse Exp $ */

/* Copyright (C) 2009 by Daniel Stenberg
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */


#include "ares_setup.h"

#include <stddef.h>

#include "ares.h"
#include "ares_data.h"
#include "ares_private.h"


/*
** ares_free_data() - c-ares external API function.
**
** This function must be used by the application to free data memory that
** has been internally allocated by some c-ares function and for which a
** pointer has already been returned to the calling application. The list
** of c-ares functions returning pointers that must be free'ed using this
** function is:
**
**   ares_parse_srv_reply()
**   ares_parse_txt_reply()
*/

void ares_free_data(void *dataptr)
{
  struct ares_data *ptr;

  if (!dataptr)
    return;

  ptr = (void *)((char *)dataptr - offsetof(struct ares_data, data));

  if (ptr->mark != ARES_DATATYPE_MARK)
    return;

  switch (ptr->type)
    {
      case ARES_DATATYPE_SRV_REPLY:

        if (ptr->data.srv_reply.next)
          ares_free_data(ptr->data.srv_reply.next);
        if (ptr->data.srv_reply.host)
          free(ptr->data.srv_reply.host);
        break;

      case ARES_DATATYPE_TXT_REPLY:

        if (ptr->data.txt_reply.next)
          ares_free_data(ptr->data.txt_reply.next);
        if (ptr->data.txt_reply.txt)
          free(ptr->data.txt_reply.txt);
        break;

      default:
        return;
    }

  free(ptr);
}


/*
** ares_malloc_data() - c-ares internal helper function.
**
** This function allocates memory for a c-ares private ares_data struct
** for the specified ares_datatype, initializes c-ares private fields
** and zero initializes those which later might be used from the public
** API. It returns an interior pointer which can be passed by c-ares
** functions to the calling application, and that must be free'ed using
** c-ares external API function ares_free_data().
*/

void *ares_malloc_data(ares_datatype type)
{
  struct ares_data *ptr;

  ptr = malloc(sizeof(struct ares_data));
  if (!ptr)
    return NULL;

  switch (type)
    {
      case ARES_DATATYPE_SRV_REPLY:
        ptr->data.srv_reply.next = NULL;
        ptr->data.srv_reply.host = NULL;
        ptr->data.srv_reply.priority = 0;
        ptr->data.srv_reply.weight = 0;
        ptr->data.srv_reply.port = 0;
        break;

      case ARES_DATATYPE_TXT_REPLY:
        ptr->data.txt_reply.next = NULL;
        ptr->data.txt_reply.txt = NULL;
        ptr->data.txt_reply.length  = 0;
        break;

      default:
        free(ptr);
        return NULL;
    }

  ptr->mark = ARES_DATATYPE_MARK;
  ptr->type = type;

  return &ptr->data;
}


/*
** ares_get_datatype() - c-ares internal helper function.
**
** This function returns the ares_datatype of the data stored in a
** private ares_data struct when given the public API pointer.
*/

ares_datatype ares_get_datatype(void * dataptr)
{
  struct ares_data *ptr;

  ptr = (void *)((char *)dataptr - offsetof(struct ares_data, data));

  if (ptr->mark == ARES_DATATYPE_MARK)
    return ptr->type;

  return ARES_DATATYPE_UNKNOWN;
}

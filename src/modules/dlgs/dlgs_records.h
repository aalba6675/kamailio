/**
 * Copyright (C) 2020 Daniel-Constantin Mierla (asipto.com)
 *
 * This file is part of Kamailio, a free SIP server.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef _DLGS_RECORDS_H_
#define _DLGS_RECORDS_H_

#include <time.h>

#define DLGS_TOTAG_SIZE 128

#include "../../core/parser/msg_parser.h"
#include "../../core/locking.h"

/* clang-format off */
#define DLGS_STATE_INIT			0
#define DLGS_STATE_PROGRESS		1
#define DLGS_STATE_ANSWERED		2
#define DLGS_STATE_CONFIRMED	3
#define DLGS_STATE_TERMINATED	4
#define DLGS_STATE_NOTANSWERED	5

typedef struct _dlgs_item {
    unsigned int hashid;   /* item hash id */
	str callid;            /* sip call-id */
	str ftag;              /* sip from-tag */
	str ttag;              /* sip to-tag */
	str ruid;              /* record unique id */
	str src;               /* src field */
	str dst;               /* dst field */
	str data;              /* data field */
	int state;             /* state */
	time_t ts_init;
	time_t ts_answer;
    struct _dlgs_item *prev;
    struct _dlgs_item *next;
} dlgs_item_t;

typedef struct _dlgs_slot {
	unsigned int esize;
	dlgs_item_t *first;
	gen_lock_t lock;	
} dlgs_slot_t;

typedef struct _dlgs_ht {
	unsigned int htexpire;
	unsigned int htinitexpire;
	unsigned int htsize;
	dlgs_slot_t *slots;
} dlgs_ht_t;
/* clang-format on */

dlgs_ht_t *dlgs_ht_init(unsigned int htsize, int expire, int initexpire);
int dlgs_ht_destroy(dlgs_ht_t *dsht);
int dlgs_add_item(dlgs_ht_t *dsht, sip_msg_t *msg, str *src, str *dst, str *data);
int dlgs_del_item(dlgs_ht_t *dsht, sip_msg_t *msg);
dlgs_item_t *dlgs_get_item(dlgs_ht_t *dsht, sip_msg_t *msg);
int dlgs_unlock_item(dlgs_ht_t *dsht, sip_msg_t *msg);

int dlgs_ht_dbg(dlgs_ht_t *dsht);
int dlgs_item_free(dlgs_item_t *cell);

#endif
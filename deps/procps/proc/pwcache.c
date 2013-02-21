/*
 * pwcache.c - memory cache passwd file handling
 *
 * Copyright (C) 1992-1998 by Michael K. Johnson, johnsonm@redhat.com
 * Note: most likely none of his code remains
 *
 * Copyright 2002, Albert Cahalan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pwd.h>
#include "alloc.h"
#include "pwcache.h"
#include <grp.h>

// might as well fill cache lines... else we waste memory anyway

#define	HASHSIZE	64		/* power of 2 */
#define	HASH(x)		((x) & (HASHSIZE - 1))

static struct pwbuf {
    struct pwbuf *next;
    uid_t uid;
    char name[P_G_SZ];
} *pwhash[HASHSIZE];

char *user_from_uid(uid_t uid) {
    struct pwbuf **p;
    struct passwd *pw;

    p = &pwhash[HASH(uid)];
    while (*p) {
	if ((*p)->uid == uid)
	    return((*p)->name);
	p = &(*p)->next;
    }
    *p = (struct pwbuf *) xmalloc(sizeof(struct pwbuf));
    (*p)->uid = uid;
    pw = getpwuid(uid);
    if(!pw || strlen(pw->pw_name) >= P_G_SZ)
	sprintf((*p)->name, "%u", uid);
    else
        strcpy((*p)->name, pw->pw_name);

    (*p)->next = NULL;
    return((*p)->name);
}

static struct grpbuf {
    struct grpbuf *next;
    gid_t gid;
    char name[P_G_SZ];
} *grphash[HASHSIZE];

char *group_from_gid(gid_t gid) {
    struct grpbuf **g;
    struct group *gr;

    g = &grphash[HASH(gid)];
    while (*g) {
        if ((*g)->gid == gid)
            return((*g)->name);
        g = &(*g)->next;
    }
    *g = (struct grpbuf *) xmalloc(sizeof(struct grpbuf));
    (*g)->gid = gid;
    gr = getgrgid(gid);
    if (!gr || strlen(gr->gr_name) >= P_G_SZ)
        sprintf((*g)->name, "%u", gid);
    else
        strcpy((*g)->name, gr->gr_name);
    (*g)->next = NULL;
    return((*g)->name);
}

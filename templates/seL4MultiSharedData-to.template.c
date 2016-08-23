/*#
 *# Copyright 2014, NICTA
 *#
 *# This software may be distributed and modified according to the terms of
 *# the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *# See "LICENSE_BSD2.txt" for details.
 *#
 *# @TAG(NICTA_BSD)
 #*/

#include <camkes/dataport.h>
#include <utils/util.h>

/*? macros.show_includes(me.instance.type.includes) ?*/

/*# Enumerate all the incoming interfaces #*/
/*- set shmems = [] -*/
/*- set client_ids = set() -*/
/*- for c in me.parent.from_ends -*/

    /*- set client_id = configuration[c.instance.name].get("%s_attributes" % c.interface.name) -*/
    /*- if client_id is none or re.match('\\d+$', client_id) is none -*/
        /*? raise(Exception('%s.%s_attributes must be set to a number' % (c.instance.name, c.interface.name))) ?*/
    /*- endif -*/
    /*- set client_id = client_id.strip('"') -*/

    /*- if client_id not in client_ids -*/
        /*- do client_ids.add(client_id) -*/

        /*- set shmem_size = configuration[c.instance.name].get("%s_shmem_size" % c.interface.name, 4096) -*/
        /*- set shmem_section = 'to_%s_%s' % (me.interface.name, client_id) -*/
        /*- set shmem_symbol = 'to_%s_%s_data' % (me.interface.name, client_id) -*/
        /*- set shmem_name = "%s_buf_%s" % (me.interface.name, client_id) -*/

        #define SHM_ALIGN (1 << 12)
        struct {
            char content[ROUND_UP_UNSAFE(/*? shmem_size ?*/, PAGE_SIZE_4K)];
        } /*? shmem_symbol ?*/
                __attribute__((aligned(SHM_ALIGN)))
                __attribute__((section("shared_/*? shmem_section ?*/")))
                __attribute__((externally_visible));

        /*- do register_shared_variable('%s_%s_data' % (me.parent.name, client_id), shmem_symbol) -*/

        volatile void * /*? shmem_name ?*/ = (volatile void *) & /*? shmem_symbol ?*/;

        /*- do shmems.append((shmem_name, client_id)) -*/
    /*- else -*/
        /* skipping /*? client_id ?*/ */
    /*- endif -*/

/*- endfor -*/


void * /*? me.interface.name ?*/_buf(unsigned int client_id) {
    /*- if len(shmems) == 0 -*/
        return NULL;
    /*- else -*/
        switch (client_id) {
            /*- for symbol, id in shmems -*/
            case /*? id ?*/:
                return (void *) /*? symbol ?*/;
            /*- endfor -*/
        default:
            return NULL;
        }
    /*- endif -*/
}

unsigned int /*? me.interface.name ?*/_num_badges(void) {
    return /*? len(shmems) ?*/;
}

unsigned int /*? me.interface.name ?*/_enumerate_badge(unsigned int i) {
    /*- if len(shmems) == 0 -*/
       return -1;
    /*- else -*/
        switch (i) {
            /*- for _, id in shmems -*/
                case /*? loop.index0 ?*/:
                    return /*? id ?*/;
            /*- endfor -*/
        default:
            return -1;
        }
    /*- endif -*/
}


int /*? me.interface.name ?*/_wrap_ptr(dataport_ptr_t *p, void *ptr) {
    return -1;
}

void * /*? me.interface.name ?*/_unwrap_ptr(dataport_ptr_t *p) {
    return NULL;
}

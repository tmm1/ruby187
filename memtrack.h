/* 
	Author:		Stephen Kellett.
	Company:	Software Verification Limited
	Date:		20 October 2006

	Purpose:	Functions for tracking object allocations, deallocations etc in the Ruby core.

	License:	The same license as all Ruby C source code.

	Copyright (C) 2006  Object Media Limited, PO Box 123, Ely, Cambs, CB6 2WQ, United Kingdom.
	This copyright notice must remain in all source code that uses this code.

    Use this code at your own risk. No warranty, express or implied.
*/

#include "ruby.h"

// functions and data used by the public API to Ruby (exposed as part of the DLL IAT/EAT functions)
// if you change this enumeration or the function prototypes please change the name of the functions
// using them so that you do not break any code that already uses these named functions

typedef enum
{
	ROTA_ALLOCATE,
	ROTA_DEALLOCATE,
} ROTA_ACTION;

// functions for tracking allocate/deallocate of objects

typedef void (*RUBY_OBJ_TRACKER)(VALUE	obj,		/* the object allocated/deallocated */
				 ROTA_ACTION action);	/* allocate or deallocate	    */

void rb_set_object_tracing_hook(RUBY_OBJ_TRACKER func);

RUBY_OBJ_TRACKER rb_get_object_tracing_hook();

// functions for heap dumping and object referencing

typedef int (*RUBY_HEAP_ROOTS)(VALUE obj, void *userData);

void rb_get_heap_roots(RUBY_HEAP_ROOTS callback,
		       void *userData);

void rb_get_referenced_objects(VALUE obj,		/* object to get referenced objects	*/
			       RUBY_HEAP_ROOTS	callback,
			       void *userData);

// functions internal to Ruby

void _mem_track_track_allocate(VALUE obj);

void _mem_track_track_deallocate(VALUE	obj);


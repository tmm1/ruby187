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

#include "memtrack.h"

static RUBY_OBJ_TRACKER	objTracker = NULL;

// set object tracing callback

void rb_set_object_tracing_hook(RUBY_OBJ_TRACKER callback)
{
	objTracker = callback;
}

// get object tracing callback, useful for chaining multiple callback functions, if necessary

RUBY_OBJ_TRACKER rb_get_object_tracing_hook()
{
	return objTracker;
}

// to use the memory tracing functions we need to call 
// mem_track_track_allocate() and mem_track_track_deallocate() from these functions:
//
//	Function				Source File
//	--------				-----------
//	rb_newobj				gc.c
//	rb_gc_force_recycle		gc.c
//	obj_free				gc.c

void _mem_track_track_allocate(VALUE obj)
{
	if (objTracker != NULL)
	{
		(*objTracker)(obj, ROTA_ALLOCATE);
	}
}

void _mem_track_track_deallocate(VALUE	obj)
{
	if (objTracker != NULL)
	{
		(*objTracker)(obj, ROTA_DEALLOCATE);
	}
}

// functions to allow querying of the GC roots and querying of which 
// objects reference other objects. The actual work is done in gc.c but the C callable API 
// is here

extern void _gc_get_roots(RUBY_HEAP_ROOTS callback,
			  void *userData);

extern void _gc_get_referenced_objects(VALUE ptr,		// the object we are checking against
				       RUBY_HEAP_ROOTS	callback,
				       void *userData);

// get Ruby GC roots

void rb_get_heap_roots(RUBY_HEAP_ROOTS	callback,
		       void *userData)
{
	_gc_get_roots(callback, userData);
}

// get objects referenced by "obj"

void rb_get_referenced_objects(VALUE obj,
			       RUBY_HEAP_ROOTS callback,
			       void *userData)
{
	_gc_get_referenced_objects(obj, callback, userData);
}


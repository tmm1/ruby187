#include "ruby.h"

#ifdef ENABLE_DTRACE
#include "dtrace.h"
#endif

VALUE rb_mDtrace;

static VALUE
ruby_dtrace_fire(argc, argv, klass)
  int argc;
  VALUE *argv;
  VALUE klass;
{
	int args;
	VALUE name, data, ret;
	char *probe_data;
	char *probe_name;
	char *start_probe;
	char *end_probe;
	
	#ifdef ENABLE_DTRACE

	args = rb_scan_args(argc, argv, "11", &name, &data);
	probe_data = args == 2 ? StringValuePtr(data) : "";
	probe_name = StringValuePtr(name);

    	if (rb_block_given_p()) {
		start_probe = malloc(strlen(probe_name) + 7);
		end_probe   = malloc(strlen(probe_name) + 5);
		
		sprintf(start_probe, "%s-start", probe_name);
		sprintf(end_probe, "%s-end", probe_name);
		
		/* Build -start and -end strings for probe names */
		if (RUBY_RUBY_PROBE_ENABLED())
			RUBY_RUBY_PROBE(start_probe, probe_data);
	#endif
	
		ret = rb_yield(Qnil);
	
	#if ENABLE_DTRACE

		if (RUBY_RUBY_PROBE_ENABLED())
			RUBY_RUBY_PROBE(end_probe, probe_data);
		
		free(start_probe);
		free(end_probe);
    	} else {
		if (RUBY_RUBY_PROBE_ENABLED())
			RUBY_RUBY_PROBE(probe_name, probe_data);
		ret = Qnil;
	}
	#endif
    	return ret;
}


void Init_Tracer()
{
	rb_mDtrace = rb_define_module("Tracer");
	rb_define_module_function(rb_mDtrace, "fire", ruby_dtrace_fire, -1);
}


#include "ruby/ruby.h"
#include <asl.h>

VALUE cLogger;

struct mr_logger {
  aslclient asl_client;
  VALUE file_descriptors;
};

uint32_t LoggerDefaultASLOptions = ASL_OPT_NO_DELAY | ASL_OPT_STDERR | ASL_OPT_NO_REMOTE;

static VALUE
mr_logger_allocate(VALUE klass, SEL sel)
{
    VALUE obj;
    struct mr_logger *logger;
    
    obj = Data_Make_Struct(klass, struct mr_logger, NULL, NULL, logger);
    logger->file_descriptors = rb_ary_new();
    
    return obj;
}

// so initialize *always* has to take variadic args?
static VALUE
mr_logger_initialize(VALUE self, SEL sel, int argc, VALUE *argv)
{
    struct mr_logger *logger;
    Data_Get_Struct(self, struct mr_logger, logger);
    
    // If no facility is specified, use "com.apple.console"
    // const char *normalizedFacility = facility ? [facility UTF8String] : "com.apple.console";
    // logger->asl_client = asl_open( NULL /*ident*/, "com.apple.console", options );
    logger->asl_client = asl_open(NULL, "com.apple.console", LoggerDefaultASLOptions);
    
    return self;
}

void
mr_logger_debug(VALUE self, SEL sel, VALUE text)
{
  struct mr_logger *logger;
  Data_Get_Struct(self, struct mr_logger, logger);
  
  aslmsg msg = asl_new(ASL_TYPE_MSG);
  asl_set(msg, ASL_KEY_FACILITY, "com.apple.console");
  
  int level = 7;
  asl_log(logger->asl_client, msg, level, "%s", rb_str_cstr(text));
  
  asl_free(msg);
}

void
Init_logger()
{
  cLogger = rb_define_class("Logger", rb_cObject);
  
  rb_objc_define_method(*(VALUE *)cLogger, "alloc", mr_logger_allocate, 0);
  
  rb_objc_define_method(cLogger, "initialize", mr_logger_initialize, -1);
  rb_objc_define_method(cLogger, "debug", mr_logger_debug, 1);
}
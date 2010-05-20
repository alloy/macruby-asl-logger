#include "ruby/ruby.h"
#include "ruby/io.h"
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
    if (argc != 1) {
      rb_raise(rb_eArgError, "wrong number of arguments (%d for 1)", argc);
    }
    
    struct mr_logger *logger;
    Data_Get_Struct(self, struct mr_logger, logger);
    
    // If no facility is specified, use "com.apple.console"
    // TODO: add custom facility
    logger->asl_client = asl_open(NULL, "com.apple.console", LoggerDefaultASLOptions);
    
    VALUE io = argv[0];
    rb_io_t *io_struct = ExtractIOStruct(io);
    if (asl_add_log_file(logger->asl_client, io_struct->fd) == 0) {
      printf("success!\n");
    }
    
    return self;
}

void
mr_logger_add(VALUE self, SEL sel, VALUE level, VALUE text)
{
  printf("add!\n");
  struct mr_logger *logger;
  Data_Get_Struct(self, struct mr_logger, logger);
  
  aslmsg msg = asl_new(ASL_TYPE_MSG);
  asl_set(msg, ASL_KEY_FACILITY, "com.apple.console");
  
  int ilevel = level == Qnil ? ASL_LEVEL_ALERT : FIX2INT(level);
  asl_log(logger->asl_client, msg, ilevel, "%s", rb_str_cstr(text));
  
  asl_free(msg);
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
  rb_objc_define_method(cLogger, "add", mr_logger_add, 2);
  rb_objc_define_method(cLogger, "debug", mr_logger_debug, 1);
  
  rb_define_alias(cLogger, "log", "add");
  
  rb_define_const(cLogger, "EMERGENCY", INT2FIX(ASL_LEVEL_EMERG));
  rb_define_const(cLogger, "ALERT", INT2FIX(ASL_LEVEL_ALERT));
  rb_define_const(cLogger, "CRITICAL", INT2FIX(ASL_LEVEL_CRIT));
  rb_define_const(cLogger, "ERROR", INT2FIX(ASL_LEVEL_ERR));
  rb_define_const(cLogger, "WARNING", INT2FIX(ASL_LEVEL_WARNING));
  rb_define_const(cLogger, "NOTICE", INT2FIX(ASL_LEVEL_NOTICE));
  rb_define_const(cLogger, "INFO", INT2FIX(ASL_LEVEL_INFO));
  rb_define_const(cLogger, "DEBUG", INT2FIX(ASL_LEVEL_DEBUG));
  
  // Ruby's stdlib Logger constant aliases
  rb_define_const(cLogger, "UNKNOWN", INT2FIX(ASL_LEVEL_ALERT));
  rb_define_const(cLogger, "FATAL", INT2FIX(ASL_LEVEL_CRIT));
  rb_define_const(cLogger, "WARN", INT2FIX(ASL_LEVEL_WARNING));
}
#include "ruby/ruby.h"
#include "ruby/io.h"
#include <asl.h>

VALUE cLogger;

struct mr_logger {
  aslclient asl_client;
  VALUE file_descriptor;
  // VALUE file_descriptors;
};

uint32_t LoggerDefaultASLOptions = ASL_OPT_NO_DELAY | ASL_OPT_STDERR | ASL_OPT_NO_REMOTE;

static VALUE
mr_logger_allocate(VALUE klass, SEL sel)
{
    VALUE obj;
    struct mr_logger *logger;
    
    obj = Data_Make_Struct(klass, struct mr_logger, NULL, NULL, logger);
    // logger->file_descriptors = rb_ary_new();
    
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
    logger->file_descriptor = io;
    
    if (asl_add_log_file(logger->asl_client, ExtractIOStruct(io)->fd) == 0) {
      // printf("success!\n");
    }
    
    return self;
}

void
mr_logger_close(VALUE self, SEL sel)
{
  struct mr_logger *logger;
  Data_Get_Struct(self, struct mr_logger, logger);
  
  rb_io_close(logger->file_descriptor);
  asl_close(logger->asl_client);
}

static void
logger_add(VALUE self, int level, VALUE text)
{
  struct mr_logger *logger;
  Data_Get_Struct(self, struct mr_logger, logger);
  
  // TODO: good way to check if it's open
  if (ExtractIOStruct(logger->file_descriptor)->write_fd != -1) {
    aslmsg msg = asl_new(ASL_TYPE_MSG);
    asl_set(msg, ASL_KEY_FACILITY, "com.apple.console");
    asl_log(logger->asl_client, msg, level, "%s", rb_str_cstr(text));
    asl_free(msg);
  }
  else {
    rb_warn("log writing failed. closed stream");
  }
}

void
mr_logger_add(VALUE self, SEL sel, VALUE level, VALUE text)
{
  logger_add(self, (level == Qnil ? ASL_LEVEL_ALERT : FIX2INT(level)), text);
}

void
mr_logger_debug(VALUE self, SEL sel, VALUE text)
{
  logger_add(self, ASL_LEVEL_DEBUG, text);
}

void
Init_logger()
{
  cLogger = rb_define_class("Logger", rb_cObject);
  
  rb_objc_define_method(*(VALUE *)cLogger, "alloc", mr_logger_allocate, 0);
  
  rb_objc_define_method(cLogger, "initialize", mr_logger_initialize, -1);
  rb_objc_define_method(cLogger, "add", mr_logger_add, 2);
  rb_objc_define_method(cLogger, "debug", mr_logger_debug, 1);
  rb_objc_define_method(cLogger, "close", mr_logger_close, 0);
  
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
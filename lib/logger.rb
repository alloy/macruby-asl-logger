class Logger
  #define ASL_LEVEL_EMERG   0
  #define ASL_LEVEL_ALERT   1
  #define ASL_LEVEL_CRIT    2
  #define ASL_LEVEL_ERR     3
  #define ASL_LEVEL_WARNING 4
  #define ASL_LEVEL_NOTICE  5
  #define ASL_LEVEL_INFO    6
  #define ASL_LEVEL_DEBUG   7
  
  # The severity level constants as defined by ASL.
  EMERGENCY = 0
  ALERT     = 1
  CRITICAL  = 2
  ERROR     = 3
  WARNING   = 4
  NOTICE    = 5
  INFO      = 6
  DEBUG     = 7
  
  # Ruby’s stdlib Logger constant aliases.
  WARN      = WARNING
  FATAL     = CRITICAL
  UNKNOWN   = ALERT
end
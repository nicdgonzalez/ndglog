#ifndef NDGLOG_LOGGING_H_
# define NDGLOG_LOGGING_H_

# ifdef __cplusplus
extern "C" {
# endif  // !__cplusplus

# include <stdint.h>  // uint64_t
# include <time.h>  // time_t
# include <stdarg.h>  // va_list

/* (MAJOR * 10000) + (MINOR * 100) + PATCH */
# define _NDGLOG_VERSION_ 100

# define NAME_LENGTH 24
# define DATE_TIME_FMT_LENGTH 50

enum LogLevel {
  kDebug,
  kInfo,
  kWarn,
  kError,
  kFatal,
  kNotSet
};

struct LoggingOptions {
  char library_name[NAME_LENGTH];
  enum LogLevel minimum_level;
  char date_time_fmt[DATE_TIME_FMT_LENGTH];
  // Additional options may be added here.
};

struct LogMessageInfo {
  enum LogLevel level;
  time_t epoch;
  va_list args;
};

struct LogMessage {
  struct LogMessageInfo info;
  char *content;
};

struct Logger {
  char name[NAME_LENGTH];
};

struct namespace_ndglog {
  void (*const init)(struct LoggingOptions *);
  void (*const debug)(struct Logger *, const char *, ...);
  void (*const info)(struct Logger *, const char *, ...);
  void (*const warn)(struct Logger *, const char *, ...);
  void (*const error)(struct Logger *, const char *, ...);
  void (*const fatal)(struct Logger *, const char *, ...);
};

extern const struct namespace_ndglog ndglog;

# ifdef __cplusplus
}
# endif  // !__cplusplus

#endif  // !NDGLOG_LOGGING_H_

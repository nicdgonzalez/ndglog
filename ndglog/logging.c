#include "logging.h"

#include <stdio.h>  // FILE, fopen
#include <string.h>  // strcpy, strlen
#include <stdint.h>  // uint64_t
#include <time.h>  // localtime, strftime, time, time_t, tm
#include <pthread.h>  // pthread_: mutex, lock, unlock
#include <stdbool.h>  // bool, false, true
#include <stdarg.h>  // va_end, va_list, va_start

#ifdef _WIN32
# include <windows.h>  // CreateDirectory
# define make_directory CreateDirectory
#else
# include <sys/stat.h>  // mkdir
# define make_directory mkdir
#endif  // !_WIN32

#define BUFFER_LENGTH 50

static struct LoggingManager {
  enum LogLevel level;
  FILE *fp;
  struct LogMessage *buffer[BUFFER_LENGTH];
  uint64_t buffer_count;
  pthread_mutex_t mutex;
  bool has_been_set;
  char date_time_fmt[DATE_TIME_FMT_LENGTH];
} manager = {
  .level = kNotSet,
  .fp = (FILE *) NULL,
  .buffer_count = 0,
  .has_been_set = false,
  .date_time_fmt = "%Y-%m-%d %H:%M:%S",
};

static void ndglog_init(struct LoggingOptions *_options) {
  pthread_mutex_lock(&manager.mutex);
  if (manager.has_been_set) {
    return;
  }
  manager.level = _options->minimum_level;
  for (int index = (BUFFER_LENGTH - 1); index--; ) {
    manager.buffer[index] = (struct LogMessage *) NULL;
  }
  // Create the "logs" directory if it does not already exists.
  make_directory("logs", NULL);
  // Check if the client defined `library_name` in `_options`.
  // If not, use a default instead.
  if (!strcmp(_options->library_name, "")) {
    strcpy(_options->library_name, "ndglog");
  }
  // Create the log file with format:
  //   "{library_name}.yyyymmdd_hhmmss.log".
  char file_name[50];
  strcpy(file_name, "./logs/");
  strcat(file_name, _options->library_name);
  strcat(file_name, ".");
  {
    char date_time[16];
    time_t epoch = time(&epoch);
    struct tm *time_info = localtime(&epoch);
    strftime(date_time, sizeof(date_time), "%Y%m%d_%H%M%S", time_info);
    strcat(file_name, date_time);
  }
  strcat(file_name, ".log");
  manager.fp = fopen(file_name, "w+");
  manager.has_been_set = true;
  pthread_mutex_unlock(&manager.mutex);
  return;
}

static void create_message(
    struct Logger *_logger,
    enum LogLevel _level,
    const char *_content,
    va_list _args
) {
  static const char level_to_str[5][6] = {
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL"
  };

  struct LogMessage *message = (
    (struct LogMessage *)
    malloc(sizeof(struct LogMessage))
  );
  message->info.level = _level;
  // ndglog DEBUG @ 2022-08-19 03:27:23]: 
  char *format = (char *) calloc(1094, sizeof(char));
  strcpy(format, _logger->name);
  strcat(format, " ");
  strcat(format, level_to_str[_level]);
  strcat(format, " @ ");
  {
    char date_time[20];
    time_t epoch = time(&epoch);
    struct tm *time_info = localtime(&epoch);
    strftime(date_time, sizeof(date_time), manager.date_time_fmt, time_info);
    strcat(format, date_time);
    message->info.epoch = epoch;
  }
  strcat(format, "]: ");
  strcat(format, _content);
  {  // Automatically append a newline character if it is missing.
    int index = (strlen(format) - 1);
    if (format[index] != '\n') {
      strcat(format, "\n");
    }
  }
  message->content = (
    (char *)
    calloc(strlen(format), sizeof(char))
  );
  memcpy(message->content, format, strlen(format));
  free(format);
  message->info.args = _args;
  pthread_mutex_lock(&manager.mutex);
  manager.buffer[manager.buffer_count++] = message;
  pthread_mutex_unlock(&manager.mutex);
  return;
}

static void delete_message(struct LogMessage *_message) {
  free(_message->content);
  free(_message);
  return;
}

static void flush_message_buffer() {
  pthread_mutex_lock(&manager.mutex);
  for (
      struct LogMessage *message = manager.buffer[manager.buffer_count - 1];
      message != NULL;
      message = manager.buffer[manager.buffer_count--]
  ) {
    if (message->info.level >= manager.level) {
      vfprintf(manager.fp, message->content, message->info.args);
    }
    delete_message(message);
  }
  pthread_mutex_unlock(&manager.mutex);
  return;
}

# define _internal_log_fn(level) {                \
  va_list args;                                   \
  va_start(args, _format);                        \
  create_message(_logger, level, _format, args);  \
  flush_message_buffer();                         \
  va_end(args);                                   \
}

static void debug(struct Logger *_logger, const char *_format, ...) {
  _internal_log_fn(kDebug);
  return;
}

static void info(struct Logger *_logger, const char *_format, ...) {
  _internal_log_fn(kInfo);
  return;
}

static void warn(struct Logger *_logger, const char *_format, ...) {
  _internal_log_fn(kWarn);
  return;
}

static void error(struct Logger *_logger, const char *_format, ...) {
  _internal_log_fn(kError);
  return;
}

static void fatal(struct Logger *_logger, const char *_format, ...) {
  _internal_log_fn(kFatal);
  exit(EXIT_FAILURE);
  return;
}

const struct namespace_ndglog ndglog = {
  .init = &ndglog_init,
  .debug = &debug,
  .info = &info,
  .warn = &warn,
  .error = &error,
  .fatal = &fatal,
};

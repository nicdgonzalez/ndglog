#include <ndglog/logging.h>

struct Logger _log = { .name = "ndglog" };

void start_server() {
  ndglog.debug(&_log, "Entering function: '%s'", __func__);
  // ...
  ndglog.info(&_log, "Server started at: '%s'", "127.0.0.1:5000");
  return;
}

int main() {
  {
    struct LoggingOptions opts = {
      .library_name = "MyLibrary",
      .minimum_level = kInfo,
      .date_time_fmt = "%Y-%m-%d %H:%M:%S",
    };
    ndglog.init(&opts);
  }
  start_server();
  return 0;
}

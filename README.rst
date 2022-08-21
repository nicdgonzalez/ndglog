NDG Logging Library
====================

.. contents:: Table of Contents


Introduction
-------------

|Github Stars| |Github Forks| |Github Open Issues| |Github Open PRs|

**ndglog** is a simple, easy-to-use, C99-compliant logging library
written in C.

The purpose of this project is to practice writing thread-safe
programs in C as well as to practice writing proper
`CMake <https://cmake.org/>`_ scripts for the depending projects
downstream since virtually *any* project can use a logger.


Technologies Used
------------------

- Windows 10 x64
- Visual Studio Code
- CMake 3.2


Project Status
---------------

This project is currently in **development**.


Version Naming
---------------

This library uses *semantic versioning*:

.. code::

  MAJOR.MINOR.PATCH

Where an increment in:

- ``MAJOR`` = Incompatible changes (may require code to be updated).
- ``MINOR`` = Backwards compatible feature changes.
- ``PATCH`` = Backwards compatible bug fixes.


Getting Started
----------------

Below is an example program showcasing how to use the library as well
as some sample output.

.. code:: c

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

.. code:: console

  $ cat "./logs/MyLibrary.20220819_231238.log"
  MyLibrary INFO @ 20220819_231240]: Server started at: '127.0.0.1:5000'


Building with CMake
++++++++++++++++++++

.. code::

  root
  ├── ndglog (clone of this repository)
  └── example/
      ├── logs/
      |   └── MyLibrary.20220819_231238.log
      ├── CMakeLists.txt
      └── main.c

- Clone the repository.

.. code:: console

  $ git clone "https://github.com/nicdgonzalez/ndglog.git"

- Switch directories to the new ``ndglog`` and build the project.

  + For line 2, run ``cmake --help`` to view a list of available
    Generators.
  + On line 2, add ``-DCMAKE_INSTALL_PREFIX=""`` to change
    the install location.

.. code:: console

  $ cd "./ndglog"
  $ cmake -G "<Your Generator>" -B "./build"
  $ cmake --build "./build" --target "install"
  $ cd "../example"

- Minimal ``CMakeLists.txt`` example including **ndglog** as a
  depenedency.
  
.. code:: cmake

  cmake_minimum_required(VERSION 3.2)
  project(MyProject VERSION 0.1.0)
  add_executable(MyApp "main.c")
  find_package(ndglog CONFIG REQUIRED)
  target_link_libraries(MyApp ndglog::ndglog)

- Now build the project same as before (minus the installation)
  and run the executable:

.. code:: console

  $ cmake -G "MinGW Makefiles" -B "./build"
  $ make -C "./build"
  $ "./build/MyApp"

For a runnable version of this example,
go to the `/demo <./demo>`_ directory.


Contributing
-------------

Want to contribute? Great!

To fix a bug or enhance an existing module, follow these steps:

- `Fork <https://github.com/nicdgonzalez/ndglog/fork>`_ the repository
  and create a new branch:

.. code:: console

  $ git clone "https://github.com/[username]/[repository_name].git"
  $ git checkout -b "improve-feature"

- Make the appropriate changes and stage the modified files:

.. code:: console

  $ git add <changed file(s)>

- Commit changes:

.. code:: console

  $ git commit -m "Improve feature."

- Push to the new branch:

.. code:: console

  $ git push "origin" "improve-feature"

- Create a `Pull Request <https://github.com/nicdgonzalez/ndglog/pulls>`_.


Bug/Feature Request
--------------------

If you find a bug (program failed to run and/or gave undesired results)
or you just want to request a feature, kindly open a new issue
`here <https://github.com/nicdgonzalez/ndglog/issues>`_.


..
  ****************************************************************************
.. |Github Stars| image:: https://badgen.net/github/stars/nicdgonzalez/ndglog
.. |Github Forks| image:: https://badgen.net/github/forks/nicdgonzalez/ndglog
.. |Github Open Issues| image:: https://badgen.net/github/open-issues/nicdgonzalez/ndglog
  :target: https://github.com/nicdgonzalez/ndglog/issues?q=is%3Aissue+is%3Aopen+
.. |Github Open PRs| image:: https://badgen.net/github/open-prs/nicdgonzalez/ndglog
  :target: https://github.com/nicdgonzalez/ndglog/pulls?q=is%3Apr+is%3Aopen+

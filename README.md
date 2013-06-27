libul - Utilities library
==============================

Build
-----

To build libul you will need Boost Library 1.49 sources and Boost.Build

Unpack the Boost library sources relative to libul:

    projects/
      |-- boost_1_49_0/
      |-- libul/

Install Boost.Build in your system. From the boost source root do:
* cd tools/build/v2
* ./bootstrap.sh
* ./b2 --prefix=/usr/local install

To build libul, run 'b2' in the libul source root

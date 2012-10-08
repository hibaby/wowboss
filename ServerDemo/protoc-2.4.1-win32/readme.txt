Protocol Buffers - Google's data interchange format
Copyright 2008 Google Inc.
http://code.google.com/p/protobuf/

This package contains a precompiled Win32 binary version of the protocol buffer
compiler (protoc).  This binary is intended for Windows users who want to
use Protocol Buffers in Java or Python but do not want to compile protoc
themselves.  To install, simply place this binary somewhere in your PATH.

This binary was built using MinGW, but the output is the same regardless of
the C++ compiler used.

You will still need to download the source code package in order to obtain the
Java or Python runtime libraries.  Get it from:
  http://code.google.com/p/protobuf/downloads/



protoc.exe 编译的命令行形式是
protoc-I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
或简单的 
protoc –cpp_out=dir*.proto
更多的参数设置可以参考自带的帮助信息（命令行下 protoc –h 或者 protoc --help

set SRC_FILE=netmessage.proto
set SRC_DIR=D:\Projects\ServerDemo\ServerDemo\protoc-2.4.1-win32
set DST_DIR=D:\Projects\ServerDemo\ServerDemo\protoc-2.4.1-win32

protoc-I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/%SRC_FILE%

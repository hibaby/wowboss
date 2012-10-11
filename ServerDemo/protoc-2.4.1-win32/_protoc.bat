set SRC_FILE=netmessage
set SRC_DIR=D:\Projects\ServerDemo\ServerDemo\protoc-2.4.1-win32
set DST_DIR=D:\Projects\ServerDemo\ServerDemo\ServerDemo

protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/%SRC_FILE%.proto

protoc --descriptor_set_out=netmsg.protobin --include_imports %SRC_FILE%.proto
protogen netmsg.protobin
set SRC_FILE=netmessage.proto
set SRC_DIR=D:\Projects\ServerDemo\ServerDemo\protoc-2.4.1-win32
set DST_DIR=D:\Projects\ServerDemo\ServerDemo\ServerDemo

protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/%SRC_FILE%
inline google::protobuf::Message* createMessage(const std::string& type_name)

{

  google::protobuf::Message* message = NULL;

  const google::protobuf::Descriptor* descriptor =

    google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);

  if (descriptor)

  {

    const google::protobuf::Message* prototype =

      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

    if (prototype)

    {

      message = prototype->New();

    }

  }

  return message;

}



//////////////以下是使用



buf[numbytes] = '\0'; //buf为从网络收到的数据

String data = buf;



//"gamesrv.RepLuaMsg"为 类名字

google::protobuf::Message* reply = createMessage("gamesrv.RepLuaMsg");



//gamesrv::RepLuaMsg p2;

reply->ParseFromString(data); //直接反序列化





//////////////////////////////////////////////////////////////////////////////
// g++ -g `pkg-config --cflags protobuf` luaclient.cpp LuaCmd.pb.cc -o luacli `pkg-config --libs protobuf`

#include <stdio.h>

#include <unistd.h>

#include <strings.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

#include <string>

#include <iostream>

#include "LuaCmd.pb.h"

#include <google/protobuf/descriptor.h>

                          

#include <google/protobuf/dynamic_message.h>



#define HOST "localhost"

#define PORT 8889

#define MAXDATASIZE 100



using namespace std;





inline google::protobuf::Message* createMessage(const std::string& type_name)

{

  google::protobuf::Message* message = NULL;

  const google::protobuf::Descriptor* descriptor =

    google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);

  if (descriptor)

  {

    const google::protobuf::Message* prototype =

      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

    if (prototype)

    {

      message = prototype->New();

    }

  }

  return message;

}





int main(int argc, char ** argv)

{

  int fd, numbytes;

  char buf[MAXDATASIZE];

  struct hostent *he;

  struct sockaddr_in server;

  

  

  

  if (argc != 2) {

    printf("Usage: %s \"COMMAND\"\n",argv[0]);

    exit(0);

  } 

  

  he = gethostbyname("10.57.220.70");

  fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&server, sizeof(server));

  server.sin_family = AF_INET;

  server.sin_port = htons(PORT);

  server.sin_addr = *((struct in_addr *)he->h_addr);



  int ret = connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr));

  

  //for send 

  string data;

  int cmdid = 1000;

  char szTemp[1024]={'\0'};

  gamesrv::LuaMsg p;

  p.set_account("123");

  p.set_msg("hideto.bj@gmail.com");

  p.SerializeToString(&data);

  



  *(int*)(szTemp+4) = htonl(cmdid);



  int len = 4+4;

  memmove(szTemp+8,data.c_str(),data.length());

  len +=data.length();

  *(int*)(szTemp) = htonl(len);

  ret =send(fd, szTemp, len, 0);

  

  memset(buf,0,sizeof(buf));

  numbytes = recv(fd, buf, 8, 0);

  numbytes = ntohl(*(int*)(buf))-8;

  ret = recv(fd, buf, numbytes, 0);

  

  buf[numbytes] = '\0';

  data = buf;

 

  google::protobuf::Message* reply = createMessage("gamesrv.RepLuaMsg");

   

  reply->ParseFromString(data);

  gamesrv::RepLuaMsg* rep = (gamesrv::RepLuaMsg*)reply;

  cout << "People: " << endl;

  cout << "ID: " << rep->account() << endl;

  cout << "Email: " << rep->msg() << endl;

  string mm = rep->msg();

  close(fd);

  

  

  

  return 0;

}


package gamesrv;





option java_package = "gamesrv.Request.protocol";

option java_outer_classname = "LuaCmdProtos";



message LuaMsg {

    required string account = 1; // Unique ID number for this person.

  optional string msg = 2;

}



message RepLuaMsg {

  required string account = 1; // Unique ID number for this person.

  optional string msg = 2;

  }






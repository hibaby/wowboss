// testProtobuf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

using namespace std;  
// This function fills in a Person message based on user input. 
void PromptForAddress(tutorial::Person* person) 
{   
	cout << "Enter person ID number: ";   
	int id;   
	cin >> id;   
	person->set_id(id);   
	cin.ignore(256, '\n');    

	cout << "Enter name: ";   
	getline(cin, *person->mutable_name());   

	cout << "Enter email address (blank for none): ";   
	string email;   
	getline(cin, email);   
	if (!email.empty()) 
	{     
		person->set_email(email);   
	}    
	
	while (true) 
	{     
		cout << "Enter a phone number (or leave blank to finish): ";     
		string number;     getline(cin, number);     
		if (number.empty()) 
		{       
			break;    
		}      
		
		tutorial::Person::PhoneNumber* phone_number = person->add_phone();    
		phone_number->set_number(number);      
		cout << "Is this a mobile, home, or work phone? ";     
		string type;     getline(cin, type);     
		if (type == "mobile") 
		{      
			phone_number->set_type(tutorial::Person::MOBILE);     
		} 
		else if (type == "home") 
		{       
			phone_number->set_type(tutorial::Person::HOME);     
		} 
		else if (type == "work") 
		{       
			phone_number->set_type(tutorial::Person::WORK);     
		} 
		else 
		{       
			cout << "Unknown phone type.  Using default." << endl;     
		}   
	} 
}  

// Iterates though all people in the AddressBook and prints info about them. 
void ListPeople(const tutorial::AddressBook& address_book) 
{   
	for (int i = 0; i < address_book.person_size(); i++) 
	{     
		const tutorial::Person& person = address_book.person(i);      
		cout << "Person ID: " << person.id() << endl;     
		cout << "  Name: " << person.name() << endl;     
		if (person.has_email()) 
		{       
			cout << "  E-mail address: " << person.email() << endl;     
		}      
		
		for (int j = 0; j < person.phone_size(); j++) 
		{       
			const tutorial::Person::PhoneNumber& phone_number = person.phone(j);        
			switch (phone_number.type()) 
			{         
			case tutorial::Person::MOBILE:           
				cout << "  Mobile phone #: ";           
				break;         
			case tutorial::Person::HOME:           
				cout << "  Home phone #: ";           
				break;         
			case tutorial::Person::WORK:           
				cout << "  Work phone #: ";           
				break;       
			}       
			
			cout << phone_number.number() << endl;     
		}  
	} 
} 

// Main function:  Reads the entire address book from a file, 
//   adds one person based on user input, then writes it back out to the same 
//   file. 
int main(int argc, char* argv[]) 
{   
	// Verify that the version of the library that we linked against is   
	// compatible with the version of the headers we compiled against.   
	GOOGLE_PROTOBUF_VERIFY_VERSION;    
	if (argc != 2) 
	{     
		cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;     
		return -1;   
	}    
	
	tutorial::AddressBook address_book; 

	{
/*
		// ::google::protobuf::Message req;

		::google::protobuf::io:: input(str.data(), str.size());
		::google::protobuf::io::CodedInputStream decoder(&input);
		decoder.SetTotalBytesLimit(1024*1024*1024, 64*1024*1024);
		bool success = req.ParseFromCodedStream(&decoder) && decoder.ConsumedEntireMessage();



		address_book.ParseFromCodedStream();
		address_book.SerializeToCodedStream()
		address_book.ParseFromString();
		address_book.SerializeToString();
*/
	}
	{    
		// Read the existing address book.     
		fstream input(argv[1], ios::in | ios::binary);     
		int a = input.gcount();
		if (!input) 
		{       
			cout << argv[1] << ": File not found.  Creating a new file." << endl;     
		} 
		else if (!address_book.ParseFromIstream(&input)) 
		{       
			cerr << "Failed to parse address book." << endl;       
			return -1;     
		}   
		else
		{
			ListPeople(address_book);
		}
	}    
	
	// Add an address.   
	PromptForAddress(address_book.add_person());    
	
	{     
		// Write the new address book back to disk.     
		fstream output(argv[1], ios::out | ios::trunc | ios::binary);     
		if (!address_book.SerializeToOstream(&output)) 
		{       
			cerr << "Failed to write address book." << endl;       
			return -1;     
		}   
		/*
		Computes the serialized size of the message
		google::protobuf::Message::ByteSize()

		自定义的内存流实现ZeroCopy
		::google::protobuf::io::ZeroCopyOutputStream

		protobuf数据序列化为内存流 从而实现转发 保存等

		*/

		//消息数据 序列化为数据流
		char buffer[1024] = "";
		int bufferSize = sizeof(buffer);
		::google::protobuf::io::ArrayOutputStream aos( buffer, bufferSize );
		::google::protobuf::io::CodedOutputStream o(&aos);
		
		int s = address_book.ByteSize();

		if (!address_book.SerializeToCodedStream(&o)) 
		{       
			cerr << "Failed to write address book." << endl;       
			return -1;     
		}   


		// 消息转发
		char buffer_other[1024] = "";
		memcpy_s( buffer_other, sizeof(buffer_other), buffer, s );

		// 根据收到的数据流 还原消息结构
		tutorial::AddressBook address_book_other; 
		::google::protobuf::io::ArrayInputStream ais( buffer_other, s );
		::google::protobuf::io::CodedInputStream i(&ais);
		if ( !address_book_other.ParseFromCodedStream(&i) )
		{       
			cerr << "Failed to parse address book." << endl;       
			return -1;     
		} 
		else
		{
			ListPeople(address_book_other);
		}

	}    
	
	// Optional:  Delete all global objects allocated by libprotobuf.   
	google::protobuf::ShutdownProtobufLibrary();   

	return 0;
}

// int _tmain(int argc, _TCHAR* argv[])
// {
// 	return 0;
// }


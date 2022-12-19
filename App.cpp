#include "App.h"
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Exception.h"
namespace sablin{	
void CreateFolder();
void CheckIsRuning();
void CreateFiles();

void LoginInteract();
char InputFilter();
void LoadAccounts(std::map<std::wstring,std::wstring>&);
void UpdateAccounts(std::map<std::wstring,std::wstring>&);
bool CheckLoginAccount();
void AddNewAccount();
void DeleteAccount();

void LoadMissHitIndex(std::set<int32_t>&);
void LoadBookMessage(std::set<int32_t>&,std::map<std::wstring,Book>&);

void RunInteract();
void AddNewBookMessage(std::set<int32_t>&,std::map<std::wstring,Book>&);
void AddBookMessageToFile(Book&);
void UpdateMissHitIndex(std::set<int32_t>&);
void DeleteBookMessage(std::set<int32_t>&,std::map<std::wstring,Book>&);
void ChangeBookMessage(std::map<std::wstring,Book>&);
void CheckBookMessage(std::map<std::wstring,Book>&);
void CheckBookInteract();
void ListAll(std::map<std::wstring,Book>&,const std::wstring,const std::wstring);
void ListByType(std::map<std::wstring,Book>&);
void ListByAuthor(std::map<std::wstring,Book>&);
void ListByNation(std::map<std::wstring,Book>&);
void ListAllInteract(const std::wstring&,const std::wstring&);
void PrintBookMessage(const Book&);
void OrderByName(const std::map<std::wstring,Book>&);
void InitializePriceBookMap(const std::map<std::wstring,Book>&,std::multimap<double,Book>&);
void OrderByPriceHigher(const std::multimap<double,Book>&);
void OrderByPriceLower(const std::multimap<double,Book>&);
void FindByName(const std::map<std::wstring,Book>&);
void FindByPrice(const std::multimap<double,Book>&);
void FindByPriceRange(const std::multimap<double,Book>&);
void ListByProperty(int32_t,const std::map<std::wstring,Book>&);
int32_t FilterNumber();
//------------------------------------------------------
void App::Initialize(){
	CreateFolder();
	CheckIsRuning();
	CreateFiles();
}

bool App::Login(){
	while(true){
		LoginInteract();
		wchar_t input = InputFilter();
		switch(input){
		case 'a':case 'A':
			if(CheckLoginAccount())
				return true;
			break;
		case 'b':case 'B':
			AddNewAccount();
			break;
		case 'c':case 'C':
			DeleteAccount();
			break;
		case 'd':case 'D':
			break;
		case 'q':case 'Q':
			return false;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void App::LoadBookMap(){
	std::wcout << L"Load Books Messages......" << std::endl;
	LoadMissHitIndex(miss_hit_index_);
	LoadBookMessage(miss_hit_index_,book_map_);
	std::wcout << L"Load Succeed!" << std::endl;
}

void App::Run(){
	while(true){
		RunInteract();
		wchar_t input = InputFilter();
		switch(input){
		case 'a':case 'A':
			AddNewBookMessage(miss_hit_index_,book_map_);
			break;		
		case 'b':case 'B':
			DeleteBookMessage(miss_hit_index_,book_map_);
			break;
		case 'c':case 'C':
			CheckBookMessage(book_map_);
			break;
		case 'd':case 'D':
			ChangeBookMessage(book_map_);
			break;
		case 'e':case 'E':
			break;
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void App::ExceptionExit() noexcept{
	char file_name[] = ".\\BookMessages\\ProgramIsRunning.txt";
	remove(file_name);
}

void App::Exit(){
	ExceptionExit();
}

//-------------------------------------------------------

void CreateFolder(){
	wchar_t folder_name[] = L".\\BookMessages";
	if(_waccess(folder_name, 0) == -1)
		_wmkdir(folder_name);
}

void CheckIsRuning(){
	std::wofstream create_file;
	wchar_t file_name[] = L".\\BookMessages\\ProgramIsRunning.txt";
	if(_waccess(file_name, 0) == -1){
		create_file.open(file_name);
		create_file.close();	
	}
	else
		throw new sablin::program_is_running("Program is running!Don't open again!");
}

void CreateFiles(){
	std::wofstream create_file;
	
	wchar_t file_one[] = L".\\BookMessages\\AccountMessages.txt";
	if(_waccess(file_one, 0) == -1){
		create_file.open(file_one);
		create_file.close();
	}
	
	
	wchar_t file_two[] = L".\\BookMessages\\BookMessages.txt";
	if(_waccess(file_two, 0) == -1){
		create_file.open(file_two);
		create_file.close();	
	}
	
	wchar_t file_three[] = L".\\BookMessages\\BitMemory.txt";
	if(_waccess(file_three, 0) == -1){
		create_file.open(file_three);
		create_file.close();
	}
}	

char InputFilter(){
	char character;
	std::cin >> character;
	if(std::cin.fail())
		throw new std::runtime_error("Input Stream is Wrong!");
	bool tipe = true;
	while(std::cin.get() != '\n'){
		if(tipe){
			std::cout << "Warning: Effectve Input Is: ";
			std::cout << character << std::endl;
			std::cout << "Other Input Will Be Ignored!" << std::endl;
			tipe = false;
		}
	}
	return character;
}

void LoginInteract(){
	std::cout << "=============================================" << std::endl;
	std::cout << "               Login Menu:                   " << std::endl;
	std::cout << "A)-----LoginAccount                          " << std::endl;
	std::cout << "B)-----AddNewAccount                         " << std::endl;
	std::cout << "C)-----DeleteAccount                         " << std::endl;
	std::cout << "D)-----DisplayMenuAagin                      " << std::endl;
	std::cout << "Q)-----QuitProgram                           " << std::endl;
	std::cout << "=============================================" << std::endl;
	std::cout << "Please Input your choice :"                    << std::endl; 
}

void LoadAccounts(std::map<std::wstring,std::wstring> &account_map){
	std::wifstream w_input_file_stream;
	w_input_file_stream.open(".\\BookMessages\\AccountMessages.txt");
	std::wstring account_plus_key;
	while(std::getline(w_input_file_stream,account_plus_key)){
		std::wistringstream w_input_string_stream(account_plus_key);
		std::wstring account;
		std::wstring key;
		w_input_string_stream >> account >> key;
		account_map[account] = key;
	}
	w_input_file_stream.close();
}

void UpdateAccounts(std::map<std::wstring,std::wstring> &account_map){
	std::wofstream w_output_file_stream;
	w_output_file_stream.open(".\\BookMessages\\AccountMessages.txt");
	for(auto iterator = account_map.begin();iterator != account_map.end();++iterator){
		std::wstring account_plus_key = iterator->first + L" " + iterator->second;
		w_output_file_stream << account_plus_key << std::endl;
	}
	w_output_file_stream.close();
}

bool CheckLoginAccount(){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to Login Interface----------" << std::endl;
	std::cout << "=============================================" << std::endl;
	std::map<std::wstring,std::wstring> account_map;
	LoadAccounts(account_map);
	while(true){
		std::cout << "-------------------Tipe----------------------" << std::endl;
		std::cout << "Enter 'c' to login or enter 'q' to Login menu" << std::endl;
		std::cout << "-------------------Tipe----------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wstring account_string;
				std::wstring key_string;
				std::cout << "Please Input The Account Which You Want To Login In:" << std::endl;
				std::wcin >> account_string;
				if(account_string.size() != 8) {
					std::cout << "Account Size Must Be Eight!" << std::endl;
					continue;
				}
				for(auto c : account_string)
					if(!std::isdigit(c)){
						std::cout << "Account Must Be Pure Number!" << std::endl;
						continue;
					}  
				if(account_map.find(account_string) == account_map.end()){
					std::cout << "Account Was Not Be Found!" << std::endl;
					continue;
				}
				std::cout << "Please Input The Key:" << std::endl;
				std::wcin >> key_string;
				if(account_map[account_string] == key_string){
					std::cout << "Login In Succeed!" << std::endl;
					return true;
				}else{
					std::cout << "Key Is Wrong!Please Try Again!" << std::endl;
					continue;
				}
				break;	
			}
		case 'q':case 'Q':
			return false;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
	return true;
}
void AddNewAccount(){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to AddNewAccount------------" << std::endl;
	std::cout << "=============================================" << std::endl;
	std::map<std::wstring,std::wstring> account_map;
	LoadAccounts(account_map);
	while(true){
		std::cout << "-------------------Tipe----------------------" << std::endl;
		std::cout << "Enter 'c' to add  or  enter 'q' to login menu" << std::endl;
		std::cout << "-------------------Tipe----------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wstring account_string;
				std::wstring key_string;
				std::cout << "Please Input The Account Which You Want To Add:" << std::endl;
				std::wcin >> account_string;
				if(account_string.size() != 8) {
					std::cout << "Account Size Must Be Eight!" << std::endl;
					continue;
				}
				for(auto c : account_string)
					if(!std::isdigit(c)){
					std::cout << "Account Must Be Pure Number!" << std::endl;
					continue;
				}  
				if(account_map.find(account_string) != account_map.end()){
					std::cout << "Account Had Exist!" << std::endl;
					continue;
				}
				std::cout << "Please Input The Key:" << std::endl;
				std::wcin >> key_string;
				if(key_string.empty()){
					std::cout << "Key Must Be No Empty!" << std::endl;
					continue;
				}
				account_map[account_string] = key_string;
				UpdateAccounts(account_map);
				LoadAccounts(account_map);
				std::cout << "Add Succeed!" << std::endl;
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}
void DeleteAccount(){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to DeleteAccount------------" << std::endl;
	std::cout << "=============================================" << std::endl;
	std::map<std::wstring,std::wstring> account_map;
	LoadAccounts(account_map);
	while(true){
		std::cout << "-------------------Tipe-----------------------" << std::endl;
		std::cout << "Enter 'c' to delete or enter 'q' to login menu" << std::endl;
		std::cout << "-------------------Tipe-----------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wstring account_string;
				std::wstring key_string;
				std::cout << "Please Input The Account Which You Want To Delete:" << std::endl;
				std::wcin >> account_string;
				if(account_string.size() != 8) {
					std::cout << "Account Size Must Be Eight!" << std::endl;
					continue;
				}
				for(auto c : account_string)
					if(!std::isdigit(c)){
					std::cout << "Account Must Be Pure Number!" << std::endl;
					continue;
				}  
				if(account_map.find(account_string) == account_map.end()){
					std::cout << "Account Does Not Exist!" << std::endl;
					continue;
				}
				std::cout << "Please Input The Key:" << std::endl;
				std::wcin >> key_string;
				if(account_map[account_string] == key_string){
					account_map.erase(account_string);
					UpdateAccounts(account_map);
					LoadAccounts(account_map);
					std::cout << "Delete Succeed!" << std::endl;
					continue;
				}else{
					std::cout << "Key Is Wrong!" << std::endl;
				}
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void LoadMissHitIndex(std::set<int32_t> &miss_hit_index){
	std::wifstream w_input_file_stream;
	w_input_file_stream.open(".\\BookMessages\\BitMemory.txt");
	std::wstring index;
	while(w_input_file_stream >> index)
		miss_hit_index.insert(std::stoi(index));
	w_input_file_stream.close();
}

void LoadBookMessage(std::set<int32_t> &miss_hit_index,std::map<std::wstring,Book> &book_map){
	std::wifstream w_input_file_stream;
	w_input_file_stream.open(".\\BookMessages\\BookMessages.txt");
	std::wstring message;
	while(getline(w_input_file_stream,message)){
		std::wstring temp;
		std::wistringstream input_string_stream(message);
		Book book;
		input_string_stream >> temp;
		book.id_ = std::stoi(temp);
		if(miss_hit_index.find(book.id_) != miss_hit_index.end()) continue;
		input_string_stream >> book.name_ >> book.type_ >> book.author_ >> book.nation_;
		sablin::StringDecrypt(book.name_);
		sablin::StringDecrypt(book.type_);
		sablin::StringDecrypt(book.author_);
		sablin::StringDecrypt(book.nation_);
		input_string_stream >> temp;
		book.price_ = std::stod(temp);
		input_string_stream >> temp;
		book.count_ = std::stoi(temp);
		book_map[book.name_] = book;
	}
	w_input_file_stream.close();
}

void RunInteract(){
	std::cout << "=============================================" << std::endl;
	std::cout << "               Main Menu:                    " << std::endl;
	std::cout << "A)-----AddNewBookMessage                     " << std::endl;
	std::cout << "B)-----DeleteBookMessage                     " << std::endl;
	std::cout << "C)-----CheckBookMessage                      " << std::endl;
	std::cout << "D)-----ChangeBookMessage                     " << std::endl;
	std::cout << "E)-----DisplayMenuAagin                      " << std::endl;
	std::cout << "Q)-----QuitProgram                           " << std::endl;
	std::cout << "=============================================" << std::endl;
	std::cout << "Please Input your choice :"                    << std::endl; 
}

void AddNewBookMessage(std::set<int32_t> &miss_hit_index,std::map<std::wstring,Book> &book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to AddBookMessage-----------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		std::cout << "-------------------Tipe------------------------" << std::endl;
		std::cout << "Enter 'c' to add book or enter 'q' to main menu" << std::endl;
		std::cout << "-------------------Tipe------------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wcout << L"Please Input The Book Name Which Your Want To Add:" << std::endl;
				std::wstring name;
				std::getline(std::wcin,name);
				if(book_map.find(name) != book_map.end()){
					std::cout << "The Book Has Exist!" << std::endl;
					continue;
				}
				std::wcout << L"Please Input The Book Type Which Your Want To Add:" << std::endl;
				std::wstring type;
				std::getline(std::wcin,type);
				
				std::wcout << L"Please Input The Book author Which Your Want To Add:" << std::endl;
				std::wcout << L"If They Are More Than One,Please Delim Them By '/'!" << std::endl;
				std::wstring author;
				std::getline(std::wcin,author);
				
				std::wcout << L"Please Input The Book nation Which Your Want To Add:" << std::endl;
				std::wcout << L"If They Are More Than One,Please Delim Them By '/'!" << std::endl;
				std::wstring nation;
				std::getline(std::wcin,nation);
				
				std::wcout << L"Please Input The Book Price Which Your Want To Add:" << std::endl;
				std::wstring price;
				std::getline(std::wcin,price);
				double price_double = std::stod(price);
				if(price_double < 0.0 || std::abs(price_double) < 10e-4){
					std::cout << "The Price Has Wrong!" << std::endl;
					continue;
				}
				
				std::wcout << L"Please Input The Book Count Which Your Want To Add:" << std::endl;
				std::wstring count;
				std::getline(std::wcin,count);
				int32_t count_int = std::stoi(count);
				if(count_int <= 0){
					std::cout << "The Count Must Be Beyond Zero!" << std::endl;
					continue;
				}
				int32_t id = 0;
				if(miss_hit_index.empty()){
					id = book_map.size();	
				}else{
					id = *miss_hit_index.begin();
					miss_hit_index.erase(miss_hit_index.begin());
				}           
				
				UpdateMissHitIndex(miss_hit_index);
				Book book(id,name,type,author,nation,price_double,count_int);
				book_map[book.name_] = book;
				AddBookMessageToFile(book);
				std::cout << "Add Succeed!" << std::endl;
				break;	
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;	
		}
	}
}

void AddBookMessageToFile(Book &book){
	std::wofstream w_output_file_stream;
	w_output_file_stream.open(".\\BookMessages\\BookMessages.txt",std::ofstream::in | std::ofstream::out);
	w_output_file_stream.seekp(book.id_ * 200,std::ios::beg);
	w_output_file_stream << book;
	w_output_file_stream.close();
}

void UpdateMissHitIndex(std::set<int32_t> &miss_hit_index){
	std::wofstream w_output_file_stream;
	w_output_file_stream.open(".\\BookMessages\\BitMemory.txt");
	for(auto i : miss_hit_index)
		w_output_file_stream << std::to_wstring(i) + L" ";
	w_output_file_stream.close();
}

void DeleteBookMessage(std::set<int32_t> &miss_hit_index,std::map<std::wstring,Book> &book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to DeleteBookMessage--------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		std::cout << "-------------------Tipe------------------------" << std::endl;
		std::cout << "Enter 'c' to delete   or enter 'q' to main menu" << std::endl;
		std::cout << "-------------------Tipe------------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wcout << "Which Book Would You Like To Delete?" << std::endl;
				std::wstring name;
				std::getline(std::wcin,name);
				std::wcout << "Process......." << std::endl;
				auto iter = book_map.find(name);
				if(iter == book_map.end()){
					std::wcout << "Sorry!There Has No This Book!" << std::endl;
					break;
				}
				std::wcout << "Warning---------This Process can not REVERSE!" << std::endl;
				std::wcout << "Do you intend to contunue?(c to continue | other to quit)" << std::endl;
				wchar_t decide_char = InputFilter();
				if(decide_char != 'c' && decide_char != 'C') break;
				std::cout << "Process......." << std::endl;
				int32_t id = (iter->second).id_;
				book_map.erase(iter);
				miss_hit_index.insert(id);
				UpdateMissHitIndex(miss_hit_index);
				std::cout << "Delete Succeed!" << std::endl;
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;		
		}
	}
}

void ChangeBookMessage(std::map<std::wstring,Book> &book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to ChangeBookMessage--------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		std::cout << "-------------------Tipe---------------------------" << std::endl;
		std::cout << "Enter 'c' to change book or enter 'q' to main menu" << std::endl;
		std::cout << "-------------------Tipe---------------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::cout << "Which Book Message Would You Want To Change?" << std::endl;
				std::wstring name;
				std::getline(std::wcin,name);
				std::wcout << "Process......." << std::endl;
				auto iter = book_map.find(name);
				if(iter == book_map.end()){
					std::wcout << "Sorry!There Has No This Book!" << std::endl;
					break;
				}
				
				std::wstring type;
				std::wcout << "Do You Want To Change The Book Type?(y/n,other to quit)" << std::endl;
				wchar_t decide_char = InputFilter();
				if(decide_char == 'y' && decide_char != 'Y'){
					std::wcout << "Please Input the new type:" << std::endl;
					std::getline(std::wcin,type);
					if(type.size() == 0){
						std::wcout << "Sorry!Type Is Empty!Change Failed!" << std::endl;
						break;
					}
				}else if(decide_char != 'n' && decide_char != 'N'){
					break;
				}
				
				std::wstring author;
				std::wcout << "Do You Want To Change The Book Author?(y/n,other to quit)" << std::endl;
				decide_char = InputFilter();
				if(decide_char == 'y' && decide_char != 'Y'){
					std::wcout << "Please Input the new Author('/' to delim):" << std::endl;
					std::getline(std::wcin,author);
					if(author.size() == 0){
						std::wcout << "Sorry!Author Is Empty!Change Failed!" << std::endl;
						break;
					}
				}else if(decide_char != 'n' && decide_char != 'N'){
					break;
				}
				
				std::wstring nation;
				std::wcout << "Do You Want To Change The Author Nation?(y/n,other to quit)" << std::endl;
				decide_char = InputFilter();
				if(decide_char == 'y' && decide_char != 'Y'){
					std::wcout << "Please Input the new Nation('/' to delim):" << std::endl;
					std::getline(std::wcin,nation);
					if(nation.size() == 0){
						std::wcout << "Sorry!Nation Is Empty!Change Failed!" << std::endl;
						break;
					}
				}else if(decide_char != 'n' && decide_char != 'N'){
					break;
				}
				
				std::wstring price;
				std::wcout << "Do You Want To Change The Price?(y/n,other to quit)" << std::endl;
				decide_char = InputFilter();
				if(decide_char == 'y' && decide_char != 'Y'){
					std::wcout << "Please Input the new Price:" << std::endl;
					std::getline(std::wcin,price);
					if(price.size() == 0){
						std::wcout << "Sorry!Price Is Empty!Change Failed!" << std::endl;
						break;
					}
					if(std::stod(price) < 0 || std::abs(std::stod(price)) < 10e-4){
						std::wcout << "Sorry!Price Is Mius Or Zero!Change Failed!" << std::endl;
						break;
					}
				}else if(decide_char != 'n' && decide_char != 'N'){
					break;
				}
				
				std::wstring count;
				std::wcout << "Do You Want To Change The Count?(y/n,other to quit)" << std::endl;
				decide_char = InputFilter();
				if(decide_char == 'y' && decide_char != 'Y'){
					std::wcout << "Please Input the new Count:" << std::endl;
					std::getline(std::wcin,count);
					if(count.size() == 0){
						std::wcout << "Sorry!Count Is Empty!Change Failed!" << std::endl;
						break;
					}
					if(std::stoi(count) < 0){
						std::wcout << "Sorry!Count Is Miuns!Change Failed!" << std::endl;
						break;
					}
					if(std::stoi(count) == 0){
						std::wcout << "Sorry!Count Is Zero Means Delete It!Please Delete It!" << std::endl;
						break;
					}
				}else if(decide_char != 'n' && decide_char != 'N'){
					break;
				}
				
				if(!type.empty())   (iter->second).type_   = type;
				if(!author.empty()) (iter->second).author_ = type;
				if(!nation.empty()) (iter->second).nation_ = type;
				if(!price.empty())  (iter->second).price_  = std::stod(price);
				if(!count.empty())  (iter->second).count_  = std::stoi(count);
				AddBookMessageToFile(iter->second);
				std::cout << "Change Has Effective!" << std::endl;
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void CheckBookMessage(std::map<std::wstring,Book> &book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to CheckBookMessage---------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		CheckBookInteract();
		wchar_t input = InputFilter();
		switch(input){
		case 'a':case 'A':
			ListAll(book_map,L"ListAll",L"CheckMenu");
			break;
		case 'b':case 'B':
			ListByType(book_map);
			break;
		case 'c':case 'C':
			ListByAuthor(book_map);
			break;
		case 'd':case 'D':
			ListByNation(book_map);
			break;
		case 'e':case 'E':
			break;
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void CheckBookInteract(){
	std::cout << "=============================================" << std::endl;
	std::cout << "               Check Menu:                   " << std::endl;
	std::cout << "A)-----ListAllBook                           " << std::endl;
	std::cout << "B)-----ListByType                            " << std::endl;
	std::cout << "C)-----ListByAuthor                          " << std::endl;
	std::cout << "D)-----ListByNation                          " << std::endl;
	std::cout << "E)-----DisplayMenuAagin                      " << std::endl;
	std::cout << "Q)-----ReturnMainMenu                        " << std::endl;
	std::cout << "=============================================" << std::endl;
	std::cout << "Please Input your choice :"                    << std::endl; 
}

void ListAll(std::map<std::wstring,Book> &book_map,std::wstring welcome,std::wstring return_menu){
	std::wcout << "=============================================" << std::endl;
	std::wcout << L"---------Welcome to " + welcome + L"------------------" << std::endl;
	std::wcout << "=============================================" << std::endl;
	std::multimap<double,Book> price_book_map;
	bool price_book_map_initialized = false;
	ListAllInteract(welcome,return_menu);
	while(true){
		std::wcout << "Please Input Your Choice:" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'a':case 'A':
			OrderByName(book_map);
			break;
		case 'b':case 'B':
			if(!price_book_map_initialized){
				InitializePriceBookMap(book_map,price_book_map);
				price_book_map_initialized = true;
			}
			OrderByPriceHigher(price_book_map);
			break;
		case 'c':case 'C':
			if(!price_book_map_initialized){
				InitializePriceBookMap(book_map,price_book_map);
				price_book_map_initialized = true;
			}
			OrderByPriceLower(price_book_map);
			break;
		case 'd':case 'D':
			FindByName(book_map);
			break;
		case 'e':case 'E':
			if(!price_book_map_initialized){
				InitializePriceBookMap(book_map,price_book_map);
				price_book_map_initialized = true;
			}
			FindByPrice(price_book_map);
			break;
		case 'f':case 'F':
			if(!price_book_map_initialized){
				InitializePriceBookMap(book_map,price_book_map);
				price_book_map_initialized = true;
			}
			FindByPriceRange(price_book_map);
			break;
		case 'g':case 'G':
			ListAllInteract(welcome,return_menu);
			break;
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void ListAllInteract(const std::wstring &welcome,const std::wstring &return_menu){
	std::cout << "=============================================" << std::endl;
	std::wcout << L"               " + welcome+ L" Menu:       " << std::endl;
	std::cout << "A)-----OrderByName                           " << std::endl;
	std::cout << "B)-----OrderByPriceHigher                    " << std::endl;
	std::cout << "C)-----OrderByPriceLower                     " << std::endl;
	std::cout << "D)-----FindByName                            " << std::endl;
	std::cout << "E)-----FindByPrice                           " << std::endl;
	std::cout << "F)-----FindByPriceRange                      " << std::endl;
	std::cout << "G)-----DisplayMenuAagin                      " << std::endl;
	std::wcout << L"Q)-----Return" + return_menu + L"             " << std::endl;
	std::cout << "=============================================" << std::endl;
}

void PrintBookMessage(const Book &book){
	std::wcout << "|" << std::left << std::setw(15) << book.name_;
	std::wcout << "|" << std::left << std::setw(8) << book.type_;
	std::wcout << "|" << std::left << std::setw(15) << book.author_;
	std::wcout << "|" << std::left << std::setw(15) << book.nation_;
	std::wcout << "|" << std::right << std::setw(6) 
		<< std::setprecision(2) << std::fixed << std::showpoint << book.price_;
	std::wcout << "|" << std::right << std::setw(4) << book.count_;
	std::wcout << "|" << std::endl;
}

void OrderByName(const std::map<std::wstring,Book> &book_map){
	if(book_map.empty())
		std::wcout << "---------------------EMPTY!----------------" << std::endl;
	else
		std::wcout << "|***BOOK NAME***|**TYPE**|*****AUTHOR****|*****NATION****|*PRICE|COUNT|" << std::endl;
	for(auto iter = book_map.begin();iter != book_map.end();++iter)
		PrintBookMessage(iter->second);
}

void InitializePriceBookMap(const std::map<std::wstring,Book> &book_map,std::multimap<double,Book> &price_book_map){
	for(auto iter = book_map.begin();iter != book_map.end();++iter)
		price_book_map.emplace((iter->second).price_,iter->second);
}

void OrderByPriceHigher(const std::multimap<double,Book> &price_book_map){
	if(price_book_map.empty())
		std::wcout << "---------------------EMPTY!----------------" << std::endl;
	else
		std::wcout << "|***BOOK NAME***|**TYPE**|*****AUTHOR****|*****NATION****|*PRICE|COUNT|" << std::endl;
	for(auto iter = price_book_map.begin();iter != price_book_map.end();++iter)
		PrintBookMessage(iter->second);
}

void OrderByPriceLower(const std::multimap<double,Book> &price_book_map){
	if(price_book_map.empty())
		std::wcout << "---------------------EMPTY!----------------" << std::endl;
	else
		std::wcout << "|***BOOK NAME***|**TYPE**|*****AUTHOR****|*****NATION****|*PRICE|COUNT|" << std::endl;
	for(auto iter = price_book_map.rbegin();iter != price_book_map.rend();++iter)
		PrintBookMessage(iter->second);
}

void FindByName(const std::map<std::wstring,Book> &book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to FindBookByName-----------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		std::cout << "-------------------Tipe-----------------------------" << std::endl;
		std::cout << "Enter 'c' to Find Book or enter 'q' to return  menu" << std::endl;
		std::cout << "-------------------Tipe-----------------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wcout << "Please Input Name Which Book You Want To Find:" << std::endl;
				std::wstring name;
				std::getline(std::wcin,name);
				if(name.size() == 0){
					std::wcout << "Sorry!The Book Name Must Not Be NULL!" << std::endl;
					break;
				}	
				auto iter = book_map.find(name);
				if(iter == book_map.end()){
					std::wcout << "Sorry!The Book Does Not Exist!" << std::endl;
				}
				std::wcout << "|***BOOK NAME***|**TYPE**|*****AUTHOR****|*****NATION****|*PRICE|COUNT|" << std::endl;
				PrintBookMessage(iter->second);
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void FindByPrice(const std::multimap<double,Book> &price_book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to FindBookByPrice----------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		std::cout << "-------------------Tipe---------------------------" << std::endl;
		std::cout << "Enter 'c' to Find Book or enter 'q' to return menu" << std::endl;
		std::cout << "-------------------Tipe---------------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wcout << "Please Input The Price Which You Want To Input:" << std::endl;
				std::wstring price;
				std::getline(std::wcin,price);
				if(price.size() == 0){
					std::wcout << "Price Must Not Be NULL!" << std::endl;
					break;
				}
				double price_double = std::stod(price);
				if(price_double < 0 || std::abs(price_double) < 10-4){
					std::wcout << "Price Must Not Be Miuns Or Zero!" << std::endl;
					break;
				}
				auto iter_lower = price_book_map.lower_bound(price_double - 0.05);
				auto iter_higher = price_book_map.upper_bound(price_double + 0.05);
				std::wcout << "|***BOOK NAME***|**TYPE**|*****AUTHOR****|*****NATION****|*PRICE|COUNT|" << std::endl;
				while(iter_lower != iter_higher){
					PrintBookMessage(iter_lower->second);
					++iter_lower;
				}
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void FindByPriceRange(const std::multimap<double,Book> &price_book_map){
	std::cout << "=============================================" << std::endl;
	std::cout << "---------Welcome to FindByPriceRange---------" << std::endl;
	std::cout << "=============================================" << std::endl;
	while(true){
		std::cout << "-------------------Tipe---------------------------" << std::endl;
		std::cout << "Enter 'c' to Find Book or enter 'q' to return menu" << std::endl;
		std::cout << "-------------------Tipe---------------------------" << std::endl;
		wchar_t input = InputFilter();
		switch(input){
		case 'c':case 'C':
			{
				std::wcout << "Please Input The Lower Price Which You Want To Input:" << std::endl;
				std::wstring low_price;
				std::getline(std::wcin,low_price);
				if(low_price.size() == 0){
					std::wcout << "Low Price Must Not Be NULL!" << std::endl;
					break;
				}
				double low_price_double = std::stod(low_price);
				if(low_price_double < 0 || std::abs(low_price_double) < 10-4){
					std::wcout << "Low Price Must Not Be Miuns Or Zero!" << std::endl;
					break;
				}
				
				std::wcout << "Please Input The Higher Price Which You Want To Input:" << std::endl;
				std::wstring high_price;
				std::getline(std::wcin,high_price);
				if(high_price.size() == 0){
					std::wcout << "High Price Must Not Be NULL!" << std::endl;
					break;
				}
				double high_price_double = std::stod(high_price);
				if(high_price_double < 0 || std::abs(high_price_double) < 10-4){
					std::wcout << "High Price Must Not Be Miuns Or Zero!" << std::endl;
					break;
				}
				
				if(high_price_double < low_price_double || std::abs(high_price_double - low_price_double) < 10e-4){
					std::wcout << "High Price And Low Price Is Wrong!" << std::endl;
					break;
				}
				
				auto iter_lower = price_book_map.lower_bound(low_price_double - 0.05);
				auto iter_higher = price_book_map.upper_bound(high_price_double + 0.05);
				std::wcout << "|***BOOK NAME***|**TYPE**|*****AUTHOR****|*****NATION****|*PRICE|COUNT|" << std::endl;
				while(iter_lower != iter_higher){
					PrintBookMessage(iter_lower->second);
					++iter_lower;
				}
				break;
			}
		case 'q':case 'Q':
			return;
		default:
			std::wcout << "Wrong Input!Please Input again:" << std::endl;
		}
	}
}

void ListByProperty(int32_t property,std::map<std::wstring,Book> &book_map){
	std::wstring string_name;
	if(property == 0) string_name = L"Type";
	else if(property == 1) string_name = L"Author";
	else if(property == 2) string_name = L"Nation";
	
	std::map<std::wstring,std::map<std::wstring,Book>> property_map;
	for(auto iter = book_map.begin();iter != book_map.end();++iter){
		std::wstring key;
		switch(property){
		case 0:
			key = (iter->second).type_;
			break;
		case 1:
			key = (iter->second).author_;
			break;
		case 2:
			key = (iter->second).nation_;
		}
		std::wistringstream input_string_stream(key);
		std::wstring children;
		while(std::getline(input_string_stream,children,L'/')){
			auto temp = property_map.find(children); 
			if(temp != property_map.end()){
				(temp->second)[(iter->second).name_] = iter->second;	
			}else{
				property_map[children] = std::map<std::wstring,Book>();
				property_map[children][children] = iter->second;
			}
		}	
	}
	
	while(true){
		std::cout << "=============================================" << std::endl;
		std::wcout << L"          " + string_name + L" Menu:       " << std::endl;
		std::wstring names[property_map.size()];
		int32_t index = 0;
		for(auto iter = property_map.begin();iter != property_map.end();++iter){
			std::wcout << std::to_wstring(index) + L")-----" + iter->first << std::endl;
			names[index] = iter->first;
			++index;
		}
		std::wcout << std::to_wstring(property_map.size()) + L")-----DisplayMenuAagin" << std::endl;
		std::wcout << std::to_wstring(property_map.size()+1) + L")-----ReturnCheckMeun" << std::endl;
		std::cout << "=============================================" << std::endl;
		std::cout << "Please Input your choice :"                    << std::endl; 
		
		int32_t choice_index = FilterNumber();
		if(choice_index == static_cast<int32_t>(property_map.size())) continue;
		if(choice_index == static_cast<int32_t>(property_map.size() + 1)) return;
		if(choice_index > static_cast<int32_t>(property_map.size() + 1) || choice_index == -1){
			std::wcout << "Wrong Input!Page Has Flush!" << std::endl;
			continue;
		}
		ListAll(property_map[names[choice_index]],names[choice_index],string_name);
	}
}

void ListByType(std::map<std::wstring,Book> &book_map){
	ListByProperty(0,book_map);
}

void ListByAuthor(std::map<std::wstring,Book> &book_map){
	ListByProperty(1,book_map);
}

void ListByNation(std::map<std::wstring,Book> &book_map){
	ListByProperty(2,book_map);
}

int32_t FilterNumber(){
	std::set<wchar_t> number_set{'0','1','2','3','4','5','6','7','8','9'};
	std::wstring input_string;
	std::getline(std::wcin,input_string);
	if(input_string.size() == 0){
		std::wcout << "Input Is Empty!" << std::endl;
		return -1;
	}
	for(auto c : input_string)
		if(number_set.find(c) == number_set.end()) return -1;
	int32_t number = std::stoi(input_string);
	std::wstring number_string = std::to_wstring(number);
	if(number_string.size() != input_string.size()){
		std::wcout << "Input Is No Effective!" << std::endl;
		return -1;
	}
	if(number < 0){
		std::wcout << "Number Is Miuns!" << std::endl;
		return -1;
	}
	return number;
}



}


#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <iostream>
#include <sstream>
namespace sablin{
	
class Book{
public:
	int32_t 	id_;
	std::wstring name_;
	std::wstring type_;
	std::wstring author_;
	std::wstring nation_;
	double 		price_;
	int32_t 	count_;
	
	Book() = default;
	Book(int32_t id,std::wstring name,std::wstring type,
		std::wstring author,std::wstring nation,
		double price,int32_t count): id_(id),name_(name),
	type_(type),author_(author),nation_(nation),
	price_(price),count_(count){}
	Book(const Book&) = default;
	Book(Book&&) = default;
	Book& operator=(const Book&) = default;
	Book& operator=(Book&&) = default;
};

inline std::wstring StringEntrypt(std::wstring input){
	for(wchar_t &c : input)
		if(c == ' ') c = '$';
	return input;
}

inline void StringDecrypt(std::wstring &input){
	for(wchar_t &c : input)
		if(c == '$') c = ' ';
}

inline std::wostream &operator<<(std::wostream &os,const sablin::Book &item){
	std::wstring message = std::to_wstring(item.id_) + L" " + StringEntrypt(item.name_) + L" "
	+ StringEntrypt(item.type_) + L" " + StringEntrypt(item.author_) + L" " + StringEntrypt(item.nation_) + L" "
	+ std::to_wstring(item.price_) + L" " + std::to_wstring(item.count_);
	os << message + std::wstring(198 - message.size(),' ') + L'\n';
	return os;
}

}

#endif

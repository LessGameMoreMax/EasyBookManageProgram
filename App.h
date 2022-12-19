#ifndef APP_H
#define APP_H
#include <map>
#include <set>
#include "Book.h"
namespace sablin{

class App{
private:
	std::set<int32_t> miss_hit_index_;
	std::map<std::wstring,Book> book_map_;
public:
	App() = default;
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App() = default;
	
	void Initialize();
	bool Login();
	void LoadBookMap();
	void Run();
	void ExceptionExit() noexcept;
	void Exit();
};

}
#endif

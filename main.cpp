#include "App.h"
#include "Exception.h"
int main(){
	sablin::App application;
	
	try{
		application.Initialize();
		if(!application.Login()){application.Exit();return 0;}
		application.LoadBookMap();
		application.Run();
	}catch(sablin::program_is_running &error){
		std::cout << error.what() << std::endl;
	}catch(std::exception &exception){
		application.ExceptionExit();
		std::cout << exception.what() << std::endl;
	}
	application.Exit();
	return 0;
}

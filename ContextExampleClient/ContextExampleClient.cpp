#include <iostream>
#include "../ContextExample/Header.h"

int main(int argc, char* argv[])
{
   unsigned char* szStringBinding = NULL; // маркер привязки .протокол -  ip конечный точки(сервер) и порт

   std::clog << "Calling RpcStringBindingCompose " <<std::endl;
  //  d:\\Projects\\C++\\lab3demo\\DebugExe\\ContextExampleClient.exe 192.168.38.20
   RpcStringBindingCompose(
      NULL,
      reinterpret_cast<unsigned char*>("ncacn_ip_tcp"), 
      reinterpret_cast<unsigned char*>(argv[1]),
      reinterpret_cast<unsigned char*>("4747"), 
      NULL, 
      &szStringBinding);


   handle_t hBinding = NULL;

   std::clog << "Calling RpcBindingFromStringBinding" << std::endl;
 
  RpcBindingFromStringBinding( 
      szStringBinding,
      &hBinding);


   std::clog << "Calling RpcStringFree" << std::endl;

   RpcStringFree(
	   &szStringBinding);


   std::clog << "Calling RpcEpResolveBinding" << std::endl;
  
 RpcEpResolveBinding(hBinding, ContextExample_v1_0_c_ifspec);

   RpcTryExcept
   {
      std::cout << "Calling Open" << std::endl;
      CONTEXT_HANDLE hContext = Open(hBinding, argv[1]);

      std::cout << "Press enter to call Output" << std::endl;
      std::cin.get();

      std::cout << "Calling Output" << std::endl;
	  std::cout << "Input numbers" << std::endl;
	  int a, a2;
	  std::cin >> a;
	  std::cin >> a2;
	  std::cout << Output(hContext, a, a2)<< std::endl;
      std::cout << "Press enter to call Close" << std::endl;
      std::cin.get();

      std::cout << "Calling Close" << std::endl;
      Close(&hContext, argv[1]);
   }
   RpcExcept(1)
   {
	   std::cout << "/n  Error Calling Remote Procedure /n";
   }
   RpcEndExcept

   std::clog << "Calling RpcBindingFree" << std::endl;
   RpcBindingFree(&hBinding);
   std::cout << "Press enter to exit" << std::endl;
   std::cin.get();
}

void* __RPC_USER midl_user_allocate(size_t size)
{
   return malloc(size);
}

void __RPC_USER midl_user_free(void* p)
{
   free(p);
}

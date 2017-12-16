#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "../ContextExample/Header.h"

CONTEXT_HANDLE Open(handle_t hBinding, const char* szString)
{
   std::string* pContext = new std::string(szString);
   CONTEXT_HANDLE hContext = pContext;
   std::cout << "Client connected: " << szString<<"\n";
   
   return hContext;
}

int Output(CONTEXT_HANDLE hContext, int result, int result2)
{
	std::cout << "Result working server: " << result + result2<<"\n";	
	return result + result2;
}

void Close(CONTEXT_HANDLE* phContext, const char* szString)
{
   std::cout << "Client disconnected " << szString << std::endl;
 
   *phContext = NULL;
}

void __RPC_USER CONTEXT_HANDLE_rundown(CONTEXT_HANDLE hContext)
{
   std::cout << "CONTEXT_HANDLE_rundown: Context = " << hContext << std::endl;
   Close(&hContext, "not found");
}

DWORD WINAPI RpcServerListenThreadProc(LPVOID /*pParam*/)
{
   return RpcServerListen(
      1, // Recommended minimum number of threads.
      RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Recommended maximum number of threads.
      FALSE); // Start listening now.
}

int main()
{
   std::cout << "Calling RpcServerUseProtseqEp" << std::endl;

   RpcServerUseProtseqEp(
      reinterpret_cast<unsigned char*>("ncacn_ip_tcp"),
      RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
      reinterpret_cast<unsigned char*>("4747"), // TCP/IP port to use.
      NULL);

   std::cout << "Calling RpcServerRegisterIf" << std::endl;

    RpcServerRegisterIf2(ContextExample_v1_0_s_ifspec,NULL,NULL, 
      RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
      RPC_C_LISTEN_MAX_CALLS_DEFAULT, 
      (unsigned)-1, 
      NULL);

   std::cout << "Creating listen thread" << std::endl;
   const HANDLE hThread = CreateThread(NULL, 0, RpcServerListenThreadProc,
      NULL, 0, NULL);

   std::cout << "Press enter to stop listening" << std::endl;
   std::cin.get();

   std::cout << "Calling RpcMgmtStopServerListening" << std::endl;

  RpcMgmtStopServerListening(NULL);
   std::cout << std::endl << "Listen thread finished" << std::endl;

   CloseHandle(hThread);

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

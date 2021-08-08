#include <stdio.h>

#include "interface_BSIT.h"
#include "interface_BSIT_s.c"

#pragma comment (lib,"Rpcrt4.lib")

void *__RPC_USER midl_user_allocate(size_t len){
	return malloc(len);
}

void __RPC_USER midl_user_free(void *ptr){
	free(ptr);
}

int error_number = 0;

void impersonation(IN RPC_BINDING_HANDLE handle, unsigned char *username, unsigned char *password, unsigned char *result)
{
	error_number++;
/*	if (!LogonUserA(username, NULL, password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &handle))
	{
		printf("impersonation failed\n");
		*result = 0;
		printf("%d\n", GetLastError());
		return;
	}
	
	if (!ImpersonateLoggedOnUser(handle))
	{
		printf("impersonation failed\n");
		*result = 0;
		return;
	}*/

	printf("impersonation is successful\n");
	*result = 1;
}


void send_(unsigned char *path, unsigned char *data, unsigned int length, unsigned char *result)
{
	printf("command --send\n");
	RPC_STATUS status;
	if (error_number==2)
	{
		printf("error 5\n");
		*result = 2;
		status = RpcRevertToSelf();
		if (status) {
			printf("RpcRevertToSelf 0x%x\n", status);
		}
		return;
	}

	FILE *file;
	file = fopen(path, "wb");
	if (!file)
	{
		printf("open error %d\n", GetLastError());		
		*result = 2;
	}
	else
	{
		fwrite(data, 1, length, file);
		fclose(file);
		*result = 0;
	}

	status = RpcRevertToSelf();
	if (status){
		printf("RpcRevertToSelf 0x%x\n", status);
	}
}

void download_(unsigned char *path, unsigned char data[25900000], unsigned int *length, unsigned char *result)
{
	printf("command --download\n");
	RPC_STATUS status;
	*result = 0;
	if (error_number == 1)
	{
		printf("error 5\n");
		*result = 2;
		status = RpcRevertToSelf();
		if (status) {
			printf("RpcRevertToSelf 0x%x\n", status);
		}
		return;
	}
	FILE *file;
	file = fopen(path, "rb");
	if (!file)
	{
		printf("open error %d\n", GetLastError());
		*result = 2;
	}
	else
	{
		fseek(file, 0, SEEK_END);
		int y = ftell(file);
		*length = ftell(file);
		fseek(file, 0, 0);
		size_t results = fread(data, 1, *length, file);
		if (results != *length)
		{
			*result = 2;
			printf("read error");
			exit(3);
		}
		fclose(file);
	}
	status = RpcRevertToSelf();
	if (status){
		printf("RpcRevertToSelf 0x%x\n", status);
	}
}

void delete_(unsigned char *path, unsigned char *result)
{
	printf("command --delete\n");
	RPC_STATUS status;

	if (remove(path))
	{
		printf("error remove\n");
		*result = -1;
	}
}

void end_(unsigned char *result)
{
	RPC_STATUS status;

	status = RpcMgmtStopServerListening(NULL);
	printf("RpcMgmtStopServerListening 0x%x\n", status);

	status = RpcRevertToSelf();
	printf("RpcRevertToSelf 0x%x\n", status);
	return;
}


int main(int argc, unsigned char *argv[])
{
	RPC_STATUS status;

	status = RpcServerUseProtseqEpA((unsigned char*)"ncacn_ip_tcp", RPC_C_PROTSEQ_MAX_REQS_DEFAULT, argv[1], NULL);
	if (status)
	{
		printf("RpcServerUseProtseqEpA returned 0x%x\n", status);
		exit(status);
	}

	status = RpcServerRegisterIf2(interface_BSIT_v1_0_s_ifspec, NULL, NULL, RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, RPC_C_LISTEN_MAX_CALLS_DEFAULT, (unsigned)-1, NULL);
	if (status)
	{
		printf("RpcServerRegisterIf2 returned 0x%x\n", status);
		exit(status);
	}

	printf("start listen.\n");
	status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0);
	if (status)
	{
		printf("RpcServerListen returned: 0x%x\n", status);
		exit(status);
	}
	printf(":)\n");
	return 0;
}
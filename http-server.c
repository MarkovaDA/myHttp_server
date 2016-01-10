#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "http_server.h"

#define WEB_ROOT "/WEB"

int init_socket(int port)
{
	int socketfd = socket(AF_INET,SOCK_STREAM,0); //дескриптор сокета
	if(socketfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	struct sockaddr_in server_addr; //структура с адресом
	//указываем параметры сервера
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	memset(server_addr.sin_zero,0,sizeof(server_addr.sin_zero));
	int opt = 1;
  //Разрешаем повторное использование локальных адресов (если данная возможность поддерживается используемым протоколом
	int err = setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
	if(err == -1)
	{
		perror("setsockopt error");
		exit(1);
	}
	//присваиваем сокету имя
	int ret = bind(socketfd,(struct sockaddr *)(&server_addr),sizeof(server_addr));
	if(ret == -1)
	{
		perror("bind error");
		exit(1);
	}
	//информируем ОС, что сервер слушает запросы на данном сокете (20 соединений)
	ret = listen(socketfd,20);
	if(ret == -1)
	{
		perror("listen error");
		exit(1);
	}
	printf("My Web Server is Ready!\n");
	return socketfd;
}
//запуск сервера
void start_server(int port){
	int socketfd = init_socket(port);
	while(1) //цикл ожидания запросов
	{ //запрос на соединение - получаем новый сокет для обмена данными с клиентом
		int clientfd = accept(socketfd,NULL,NULL);
		if(clientfd == -1)
		{
			perror("accept error");
			continue;
		}
		//обрабатываем сигнал SIGCHLD - изменение состояние дочернего процесса
		signal(SIGCHLD,sighandler);
		if(shmid == -1)
		{
			perror("error");
			exit(1);
		}
		int pid = fork(); //каждый запрос обрабатывается новом процессе
		if(pid < 0)
		{
			perror("fork error");
			break;
		}
		else if(pid == 0)
		{
			//здесь обработка запроса
		}
		close(clientfd);
	}
    close(socketfd);
}

//обработчик сигнала
void sighandler(int signo)
{
	printf("catch signo = %d\n",signo);
	while(1)
	{
		int ret= waitpid(-1,NULL,WNOHANG); //немедленное возвращение управления, если ни один дочерний процесс не завершил выполнение. 
		printf("ret = %d\n",ret);
		if(ret > 0) return;
	}
}

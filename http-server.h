#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_
void do_get(int fd,char *fileName);
int init_socket(int port);
void start_server(int port);
void sighandler(int signo);
#endif

#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_
int read_line(int fd,char *buf,int size);
void do_get(int fd,char *fileName);
int init_socket(int port);
void data_process(int clientfd);
void start_server(int port);
void sighandler(int signo);
#endif

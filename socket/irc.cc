/*
2013-2-27
在linux下编译
一个简单的socket例程，主要用到的技术有socket,pthread
用select作为io选择检测,并且全面使用UTF8编码
还将会使用信号量(semaphore)作为连接的socket数组控制,
这应用在run_server函数中
*/
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>
#include "parser_cmd.h"

#define BUF_SIZE	512
/*
void encode(char *src,wchar_t *dst){
	for(int i=0;i<MSG_SIZE;i++){
		*(dst++)=*(src++);
	}
}*/
struct conn_info{
	pthread_t pid;
	int socket_fd;
};
class AtomConnMgr{
private:
	std::vector<struct conn_info*> _cnninfo;
public:
	void add(struct conn_info* ci){
		_cnninfo.push_back(ci);
	}
	void broadcast(wchar_t *msg){
	}
	
};
static const wchar_t *WELCOME_MSG=L"欢迎";
void show_buf(const unsigned char *b,int size){
	for(int i=0;i<size;i++){
		std::wcout << std::hex << (unsigned char)*(b+i) << L' ';
	}
}
void usage(const char *n){
	const char *p=n;
	while(*n!=0){
		if(*n=='/')p=n+1;
		n++;
	}
	std::wcout << L"Usage:" << std::endl;
	std::wcout << p <<
L"\t [--server <port> | -s <port>] \trunning in server mode\n\
\t [--connect <ip address>| -c <ip address>] \tconnect to ip address.\n" << std::endl;
}
void *cnn_establish(void *args){
	struct conn_info *p_ci =(struct conn_info *)args;
	wchar_t *utf8_str;
	char *buf;
	int status;
	//fd_set fdset;
	//int new_fd=(int )args;
	const char* p=(const char*)WELCOME_MSG;
	send(p_ci->socket_fd,p,sizeof(WELCOME_MSG)*4,0);
	buf=new char[BUF_SIZE];
	while(1){
		status=recv(p_ci->socket_fd,buf,BUF_SIZE,0);
		if(status == 0){
			break;
		}
		utf8_str=(wchar_t*)buf;
		std::wcout << L'[' <<std::hex<< p_ci->socket_fd  << L']' << utf8_str << std::endl;
	}
	close(p_ci->socket_fd);
	std::wcout << L"connection is properly shutdown!" << std::endl;
	//encode(buf,utf8_str);
	delete [] buf;
	return NULL;
}
const wchar_t * run_server(const char*port){
// difine at protocals, tcp = 6
	struct sockaddr_storage client_addr;
	int status;
	int client_count=0;
	int socket_fd;
	struct sockaddr_in s_addr;
	std::wstring cmd;
	s_addr.sin_family=AF_INET;
	s_addr.sin_port=htons(atoi(port));
	s_addr.sin_addr.s_addr=INADDR_ANY;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	status = bind(socket_fd,(struct sockaddr*)&s_addr,sizeof(s_addr));
	//std::wcout << L"checkpoint 1" << std::endl;
	listen(socket_fd,10);
	//std::wcout << L"checkpoint 2" << std::endl;
	socklen_t addr_size=sizeof(client_addr);
	int thread_res;
	while(1){
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(0,&fdset);
		FD_SET(socket_fd,&fdset);
		select(socket_fd+10,&fdset,NULL,NULL,NULL);
		if(FD_ISSET(0,&fdset)){ // 从控制台输入
			//std::wcout << ']'<<std::endl;
			std::wcin >> cmd;
			if(cmd==L"/exit"){
				break;
			}
			continue;
		}
		int new_fd=accept(socket_fd,(struct sockaddr*)&client_addr,&addr_size);
		std::wcout << L"connecting request comming" << std::endl;
		struct conn_info ci;
		ci.socket_fd=new_fd;
		thread_res=pthread_create(&ci.pid,NULL,cnn_establish,(void*)&ci);
		std::wcout << L"connection:" << ++client_count << std::endl;
	}
	close(socket_fd);
	if(status == -1) return L"bind 出错了";
	return (wchar_t*)0;
}
/*return 1 indicate error*/
const wchar_t * connect_to(const char *addr,const char*port){
	std::wstring msg;
	wchar_t *utf8_char;
	char *buf;
//std::wcout << addr << ':' << port << std::endl;
//const wchar_t CONST_MSG[]=L"缅甸";//Berma is turnning to demercry";
//	wchar_t  w_ipstr[INET6_ADDRSTRLEN];
	struct addrinfo hints, *res, *p;
	int status;
	fd_set fdset;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if((status=getaddrinfo(addr,port,&hints,&res))!=0){
		std::wcout << gai_strerror(status);
		return L"above error occurred";
	}
	for(p=res;p!=NULL;p=p->ai_next){
		if(p->ai_family==AF_INET){
			break;
		}
	}
	int socket_fd=socket(p->ai_family,res->ai_socktype,res->ai_protocol);
	status=connect(socket_fd,p->ai_addr,p->ai_addrlen);
	if(status!=-1){
		//show_buf((const unsigned char*)p,sizeof(CONST_MSG)*4);
		//int len=sizeof(CONST_MSG)*4;
		//std::wcout << len << std::endl;
		buf=new char[BUF_SIZE];
		while(1){
			FD_ZERO(&fdset);
			FD_SET(0,&fdset);
			FD_SET(socket_fd,&fdset);
			std::wcout << ']';
			select(socket_fd+4,&fdset,NULL,NULL,NULL);
			if(FD_ISSET(0,&fdset)){
				std::wcin >> msg;
				//send(socket_fd,msg.c_str(),msg.size()*4,0);
				if(msg==L"/exit")break;
				write(socket_fd,msg.c_str(),msg.size()*4);
				continue;
			}
			if(FD_ISSET(socket_fd,&fdset)){
				int len=read(socket_fd,buf,BUF_SIZE);
				if(len<=0)break;
				utf8_char=(wchar_t*)buf;
				std::wcout << L"S :" <<utf8_char << std::endl;
			}
		}
		close(socket_fd);
		delete [] buf;
	} else {
		freeaddrinfo(res);
		return L"connect failure";
	}
	
	freeaddrinfo(res);
	return (const wchar_t *)0;
}
int main(int argc, char **argv){
	std::setlocale(LC_ALL,"");
	ParserCmd parser("c:s:",NULL);
	const wchar_t* error_msg=NULL;
	char *port;
	char *server_addr;
	int c;
	int as_client=1;
//	std::wcout << L"无码高清" << std::endl;
	if(argc == 1){
		usage(argv[0]);
		return 0;
	}
	parser.parser(argc,argv);
	while(1){
		c=parser.next();
		if(c==-1)break;
		switch(c){
		case 'c':
			server_addr=parser.opt_arg();
			as_client=1;
			break;
		case 's':
			port=parser.opt_arg();
			as_client=0;
			break;
		case '?':
			usage(argv[0]);
			return 0;
		}
	}
	if(as_client){
		if(parser.count_arg()==0)
			error_msg=L"no port specific";
		else{
			
			error_msg=connect_to(server_addr,parser.arg(0));
		}
	}
	else {
		error_msg=run_server(port);
	}

	if(error_msg)
		std::wcout << L"error:"<<error_msg << std::endl;
	return 0;
	
}


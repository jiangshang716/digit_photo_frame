#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>
#include "main.h"
#include "weather.h"
#include "text.h"
#include "framebuffer.h"
#include "font.h"

extern FB_FONT * font;
extern unsigned char *weather_info;
void show_weather(Tframebuffer *fb , unsigned char * text)
{
	fb_text_show_cn(fb, (unsigned char *)text, font, 0, 30);
}

void init_font()
{
	
	font = malloc(sizeof(FB_FONT));
	fb_font_open(FONT_PATH, font);
	fb_font_set_charsize(font, 100);
	
	weather_info = malloc(DB_NAME_MAX);

}

int weather_getinfo(unsigned char *weatherinfo, int second)
{
	int nfds;
	fd_set rfds;
	int retval;
	struct timeval tv;
	pid_t pid_weather;
	int pipefd[2];
	unsigned char weather_buf[DB_NAME_MAX + 1];

	memset(weatherinfo, 0, DB_NAME_MAX + 1);

	//int pipe(int pipefd[2]);
	if(pipe(pipefd) < 0){
		fprintf(stderr, "%s: pipe failed: %s\n", __func__, 
			strerror(errno));
		return -1;
	}

	if((pid_weather = fork()) < 0){
		fprintf(stderr, "fork() for weather failed: %s\n", 
			strerror(errno));
	}else if(pid_weather == 0){
		/* child process */
		close(pipefd[0]);

		if(weather_child_getinfo(weather_buf) < 0){
			fprintf(stderr, "%s: child get weather failed: %s\n",
				__func__, strerror(errno));
			
			exit(1);
		}
		
		//ssize_t write(int fd, const void *buf, size_t count);
		write(pipefd[1], weather_buf, DB_NAME_MAX);

		close(pipefd[1]);

		exit(0);
	}
	/* parent process */
	close(pipefd[1]);

	FD_ZERO(&rfds);
	FD_SET(pipefd[0], &rfds);
	nfds = pipefd[0] + 1;
	
	tv.tv_sec = second;
	tv.tv_usec = 0;

	//int select(int nfds, fd_set *readfds, fd_set *writefds,
        //          fd_set *exceptfds, struct timeval *timeout);
	retval = select(nfds, &rfds, NULL, NULL, &tv);

	if (retval == -1){
		fprintf(stderr, "%s: select() failed: %s", __func__,
			strerror(errno));
		close(pipefd[0]);

		return -1;
	}else if(FD_ISSET(pipefd[0], &rfds)){
		//ssize_t read(int fd, void *buf, size_t count);
		read(pipefd[0], weatherinfo, DB_NAME_MAX);
	}else{
		
		memcpy(weather_info , "get date failed -_-!\0" ,DB_NAME_MAX);	
		printf("get weather information failed.\n");
		close(pipefd[0]);
		
		return -1;
	}

	close(pipefd[0]);
	//int kill(pid_t pid, int sig);
	kill(pid_weather, SIGINT);
	waitpid(pid_weather, NULL, 0);

	return 0;
}

int weather_child_getinfo(unsigned char *weatherinfo)
{
	struct hostent *h;
	char **p;
	//char ip_address_string[INET6_ADDRSTRLEN + 1];
	int fd;
	struct sockaddr_in addr;
	char *hostname = "www.weather.com.cn";
	ssize_t n;
	char *request;
	char buf[128];

	//struct hostent *gethostbyname(const char *name);
	h = gethostbyname(hostname);
	if (h == NULL)
	{
		fprintf(stderr, "Failed to resolv domain name %s: %s\n", hostname, hstrerror(h_errno));
		return -1;
	}

#if 0
	struct hostent
	{
		char *h_name;		/* official name of host */
		char **h_aliases;		/* alias list */
		int h_addrtype;		/* host address type */
		int h_length;		/* length of address */
		char **h_addr_list;		/* list of addresses */
	}
#  define h_addr  h_addr_list[0]	/* for backward compatibility */
#endif

	p = h->h_addr_list;

	if((fd = socket(h->h_addrtype, SOCK_STREAM, 0)) < 0){
		perror("socket failed");
		return -1;
	}

	while (p && *p)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(80);
		addr.sin_addr = *((struct in_addr *)(*p));
		       
#if _DEBUG_
		memset(ip_address_string, 0, sizeof(ip_address_string));

		//const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);
		inet_ntop(h->h_addrtype, *p, ip_address_string, sizeof(ip_address_string));

		fprintf(stdout, "\t%s\n", ip_address_string);
#endif

		//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);		
		if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
			perror("connect failed");
			return -1;
		}else{
			fprintf(stdout, "connect successful\n");
			break;
		}

		p++;
	}

	request = "GET /weather/101010100.shtml\n";
	//ssize_t write(int fd, const void *buf, size_t count);
	write(fd, request, strlen(request));

	//ssize_t read(int fd, void *buf, size_t count);
	while((n = read(fd, buf, 7))){
		if(n < 0){
			perror("read failed");
			return -1;
		}else if(n == 0){
			break;
		}else{
			if(strncmp(buf, "<title>", 7) == 0){
				weather_filter_weather(fd, weatherinfo);
			}else{
				weather_nextline(fd);
			}
		}
	}

	close(fd);

	return 0;
}

int weather_filter_weather(int fd, unsigned char *weatherinfo)
{
	char buf[256];
	char *p;
	ssize_t n;

	
	n = read(fd, buf, sizeof(buf) - 1);
	if(n < 0){
		return -1;
	}
	buf[n] = '\0';
	
	p = strchr(buf, '<');
	*p = '\0';

	p = strstr(buf, ":");
#if _DEBUG_
	fprintf(stdout, "%s\n", p + 1);
#endif

	strncpy((char *)weatherinfo, p + 1, DB_NAME_MAX);
	
	return 0;
	
}

int weather_nextline(int fd)
{
	ssize_t n;
	char ch;

	while((n = read(fd, &ch, 1))){
		if(n < 0){
			perror("read failed");
			return -1;
		}else if(n == 0){
			break;
		}else{
			if(ch == '\n'){
				break;
			}
		}
	}
	
	return 0;
}


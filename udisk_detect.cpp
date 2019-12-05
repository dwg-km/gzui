#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include <sys/un.h> 
#include <sys/ioctl.h> 
#include <sys/socket.h> 
#include <linux/types.h> 
#include <linux/netlink.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define UEVENT_BUFFER_SIZE 2048 

static int DiskIndex = 0;

static int init_hotplug_sock() 
{ 
    const int buffersize = 1024; 
    int ret; 

    struct sockaddr_nl snl; 
    bzero(&snl, sizeof(struct sockaddr_nl)); 
    snl.nl_family = AF_NETLINK; 
    snl.nl_pid = getpid(); 
    snl.nl_groups = 1; 

    int s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT); 
    if (s == -1)  
    { 
        perror("socket"); 
        return -1; 
    } 
    setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize)); 

    ret = bind(s, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl)); 
    if (ret < 0)  
    { 
        perror("bind"); 
        close(s); 
        return -1; 
    } 

    return s; 
} 

int sd_check(const char * s){
	if(s == NULL)
		return -1;
	if(strlen(s) != 4)
		return -1;
	if(s[0] != 's')
		return -1;
	if(s[1] != 'd')
 		return -1;
	if(s[2] < 'b' || s[2] > 'f')
		return -1;
	if(s[3] < '1' && s[3] > '9')
		return -1;

	return 0;
}

int check_tty(const char * s)
{
	if(s == NULL)
		return -1;
	if(strlen(s) != 7)
		return -1;
	if(memcmp(s, "ttyUSB", 6))
		return -1;
	if(s[6] < '0' || s[6] > '9')
		return -1;

	return 0;
}
#define		RM_SHELL	\
"path=$(mount | grep %s | cut -d ' ' -f3)\n\
if [ $path ];then\n\
	umount $path\n\
	rmdir $path\n\
fi\n\
"

int deal_one_line(char * const s)
{
	char * h = s;
	char * header = s;
	int len =  strlen(s);
	while(*h){
		if(*h == '@'){
			*h = '\0';
			break;
		}
		h++;
	}

	char * d = s;
	char * device = 0;
	while(len--){
		if(d[len] == '/'){
			device = &d[len + 1];
			break;	
		}
	}

	if(header && device && *header && *device){
		if(sd_check(device) == 0){
			char cmd[128];
			char dev[16];
			char path[16];
			sprintf(dev, "%s/%s", "/dev", device);
			if(strcmp(header, "add") == 0){
				sprintf(path, "%s%d", "/media/udisk", DiskIndex++);
				sprintf(cmd, "%s %s", "mkdir", path);
				printf("%s\n", cmd);
				system(cmd);
				sprintf(cmd, "%s %s %s", "mount", dev, path);
				printf("%s\n", cmd);
				system(cmd);
			}else if(strcmp(header, "remove") == 0){
				sprintf(cmd, RM_SHELL, dev);
				printf("%s\n", cmd);
				system(cmd);
			}
		}else if(check_tty(device) == 0){
			if(strcmp(header, "add") == 0){
				printf("add %s\n", device);
			}else if(strcmp(header, "remove") == 0){
				printf("remove %s\n", device);
			}
		}
	}

	return 0;
}

static char buf[UEVENT_BUFFER_SIZE * 2]; 
void udisk_thread() 
{
	system("rm -rf /media/udisk*");
	int hotplug_sock = init_hotplug_sock();
	while(1){
		/* Netlink message buffer */
		int len = recv(hotplug_sock, &buf, sizeof(buf), 0);
		char * s = buf;
		int cnt = 0;
		int n = 0;
		//printf("recv len=%d : len %d\n", len, strlen(buf));
		if(len > 0 && len <= UEVENT_BUFFER_SIZE){
			deal_one_line(s);
		}
	}
}


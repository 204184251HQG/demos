#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <strings.h>
#include <errno.h>

#define FIFO_CLEAR 0x1
#define BUFFER_LEN 20
#define READ_BUF_SIZE 1024
uint8_t read_buf[READ_BUF_SIZE];
void main(void)
{
	int fd;

	fd = open("/dev/globalfifo", O_RDONLY | O_NONBLOCK);
	if (fd != -1) {
		struct epoll_event ev_globalfifo;
		int err;
		int epfd;

		printf("open globalfifo dev fd %d\n", fd);
		if (ioctl(fd, FIFO_CLEAR, 0) < 0)
			printf("ioctl command failed\n");

		epfd = epoll_create(1);
		if (epfd < 0) {
			perror("epoll_create()");
			return;
		}

		bzero(&ev_globalfifo, sizeof(struct epoll_event));
		ev_globalfifo.events = EPOLLIN | EPOLLPRI;
		ev_globalfifo.data.fd = fd;
		err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev_globalfifo);
		if (err < 0) {
			perror("epoll_ctl()");
			return;
		}
		while(1)
		{
			int ret = 0;
			ret = epoll_wait(epfd, &ev_globalfifo, 1, 5000);
			if (ret < 0) {
				perror("epoll_wait()");
			} else if (ret == 0) {
				printf("No data input in FIFO within 5 seconds.\n");
			} else {
				if(ret != 0)
					printf("FIFO is not empty %d, event %d, fd %d\n", ret, ev_globalfifo.events, ev_globalfifo.data.fd);
					ret = read(ev_globalfifo.data.fd,read_buf,READ_BUF_SIZE);
					printf("%s", read_buf);
			}
		}
		err = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev_globalfifo);
		if (err < 0)
			perror("epoll_ctl()");
	} else {
		printf("Device open failure %d\n", errno);
	}
}

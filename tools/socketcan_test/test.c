#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int soc;

int open_socket(const char *ifname)
{
	struct ifreq ifr;
	struct sockaddr_can addr;

	if((soc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error: socket - ");
		return -1;
	}
	strcpy(ifr.ifr_name, ifname);
	if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0) {
		perror("Error: ioctl - ");
		return -1;
	}

	// ioctl(soc, F_SETFL, O_NONBLOCK);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if(bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error: bind - ");
		return -1;
	}

	return 0;
}

int close_socket()
{
	close(soc);
	return 0;
}

int send_can_frame(struct can_frame *frame)
{
	int retbyte;

	retbyte = write(soc, frame, sizeof(struct can_frame));
	if (retbyte != sizeof(struct can_frame)) {
		perror("Error: write");
		return -1;
	}
	return 0;
}

/* this is just an example, run in a thread */
void read_port()
{
	struct can_frame frame_rd;
	int recvbytes = 0;

	int read_can_port = 1;
	while(read_can_port)
	{
		struct timeval timeout = {1, 0};
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(soc, &readSet);

		if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0)
		{
			if (!read_can_port)
			{
				break;
			}
			if (FD_ISSET(soc, &readSet))
			{
				recvbytes = read(soc, &frame_rd, sizeof(struct can_frame));
				if(recvbytes)
				{
					printf("dlc = %d, data = %s\n", frame_rd.can_dlc,frame_rd.data);
				}
			}
		}
	}
}

int main()
{
	int rc;
	struct can_frame f1;

	rc = open_socket("vcan0");
	if (rc < 0) {
		fprintf(stderr, "Fail to open_socket()\n");
		return -1;
	}

#if 0
	read_port();
#endif
	
	f1.can_id = 0x321;
	f1.can_dlc = 3;
	f1.data[0] = 0x33;
	f1.data[1] = 0x22;
	f1.data[2] = 0x11;

	send_can_frame(&f1);
#endif

	close_socket();
	return 0;
}




/*
 * netdevice(7) - low-level access to Linux network devices
 */

#define _GNU_SOURCE

#include <linux/unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/klog.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <linux/fs.h>

//#define DEBUG

#ifdef DEBUG
	#include <assert.h>
#endif

#include <syslog.h>

static ssize_t read_kmsg_one(int kmsg, char* kmsg_buf, int kmsg_buf_sz)
{
	ssize_t size;
	/* kmsg returns EPIPE if record was modified while reading */
	do {
		size = read(kmsg, kmsg_buf, kmsg_buf_sz);
	} while (size < 0 && (errno == EPIPE || errno == EINVAL));

	return size;
}

void EmptyKMsg(int kmsg, char* kmsg_buf, int kmsg_buf_sz)
{

	int oldfl;

	// empty kmsg buffer after reload

	oldfl = fcntl(kmsg, F_GETFL);
	if (oldfl == -1)
	{
		exit(2);
	}
	oldfl = fcntl(kmsg, F_SETFL, oldfl | O_NONBLOCK);

	if (oldfl == -1)
	{
		exit(3);
	}

	while (errno != EAGAIN)
	{
		read_kmsg_one(kmsg, kmsg_buf, 32);
	}

	// back to blocking mode
	fcntl(kmsg, F_SETFL, oldfl & ~O_NONBLOCK);
	if (oldfl == -1)
	{
		exit(3);
	}
}

void SetWifi(int on)
{
	if (on)
	{
		system("/usr/sbin/wifireboot 1");
	}
	else
	{
		system("/usr/sbin/wifireboot 0");
	};
}

#define MIN_KMSG_LEN 90
#define MAX_KMSG_LEN 234
int main(void)
{
	int kmsg;
	char kmsg_buf[MAX_KMSG_LEN]; // fits all messages we want to read. EINVAL for others
	ssize_t kmsg_len;
	char* kmsg_buf_seek;
	const char * const findstr1   = "SUBSYSTEM=ieee80211";
	const char * const findstr2   = "DEVICE=+ieee80211:";
	const char * const findstr3   = "rt2x00queue_";
	const char * const findstr3_a = "rt2x00queue_write_tx_frame";
	const char * const findstr3_b = "rt2x00queue_flush_queue";

	openlog("rtrebooter", LOG_PID, LOG_DAEMON);

	syslog(LOG_NOTICE, "Starting...");

	kmsg = open("/dev/kmsg", O_RDONLY);
	if (kmsg < 0)
	{
		syslog(LOG_ERR, "Fatal: /dev/kmsg");
		return 1;
	}

	// ???
	if (lseek(kmsg, 0, SEEK_DATA) < 0)
	{
		syslog(LOG_ERR, "Fatal: seekfail");
		return 2;
	}

	syslog(LOG_NOTICE, "Started");

	while (1)
	{

		kmsg_len = read_kmsg_one(kmsg, kmsg_buf, sizeof(kmsg_buf) - 1);

		if (kmsg_len < 0)
		{
			if (errno == EINVAL)
				syslog(LOG_ERR, "Fatal: read_kmsg_one");
			return 3;
		};

		kmsg_buf[kmsg_len] = '\0';

#ifdef DEBUG
		printf("%s", kmsg_buf);
#endif

		// Not big enough
		if (kmsg_len < MIN_KMSG_LEN)
		{
#ifdef DEBUG
			printf("> minmsg: %d\n\n", kmsg_len);
#endif
			continue;
		}

		// Let's start finding things

		kmsg_buf_seek = kmsg_buf;
		kmsg_buf_seek = strstr(kmsg_buf_seek, findstr1);

		if (kmsg_buf_seek == NULL)
		{
#ifdef DEBUG
			printf("> missing findstr1\n\n");
#endif
			continue;
		}

		kmsg_buf_seek = strstr(kmsg_buf, findstr2);

		if (kmsg_buf_seek == NULL)
		{
#ifdef DEBUG
			printf("> missing findstr2\n\n");
#endif
			continue;
		}

		kmsg_buf_seek = kmsg_buf;

		if (strstr(kmsg_buf, findstr3) == NULL)
		{
#ifdef DEBUG
			printf("> missing findstr3\n\n");
#endif
			continue;
		}

		int do_reload = 0;
		if (strstr(kmsg_buf, findstr3_a) != NULL)
		{
			do_reload = 1;
		}
		else if (strstr(kmsg_buf, findstr3_b) != NULL)
		{
			do_reload = 2;
		};

		if (do_reload > 0)
		{
			syslog(LOG_NOTICE, "Detected malfunction, reloading");

			SetWifi(0);

			EmptyKMsg(kmsg, kmsg_buf, sizeof(kmsg_buf) - 1);

			SetWifi(1);
			syslog(LOG_NOTICE, "Reloaded, monitoring again");
		}
		else
		{
#ifdef DEBUG
			printf("> not a detected wifi error\n\n");
#endif
		}
	}

	syslog(LOG_NOTICE, "Terminating (bug?)");

	return 0;
}

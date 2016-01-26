/*

TODO: assumes single phy device

*/

#define _GNU_SOURCE

#include <linux/unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/klog.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <linux/fs.h>

//#define DEBUG

#define also_rt2x00queue_flush_queue

#ifdef DEBUG
	#include <assert.h>
#endif

static ssize_t read_kmsg_one(int kmsg, char * kmsg_buf, int kmsg_buf_sz )
{
	ssize_t size;
	/* kmsg returns EPIPE if record was modified while reading */
	do {
		size = read(kmsg, kmsg_buf, kmsg_buf_sz );
	} while (size < 0 && (errno == EPIPE || errno == EINVAL) );

	return size;
}


void RebootWifi(int kmsg, char * kmsg_buf, int kmsg_buf_sz) {
	
	int oldfl;
	ssize_t sz;
	
	system("/root/wifireboot 0");
	
	// empty kmsg buffer after reboot
	
	oldfl = fcntl(kmsg, F_GETFL);
	if (oldfl == -1) {
		exit(2);
	}
	oldfl = fcntl(kmsg, F_SETFL, oldfl | O_NONBLOCK);
	
	if (oldfl == -1) {
		exit(3);
	}
	
	while (errno!=EAGAIN) {
		sz = read_kmsg_one(kmsg, kmsg_buf, 32);
	}
	
	// back to blocking mode
	fcntl(kmsg, F_SETFL, oldfl & ~O_NONBLOCK);
	if (oldfl == -1) {
		exit(3);
	}
	
	// now we are ready to receive new bad messages
	
	system("/root/wifireboot 1");
	
}


#define MINMSG 100
#define sizeof_str(x) (sizeof(x) - 1)
int main(void)
{
	int	kmsg; 
	char kmsg_buf[255]; // fits all messages we want to read. EINVAL for others.
	ssize_t sz;
	size_t offset;
	char * kmsg_buf_offset,kmsg_buf_offset2;
	const char findstr1[] = "DEVICE=+ieee80211:";
	const char findstr2[] = "\n SUBSYSTEM=ieee80211\n";

#ifdef DEBUG
	assert(MINMSG>(sizeof_str(findstr2)+sizeof_str(findstr1)+46));
	
	assert(sizeof_str(findstr1)>11);
	
	assert(MINMSG<sizeof_str(kmsg_buf));
#endif

	
	kmsg = open("/dev/kmsg", O_RDONLY);
	if (kmsg < 0) {
		return 1;
	}
	
	if (lseek(kmsg, 0, SEEK_DATA) < 0) {
		
		return 2;
	}	
	
	
	while (1) {
		
		sz = read_kmsg_one(kmsg, kmsg_buf, sizeof_str(kmsg_buf));
		
		if (sz<=0) break;
		
		*(kmsg_buf + sz) = '\0';
		
#ifdef DEBUG
		printf("%s",kmsg_buf);
#endif
		
		if (sz<MINMSG) 
		{
#ifdef DEBUG
			printf("> minmsg: %d\n\n",sz);
#endif
			continue;
		}
		
		
		kmsg_buf_offset = kmsg_buf+sz-1-sizeof_str(findstr1)-32;
		

		if (kmsg_buf_offset<=kmsg_buf) {
#ifdef DEBUG
			printf("> negoff: %d\n\n",kmsg_buf-kmsg_buf_offset);
#endif			
			continue;
		}
		
		kmsg_buf_offset = memmem(kmsg_buf_offset,sizeof_str(findstr1)+32-1,findstr1,sizeof_str(findstr1));
		
		if (!kmsg_buf_offset) {
#ifdef DEBUG
			printf("> missing findstr1\n\n");
#endif			
			continue;
		}
		
		kmsg_buf_offset2 = kmsg_buf_offset-sizeof_str(findstr2)-5-1;
		
		if (kmsg_buf_offset2<=kmsg_buf) {
#ifdef DEBUG
			printf("> missing findstr2\n\n");
#endif			
			continue;
		}
	
		kmsg_buf_offset = memmem(	kmsg_buf_offset2,
									sizeof_str(findstr2) + 5,
									findstr2,
									sizeof_str(findstr2)     );
									

		if (!kmsg_buf_offset) {
#ifdef DEBUG
			printf("> missing findstr2\n\n");
#endif			
			continue;
		}
		
		kmsg_buf_offset = strchr(kmsg_buf,';');
		if (kmsg_buf_offset &&
			
			#ifdef also_rt2x00queue_flush_queue
				strstr(kmsg_buf_offset,"rt2x00queue_") 	  && ( 
				strstr(kmsg_buf_offset,"rt2x00queue_flush_queue") 
				||
			#endif
				strstr(kmsg_buf_offset,"rt2x00queue_write_tx_frame")
			#ifdef also_rt2x00queue_flush_queue
				)
			#endif
				
			
		) {
			
			RebootWifi(kmsg, kmsg_buf, sizeof_str(kmsg_buf));
			
		}
		
#ifdef DEBUG
		else {
			printf("> No rt2x00queue_write_tx_frame\n\n");
		}
		
		printf("\n");
		
#endif
		

	}
	
	return 0;
}

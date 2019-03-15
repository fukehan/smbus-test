#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
//#include <pthread.h>
//#include "../../include/test_utils.h"

#define I2C_SLAVE_FORCE 0x0706

#define I2C_MASTER_DEV "/dev/i2c-0"
#define I2C_SLAVE_ADDR 0x2e
#define I2C_SLAVE_DEV "/dev/slave-i2c-0"

#define I2C_SLAVE_TEST_DATA 0x55
#define I2C_SLAVE_TEST_NUM 10

static char master_buffer[I2C_SLAVE_TEST_NUM];
static int int_buffer[I2C_SLAVE_TEST_NUM];
static char slave_buffer[I2C_SLAVE_TEST_NUM];

#if 0
static void *slave_thread(void *data)
{
	int fd;
	int ret, i;

	fd = open(I2C_SLAVE_DEV, O_RDWR);
	if (fd < 0) {
		printf("i2c-slave-test:open device error\n");
		printf("i2c-slave-test: slave thread: Fail\n");
		return 0;
	}

	memset(slave_buffer, I2C_SLAVE_TEST_DATA, I2C_SLAVE_TEST_NUM);

	printf
		("i2c-slave-test: slave thread: about to send %d bytes, value=0x%x\n",
		 I2C_SLAVE_TEST_NUM, I2C_SLAVE_TEST_DATA);

	ret = write(fd, slave_buffer, I2C_SLAVE_TEST_NUM);

	if (ret < 0) {
		printf("i2c-slave-test: slave thread: send data error\n");
		goto fail;
	} else {
		printf
			("i2c-slave-test: slave thread: send %d bytes, while %d wanted\n",
			 ret, I2C_SLAVE_TEST_NUM);
	}

	memset(slave_buffer, 0, I2C_SLAVE_TEST_NUM);
	printf
		("i2c-slave-test: slave thread: about to receive %d bytes, value=0x%x\n",
		 I2C_SLAVE_TEST_NUM, I2C_SLAVE_TEST_DATA);

	ret = read(fd, slave_buffer, I2C_SLAVE_TEST_NUM);
	if (ret < 0) {
		printf("i2c-slave-test: slave thread: receive data error\n");
		goto fail;
	} else {
		for (i = 0; i < I2C_SLAVE_TEST_NUM; i++) {
			if (slave_buffer[i] != I2C_SLAVE_TEST_DATA){
				printf
					("i2c-slave-test: slave thread: received data wrong!\n");
				goto fail;
			}
		}

		printf
			("i2c-slave-test: slave thread: reveive %d bytes while %d wanted\n",
			 ret, I2C_SLAVE_TEST_NUM);
	}

	printf("i2c-slave-test: slave thread: Success\n");
	close(fd);
	return 0;

fail:
	printf("i2c-slave-test: slave thread: Fail\n");
	close(fd);

	return 0;
}
#endif

int main(int argc, char *argv[])
{
	int fd;
	int ret, i, j;
	//pthread_t slave_thread_id;

	//print_name(argv);
	printf("args: %d,%s\n", argc, argv[0]);

	fd = open(I2C_MASTER_DEV, O_RDWR);
	if (fd < 0) {
		printf("i2c-slave-test:master thread:open device error\n");
		return -1;
	}

	ret = ioctl(fd, I2C_SLAVE_FORCE, I2C_SLAVE_ADDR);
	if (ret < 0) {
		printf("i2c-slave-test:master thread:ioctl error\n");
		goto fail;
	}

	//pthread_create(&slave_thread_id, 0, slave_thread, (void *)0);

	//usleep(1000*100);

	if (argc == 1)
	{
		/*reveive data test */
		//printf("i2c-slave-test:master thread:about to receive %d bytes\n",
		//       I2C_SLAVE_TEST_NUM);
		while (1) {
			printf("cpu1 cpu2 cpu3 cpu4 cpu5 cpu6 cpu7 cpu8\n");
			for (i = 0; i < 8; i++)
			{
				for(j = 0; j < 4; j++) {
					master_buffer[j] = 4 * i + j;
					write(fd, &master_buffer[j], 1);
					ret = read(fd, &master_buffer[j], 1);
					if (ret < 0) {
						printf("i2c-slave-test:master thread: read error\n");
						goto retry;
					}
				}
				int_buffer[i] = (master_buffer[0] << 24) + (master_buffer[1] << 16) + (master_buffer[2] << 8) + (master_buffer[3] << 0);
				//printf("0x%x%x%x%x ", master_buffer[0],master_buffer[1],master_buffer[2],master_buffer[3]);
			}
			//printf("\n");
			for (i = 0; i < 8; i++) {
				//if (master_buffer[i] != I2C_SLAVE_TEST_DATA) {
				//	printf
				//	    ("i2c-slave-test:master thread:received data error\n");
				//	goto fail;
				//}
				//printf("i2c-slave-test:master thread:received data %d\n", master_buffer[i]);
				//printf("%d ", master_buffer[i]);
				printf("%d ", int_buffer[i]);
			}
			printf("\n");

			//printf
			//    ("i2c-slave-test:master thread:%d bytes received, while %d bytes wanted\n",
			//     I2C_SLAVE_TEST_NUM, I2C_SLAVE_TEST_NUM);

			for (i = 0; i < 8; i++)
			{
				for(j = 0; j < 4; j++) {
					master_buffer[j] = 40 + 4 * i + j;
					write(fd, &master_buffer[j], 1);
					ret = read(fd, &master_buffer[j], 1);
					if (ret < 0) {
						printf("i2c-slave-test:master thread: read error\n");
						goto retry;
					}
				}
				int_buffer[i] = (master_buffer[0] << 24) + (master_buffer[1] << 16) + (master_buffer[2] << 8) + (master_buffer[3] << 0);
				//printf("0x%x%x%x%x ", master_buffer[0],master_buffer[1],master_buffer[2],master_buffer[3]);
			}
			//printf("\n");
			for (i = 0; i < 8; i++) {
				printf("%d ", int_buffer[i]);
			}
			printf("\n");
retry:
			sleep(1);
		}
	}

	if (argc > 1)
	{
		/*send data test */
		printf("i2c-slave-test:master thread:about to send %d bytes\n",
				I2C_SLAVE_TEST_NUM);
		memset(master_buffer, I2C_SLAVE_TEST_DATA, I2C_SLAVE_TEST_NUM);
		for (i = 0; i < argc - 1; i++)
		{
			master_buffer[i+1] = atoi(argv[i+1]);
		}
		master_buffer[0] = 0;
		ret = write(fd, master_buffer, I2C_SLAVE_TEST_NUM);

		if (ret < 0) {
			printf("i2c-slave-test:master thread: send error\n");
			goto fail;
		} else {
			printf
				("i2c-slave-test:master thread:%d bytes sent, while %d bytes wanted\n",
				 I2C_SLAVE_TEST_NUM, I2C_SLAVE_TEST_NUM);
		}
	}

	printf("i2c-slave-test:master thread: Success\n");
	close(fd);

	/*wait slave thread to finish. */
	usleep(1000*100);
	return 0;

fail:
	printf("i2c-slave-test:master thread: Fail\n");
	close(fd);

	return -1;
}

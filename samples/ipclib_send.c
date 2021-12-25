#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "siglib.h"
#include "debug.h"
#include "ipc.h"
#include "daemon.h"

void app_msg_handler(void *data)
{
	struct ipc_msg *msg = (struct ipc_msg *)data;

	switch(msg->type) {
	case MSG_TYPE_WATCHDOG:
		pr_info("watchdog message received!\n");
		ipc_watchdog_feed();
		break;
	default:
		pr_info("unexpected message received! type:%d\n", msg->type);
		ipc_stop_loop();
		break;
	}
}


int main(int argc, char *argv[])
{
    int ret;
	ret = ipc_init("send", app_msg_handler);
	if (ret < 0)
		err_exit("ipc_init error\n");

	if(argc < 4) {
		printf("Usage:%s <app_name> <source> <content>\n", argv[0]);
		printf("Example:%s mango %s \"I am %s.\"\n", argv[0], argv[0], argv[0]);
		return -1;
	}

    char *app_name = argv[1];

	struct ipc_msg msg = {0};
	msg.type = 1;
	memcpy(msg.source, argv[2], strlen(argv[2]));
	memcpy(msg.content, argv[3], strlen(argv[3]));

    ret = ipc_send_msg_async(app_name, &msg);
    if(ret) printf("ipc_send_msg_async fail\n");

	return 0;
}

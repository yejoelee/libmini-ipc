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
    
    char *app_name = argv[1];

	struct ipc_msg msg = {0};

    ret = ipc_send_msg_async(app_name, &msg);
    if(ret) printf("ipc_send_msg_async fail\n");

	return 0;
}

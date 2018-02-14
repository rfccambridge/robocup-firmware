#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xbee.h>


#define XBEE_BAUD_RATE 9600
#define XBEE_USB_ADDR "/dev/ttyUSB1"
#define XBEE_MODEL_NO "xbee1"
#define STOP "0,0,0"
#define FORWARD "90,0,0"
#define BACKWARD "-90,0,0"
#define LEFT "0,-90,0"
#define RIGHT "0,90,0"
#define FORWARD_RIGHT "90,0,20"
#define FORWARD_LEFT "90,0,-20"

int main(void) {
	int i;

	void *d;
	struct xbee *xbee;
	struct xbee_con *con;
	struct xbee_conAddress address;
	struct xbee_conSettings settings;
	xbee_err ret;
	printf("hello\n");
	if ((ret = xbee_setup(&xbee, XBEE_MODEL_NO, XBEE_USB_ADDR, XBEE_BAUD_RATE)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		return ret;
	}
	printf("hello\n");
	memset(&address, 0, sizeof(address));
	address.addr64_enabled = 1;
	address.addr64[0] = 0x00;
	address.addr64[1] = 0x00;
	address.addr64[2] = 0x00;
	address.addr64[3] = 0x00;
	address.addr64[4] = 0x00;
	address.addr64[5] = 0x00;
	address.addr64[6] = 0xFF;
	address.addr64[7] = 0xFF;
	printf("hello\n");
	if ((ret = xbee_conNew(xbee, &con, "64-bit Data", &address)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
		return ret;
	}

	printf("hello\n");
	/* getting an ACK for a broadcast message is kinda pointless... */
	xbee_conSettings(con, NULL, &settings);
	settings.disableAck = 1;
	xbee_conSettings(con, &settings, NULL);

	printf("hello\n");
	//for (i = 0; i < 1000; i++) {
	//	xbee_conTx(con, NULL, "%d\r\n", i);
	//	printf("%d\n", i);
	//	/* XBee Series 1 modules don't use meshing, so you can broadcast much faster than Series 2 */
	//	usleep(10000); /* 10ms */
	//}
	char command[20];
	for(;;) {
		gets(command);
		//xbee_conTx(con, NULL, "%s\r\n", command);
		switch(command[0]){
			case 'w':{
				xbee_conTx(con, NULL, "%s\r\n", FORWARD);
				break;
			}
			case 's':{
				xbee_conTx(con, NULL, "%s\r\n", BACKWARD);
				break;
			}
			case 'a':{
				xbee_conTx(con, NULL, "%s\r\n", LEFT);
				break;
			}
			case 'd':{
				xbee_conTx(con, NULL, "%s\r\n", RIGHT);
				break;
			}
			case 'e':{
				xbee_conTx(con, NULL, "%s\r\n", FORWARD_RIGHT);
				break;
			}
			case ' ':{
				xbee_conTx(con, NULL, "%s\r\n", STOP);
				break;
			}
		}
	}

	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}

	xbee_shutdown(xbee);

	return 0;
}

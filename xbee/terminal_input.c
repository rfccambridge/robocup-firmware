/*
	libxbee - a C/C++ library to aid the use of Digi's XBee wireless modules
	          running in API mode.

	Copyright (C) 2009 onwards  Attie Grande (attie@attie.co.uk)

	libxbee is free software: you can redistribute it and/or modify it
	under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libxbee is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with libxbee. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xbee.h>


void initialize_xbee(struct xbee_con *con, struct xbee *xbee, xbee_err ret) {
	int i;
	void *d;
	struct xbee_conAddress address;
	struct xbee_conSettings settings;
	if ((ret = xbee_setup(&xbee, "xbee1", "/dev/ttyUSB0", 9600)) != XBEE_ENONE) {
		printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
		exit(ret);
	}

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

	if ((ret = xbee_conNew(xbee, &con, "64-bit Data", &address)) != XBEE_ENONE) {
		xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
		exit(ret);
	}
	xbee_conSettings(con, NULL, &settings);
	settings.disableAck = 1;
	xbee_conSettings(con, &settings, NULL);
}

int main(void) {
	xbee_err ret;
	struct xbee* xb;
	struct xbee_con* con;
	initialize_xbee(con, xb, ret);
	
	char command[20];
	for(;;) {
		gets(command);
		xbee_conTx(con, NULL, "%s\r\n", command);
		if (strcmp(command, "p") == 0) {
			break;
		}
	}

	if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
		xbee_log(xb, -1, "xbee_conEnd() returned: %d", ret);
		return ret;
	}

	xbee_shutdown(xb);

	return 0;
}

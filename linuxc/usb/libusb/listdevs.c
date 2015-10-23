/*
 * libusb example program to list devices on the bus
 * Copyright © 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>

#include "libusb-1.0/libusb.h"

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	struct libusb_config_descriptor *conf_desc;
	int nb_ifaces = 0;
	int i = 0, j = 0, n = 0;
	uint8_t path[8]; 

	while ((dev = devs[n++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");

		libusb_get_config_descriptor(dev, 0, &conf_desc);
		nb_ifaces = conf_desc->bNumInterfaces;
		fprintf(stdout, "nb_ifaces:::%d\n", nb_ifaces);
		for (i=0; i<nb_ifaces; i++) {
			printf("              interface[%d]: id = %d\n", i,
				conf_desc->interface[i].altsetting[0].bInterfaceNumber);
			for (j=0; j<conf_desc->interface[i].num_altsetting; j++) {
				printf("interface[%d].altsetting[%d]: num endpoints = %d\n",
					i, j, conf_desc->interface[i].altsetting[j].bNumEndpoints);
				printf("   Class.SubClass.Protocol: %02X.%02X.%02X\n",
					conf_desc->interface[i].altsetting[j].bInterfaceClass,
					conf_desc->interface[i].altsetting[j].bInterfaceSubClass,
					conf_desc->interface[i].altsetting[j].bInterfaceProtocol);
			}
		}
	}
}

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	return 0;
}

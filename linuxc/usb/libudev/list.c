/**
 * @brief 使用libudev列出所有usb设备并检测usb设备插拔
 *
 * 编译: gcc list.c -o list -Wall -ludev 
 * 参考: http://www.signal11.us/oss/udev/
 */
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct udev *udev;
	struct udev_enumerate* enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;

	udev = udev_new();
	/* Create the udev object */
	if (!udev) {
		fprintf(stderr, "Can't create udev\n");
		exit(1);
	}
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	udev_list_entry_foreach(dev_list_entry, devices) {
		const char * path;
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);
		if (!dev) {
			continue;
		}

		const char *devnode;
		devnode = udev_device_get_devnode(dev);
		if (!devnode) {
			continue;
		}
		fprintf(stdout, "Device Node Path: %s", devnode);
		fprintf(stdout, "  VID/PID: %s %s",
		        udev_device_get_sysattr_value(dev,"idVendor"),
		        udev_device_get_sysattr_value(dev, "idProduct"));
		fprintf(stdout, "  %s  %s\n",
		        udev_device_get_sysattr_value(dev,"manufacturer"),
		        udev_device_get_sysattr_value(dev,"product"));
		udev_device_unref(dev);
	}
	udev_enumerate_unref(enumerate);

    /* 检测usb插拔 */
	struct udev_monitor* mon;
	int fd;
	mon = udev_monitor_new_from_netlink(udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", "usb_device");
	udev_monitor_enable_receiving(mon);
	fd = udev_monitor_get_fd(mon);
    
	while (1) {
		fd_set fds;
		struct timeval tv;
		int ret;

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		ret = select(fd+1, &fds, NULL, NULL, &tv);
		if (ret>0 && FD_ISSET(fd, &fds)) {
			fprintf(stdout, "\nselect() says there should be data\n");
			dev = udev_monitor_receive_device(mon);
			if (dev) {
				printf("Got Device\n");
				printf("   Node: %s\n", udev_device_get_devnode(dev));
				printf("   Subsystem: %s\n", udev_device_get_subsystem(dev));
				printf("   Devtype: %s\n", udev_device_get_devtype(dev));

				printf("   Action: %s\n",udev_device_get_action(dev));
				udev_device_unref(dev);
			} else {
				printf("No Device from receive_device(). An error occured.\n");
			}
		}
		usleep(250*1000);
		printf(".");
		fflush(stdout);
	}

	udev_unref(udev);
	return 0;
}

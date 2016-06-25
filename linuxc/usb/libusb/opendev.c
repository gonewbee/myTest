#include <stdio.h>
#include "libusb-1.0/libusb.h"

static void zs_callback(struct libusb_transfer *xfer) {
    int *completed = xfer->user_data;
    fprintf(stdout, "status:%d actual_length:%d\n", xfer->status, xfer->actual_length);
    int i;
    fprintf(stdout, "host->device:");
    for (i=0; i<8; i++) {
        fprintf(stdout, "%x ", xfer->buffer[i]);
    }
    fprintf(stdout, "\ndevice->host:");
    for (; i<0x12+8; i++) {
        fprintf(stdout, "%x ", xfer->buffer[i]);
    }
    fprintf(stdout, "\n");
    *completed = 1;
}

/**
 * @brief 取消使用linux驱动
 */
static void usb_host_detach_kernel(libusb_device *dev, libusb_device_handle *dh) {
    struct libusb_config_descriptor *conf;
    int rc, i;

    // 获取当前生效的usb配置的描述
    rc = libusb_get_active_config_descriptor(dev, &conf);
    if (0!=rc) {
        fprintf(stdout, "libusb_get_active_config_descriptor failed:%d\n", rc);
        return;
    }
    for (i=0; i<conf->bNumInterfaces; i++) {
        // 判断该接口上的kernel驱动是否有效，0无效 1有效
        rc = libusb_kernel_driver_active(dh, i);
        if (1!=rc) {
            continue;
        }
        rc = libusb_detach_kernel_driver(dh, i);
        fprintf(stdout, "detech %i driver ret:%d\n", i, rc);
    }
    libusb_free_config_descriptor(conf);
}

/**
 * @brief 使用linux驱动
 */
static void usb_host_attach_kernel(libusb_device *dev, libusb_device_handle *dh) {
    struct libusb_config_descriptor *conf;
    int rc, i;

    rc = libusb_get_active_config_descriptor(dev, &conf);
    if (0!=rc) {
        return;
    }
    for (i=0; i<conf->bNumInterfaces; i++) {
        libusb_attach_kernel_driver(dh, i);
    }
    libusb_free_config_descriptor(conf);
}

static void usb_host_control_test(libusb_device_handle *dh) { 
    int completed = 0;
    struct libusb_transfer *xfer = libusb_alloc_transfer(0);
    // buffer的长度为输出和输入总长度和
    uint8_t buffer[8+0x12] = {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x12, 0x00};
    libusb_fill_control_transfer(xfer, dh, buffer, zs_callback, &completed, 1000);
    libusb_submit_transfer(xfer);
    
    int i = 0;
    // 异步传输，使用libusb_handle_events响应usb事件
    for (i=0; i<10&&!completed; i++) {
        libusb_handle_events(NULL);
    }
}

int main(int argc, char *argv[]) {
    uint16_t vendor_id = 0x0930;
    uint16_t product_id = 0x6545;

    // 必须有libusb_init，否则出现异常
    libusb_init(NULL);
    // 按照vid和pid打开设备
    libusb_device_handle *dh = libusb_open_device_with_vid_pid(NULL, vendor_id, product_id);
    if (!dh) {
        fprintf(stderr, "%x %x open error!\n", vendor_id, product_id);
        return 1;
    }

    libusb_device *device = libusb_get_device(dh);
    uint8_t bus_num = libusb_get_bus_number(device);
    uint8_t addr = libusb_get_device_address(device);
    int speed = libusb_get_device_speed(device);
    fprintf(stdout, "bus:%d addr:%d speed:%d\n", bus_num, addr, speed);

    usb_host_detach_kernel(device, dh);

    sleep(2);
    usb_host_control_test(dh);

    usb_host_attach_kernel(device, dh);
    libusb_close(dh);
    return 0;
}


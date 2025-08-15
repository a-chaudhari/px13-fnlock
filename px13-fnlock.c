#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, char *argv[])
{
    int hidraw_fd, fn_lock, retval, ioctl_res;
    if (argc != 3)
    {
        printf("Usage: %s <device_path> <off|on>\n", argv[0]);
        printf("Example: %s /dev/hidraw1 on\n", argv[0]);
        return 1;
    }

    // validate path
    if (strncmp(argv[1], "/dev/hidraw", 11) != 0) {
        printf("Invalid device path: %s. Use a path like /dev/hidrawX.\n", argv[1]);
        return 1;
    }

    // parse the on/off argument
    if (strcmp(argv[2], "on") == 0) {
        fn_lock = 0;
    } else if (strcmp(argv[2], "off") == 0) {
        fn_lock = 1;
    } else {
        printf("Invalid argument: %s. Use 'on' or 'off'.\n", argv[2]);
        return 1;
    }

    /*
     *  byte #4 (index 3) toggles with fn lock off an on.
     *  0 = fn lock on (ie: f1-12 buttons act as hotkeys)
     *  1 = fn lock off (ie: f1-12 buttons act as normal)
     */
    unsigned char bytes[] = {
        0x5a, 0xd0, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x53, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    bytes[3] = fn_lock; // Set fn lock byte

    // Open the hidraw device
    hidraw_fd = open(argv[1], O_RDWR);
    if (hidraw_fd < 0) {
        perror("Unable to open hidraw device. Verify device path and run as root.  Error");
        return -1;
    }

    // Send feature report using ioctl
    ioctl_res = ioctl(hidraw_fd, HIDIOCSFEATURE(64), bytes);
    if (ioctl_res < 0) {
        perror("HIDIOCSFEATURE");
        retval = -1;
    } else {
        printf("Fn lock changed\n");
        retval = 0;
    }

    close(hidraw_fd);
    return retval;
}

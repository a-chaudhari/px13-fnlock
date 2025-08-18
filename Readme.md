## Background
This is a tool I wrote to enable or disable the Fn lock on the Asus ProArt PX13 laptop under linux.

Note: I've only tested this on Arch 6.15.  In theory, it should work just as fine on any PX13 regardless of distro.  

## Install
1. make sure your distros `linux-headers` and general development packages are installed ("make", "gcc", etc...)
3. run `make` in the root directory of this repository to build the tool

## Usage
1. syntax is `sudo ./px13-fnlock <hidraw path>  <on|off>` to disable or enable the fn lock
   * example: `sudo ./px13-fnlock /dev/hidraw1 on`
2. verify your hidraw path and update the command.  
   * It's `/dev/hidraw1` for me. yours may vary
   * `udevinfo /dev/hidraw0` will print useful info. try each number
   * look for the string `0B05:19B6.0002` in the DEVPATH line.
2. settings don't save across reboots, so you'll have to run this again after each boot (or use the service file).

## Auto apply on boot
1. copy the binary to /usr/local/bin or somewhere else you prefer
2. copy the service file to /etc/systemd/system
3. edit the service file to point to your binary and hidraw path
4. enable the service with `sudo systemctl enable --now px13-fnlock.service`

## Tech Details
This was discovered by reading the hid feature status from windows after using the OEM driver to enable/disable fn lock. 

The Fn+Esc key combo won't work currently as the hid-asus driver in linux doesn't recognize the emitted scancode and 
drops it (along with a few other ones).  Maybe one day I'll submit a patch or create a hid-bpf
program to work around it.

VID: 0x0B05
PID: 0x19B6
Report ID: 0x5a

## TODO (maybe)
- [x] Add a systemd service to run this on boot
- [ ] Persist the mode across reboots
- [ ] Enumerate hid devices and programatically select hidraw path
- [ ] Submit a patch to hid-asus to support the Fn+Esc key combo (and the other missing ones)
- [ ] Make a userspace utility to listen to the fn+esc key combo and toggle the lock automatically
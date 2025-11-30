install -v -p -m 644 /opt/carroll/xctl10.ko /lib/modules/6.13.5-lexne/kernel/drivers/usb/
insmod xctl10.ko
/sbin/depmod -a 6.13.5-lexne 


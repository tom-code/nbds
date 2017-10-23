# nbds
primitive incomplete network block device server (just enough to work with qemu)

purpose
 - experiment with libuv
 - learn something about nbd and block traffic profile of various filesystms

qemu arguments:
```
system-x86_64 -enable-kvm -m 4G ./system-boot-image --drive file=nbd:127.0.0.1:10809:exportname=abc,format=raw
```

# nbds
primitive incomplete network block device server (just enough to work with qemu)

purpose
 - experiment with libuv
 - learn something about nbd and block traffic profile of various filesystms

qemu arguments:
```
qemu-system-x86_64 -enable-kvm -m 4G ./system-boot-image --drive file=nbd:127.0.0.1:10809:exportname=abc,format=raw
```

use with linux kernel client:
```
nbd-client 127.0.0.1 10809 /dev/nbd0
```

server will by default listen on 127.0.0.1:10809. for every incoming connection it will create new ramdisk with default size ~100MB (see main.cc)



for nbd protocol see: https://sourceforge.net/p/nbd/code/ci/master/tree/doc/proto.md.

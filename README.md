## Synopsis

TBD...

## Motivation

TBD... Platform/infrastructure project prototype.

## Installation

Virtualbox image (dev/dtrace enabled, incremental)
```bash
cd /usr/src/tools/tools/nanobsd

sh nanobsd.sh -n -c sbconf_devdtrace.nano

VBoxManage convertfromraw /usr/obj/nanobsd.GENERIC/_.disk.full ~/freebsd_nano.vdi
```

Virtualbox image (dev/dtrace enabled, clean)
```bash
cd /usr/src/tools/tools/nanobsd

sh nanobsd.sh -c sbconf_devdtrace.nano

VBoxManage convertfromraw /usr/obj/nanobsd.GENERIC/_.disk.full ~/freebsd_nano.vdi
```

Virtualbox image (production, clean)
```bash
cd /usr/src/tools/tools/nanobsd

sh nanobsd.sh -c sbconf.nano

VBoxManage convertfromraw /usr/obj/nanobsd.SB0001/_.disk.full ~/freebsd_nano.vdi
```

Virtualbox will have the following config requirements:
* 2 CPU cores (so Erlang runs in SMP w/o additional flags)
* 640MB RAM (sufficient for dev/test, production running ZFS should have more)
* SATA or SAS disk emulation (legacy IDE disabled in SB kernel, legacy SCSI is slower)
* 2 Network adapters (Intel PRO/1000 MT, bridged to actual device for external NTP access)
* 1 Serial port enabled (for secondary terminal, socat/screen access {https://gist.github.com/snb/284940/11e6354f170be602c9c2f67b59d489ed49ebd143})



## Notes
### Erlang compilation
Requires gmake and perl as compile-time dependencies.
```bash
./configure --prefix=/opt/otp_r18.1 \
  --enable-hipe \
  --enable-kernel-poll \
  --enable-m64-build \
  --enable-sctp \
  --enable-smp-support \
  --enable-threads \
  --enable-vm-probes \
  --with-dynamic-trace=dtrace \
  -- with-ssl \
  --with-termcap
gmake -j5
gmake install
```

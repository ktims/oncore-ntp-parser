oncore-ntp-parser
=================

Motorola Oncore NTPd shared memory parser
-----------------------------------------

This is some C code to parse the oncore.x shmem file generated by ntpd when using the oncore refclock driver. It will tell you if the GPS is locked, how many birds are being tracked etc. I created a couple options to give just a number on the output for use in some mini_snmpd hacks to return GPS status via SNMP. Those I have, unfortunately, lost the source for.

Invoke it thusly:

    oncore [-adtv] <shmem>:
      -a    Show all parameters in a human-readable way
      -d    Show only the dilution of precision value (DOP)
      -t    Show only the number of satellites being tracked
      -v    Show only the number of satellites visible in the sky

Options are binary; selecting multiples will return multiple values in the output, though the order is deterministic, not set by the option order.

Examples:
---------

    root@tick:~# oncore -a /var/lib/ntp/oncore.0
    2013-05-09 07:53:54.00024 pos: 39.224158,-106.002660 vel: 0.000 m/s dop: 0.0 flags: 0x1 [2d] sats: 4/11

    root@tick:~# oncore -d /var/lib/ntp/oncore.0
    0.0

    root@tick:~# oncore -t /var/lib/ntp/oncore.0
    4

    root@tick:~# oncore -v /var/lib/ntp/oncore.0
    11

(note DOP is 0.0 when running in 2D fixed position mode)

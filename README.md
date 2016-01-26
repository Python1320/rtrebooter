
rtrebooter
==========


A sad fix for WiFi dying on Arv7510pw:

```
rt2x00queue_write_tx_frame: Error - Dropping frame due to full tx queue 2
```

Hardcoded only for: https://wiki.openwrt.org/toh/arcadyan/arv7510pw

### TODO
 - Convert to a daemon
 - fix paths
 - Make opkg-package work
 - More generic if there is demand
 - etc...

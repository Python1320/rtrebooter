
rtrebooter
==========


A sad fix for WiFi dying on Arv7510pw:

```
rt2x00queue_write_tx_frame: Error - Dropping frame due to full tx queue 2
```

Hardcoded only for: https://wiki.openwrt.org/toh/arcadyan/arv7510pw

### TODO
 - Convert to a proper daemon
 - fix paths
 - Make opkg-package work
 - More generic if there is demand
 - etc...

#### Example behaviour under heavy load
```
kern.err kernel:  eth0: tx ring full
...
kern.err kernel:  eth0: tx ring full
kern.err kernel:  ieee80211 phy0: rt2x00queue_write_tx_frame: Error - Dropping frame due to full tx queue 2
kern.err kernel:  ieee80211 phy0: rt2x00queue_write_tx_frame: Error - Dropping frame due to full tx queue 2
kern.err kernel:  ieee80211 phy0: rt2x00queue_write_tx_frame: Error - Dropping frame due to full tx queue 2
daemon.notice rtrebooter[5009]: Detected WiFi malfunction, rebooting
kern.info kernel:  device wlan0 left promiscuous mode
kern.info kernel:  br-lan: port 2(wlan0) entered disabled state
kern.warn kernel:  ieee80211 phy0: rt2x00queue_flush_queue: Warning - Queue 2 failed to flush
daemon.notice netifd: Network device 'wlan0' link is down
kern.warn kernel:  ieee80211 phy0: rt2x00queue_flush_queue: Warning - Queue 2 failed to flush
daemon.notice rtrebooter[5009]: Turning WiFi back up
kern.info kernel:  cfg80211: Calling CRDA to update world regulatory domain
kern.info kernel:  cfg80211: World regulatory domain updated:
kern.info kernel:  cfg80211:  DFS Master region: unset
kern.info kernel:  cfg80211:   (start_freq - end_freq @ bandwidth), (max_antenna_gain, max_eirp), (dfs_cac_time)
kern.info kernel:  cfg80211:   (2402000 KHz - 2472000 KHz @ 40000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (2457000 KHz - 2482000 KHz @ 40000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (2474000 KHz - 2494000 KHz @ 20000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (5170000 KHz - 5250000 KHz @ 160000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (5250000 KHz - 5330000 KHz @ 160000 KHz), (N/A, 2000 mBm), (0 s)
kern.info kernel:  cfg80211:   (5490000 KHz - 5730000 KHz @ 160000 KHz), (N/A, 2000 mBm), (0 s)
kern.info kernel:  cfg80211:   (5735000 KHz - 5835000 KHz @ 80000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (57240000 KHz - 63720000 KHz @ 2160000 KHz), (N/A, 0 mBm), (N/A)
kern.info kernel:  ieee80211 phy0: rt2x00_set_rt: Info - RT chipset 2860, rev 0103 detected
kern.info kernel:  ieee80211 phy0: rt2x00_set_rf: Info - RF chipset 0001 detected
kern.debug kernel:  ieee80211 phy0: Selected rate control algorithm 'minstrel_ht'
daemon.notice rtrebooter[5009]: Monitoring again...
kern.info kernel:  cfg80211: Calling CRDA for country: FI
kern.info kernel:  cfg80211: Regulatory domain changed to country: FI
kern.info kernel:  cfg80211:  DFS Master region: ETSI
kern.info kernel:  cfg80211:   (start_freq - end_freq @ bandwidth), (max_antenna_gain, max_eirp), (dfs_cac_time)
kern.info kernel:  cfg80211:   (2402000 KHz - 2482000 KHz @ 40000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (5170000 KHz - 5250000 KHz @ 80000 KHz), (N/A, 2000 mBm), (N/A)
kern.info kernel:  cfg80211:   (5250000 KHz - 5330000 KHz @ 80000 KHz), (N/A, 2000 mBm), (0 s)
kern.info kernel:  cfg80211:   (5490000 KHz - 5710000 KHz @ 80000 KHz), (N/A, 2700 mBm), (0 s)
kern.info kernel:  cfg80211:   (57240000 KHz - 65880000 KHz @ 2160000 KHz), (N/A, 4000 mBm), (N/A)
kern.info kernel:  ieee80211 phy0: rt2x00lib_request_firmware: Info - Loading firmware file 'rt2860.bin'
kern.info kernel:  ieee80211 phy0: rt2x00lib_request_firmware: Info - Firmware detected - version: 0.34
kern.info kernel:  IPv6: ADDRCONF(NETDEV_UP): wlan0: link is not ready
kern.info kernel:  device wlan0 entered promiscuous mode
kern.info kernel:  br-lan: port 2(wlan0) entered forwarding state
kern.info kernel:  br-lan: port 2(wlan0) entered forwarding state
kern.info kernel:  IPv6: ADDRCONF(NETDEV_CHANGE): wlan0: link becomes ready
daemon.notice netifd: Network device 'wlan0' link is up
kern.info kernel:  br-lan: port 2(wlan0) entered forwarding state
daemon.info hostapd: wlan0: STA DE:AD:BE:EF:CA:FE IEEE 802.11: authenticated
daemon.info hostapd: wlan0: STA DE:AD:BE:EF:CA:FE IEEE 802.11: associated (aid 1)
daemon.info hostapd: wlan0: STA DE:AD:BE:EF:CA:FE RADIUS: starting accounting session


```

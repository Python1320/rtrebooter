#!/bin/sh


usage() {
		cat <<EOF
Usage: $0 [0|1|help]
EOF
		exit 1
}
wifi_force_up() {

	insmod cfg80211   	;sleep 0
	insmod mac80211   	;sleep 0
	insmod rt2x00lib  	;sleep 0
	insmod rt2x00mmio 	;sleep 0
	insmod rt2800lib  	;sleep 0
	insmod rt2800mmio 	;sleep 0
	insmod rt2x00pci  	;sleep 0
	insmod rt2800pci  	;sleep 0

	uci set wireless.radio0.disabled=0
	
	#sleep 0; wifi reload; wifi detect; sleep 0
	
	wifi
}


wifi_force_down() {
	
	
	
	while wifi status | grep -E pending.*true > /dev/null; do sleep 1; done
	

	/sbin/uci set wireless.radio0.disabled=1
	
	/sbin/wifi down > /dev/null
	
	while wifi status | grep -E pending.*true > /dev/null; do sleep 1; done
	
	rmmod rt2800pci  	;sleep 0
	rmmod rt2x00pci  	;sleep 0
	rmmod rt2800mmio 	;sleep 0
	rmmod rt2800lib  	;sleep 0
	rmmod rt2x00mmio 	;sleep 0
	rmmod rt2x00lib  	;sleep 0
	rmmod mac80211   	;sleep 0
	rmmod cfg80211   	;sleep 0
	
	/usr/bin/killall hostapd 2>/dev/null >/dev/null
	
	sleep 1
}


case "$1" in
		1) wifi_force_up;;
		0) wifi_force_down;;
		*) usage;;
esac


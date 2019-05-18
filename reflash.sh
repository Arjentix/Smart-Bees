#! /bin/bash

timeout=7

rm photon_firmware_*.bin
# particle compile photon

echo "Make photon enter in DFU mode for $(timeout) seconds!"
for (( i=1; i <= timeout; i++ ))
do
	echo "$i"
	sleep 1
done

particle flash --usb photon_firmware_*.bin

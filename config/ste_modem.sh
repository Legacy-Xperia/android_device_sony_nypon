#!/system/bin/sh

# Start CSPSA2NWM which uses CSPSA server to copy radio parameters
# from CSPSA and store them as files in the file system in the way expected
# by NWM. Note that this process does not background itself, but rather
# blocks until the copying has completed.

# Not needed
# echo "Synchronizing modem parameters from CSPSA to file system"
# /system/bin/logwrapper /system/bin/cspsa2nwm -r / -u 0xfffffffd:0 -g /modemfs/guardfile -p /modemfs/RFHAL CSPSA0

# Finally start Modem Storage Agent, serving files system requests from the
# modem. It is important that this process does not background itself since
# Androids init will restart this script once this command returns.
# Use exec to replace the shell as it serves no further purpose.
echo "Starting Modem Storage Agent"
exec /system/bin/logwrapper /system/bin/msa -l stdout

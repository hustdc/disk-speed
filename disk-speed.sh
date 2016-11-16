#!/bin/sh
#
# Read and write tests via hdparm and dd.
#

current_device=$(df -h . | tail -1 | cut -d ' ' -f 1)
test_description='(hdparm) Cached Read,(hdparm) Read,Write,Read,Cached Read'

# hdparm
test_with_hdparm() {
  hdparm -Tt $current_device
}

read_tempfile() {
  dd if=tempfile of=/dev/null bs=1M count=1024
}

test_with_dd() {
  # Write
  echo "Write"
  dd if=/dev/zero of=tempfile bs=1M count=1024 conv=fdatasync,notrunc

  # Read Without Cache
  echo "Read w/o Cache"
  sudo echo 3 > /proc/sys/vm/drop_caches
  read_tempfile

  # Read With Cache
  echo "Read w/ Cache"
  read_tempfile
}

# Run tests

for i in $(seq 6); do
  test_with_hdparm
  test_with_dd
done 2>&1 |
grep -E -o '[0-9.]+ MB/s' |
grep -E -o '[0-9.]+'

# cleanup

rm tempfile

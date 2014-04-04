#!/bin/bash

dd if=/dev/urandom of=1KiB bs=1024 count=1
dd if=/dev/urandom of=1MiB bs=1024 count=1024
dd if=/dev/urandom of=1GiB bs=65536 count=16384

#!/bin/bash
#courtesy of tjadam, enjoy!

for blocksize in 128 256 512 1024 2048 4096 65536
do
    echo ----------------------------------- Blocksize $blocksize ----------- 
    for file in 1KiB 1MiB 1GiB
    do
        echo ------------ file $file ----------------------------------------
        echo testing copy $file with blocksize $blocksize
        time ./copy $file /dev/null $blocksize
        echo $'\n'testing dd $file with blocksize $blocksize
        time dd if=$file of=/dev/null bs=$blocksize
    done
done

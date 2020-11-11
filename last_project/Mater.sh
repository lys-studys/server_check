#!/bin/bash
gcc -D _DEBUG Mater.c ./common/common.c ./common/linklist.c -I ./common/ -o Mater -l pthread


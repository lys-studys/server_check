#!/bin/bash
gcc -D _DEBUG client.c ./common/common.c ./common/linklist.c -I ./common/ -o client -l pthread


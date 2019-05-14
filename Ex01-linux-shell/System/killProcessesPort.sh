#!/bin/bash
netstat -lnpt | grep $1 | awk '{print $7}' | awk -F/ '{print $1}' | xargs kill -9

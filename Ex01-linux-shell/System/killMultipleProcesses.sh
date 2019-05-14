#!/bin/bash
ps -a | grep -i $1 | awk '{print $1}' | xargs kill -9

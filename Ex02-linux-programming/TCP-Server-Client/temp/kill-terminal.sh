#!/bin/bash
ps -a | awk '/bash/ { print $1 }' | xargs sudo kill
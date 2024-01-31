#!/usr/bin/env bash

export START=$(date +%s%3N);
while true; do ./main --color --badapple; sleep 0.016667; done;
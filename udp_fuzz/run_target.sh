#!/bin/bash

TARGET_IP="127.0.0.1"
TARGET_PORT="12345"
AFL_INPUT_FILE="$1"

# AFL++가 생성한 입력 파일을 UDP 패킷으로 변환하여 타겟 프로그램에 전달
cat "$AFL_INPUT_FILE" | nc -u $TARGET_IP $TARGET_PORT

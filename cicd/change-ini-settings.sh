#!/bin/bash

SETTINGS_INI_PATH=$1
VARIABLE_CATEGORY=$2
VARIABLE_NAME=$3
VARIABLE_VALUE=$4

echo "\n[$VARIABLE_CATEGORY]" >> $SETTINGS_INI_PATH
echo "$VARIABLE_NAME=$VARIABLE_VALUE" >> $SETTINGS_INI_PATH
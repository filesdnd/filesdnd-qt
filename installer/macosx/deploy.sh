#!/bin/bash

macdeploycmd="macdeployqt"
binary="Files Drag and Drop.app"
qmdestination="$binary/Contents/MacOS/"
qmfiles="../../linguist/*.qm"

hash $macdeploycmd 2>/dev/null || { echo "Error, $macdeploycmd not found. Is Qt in your path ?"; exit 1; }

$macdeploycmd "$binary"

cp -v $qmfiles "$qmdestination"

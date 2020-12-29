#!/bin/sh
cd s2client-proto
protoc --c_out=../proto  s2clientprotocol/*.proto

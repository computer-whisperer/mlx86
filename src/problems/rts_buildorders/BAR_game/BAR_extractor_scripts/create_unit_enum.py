#!/bin/python3
import json


with open('../Beyond-All-Reason/language/en/units.json') as f:
    data = json.load(f)

print("enum BAR_UnitType {")

for unit in data["units"]["names"]:
    print("BAR_UnitType_" + unit + ",")

print("BAR_UnitType_MAX\n};")
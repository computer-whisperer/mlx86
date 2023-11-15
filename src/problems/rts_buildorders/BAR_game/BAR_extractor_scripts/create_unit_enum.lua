local json = require("json")
local file = io.open("../Beyond-All-Reason/language/en/units.json", "r")
local content = file:read("*all")
file:close()

local desiredUnits = dofile("unitPaths.lua")

local language = json.decode(content)

print("enum BAR_UnitType {")

for unit, _ in pairs(language["units"]["names"]) do
    if desiredUnits[unit] then
        print("BAR_UnitType_" unit .. ",")
    end
end

print("BAR_UnitType_MAX\n};")
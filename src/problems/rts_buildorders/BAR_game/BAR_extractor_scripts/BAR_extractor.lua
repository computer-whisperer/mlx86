local units = dofile("unitPaths.lua")

local json = require("json")
local file = io.open("../Beyond-All-Reason/language/en/units.json", "r")
local content = file:read("*all")
file:close()

local jsonData = json.decode(content)["units"]
local unitNames = jsonData["names"]
local factions = jsonData["factions"]

print("constexpr inline static struct BARUnitTypeMetadata_T bar_game_get_unit_type_metadata(const BAR_UnitType unit_type) {\
    switch (unit_type) {")

for _, unit in ipairs(units) do
    local data = dofile(unit[2])[unit[1]]
    print("case " .. unit[1] .. ":")
    print("  return {")

    local faction = ""
    local namePrefix = string.sub(unit[1], 1, 3)
    
    if factions[namePrefix] then
        faction = factions[namePrefix]
    end

    if unitNames[unit[1]] then
        print("    .name = \"" .. faction .. " " .. unitNames[unit[1]] .. "\",")
    end
    if data["buildcostmetal"] then
        print("    .metal_cost = " .. data["buildcostmetal"] .. ",")
    end
    if data["buildcostenergy"] then
        print("    .energy_cost = " .. data["buildcostenergy"] .. ",")
    end
    if data["buildtime"] then
        print("    .build_cost = " .. data["buildtime"] .. ",")
    end
    if data["workertime"] then
        print("    .build_power = " .. data["workertime"] .. ",")
    end
    if data["metalmake"] then
        print("    .metal_production = " .. data["metalmake"] .. ",")
    end
    if data["energymake"] then
        print("    .energy_production = " .. data["energymake"] .. ",")
    end
    if data["energystorage"] then
        print("    .energy_capacity = " .. data["energystorage"] .. ",")
    end
    if data["metalstorage"] then
        print("    .metal_capacity = " .. data["metalstorage"] .. ",")
    end

    if data["buildoptions"] then
        print("    .build_options = {")
        for _, option in ipairs(data["buildoptions"]) do
            print("      \"" .. option .. "\",")
        end
        print("    },")
    end

    print("  };")
end
print("}\n}")
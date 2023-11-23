local units = dofile("unitPaths.lua")

VFS = {}
VFS.Include = function(path)
    return dofile("../Beyond-All-Reason/" .. path)
end

local json = require("json")
local file = io.open("../Beyond-All-Reason/language/en/units.json", "r")
local content = file:read("*all")
file:close()

local jsonData = json.decode(content)["units"]
local unitNames = jsonData["names"]
local factions = jsonData["factions"]

print("template <const BAR_UnitType unit_type> constexpr inline static struct BARUnitTypeMetadata_T bar_game_get_unit_type_metadata_generated() {\
    switch (unit_type) {\
        default:\
          return {.name = \"Invalid\"};")

for _, unit in ipairs(units) do
    local data = dofile(unit[2])[unit[1]]
    print("case BAR_UnitType_" .. unit[1] .. ":")
    print("  return {")

    local faction = ""
    local namePrefix = string.sub(unit[1], 1, 3)
    
    if factions[namePrefix] then
        faction = factions[namePrefix]
    end

    if unitNames[unit[1]] then
        print("    .name = \"" .. unitNames[unit[1]] .. "\",")
    end
    if data["buildcostmetal"] then
        print("    .metal_cost = (uint32_t)(" .. data["buildcostmetal"] .. " * bar_game_resource_denominator),")
    end
    if data["buildcostenergy"] then
        print("    .energy_cost = (uint32_t)(" .. data["buildcostenergy"] .. " * bar_game_resource_denominator),")
    end
    if data["buildtime"] then
        print("    .build_cost = (uint32_t)(" .. data["buildtime"] .. "),")
    end
    if data["workertime"] then
        print("    .build_power = (uint32_t)(" .. data["workertime"] .. "/ bar_game_tps),")
    end
    if data["metalmake"] then
        print("    .metal_production = (uint32_t)(" .. data["metalmake"] .. " * bar_game_resource_denominator / bar_game_tps),")
    end
    if data["energymake"] then
        print("    .energy_production = (uint32_t)(" .. data["energymake"] .. " * bar_game_resource_denominator / bar_game_tps),")
    end
    if data["energystorage"] then
        print("    .energy_capacity = (uint32_t)(" .. data["energystorage"] .. " * bar_game_resource_denominator),")
    end

    if data["metalstorage"] then
        print("    .metal_capacity = (uint32_t)(" .. data["metalstorage"] .. " * bar_game_resource_denominator),")
    end

    if data["energyuse"] then
        print("    .energy_upkeep = (uint32_t)(" .. data["energyuse"] .. " * bar_game_resource_denominator/bar_game_tps),")
    end

    if data["metaluse"] then
        print("    .metal_upkeep = (uint32_t)(" .. data["metaluse"] .. " * bar_game_resource_denominator/bar_game_tps),")
    end

    if data["buildoptions"] then
        print("    .build_options = {")
        for _, option in pairs(data["buildoptions"]) do
            print("      BAR_UnitType_" .. option .. ",")
        end
        print("    },")
    end

    if data["customparams"] and data["customparams"]["energyconv_capacity"] then
      print("    .energy_convert_capacity = (uint32_t)(".. data["customparams"]["energyconv_capacity"] .. "* bar_game_resource_denominator),")
      print("    .energy_convert_return = (uint32_t)(".. data["customparams"]["energyconv_capacity"] .. "*" .. data["customparams"]["energyconv_efficiency"] .."* bar_game_resource_denominator),")
    end

    if data["customparams"] and data["customparams"]["geothermal"] then
      print("    .geothermal = ".. data["customparams"]["geothermal"] ..",")
    end

    if data["customparams"] and data["customparams"]["metal_extractor"] then
      print("    .metal_extractor = ".. data["customparams"]["metal_extractor"] ..",")
    end

    if data["tidalgenerator"] ~= nil then
        print("    .tidal_generator = ".. data["tidalgenerator"] .. ",")
    end

    if data["windgenerator"] ~= nil then
        print("    .wind_generator = ".. data["windgenerator"] .. ",")
    end

    if data["builder"] ~= nil then
        print("    .is_builder = " .. (data["builder"] and 1 or 0) .. ",")
        if data["canassist"] ~= nil then
            print("    .can_assist = " .. (data["canassist"] and 1 or 0) .. ",")
        else
            print("    .can_assist = 1,")
        end
        if data["builddistance"] ~= nil then
            print("    .build_distance = ".. data["builddistance"].. ",")
        end
    else
        if data["canassist"] ~= nil then
            print("    .can_assist = " .. (data["canassist"] and 1 or 0) .. ",")
        end
    end

    if data["maxvelocity"] ~= nil then
        print("    .max_velocity = ".. data["maxvelocity"] .. ",")
    end

    if data["minwaterdepth"] ~= nil then
        print("    .min_water_depth = ".. data["minwaterdepth"] .. ",")
    end

    if data["maxwaterdepth"] ~= nil then
        print("    .max_water_depth = ".. data["maxwaterdepth"] .. ",")
    end

    if data["footprintx"] ~= nil then
        print("    .footprint_x = ".. data["footprintx"] .. ",")
    end

    if data["footprintz"] ~= nil then
        print("    .footprint_z = ".. data["footprintz"] .. ",")
    end

    print("  };")
end
print("}\n}")
units=$(cat desired_units.txt)

echo "return {" > unitPaths.lua

for tool in $units; do
  path=$(find .. -name $tool.lua 2>/dev/null | head -n 1)
  echo "{\"$tool\", \"$path\"}," >> unitPaths.lua
done

echo "}" >> unitPaths.lua


echo "enum BAR_UnitType {"
echo BAR_UnitType_None,
for tool in $units; do
  echo "BAR_UnitType_$tool,"
done

echo BAR_UnitType_MAX
echo "};"

echo "#include \"bar_game_unit_type_metadata_type.hpp\""

lua BAR_extractor.lua
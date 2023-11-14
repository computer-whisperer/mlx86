units=$(cat desired_units.txt)

echo "return {" > unitPaths.lua

for tool in $units; do
  path=$(find .. -name $tool.lua 2>/dev/null)
  echo "{\"$tool\", \"$path\"}," >> unitPaths.lua
done

echo "}" >> unitPaths.lua

echo "enum BAR_UnitType {"

for tool in $units; do
  echo "$tool,"
done

echo   BAR_UnitType_MAX
echo "};"

lua BAR_extractor.lua
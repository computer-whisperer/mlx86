import subprocess
import tempfile
from pathlib import Path

import lupa
import pyunpack
import cv2
import os


spring_map_conv_ng_path = "../../../../../SpringMapConvNG/mapdecompile"

def main():
    #map_file = "~/.local/state/Beyond All Reason/maps/altair_crossing_v4.1.sd7"
    #map_file = "~/.local/state/Beyond All Reason/maps/BYAR-maps/all_that_glitters_v2.1.sd7"
    #map_file = "~/.local/state/Beyond All Reason/maps/BYAR-maps/center_command_bar_v1.0.sd7"
    #map_file = "~/.local/state/Beyond All Reason/maps/BYAR-maps/serene_caldera_v1.3.sd7"
    map_file = "~/.local/state/Beyond All Reason/maps/BYAR-maps/scylla_and_charybdis_1.0.sd7"
    with tempfile.TemporaryDirectory() as map_extract_dir:
        pyunpack.Archive(map_file).extractall(map_extract_dir)
        mapinfo_lua = ""
        with open(os.path.join(map_extract_dir, "mapinfo.lua"), "r") as main_file:
            mapinfo_lua = main_file.read()
        lua = lupa.LuaRuntime(unpack_returned_tuples=True)
        lua.execute("function getfenv() return {} end")
        lua.execute("VFS = {}")
        lua.execute("VFS.DirList = function() return {} end")
        mapinfo_data = lua.execute(mapinfo_lua)
        inner_map_fname = os.path.join(map_extract_dir, mapinfo_data["mapfile"])
        inner_map_dir = os.path.dirname(inner_map_fname)
        s_data = [
            os.path.abspath(spring_map_conv_ng_path),
            "-directory",
            inner_map_dir,
            "-mapfile",
            os.path.basename(inner_map_fname)]
        subprocess.run(s_data)

        main_texture = cv2.imread(os.path.join(inner_map_dir, "texture.png"))
        cv2.imwrite("map_texture.png", main_texture)

        xsize = main_texture.shape[1]
        ysize = main_texture.shape[0]

        max_metal = mapinfo_data["maxmetal"]

        mine_positions = read_mine_positions(os.path.join(inner_map_dir, "metalmap.png"), max_metal, xsize, ysize)
        geo_vent_positions = parse_features(os.path.join(inner_map_dir, "features.txt"))

        out_file = open("bar_game_map_data.hpp", "w")
        out_file.write("#pragma once\n")
        out_file.write("#include \"bar_game_types.hpp\"\n")
        out_file.write("\n")
        out_file.write("constexpr const char* bar_game_map_name = \"{}\";\n".format(mapinfo_data["name"]))
        out_file.write("constexpr float bar_game_map_width  = {};\n".format(xsize))
        out_file.write("constexpr float bar_game_map_height  = {};\n".format(ysize))
        out_file.write("constexpr float bar_game_map_max_wind_speed = {};\n".format(mapinfo_data["atmosphere"]["maxwind"]))
        out_file.write("constexpr float bar_game_map_min_wind_speed = {};\n".format(mapinfo_data["atmosphere"]["minwind"]))
        out_file.write("constexpr float bar_game_map_tidal_strength = {};\n".format(mapinfo_data["tidalstrength"]))
        out_file.write("constexpr uint32_t bar_game_map_num_metal_positions = {};\n".format(len(mine_positions)))
        out_file.write("constexpr uint32_t bar_game_map_num_geothermal_positions = {};\n".format(len(geo_vent_positions)))
        out_file.write("constexpr uint32_t bar_game_map_num_starting_positions = {};\n".format(len(mapinfo_data["teams"])))
        out_file.write("constexpr BARMetalMineMetadata bar_game_map_metal_positions[] = {\n")

        for mine in mine_positions:
            out_file.write("{{ {}, {}, {} }},\n".format(mine[0], mine[1], mine[2]))

        out_file.write("};\n")

        out_file.write("constexpr BARCoordinate bar_game_map_geo_vent_positions[] = {\n")
        for geo_vent in geo_vent_positions:
            out_file.write("{{ {}, {}}},\n".format(geo_vent[0], geo_vent[2]))
        out_file.write("};\n")

        out_file.write("constexpr BARCoordinate bar_game_map_start_positions[] = {\n")
        for team in mapinfo_data["teams"]:
            out_file.write("{{ {}, {} }},\n".format(mapinfo_data["teams"][team]["startpos"]["x"], mapinfo_data["teams"][team]["startpos"]["z"]))
        out_file.write("};\n")

        out_file.close()


def read_mine_positions(file_path, max_metal, map_width, map_height):
    mine_positions = []
    src = cv2.imread(file_path)
    gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
    #blur = cv2.blur(gray, (3, 3))
    ret, thresh = cv2.threshold(gray, 1, 256, cv2.THRESH_BINARY)

    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    xsize = src.shape[1]
    ysize = src.shape[0]

    x_scale = map_width/xsize
    y_scale = map_height/ysize

    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        center_x = (x + w/2)
        center_y = (y + h/2)

        total = 0
        for x_sample in range(x, x+w):
            for y_sample in range(y, y+h):
                total += gray[y_sample, x_sample]
        total = round(total*max_metal)
        mine_positions.append((round(center_x*x_scale), round(center_y*y_scale), total))
    return mine_positions


def parse_features(file_path):
    geo_vent_positions = []
    with open(file_path, "r") as feature_file:
        for line in feature_file:
            data = line.rstrip().split(" ")
            if data[0] == "geovent":
                geo_vent_positions.append((round(float(data[1])), round(float(data[2])), round(float(data[3])), round(float(data[4]))))
    return geo_vent_positions



if "__main__" == __name__:
    main()

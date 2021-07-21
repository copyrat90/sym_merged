import os
import csv
from glob import glob
import argparse
import xml.etree.ElementTree as ET
from pathlib import Path
from datetime import datetime
from io import StringIO


def check_tilemap_entity_layer_match(root):
    return {layer.attrib['name'] for layer in root.findall("*[@name='Zone Tilemap']/layer")} == {layer.attrib['name'] for layer in root.findall("*[@name='DISABLE ON EXPORT AS IMAGE']/*[@name='Entity']/objectgroup")}


def get_zone_boundary(zone_index, root):
    tilemap_xpath_str = f"*[@name='Zone Tilemap']/*[@name='{zone_index}']"
    tilemap_csv_str = root.find(tilemap_xpath_str)[
        0].text.strip().replace(',\n', '\n')
    tilemap_csv_str_f = StringIO(tilemap_csv_str)
    reader = csv.reader(tilemap_csv_str_f, delimiter=',')
    tilemap = [row for row in reader]
    top_left = None
    bottom_right = None
    for y in range(int(root.attrib['height'])):
        for x in range(int(root.attrib['width'])):
            if tilemap[y][x] != '0':
                top_left = (x, y)
                break
        if top_left:
            break
    for y in range(int(root.attrib['height'])-1, -1, -1):
        for x in range(int(root.attrib['width'])-1, -1, -1):
            if tilemap[y][x] != '0':
                bottom_right = (x, y)
                break
        if bottom_right:
            break
    return top_left, bottom_right


def get_stage_id(tmx_path):
    basename = os.path.basename(tmx_path)
    l = basename.split('_')
    if l[0] == 'bg':
        return f"{l[1].upper()}_{l[2].upper()}"
    else:
        filename = os.path.splitext(basename)[0]
        l = filename.split('_')
        return f"{l[2].upper()}_{l[3].upper()}"


def write_cpp_ending(current_stage_id, current_stage_names, current_stage_is_player_gravity_reversed_by_default, bgm_name, cpp_file, black_hole_idx, zone_infos, current_stage_tmx_files_wo_extension, root, map_width_cpp_text, map_height_cpp_text, black_holes):
    if black_hole_idx == -1:
        print("[WARN] No black hole in "+current_stage_id)
    else:
        x = round(float(black_holes[0].attrib['x']))
        y = round(float(black_holes[0].attrib['y']))
        cpp_file.write("constexpr ZoneInfo::BlackHoleInfo blackHole = {"+'\n')
        cpp_file.write(
            f"{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}},"+'\n')
        cpp_file.write("};"+'\n')

    cpp_file.write("constexpr ZoneInfo zoneInfos_[] = {"+'\n')
    zone_idx = 0
    while zone_idx < len(zone_infos):
        cpp_file.write(
            f"ZoneInfo(bn::affine_bg_items::{zone_infos[zone_idx][0]}, Z{zone_idx}_symbolInfos_, Z{zone_idx}_doorInfos_, Z{zone_idx}_shutterInfos_, Z{zone_idx}_hoverButtonInfos_, Z{zone_idx}_pressureButtonInfos_,"+'\n')
        cpp_file.write(
            f'helper::tilemap::IndexRect({{{zone_infos[zone_idx][1]}, {zone_infos[zone_idx][2]}}}, bn::size{{{zone_infos[zone_idx][3]}, {zone_infos[zone_idx][4]}}}, {zone_infos[zone_idx][0]}.map_item().dimensions()), Z{zone_idx}_signInfos_, Z{zone_idx}_exitInfos_, Z{zone_idx}_entranceInfos_{", blackHole" if zone_idx == black_hole_idx else ""}),'+'\n')
        zone_idx += 1
    cpp_file.write("};"+'\n')
    cpp_file.write('\n')
    cpp_file.write(
        f'constexpr StageInfo {current_stage_id.lower()}_(stage::Id::{current_stage_id}, "{current_stage_names[0]}", "{current_stage_names[1]}", "{current_stage_names[2]}", "{current_stage_names[3]}", {"true" if current_stage_is_player_gravity_reversed_by_default else "false"}, bn::music_items::{bgm_name}, zoneInfos_);'+'\n')
    cpp_file.write('\n')
    cpp_file.write("} // namespace"+'\n')
    cpp_file.write('\n')
    cpp_file.write(
        f"const StageInfo& Get_{current_stage_id}()"+'\n')
    cpp_file.write("{"+'\n')
    cpp_file.write(f"    return {current_stage_id.lower()}_;"+'\n')
    cpp_file.write("}"+'\n')
    cpp_file.write('\n')
    cpp_file.write("} // namespace sym::game::stage"+'\n')


def process(build_path):
    source_path = os.path.join(build_path, 'src')
    include_path = os.path.join(build_path, 'include')
    if not os.path.exists(source_path):
        os.makedirs(source_path)
    if not os.path.exists(include_path):
        os.makedirs(include_path)
    stage_ids_to_rebuild = []
    stage_tmx_m_times = {}
    for tmx_path in glob('graphics_source/bg_w*.tmx'):
        stage_id = get_stage_id(tmx_path)
        if stage_id not in stage_tmx_m_times or stage_tmx_m_times[stage_id] < os.path.getmtime(tmx_path):
            stage_tmx_m_times[stage_id] = os.path.getmtime(tmx_path)
    header_rebuild_required = False
    for stage_id, m_time in stage_tmx_m_times.items():
        cpp_path = os.path.join(source_path, f'game_stage_{stage_id}.cpp')
        if not os.path.exists(cpp_path):
            header_rebuild_required = True
        if not os.path.exists(cpp_path) or os.path.getmtime(cpp_path) < m_time:
            stage_ids_to_rebuild.append(stage_id)
    tmx_files_to_rebuild = []
    for tmx_path in glob('graphics_source/bg_w*.tmx'):
        if get_stage_id(tmx_path) in stage_ids_to_rebuild:
            tmx_files_to_rebuild.append(tmx_path)
    if not tmx_files_to_rebuild:
        return True

    if header_rebuild_required:
        header_file = open(os.path.join(
            include_path, 'game_stage_getter.h'), encoding='utf-8', mode='w')
        header_file.write(
            "// Generated by `stage-codegen.py` written by copyrat90"+'\n')
        header_file.write(f"// Last generated on {str(datetime.now())}"+'\n')
        header_file.write('\n')
        header_file.write('#pragma once' + '\n')
        header_file.write('\n')
        header_file.write('#include "game_stage_StageInfo.h"'+'\n')
        header_file.write('\n')
        header_file.write('namespace sym::game::stage'+'\n')
        header_file.write('{'+'\n')
        for stage_id in stage_tmx_m_times:
            header_file.write(f"const StageInfo& Get_{stage_id}();"+'\n')
        header_file.write("} // namespace sym::game::stage"+'\n')
        header_file.close()

    current_stage_id = ""
    # (stageNameEng, stageSubNameEng, stageNameKor, stageSubNameKor)
    current_stage_names = ()
    current_stage_is_player_gravity_reversed_by_default = False
    bgm_name = ""
    cpp_file = None
    black_hole_idx = -1
    black_holes = []
    # idx: ("bg_wx_sy_n",x,y,w,h)
    zone_infos = {}
    current_stage_tmx_files_wo_extension = []
    map_width = -1
    map_height = -1
    map_width_cpp_text = ''
    map_height_cpp_text = ''

    for tmx_path in tmx_files_to_rebuild:
        tree = ET.parse(tmx_path)
        root = tree.getroot()
        current_tmx_filename_wo_extension = os.path.splitext(
            os.path.basename(tmx_path))[0]
        if not check_tilemap_entity_layer_match(root):
            raise Exception(
                tmx_path + ": Zone Tilemap and Entity layer numbers mismatch")
        if current_stage_id != get_stage_id(tmx_path):
            if cpp_file:
                write_cpp_ending(current_stage_id, current_stage_names, current_stage_is_player_gravity_reversed_by_default,
                                 bgm_name, cpp_file, black_hole_idx, zone_infos, current_stage_tmx_files_wo_extension, root, map_width_cpp_text, map_height_cpp_text, black_holes)

            current_stage_id = get_stage_id(tmx_path)
            current_stage_names = (root.find(f"*[@name='Zone Tilemap']/properties/*[@name='stageNameEng']").attrib['value'], root.find(f"*[@name='Zone Tilemap']/properties/*[@name='stageSubNameEng']").attrib['value'], root.find(
                f"*[@name='Zone Tilemap']/properties/*[@name='stageNameKor']").attrib['value'], root.find(f"*[@name='Zone Tilemap']/properties/*[@name='stageSubNameKor']").attrib['value'])
            current_stage_is_player_gravity_reversed_by_default = root.find(
                f"*[@name='Zone Tilemap']/properties/*[@name='isPlayerGravityReversedByDefault']").attrib['value'] == 'true'
            bgm_name = root.find(
                f"*[@name='Zone Tilemap']/properties/*[@name='backgroundMusic']").attrib['value']
            cpp_file = open(os.path.join(
                source_path, f'game_stage_{current_stage_id}.cpp'), encoding='utf-8', mode='w')
            black_hole_idx = -1
            black_holes = []
            zone_infos = {}
            current_stage_tmx_files_wo_extension = [os.path.splitext(os.path.basename(basename))[
                0] for basename in glob(os.path.join('graphics_source', f'bg_{current_stage_id.lower()}*.tmx'))]
            map_width = int(root.attrib['width'])
            map_height = int(root.attrib['height'])
            map_width_cpp_text = f"{current_tmx_filename_wo_extension}.map_item().dimensions().width() * 8"
            map_height_cpp_text = f"{current_tmx_filename_wo_extension}.map_item().dimensions().height() * 8"

            cpp_file.write(f'// game_stage_{current_stage_id}.cpp'+'\n')
            cpp_file.write(
                "// Generated by `stage-codegen.py` written by copyrat90"+'\n')
            cpp_file.write(f"// Last generated on {str(datetime.now())}"+'\n')
            cpp_file.write('\n')
            cpp_file.write("#include <bn_affine_bg_item.h>"+'\n')
            cpp_file.write("#include <bn_array.h>"+'\n')
            cpp_file.write("#include <bn_fixed_rect.h>"+'\n')
            cpp_file.write('#include <bn_string_view.h>'+'\n')
            cpp_file.write('\n')
            for bg_name in current_stage_tmx_files_wo_extension:
                cpp_file.write(
                    f'#include <bn_affine_bg_items_{bg_name}.h>'+'\n')
            cpp_file.write('#include <bn_music_items.h>'+'\n')
            cpp_file.write('#include "game_stage_StageInfo.h"'+'\n')
            cpp_file.write('\n')
            cpp_file.write('namespace sym::game::stage'+'\n')
            cpp_file.write('{'+'\n')
            cpp_file.write('\n')
            cpp_file.write('namespace'+'\n')
            cpp_file.write('{'+'\n')
            cpp_file.write('\n')
            cpp_file.write('using namespace bn::affine_bg_items;'+'\n')
            cpp_file.write('\n')

        for tilemap in root.findall(f"*[@name='Zone Tilemap']/layer"):
            zone_idx = int(tilemap.attrib['name'])
            if zone_idx in zone_infos:
                raise Exception(f"Zone {zone_idx} duplicated in {tmx_path}")
            top_left, bottom_right = get_zone_boundary(zone_idx, root)
            w = bottom_right[0] - top_left[0] + 1
            h = bottom_right[1] - top_left[1] + 1
            zone_infos[zone_idx] = (
                current_tmx_filename_wo_extension, top_left[0], top_left[1], w, h)

        for entity_layer in root.findall("*[@name='DISABLE ON EXPORT AS IMAGE']/*[@name='Entity']/objectgroup"):
            zone_idx = int(entity_layer.attrib['name'])
            # write symbols
            symbols = entity_layer.findall("*[@type='Symbol']")
            if symbols:
                cpp_file.write(
                    f"constexpr ZoneInfo::SymbolInfo Z{zone_idx}_symbolInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::SymbolInfo> Z{zone_idx}_symbolInfos_ = {{"+'\n')
            for symbol in symbols:
                x = round(float(symbol.attrib['x']))
                y = round(float(symbol.attrib['y']))
                symbol_type = symbol.find(
                    'properties/*[@name="symbolType"]').attrib['value']
                symbol_reversed_text = 'false'
                if symbol.find('properties/*[@name="isGravityReversedByDefault"]') != None:
                    symbol_reversed_text = 'true'
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}, entity::Symbol::Type::{symbol_type.upper()}, {symbol_reversed_text}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write doors
            doors = entity_layer.findall("*[@type='Door']")
            if doors:
                cpp_file.write(
                    f"constexpr ZoneInfo::DoorInfo Z{zone_idx}_doorInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::DoorInfo> Z{zone_idx}_doorInfos_ = {{"+'\n')
            for door in doors:
                x = round(float(door.attrib['x']))
                y = round(float(door.attrib['y']))
                text_sprite_number = door.find(
                    'properties/*[@name="textSpriteNumber"]').attrib['value']
                is_opened_text = 'false'
                if door.find('properties/*[@name="isOpenedByDefault"]') != None:
                    is_opened_text = 'true'
                destination_zone_idx = door.find(
                    'properties/*[@name="destinationZoneIndex"]').attrib['value']
                entrance_idx = door.find(
                    'properties/*[@name="entranceOfZoneIndex"]').attrib['value']
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}, {is_opened_text}, {destination_zone_idx}, {entrance_idx}, {text_sprite_number}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write shutters
            shutters = entity_layer.findall("*[@type='Shutter']")
            if shutters:
                cpp_file.write(
                    f"constexpr ZoneInfo::ShutterInfo Z{zone_idx}_shutterInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::ShutterInfo> Z{zone_idx}_shutterInfos_ = {{"+'\n')
            for shutter in shutters:
                x = round(float(shutter.attrib['x']))
                y = round(float(shutter.attrib['y']))
                text_sprite_number = shutter.find(
                    'properties/*[@name="textSpriteNumber"]').attrib['value']
                is_opened_text = 'false'
                if shutter.find('properties/*[@name="isOpenedByDefault"]') != None:
                    is_opened_text = 'true'
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}, {is_opened_text}, {text_sprite_number}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write hover buttons
            buttons = entity_layer.findall("*[@type='HoverButton']")
            if buttons:
                cpp_file.write(
                    f"constexpr ZoneInfo::ButtonInfo Z{zone_idx}_hoverButtonInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::ButtonInfo> Z{zone_idx}_hoverButtonInfos_ = {{"+'\n')
            for button in buttons:
                x = round(float(button.attrib['x']))
                y = round(float(button.attrib['y']))
                text_sprite_number = button.find(
                    'properties/*[@name="textSpriteNumber"]').attrib['value']
                required_symbol_type = None
                if button.find('properties/*[@name="requiredSymbolType"]') != None:
                    required_symbol_type = button.find(
                        'properties/*[@name="requiredSymbolType"]').attrib['value']
                vertically_reversed_text = 'false'
                if button.find('properties/*[@name="isVerticallyReversed"]') != None:
                    vertically_reversed_text = 'true'
                is_on_text = 'false'
                if button.find('properties/*[@name="isOnByDefault"]') != None:
                    is_on_text = 'true'
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}, {is_on_text}, {text_sprite_number}, {vertically_reversed_text}, {f'entity::Symbol::Type::{required_symbol_type}' if required_symbol_type else 'bn::nullopt'}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write pressure buttons
            buttons = entity_layer.findall("*[@type='PressureButton']")
            if buttons:
                cpp_file.write(
                    f"constexpr ZoneInfo::ButtonInfo Z{zone_idx}_pressureButtonInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::ButtonInfo> Z{zone_idx}_pressureButtonInfos_ = {{"+'\n')
            for button in buttons:
                x = round(float(button.attrib['x']))
                y = round(float(button.attrib['y']))
                try:
                    text_sprite_number = button.find(
                        'properties/*[@name="textSpriteNumber"]').attrib['value']
                except:
                    print(
                        f'[ERROR] A PressureButton in {current_tmx_filename_wo_extension} has no textSpriteNumber!')
                    raise
                required_symbol_type = None
                if button.find('properties/*[@name="requiredSymbolType"]') != None:
                    required_symbol_type = button.find(
                        'properties/*[@name="requiredSymbolType"]').attrib['value']
                vertically_reversed_text = 'false'
                if button.find('properties/*[@name="isVerticallyReversed"]') != None:
                    vertically_reversed_text = 'true'
                is_on_text = 'false'
                if button.find('properties/*[@name="isOnByDefault"]') != None:
                    is_on_text = 'true'
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}, {is_on_text}, {text_sprite_number}, {vertically_reversed_text}, {f'entity::Symbol::Type::{required_symbol_type}' if required_symbol_type else 'bn::nullopt'}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write signs
            eng_arr_name = f'Z{zone_idx}_signTexts_English'
            kor_arr_name = f'Z{zone_idx}_signTexts_Korean'
            signs = entity_layer.findall("*[@type='Sign']")
            if signs:
                eng_str_max_cnt = max((sign.find(
                    'properties/*[@name="engMessage"]').attrib['value'].count(',')+1 for sign in signs))
                cpp_file.write(
                    f"constexpr bn::array<const bn::string_view, {eng_str_max_cnt}> {eng_arr_name}[] = {{"+'\n')
                for sign in signs:
                    cpp_file.write("{"+sign.find(
                        'properties/*[@name="engMessage"]').attrib['value']+"},"+'\n')
                cpp_file.write("};"+'\n')
                cpp_file.write('\n')

                kor_str_max_cnt = max((sign.find(
                    'properties/*[@name="korMessage"]').attrib['value'].count(',')+1 for sign in signs))
                cpp_file.write(
                    f"constexpr bn::array<const bn::string_view, {kor_str_max_cnt}> {kor_arr_name}[] = {{"+'\n')
                for sign in signs:
                    cpp_file.write("{"+sign.find(
                        'properties/*[@name="korMessage"]').attrib['value']+"},"+'\n')
                cpp_file.write("};"+'\n')
                cpp_file.write('\n')

                cpp_file.write(
                    f"constexpr ZoneInfo::SignInfo Z{zone_idx}_signInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::SignInfo> Z{zone_idx}_signInfos_ = {{"+'\n')
            sign_idx = 0
            for sign in signs:
                x = round(float(sign.attrib['x']))
                y = round(float(sign.attrib['y']))
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}, {eng_arr_name}[{sign_idx}], {kor_arr_name}[{sign_idx}]}},"+'\n')
                sign_idx += 1
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write exits
            exits = entity_layer.findall("*[@type='Exit']")
            if exits:
                cpp_file.write(
                    f"constexpr ZoneInfo::ExitInfo Z{zone_idx}_exitInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::ExitInfo> Z{zone_idx}_exitInfos_ = {{"+'\n')
            for exit in exits:
                x = round(float(exit.attrib['x']))
                y = round(float(exit.attrib['y']))
                w = round(float(exit.attrib['width']))
                h = round(float(exit.attrib['height']))
                destination_zone_idx = exit.find(
                    'properties/*[@name="destinationZoneIndex"]').attrib['value']
                entrance_idx = exit.find(
                    'properties/*[@name="entranceOfZoneIndex"]').attrib['value']
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2, {w}, {h}}}, {destination_zone_idx}, {entrance_idx}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # write entrances
            entrances = entity_layer.findall("*[@type='Entrance']")
            entrances.sort(key=lambda entrance: int(entrance.find(
                'properties/*[@name="entranceIndex"]').attrib['value']))
            if entrances:
                cpp_file.write(
                    f"constexpr ZoneInfo::EntranceInfo Z{zone_idx}_entranceInfos_[] = {{"+'\n')
            else:
                cpp_file.write(
                    f"constexpr bn::span<ZoneInfo::EntranceInfo> Z{zone_idx}_entranceInfos_ = {{"+'\n')
            for entrance in entrances:
                x = round(float(entrance.attrib['x']))
                y = round(float(entrance.attrib['y']))
                cpp_file.write(
                    f"{{{{{x} - {map_width_cpp_text} / 2, {y} - {map_height_cpp_text} / 2}}}},"+'\n')
            cpp_file.write("};"+'\n')
            cpp_file.write('\n')
            # add black hole
            prev_black_hole_count = len(black_holes)
            black_holes += entity_layer.findall('*[@type="BlackHole"]')
            if len(black_holes) >= 2:
                raise Exception(
                    f"There are more than 2 black holes in {current_stage_tmx_files_wo_extension}!")
            elif len(black_holes) == 1 and prev_black_hole_count == 0:
                black_hole_idx = zone_idx

    if cpp_file:
        write_cpp_ending(current_stage_id, current_stage_names, current_stage_is_player_gravity_reversed_by_default, bgm_name, cpp_file,
                         black_hole_idx, zone_infos, current_stage_tmx_files_wo_extension, root, map_width_cpp_text, map_height_cpp_text, black_holes)
        cpp_file.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    print('[stage-codegen.py] Generating stages...')
    skipped = process(args.build)
    if skipped:
        print('[stage-codegen.py] No change on stages; Skipped generating.')
    else:
        print('[stage-codegen.py] Generated stages!')

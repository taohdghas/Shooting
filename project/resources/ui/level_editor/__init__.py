import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json

#ブレンダーに登録するアドオン情報
bl_info = {
       "name": "レベルエディタ",
       "author": "Hayate Sano",
      "version": (1,0),
      "blender": (3,3,1),
      "location": "",
      "description": "レベルエディタ",
      "warning": "",
      "wiki_url": "",
      "tracker_url": "",
      "category": "Object"
}

#モジュールのインポート
from .stretch_vertex import MYADDON_OT_stretch_vertex
from .create_ico_sphere import MYADDON_OT_create_ico_sphere
from .create_spline_rail import MYADDON_OT_create_spline_rail
from .create_rail_point import MYADDON_OT_create_rail_point
from .export_scene import MYADDON_OT_export_scene
from .add_filename import MYADDON_OT_add_filename
from .add_collider import MYADDON_OT_add_collider
from .disabled import MYADDON_OT_disable_option
from .spawn import MYADDON_OT_spawn_import_symbol
from .spawn import MYADDON_OT_spawn_create_symbol
from . spawn import MYADDON_OT_spawn_create_player_symbol
from . spawn import MYADDON_OT_spawn_create_enemy_symbol
from .spawn import MYADDON_OT_spawn_create_boss_symbol
from .add_rail_link import MYADDON_OT_add_rail_link
from .my_menu import TOPBAR_MT_my_menu
from .filename import OBJECT_PT_file_name
from .collider import OBJECT_PT_collider
from .disabled import OBJECT_PT_disable_option
from .add_rail_link import OBJECT_PT_rail_link
from .draw_collider import DrawCollider


#Blenderに登録するクラスリスト
classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_create_spline_rail,
    MYADDON_OT_create_rail_point,
    MYADDON_OT_export_scene,
    MYADDON_OT_add_filename,
    MYADDON_OT_add_collider,
    MYADDON_OT_disable_option,
    MYADDON_OT_spawn_import_symbol,
    MYADDON_OT_spawn_create_symbol,
    MYADDON_OT_spawn_create_player_symbol,
    MYADDON_OT_spawn_create_enemy_symbol,
    MYADDON_OT_spawn_create_boss_symbol,
    MYADDON_OT_add_rail_link,
    TOPBAR_MT_my_menu,
    OBJECT_PT_file_name,
    OBJECT_PT_collider,
    OBJECT_PT_disable_option,
    OBJECT_PT_rail_link,
    )


#アドオン有効化時コールバック
def register():
    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    #3Dビューに描画関数を追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider,(),"WINDOW","POST_VIEW")
    
    print("レベルエディタが有効化されました。")
    
#アドオン無効化時コールバック
def unregister():
    #メニューから項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    #3Dビューから描画関数を削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle,"WINDOW")

    #Blenderからクラスを削除
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効化されました。")

#test
if __name__ == "__main__":
    register()
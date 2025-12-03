import bpy

# オペレータ ['rail_name'] カスタムプロパティ追加
class MYADDON_OT_add_rail_link(bpy.types.Operator):
    bl_idname = "myaddon.add_rail_link"
    bl_label = "RailName 追加"
    bl_description = "['rail_name'] カスタムプロパティを追加します（選択した敵にアクティブなレール名を登録）"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):
        active_obj = context.active_object
        selected_objs = context.selected_objects

        # アクティブオブジェクトがカーブであるか確認
        if not active_obj or active_obj.type != 'CURVE':
            self.report({'WARNING'}, "アクティブオブジェクトがカーブではありません。")
            return {'CANCELLED'}

        rail_name = active_obj.name

        # 選択中オブジェクトに rail_name プロパティを追加
        count = 0
        for obj in selected_objs:
            if obj == active_obj:
                continue
            # EnemySpawn 等、敵オブジェクトを想定
            if "EnemySpawn" in obj.name or obj.get("type") == "EnemySpawn":
                obj["rail_name"] = rail_name
                count += 1

        if count == 0:
            self.report({'WARNING'}, "敵オブジェクトが選択されていません。")
            return {'CANCELLED'}

        self.report({'INFO'}, f"{count} 個の敵を {rail_name} に紐付けました。")
        return {"FINISHED"}


# パネル RailLink 表示
class OBJECT_PT_rail_link(bpy.types.Panel):
    """レール紐付け用パネル"""
    bl_idname = "OBJECT_PT_rail_link"
    bl_label = "レールリンク"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self, context):
        layout = self.layout
        obj = context.object

        if "rail_name" in obj:
            layout.prop(obj, '["rail_name"]', text="Rail Name")
        else:
            layout.operator(MYADDON_OT_add_rail_link.bl_idname)

import bpy

class MYADDON_OT_create_rail_point(bpy.types.Operator):
    bl_idname = "myaddon.create_rail_point"
    bl_label = "レール制御点追加"
    bl_description = "RailPoint_### のEmptyをシーンに追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # 既存の RailPoint_### を検索して最大番号を求める
        existing_points = [
            obj for obj in bpy.context.scene.objects
            if obj.name.startswith("RailPoint_")
        ]

        # 次の番号を決定
        next_index = 1
        if existing_points:
            nums = []
            for obj in existing_points:
                try:
                    nums.append(int(obj.name.split("_")[1]))
                except:
                    pass
            if nums:
                next_index = max(nums) + 1

        # 新しいEmptyを作成
        new_name = f"RailPoint_{next_index:03}"
        bpy.ops.object.empty_add(type='PLAIN_AXES')
        empty = bpy.context.active_object
        empty.name = new_name

        self.report({'INFO'}, f"{new_name} を追加しました。")
        return {'FINISHED'}

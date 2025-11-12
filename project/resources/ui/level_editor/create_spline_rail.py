import bpy

# ============================================================
# レール全体のリアルタイム更新処理
# ============================================================
def update_all_rails(scene):
    """シーン内のレールをすべて更新"""
    for curve_obj in [o for o in bpy.data.objects if o.type == 'CURVE' and o.name.startswith("RailCurveObj_")]:
        curve_data = curve_obj.data
        spline = curve_data.splines[0]

        # RailPoint の参照がなければスキップ
        if "rail_points" not in curve_obj:
            continue

        control_points = []
        for rp_name in curve_obj["rail_points"]:
            obj = bpy.data.objects.get(rp_name)
            if obj and obj.type == 'EMPTY':
                control_points.append(obj)

        if not control_points:
            continue

        n = len(control_points)
        if len(spline.bezier_points) != n:
            spline.bezier_points.add(n - len(spline.bezier_points))

        for i, rp in enumerate(control_points):
            bp = spline.bezier_points[i]
            # ✅ ローカル座標ではなくワールド座標を参照
            bp.co = rp.matrix_world.translation.copy()
            bp.handle_left_type = 'AUTO'
            bp.handle_right_type = 'AUTO'


# ============================================================
# レール作成オペレーター
# ============================================================
class MYADDON_OT_create_spline_rail(bpy.types.Operator):
    bl_idname = "myaddon.create_spline_rail"
    bl_label = "スプラインレール作成"
    bl_description = "敵の位置を始点として、選択中の RailPoint_### を制御点にスプラインレールを作成します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # 敵オブジェクト（EnemySpawn）を取得
        enemy_objs = [obj for obj in bpy.context.selected_objects if "EnemySpawn" in obj.name]
        enemy_obj = enemy_objs[0] if enemy_objs else None

        # RailPoint を取得
        rail_points = [obj for obj in bpy.context.selected_objects
                       if obj.type == 'EMPTY' and obj.name.startswith("RailPoint_")]

        if not enemy_obj:
            self.report({'WARNING'}, "EnemySpawn が選択されていません。")
            return {'CANCELLED'}

        if not rail_points:
            self.report({'WARNING'}, "RailPoint_### が選択されていません。")
            return {'CANCELLED'}

        # 名前順にソート
        rail_points.sort(key=lambda o: o.name)

        # 敵の位置を始点として Empty を作成
        start_point = bpy.data.objects.new(f"{enemy_obj.name}_Start", None)
        start_point.empty_display_type = 'PLAIN_AXES'
        start_point.location = enemy_obj.location
        bpy.context.collection.objects.link(start_point)

        # ✅ 敵を親にして追従させる
        start_point.parent = enemy_obj
        start_point.matrix_parent_inverse = enemy_obj.matrix_world.inverted()

        # 制御点リストの先頭に追加
        rail_points.insert(0, start_point)

        # 制御点位置リスト
        control_points = [obj.matrix_world.translation.copy() for obj in rail_points]

        # カーブ名をユニーク化
        base_name = "RailCurveObj"
        existing = [obj for obj in bpy.data.objects if obj.name.startswith(base_name)]
        if existing:
            nums = []
            for obj in existing:
                try:
                    nums.append(int(obj.name.split("_")[-1]))
                except:
                    pass
            next_index = max(nums) + 1 if nums else 1
            curve_name = f"{base_name}_{next_index:03}"
        else:
            curve_name = base_name + "_001"

        # カーブ作成
        curve_data = bpy.data.curves.new(name=curve_name + "_Data", type='CURVE')
        curve_data.dimensions = '3D'

        spline = curve_data.splines.new('BEZIER')
        spline.bezier_points.add(len(control_points) - 1)

        for i, coord in enumerate(control_points):
            bp = spline.bezier_points[i]
            bp.co = coord
            bp.handle_left_type = 'AUTO'
            bp.handle_right_type = 'AUTO'

        # オブジェクト作成してシーンにリンク
        curve_obj = bpy.data.objects.new(curve_name, curve_data)
        bpy.context.collection.objects.link(curve_obj)

        # レールに制御点リストを記録
        curve_obj["rail_points"] = [rp.name for rp in rail_points]

        # 敵オブジェクトにレール名を記録
        enemy_obj["rail_name"] = curve_name

        # リアルタイム更新ハンドラ登録
        if update_all_rails not in bpy.app.handlers.depsgraph_update_post:
            bpy.app.handlers.depsgraph_update_post.append(update_all_rails)

        self.report({'INFO'}, f"敵 {enemy_obj.name} から始まるレールを作成しました（{curve_name}）")
        return {'FINISHED'}

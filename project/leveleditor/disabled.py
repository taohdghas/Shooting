import bpy

#オペレータ 無効オプションを追加する
class MYADDON_OT_disable_option(bpy.types.Operator):
    bl_idname = "myaddon.disable_option"
    bl_label = "無効フラグ"
    bl_description = "['disabled_flag'] カスタムプロパティを追加します"
    #リドゥ、アンドゥ可能オプション
    bl_options = {'REGISTER','UNDO'}   

    def execute(self,context):
        #['disabled_flag']カスタムプロパティを追加
        context.object["disabled_flag"] = False

        return {'FINISHED'}
    
#パネル 無効オプション
class OBJECT_PT_disable_option(bpy.types.Panel):
    """オブジェクトの無効フラグパネル"""
    bl_idname = "OBJECT_PT_disabled_flag"
    bl_label = "無効フラグ"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self,context):

        #パネルに項目を追加
        if "disabled_flag" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["disabled_flag"]',text=self.bl_label)
        else:
            #プロパティが無ければ、プロパティ追加ボタンを表示
             self.layout.operator(MYADDON_OT_disable_option.bl_idname)
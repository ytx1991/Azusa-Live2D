//本类用于声明live2d.dll的非托管接口--by Cronus 2013.11
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
namespace live
{
    class Live2D
    {
        //启动live2d
        //参数1：线程ID
        //启动后会按res文件夹下的config.txt\model.txt加载指定模型
        //config.txt格式：窗口left 窗口top  窗口width 窗口height
        //model.txt格式i：模型1路径；模型2路径；....；模型n路径
        [DllImport("Live2D.dll", EntryPoint = "Live2DStart")]
        public static extern bool Live2DStart(int tid);

        //终止live2d
        //参数1：线程ID
        [DllImport("Live2D.dll", EntryPoint = "Live2DAbort")]
        public static extern bool Live2DAbort(int hinst);
        //设置表情
        //参数1：线程ID,参数2：模型表情编号,参数3：模型索引
        [DllImport("Live2D.dll", EntryPoint = "SetExpression")]
        public static extern bool SetExpression(int hinst, string expid, int index);
        //设置动作
        //参数1：线程ID,参数2：动作类型名称,参数3：动作索引，参数4：优先级，参数5：模型索引
        [DllImport("Live2D.dll", EntryPoint = "StartMotion")]
        public static extern bool StartMotion(int hinst, string motiontype, int motionindex,int priority,int index);
        //添加模型
        //参数1：线程ID，参数2：要添加的模型路径
        //添加模型需保证上一次添加的模型已添加完毕（返回非-1），若返回-1表示上次添加的模型未添加，这是由于模型添加不能在渲染中添加，需等一次渲染结束后添加
        [DllImport("Live2D.dll", EntryPoint = "AddModel")]
        public static extern int AddModel(int hinst, string path);
        //删除所有模型
        //参数1：线程ID
        //更换模型请先清除再添加，官方例程也是如此操作，其原因不明
        [DllImport("Live2D.dll", EntryPoint = "RemoveModels")]
        public static extern bool RemoveModel(int hinst);
        //获得模型的JSON路径
        //参数1：线程ID，参数2：模型索引，参数3：返回的模型json相对路径
        [DllImport("Live2D.dll", EntryPoint = "GetModelInfo")]
        public static extern bool GetModelInfo(int hinst,int index,StringBuilder info);
        //获取表情信息
        public static string GetModelExpression(int hinst,int index)
        {
            StringBuilder str=new StringBuilder(2000);
            GetModelInfo(hinst, index, str);
            string model = new StreamReader(str.ToString()).ReadToEnd();
            string[] tmp = model.Split(']');
            foreach (string s in tmp)
            {
                if (s.IndexOf("expression") > 0)
                {
                    string[] ss = s.Split('[');
                    return ss[1];
                }
            }
            return "没有找到表情";
        }
        //获取动作信息
        public static string GetModelMotion(int hinst, int index)
        {
            StringBuilder str = new StringBuilder(2000);
            GetModelInfo(hinst, index, str);
            string model = new StreamReader(str.ToString()).ReadToEnd();
            string tmp = model.Substring(model.IndexOf("\"motions\":"));
            tmp = tmp.Substring(0, tmp.Length - 2);
            
            
            return tmp;    
        }
        //设置眼睛朝向
        //参数1：线程ID，参数2：x方向（-1~1），参数3：y方向（-1~1），参数4：模型索引
        [DllImport("Live2D.dll", EntryPoint = "SetEyeBallDirection")]
        public static extern bool SetEyeBallDirection(int hinst,float x,float y,int index);
        //设置脸朝向
        //参数1：线程ID，参数2：x方向（-1~1），参数3：y方向（-1~1），参数4：z方向（-1~1），参数5：模型索引
        [DllImport("Live2D.dll", EntryPoint = "SetFaceDirection")]
        public static extern bool SetFaceDirection(int hinst, float x, float y,float z,int index);
        //设置身体朝向
        //参数1：线程ID，参数2：x方向（-1~1），参数3：模型索引
        [DllImport("Live2D.dll", EntryPoint = "SetBodyDirection")]
        public static extern bool SetBodyDirection(int hinst, float x,int index);
        //设置缩放
        //参数1：线程ID，参数2：缩放中心x坐标，参数3：缩放中心y坐标，参数4：缩放
        [DllImport("Live2D.dll", EntryPoint = "SetViewDepth")]
        public static extern bool SetViewDepth(int hinst, int x, int y,int depth);
        //显示文本消息
        //参数1：线程ID，参数2：文本区域x坐标，参数3：文本区域y坐标，参数4：文本区域width，参数5：文本区域height，参数6：文本消息，参数7：字体高度，参数8：字体宽度，参数9：字体粗度，参数10：斜体，参数11：字体族，参数12：字体颜色ARGB（int32）
        [DllImport("Live2D.dll",CharSet=CharSet.Unicode, EntryPoint = "ShowMessage")]
        public static extern bool ShowMessage(int hinst, int x, int y, int width,int  height,string text,int fontHeight,int fontWidth,int fontWeight,bool italic,string family,uint color);
    }
}

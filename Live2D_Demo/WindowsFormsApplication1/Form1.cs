using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
namespace live
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        Thread l2dt;

        int id;
        private void Form1_Load(object sender, EventArgs e)
        {
            l2dt = new Thread(startlive2d);
            l2dt.Start();
          //  MessageBox.Show(" dll:" + id);
        }
        void startlive2d()
        {
            try
            {
                Live2D.Live2DStart(l2dt.ManagedThreadId);
            }
            catch (Exception ee)
            {
                MessageBox.Show(ee.Message);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

            

            Live2D.Live2DAbort(l2dt.ManagedThreadId);
            l2dt.Abort();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox4.Text == "" || textBox6.Text == "")
                MessageBox.Show("请输入表情编号和模型索引");
            else
            Live2D.SetExpression(l2dt.ManagedThreadId, textBox4.Text, int.Parse(textBox6.Text));
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
                MessageBox.Show("请输入模型路径");
            else
              if(  Live2D.AddModel(l2dt.ManagedThreadId, textBox1.Text)==-1)
                  MessageBox.Show("当前仍有模型未添加完毕") ;
           
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if(!Live2D.RemoveModel(l2dt.ManagedThreadId))
                MessageBox.Show("删除失败");
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (textBox3.Text == "")
                MessageBox.Show("请输入模型索引");
            else
            {
                StringBuilder str = new StringBuilder(1000);
                if (!Live2D.GetModelInfo(l2dt.ManagedThreadId, int.Parse(textBox3.Text), str))
                    MessageBox.Show("获取信息失败");
                textBox2.Text = new StreamReader(str.ToString()).ReadToEnd();
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (textBox6.Text == "")
                MessageBox.Show("请输入模型索引");
            else
            textBox5.Text = Live2D.GetModelExpression(l2dt.ManagedThreadId, int.Parse(textBox6.Text));
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (textBox7.Text == "")
                MessageBox.Show("请输入模型索引");
            else
            textBox8.Text = Live2D.GetModelMotion(l2dt.ManagedThreadId, int.Parse(textBox7.Text));
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (textBox7.Text == "" || textBox9.Text == "" || textBox10.Text == "" || textBox11.Text == "")
                MessageBox.Show("请输入动作类型名称、动作索引、动作优先级、模型索引");
            else
                Live2D.StartMotion(l2dt.ManagedThreadId, textBox9.Text, int.Parse(textBox10.Text), int.Parse(textBox11.Text), int.Parse(textBox7.Text));
        }

        private void button8_Click(object sender, EventArgs e)
        {
            
            if (eyex.Text == "" || eyey.Text == ""  || modelindex.Text == "")
                MessageBox.Show("请输入完整信息");
            else { Live2D.SetEyeBallDirection(l2dt.ManagedThreadId, float.Parse(eyex.Text), float.Parse(eyey.Text), int.Parse(modelindex.Text)); }
       }

        private void button9_Click(object sender, EventArgs e)
        {
            
            if (bodyx.Text == ""  || modelindex.Text == "")
                MessageBox.Show("请输入完整信息");
            else { Live2D.SetBodyDirection(l2dt.ManagedThreadId, float.Parse(bodyx.Text), int.Parse(modelindex.Text)); }
       
        }

        private void button10_Click(object sender, EventArgs e)
        {

            if (zoomy.Text == "" || zoomx.Text == ""||zoomz.Text=="")
                MessageBox.Show("请设置x，y坐标");
            else
                Live2D.SetViewDepth(l2dt.ManagedThreadId, int.Parse(zoomx.Text),int.Parse(zoomz.Text), int.Parse(zoomz.Text));
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void button11_Click(object sender, EventArgs e)
        {
            if (facex.Text == "" || facey.Text == "" || facez.Text == "" || modelindex.Text == "")
                MessageBox.Show("请输入完整信息");
            else { Live2D.SetFaceDirection(l2dt.ManagedThreadId, float.Parse(facex.Text), float.Parse(facey.Text), float.Parse(facez.Text), int.Parse(modelindex.Text)); }
        }

        private void button12_Click(object sender, EventArgs e)
        {
           
            if (text.Text == "" || textX.Text == "" || textY.Text == "" || textHeight.Text == "" || textWidth.Text == "" || fontColor.Text == "" || fontFamily.Text == "" || fontHeight.Text == "" || fontWeight.Text == "" || fontWidth.Text == "")
                MessageBox.Show("请输入完整信息");
            else
            {
         
                Live2D.ShowMessage(l2dt.ManagedThreadId, int.Parse(textX.Text),
                    int.Parse(textY.Text), int.Parse(textWidth.Text), 
                    int.Parse(textHeight.Text),
                   //      Encoding.Unicode.GetString(Encoding.Unicode.GetBytes(text.Text)),
                   text.Text,
                          int.Parse(fontHeight.Text),
                    int.Parse(fontWidth.Text),
                    int.Parse(fontWeight.Text), 
                    checkBox1.Checked,
                  //   Encoding.Unicode.GetString(Encoding.Unicode.GetBytes(fontFamily.Text)),
                  fontFamily.Text,
                    uint.Parse(fontColor.Text, System.Globalization.NumberStyles.AllowHexSpecifier));
                }
        }
        
    }
}

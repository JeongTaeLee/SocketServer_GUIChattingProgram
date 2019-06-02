using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace JIGAPProtoHelper
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }
        private void ProtoAppPathButton_Click(object sender, EventArgs e)
        {
            protoAppTextBox.Clear();
            protoAppOpenDialog.InitialDirectory = "C:\\";


            if (protoAppOpenDialog.ShowDialog() == DialogResult.OK)
            {
                string strPath = null;
                strPath = protoAppOpenDialog.FileName;
                protoAppTextBox.Text = strPath;
            }
        }

        private void ProtoFilePathOpenButton_Click(object sender, EventArgs e)
        {
            protoFilePathTextBox.Clear();
            protoFileOpenDialog.InitialDirectory = "C:\\";

            if (protoFileOpenDialog.ShowDialog() == DialogResult.OK)
            {
                string strPath = null;
                strPath = protoFileOpenDialog.FileName;
                protoFilePathTextBox.Text = strPath;
            }
        }

        private void OutputPathOpen_Click(object sender, EventArgs e)
        {
            outputPathTextBox.Clear();


            if (outputPathDialog.ShowDialog() == DialogResult.OK)
            {
                string strPath = null;
                strPath = outputPathDialog.SelectedPath;
                outputPathTextBox.Text = strPath;
            }
        }

        private void OutputButton_Click(object sender, EventArgs e)
        {
            if (protoAppTextBox.Text.Length > 0)
            {
                System.IO.FileInfo fi = new System.IO.FileInfo(protoAppTextBox.Text);

                if (!fi.Exists)
                {
                    MessageBox.Show("프로토앱의 경로가 올바르지 않습니다.");
                    return;
                }
            }
            else
            {
                MessageBox.Show("프로토앱의 경로를 지정해주세요");
                return;
            }

            if (protoFilePathTextBox.Text.Length > 0)
            {
                System.IO.FileInfo fi = new System.IO.FileInfo(protoFilePathTextBox.Text);

                if (!fi.Exists)
                {
                    MessageBox.Show(".proto 경로가 올바르지 않습니다");
                    return;
                }
            }
            else
            {
                MessageBox.Show(".proto 파일 경로를 지정해주세요");
                return;
            }

            if ((checkBox1.Checked) == true || outputPathTextBox.Text.Length > 0)
            {
                if (!checkBox1.Checked)
                {

                    System.IO.DirectoryInfo fi = new System.IO.DirectoryInfo(outputPathTextBox.Text);


                    if (!fi.Exists)
                    {
                        MessageBox.Show("출력 경로가 존재하지 않습니다!");
                        return;
                    }
                }
            }
            else
            {
                MessageBox.Show("출력 경로를 지정해주세요");
                return;
            }

            System.Diagnostics.ProcessStartInfo proInfo = new System.Diagnostics.ProcessStartInfo();
            System.Diagnostics.Process pro = new System.Diagnostics.Process();

            proInfo.FileName = @"cmd";
            proInfo.CreateNoWindow = false;
            proInfo.RedirectStandardOutput = true;
            proInfo.RedirectStandardInput = true;
            proInfo.RedirectStandardError = true;
            proInfo.UseShellExecute = false;

            pro.StartInfo = proInfo;
            pro.Start();

            int index = protoAppTextBox.Text.IndexOf(':');
            pro.StandardInput.Write(protoAppTextBox.Text.Substring(0, index + 1) + Environment.NewLine);

            index = protoAppTextBox.Text.LastIndexOf('\\');
            pro.StandardInput.Write("cd " + protoAppTextBox.Text.Substring(0, index + 1) + Environment.NewLine);

            index = protoFilePathTextBox.Text.LastIndexOf('\\');

            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append("protoc -I=");
            stringBuilder.Append(protoFilePathTextBox.Text.Substring(0, index + 1));
            stringBuilder.Append(" --cpp_out=");
 
            if (checkBox1.Checked)
            {
                index = protoFilePathTextBox.Text.LastIndexOf('\\');
                stringBuilder.Append(protoFilePathTextBox.Text.Substring(0, index + 1));
            }
            else
                stringBuilder.Append(outputPathTextBox.Text);

            stringBuilder.Append(" ");
            stringBuilder.Append(protoFilePathTextBox.Text);
            stringBuilder.Append(Environment.NewLine);


            pro.StandardInput.Write(stringBuilder.ToString());
            pro.StandardInput.Close();

            MessageBox.Show(pro.StandardOutput.ReadToEnd());
            MessageBox.Show("출력 실행! 폴더를 확인해주세요\n");

            pro.WaitForExit();
            pro.Close();
        }

        private void CheckBox1_CheckedChanged_1(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                outputPathOpenBtn.Enabled = false;
                outputPathTextBox.Enabled = false;
            }
            else
            {
                outputPathOpenBtn.Enabled = true;
                outputPathTextBox.Enabled = true;
            }
        }

    }
}

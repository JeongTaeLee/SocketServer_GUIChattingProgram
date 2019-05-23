using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace JIGAPClientGUI
{
    public partial class Form1 : Form
    {
        MessageForm messageForm;
        public Form1()
        {
            InitializeComponent();
        }

        //'연결' 버튼을 클릭했을때 작동하는 이벤트 함수 입니다.
        private void Button1_Click(object sender, EventArgs e)
        {
            if (!NetworkManager.GetInst().bOnConnect)
            {
                messageForm = new MessageForm();
                messageForm.Show();

                NetworkManager.GetInst().ClientStart(ipAddrTextBox.Text, portAddrTextBox.Text);
            }
        }
    }
}

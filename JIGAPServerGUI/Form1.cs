using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace JIGAPServerGUI
{
    public partial class Form1 : Form
    {
        private LogForm logForm = null;      //Server의 Log를 출력하는 Form입니다.

        public Form1()
        {
            InitializeComponent();
        }

        /*서버 열기 버튼을 클릭했을때 함수입니다*/
        private void ServerOpenButton_Click(object sender, EventArgs e)
        {
            if(!ServerManager.GetInst().bOnServer)
            {
                /*Server의 Log를 띄우기 위한 창을 생성하고 띄웁니다*/
                logForm = new LogForm();
                logForm.Show();

                /*사용자가 입력한 정보로 서버를 생성 합니다*/
                ServerManager.GetInst().ServerOpen(IPTextBox.Text, PortTextBox.Text);
            }
        } 

        /*서버 리셋 버튼을 클릭했을때 함수입니다.*/
        /*서버 종료 코드는 LogForm.cs 파일 LogForm::ServerCloseButton_Click 함수에 있습니다*/
        private void ServerResetButton_Click(object sender, EventArgs e)
        {

        }
    }
}

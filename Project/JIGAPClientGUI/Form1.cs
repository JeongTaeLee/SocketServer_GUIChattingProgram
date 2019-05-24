using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace JIGAPClientGUI
{
    public partial class Form1 : Form
    {
        Thread messageLogThread = null;
        bool messageLogThreadExit = false;

        public Form1()
        {
            InitializeComponent();

            sendingTextBox.Enabled = false;
            sendingButton.Enabled = false;

            this.Shown += new EventHandler(ClientFormShown);
            this.FormClosing += new FormClosingEventHandler(ClientFormClosing);
        }

        #region WinFormEvent
        //'연결' 버튼을 클릭했을때 작동하는 이벤트 함수 입니다.
        private void Button1_Click(object sender, EventArgs e)
        {
            if (ipAddrTextBox.Text.Length == 0 ||
                portAddrTextBox.Text.Length == 0 ||
                nickNameTextbox.Text.Length == 0)
                return;

            if (!NetworkManager.GetInst().bOnConnect)
            {
                if (NetworkManager.GetInst().ClientStart(ipAddrTextBox.Text, portAddrTextBox.Text))
                {
                    NetworkManager.GetInst().SendMessage(nickNameTextbox.Text);

                    sendingTextBox.Enabled = true;
                    sendingButton.Enabled = true;

                    button1.Enabled = false;
                    ipAddrTextBox.Enabled = false;
                    portAddrTextBox.Enabled = false;
                    nickNameTextbox.Enabled = false;
                }
            }
        }

        private void SendingButton_Click(object sender, EventArgs e)
        {
            SendingMessage();
        }

        private void SendingTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                SendingMessage();
        }

        public void ClientFormShown(object obj, EventArgs args)
        {
            messageLogThread = new Thread(new ThreadStart(delegate ()
            {
                while (true)
                {
                    if (messageLogThreadExit)
                        break;

                    if (NetworkManager.GetInst().CheckMessageLog())
                    {
                        string log = NetworkManager.GetInst().GetMessageLog() + "\r\n";
                        this.Invoke(new Action(delegate ()
                        {
                            messageLogTextBox.AppendText(log);
                        }));
                    }
                }
            }
            ));

            messageLogThread.Start();
        }

        public void ClientFormClosing(object obj, EventArgs args)
        {
            messageLogThreadExit = true;
            messageLogThread.Join();

            NetworkManager.GetInst().ClientEnd();
        }
        #endregion

        private void SendingMessage()
        {
            if (NetworkManager.GetInst().bOnConnect)
            {
                if (sendingTextBox.Text.Length != 0)
                {
                    NetworkManager.GetInst().SendMessage(lsendingTextBox.Text);
                    sendingTextBox.Clear();
                }
            }
        }

    }
}

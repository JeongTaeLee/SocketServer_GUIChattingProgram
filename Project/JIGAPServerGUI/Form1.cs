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
using System.Runtime.InteropServices;

namespace JIGAPServerGUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            ServerManager.Instance.RegisterLogFunc(ServerLogFunc);

            this.FormClosing += Closing;
        }

        protected virtual new void Closing(object sender, FormClosingEventArgs e)
        {
            ServerManager.Instance.CloseServer();
            ServerManager.Instance.jigapServer.Dispose();

        }

        public void ServerLogFunc(IntPtr strLog)
        {
            StringBuilder str = new StringBuilder();
            str.Append("System : ");
            str.Append(Marshal.PtrToStringUni(strLog));
            str.Append("\r\n");

            if (LogTextBox.InvokeRequired)
            {
                LogTextBox.BeginInvoke(new MethodInvoker(delegate {
                    LogTextBox.AppendText(str.ToString());
                }));
            }
            else
            {
                LogTextBox.AppendText(str.ToString());
            }
        }

        private void ServerSwitch_Click(object sender, EventArgs e)
        {
            if (ServerManager.Instance.IsOnServer == false)
            {
                if (ServerManager.Instance.OpenServer(PortTextBox.Text))
                {
                    PortTextBox.Enabled = false;

                    ServerSwitch.Text = "Close";
                }
            }
            else
            {
                ServerManager.Instance.CloseServer();

                PortTextBox.Enabled = true;

                ServerSwitch.Text = "Open";
            }
        }
    }
}

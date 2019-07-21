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

            ServerManager.Instance.Initialize();
            ServerManager.Instance.RegisterLogFunc(ServerLogFunc);
        }

       

        public void ServerLogFunc(IntPtr strLog)
        {
            string str = Marshal.PtrToStringUni(strLog);
        }

        private void ServerSwitch_Click(object sender, EventArgs e)
        {
            if (ServerManager.Instance.IsOnServer == false)
            {
                if (ServerManager.Instance.OpenServer(IpTextBox.Text, PortTextBox.Text))
                {
                    IpTextBox.Enabled   = false;
                    PortTextBox.Enabled = false;

                    ServerSwitch.Text = "Close";
                }
            }
            else
            {
                ServerManager.Instance.CloseServer();

                IpTextBox.Enabled   = true;
                PortTextBox.Enabled = true;

                ServerSwitch.Text = "Open";
            }
        }
    }
}

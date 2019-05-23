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
    public partial class MessageForm : Form
    {
        Thread messageThread = null;
        bool messageThreadExit = false;

        public MessageForm()
        {
            InitializeComponent();

            this.Shown += new EventHandler(MessageFormShown);
            this.FormClosing += new FormClosingEventHandler(MessageFormClosing);
        }

        public void MessageFormShown(object obj, EventArgs args)
        {
            messageThread = new Thread(new ThreadStart(delegate()
            {
                while(true)
                {
                    if (messageThreadExit)
                        break;

                    if (NetworkManager.GetInst().CheckMessageLog())
                    {
                        this.Invoke(new Action(delegate ()
                        {
                            messageBox.AppendText(NetworkManager.GetInst().GetMessageLog() + "\r\n");
                        }));
                    }
                }
            }
            ));

            messageThread.Start();
        }

        public void MessageFormClosing(object obj, EventArgs args)
        {
            messageThreadExit = true;

            messageThreadExit = true;
            messageThread.Join();

            NetworkManager.GetInst().ClientEnd();
        }
    }
}

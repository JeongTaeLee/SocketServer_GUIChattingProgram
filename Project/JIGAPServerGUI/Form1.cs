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

namespace JIGAPServerGUI
{
    public partial class Form1 : Form
    {
        Thread systemLogThread = null;
        bool systemLogThreadExit = false;

        public Form1()
        {
            InitializeComponent();
        }
    }
}

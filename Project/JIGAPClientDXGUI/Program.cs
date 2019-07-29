using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D9;
using SharpDX.Windows;
using SharpDX.Multimedia;
using JIGAPClientDXGUI.Engine;
using System.Runtime.InteropServices;

namespace JIGAPClientDXGUI
{
    class Program
    {
        public static void Main(string[] args)
        {
            using (MainGame mainGame = new MainGame())
            {
                if(mainGame.Initialize())
                    mainGame.Run();
            }
        }
    }
}

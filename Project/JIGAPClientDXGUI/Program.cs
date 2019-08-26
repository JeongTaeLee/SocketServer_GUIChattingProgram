using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;
using System.Runtime.InteropServices;
using System.Threading;

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

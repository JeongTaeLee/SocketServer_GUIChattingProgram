using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class BackGround : GameObject 
    {
        public texture BGTexture { get; set; } = null;

        public override void Render()
        {
            BGTexture?.Draw();
        }
    }
}

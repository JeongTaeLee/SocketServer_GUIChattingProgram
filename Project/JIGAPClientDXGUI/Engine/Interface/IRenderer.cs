using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    public enum TexturePivot
    { 
        LeftTop,
        Center
    }

    public interface IRenderer
    {
        void Render();
    }
}

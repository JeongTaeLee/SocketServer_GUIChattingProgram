﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{ 
    public abstract class Scene
    {

        public abstract void Init();
        public abstract void Release();

    }
}

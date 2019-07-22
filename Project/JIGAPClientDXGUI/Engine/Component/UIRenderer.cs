using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class UIRenderer : SpriteRenderer
    {
        public override void Init()
        {
            ObjectManager.Instance.RegisterUIRenderer(this);
        }
        public override void Release()
        {
            ObjectManager.Instance.UnRegisterUIRenderer(this);
        }
    }
}

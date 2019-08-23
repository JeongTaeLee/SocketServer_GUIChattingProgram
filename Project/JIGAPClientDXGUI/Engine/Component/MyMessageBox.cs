using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class MyMessageBox : Component 
    {
        private UIRenderer  _uiRenderer = null;
        private Button      _button = null;
        private Text        _titleText = null;
        private Text        _strText = null;

        public override void Init()
        {
            base.Init();

            _uiRenderer = gameObject.GetComponent<UIRenderer>();
            if (_uiRenderer == null)
                _uiRenderer = gameObject.AddComponent<UIRenderer>();

            _uiRenderer.Texture = ResourceManager.Instance.LoadTexture("MessageBox");

            _button = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            _button.transform.Parent = transform;
            _button.transform.position = new SharpDX.Vector3(148f, 244f, 0f);
            _button.SetButton(ResourceManager.Instance.LoadTexture("OkButton"), () => { gameObject.Destroy = true; });


            _titleText = ObjectManager.Instance.RegisterObject().AddComponent<Text>();
            _strText = ObjectManager.Instance.RegisterObject().AddComponent<Text>();

            _titleText.transform.Parent = transform;
            _titleText.font = ResourceManager.Instance.LoadFont("MessageBoxTitle");
            _titleText.drawFlag = SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.VerticalCenter | SharpDX.Direct3D9.FontDrawFlags.NoClip;
            _titleText.transform.position = new SharpDX.Vector3(224f, 32f, 0f);

            _strText.transform.Parent = transform;
            _strText.font = ResourceManager.Instance.LoadFont("MessageBox");
            _strText.drawFlag = SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.VerticalCenter |SharpDX.Direct3D9.FontDrawFlags.NoClip;
            _strText.transform.position = new SharpDX.Vector3(224f, 146, 0f);

            transform.position = new SharpDX.Vector3(416f, 214f, 0f);
        }

        public void Initialize(string inTitle, string inStr)
        {
            _titleText.SetString(inTitle);
            _strText.SetString(inStr);
        }


        public static MyMessageBox Show(string inTile, string inStr)
        {
            MyMessageBox box = ObjectManager.Instance.RegisterObject().AddComponent<MyMessageBox>();
            box.Initialize(inTile, inStr);
            return box;
        }
    }
}

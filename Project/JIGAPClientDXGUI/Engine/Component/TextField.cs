using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI
{
   
    partial class TextField : Component
    {
        public delegate void FieldBehavior(string str);

        private Rectangle _boxRange = new Rectangle();
        private UIRenderer _uiRenderer = null;
        private Text _textComponent = null;
        private bool _activeBox = false;
        private FieldBehavior _enterBehavior = null;

        public int Width { get; set; } = 0;
        public int Height { get; set; } = 0;

        public texture texture
        {
            get
            {
                return _uiRenderer.Texture;
            }
            set
            {
                Width = value.d3dInfo.Width;
                Height = value.d3dInfo.Height;

                _uiRenderer.Texture = value;
            }
        }

        public Text textComponent
        {
            get
            {
                return _textComponent;
            }
        }


        public FieldBehavior enterBehavior
        {
            get => _enterBehavior;
            set => _enterBehavior = value;
        }
    }



    partial class TextField : Component
    {
        public override void Init()
        {
            DXManager.Instance.RegisterWndProc(WndProc);

            _uiRenderer = gameObject.GetComponent<UIRenderer>();

            if (_uiRenderer == null)
                _uiRenderer = gameObject.AddComponent<UIRenderer>();

            _textComponent = ObjectManager.Instance.RegisterObject().AddComponent<Text>();
            _textComponent.transform.Parent = transform;

            _textComponent.transform.position = new Vector3(5f, -2f, 0f);

            _activeBox = false;
            _uiRenderer.Color = new Color(150, 150, 150);
        }
        public override void Release()
        {
            DXManager.Instance.UnRegisterwndProc(WndProc);
        }

        public override void Update()
        {
            Renew();

            System.Drawing.Point cursorPoint = InputManager.Instance.MousePoint;

            if (InputManager.Instance.GetMouseDown(InputManager.MouseButtonType.Left))
            {
                if (_boxRange.Contains(cursorPoint.X, cursorPoint.Y))
                    OnActiveBox();
                else
                    OnUnActiveBox();

            }
        }

        /*움직였을 경우 위치나 충돌 범위 업데이트*/
        private void Renew()
        {
            _boxRange = new Rectangle((int)transform.position.X, (int)transform.position.Y, Width, Height);
            _textComponent.range = new SharpDX.Mathematics.Interop.RawRectangle(0, 0, Width - 10, Height);
        }

        public void OnActiveBox()
        {
            if (_activeBox) return;

            _activeBox = true;
            _uiRenderer.Color = new Color(255, 255, 255);
        }
        public void OnUnActiveBox()
        {
            if (!_activeBox) return;

            _activeBox = false;
            _uiRenderer.Color = new Color(150, 150, 150);
        }

        private void AppendText(char c)
        {
            if (!_activeBox)
                return;

            switch ((int)c)
            {
                case KeyType.VK_TAB:
                    break;
                case KeyType.VK_BACKSPACE:
                    if (_textComponent.str.Length > 0)
                        _textComponent.DeleteCharInString();
                    break;
                case KeyType.VK_ENTER:
                    if (_activeBox)
                        _enterBehavior?.Invoke(_textComponent.str);
                    break;
                default:
                    _textComponent.SetAddString(c);
                    break;
            }
        }

        private void WndProc(object sender, Message msg)
        {
            switch (msg.Msg)
            {
                case MsgType.WM_CHAR:
                    AppendText((char)msg.WParam);
                    break;
            }

        }
    }


}

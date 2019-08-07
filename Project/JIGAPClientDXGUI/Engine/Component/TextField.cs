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

        private Rectangle boxRange = new Rectangle();
        private GameObject textChild = null;
        private UIRenderer UIRenderer = null;
        private Text Text = null;
        private bool ActiveBox = false;
        private FieldBehavior enterBehavior = null;

        public int Width { get; set; } = 0;
        public int Height { get; set; } = 0;

        public texture Texture
        {
            get
            {
                return UIRenderer.Texture;
            }
            set
            {
                Width = value.d3dInfo.Width;
                Height = value.d3dInfo.Height;

                UIRenderer.Texture = value;
            }
        }

        public string String
        {
            get
            {
                return Text.text.ToString();
            }
            set
            {
                Text.text.Clear();
                Text.text.Append(value);
            }
        }

        public Vector3 TextOffset
        {
            get
            {
                return textChild.transform.position;
            }

            set
            {
                textChild.transform.position = value;
            }
        }
        public Color TextColor
        {
            get
            {
                return Text.color;
            }
            set
            {
                Text.color = value;
            }
        }


        public FontDrawFlags TextDrawFlag
        {
            get
            {
                return Text.drawFlag;
            }
            set
            {
                Text.drawFlag = value;
            }
        }

        public FieldBehavior EnterBehavior
        {
            get => enterBehavior;
            set => enterBehavior = value;
        }
    }



    partial class TextField : Component
    {
        public override void Init()
        {
            DXManager.Instance.RegisterWndProc(WndProc);

            UIRenderer = gameObject.GetComponent<UIRenderer>();

            if (UIRenderer == null)
                UIRenderer = gameObject.AddComponent<UIRenderer>();

            textChild = ObjectManager.Instance.RegisterObject();
            textChild.transform.Parent = this.transform;
            Text = textChild.AddComponent<Text>();

            TextOffset = new Vector3(5f, -2f, 0f);

            ActiveBox = false;
            UIRenderer.Color = new Color(150, 150, 150);
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
                if (boxRange.Contains(cursorPoint.X, cursorPoint.Y))
                    OnActiveBox();
                else
                    OnUnActiveBox();

            }
        }

        /*움직였을 경우 위치나 충돌 범위 업데이트*/
        private void Renew()
        {
            boxRange = new Rectangle((int)transform.position.X, (int)transform.position.Y, Width, Height);
            Text.range = new SharpDX.Mathematics.Interop.RawRectangle(0, 0, Width - 10, Height);
        }

        public void OnActiveBox()
        {
            if (ActiveBox) return;

            ActiveBox = true;
            UIRenderer.Color = new Color(255, 255, 255);
        }
        public void OnUnActiveBox()
        {
            if (!ActiveBox) return;

            ActiveBox = false;
            UIRenderer.Color = new Color(150, 150, 150);
        }

        private void AppendText(char c)
        {
            if (!ActiveBox)
                return;

            switch ((int)c)
            {
                case KeyType.VK_TAB:
                    break;
                case KeyType.VK_BACKSPACE:
                    if (Text.text.Length > 0)
                        Text.text.Remove(Text.text.Length - 1, 1);
                    break;
                case KeyType.VK_ENTER:
                    if (ActiveBox)
                        enterBehavior?.Invoke(Text.text.ToString());
                    break;
                default:
                    Text.text.Append(c);
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

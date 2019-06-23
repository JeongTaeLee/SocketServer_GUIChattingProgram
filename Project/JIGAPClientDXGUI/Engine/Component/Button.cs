using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
namespace JIGAPClientDXGUI.Engine
{
    partial class Button : Component
    {
        public delegate void ButtonEvent();

        public UIRenderer UIRenderer { get; set; } = null;
        public int Width { get; set; } = 10;
        public int Height { get; set; } = 10;

        private Rectangle rectangle = new Rectangle();

        private ButtonEvent buttonEvent = null;

        private bool buttonStart = false;
    }
    partial class Button : Component
    {
        public override void Init()
        {
            UIRenderer = gameObject.GetComponent<UIRenderer>();

            if (UIRenderer == null)
                UIRenderer = gameObject.AddComponent<UIRenderer>();
        }

        public override void Update()
        {
            RenewButton();

            if (UIRenderer == null)
                return;

            System.Drawing.Point mousePoint = InputManager.Instance.MousePoint;

            if (rectangle.Contains(mousePoint.X, mousePoint.Y))
            {
                if (InputManager.Instance.GetMouseDown(InputManager.MouseButtonType.Left))
                    buttonStart = true;

                if (UIRenderer.Color.R > 150)
                {
                    UIRenderer.Color = new Color(UIRenderer.Color.R - 1, UIRenderer.Color.G - 1,
                        UIRenderer.Color.B - 1, UIRenderer.Color.A);
                }

                
                if (InputManager.Instance.GetMouse(InputManager.MouseButtonType.Left) && buttonStart)
                    UIRenderer.Color = new Color(100, 100, 100);
                else
                    UIRenderer.Color = new Color(150, 150, 150);

                if (InputManager.Instance.GetMouseUp(InputManager.MouseButtonType.Left) && buttonStart)
                    buttonEvent?.Invoke();
            }
            else
            {
                buttonStart = false;
                UIRenderer.Color = new Color(255, 255, 255, 255);
            }
        }

        public void SetButton(texture inTexture, float x, float y, int inWidth, int inHeight, ButtonEvent inButtonEvent)
        {
            UIRenderer.Texture = inTexture;

            Width   = inWidth;
            Height  = inHeight;

            transform.position = new Vector3(x, y, 0f);

            rectangle = new Rectangle((int)x, (int)y, inWidth, inHeight);

            buttonEvent = inButtonEvent;
        }

        /*
         * 버튼 정보를 변경한 정보로 다시 갱신합니다
         */
        public void RenewButton()
        {
            rectangle = new Rectangle((int)transform.position.X, (int)transform.position.Y, Width, Height);
        }

    }
}

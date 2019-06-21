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
        public UIRenderer UIRenderer { get; set; } = null;
        public int Width { get; set; } = 10;
        public int Height { get; set; } = 10;

        private Rectangle rectangle = new Rectangle();
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
            if (UIRenderer == null)
                return;

            System.Drawing.Point mousePoint = InputManager.Instance.MousePoint;

            if (rectangle.Contains(mousePoint.X, mousePoint.Y))
            {
                if (UIRenderer.Color.R > 150)
                {
                    UIRenderer.Color = new Color(UIRenderer.Color.R - 1, UIRenderer.Color.G - 1,
                        UIRenderer.Color.B - 1, UIRenderer.Color.A);
                }
            }
            else
            {
                UIRenderer.Color = new Color(255, 255, 255, 255);
            }
                
        }

        public void SetButton(texture inTexture, int x, int y, int inWidth, int inHeight)
        {
            UIRenderer.Texture = inTexture;

            Width   = inWidth;
            Height  = inHeight;

            transform.position = new Vector3(x, y, 0f);

            rectangle = new Rectangle(x, y, inWidth, inHeight);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Direct3D9;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class RoomButton : Button
    {
        public string RoomName { get; set; }

        public override void Init()
        {
            base.ButtonTexture = ImageManager.GetInst().LoadTexture("RoomButton", "./Image/RoomButton.png");
            base.ButtonEventCallBack = delegate ()
            {
                NetworkManager.GetInst().RequestJoinedRoom(RoomName);
            };
        }

        public override void Render()
        {
            base.Render();

            if (RoomName?.Length > 0)
            {
                Font font = new Font(DXManager.GetInst().d3dDevice, 40, 0, FontWeight.Bold,
                    0, false, FontCharacterSet.Default, FontPrecision.Default, FontQuality.Default,
                    FontPitchAndFamily.Default, "돋움");

                DXManager.GetInst().d3dSprite.Transform = Matrix.Translation(new Vector3(transform.worldPos.X + 290,
                    transform.worldPos.Y + 15, 0f));
                font.DrawText(DXManager.GetInst().d3dSprite, RoomName, new Rectangle(), SharpDX.Direct3D9.FontDrawFlags.Center | SharpDX.Direct3D9.FontDrawFlags.NoClip, Color.Black);

                font.Dispose();
            }
        }

    }
}

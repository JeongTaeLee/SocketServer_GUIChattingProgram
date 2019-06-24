using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;


using ProtoBuf;

using JIGAPClientDXGUI.Engine;

using SharpDX;
using SharpDX.Direct3D9;

namespace JIGAPClientDXGUI
{
    public class LoginScene : Scene
    {

        public override void Init()
        {
            if (!NetworkManager.Instance.ConnectServer())
            {
                System.Windows.Forms.Application.Exit();
                return;
            }

            GameObject BackGround = ObjectManager.Instance.RegisterObject();
            BackGround.AddComponent<UIRenderer>().Texture = ImageManager.Instance.LoadTexture("LoginBackGround");

            GameObject LoginTextBox = ObjectManager.Instance.RegisterObject();
            TextField field= LoginTextBox.AddComponent<TextField>();
            field.Texture = ImageManager.Instance.LoadTexture("LoginTextBox");
            field.String = "Login 정보를 입력해주세요";
            field.EnterBehavior = (string str) => { OnLogin(); };
            LoginTextBox.transform.position = new SharpDX.Vector3(429f, 315f, 0f);

            GameObject loginButton = ObjectManager.Instance.RegisterObject();
            loginButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("LoginButton"), 560f, 418f, 223, 58, () => { OnLogin(); });

        }
        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }


        private void OnLogin()
        {
            SceneManager.Instance.ChanageScene("IngameScene");
        }

    }
}

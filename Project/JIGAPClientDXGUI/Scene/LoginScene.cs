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

using System.Net;
using System.Net.Sockets;

namespace JIGAPClientDXGUI
{
    public class LoginScene : Scene
    {

        public override void Init()
        {
  
            GameObject backGround = ObjectManager.Instance.RegisterObject();
            backGround.AddComponent<UIRenderer>().Texture = ImageManager.Instance.LoadTexture("LoginBackGround");

            GameObject fieldObject = ObjectManager.Instance.RegisterObject();
            TextField loginField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, loginField, new SharpDX.Vector3(340, 268, 0f), null);

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField passwordField  = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, passwordField, new SharpDX.Vector3(340, 410, 0f), null);

            GameObject loginButton = ObjectManager.Instance.RegisterObject();
            loginButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("LoginButton"), 528f, 481f, 223, 58, 
                () => { OnLoginRequest(loginField.String, passwordField.String); });

            GameObject singUpButton = ObjectManager.Instance.RegisterObject();
            singUpButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("LoginSingUpButton"), 1057, 0, 223, 58,
                () => { SceneManager.Instance.ChanageScene("SingUpScene"); });

            ObjectManager.Instance.RegisterObject().AddComponent<LoginLauncher>();

        }
        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

        private void OnLoginRequest(string inid, string inpassword)
        {
            NetworkManager.Instance.SendLoginRequest(inid, inpassword);
        }

        private void CreateLoginTextBox(GameObject inObject, TextField inField, SharpDX.Vector3 v3, Action<string> _delgate)
        {
            inField.Texture = ImageManager.Instance.LoadTexture("LoginTextBox");
            inField.String = "Empty";
            inField.EnterBehavior = (string _inStr) => _delgate(_inStr);
            inObject.transform.position = v3;

        }

    }
}

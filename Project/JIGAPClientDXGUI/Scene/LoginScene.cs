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
            backGround.AddComponent<UIRenderer>().Texture = ResourceManager.Instance.LoadTexture("LoginBackGround");

            GameObject fieldObject = ObjectManager.Instance.RegisterObject();
            TextField loginField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, loginField, new SharpDX.Vector3(340, 268, 0f));

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField passwordField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, passwordField, new SharpDX.Vector3(340, 410, 0f));

            Button loginButton = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            loginButton.transform.position = new SharpDX.Vector3(528f, 481f, 0f);
            loginButton.SetButton(ResourceManager.Instance.LoadTexture("LoginButton"), 
                () => { OnLoginRequest(loginField.textComponent.str, passwordField.textComponent.str); });

            Button singUpButton = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            singUpButton.transform.position = new SharpDX.Vector3(1057, 0, 0f);
            singUpButton.SetButton(ResourceManager.Instance.LoadTexture("LoginSingUpButton"),
               () => { SceneManager.Instance.ChanageScene("SingUpScene"); });

            ObjectManager.Instance.RegisterObject().AddComponent<LoginLauncher>();

        }
        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

        private void OnLoginRequest(string inid, string inpassword)
        {
            NetworkManager.Instance.SendProcess.SendLoginRequest(inid, inpassword);
        }

        private void CreateLoginTextBox(GameObject inObject, TextField inField, SharpDX.Vector3 v3)
        {
            inField.texture = ResourceManager.Instance.LoadTexture("LoginTextBox");
            inField.textComponent.SetString("Empty");
            inObject.transform.position = v3;

        }

    }
}

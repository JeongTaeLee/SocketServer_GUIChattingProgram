using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JIGAPClientDXGUI
{
    class SingUpScene : Scene
    {
        public override void Init()
        {
            GameObject backGround = ObjectManager.Instance.RegisterObject();
            backGround.AddComponent<UIRenderer>().Texture = ResourceManager.Instance.LoadTexture("SingUpBackGround");

            GameObject fieldObject = ObjectManager.Instance.RegisterObject();
            TextField nameField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, nameField, new SharpDX.Vector3(340, 210, 0f));

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField idField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, idField, new SharpDX.Vector3(340, 380, 0f));

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField passwordField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, passwordField, new SharpDX.Vector3(340, 550, 0f));

            Button singUpButton = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            singUpButton.transform.position = new SharpDX.Vector3(528f, 650f, 0f);
            singUpButton.SetButton(ResourceManager.Instance.LoadTexture("SingUpButton"),
               () => { OnSingUpRequest(nameField.textComponent.str, idField.textComponent.str, passwordField.textComponent.str); });

            Button loginButton = ObjectManager.Instance.RegisterObject().AddComponent<Button>();
            loginButton.transform.position = new SharpDX.Vector3(1057f, 0f, 0f);
            loginButton.SetButton(ResourceManager.Instance.LoadTexture("SingUpLoginButton"),
              () => { SceneManager.Instance.ChanageScene("LoginScene"); });

            ObjectManager.Instance.RegisterObject().AddComponent<SingUpLauncher>();

        }
        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

        private void OnSingUpRequest(string inName, string inId, string inpassword)
        {
            NetworkManager.Instance.SendProcess.SendSingUpRequest(inName, inId, inpassword);
        }

        private void CreateLoginTextBox(GameObject inObject, TextField inField, SharpDX.Vector3 v3)
        {
            inField.texture = ResourceManager.Instance.LoadTexture("SingUpTextBox");
            inField.textComponent.SetString("");
            inObject.transform.position = v3;
        }

    }
}

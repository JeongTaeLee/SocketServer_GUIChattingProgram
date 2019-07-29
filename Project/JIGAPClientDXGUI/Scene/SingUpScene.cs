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
            backGround.AddComponent<UIRenderer>().Texture = ImageManager.Instance.LoadTexture("SingUpBackGround");

            GameObject fieldObject = ObjectManager.Instance.RegisterObject();
            TextField nameField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, nameField, new SharpDX.Vector3(340, 210, 0f));

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField idField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, idField, new SharpDX.Vector3(340, 380, 0f));

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField passwordField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, passwordField, new SharpDX.Vector3(340, 550, 0f));

            GameObject singUpButton = ObjectManager.Instance.RegisterObject();
            singUpButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("SingUpButton"), 528f, 641f, 223, 58,
                () => { OnSingUpRequest(nameField.String, idField.String, passwordField.String); });

            GameObject loginButton = ObjectManager.Instance.RegisterObject();
            loginButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("SingUpLoginButton"), 1057, 0, 223, 58,
                () => { SceneManager.Instance.ChanageScene("LoginScene"); });

            ObjectManager.Instance.RegisterObject().AddComponent<SingUpLauncher>();

        }
        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

        private void OnSingUpRequest(string inName, string inId, string inpassword)
        {
            NetworkManager.Instance.SendSingUpRequest(inName, inId, inpassword);
        }

        private void CreateLoginTextBox(GameObject inObject, TextField inField, SharpDX.Vector3 v3)
        {
            inField.Texture = ImageManager.Instance.LoadTexture("SingUpTextBox");
            inField.String = "Empty";
            inObject.transform.position = v3;
        }

    }
}

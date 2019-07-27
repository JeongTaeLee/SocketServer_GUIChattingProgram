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
            if (!NetworkManager.Instance.ConnectServer())
            {
                 System.Windows.Forms.Application.Exit();
                 return;
            }

            GameObject backGround = ObjectManager.Instance.RegisterObject();
            backGround.AddComponent<UIRenderer>().Texture = ImageManager.Instance.LoadTexture("SingUpBackGround");

            GameObject fieldObject = ObjectManager.Instance.RegisterObject();
            TextField nameField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, nameField, new SharpDX.Vector3(340, 210, 0f), null);

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField idField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, idField, new SharpDX.Vector3(340, 380, 0f), null);

            fieldObject = ObjectManager.Instance.RegisterObject();
            TextField passwordField = fieldObject.AddComponent<TextField>();
            CreateLoginTextBox(fieldObject, passwordField, new SharpDX.Vector3(340, 550, 0f), null);

            GameObject loginButton = ObjectManager.Instance.RegisterObject();
            loginButton.AddComponent<Button>().SetButton(ImageManager.Instance.LoadTexture("SingUpButton"), 528f, 641f, 223, 58,
                () => { OnSingUpRequest(nameField.String, idField.String, passwordField.String); });

            ObjectManager.Instance.RegisterObject().AddComponent<LoginLauncher>();

        }
        public override void Release()
        {
            ObjectManager.Instance.ClearObjects();
        }

        private void OnSingUpRequest(string inName, string inId, string inpassword)
        {
            NetworkManager.Instance.SendSingUpRequest(inName, inId, inpassword);
        }

        private void CreateLoginTextBox(GameObject inObject, TextField inField, SharpDX.Vector3 v3, Action<string> _delgate)
        {
            inField.Texture = ImageManager.Instance.LoadTexture("SingUpTextBox");
            inField.String = "Empty";
            inField.EnterBehavior = (string _inStr) => _delgate(_inStr);
            inObject.transform.position = v3;
        }

    }
}

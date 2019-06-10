using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JIGAPClientDXGUI.Engine;

namespace JIGAPClientDXGUI
{
    class LoginLauncher : NetworkObject
    {
        private InputLine inputLine = null;

        public override void Init()
        {
            //field = ObjectManager.GetInst().AddObject<LoginField>() as LoginField;
            //field.TextBoxInputLine.OnEnterCallBack = RequestLogin;

            inputLine = ObjectManager.GetInst().AddObject<InputLine>();
            inputLine.transform.position = new SharpDX.Vector3(527.5f, 595f, 0f);
            inputLine.FontColor = SharpDX.Color.Black;
            inputLine.FontSize = 80;
            inputLine.SetCenterText();
            inputLine.OnEnterCallBack = RequestLogin;

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("LoginButton", "./Image/LoginButton.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 250, 66),
                ButtonPos = new SharpDX.Vector3(1056f, 593f, 0f),
                ButtonEventCallBack = RequestLogin
            });
        }

        public void RequestLogin()
        {
            if (inputLine.InputText.Length > 0)
            {
                NetworkManager.GetInst().RequestLogin(inputLine.InputText.ToString());
                NetworkManager.GetInst().MyName = inputLine.InputText.ToString();
            }
        }

        public override void OnLoginSuccess(object sender, EventArgs e)
        {
            base.OnLoginSuccess(sender, e);
            SceneManager.GetInst().ChanageScene("LobbyScene");
        }
        public override void OnLoginFailed(object sender, EventArgs e)
        {
            base.OnLoginFailed(sender, e);
            System.Windows.Forms.MessageBox.Show("로그인에 실패했습니다. 닉네임이 중복됩니다.");
        }
    }
}

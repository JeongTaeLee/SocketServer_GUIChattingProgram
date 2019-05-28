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
        private LoginField field = null;

        public override void Init()
        {
            field = ObjectManager.GetInst().AddObject<LoginField>() as LoginField;
            field.TextBoxInputLine.OnEnterCallBack = RequestLogin;

            ObjectManager.GetInst().AddObject(new Button
            {
                ButtonTexture = ImageManager.GetInst().LoadTexture("LoginButton", "./Image/LoginButton.png"),
                ButtonRange = new System.Drawing.Rectangle(0, 0, 250, 66),
                ButtonPos = new SharpDX.Vector3(DXManager.GetInst().Width / 2f - 125f, DXManager.GetInst().Height / 2f - 33f + 100, 0f),
                ButtonEventCallBack = RequestLogin
            });
        }

        public void RequestLogin()
        {
            if (field.TextBoxInputLine.InputText.Length > 0)
            {
                NetworkManager.GetInst().RequestLogin(field.TextBoxInputLine.InputText.ToString());
                NetworkManager.GetInst().MyName = field.TextBoxInputLine.InputText.ToString();
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

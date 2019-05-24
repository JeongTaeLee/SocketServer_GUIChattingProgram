namespace JIGAPClientGUI
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.ipAddrTextBox = new System.Windows.Forms.TextBox();
            this.portAddrTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.nickNameTextbox = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.messageLogTextBox = new System.Windows.Forms.TextBox();
            this.sendingTextBox = new System.Windows.Forms.TextBox();
            this.sendingButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(23, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "IP 주소";
            // 
            // ipAddrTextBox
            // 
            this.ipAddrTextBox.Location = new System.Drawing.Point(72, 6);
            this.ipAddrTextBox.Name = "ipAddrTextBox";
            this.ipAddrTextBox.Size = new System.Drawing.Size(209, 21);
            this.ipAddrTextBox.TabIndex = 1;
            // 
            // portAddrTextBox
            // 
            this.portAddrTextBox.Location = new System.Drawing.Point(72, 33);
            this.portAddrTextBox.Name = "portAddrTextBox";
            this.portAddrTextBox.Size = new System.Drawing.Size(209, 21);
            this.portAddrTextBox.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 36);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Port 주소";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(23, 63);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "닉네임";
            // 
            // nickNameTextbox
            // 
            this.nickNameTextbox.Location = new System.Drawing.Point(72, 60);
            this.nickNameTextbox.Name = "nickNameTextbox";
            this.nickNameTextbox.Size = new System.Drawing.Size(209, 21);
            this.nickNameTextbox.TabIndex = 5;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(287, 5);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(64, 76);
            this.button1.TabIndex = 6;
            this.button1.Text = "연결";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.Button1_Click);
            // 
            // messageLogTextBox
            // 
            this.messageLogTextBox.Location = new System.Drawing.Point(4, 87);
            this.messageLogTextBox.Multiline = true;
            this.messageLogTextBox.Name = "messageLogTextBox";
            this.messageLogTextBox.ReadOnly = true;
            this.messageLogTextBox.Size = new System.Drawing.Size(346, 294);
            this.messageLogTextBox.TabIndex = 7;
            // 
            // sendingTextBox
            // 
            this.sendingTextBox.Location = new System.Drawing.Point(4, 394);
            this.sendingTextBox.Name = "sendingTextBox";
            this.sendingTextBox.Size = new System.Drawing.Size(277, 21);
            this.sendingTextBox.TabIndex = 8;
            this.sendingTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.SendingTextBox_KeyDown);
            // 
            // sendingButton
            // 
            this.sendingButton.Location = new System.Drawing.Point(287, 392);
            this.sendingButton.Name = "sendingButton";
            this.sendingButton.Size = new System.Drawing.Size(64, 23);
            this.sendingButton.TabIndex = 9;
            this.sendingButton.Text = "전송";
            this.sendingButton.UseVisualStyleBackColor = true;
            this.sendingButton.Click += new System.EventHandler(this.SendingButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(354, 427);
            this.Controls.Add(this.sendingButton);
            this.Controls.Add(this.sendingTextBox);
            this.Controls.Add(this.messageLogTextBox);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.nickNameTextbox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.portAddrTextBox);
            this.Controls.Add(this.ipAddrTextBox);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "JIGAPClient";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox ipAddrTextBox;
        private System.Windows.Forms.TextBox portAddrTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox nickNameTextbox;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox messageLogTextBox;
        private System.Windows.Forms.TextBox sendingTextBox;
        private System.Windows.Forms.Button sendingButton;
    }
}


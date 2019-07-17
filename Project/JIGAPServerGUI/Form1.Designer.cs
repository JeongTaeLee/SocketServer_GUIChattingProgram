namespace JIGAPServerGUI
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
            this.IpLabel = new System.Windows.Forms.Label();
            this.PortLabel = new System.Windows.Forms.Label();
            this.IPTextBox = new System.Windows.Forms.TextBox();
            this.PortTextBox = new System.Windows.Forms.TextBox();
            this.ServerOpenButton = new System.Windows.Forms.Button();
            this.systemLogTextBox = new System.Windows.Forms.TextBox();
            this.serverCloseButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // IpLabel
            // 
            this.IpLabel.AutoSize = true;
            this.IpLabel.Location = new System.Drawing.Point(12, 9);
            this.IpLabel.Name = "IpLabel";
            this.IpLabel.Size = new System.Drawing.Size(44, 12);
            this.IpLabel.TabIndex = 0;
            this.IpLabel.Text = "IP 주소";
            // 
            // PortLabel
            // 
            this.PortLabel.AutoSize = true;
            this.PortLabel.Location = new System.Drawing.Point(12, 34);
            this.PortLabel.Name = "PortLabel";
            this.PortLabel.Size = new System.Drawing.Size(55, 12);
            this.PortLabel.TabIndex = 1;
            this.PortLabel.Text = "Port 주소";
            // 
            // IPTextBox
            // 
            this.IPTextBox.Location = new System.Drawing.Point(79, 6);
            this.IPTextBox.Name = "IPTextBox";
            this.IPTextBox.Size = new System.Drawing.Size(151, 21);
            this.IPTextBox.TabIndex = 2;
            // 
            // PortTextBox
            // 
            this.PortTextBox.Location = new System.Drawing.Point(79, 34);
            this.PortTextBox.Name = "PortTextBox";
            this.PortTextBox.Size = new System.Drawing.Size(151, 21);
            this.PortTextBox.TabIndex = 3;
            // 
            // ServerOpenButton
            // 
            this.ServerOpenButton.Location = new System.Drawing.Point(236, 6);
            this.ServerOpenButton.Name = "ServerOpenButton";
            this.ServerOpenButton.Size = new System.Drawing.Size(79, 21);
            this.ServerOpenButton.TabIndex = 4;
            this.ServerOpenButton.Text = "서버 열기";
            this.ServerOpenButton.UseVisualStyleBackColor = true;
            // 
            // systemLogTextBox
            // 
            this.systemLogTextBox.Location = new System.Drawing.Point(14, 70);
            this.systemLogTextBox.Multiline = true;
            this.systemLogTextBox.Name = "systemLogTextBox";
            this.systemLogTextBox.ReadOnly = true;
            this.systemLogTextBox.Size = new System.Drawing.Size(301, 368);
            this.systemLogTextBox.TabIndex = 5;
            // 
            // serverCloseButton
            // 
            this.serverCloseButton.Location = new System.Drawing.Point(236, 32);
            this.serverCloseButton.Name = "serverCloseButton";
            this.serverCloseButton.Size = new System.Drawing.Size(79, 23);
            this.serverCloseButton.TabIndex = 6;
            this.serverCloseButton.Text = "서버 닫기";
            this.serverCloseButton.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(327, 450);
            this.Controls.Add(this.serverCloseButton);
            this.Controls.Add(this.systemLogTextBox);
            this.Controls.Add(this.ServerOpenButton);
            this.Controls.Add(this.PortTextBox);
            this.Controls.Add(this.IPTextBox);
            this.Controls.Add(this.PortLabel);
            this.Controls.Add(this.IpLabel);
            this.Name = "Form1";
            this.Text = "JIGAPServer";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label IpLabel;
        private System.Windows.Forms.Label PortLabel;
        private System.Windows.Forms.TextBox IPTextBox;
        private System.Windows.Forms.TextBox PortTextBox;
        private System.Windows.Forms.Button ServerOpenButton;
        private System.Windows.Forms.TextBox systemLogTextBox;
        private System.Windows.Forms.Button serverCloseButton;
    }
}


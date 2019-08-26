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
            this.label2 = new System.Windows.Forms.Label();
            this.PortTextBox = new System.Windows.Forms.TextBox();
            this.ServerSwitch = new System.Windows.Forms.Button();
            this.LogTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "PORT";
            // 
            // PortTextBox
            // 
            this.PortTextBox.Location = new System.Drawing.Point(69, 12);
            this.PortTextBox.Name = "PortTextBox";
            this.PortTextBox.Size = new System.Drawing.Size(356, 21);
            this.PortTextBox.TabIndex = 3;
            this.PortTextBox.Text = "9199";
            // 
            // ServerSwitch
            // 
            this.ServerSwitch.Location = new System.Drawing.Point(431, 12);
            this.ServerSwitch.Name = "ServerSwitch";
            this.ServerSwitch.Size = new System.Drawing.Size(307, 23);
            this.ServerSwitch.TabIndex = 4;
            this.ServerSwitch.Text = "Open";
            this.ServerSwitch.UseVisualStyleBackColor = true;
            this.ServerSwitch.Click += new System.EventHandler(this.ServerSwitch_Click);
            // 
            // LogTextBox
            // 
            this.LogTextBox.BackColor = System.Drawing.SystemColors.WindowText;
            this.LogTextBox.Font = new System.Drawing.Font("굴림", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LogTextBox.ForeColor = System.Drawing.SystemColors.Window;
            this.LogTextBox.Location = new System.Drawing.Point(12, 39);
            this.LogTextBox.Multiline = true;
            this.LogTextBox.Name = "LogTextBox";
            this.LogTextBox.Size = new System.Drawing.Size(1007, 540);
            this.LogTextBox.TabIndex = 5;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1031, 591);
            this.Controls.Add(this.LogTextBox);
            this.Controls.Add(this.ServerSwitch);
            this.Controls.Add(this.PortTextBox);
            this.Controls.Add(this.label2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox PortTextBox;
        private System.Windows.Forms.Button ServerSwitch;
        private System.Windows.Forms.TextBox LogTextBox;
    }
}

